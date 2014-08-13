#include <cmath>
#include <iostream>
#include <QtGui>
//#include <debug.h>
//#include <main_window.h>
#include <qwt_legend.h>
#include "clamp-protocol.h"

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
			break;

		case PAUSE:
			break;

		case UNPAUSE:
			break;

		case PERIOD:
			break;

		default:
			break;
	}
}

void ClampProtocol::execute(void) {

}

void ClampProtocol::customizeGUI(void) {
	QGridLayout *customLayout = DefaultGUIModel::getLayout();

	QGroupBox *controlGroup = new QGroupBox("Controls");
	QVBoxLayout *controlGroupLayout = new QVBoxLayout;
	controlGroup->setLayout(controlGroupLayout);

	QHBoxLayout *loadRow = new QHBoxLayout;
	loadButton = new QPushButton("Load");
	loadFilePath = new QLineEdit;
	loadFilePath->setReadOnly(true);
	loadRow->addWidget(loadButton);
	loadRow->addWidget(loadFilePath);
	controlGroupLayout->addLayout(loadRow);

	QHBoxLayout *toolsRow = new QHBoxLayout;
	editorButton = new QPushButton("Editor");
	editorButton->setCheckable(true);
	viewerButton = new QPushButton("Viewer");
	viewerButton->setCheckable(true);
	toolsRow->addWidget(editorButton);
	toolsRow->addWidget(viewerButton);
	controlGroupLayout->addLayout(toolsRow);

	customLayout->addWidget(controlGroup, 0, 0);
	setLayout(customLayout);

	QObject::connect(editorButton, SIGNAL(clicked(void)), this, SLOT(openProtocolEditor(void)));
	QObject::connect(viewerButton, SIGNAL(clicked(void)), this, SLOT(openProtocolViewer(void)));
}

void ClampProtocol::openProtocolEditor(void) {
	ClampProtocolEditor();
}

void ClampProtocol::openProtocolViewer(void) {

}
