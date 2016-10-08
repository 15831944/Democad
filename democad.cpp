#include "democad.h"
#include "ui_democad.h"

#include "qg_actionhandler.h"
#include "qg_snaptoolbar.h"
#include "qg_cadtoolbarmain.h"
#include "qg_actionfactory.h"

#include "dialogmediator.h"//���ڸ��½�����߼�
#include "rs_document.h"
#include "QMessageBox"
#include "global_Values.h"
#include <QFileDialog>
#include "Parameter.h"
#include "PublicFuction.h"
#ifndef Q_MOC_RUN
#include <boost/date_time/posix_time/posix_time.hpp>
#endif
#include <limits>
#include <QFontDatabase>

QAction* DemoCad::undoButton=NULL;
QAction* DemoCad::redoButton=NULL;

#define CONTAINER graphicView->getContainer()
#define ENTITIES  graphicView->getContainer()->getEntities()

DemoCad::DemoCad(QWidget *parent) 
	: QMainWindow(parent)
	, ui(new Ui::DemoCad)
	, m_AlwTimerId( 0 )
{
	//< ����������
#ifdef Q_OS_WIN
	this->setMinimumSize( 800, 600 );
	this->resize( 800, 600 );
#else
	g_CIAddress = CreateCIKernel();
	qDebug() << "liuyc : DemoCAD get CIaddress = " << g_CIAddress;
	if( g_CIAddress == 0 ) //< CI�����ڴ��ȡʧ��
	{
		qDebug() << "liuyc : DemoCAD CreateCIKernel failed! ";
	}
	//this->resize( 800, 580 );
	//this->setMinimumSize( 800, 580 );
	this->setWindowState( Qt::WindowMaximized );
#endif

	//< N3�ĺ�����ʾ������΢��һ��ͻ��ݺ����أ����Գ����˿��壬����Ŵ�û���κ�����
	//< �����������simkai�󣬿��ڴ�����ʹ�ÿ��壬����������=_=
	//< ����Ŀǰ�Ľ���취���ǰ�N3�ĺ����С�����ں�С�ķ�Χ�ڣ�X86�Ĳ��ô���
//#ifdef __arm__
//	int fontId = QFontDatabase::addApplicationFont("/usr/lib/fonts/simkai.ttf");
//	QStringList fontFamilies = QFontDatabase::applicationFontFamilies( fontId );
//
//	//< ��������
//	QFont font( fontFamilies.at(0) );
//	font.setPixelSize( 13 );
//#endif

	QApplication::setStyle("cleanlooks");
	this->setStyleSheet("*{ \
						    font-family: simhei; \
						    font-size: 13px; \
						} \
		                QLineEdit{ \
						    border-radius: 2px; \
						    border-color: #757575; \
						    border-width: 2px; \
						    padding: 0px; \
							border-style: groove; \
							background-color: #FFFFFF; \
						} \
						QLineEdit:disabled{ \
							background-color: #DDDDDD; \
						} \
						QLineEdit:focus{ \
							border-color: blue; \
						} \
						QScrollBar:vertical{ \
						    width: 14px; \
							background-color: rgba(0,0,0,0%); \
							padding-top: 10px; \
							padding-bottom: 10px; \
							} \
                        QScrollBar:horizontal { \
							height:14px; \
							background-color:rgba(0,0,0,0%); \
							padding-left:10px; \
							padding-right:10px; \
						}\
						QScrollBar::handle:vertical { \
						    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #B8B8B8, stop:1 #D6D6D6); \
						} \
						QScrollBar::handle:horizontal { \
							background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #B8B8B8, stop:1 #D6D6D6); \
						} \
						QTabBar::tab{ \
							border-bottom-left-radius:0px; \
							border-bottom-right-radius:0px; \
							color: #F0F0F0; \
							min-width: 60px; \
							min-height: 20px; \
							padding: 3px 8px 3px 8px; \
							margin:1px; \
							background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #B8B8B8, stop:1 #D6D6D6); \
						} \
						QTabBar::tab:selected, QTabBar::tab:hover { \
							background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #454648, stop:1 #7A7A7A); \
						} \
						QGroupBox{ \
						    border-width: 2px; \
							border-color: #777777; \
							border-style: inset; \
							background-color: rgb(255,255,255); \
						} \
						QRadioButton { \
                            spacing: 5px; \
                        } \
                        QRadioButton::indicator { \
                            width: 12px; \
                            height: 12px; \
                        } \
						QTextEdit{ \
						    padding-left: 8px; \
							padding-right:8px; \
						} \
						");
	
	//< ��δ�����Կ�������ͼ�εı߿�UI���Ժܺ���
	/*QFrame{ \
	border:1px solid #B8B8B8; \
	border-radius: 0px; \
	} \ */
	
	
	this->setWindowTitle("Lynuc CAD V1.1.0921");
	gMainWindow = this;
	//this->setFont( FONT_10_SIMHEI_LIGHT );

	g_rsvDemoCadS.x  = geometry().x();
	g_rsvDemoCadS.y  = geometry().y();
	g_rsvDemoCadWH.x = geometry().width();
	g_rsvDemoCadWH.y = geometry().height();

    setFocusPolicy(Qt::StrongFocus);
    actionHandler = new QG_ActionHandler( this );
    graphicView   = new GraphicView( this );
    setCentralWidget( graphicView );

	//< �����½�����Ϊ�ұ�DOCK���У����������DOCK��ռ���������ڵĿ��
	this->setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

	//< �ұߵ�DOCK�����ø߹���صĸ��ֲ���
	m_ParaSetWdt = new ParaSetWdt( this );
	m_ParaSetWdt->setWindowTitle( tr("̽�����") );
	addDockWidget( Qt::RightDockWidgetArea, m_ParaSetWdt);
	m_ParaSetWdt->setFeatures( QDockWidget::NoDockWidgetFeatures );
	
	connect( m_ParaSetWdt, SIGNAL( sglSetMsg( QString )), this, SLOT( slotSetMsg( QString )));

	m_OffsetWdt = new OffsetWdt( this );
	m_OffsetWdt->setWindowTitle( tr("����") );
	addDockWidget( Qt::RightDockWidgetArea, m_OffsetWdt);
	m_OffsetWdt->setFeatures( QDockWidget::NoDockWidgetFeatures );
	connect( this, SIGNAL( sglOpenDXFFile( QString &)), m_OffsetWdt, SLOT(slotSetDXFFile(QString &)));
	connect( m_ParaSetWdt, SIGNAL( sglCountChanged()), m_OffsetWdt, SLOT(slotCountChanged()));
	connect( m_OffsetWdt, SIGNAL( sglConfigRdy()), m_ParaSetWdt, SLOT(slotConfig()));
	connect( m_OffsetWdt, SIGNAL( sglSetMsg( QString )), this, SLOT( slotSetMsg( QString )));

	//< ����DOCKWIDGET��TAB���ԣ���������ʱ��ʾ��̽�����������
	setDockOptions( QMainWindow::ForceTabbedDocks );
	tabifyDockWidget( m_ParaSetWdt, m_OffsetWdt );
	m_ParaSetWdt->raise();
	setTabPosition( Qt::RightDockWidgetArea, QTabWidget::North );


	//< ����ͼ�ؼ�������ͼ�Ͳ���ͼ��
	m_SideGraphicView = new GraphicViewSide( this );
	addDockWidget( Qt::BottomDockWidgetArea, m_SideGraphicView );
	m_SideGraphicView->setFeatures( QDockWidget::NoDockWidgetFeatures );
	//connect( this, SIGNAL( sglGetNewContainer(RS_EntityContainer *) )
	//, m_SideGraphicView, SLOT( doGetNewContainer(RS_EntityContainer *)));


    //< �˵����Ͳ˵�������������г�ʼ��
    initToolBar();
    initActions();

    DIALOGMEDIATOR->setCadToolBarMain(cadToolBarMain); 
    DIALOGMEDIATOR->setOptionWidget(optionWidget);
    DIALOGMEDIATOR->setRedoButton(redoButton);
    DIALOGMEDIATOR->setUndoButton(undoButton);

	setContextMenuPolicy(Qt::NoContextMenu);

	//< �ļ�����
	connect(g_pQPublicSignal, SIGNAL(SglOpenOpt()),	  this, SLOT(openFile()));
	connect(g_pQPublicSignal, SIGNAL(SglNewOpt()),	  this, SLOT(newFile()));
	connect(g_pQPublicSignal, SIGNAL(SglSaveOpt()),	  this, SLOT(saveFile()));
	connect(g_pQPublicSignal, SIGNAL(SglSaveAsOpt()), this, SLOT(saveAsFile()));
	connect(g_pQPublicSignal, SIGNAL(SglGetNewContainer( RS_EntityContainer * )),
		m_SideGraphicView, SLOT( doGetNewContainer(RS_EntityContainer *)));
	connect(g_pQPublicSignal, SIGNAL(SglGetNewContainer( RS_EntityContainer * )),
		m_OffsetWdt, SLOT( slotGetNewContainer(RS_EntityContainer *)));

#ifdef __arm__  //< ֻ��N3��Ҫ��U5����ֱ������
	//< N3ר�õĹر�С��ť
	m_HideLabelN3 = new hideLabelN3( NULL );
	m_HideLabelN3->show();
	m_HideLabelN3->setFocusPolicy( Qt::NoFocus );
	m_HideLabelN3->move( QApplication::desktop()->width() * 0.67, 0 );
	connect( m_HideLabelN3, SIGNAL( sglSetVisible(bool) ), this, SLOT( slotThisVis(bool) ) );
#endif
}

