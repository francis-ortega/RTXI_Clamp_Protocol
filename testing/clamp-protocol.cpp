#include <cmath>
#include <iostream>
#include <qwt_legend.h>
#include "clamp-protocol.h"
#include "clamp-protocol-editor.h"

extern "C" Plugin::Object *createRTXIPlugin(void) {
	return new ClampProtocol();
}

static DefaultGUIModel::variable_t vars[] = {
	{ "Current Input", "A", DefaultGUIModel::INPUT, },
	{ "Voltage Output", "V w/ LJP", DefaultGUIModel::OUTPUT, }, 
	{ "Protocol Name", "", DefaultGUIModel::COMMENT, },
	{ "Trial", "", DefaultGUIModel::STATE, },
	{ "Segment", "", DefaultGUIModel::STATE, }, 
	{ "Sweep", "", DefaultGUIModel::STATE, },
	{ "Time", "ms", DefaultGUIModel::STATE, },
	{ "Voltage Output", "V w/ LJP", DefaultGUIModel::STATE, },
	{ "Interval Time", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
	{ "Number of Trials", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::INTEGER, }, 
	{ "Liquid Junction Potential", "mV", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

ClampProtocol::ClampProtocol(void) : DefaultGUIModel("Clamp Protocol", ::vars, ::num_vars ) {
	setWhatsThis("I'll get to this later");
	DefaultGUIModel::createGUI(vars, num_vars);
	initParameters();
	customizeGUI();
	update( INIT );
}

ClampProtocol::~ClampProtocol(void) {};

void ClampProtocol::initParameters(void) {

}

void ClampProtocol::update(DefaultGUIModel::update_flags_t flag) {
	switch(flag) {
		case INIT:
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
				
				if (stepTime > stepEndTime) {
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
				}
			}

			if (protocolMode == WAIT) {
				if ( ((RT::OS::getTime() * 1e-6) - protocolEndTime) > intervalTime ) {
					time = 0;
					segmentIdx = 0;
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

//	setComment("Protocol Name", fileName.section('/', -1));//, QString::SectionSkipEmpty));
	setComment("Protocol Name", fileName);
}

void ClampProtocol::openProtocolEditor(void) {
	ClampProtocolEditor(this);
}

void ClampProtocol::openProtocolViewer(void) {

}
