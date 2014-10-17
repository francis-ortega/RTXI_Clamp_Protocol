#ifndef CP_PROTOCOL_EDITOR_H
#define CP_PROTOCOL_EDITOR_H

#include <QtGui>
#include "protocol.h"

// No function to change alignment in Qt3.3, so subclassing must be done
class CenterAlignTableItem : public QTableWidgetItem {
	public:
		CenterAlignTableItem( QTableWidget * /*, EditType*/ );
		int alignment() const;
};


//class ClampProtocolEditor:public QDialog { // QWidget dialog, inherits Qt Designer designed GUI
class ClampProtocolEditor : public QWidget {
	Q_OBJECT

	public:
		ClampProtocolEditor( QWidget * );
		~ClampProtocolEditor( void ) { };
		Protocol protocol; // Clamp protocol
		void createGUI(void);

		QPushButton *saveProtocolButton, *loadProtocolButton, *exportProtocolButton, *previewProtocolButton, *clearProtocolButton;
		QGroupBox *protocolDescriptionBox;
		QLabel *segmentStepLabel;
		QTableWidget *protocolTable;
		QPushButton *addStepButton, *insertStepButton, *deleteStepButton;
		QGroupBox *segmentSummaryGroup, *segmentSweepGroup;
		QLabel *segmentSweepLabel;
		QSpinBox *segmentSweepSpinBox;
		QTreeWidget *segmentListWidget;
		QPushButton *addSegmentButton, *deleteSegmentButton;

		QMdiSubWindow *subWindow;
	
	private:
		int currentSegmentNumber;
		QStringList ampModeList, stepTypeList;
		void createStep( int );
		int loadFileToProtocol( QString );
		bool protocolEmpty( void );

	protected:
		QHBoxLayout *layout1, *layout2, *layout4, *segmentSweepGroupLayout, *layout6;
		QVBoxLayout *windowLayout, *layout3, *protocolDescriptionBoxLayout, *layout5, *segmentSummaryGroupLayout;
			
	signals:
		void protocolTableScroll( void );
	
	public slots:
		QString loadProtocol( void );
		void loadProtocol( QString );
		void clearProtocol( void );
		void exportProtocol( void );
		void previewProtocol( void );
		virtual void protocolTable_currentChanged(int, int);
		virtual void protocolTable_verticalSliderReleased();
	
	private slots:
		void addSegment( void );
		void deleteSegment( void );
		void addStep( void );
		void insertStep( void );
		void deleteStep( void );
		void updateSegment( QListWidgetItem* );
		void updateSegmentSweeps( int );
		void updateTableLabel( void );
		void updateTable( void );
		void updateStepAttribute( int, int );
		void updateStepType( int, ProtocolStep::stepType_t );
		void saveProtocol( void );
};

#endif // CP_protocol_editor.h