DemoCad::~DemoCad()
{
	if( m_AlwTimerId != 0 )
	{
		killTimer( m_AlwTimerId );
		m_AlwTimerId = 0;
#ifndef Q_OS_WIN
		SetMacroVal( g_CIAddress, MACRO_CAD_START_SUC, 0. );
#endif
	}

#ifdef __arm__  
	if( m_HideLabelN3 != NULL )
	{
		delete m_HideLabelN3;
		m_HideLabelN3 = NULL;
	}
#endif
#ifndef Q_OS_WIN
	DestroyCIKernel( &g_CIAddress );
#endif

	QCoreApplication::exit();  //< Ϊ��ֹ����˳�ʱ��������֪��Ϊʲô���Է�ֹ������ȷʵ��Ч����ֻ���DEBUG�棩
}

void DemoCad::initToolBar() 
{
    QSizePolicy toolBarPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	// ȫ��
	qToolBarFullScrn = new QToolBar(tr("ȫ��"), this);
	qToolBarFullScrn->setMinimumSize(38, 38);
	qToolBarFullScrn->setSizePolicy(toolBarPolicy);
	qToolBarFullScrn->setObjectName (tr("ȫ��"));
	this->addToolBar(qToolBarFullScrn);
	cadToolBarFullScrn = new QG_CadToolBar_FSCreen(qToolBarFullScrn);
	cadToolBarFullScrn->setActionHandler(actionHandler);//������actionHandler����ۻ��õ�
	addToolBar(Qt::TopToolBarArea, qToolBarFullScrn);

	snapToolBar = new QG_SnapToolBar(tr("��׽ѡ��"),actionHandler, this);
	snapToolBar->setSizePolicy(toolBarPolicy);
	snapToolBar->setObjectName (tr("��׽ѡ��"));
	snapToolBar->setMovable(false);
	this->addToolBar(snapToolBar);

	editToolBar = new QToolBar( tr("�༭ѡ��"), snapToolBar);
	editToolBar->setSizePolicy(toolBarPolicy);
	editToolBar->setObjectName (tr("�༭ѡ��"));
	editToolBar->setMovable(false);
	this->addToolBar(editToolBar);

	// CAD ������
    cadToolBar = new QToolBar(tr("CAD������"), this);
    cadToolBar->setMinimumSize(65,300);
    QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    cadToolBar->setSizePolicy(policy);
    cadToolBar->setObjectName (tr("CAD������"));
    cadToolBar->setFixedWidth(66);
    cadToolBar->setMovable( false );//�������϶�����toolbar
    cadToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);//�����϶�ͣ�������ұ�
    cadToolBarMain = new QG_CadToolBarMain(cadToolBar);
    cadToolBarMain->setActionHandler(actionHandler);//������actionHandler����ۻ��õ�
	cadToolBarMain->getGraphicView_RS_EntityContainer(graphicView->getContainer(), graphicView);
    addToolBar(Qt::LeftToolBarArea, cadToolBar);
	cadToolBar->show();

	// ���߹�����
	qToolBarLeadWire = new QToolBar(tr("���߹�����"), this);
	qToolBarLeadWire->setMinimumSize(65,100);
	QSizePolicy policy1(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
	qToolBarLeadWire->setSizePolicy(policy1);
	qToolBarLeadWire->setObjectName (tr("���߹�����"));
	qToolBarLeadWire->setFixedWidth(66);
	qToolBarLeadWire->setMovable( false );//�������϶�����toolbar
	qToolBarLeadWire->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);//�����϶�ͣ�������ұ�
	cadToolBarLeadWire = new QG_CadToolBar_LeadWire(qToolBarLeadWire);
	cadToolBarLeadWire->setActionHandler( actionHandler );//������actionHandler����ۻ��õ�
	cadToolBarLeadWire->getGraphicView_RS_EntityContainer(graphicView->getContainer(), graphicView);
	addToolBar(Qt::LeftToolBarArea, qToolBarLeadWire);
	qToolBarLeadWire->setVisible( false );

	// �ļ�������
	qToolBarFile = new QToolBar(tr("�ļ�������"), this);
	qToolBarFile->setMinimumSize(65,100);
	QSizePolicy policyFile(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
	qToolBarFile->setSizePolicy(policyFile);
	qToolBarFile->setObjectName (tr("�ļ�������"));
	qToolBarFile->setFixedWidth(66);
	qToolBarFile->setMovable( false );//�������϶�����toolbar
	qToolBarFile->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);//�����϶�ͣ�������ұ�
	cadToolBarFile = new QG_CadToolBar_FileOpt(qToolBarFile);
	cadToolBarFile->setActionHandler(actionHandler);//������actionHandler����ۻ��õ�
	addToolBar(Qt::LeftToolBarArea, qToolBarFile);
	qToolBarFile->setVisible( true );

	// �������߹�����
	qToolBarAssistTool = new QToolBar(tr("��������ѡ��"), this);
	qToolBarAssistTool->setMinimumSize(65,100);
	QSizePolicy policyFileAT(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
	qToolBarAssistTool->setSizePolicy(policyFileAT);
	qToolBarAssistTool->setObjectName (tr("��������ѡ��"));
	qToolBarAssistTool->setFixedWidth(66);
	qToolBarAssistTool->setMovable(false);//�������϶�����toolbar
	qToolBarAssistTool->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);//�����϶�ͣ�������ұ�
	cadToolBarAssistTool = new QG_CadToolBar_AssistTool(qToolBarAssistTool);
	cadToolBarAssistTool->setActionHandler(actionHandler);//������actionHandler����ۻ��õ�
	addToolBar(Qt::LeftToolBarArea, qToolBarAssistTool);
	qToolBarAssistTool->setVisible( true );

    optionWidget = new QToolBar(tr("������Ϣѡ��"), snapToolBar);
	QSizePolicy optionWidgetBarPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	optionWidget->setSizePolicy(optionWidgetBarPolicy);
	optionWidget->setObjectName (tr("������Ϣѡ��"));
	optionWidget->setMovable(false);
	this->addToolBar(optionWidget);
}

