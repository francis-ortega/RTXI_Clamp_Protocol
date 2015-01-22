#include "clamp-protocol-window.h"
#include "clamp-protocol.h"

#include <QtGui>
#include <qwt_text.h>
#include <qwt_legend.h>
#include <basicplot.h>

//ClampProtocolWindow::ClampProtocolWindow( QWidget *parent, Panel *p ): ClampProtocolWindowUI( parent, "Plot Window", Qt::WDestructiveClose ), panel( p ), overlaySweeps( false ), plotAfter( false ), colorScheme( 0 ),  runCounter( 0 ), sweepsShown( 0 ) {
ClampProtocolWindow::ClampProtocolWindow( QWidget *parent ) : QWidget( parent ) {
	setWindowTitle("Protocol Viewer");

	overlaySweeps = false;
	plotAfter = false;
	colorScheme = 0;
	runCounter = 0;
	sweepsShown = 0;

	createGUI();
}

ClampProtocolWindow::~ClampProtocolWindow( void ) {
//	panel->removeClampProtocolWindow( this );
}

void ClampProtocolWindow::createGUI( void ) {
	QWidget::setAttribute(Qt::WA_DeleteOnClose);

	subWindow = new QMdiSubWindow;
	subWindow->setWindowIcon(QIcon("/usr/local/lib/rtxi/RTXI-widget-icon.png"));
	subWindow->setWindowTitle("Protocol Viewer");
	MainWindow::getInstance()->createMdi(subWindow);

	QVBoxLayout *plotWindowUILayout = new QVBoxLayout( this );
	frame = new QFrame;
	frameLayout = new QHBoxLayout;
	plotWindowUILayout->addLayout(frameLayout);

// Make the top of the GUI
	layout1 = new QGridLayout;
	currentScaleLabel = new QLabel("Current");
	currentScaleEdit = new QComboBox;
	currentScaleEdit->addItem( trUtf8( "\xce\xbc\x41" ) );
	currentScaleEdit->addItem( tr( "nA" ) );
	currentScaleEdit->addItem( tr( "pA" ) );
	currentScaleEdit->setCurrentIndex( 1 );
	currentY1Edit = new QSpinBox;
	currentY1Edit->setMaximum( 99999 );
	currentY1Edit->setMinimum( -99999 );
	currentY1Edit->setValue(-20);
	currentY2Edit = new QSpinBox;
	currentY2Edit->setMaximum( 99999 );
	currentY2Edit->setMinimum( -99999 );
	currentY2Edit->setValue(0);
	layout1->addWidget(currentScaleLabel, 1, 0, 1, 1);
	layout1->addWidget(currentY1Edit, 1, 1, 1, 1);
	layout1->addWidget(currentY2Edit, 1, 2, 1, 1);
	layout1->addWidget(currentScaleEdit, 1, 3, 1, 1);

	timeScaleLabel = new QLabel("Time");
	timeScaleEdit = new QComboBox;
	timeScaleEdit->addItem( tr( "s" ) );
	timeScaleEdit->addItem( tr( "ms" ) );
	timeScaleEdit->addItem( trUtf8( "\xce\xbc\x73" ) );
	timeScaleEdit->addItem( tr( "ns" ) );
	timeScaleEdit->setCurrentIndex( 1 );
	timeX1Edit = new QSpinBox;
	timeX1Edit->setMaximum( 99999 );
	timeX1Edit->setValue(0);
	timeX2Edit = new QSpinBox;
	timeX2Edit->setMaximum( 99999 );
	timeX2Edit->setValue(1000);
	layout1->addWidget(timeScaleLabel, 0, 0, 1, 1);
	layout1->addWidget(timeX1Edit, 0, 1, 1, 1);
	layout1->addWidget(timeX2Edit, 0, 2, 1, 1);
	layout1->addWidget(timeScaleEdit, 0, 3, 1, 1);

	frameLayout->addLayout(layout1);

	setAxesButton = new QPushButton("Set Axes");
	setAxesButton->setEnabled(true);
	frameLayout->addWidget(setAxesButton);

	layout2 = new QVBoxLayout;
	overlaySweepsCheckBox = new QCheckBox("Overlay Sweeps");
	layout2->addWidget(overlaySweepsCheckBox);
	plotAfterCheckBox = new QCheckBox("Plot after Protocol");
	layout2->addWidget(plotAfterCheckBox);
	frameLayout->addLayout(layout2);

	layout3 = new QVBoxLayout;
	textLabel1 = new QLabel("Color by:");
	colorByComboBox = new QComboBox;
	colorByComboBox->addItem( tr( "Run" ) );
	colorByComboBox->addItem( tr( "Trial" ) );
	colorByComboBox->addItem( tr( "Sweep" ) );
	layout3->addWidget(textLabel1);
	layout3->addWidget(colorByComboBox);
	frameLayout->addLayout(layout3);

	clearButton = new QPushButton("Clear");
	frameLayout->addWidget(clearButton);

// And now the plot on the bottom...
	plot = new BasicPlot( this );

	// Add scrollview for top part of widget to allow for smaller widths
//	QScrollView *sv = new QScrollView( this );
//	sv->addChild( frame ); // UI contains a frame not bound to a layout, this is added to scroll view
//	sv->setResizePolicy( QScrollView::AutoOneFit ); // Makes sure frame is the size of scrollview
//	sv->setVScrollBarMode( QScrollView::AlwaysOff );
//	sv->setFixedHeight( 85 );
	plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//	plotWindowUILayout->addWidget( sv );
	plotWindowUILayout->addWidget( plot );

	resize( 625, 400 ); // Default size

	// Plot settings
//	font = timeScaleLabel->font(); // Use label font as template
//	font.setPixelSize(12);

	QwtText xAxisTitle, yAxisTitle;
	xAxisTitle.setText( "Time (ms)" );
	xAxisTitle.setFont( font );
	yAxisTitle.setText( "Current (nA)" );
	yAxisTitle.setFont( font );
	plot->setAxisTitle( QwtPlot::xBottom, xAxisTitle );
	plot->setAxisTitle( QwtPlot::yLeft, yAxisTitle );
	setAxes(); // Set axes to defaults (1,1000)(-20,20)

	QwtLegend *legend = new QwtLegend();
	plot->insertLegend( legend, QwtPlot::RightLegend );

	// Signal/Slot Connections
	QObject::connect( setAxesButton, SIGNAL(clicked(void)), this, SLOT(setAxes(void)) );
	QObject::connect( timeX1Edit, SIGNAL(valueChanged(int)), this, SLOT(setAxes(void)) );
	QObject::connect( timeX2Edit, SIGNAL(valueChanged(int)), this, SLOT(setAxes(void)) );
	QObject::connect( currentY1Edit, SIGNAL(valueChanged(int)), this, SLOT(setAxes(void)) );
	QObject::connect( currentY2Edit, SIGNAL(valueChanged(int)), this, SLOT(setAxes(void)) );
	QObject::connect( clearButton, SIGNAL(clicked(void)), this, SLOT(clearPlot(void)) );
	QObject::connect( overlaySweepsCheckBox, SIGNAL(clicked(void)), this, SLOT(toggleOverlay(void)) );
	QObject::connect( plotAfterCheckBox, SIGNAL(clicked(void)), this, SLOT(togglePlotAfter(void)) );
	QObject::connect( colorByComboBox, SIGNAL(activated(int)), this, SLOT(changeColorScheme(int)) );

//	font.setBold( false );

	// Add tooltip to color scheme combo box
	QString tooltip =
		QString( "There are 10 colors which rotate in the same order\n" ) +
		QString( "Run: Change color after every protocol run\n" ) +
		QString( "Trial: For use when running multiple trials - A color will correspond to a specific trial number\n" ) +
		QString( "Sweep: A color will correspond to a specific sweep" );
	colorByComboBox->setToolTip(tooltip); //QToolTip::add( colorByComboBox, tooltip );

	subWindow->setWidget(this);
	show();
	subWindow->adjustSize();
}

