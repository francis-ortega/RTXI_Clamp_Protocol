#include <QtGui>
//#include <string>
#include <default_gui_model.h>
#include <clamp-protocol-editor.h>
#include "protocol.h"

class ClampProtocol : public DefaultGUIModel {

	Q_OBJECT

	public:
/*		
		typedef boost::shared_ptr<ProtocolStep> Step;
		typedef std::vector<Step> SegmentContainer;
		typedef std::vector<Step>::iterator SegmentContainerIt;
		typedef boost::shared_ptr<ProtocolSegment> Segment;
		typedef std::vector<Segment> ProtocolContainer;
		typedef std::vector<Segment>::iterator ProtocolContainerIt;
*/

		ClampProtocol(void);
		~ClampProtocol(void);

		void initParameters(void);
		void customizeGUI(void);
		void execute(void);
//		void refresh(void);

	protected:
		virtual void update(DefaultGUIModel::update_flags_t);

	private:
		QString protocol_file;
		double period;
		double voltage, junctionPotential;
		double trial, time, sweep, segmentNumber, intervalTime;
		int numTrials;

		Protocol protocol;
		enum executeMode_t { IDLE, PROTOCOL } executeMode;
		enum protocolMode_t { SEGMENT, STEP, EXECUTE, END, WAIT } protocolMode;
		Step step;
		ProtocolStep::stepType_t stepType;
		int segmentIdx;
		int sweepIdx;
		int stepIdx;
		int trialIdx;
		int numSweeps;
		int numSteps;
		int stepTime, stepEndTime;
		double protocolEndTime;
		double stepOutput;
		double outputFactor, inputFactor;
		double rampIncrement;
		double pulseWidth;
		int pulseRate;
/**/		std::vector<double> data;
		
		bool recordData;
		bool recording;
		bool plotting;

		QPushButton *loadButton, *editorButton, *viewerButton;
		QLineEdit *loadFilePath;

	public slots:
		void loadProtocolFile(void);
		void openProtocolEditor(void);
		void openProtocolViewer(void);
};