void DemoCad::initActions() {

    QG_ActionFactory actionFactory(actionHandler, this);//�Ȱ�action����������������
    QAction* action;
    QToolBar* tb;

    tb = editToolBar;
    tb->setWindowTitle(tr("Edit"));

    undoButton = actionFactory.createAction(RS2::ActionEditUndo, actionHandler);
    undoButton->setEnabled(false);
    tb->addAction(undoButton);

    redoButton = actionFactory.createAction(RS2::ActionEditRedo, actionHandler);
    redoButton->setEnabled(false);
    tb->addAction(redoButton);

    action = actionFactory.createAction(RS2::ActionViewGrid, this);
    tb->addAction(action);
    action->setChecked(true);
    tb->addAction(action);

	action = actionFactory.createAction(RS2::ActionZoomIn, actionHandler);
	tb->addAction(action);

	action = actionFactory.createAction(RS2::ActionZoomOut, actionHandler);
	tb->addAction(action);
}


void DemoCad::slotViewGrid(bool toggle) 
{
    graphicView->setGridOn(toggle);
    graphicView->redraw(RS2::RedrawGrid);
}

void DemoCad::slotThisVis( bool flag )
{
	this->setVisible( flag );
	if( flag )
		this->raise();
}

void DemoCad::slotSetMsg( QString msg )
{
	OperationPromptInfo( msg );
}

void DemoCad::setModified(bool m) {
	getDocument()->setModified(m);
}

bool DemoCad::isModified() {
	return getDocument()->isModified();
}

void DemoCad::setToolBarVisiable(bool show) {
	snapToolBar->setVisible(show);
	cadToolBar->setVisible(show);
	editToolBar->setVisible(show);
	optionWidget->setVisible(show);
}

void DemoCad::setToolBarEnabled(bool enable,int num){
	//changed by wang.k.h
	if (6==num)      
	{
		snapToolBar->setEnabled(true);
	}
	else
	{
		snapToolBar->setEnabled(enable);
	}
	cadToolBar->setEnabled(enable);
	editToolBar->setEnabled(enable);
	optionWidget->setEnabled(true);
}

void DemoCad::openFile()			// ��
{
	//< step1 ��ȡ��ǰ���ļ������Ѿ��޸����ж��Ƿ񱣴��޸ĺ���ļ�
	RS_Document *doc = getDocument();

	if (doc->isModified())
	{
		QString fileName;
		ErrorMessage errorMessage(this);
		int nRtn = -1;

		if (getDocument()->getFilename().isEmpty())
		{
			fileName = getDocument()->getAutoSaveFilename();
			nRtn     = errorMessage.showErrorMessage(10007, 0, fileName);
		}
		else
		{
			fileName = getDocument()->getFilename();
			nRtn     = errorMessage.showErrorMessage(10006, 0, fileName);
		}

		switch (nRtn)
		{
		case 2:			// �����ļ�
			if (saveFile())
			{
				errorMessage.showErrorMessage(10008, 2);
			}
			else
			{
				return;
			}
			break;
		case 3:			// �������ļ���ֱ�Ӵ��ļ�
			break;
		case 4:			// ȡ����ǰ����
			return;
		default:
			break;
		}

	}

    //< step2 ���µ�DXF�ļ�
#ifdef Q_OS_WIN
	QString fileName = QFileDialog::getOpenFileName(this, tr("���ļ�"), "./DXFfiles/", tr("DXF (*.dxf *.DXF)"));
#else
	QString fileName = QFileDialog::getOpenFileName(this, tr("���ļ�"), "/home/Lynuc/Users/NCFiles/", tr("DXF (*.dxf *.DXF)"));
#endif
	if (fileName.isNull() || fileName.isEmpty()) 
	{
		return;
	}

	openFile( fileName );
}

bool DemoCad::openFile( QString filename )
{
	bool bEmpty = false;
	bool bOpen  = getDocument()->open(filename, bEmpty);

	if (!bOpen)					// �ļ���ʧ��
	{
		popMessageBox( "Lynuc CAD Warning", tr("DXF�ļ���ʧ�ܣ�"), 5, QColor(255,0,0), true );
		return false;
	}
	else						// �򿪳ɹ�
	{
		if (bEmpty)				// �ļ�Ϊ��
		{
			popMessageBox( "Lynuc CAD Warning", tr("DXF�ļ�Ϊ�գ�"), 5, QColor(255,0,0), true );
			return false;
		}
		RS_EntityContainer * tmpCont = graphicView->getContainer();
		g_pQPublicSignal->SendGetNewContainer( tmpCont );

		emit sglOpenDXFFile( filename );
	}

	return true;
}

void DemoCad::analyseBorder()
{
	if( CONTAINER->isEmpty() )
		return;

	QList< RS_Entity * > savedXLines;
	QList< RS_Entity * > savedYLines;
	//< ����ʵ��Ѱ�����������ĸ��ߣ���һ�����еı߶����ڣ�
	for( QList<RS_Entity *>::iterator it = ENTITIES.begin(); 
		 it != ENTITIES.end(); 
		 ++it )
	{
		//< ���ɼ���ʵ���������ֱ�Ӻ���
		if( !(*it)->isVisible() || (*it)->isLead() )
			continue;

		//< �������ֱ�������
		if( (*it)->rtti() != RS2::EntityLine )
			continue;

		//< �����ֱ�ߵ����̣�һ�㲻��ֱ�߽߱����Բ�ǵĵط�
		if( (*it)->getLength() < 0.5 )
			continue;

        if( fabs( (*it)->getEndpoint().x - (*it)->getStartpoint().x ) < 10e-5 )
			savedYLines.append( *it );
		else if( fabs( (*it)->getEndpoint().y - (*it)->getStartpoint().y ) < 10e-5 )
			savedXLines.append( *it );
	}

	qDebug() << "liuyc saved lines count = " << savedXLines.count() << savedYLines.count();
	int leftIndex = -1;
	int rightIndex = -1;
	double minX = RS_MAXDOUBLE;  //< �����X
	double maxX = RS_MINDOUBLE;  //< ���ұ�X
	if( savedYLines.count() >= 2 )
	{
		for( int i = 0; i < savedYLines.count(); ++i )
		{
			if( savedYLines.at( i )->getMiddlePoint().x >= maxX )
			{
				maxX = savedYLines.at( i )->getMiddlePoint().x;
				rightIndex = i;
			}
			if( savedYLines.at( i )->getMiddlePoint().x <= minX )
			{
				minX = savedYLines.at( i )->getMiddlePoint().x;
				leftIndex = i;
			}
		}
	}
	else if( savedYLines.count() == 1 )
	{
		maxX = minX = savedYLines.first()->getMiddlePoint().x;
	}
	else if( savedYLines.count() == 0 )
	{
		qDebug() << "liuyc x has no edge!";
		maxX = minX = RS_MINDOUBLE;
	}
		
}