void ClampProtocolWindow::addCurve( double *output, curve_token_t token ) { // Attach curve to plot
	double time[ token.points ];

	if( overlaySweeps ) 
		for( size_t i = 0; i < token.points; i++ )
			time[ i ] = token.period * ( token.stepStartSweep + i );   
	else 
		for( size_t i = 0; i < token.points; i++ ) 
			time[ i ] = token.period * ( token.stepStart + i );                          

	if( token.stepStart == -1 ) // stepStart is offset by -1 in order to connect curves, but since i is unsigned, must be careful of going negative  
		time[ 0 ] = 0;

	int idx;
	QString curveTitle;

	bool legendShow = token.lastStep; // Whether legend entry will be added

	switch( colorScheme ) {
		case 0: // Color by Run
			idx = runCounter % 10;
			curveTitle = "Run " + QString::number( runCounter + 1 );

			if( token.lastStep ) // Increase run counter if curve is last step in a run
			runCounter++;              
			break;

		case 1: // Color by Trial
			idx = token.trial;
			curveTitle = "Trial " + QString::number( idx + 1 );
			break;

		case 2: // Color by sweep
			idx = token.sweep;

			if( idx >= sweepsShown ) {
				legendShow = true;
				sweepsShown++;
			}
			else
				legendShow = false;

			curveTitle = "Sweep " + QString::number( idx + 1 );
			break;

		default:
			break;
	}	 

	curveContainer.push_back( QwtPlotCurvePtr( new QwtPlotCurve(curveTitle) ) );
	QwtPlotCurvePtr curve = curveContainer.back();
	curve->setSamples( time, output, token.points ); // Makes a hard copy of both time and output
	colorCurve( curve, idx );
	curve->setItemAttribute( QwtPlotItem::Legend, legendShow ); // Set whether curve will appear on legend
	curve->attach( plot );

	if( legendShow ) {
//		qobject_cast<QwtLegend*>(plot->legend())->legendWidgets().back()->setFont( font ); // Adjust font
	}

	if( plotAfter && !token.lastStep ) // Return before replot if plotAfter is on and its not last step of protocol
		return ;

	plot->replot(); // Attaching curve does not refresh plot, must replot
}

