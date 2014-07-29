#include <QtGui>
#include <default_gui_model.h>

class ClampProtocol : public DefaultGUIModel {
	Q_OBJECT

	public:
		ClampProtocol(void);
		~ClampProtocol(void);

		void initParameters(void);
		void customizeGUI(void);
		void execute(void);
//		void refresh(void);

	protected:
		virtual void update(DefaultGUIModel::update_flags_t);

	private:
		QPushButton *loadButton, *editorButton, *viewerButton;
		QLineEdit *loadFilePath;

	public slots:
};