bool DemoCad::saveFile()
{
	RS_Document *doc = getDocument();

	if (!doc->isModified()) 
	{
		return true;
	}

	if (doc->isNewFile() || (doc->getFilename() == QString("DefaultFile")))
	{
		//qDebug() << "liuyc saving default file!";
		if (!saveAsFile())
		{
			return false;
		}
	} 
	else 
	{
		qDebug() << "liuyc not saving default file!";
		if (!doc->save()) 
		{
			qDebug() << "liuyc save failed!";
			QMessageBox::information(this, "Error!", tr("����ʧ��!"));
			return false;
		}
	}

	return true;

}

bool DemoCad::saveAsFile()
{
	RS_Document *doc = getDocument();


	QString selectFliter;
	RS2::FormatType type;
#ifdef Q_OS_WIN
	QString fileName = QFileDialog::getSaveFileName(this, tr("�����ļ�"),  "./DXFfiles/", tr("DXF (*.dxf *.DXF)"), &selectFliter);
#else
	QString fileName = QFileDialog::getSaveFileName(this, tr("�����ļ�"),  "/home/Lynuc/Users/NCFiles/", tr("DXF (*.dxf *.DXF)"), &selectFliter);
#endif
	if (fileName.isEmpty())
	{
		return false;
	}

	if (selectFliter.contains("dxf", Qt::CaseInsensitive)) 
	{
		type = RS2::FormatDXFRW;
		selectFliter = QString("dxf");
	}
	else 
	{
		type = RS2::FormatDYWRW;
		selectFliter = QString("edu");
	}

	if (fileName.section(".", -1, -1).compare(selectFliter, Qt::CaseInsensitive) != 0)
	{
		fileName = fileName + "." + selectFliter;
	}
	qDebug() << "liuyc save as " << fileName;
	doc->setFilename(fileName);

	if (!doc->saveAs(fileName, type)) 
	{
		qDebug() << "liuyc save as file failed!";
		QMessageBox::information(this, "Error!", "����ʧ��!");
		return false;
	}

	return true;

}

void DemoCad::timerEvent(QTimerEvent * event)
{
#ifndef Q_OS_WIN
	if( event->timerId() == m_AlwTimerId )
	{
		boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();   

		double flagG100P150L10 = 0.;
		double flagG100P150L11 = 0.;
		double flagG100P150L12 = 0.;
		double flagG100P150L25 = 0.;
		double flagG100P150L20 = 0.;
		double flagG100P150L22 = 0.;
		double flagG100P150L21 = 0.;
		double flagG100P150L35 = 0.;
		double flagG100P150L30 = 0.;
		double flagG100P150L32 = 0.;
		double flagG100P150L31 = 0.;

		double flagG100P151L51 = 0.;
		double flagG100P151L10 = 0.;
		double flagG100P151L11 = 0.;
		double flagG100P151L21 = 0.;
		double flagG100P151L60 = 0.;
		double flagG100P151L61 = 0.;

		double flagG100P152L11 = 0.;

		double flagG100P155L31 = 0.;

		GetMacroVal( g_CIAddress, MACRO_P150L10_FALG, flagG100P150L10 );
		if( fabs( flagG100P150L10 - 1. ) < 10e-3 )
		{
			clearData_L10();
			SetMacroVal( g_CIAddress, MACRO_P150L10_FALG, 0. );
		}

		GetMacroVal( g_CIAddress, MACRO_P150L11_FLAG, flagG100P150L11 );
		if( abs( flagG100P150L11 - 1. ) < 10e-3 && clearDataAccIndex_L11() )
		{
			SetMacroVal( g_CIAddress, MACRO_P150L11_FLAG, 0. );
		}

		GetMacroVal( g_CIAddress, MACRO_P150L12_FLAG, flagG100P150L12 );
		if( fabs( flagG100P150L12 - 1. ) < 10e-3 && clearSaveralData_L12() )
		{
			SetMacroVal( g_CIAddress, MACRO_P150L12_FLAG, 0. );
		}

		GetMacroVal( g_CIAddress, MACRO_P150L25_FLAG, flagG100P150L25 );
		if( fabs( flagG100P150L25 - 1. ) < 10e-3 && saveDataAccIndex_L25() )
		{
		    SetMacroVal( g_CIAddress, MACRO_P150L25_FLAG, 0. );
		}

		GetMacroVal( g_CIAddress, MACRO_P150L20_FLAG, flagG100P150L20 );
		if( fabs( flagG100P150L20 - 1.0 ) < 10e-3 )  //< ����ѭ����ȡ���ݱ��浽�ڴ��QMAP�� 1007
		{
			GetMacroVal( g_CIAddress, MACRO_P150L22_DATARDY, flagG100P150L22 );  //< 1009
			if( fabs( flagG100P150L22 - 1. ) < 10e-3 && saveDataAccOrder_L22())  //< ����׼����ɣ��洢��QMAP����ȥ
			{
				SetMacroVal( g_CIAddress, MACRO_P150L22_DATARDY, 0. );
				qDebug() << "liuyc L22 flag reset!";
			}

			GetMacroVal( g_CIAddress, MACRO_P150L21_FLAG, flagG100P150L21 );  
			if( fabs( flagG100P150L21 - 1. ) < 10e-3 )  //< �����洢����
			{
				SetMacroVal( g_CIAddress, MACRO_P150L21_FLAG, 0.);
				SetMacroVal( g_CIAddress, MACRO_P150L20_FLAG, 0.);
			}
		}
		
		GetMacroVal( g_CIAddress, MACRO_P150L35_FLAG, flagG100P150L35 );
		if( fabs( flagG100P150L35 - 1. ) < 10e-3 && getDataAccIndex_L35() )
		{
			SetMacroVal( g_CIAddress, MACRO_P150L35_FLAG, 0. );
		}

		GetMacroVal( g_CIAddress, MACRO_P150L30_FLAG, flagG100P150L30 );
		if( fabs( flagG100P150L30 - 1.) < 10e-3 )  //< ����ѭ����ȡ����
		{
			GetMacroVal( g_CIAddress, MACRO_P150L32_FLAG, flagG100P150L32 );
			if( fabs( flagG100P150L32 - 1.) < 10e-3 && getDataAccOrder_L32() )
			{
				SetMacroVal( g_CIAddress, MACRO_P150L32_FLAG, 0. );
			}

			GetMacroVal( g_CIAddress, MACRO_P150L31_FLAG, flagG100P150L31 );
			if( fabs( flagG100P150L31 - 1.) < 10e-3 )
			{
				SetMacroVal( g_CIAddress, MACRO_P150L31_FLAG, 0.);
				SetMacroVal( g_CIAddress, MACRO_P150L30_FLAG, 0.);
			}
		}

		GetMacroVal( g_CIAddress, MACRO_P151L51_FALG, flagG100P151L51 );
		if( fabs( flagG100P151L51 - 1. ) < 10e-3 && getAngleOff_P151L51() )
		{
			SetMacroVal( g_CIAddress, MACRO_P151L51_FALG, 0.);
			qDebug() << "G100P151L51 flag reset!";
		}

		GetMacroVal( g_CIAddress, MACRO_P151L10_FLAG, flagG100P151L10 );
		if( fabs( flagG100P151L10 - 1. ) < 10e-3 && getXYOffset_P151L10() )
		{
			SetMacroVal( g_CIAddress, MACRO_P151L10_FLAG, 0. );
			qDebug() << "G100P151L10 flag reset!";
		}

		GetMacroVal( g_CIAddress, MACRO_P151L11_FLAG, flagG100P151L11 );
		if( fabs( flagG100P151L11 - 1. ) < 10e-3 && getXYOffset_P151L11() )
		{
			SetMacroVal( g_CIAddress, MACRO_P151L11_FLAG, 0. );
			qDebug() << "G100P151L11 flag reset!";
		}

		GetMacroVal( g_CIAddress, MACRO_P151L21_FLAG, flagG100P151L21 );
		if( fabs( flagG100P151L21 - 1. ) < 10e-3 && getZOffset_P151L21() )
		{
			SetMacroVal( g_CIAddress, MACRO_P151L21_FLAG, 0. );
			qDebug() << "G100P151L21 flag reset!";
		}

		GetMacroVal( g_CIAddress, MACRO_P151L60_FLAG, flagG100P151L60 );
		if( fabs( flagG100P151L60 - 1. ) < 10e-3 && rmOffXY_P151L60() )
		{
			SetMacroVal( g_CIAddress, MACRO_P151L60_FLAG, 0. );
			qDebug() << "G100P151L60 flag reset!";
		}

		GetMacroVal( g_CIAddress, MACRO_P151L61_FLAG, flagG100P151L61 );
		if( fabs( flagG100P151L61 - 1. ) < 10e-3 && rmOffZ_P151L61() )
		{
			SetMacroVal( g_CIAddress, MACRO_P151L61_FLAG, 0. );
			qDebug() << "G100P151L61 flag reset!";
		}

		GetMacroVal( g_CIAddress, MACRO_P155L31_FLAG, flagG100P155L31 );
		if( fabs( flagG100P155L31 - 1. ) < 10e-3 && getDxfDataFromFile_P155L31() )
		{
			SetMacroVal( g_CIAddress, MACRO_P155L31_FLAG, 0. );
			qDebug() << "G100P155L31 flag reset!";
		}

		GetMacroVal( g_CIAddress, MACRO_P152L11_FLAG, flagG100P152L11 );
		if( fabs( flagG100P152L11 - 1. ) < 10e-3 && genNewNC_P152L11() )
		{
			SetMacroVal( g_CIAddress, MACRO_P152L11_FLAG, 0. );
			qDebug() << "G100P152L11 flag reset!";
		}



		boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();  
		//std::string strTime = boost::posix_time::to_iso_string(ptEnd-ptStart);  
		//std::cout << strTime << "s" << std::endl;  
	}
#endif
}