void ClampProtocolWindow::colorCurve( QwtPlotCurvePtr curve, int idx ) {
	QColor color;

	switch( idx ) {
		case 0: color = QColor( Qt::black ); break;
		case 1: color = QColor( Qt::red ); break;
		case 2: color = QColor( Qt::blue ); break;
		case 3: color = QColor( Qt::green ); break;
		case 4: color = QColor( Qt::cyan ); break;
		case 5: color = QColor( Qt::magenta ); break;
		case 6: color = QColor( Qt::yellow ); break;
		case 7: color = QColor( Qt::lightGray ); break;
		case 8: color = QColor( Qt::darkRed ); break;
		case 9: color = QColor( Qt::darkGreen ); break;
		default: color = QColor( Qt::black ); break;
	}

	QPen pen( color, 2 ); // Set color and width
	curve->setPen( pen );
}

void ClampProtocolWindow::setAxes( void ) {    
	double timeFactor, currentFactor;

	switch( timeScaleEdit->currentIndex() ) { // Determine time scaling factor, convert to ms
		case 0: timeFactor = 10; // (s)
			break;
		case 1: timeFactor = 1; // (ms) default
			break;
		case 2: timeFactor = 0.1; // (us)
			break;
		default: timeFactor = 1; // should never be called
			break;
	}

	switch( currentScaleEdit->currentIndex() ) { // Determine current scaling factor, convert to nA
		case 0: currentFactor = 10; // (uA)
			break;
		case 1: currentFactor = 1; // (nA) default
			break;
		case 2: currentFactor = 0.1; // (pA)
			break;
		default: currentFactor = 1; // shoudl never be called
			break;
	}

	// Retrieve desired scale
	double x1, x2, y1, y2;

	x1 = timeX1Edit->value() * timeFactor;
	x2 = timeX2Edit->value() * timeFactor;
	y1 = currentY1Edit->value() * currentFactor;
	y2 = currentY2Edit->value() * currentFactor;

	plot->setAxes( x1, x2, y1, y2 );
}

void ClampProtocolWindow::clearPlot( void ) {
	curveContainer.clear();
	plot->replot();
}

void ClampProtocolWindow::toggleOverlay( void ) {    
	if( overlaySweepsCheckBox->isChecked() ) { // Checked
	// Check if curves are plotted, if true check if user wants plot cleared in
	// order to overlay sweeps during next run
		overlaySweeps = true;
	}
	else { // Unchecked
		overlaySweeps = false;
	}
}

void ClampProtocolWindow::togglePlotAfter( void ) {
	if( plotAfterCheckBox->isChecked() ) // Checked
		plotAfter = true;    
	else  // Unchecked
		plotAfter = false;

	plot->replot(); // Replot since curve container is cleared    
}

