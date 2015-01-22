#include <cmath>
#include <iostream>
#include <main_window.h>
#include <qwt_legend.h>
#include "clamp-protocol.h"

extern "C" Plugin::Object *createRTXIPlugin(void) {
	return new ClampProtocol();
}

static DefaultGUIModel::variable_t vars[] = {
	{ "Current In (A)", "A", DefaultGUIModel::INPUT, },
	{ "Voltage Out (V w/ LJP)", "V w/ LJP", DefaultGUIModel::OUTPUT, }, 
	{ "Protocol Name", "Name of loaded protocol", DefaultGUIModel::COMMENT, },
	{ "Trial", "Number of current trial", DefaultGUIModel::STATE, },
	{ "Segment", "Current segment number", DefaultGUIModel::STATE, }, 
	{ "Sweep", "Sweep number in current segment", DefaultGUIModel::STATE, },
	{ "Time (ms)", "Elapsed time for current trial", DefaultGUIModel::STATE, },
	{ "Voltage Out (V w/ LJP)", "V w/ LJP", DefaultGUIModel::STATE, },
	{ "Interval Time", "Time allocated between intervals", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
	{ "Number of Trials", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::INTEGER, }, 
	{ "Liquid Junct. Potential (mV)", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

ClampProtocol::ClampProtocol(void) : DefaultGUIModel("Clamp Protocol", ::vars, ::num_vars ) {
	setWhatsThis("I'll get to this later");
	DefaultGUIModel::createGUI(vars, num_vars);
	initParameters();
	customizeGUI();
	update( INIT );
	refresh();
}

ClampProtocol::~ClampProtocol(void) {};

void ClampProtocol::initParameters(void) {
   time = 0;
   trial = 1;
	segmentNumber = 1;
	sweep = 1;
	voltage = 0;
	intervalTime = 1000;
	numTrials = 1;
   junctionPotential = 0;
   executeMode = IDLE;
   segmentIdx = 0;
   sweepIdx = 0;
   stepIdx = 0;
   trialIdx = 0;
//	fifo = 10 * 1048576;

   recordData = false;
   recording = false;
   plotting = false;
}

void ClampProtocol::update(DefaultGUIModel::update_flags_t flag) {
	switch(flag) {
		case INIT:
			period = RT::System::getInstance()->getPeriod() * 1e-9;
			setComment("Protocol Name", QString::fromStdString("none"));
			setParameter("Interval Time", intervalTime);
			setParameter("Number of Trials", numTrials);
			setParameter("Liquid Junct. Potential (mV)", voltage);
			setState("Trial", trial);
			setState("Segment", segmentNumber);
			setState("Sweep", sweep);
			setState("Time (ms)", time);
			setState("Voltage Out (V w/ LJP)", voltage);
			break;
			
		case MODIFY:
//			fileName = getComment("Protocol Name").toStdString();
			intervalTime = getParameter("Interval Time").toDouble();
			numTrials = getParameter("Number of Trials").toInt();
			voltage = getParameter("Liquid Junction Potential (mv)").toDouble();
			break;

		case PAUSE:
			break;

		case UNPAUSE:
			break;

		case PERIOD:
			period = RT::System::getInstance()->getPeriod()*1e-6; //Grabs RTXI thread period and converts to ms (from ns)
			break;

		default:
			break;
	}
}

void ClampProtocol::execute(void) {
	switch (executeMode) {
		case IDLE:
			break;

		case PROTOCOL:

			if( protocolMode == END ) { // End of protocol

				if( recordData && recording ) { // Data record checkbox is ticked and data recorder is on
					 // Stop data recorder
					 Event::Object event(Event::STOP_RECORDING_EVENT);
					 Event::Manager::getInstance()->postEventRT(&event);
				}

				if( trialIdx < ( numTrials - 1 ) ) { // Restart protocol if additional trials are needed
					 trialIdx++; // Advance trial
					 trial++;
					 protocolEndTime = RT::OS::getTime() * 1e-6; // Time at end of protocol (ms)
					 protocolMode = WAIT; // Wait for interval time to be finished
				} else { // All trials finished
					 executeMode = IDLE;
				}
			} // end ( protocolMode == END )
		
			if (protocolMode == SEGMENT) {
				numSweeps = protocol.numSweeps(segmentIdx);
				numSteps = protocol.numSteps(segmentIdx);
				protocolMode = STEP;
			}

			if (protocolMode == STEP) {
				step = protocol.getStep( segmentIdx, stepIdx );
				stepType = step->stepType;
				stepTime = 0;

				stepEndTime = ((step->stepDuration + (step->deltaStepDuration * sweepIdx)) / period) - 1;
				stepOutput = step->holdingLevel1 + (step->deltaHoldingLevel1 * sweepIdx);

				if (stepType == ProtocolStep::RAMP) {
					double h2 = step->holdingLevel2 + (step->deltaHoldingLevel2 * sweepIdx);
					rampIncrement = (h2 - stepOutput) / stepEndTime;
				} else if (stepType == ProtocolStep::TRAIN) {
					pulseWidth = step->pulseWidth / period;
					pulseRate = step->pulseRate / (period * 1000);
				}

				outputFactor = 1e-3;
				inputFactor = 1e9;

//				if (plotting) stepStart = time / period;

				protocolMode = EXECUTE;
			}

			if (protocolMode == EXECUTE) {
				switch (stepType) {
					case ProtocolStep::STEP:
						voltage = stepOutput;
						output(0) = (voltage + junctionPotential) * outputFactor;
						break;

					case ProtocolStep::RAMP:
						voltage = stepOutput + (stepTime * rampIncrement);
						output(0) = (voltage + junctionPotential) * outputFactor;
						break;

					case ProtocolStep::TRAIN:
						if (stepTime % pulseRate < pulseWidth) {
							voltage = stepOutput;
							output(0) = (voltage + junctionPotential) * outputFactor;
						} else {
							voltage = 0;
							output(0) = (voltage + junctionPotential) * outputFactor;
						}
						break;

					default:
						std::cout << "ERROR - In function ClampProtocol::execute() switch( stepType ) default case called" << std::endl;
						break;
				}

				stepTime++;

//				if( plotting ) data.push_back( input(0) * inputFactor);
				
				if (stepTime > stepEndTime) {

/*
					if (plotting) {
						int stepStartSweep = 0;

						for (int i = 0; i < segmentIdx; i++) 
							stepStartSweep += protocol.segmentLength(segmentIdx-1, period, false);
						for (int i = 0; i < stepIdx; i++)
							stepStartSweep += protocol.getStep(segmentIdx, i)->stepDuration/period;

						token.trial = trialIdx;
						token.sweep = sweepIdx;
						token.stepStartSweep = stepStartSweep;
						token.stepStart = stepStart - 1;

						token.period = period;
						token.points = data.size();
						toek.laststep = false;
					}
*/
					stepIdx++;
					protocolMode = STEP;

					if (stepIdx == numSteps) {
						sweepIdx++;
						sweep++;
						stepIdx = 0;

						if (sweepIdx == numSweeps) {
							segmentIdx++;
							segmentNumber++;
							sweepIdx = 0;
							sweep = 1;

							protocolMode = SEGMENT;

							if (segmentIdx >= protocol.numSegments()) {
								protocolMode = END;
							}
						}
					}

/*
					if (plotting) {
						fifo.write( &token, sizeof(token) );
						fifo.write( &data[0], token.points * sizeof(double) );

						data.clear();

						data.push_back( input(0) * inputFactor );
					}
*/
				}
			}

			if (protocolMode == WAIT) {
				if ( ((RT::OS::getTime() * 1e-6) - protocolEndTime) > intervalTime ) {
					time = 0;
					segmentIdx = 0;
/*
					if (recordData && !recording) {
						Event::Object event(Event::START_RECORDING_EVENT);
						Event::Manager::getInstance()->postEventRT(&event);
					}
*/
					protocolMode = SEGMENT;
					executeMode = PROTOCOL;
				}
				return;
			}

			time += period;
			break;

		default:
			break;
	}
}

void ClampProtocol::customizeGUI(void) {
	QGridLayout *customLayout = DefaultGUIModel::getLayout();

	QGroupBox *controlGroup = new QGroupBox("Controls");
	QVBoxLayout *controlGroupLayout = new QVBoxLayout;
	controlGroup->setLayout(controlGroupLayout);

	QHBoxLayout *toolsRow = new QHBoxLayout;
	loadButton = new QPushButton("Load");
	editorButton = new QPushButton("Editor");
//	editorButton->setCheckable(true);
	viewerButton = new QPushButton("Viewer");
//	viewerButton->setCheckable(true);
	toolsRow->addWidget(loadButton);
	toolsRow->addWidget(editorButton);
	toolsRow->addWidget(viewerButton);
	controlGroupLayout->addLayout(toolsRow);

//	QHBoxLayout *loadRow = new QHBoxLayout;
//	loadButton = new QPushButton("Load");
//	loadFilePath = new QLineEdit;
//	loadFilePath->setReadOnly(true);
//	loadRow->addWidget(loadButton);
//	loadRow->addWidget(loadFilePath);
//	controlGroupLayout->addLayout(loadRow);

	customLayout->addWidget(controlGroup, 0, 0);
	setLayout(customLayout);

	QObject::connect(loadButton, SIGNAL(clicked(void)), this, SLOT(loadProtocolFile(void)));
	QObject::connect(editorButton, SIGNAL(clicked(void)), this, SLOT(openProtocolEditor(void)));
	QObject::connect(viewerButton, SIGNAL(clicked(void)), this, SLOT(openProtocolViewer(void)));
//	QObject::connect(runProtocolButton, SIGNAL(clicked(void)), this, SLOT(toggleProtocol));
}

void ClampProtocol::loadProtocolFile(void) {
	QString fileName = QFileDialog::getOpenFileName(this, "Open a Protocol File", "~/", "Clamp Protocol Files (*.csp)");

	if (fileName == NULL) return;

	QDomDocument doc("protocol");
	QFile file( fileName );

	if (!file.open( QIODevice::ReadOnly ) ){
		QMessageBox::warning(this, "Error", "Unable to open file");
		return;
	}
	if (!doc.setContent( &file )) {
		QMessageBox::warning(this, "Error", "Unable to set file contents to document" );
		file.close();
		return;
	}
	file.close();

	protocol.fromDoc(doc);

	if(protocol.numSegments() <= 0) {
		QMessageBox::warning(this, "Error", "Protocol did not contain any segments");
	}

	setComment("Protocol Name", fileName);
}

void ClampProtocol::openProtocolEditor(void) {
	ClampProtocolEditor *protocolEditor = new ClampProtocolEditor(MainWindow::getInstance()->centralWidget());
	protocolEditor->show();
}

void ClampProtocol::openProtocolViewer(void) {
	ClampProtocolWindow *plotWindow = new ClampProtocolWindow(MainWindow::getInstance()->centralWidget());
	plotWindow->show();
}