void DemoCad::showEvent(QShowEvent * event)
{
#ifndef Q_OS_WIN
	QString g100FilePath = "/home/Lynuc/Users/Motion/O0100.NC";
	QFile file( g100FilePath );
	if( !file.exists() )
	{
		qDebug() << "liuyc: O0100.NC is not exist!";
		m_ErrorBox.showErrorMessage( 10000, 1 );
		this->setEnabled( false );
		if( m_AlwTimerId != 0 )
		{
			killTimer( m_AlwTimerId );
			m_AlwTimerId = 0;
			SetMacroVal( g_CIAddress, MACRO_CAD_START_SUC, 0. );
		}
	}
	else
	{
		this->setEnabled( true );
		//< ������ʱ�������G100������к����
		if( m_AlwTimerId == 0 )
		{
			m_AlwTimerId = startTimer( 200 );
			SetMacroVal( g_CIAddress, MACRO_CAD_START_SUC, 1. );
		}
	}
#endif

	QMainWindow::showEvent( event );
}

void DemoCad::closeEvent(QCloseEvent * event)
{
#ifdef __arm__
	if( m_HideLabelN3 != NULL )
	{
        delete m_HideLabelN3;
		m_HideLabelN3 = NULL;
	}
#endif
	QMainWindow::closeEvent( event );
}


void DemoCad::newFile()
{
	RS_Document *doc = getDocument();

	if (doc->isModified())
	{
		QString fileName;
		ErrorMessage errorMessage(this);
		int nRtn = -1;

		if (getDocument()->getFilename().isEmpty())
		{
			fileName = getDocument()->getAutoSaveFilename();
			nRtn     = errorMessage.showErrorMessage(10007, 0, fileName);
		}
		else
		{
			fileName = getDocument()->getFilename();
			nRtn     = errorMessage.showErrorMessage(10006, 0, fileName);
		}

		switch (nRtn)
		{
		case 2:			// �����ļ�
			if (!saveFile())
			{
				return;
			}
			break;
		case 3:			// �������ļ���ֱ�Ӵ��ļ�
			break;
		case 4:			// ȡ����ǰ����
			return;
		default:
			break;
		}

	}

	doc->newDoc();

}

void DemoCad::WindowResize(int screenx, int screeny, int realx, int realy)
{
	int nx = 0;
	int ny = 0;
	int nw = 0;
	int nh = 0;

	ReSizeCsCal(this->x(),this->y(),this->width(),
	this->height(),&nx, &ny, &nw, &nh, screenx, screeny, realx, realy,1,0);
	this->setGeometry(nx,ny,nw,nh);
}

void DemoCad::clearData_L10()
{
	g_InputMode = motion_input;
	g_SavedData.clear();
}

bool DemoCad::clearDataAccIndex_L11()
{
	g_InputMode = motion_input;

	double index = 0.;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_INDEX, index );
#endif
	unsigned short nIndex = static_cast<unsigned short>( index + 10e-3 );
	//< ���ж������INDEX�Ƿ�Ϸ����ұ�������������޴�INDEX
	if( !G100IndexIsLegal( nIndex ) )
	{
		qDebug() << "liuyc: index input ilegal!";
		return false;
	}

	if( !g_SavedData.contains( nIndex ) )
	{
		qDebug() << "liuyc: has no data!";
		return false;
	}

	g_SavedData.remove( nIndex );
	return true ;
}

bool DemoCad::clearSaveralData_L12()
{
	g_InputMode = motion_input;

	double indexBegin = 0.;
	double indexEnd   = 0.;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_P150L12_F, indexBegin );
	GetMacroVal( g_CIAddress, MACRO_P150L12_T, indexEnd );