void ClampProtocolWindow::changeColorScheme( int choice ) {
	if( choice == colorScheme ) // If choice is the same
		return ;    

	// Check if curves are plotted, if true check if user wants plot cleared in
	// order to change color scheme
	if ( !curveContainer.empty() && QMessageBox::warning(
					this,
					"Warning",
					"Switching the color scheme will clear the plot.\nDo you wish to continue?",
					QMessageBox::Yes | QMessageBox::Default, QMessageBox::No
					| QMessageBox::Escape) != QMessageBox::Yes) {

		colorByComboBox->setCurrentIndex( colorScheme ); // Revert to old choice if answer is no
		return ;
	}

	colorScheme = choice;
	curveContainer.clear();
	plot->replot(); // Replot since curve container is cleared
}

void  ClampProtocolWindow::doDeferred( const Settings::Object::State &s ) { }

void  ClampProtocolWindow::doLoad( const Settings::Object::State &s ) {
	if ( s.loadInteger("Maximized") )
		showMaximized();
	else if ( s.loadInteger("Minimized") )
		showMinimized();

// Window Position
	if ( s.loadInteger( "W" ) != NULL ) {
		resize( s.loadInteger("W"), s.loadInteger("H") );
		parentWidget()->move( s.loadInteger("X"), s.loadInteger("Y") );
	}

	// Load Parameters
	timeX1Edit->setValue( s.loadInteger("X1") );
	timeX2Edit->setValue( s.loadInteger("X2") );
	timeScaleEdit->setCurrentIndex( s.loadInteger("Time Scale") );
	currentY1Edit->setValue( s.loadInteger("Y1") );
	currentY2Edit->setValue( s.loadInteger("Y2") );
	currentScaleEdit->setCurrentIndex( s.loadInteger("Current Scale") );
	overlaySweepsCheckBox->setChecked( s.loadInteger("Overlay Sweeps") );
	plotAfterCheckBox->setChecked( s.loadInteger("Plot After") );
	colorByComboBox->setCurrentIndex( s.loadInteger("Color Scheme") );
	changeColorScheme( s.loadInteger("Color Scheme") );
	setAxes();
	toggleOverlay();
	togglePlotAfter();
}

void  ClampProtocolWindow::doSave( Settings::Object::State &s ) const {
	if ( isMaximized() )
		s.saveInteger( "Maximized", 1 );
	else if ( isMinimized() )
		s.saveInteger( "Minimized", 1 );

	// Window Position
	QPoint pos = parentWidget()->pos();
	s.saveInteger( "X", pos.x() );
	s.saveInteger( "Y", pos.y() );
	s.saveInteger( "W", width() );
	s.saveInteger( "H", height() );

	// Save parameters
	s.saveInteger( "X1", timeX1Edit->value() );
	s.saveInteger( "X2", timeX2Edit->value() );
	s.saveInteger( "Time Scale", timeScaleEdit->currentIndex() );
	s.saveInteger( "Y1", currentY1Edit->value() );
	s.saveInteger( "Y2", currentY2Edit->value() );
	s.saveInteger( "Current Scale", currentScaleEdit->currentIndex() );
	s.saveInteger( "Overlay Sweeps", overlaySweepsCheckBox->isChecked() );
	s.saveInteger( "Plot After", plotAfterCheckBox->isChecked() );
	s.saveInteger( "Color Scheme", colorByComboBox->currentIndex() );
}