#endif
	unsigned short nIndexBegin = static_cast<unsigned short>( indexBegin + 10e-3 );
	unsigned short nIndexEnd   = static_cast<unsigned short>( indexEnd + 10e-3 );

	if( !G100IndexIsLegal(nIndexEnd) || !G100IndexIsLegal(nIndexBegin) || nIndexBegin > nIndexEnd )
	{
	    qDebug() << "liuyc: index ilegal!";
	}

	for( unsigned short i = nIndexBegin; i <= indexEnd; ++i )
	{
		if( !g_SavedData.contains( i ) )
			continue;

		g_SavedData.remove( i );
	}

	return true ;
}

bool DemoCad::saveDataAccIndex_L25()
{
	g_InputMode = motion_input;

	double index = 0.;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_INDEX, index );
#endif
	unsigned short nIndex = static_cast<unsigned short>( index + 10e-3 );
	//< ���ж������INDEX�Ƿ�Ϸ����ұ�������������޴�INDEX
	if( !G100IndexIsLegal( nIndex ) )
	{
		qDebug() << "liuyc: index input ilegal!";
		return false;
	}

	//< ��ӦINDEX����д�뵽QMAP����ȥ
	G100RWData( G100WRITE, nIndex );

	return true;
}

bool DemoCad::saveDataAccOrder_L22()
{
	g_InputMode = motion_input;

	//qDebug() << "liuyc L22 start!";
	double index = 0.;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_P150L22_S, index );
#endif
	unsigned short nIndex = static_cast<unsigned short>( index + 10e-3 );
	//< ���ж������INDEX�Ƿ�Ϸ����ұ�������������޴�INDEX
	if( !G100IndexIsLegal( nIndex ) )
	{
		qDebug() << "liuyc: index input ilegal!";
		return false;
	}

	//< ��ӦINDEX����д�뵽QMAP����ȥ
	G100RWData( G100WRITE, nIndex );

    //qDebug() << "liuyc L22 finished!";
	return true;
}

bool DemoCad::getDataAccIndex_L35()
{
	g_InputMode = motion_input;

	double index = 0.;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_INDEX, index );
#endif
	unsigned short nIndex = static_cast<unsigned short>( index + 10e-3 );
	//< ���ж������INDEX�Ƿ�Ϸ����ұ�������������޴�INDEX
	if( !G100IndexIsLegal( nIndex ) )
	{
		qDebug() << "liuyc: index input ilegal!";
		return false;
	}

	//< 
	G100RWData( G100READ, nIndex );
    
	//< end
	return true;
}

bool DemoCad::getDataAccOrder_L32()
{
	g_InputMode = motion_input;

	double index = 0.;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_P150L32_S, index );
#endif
	unsigned short nIndex = static_cast<unsigned short>( index + 10e-3 );
	//< ���ж������INDEX�Ƿ�Ϸ����ұ�������������޴�INDEX
	if( !G100IndexIsLegal( nIndex ) )
	{
		qDebug() << "liuyc: index input ilegal!";
		return false;
	}

	//< 
	G100RWData( G100READ, nIndex );

	//< end
	return true;
}

bool DemoCad::getAngleOff_P151L51()
{
	g_InputMode = motion_input;

	double beginIndex = 0.;
	double endIndex = 0.;
	double baseAngle = 0.;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_P151L51_I, beginIndex );
	GetMacroVal( g_CIAddress, MACRO_P151L51_J, endIndex );
	GetMacroVal( g_CIAddress, MACRO_P151L51_K, baseAngle );
#endif
	int nBIndex = static_cast<int>( beginIndex + 10e-3 );
	int nEIndex = static_cast<int>( endIndex + 10e-3 );

	for( int i = nBIndex; i < nEIndex; ++i )
	{
		if( !g_SavedData.contains( i ) )
			return false;
	}
	int lineFlag = 0;
	if( fabs(baseAngle) > 89 )  //< 90�ȶ�ӦLINEY
		lineFlag = 1;

	double rad = getAngleOfLine( nBIndex, nEIndex, lineFlag );  //< ����
	double angle = rad / M_PI * 180.;                           //< �Ƕ�
	qDebug() << "liuyc rad offset = " << rad;
	qDebug() << "liuyc angle offset = " << angle;

#ifndef Q_OS_WIN
	SetMacroVal( g_CIAddress, MACRO_RAD_OFF, rad );
	SetMacroVal( g_CIAddress, MACRO_ANGLE_OFF, angle );
#endif

	return true;
}

bool DemoCad::getXYOffset_P151L11()
{
	g_InputMode = motion_input;

	if( !getXYOffset_P151L10() )
		return false;

	enum
	{
		L11_all,
		L11_left,
		L11_right,
		L11_top,
		L11_btm,
		L11_leftAndRight,
		L11_topAndBtm
	};

	double d_kData = 0.;
	int    n_kData = 0;
	double angle   = 0.;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_P151L11_K, d_kData );
#endif
	n_kData = static_cast<int>( d_kData + 10e-3 );

	double indexArray[8] = { 0. };
	int    n_indexAry[8] = { 0 };
	for( int i = 0; i < 8; ++i )
	{
#ifndef Q_OS_WIN
		GetMacroVal( g_CIAddress, MACRO_P151L10L11_I + i, indexArray[i]);
#endif
		n_indexAry[i] = static_cast<int>( indexArray[i] + 10e-3 );
	}

	//< G100 MOTION���Ա�֤��Ӧ�ı��һ��������(I,J,A,B...)�����ǲ��ܱ�֤���б�Ŷ���Ӧ�б�������
	switch( n_kData )
	{
	case L11_all:
		//qDebug() << "l11 all!";
		angle = getAngleOffOfAll( n_indexAry, 8 );
		break;
	case L11_left:
		//qDebug() << "l11 left!";
		angle = getAngleOfLine( n_indexAry[XY_MIN_LEFT], n_indexAry[XY_MAX_LEFT], LineY );
		break;
	case L11_right:
		//qDebug() << "l11 right!";
		angle = getAngleOfLine( n_indexAry[XY_MIN_RIGHT], n_indexAry[XY_MAX_RIGHT], LineY );
		break;
	case L11_top:
		angle = getAngleOfLine( n_indexAry[XY_MIN_TOP], n_indexAry[XY_MAX_TOP], LineX );
		break;
	case L11_btm:
		angle = getAngleOfLine( n_indexAry[XY_MIN_BTM], n_indexAry[XY_MAX_BTM], LineX );
		break;
	case L11_leftAndRight:
		angle = 0.;
		angle += getAngleOfLine( n_indexAry[XY_MIN_LEFT], n_indexAry[XY_MAX_LEFT], LineY );
		angle += getAngleOfLine( n_indexAry[XY_MIN_RIGHT], n_indexAry[XY_MAX_RIGHT], LineY );
		angle /= 2.;
		break;
	case L11_topAndBtm:
		angle = 0.;
		angle += getAngleOfLine( n_indexAry[XY_MIN_TOP], n_indexAry[XY_MAX_TOP], LineX );
		angle += getAngleOfLine( n_indexAry[XY_MIN_BTM], n_indexAry[XY_MAX_BTM], LineX );
		angle /= 2.;
		break;
	default:
		break;
	}

	g_CalRadOff = angle;  //< ʵ�����ǻ���
	double actualAngle = angle / M_PI * 180.;
	qDebug() << "angle offset = " << QString::number( angle, 'd', 6 );