/*
*  Constructs a ClampProtocolWindowUI as a child of 'parent', with the
*  name 'name' and widget flags set to 'f'.
*/
/*
ClampProtocolWindowUI::ClampProtocolWindowUI( QWidget* parent, const char* name, WFlags fl ) : QWidget( parent, name, fl ) {
	QImage img;
	img.loadFromData( image0_data, sizeof( image0_data ), "PNG" );
	image0 = img;
	img.loadFromData( image1_data, sizeof( image1_data ), "PNG" );
	image1 = img;

	if ( !name ) setName( "ClampProtocolWindowUI" );

	frame = new QFrame( this, "frame" );
	frame->setGeometry( QRect( 40, 30, 645, 74 ) );
	frame->setFrameShape( QFrame::NoFrame );
	frame->setFrameShadow( QFrame::Plain );
	frameLayout = new QHBoxLayout( frame, 11, 6, "frameLayout"); 

	layout1 = new QGridLayout( 0, 1, 1, 0, 6, "layout1"); 

	currentScaleLabel = new QLabel( frame, "currentScaleLabel" );
	QFont currentScaleLabel_font(  currentScaleLabel->font() );
	currentScaleLabel_font.setBold( TRUE );
	currentScaleLabel->setFont( currentScaleLabel_font ); 

	layout1->addWidget( currentScaleLabel, 1, 0 );

	currentScaleEdit = new QComboBox( FALSE, frame, "currentScaleEdit" );
	currentScaleEdit->setMaximumSize( QSize( 50, 32767 ) );
	QFont currentScaleEdit_font(  currentScaleEdit->font() );
	currentScaleEdit->setFont( currentScaleEdit_font ); 

	layout1->addWidget( currentScaleEdit, 1, 3 );

	currentY2Edit = new QSpinBox( frame, "currentY2Edit" );
	currentY2Edit->setMinimumSize( QSize( 70, 0 ) );
	currentY2Edit->setMaximumSize( QSize( 70, 32767 ) );
	currentY2Edit->setMaxValue( 999999999 );
	currentY2Edit->setMinValue( -999999999 );
	currentY2Edit->setValue( 20 );

	layout1->addWidget( currentY2Edit, 1, 2 );

	timeScaleEdit = new QComboBox( FALSE, frame, "timeScaleEdit" );
	timeScaleEdit->setMaximumSize( QSize( 50, 32767 ) );
	QFont timeScaleEdit_font(  timeScaleEdit->font() );
	timeScaleEdit->setFont( timeScaleEdit_font ); 

	layout1->addWidget( timeScaleEdit, 0, 3 );

	timeX2Edit = new QSpinBox( frame, "timeX2Edit" );
	timeX2Edit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, timeX2Edit->sizePolicy().hasHeightForWidth() ) );
	timeX2Edit->setMinimumSize( QSize( 70, 0 ) );
	timeX2Edit->setMaximumSize( QSize( 70, 32767 ) );
	timeX2Edit->setMaxValue( 999999999 );
	timeX2Edit->setValue( 1000 );

	layout1->addWidget( timeX2Edit, 0, 2 );

	currentY1Edit = new QSpinBox( frame, "currentY1Edit" );
	currentY1Edit->setMinimumSize( QSize( 70, 0 ) );
	currentY1Edit->setMaximumSize( QSize( 70, 32767 ) );
	currentY1Edit->setMaxValue( 999999999 );
	currentY1Edit->setMinValue( -999999999 );
	currentY1Edit->setValue( -20 );

	layout1->addWidget( currentY1Edit, 1, 1 );

	timeScaleLabel = new QLabel( frame, "timeScaleLabel" );
	QFont timeScaleLabel_font(  timeScaleLabel->font() );
	timeScaleLabel_font.setBold( TRUE );
	timeScaleLabel->setFont( timeScaleLabel_font ); 

	layout1->addWidget( timeScaleLabel, 0, 0 );

	timeX1Edit = new QSpinBox( frame, "timeX1Edit" );
	timeX1Edit->setMinimumSize( QSize( 70, 0 ) );
	timeX1Edit->setMaximumSize( QSize( 70, 32767 ) );
	timeX1Edit->setMaxValue( 999999999 );

	layout1->addWidget( timeX1Edit, 0, 1 );
	frameLayout->addLayout( layout1 );

	setAxesButton = new QPushButton( frame, "setAxesButton" );
	setAxesButton->setEnabled( TRUE );
	setAxesButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, setAxesButton->sizePolicy().hasHeightForWidth() ) );
	setAxesButton->setMinimumSize( QSize( 40, 40 ) );
	setAxesButton->setMaximumSize( QSize( 40, 40 ) );
	QFont setAxesButton_font(  setAxesButton->font() );
	setAxesButton->setFont( setAxesButton_font ); 
	setAxesButton->setPixmap( image0 );
	setAxesButton->setToggleButton( FALSE );
	setAxesButton->setFlat( FALSE );
	frameLayout->addWidget( setAxesButton );

	layout2 = new QVBoxLayout( 0, 0, 6, "layout2"); 

	overlaySweepsCheckBox = new QCheckBox( frame, "overlaySweepsCheckBox" );
	overlaySweepsCheckBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, overlaySweepsCheckBox->sizePolicy().hasHeightForWidth() ) );
	QFont overlaySweepsCheckBox_font(  overlaySweepsCheckBox->font() );
	overlaySweepsCheckBox_font.setBold( TRUE );
	overlaySweepsCheckBox->setFont( overlaySweepsCheckBox_font ); 
	layout2->addWidget( overlaySweepsCheckBox );

	plotAfterCheckBox = new QCheckBox( frame, "plotAfterCheckBox" );
	plotAfterCheckBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, plotAfterCheckBox->sizePolicy().hasHeightForWidth() ) );
	QFont plotAfterCheckBox_font(  plotAfterCheckBox->font() );
	plotAfterCheckBox_font.setBold( TRUE );
	plotAfterCheckBox->setFont( plotAfterCheckBox_font ); 
	layout2->addWidget( plotAfterCheckBox );
	frameLayout->addLayout( layout2 );

	layout3 = new QVBoxLayout( 0, 0, 6, "layout3"); 

	textLabel1 = new QLabel( frame, "textLabel1" );
	QFont textLabel1_font(  textLabel1->font() );
	textLabel1_font.setBold( TRUE );
	textLabel1->setFont( textLabel1_font ); 
	layout3->addWidget( textLabel1 );

	colorByComboBox = new QComboBox( FALSE, frame, "colorByComboBox" );
	QFont colorByComboBox_font(  colorByComboBox->font() );
	colorByComboBox->setFont( colorByComboBox_font ); 
	layout3->addWidget( colorByComboBox );
	frameLayout->addLayout( layout3 );
	spacer = new QSpacerItem( 1, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	frameLayout->addItem( spacer );

	clearButton = new QPushButton( frame, "clearButton" );
	clearButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, clearButton->sizePolicy().hasHeightForWidth() ) );
	clearButton->setMinimumSize( QSize( 40, 40 ) );
	clearButton->setMaximumSize( QSize( 40, 40 ) );
	QFont clearButton_font(  clearButton->font() );
	clearButton->setFont( clearButton_font ); 
	clearButton->setPixmap( image1 );
	clearButton->setToggleButton( FALSE );
	frameLayout->addWidget( clearButton );
	languageChange();
	resize( QSize(938, 215).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
}
*/

/*
*  Sets the strings of the subwidgets using the current
*  language.
*/
/*
void ClampProtocolWindow::languageChange() {
	setCaption( tr( "Clamp Protocol Plot Window" ) );
	currentScaleLabel->setText( tr( "Current" ) );
	QToolTip::add( currentScaleLabel, tr( "Current Scale" ) );
	currentScaleEdit->clear();
	currentScaleEdit->insertItem( trUtf8( "\xce\xbc\x41" ) );
	currentScaleEdit->insertItem( tr( "nA" ) );
	currentScaleEdit->insertItem( tr( "pA" ) );
	currentScaleEdit->setCurrentItem( 1 );
	timeScaleEdit->clear();
	timeScaleEdit->insertItem( tr( "s" ) );
	timeScaleEdit->insertItem( tr( "ms" ) );
	timeScaleEdit->insertItem( trUtf8( "\xce\xbc\x73" ) );
	timeScaleEdit->insertItem( tr( "ns" ) );
	timeScaleEdit->setCurrentItem( 1 );
	timeScaleLabel->setText( tr( "Time" ) );
	QToolTip::add( timeScaleLabel, tr( "Time Scale" ) );
	setAxesButton->setText( QString::null );
	QToolTip::add( setAxesButton, tr( "Set axes to desired scale" ) );
	overlaySweepsCheckBox->setText( tr( "Overlay Sweeps" ) );
	QToolTip::add( overlaySweepsCheckBox, tr( "Check to overlay all sweeps in a particular segment" ) );
	plotAfterCheckBox->setText( tr( "Plot after protocol" ) );
	QToolTip::add( plotAfterCheckBox, tr( "Check to plot only at the end of a protocol instead of after each step in a protocol" ) );
	textLabel1->setText( tr( "Color by:" ) );
	QToolTip::add( textLabel1, tr( "Change the color scheme of the plot" ) );
	colorByComboBox->clear();
	colorByComboBox->insertItem( tr( "Run" ) );
	colorByComboBox->insertItem( tr( "Trial" ) );
	colorByComboBox->insertItem( tr( "Sweep" ) );
	QToolTip::add( colorByComboBox, QString::null );
	clearButton->setText( QString::null );
	QToolTip::add( clearButton, tr( "Clear plot" ) );
}
*/