#ifndef Q_OS_WIN
	SetMacroVal( g_CIAddress, MACRO_RAD_OFF, angle );
	SetMacroVal( g_CIAddress, MACRO_ANGLE_OFF, actualAngle );
#endif

	return true;
}

bool DemoCad::getZOffset_P151L21()
{
	g_InputMode = motion_input;

	int tmpBegin = getMacroInt( MACRO_P151L21_I );
	int tmpEnd   = getMacroInt( MACRO_P151L21_J );
	int tmpCount = tmpEnd - tmpBegin + 1;
	double d_zoff = 0.;


	for( int i = tmpBegin; i <= tmpEnd; ++i )
	{
		if( !g_SavedData.contains( i ))
		{
#ifndef Q_OS_WIN
			SetMacroVal( g_CIAddress, MACRO_P151L10_INDEXWRONG, 1. );
#endif
			return false;
		}

		d_zoff += g_SavedData[i].z;
	}

	d_zoff /= static_cast<double>( tmpCount );
#ifndef Q_OS_WIN
	SetMacroVal( g_CIAddress, MACRO_Z_GET_DATA, d_zoff );
#endif

	return true;
}


bool DemoCad::rmOffXY_P151L60()
{
	g_InputMode = motion_input;

	int tmpBegin = getMacroInt( MACRO_P151L60_I );
	int tmpEnd   = getMacroInt( MACRO_P151L60_J );

    if( tmpEnd < tmpBegin )
		return false;
    for( int i = tmpBegin; i <= tmpEnd; ++i )
	{
		if( !g_SavedData.contains( i ) )
		{
			qDebug() << "liuyc input index ilegal!";
#ifndef Q_OS_WIN
			SetMacroVal( g_CIAddress, MACRO_P151L10_INDEXWRONG, 1. );
#endif
			return false;
		}
	}
	g_XYSrcDataBeginNum = tmpBegin;
	g_XYSrcDataCount  = tmpEnd - tmpBegin + 1;
	g_XYSrcDataEndNum = tmpEnd;

	for( int i = g_XYSrcDataBeginNum; i <= g_XYSrcDataEndNum; ++i )
	{
		cout << "liuyc xy det  point: " << g_SavedData[i] << endl;
	}

	if( !m_OffsetWdt->isIndexLegal( RM_Axis_XY, motion_input ) )
		return false;

	double d_xoff = 0.;
	double d_yoff = 0.;
	double d_angleoff = 0.;
#ifndef Q_OS_WIN 
	GetMacroVal( g_CIAddress, MACRO_X_DATA, d_xoff );
	GetMacroVal( g_CIAddress, MACRO_Y_DATA, d_yoff );
	GetMacroVal( g_CIAddress, MACRO_P151L60_A, d_angleoff );
#endif

	m_OffsetWdt->rmOffsetXY( d_xoff, d_yoff, d_angleoff );
	return true;
}


bool DemoCad::rmOffZ_P151L61()
{
	g_InputMode = motion_input;

	//< step1 ��ȡI,J��ֵ������ֵ��ȫ�ֵ�Z����index
	int tmpBegin = getMacroInt( MACRO_P151L60_I );
	int tmpEnd   = getMacroInt( MACRO_P151L60_J );

	if( tmpEnd < tmpBegin )
		return false;
	for( int i = tmpBegin; i <= tmpEnd; ++i )
	{
		if( !g_SavedData.contains( i ) )
		{
			qDebug() << "liuyc input index ilegal!";
#ifndef Q_OS_WIN
			SetMacroVal( g_CIAddress, MACRO_P151L10_INDEXWRONG, 1. );
#endif
			return false;
		}
	}
	g_Index[Z_MIN] = g_ZSrcDataBeginNum = tmpBegin;
	g_Index[Z_MAX] = g_ZSrcDataEndNum   = tmpEnd;
	g_ZDataCount   = tmpEnd - tmpBegin + 1;

	//< 
	if( !m_OffsetWdt->isIndexLegal( RM_Axis_Z, motion_input ) )
		return false;

	//< 
	double d_zoff = 0.;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_Z_DATA, d_zoff );
#endif
	qDebug() << "liuyc: z_off = " << d_zoff;
	m_OffsetWdt->rmOffsetZ( d_zoff );

	return true;
}


bool DemoCad::genNewNC_P152L11()
{
	g_InputMode = motion_input;

	//< step1 ��ȡԭʼNC�ļ�������NC�ļ���
	double d_L11I = 0.;
	double d_L11J = 0.;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_P152L11_I, d_L11I);
	GetMacroVal( g_CIAddress, MACRO_P152L11_J, d_L11J);
#endif
	int n_L11I = static_cast<int>( d_L11I + 10e-3 );
	int n_L11J = static_cast<int>( d_L11J + 10e-3 );
	if( n_L11I < 0 ||  n_L11J < 0 )
	{
		qDebug() << "liuyc: generate new file_ index is less than 0!";
		return false;
	}

	QString srcFileName = QString( NCFilePath ) + "O" + QString::number( n_L11I ) + ".NC"; // eg: "..../NCFiles/O110.NC"
	QString newFileName = QString( NCFilePath ) + "O" + QString::number( n_L11J ) + ".NC"; // eg: "..../NCFiles/O110.NC"
    QFile srcFile( srcFileName );
	if( !srcFile.exists() )
	{
		qDebug() << "liuyc: src nc is not found!";
		return false;
	}

	//< step2 �������ֻ�ȡNC�ļ�����������NC��λ��Ϣ
	m_OffsetWdt->openSrcNcFile( srcFileName );

	//< step3 ��ȡ�ֶ�����������ֵ
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_X_DATA, g_ManOff[0] );
	GetMacroVal( g_CIAddress, MACRO_Y_DATA, g_ManOff[1] );
	GetMacroVal( g_CIAddress, MACRO_Z_DATA, g_ManOff[2] );
	GetMacroVal( g_CIAddress, MACRO_P152_DET_RAD, g_DecRadius);
#endif

	//< step4 �����µ�NC
	m_OffsetWdt->genNewNC( newFileName, motion_input );
	
	return true;
}


double DemoCad::getAngleOffOfAll( int * indexary, int size )
{
	int dirSum = 0;
	double angle = 0.;

	if( indexary[XY_MIN_LEFT] != 0 && indexary[XY_MAX_LEFT] != 0 )
	{
		dirSum++;
		angle += getAngleOfLine( indexary[XY_MIN_LEFT], indexary[XY_MAX_LEFT], LineY );
		//qDebug() << "angle = " << angle;
	}
	if( indexary[XY_MIN_RIGHT] != 0 && indexary[XY_MAX_RIGHT] != 0 )
	{
		dirSum++;
		angle += getAngleOfLine( indexary[XY_MIN_RIGHT], indexary[XY_MAX_RIGHT], LineY );
		//qDebug() << "angle = " << angle;
	}
	if( indexary[XY_MAX_TOP] != 0 && indexary[XY_MIN_TOP] != 0 )
	{
		dirSum++;
		angle += getAngleOfLine( indexary[XY_MIN_TOP], indexary[XY_MAX_TOP], LineX );
		//qDebug() << "angle = " << angle;
	}
	if( indexary[XY_MIN_BTM] != 0 && indexary[XY_MAX_BTM] != 0 )
	{
		dirSum++;
		angle += getAngleOfLine( indexary[XY_MIN_BTM], indexary[XY_MAX_BTM], LineX );
		//qDebug() << "angle = " << angle;
	}
	//qDebug() << "liuyc: dir sum = " << dirSum;

	if( dirSum == 0 )
		return 0.;
	else
		return ( angle / static_cast<double>( dirSum ) );
}

bool DemoCad::getDxfDataFromFile_P155L31()
{
	g_InputMode = motion_input;

	double d_l31q = 0.;
	int    n_l31q = 0;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, MACRO_P155L31_Q, d_l31q );
#endif
	n_l31q = static_cast<int>( d_l31q + 10e-3 );
	if( n_l31q <= 0 || n_l31q > 20 )  //< Q�ķ�Χ1-20
		return false;

	QString filename = QString( NCFilePath ) + QString::number( n_l31q ) + ".dxf";
	QFile file( filename );
	if( !file.exists() )
	{
		qDebug() << "liuyc: dxf file is not found!";
		return false;
	}

	return openFile( filename );
}

bool DemoCad::getXYOffset_P151L10()
{
	g_InputMode = motion_input;

	enum 
	{
		X,
		Y,
		TMP_COUNT
	};

	double indexArray[8] = {0.};
	int    n_indexAry[8] = {0};
	double d_calOff[TMP_COUNT] = {0.};
	bool   b_needToCal[TMP_COUNT] = { true, true };
	for( int i = XY_MIN_LEFT; i <= XY_MAX_BTM; ++i )
	{
#ifndef Q_OS_WIN
		GetMacroVal( g_CIAddress, MACRO_P151L10L11_I + i, indexArray[i]);
#endif
		n_indexAry[i] = static_cast<int>( indexArray[i] + 10e-3 );
		g_Index[i]    = n_indexAry[i];
	}

	if( !m_ParaSetWdt->isInputLegel( motion_input ) )
	{
		qDebug() << "liuyc: index input in motion is ilegal!";
#ifndef Q_OS_WIN
		SetMacroVal( g_CIAddress, MACRO_P151L10_INDEXWRONG, 1. );
#endif
		return false;
	}

	//< ����X��ƫ����
	double tmp = 0.;
	double tmp2 = 0.;
	for( int i = XY_MIN_LEFT; i <= XY_MAX_RIGHT; ++i )
	{
		if( n_indexAry[i] == 0 )
		{
			d_calOff[X] = 0.;
			b_needToCal[X] = false;  //< x�Ჹ������Ҫ�ټ���
			break;
		}
	}
	if( b_needToCal[X] )
	{
		for( int i = n_indexAry[XY_MIN_LEFT]; i <= n_indexAry[XY_MAX_LEFT]; ++i )
		{
			tmp += g_SavedData[i].x;
		}
		tmp /= static_cast<double>( n_indexAry[XY_MAX_LEFT] - n_indexAry[XY_MIN_LEFT] + 1 );
		for( int i = n_indexAry[XY_MIN_RIGHT]; i <= n_indexAry[XY_MAX_RIGHT]; ++i )
		{
			tmp2 += g_SavedData[i].x;
		}
		tmp2 /= static_cast<double>( n_indexAry[XY_MAX_RIGHT] - n_indexAry[XY_MIN_RIGHT] + 1 );

		d_calOff[X] = ( tmp + tmp2 ) * 0.5;
	}
	qDebug() << "liuyc x offset = " << QString::number( d_calOff[X], 'd', 6);

	//< ����Y��ƫ����
	tmp = tmp2 = 0.;
	for( int i = XY_MIN_TOP; i <= XY_MAX_BTM; ++i )
	{
		if( n_indexAry[i] == 0 )
		{
			d_calOff[Y] = 0.;
			b_needToCal[Y] = false;
			break;
		}
	}
	if( b_needToCal[Y] )
	{
		for( int i = n_indexAry[XY_MIN_BTM]; i <= n_indexAry[XY_MAX_BTM]; ++i )  //< 2016.08.27 �Ϸ��ֳ�����BUG��������
		{
			tmp += g_SavedData[i].y;
		}
		tmp /= static_cast<double>( n_indexAry[XY_MAX_BTM] - n_indexAry[XY_MIN_BTM] + 1 );
		for( int i = n_indexAry[XY_MIN_TOP]; i <= n_indexAry[XY_MAX_TOP]; ++i )
		{
			tmp2 += g_SavedData[i].y;
		}
		tmp2 /= static_cast<double>( n_indexAry[XY_MAX_TOP] - n_indexAry[XY_MIN_TOP] + 1 );

		d_calOff[Y] = ( tmp + tmp2 ) * 0.5;
	}
	qDebug() << "liuyc y offset = " << QString::number( d_calOff[Y], 'd', 6);

#ifndef Q_OS_WIN
    SetMacroVal( g_CIAddress, MACRO_X_GET_DATA, d_calOff[X]);
	SetMacroVal( g_CIAddress, MACRO_Y_GET_DATA, d_calOff[Y]);
#endif

	return true;
}


void DemoCad::G100RWData( int flag, int index )
{
	double xData = 0.;
	double yData = 0.;
	double zData = 0.;

	if( flag == G100WRITE )
	{
#ifndef Q_OS_WIN
		GetMacroVal( g_CIAddress, MACRO_X_DATA, xData );
		GetMacroVal( g_CIAddress, MACRO_Y_DATA, yData );
		GetMacroVal( g_CIAddress, MACRO_Z_DATA, zData );
#endif
		RS_Vector vec( xData, yData, zData );
		//< ��������и������ݣ�����ɾ��
		if( g_SavedData.contains( index ))
			g_SavedData.remove( index );

		//qDebug() << xData << " " << yData << " " << zData;
		g_SavedData.insert( index, vec );
	}
	else if( flag == G100READ )
	{
		if( g_SavedData.contains( index ))
		{
			xData = g_SavedData[index].x;
			yData = g_SavedData[index].y;
			zData = g_SavedData[index].z;
		}
#ifndef Q_OS_WIN
        SetMacroVal( g_CIAddress, MACRO_X_GET_DATA, xData );
		SetMacroVal( g_CIAddress, MACRO_Y_GET_DATA, yData );
		SetMacroVal( g_CIAddress, MACRO_Z_GET_DATA, zData );
#endif
	}
}

bool DemoCad::G100IndexIsLegal( int index )
{
	return ( index >= G100_INDEX_MIN && index <= G100_INDEX_MAX );
}

/************************************************************************/
/* ������Ϣ��ʾ���������Ͻǵ���ʾ��
/************************************************************************/
void DemoCad::OperationPromptInfo(QString qstrInfo)			// ������Ϣ��ʾ
{
	DIALOGMEDIATOR->OperationPromptInfo(qstrInfo);
}