#include "offsetwdt.h"
#include <QDebug>
#include <QFileDialog>
#include <QValidator>
#include <algorithm>
#include <cstdio>
#include <cctype>
#include <list>

#define arraydelete(a) delete [] a; \
	                   a = NULL
#define xdelete(a)     delete a; \
	                   a = NULL;

/************************************************************************/
/* OffsetWdt constructor
/* parameter init
/* UI init
/************************************************************************/
OffsetWdt::OffsetWdt(QWidget *parent)
	: QDockWidget(parent)
	, m_NewNCFile( "" )
	, m_SrcNCFile( "" )
	, m_dxfFilename( "" )
	, m_SrcContent( "" )
	, m_NCPoints( NULL )
	, m_lNCLineCount( 0 )
	, m_lPointCount( 0 )
	, m_nSrcNCPntFirstLine( 0 )
	, m_dNCFileROff( -1. )
	, m_nSrcNCPntLastLine( 0 )
	, m_dNCFileFspd( 0. )
	, m_ncLineNumXY( NULL )
	, m_ncLineNumZ( NULL )
	, m_NewNCPntDlg( NULL )
	, m_bHasRmXY( true )
	, m_bHasRmZ( true )
	, m_nCutFirstPointIndex( -1 )
	, m_nCutLastPointIndex( -1 )
	, m_lActFstOffPnt( -1 )
	, m_lActLstOffPnt( -1 )
	, m_bIsClose( false )
{
	setMinimumSize( 300, 10 );
	setFont( FONT_10_SIMHEI_LIGHT );

	QRegExp regExp("^(500|[1-4]?\\d?\\d?)$");  //< �������ֵ500
	QRegExpValidator *pReg = new QRegExpValidator(regExp, this); 
	QRegExp regExp2("^(\\-?)(2(\\.[0]{0,4})?|([0-1]?)(\\.\\d{0,3})?)$");  //< ���ֵ2.0000
	QRegExpValidator *pReg2 = new QRegExpValidator(regExp2, this); 

#ifdef TEST_SCROLL
	m_Area = new QScrollArea( this );
	m_WholeWdt = new QWidget( m_Area );
	m_Area->setWidget( m_WholeWdt );
#endif
	//< �յı���ؼ�
	m_TitleWdt = new QWidget( this );
	setTitleBarWidget( m_TitleWdt );
	//< ��ʾ
	m_HelpLabel = new QLabel( this );
	m_HelpLabel->setText( tr("�밴����ʾ������в������������ã�") );
	m_HelpLabel->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

	//< ���������GroupBox
	for ( int i = 0; i < OFF_GRP_COUNT; ++i )
	{
		m_GrpBox[i] = new QGroupBox( this );
		m_Layout[i] = new QGridLayout( m_GrpBox[i] );
	}
	m_GrpBox[RM_OFFSET]->setTitle( tr("1. ����ƫ��") );
	m_GrpBox[READ_DXF]->setTitle( tr("2. ��ȡDXF") );
	m_GrpBox[MANUAL]->setTitle( tr("3. �ֶ�����") );
	m_GrpBox[NEW_NC]->setTitle( tr("4. ������NC") );

	//< �Ƴ�ƫ����ģ��
	m_RmHlpLbl = new QLabel( this );
	m_RmHlpLbl->setWordWrap( true );
    m_RmHlpLbl->setText( tr("������������ƫ������浽�µ�һ�������ȥ���Ա��������㣺") );
	m_Layout[RM_OFFSET]->addWidget( m_RmHlpLbl, 0, 0, 1, 4 );
	for( int i = 0; i < RM_Axis_Count; ++i )
	{
		m_RmAxisLbl[i] = new QLabel( this );
		m_Layout[RM_OFFSET]->addWidget( m_RmAxisLbl[i], i+1, 0, 1, 1);
		m_RmPntCntLbl[i] = new QLabel( this );
		m_Layout[RM_OFFSET]->addWidget( m_RmPntCntLbl[i], i+1, 2, 1, 1);
		m_RmPntCntLbl[i]->setText( tr("����: 0 ") );

		m_RmBtn[i] = new RS_CustomBtn( this );
		m_RmBtn[i]->setFocusPolicy( Qt::NoFocus );
		m_Layout[RM_OFFSET]->addWidget(m_RmBtn[i], i+1, 3, 1, 1 );
		m_RmBtn[i]->setText( tr("����") );
		m_RmBtn[i]->setEnabled( false );
		m_RmBtn[i]->setMaximumWidth( 60 );
	}
	m_RmAxisLbl[RM_Axis_XY]->setText( tr("xy��ʼ������") );
	m_RmAxisLbl[RM_Axis_Z]->setText( tr("z ��ʼ������") );
	connect( m_RmBtn[RM_Axis_XY], SIGNAL( clicked() ), this, SLOT( slotRemoveOffsetXY() ));
	connect( m_RmBtn[RM_Axis_Z],  SIGNAL( clicked() ), this, SLOT( slotRemoveOffsetZ() ));

	for( int i = 0; i < RM_Axis_Count; ++i )
	{
		m_RmMacroLEdit[i] = new QLineEdit( this );
		m_RmMacroLEdit[i]->setEnabled( false );
		m_RmMacroLEdit[i]->setValidator( pReg );
		m_RmMacroLEdit[i]->setAlignment( Qt::AlignCenter );
		m_Layout[RM_OFFSET]->addWidget( m_RmMacroLEdit[i], i+1, 1, 1, 1);
	}
	connect( m_RmMacroLEdit[RM_Axis_XY], SIGNAL( returnPressed() ), this, SLOT( slotRemoveOffsetXY() ));
	connect( m_RmMacroLEdit[RM_Axis_Z],  SIGNAL( returnPressed() ), this, SLOT( slotRemoveOffsetZ() ));

	//< ��ȡDXFģ��
	m_ReadHlpLbl = new QLabel( this );
	m_ReadHlpLbl->setWordWrap( true );
	m_ReadHlpLbl->setText( tr("������ߡ����ļ�����ť��DXF�ļ�:") );
	m_Layout[READ_DXF]->addWidget( m_ReadHlpLbl, 0, 0, 1, 1 );
	m_ReadFnameLbl = new QLabel( this );
	m_ReadFnameLbl->setWordWrap( true );
	m_ReadFnameLbl->setText( tr("��ǰ�ļ�����...") );
	m_Layout[READ_DXF]->addWidget( m_ReadFnameLbl, 1, 0, 1, 1 );

	//< �ֶ�����ģ��
	m_ManHlpLbl = new QLabel( this ); 
	m_ManHlpLbl->setWordWrap( true );
	m_ManHlpLbl->setText( tr("���������NC�ӹ�����һЩ���⣬�ɳ����ֶ�ƫ�ƻ����Ų����ķ�ʽ����λmm��") );
	m_Layout[MANUAL]->addWidget( m_ManHlpLbl, 0, 0, 1, 5);
	m_ManTipLbl = new QLabel( this );
	m_ManTipLbl->setText( tr("���Ჹ����") );
	m_Layout[MANUAL]->addWidget( m_ManTipLbl, 1, 0, 1, 1);
	m_ManZoomLbl = new QLabel( this );
	m_ManZoomLbl->setText( tr("xy������") );
	m_Layout[MANUAL]->addWidget( m_ManZoomLbl, 2, 0, 1, 1);
	for( int i = 0; i < 3; ++i )
	{
		m_ManOffLEdit[i] = new QLineEdit( this );
		m_ManOffLEdit[i]->setText( QString::number( 0., 'f', 3) );
		m_ManOffLEdit[i]->setValidator( pReg2 );
		m_ManOffLEdit[i]->setAlignment( Qt::AlignVCenter | Qt::AlignRight );
		m_Layout[MANUAL]->addWidget( m_ManOffLEdit[i], 1, i+1, 1, 1 );
		connect( m_ManOffLEdit[i], SIGNAL(editingFinished()), this, SLOT(slotFormatManualOff()) );
	}
	for( int i = 0; i < ZOOM_DIR_COUNT; ++i )
	{
		m_ManOffZoom[i] = new QLineEdit( this );
		m_ManOffZoom[i]->setText( QString::number( 0., 'f', 3) );
		m_ManOffZoom[i]->setValidator( pReg2 );
		m_ManOffZoom[i]->setAlignment( Qt::AlignVCenter | Qt::AlignRight );
		m_Layout[MANUAL]->addWidget( m_ManOffZoom[i], 2, i+1, 1, 1 );
		connect( m_ManOffZoom[i], SIGNAL(editingFinished()), this, SLOT(slotFormatManualOff()) );
	}

	//< �����µ�NC�ļ�
	m_NewHlpLbl = new QLabel( this );
	m_NewHlpLbl->setWordWrap( true );
	m_NewHlpLbl->setText( tr("�������ѡ��ԭNC�ļ�����������ɡ���ȴ����ɲ�����NC��") );
	m_Layout[NEW_NC]->addWidget( m_NewHlpLbl, 0, 0, 1, 4 );
	for( int i = 0; i < FILE_COUNT; ++i )
	{
		m_NewFTtlLbl[i] = new QLabel( this );
		m_Layout[NEW_NC]->addWidget( m_NewFTtlLbl[i], i+1, 0, 1, 1 );
		m_NewFNameLbl[i] = new QLabel( this );
		m_Layout[NEW_NC]->addWidget(m_NewFNameLbl[i], i+1, 1, 1, 2 );
		m_NewBtn[i] = new RS_CustomBtn( this );
		m_Layout[NEW_NC]->addWidget(m_NewBtn[i], i+1, 3, 1, 1 );
		m_NewBtn[i]->setFocusPolicy( Qt::NoFocus );
	}
	m_NewFTtlLbl[FILE_SRC]->setText(tr("ԭNC��"));
	m_NewFTtlLbl[FILE_NEW]->setText(tr("��NC��"));
	m_NewBtn[FILE_SRC]->setText( tr("���...") );
	m_NewBtn[FILE_NEW]->setText( tr("����") );
	m_NewBtn[FILE_NEW]->setEnabled( false );
	connect( m_NewBtn[FILE_SRC], SIGNAL(clicked()), this, SLOT(slotBrwSrcFile()));
	connect( m_NewBtn[FILE_NEW], SIGNAL(clicked()), this, SLOT(slotGenNewNC()));
}

/************************************************************************/
/* OffsetWdt Destructor
/* memory release and kill timer
/************************************************************************/
OffsetWdt::~OffsetWdt()
{
	//< Qt�Զ��������е��ӿؼ��������ӿؼ������ֶ�����
	if( m_NCPoints != NULL )
	{
		delete [] m_NCPoints;
		m_NCPoints = NULL;
	}

	m_lineNumToDataIndex.clear();

	if( m_ncLineNumXY != NULL )
	{
		delete [] m_ncLineNumXY;
		m_ncLineNumXY = NULL;
	}
	if( m_ncLineNumZ != NULL )
	{
		delete [] m_ncLineNumZ;
		m_ncLineNumZ = NULL;
	}
}

/************************************************************************/
/* OffsetWdt resizeEvent
/* set size and position of child-widgets
/************************************************************************/
void OffsetWdt::resizeEvent( QResizeEvent * event )
{
	float width  = this->width();
	float height = this->height();

#ifdef TEST_SCROLL
	m_Area->setGeometry( 0, 0 , static_cast<int>( width ), static_cast<int>( height ));
	m_WholeWdt->setGeometry( 0, 0, static_cast<int>( width ), static_cast<int>( height * 1.3 ));
#endif

	float fTopPer  = 0.01f;
	float fLeftPer = 0.02f;
	float fRightPer  = fLeftPer;
	float fHlpLblWth = 1.f - fRightPer - fLeftPer;
	float fHlpLblHgt = 0.05f;

	m_HelpLabel->setGeometry( static_cast<int>( width * fLeftPer ), static_cast<int>( height * fTopPer)
							, static_cast<int>( width * fHlpLblWth), static_cast<int>( height * fHlpLblHgt));
	
	float fVGapPer = 0.01f;
	float fGrpTopPer1 = fTopPer + fHlpLblHgt + fVGapPer;
	float fGrpHgt1 = 0.25f;
	m_GrpBox[RM_OFFSET]->setGeometry( static_cast<int>( width * fLeftPer ), static_cast<int>( height * fGrpTopPer1)
									, static_cast<int>( width * fHlpLblWth), static_cast<int>( height * fGrpHgt1 ));

	float fGrpTopPer2 = fGrpTopPer1 + fGrpHgt1 + fVGapPer;
	float fGrpHgt2 = 0.14f;
	m_GrpBox[READ_DXF]->setGeometry( static_cast<int>( width * fLeftPer ), static_cast<int>( height * fGrpTopPer2)
								   , static_cast<int>( width * fHlpLblWth), static_cast<int>( height * fGrpHgt2 ) );

	float fGrpTopPer3 = fGrpTopPer2 + fGrpHgt2 + fVGapPer;
	float fGrpHgt3 = 0.25f;
	m_GrpBox[MANUAL]->setGeometry( static_cast<int>( width * fLeftPer ), static_cast<int>( height * fGrpTopPer3)
								 , static_cast<int>( width * fHlpLblWth), static_cast<int>( height * fGrpHgt3 ));

	float fGrpTopPer4 = fGrpTopPer3 + fGrpHgt3 + fVGapPer;
	float fGrpHgt4 = 0.25f;
	m_GrpBox[NEW_NC]->setGeometry( static_cast<int>( width * fLeftPer ), static_cast<int>( height * fGrpTopPer4)
		, static_cast<int>( width * fHlpLblWth), static_cast<int>( height * fGrpHgt4 ));

	QDockWidget::resizeEvent( event );

}

void OffsetWdt::showEvent( QShowEvent * event )
{
	QDockWidget::showEvent( event );
}

/************************************************************************/
/* slot
/* browser source nc file and get source points
/************************************************************************/
void OffsetWdt::slotBrwSrcFile()
{
	qDebug() << "liuyc browser nc file !";

	g_InputMode = ui_input;

	m_SrcNCFile = QFileDialog::getOpenFileName( this, tr("���ļ�"), QString( NCFilePath ), ("NC (*.NC *.nc)") );
	if( m_SrcNCFile.isNull() || m_SrcNCFile.isEmpty() )
	{
		qDebug() << "liuyc has not seleced nc file !";
		return;
	}

	openSrcNcFile( m_SrcNCFile );

}

/************************************************************************/
/* �ۺ�����������NC
/************************************************************************/
void OffsetWdt::slotGenNewNC()
{
	qDebug() << "liuyc: start generatint nc file !*************\n";

	g_InputMode = ui_input;

	//< step0 �Ƿ������
	if( !isRdyToGen() )
		return;

	RS_Vector sizeVec = container->getMax() - container->getMin();
	sizeVec.x = fabs( sizeVec.x ) + g_nInOrOut * m_dNCFileROff * 2;
	sizeVec.y = fabs( sizeVec.y ) + g_nInOrOut * m_dNCFileROff * 2;

	//< step1 ��ȡ��������ֶ�����ֵ
	for( int i = 0; i < 3; ++i )
	{
		g_ManOff[i] = m_ManOffLEdit[i]->text().toDouble();
	}
	for( int i = 0; i < ZOOM_DIR_COUNT; ++i )
	{
		g_ManOffZoom[i] = m_ManOffZoom[i]->text().toDouble();
	}
	//< step2 �趨����ļ���
	m_NewNCFile = QFileDialog::getSaveFileName( this, tr("���ļ�"), QString( NCFilePath ), ("NC (*.NC *.nc)") );
	if( m_NewNCFile.isNull() || m_NewNCFile.isEmpty() )
	{
		qDebug() << "liuyc has not seleced nc file to generate !";
		return;
	}
	if( m_NewNCFile == m_SrcNCFile 
		&& QMessageBox::Cancel == QMessageBox::warning( this, "Lynuc CAD warning", tr("�µ�NC�ļ��Ḳ��ԭNC���Ƿ������  "), QMessageBox::Ok | QMessageBox::Cancel))
	{
		return;
	}

	//< step3 ������NC֮ǰ���¶�һ��Դ�ļ������µ�λ��Ϣ����Ϊ������������һ��NCʱǰһ�εĲ���δɾ��
	if( g_bIsNCRdy )
	    getSrcPoints( m_SrcNCFile );
	else
	{
		qDebug() << "liuyc : genrate again, NC is not ready!";
		return;
	}

	genNewNC( m_NewNCFile );
	
}

/************************************************************************/
/* ��ȡ��ǰ������ķ����������һ������ĸ��߽�
/************************************************************************/
XYZ_DIR OffsetWdt::getDetDir( int pointIndex )
{
	//qDebug() << __FUNCTION__;

	//< step1 ������ı��ת�Ƶ�̽��ԭʼ���ݵķ�Χ�У���������ֱ��ͨ�������������ж�̽��㷽��
	int tmpIndex = 0;
	XYZ_DIR dir = XY_BTM;
	if( pointIndex >= g_XYDesDataBeginNum && pointIndex <= g_XYDesDataEndNum )
		tmpIndex = pointIndex - g_XYDesDataBeginNum + g_XYSrcDataBeginNum;
	else if( pointIndex >= g_XYSrcDataBeginNum && pointIndex <= g_XYSrcDataEndNum )
		tmpIndex = pointIndex;
	else
	{
		qDebug() << __FUNCTION__ << " liuyc: index is ilegal ";
		return dir;
	}

	//< step2 ���ݱ����ȷ����ǰ���̽�뷽��
	if( tmpIndex >= g_XYIndex[_leftB] && tmpIndex <= g_XYIndex[_leftE] )
		dir = XY_LEFT;
	else if( tmpIndex >= g_XYIndex[_rightB] && tmpIndex <= g_XYIndex[_rightE] )
		dir = XY_RIGHT;
	else if( tmpIndex >= g_XYIndex[_topB] && tmpIndex <= g_XYIndex[_topE] )
		dir = XY_TOP;
	else if( tmpIndex >= g_XYIndex[_btmB] && tmpIndex <= g_XYIndex[_btmE] )
		dir = XY_BTM;
	else if( tmpIndex >= g_XYIndex[_rightTopB] && tmpIndex <= g_XYIndex[_rightTopE] )
		dir = XY_RIGHT_TOP;
	else if( tmpIndex >= g_XYIndex[_rightBtmB] && tmpIndex <= g_XYIndex[_rightBtmE] )
		dir = XY_RIGHT_BTM;
	else if( tmpIndex >= g_XYIndex[_leftTopB] && tmpIndex <= g_XYIndex[_leftTopE] )
		dir = XY_LEFT_TOP;
	else if( tmpIndex >= g_XYIndex[_leftBtmB] && tmpIndex <= g_XYIndex[_leftBtmE] )
		dir = XY_LEFT_BTM;
	    
	return dir;
}

void OffsetWdt::genNewNC( QString filename, int inputmd )
{
#ifdef Q_OS_LINUX
	timeval starttime, endtime;
	gettimeofday(&starttime, 0);
#endif

	if( g_XYDesDataCount != g_XYSrcDataCount )
	{
		qDebug() << " liuyc : counts not match! ";
#ifndef Q_OS_WIN
		if( inputmd == motion_input )
			SetMacroVal( g_CIAddress, MACRO_ALL_WRONG, 2. );  //< #231 == 2 ʱ������������ݸ�����ȥƫ�Ƶ����ݸ�������
		else
			popMessageBox( "Lynuc CAD Warning", tr("���ݸ�����ƥ�䣡"), 5, QColor(255,0,0), true );
#endif
		return;
	}

	//<  �������� �����µ�NC ��������
	//< step1.1 ����ƫ�Ƶ��ȥ̽��뾶��ͬʱ��ȡ��Ӧ��ʵ����Ϣ
	qDebug() << "\n\nliuyc step1.1...";
	if( g_XYDesDataCount <= 0 || g_XYSrcDataCount <= 0)
	{
#ifndef Q_OS_WIN
		if( inputmd == motion_input )
			SetMacroVal( g_CIAddress, MACRO_ALL_WRONG, 3. );
		else
			popMessageBox( "Lynuc CAD Warning", tr("ԭ���ݸ���Ϊ0��"), 5, QColor(255,0,0), true );
#endif
		return;
	}
	if( container == NULL )
	{
#ifndef Q_OS_WIN
		if( inputmd == motion_input )
			SetMacroVal( g_CIAddress, MACRO_ALL_WRONG, 4. );
		else
			popMessageBox( "Lynuc CAD Warning", tr("û������DXFͼ����"), 5, QColor(255,0,0), true );
#endif
		return;
	}
	RS_Vector * dataWthoutR = new RS_Vector[g_XYDesDataCount];  //< ����̽����뾶���̽������  checked leak
	int       * dataDir     = new int[g_XYDesDataCount];        //< �������ݶ�Ӧ�ķ���         checked leak
	for( int i = 0; i < g_XYDesDataCount; ++i )
	{
		dataWthoutR[i] = g_SavedData[i+g_XYDesDataBeginNum];  //< ����һ������
	}

	RS_Vector tmpNearestPnt;  
	int tmpIndex = 0;
	for( int i = g_XYDesDataBeginNum; i <= g_XYDesDataEndNum; ++i )
	{
		tmpIndex = i - g_XYDesDataBeginNum;

		tmpNearestPnt = container->getNearestPointOnEntity( g_SavedData.value(i) );
		tmpNearestPnt.z = g_SavedData.value(i).z;
		tmpNearestPnt = tmpNearestPnt - g_SavedData.value(i);

		dataDir[tmpIndex] = getDetDir( i );

		tmpNearestPnt = tmpNearestPnt / tmpNearestPnt.magnitude();
		dataWthoutR[tmpIndex] = g_SavedData.value(i) + tmpNearestPnt * g_DecRadius;
	}

	qDebug() << "liuyc : get data without det R finished!";
	for( int i = 0; i < g_XYDesDataCount; ++i )
	{
		cout << "liuyc : data without r = " << dataWthoutR[i] << endl;
	}

	//< step1.2 ����XY���յĲ���ֵ�Ͷ�Ӧ��NC�к�
	qDebug() << "\n\nliuyc step 1.2...";
	RS_Vector * finalOffsetXY = new RS_Vector[g_XYDesDataCount];  //< XY�����ϵĲ���      checked leak
	if( m_ncLineNumXY != NULL )
	{
		//delete [] m_ncLineNumXY;
		//m_ncLineNumXY = NULL;
		arraydelete( m_ncLineNumXY );
	}

	//< step1.2.1 ���㲹��ֵ֮ǰ��Ҫ��XY�����ϵ����żӽ�ȥ����
	for( int i = 0; i < g_XYDesDataCount; ++i )
	{
		switch( dataDir[i] )
		{
		case XY_LEFT:
			dataWthoutR[i].x += g_ManOffZoom[ZOOM_LEFT];
			break;
		case XY_RIGHT:
			dataWthoutR[i].x += g_ManOffZoom[ZOOM_RIGHT];
			break;
		case XY_TOP:
			dataWthoutR[i].y += g_ManOffZoom[ZOOM_TOP];
			break;
		case XY_BTM:
			dataWthoutR[i].y += g_ManOffZoom[ZOOM_BTM];
			break;
		default:
			break;
		}
	}
	if( g_XYDesDataCount > 0 )
	{
		m_ncLineNumXY = new int[g_XYDesDataCount];  //< checked leak
		calculataXYOff( dataWthoutR, dataDir, finalOffsetXY, g_XYDesDataCount, inputmd);  //< ���м����޸�NC��λ��Ϣ������û�м��벹��
        for( int i = 0; i < g_XYDesDataCount; ++i )
		{
			if( finalOffsetXY[i].magnitude() > 20. )  //< �������󣬾Ϳ���������������
			{
				if( inputmd = ui_input )
				{
					QString tmpContent = tr("�������ò�������������һ�����⣺\n")
						+ tr("1. ̽���Ƿ��Ѿ��궨�����ѱ궨�뾶��0��δ�궨�Ļ����ȱ궨��\n")
						+ tr("2. DXFͼ���ĳߴ��Ƿ�����\n");
					popMessageBox( "Lynuc CAD Warning", tmpContent, 5, QColor(255,0,0), true );
				}
				else
				{
#ifndef Q_OS_WIN
					SetMacroVal( g_CIAddress, MACRO_ALL_WRONG, 5. );
#endif
					qDebug() << "liuyc: offset is to big!";
				}

				arraydelete( m_ncLineNumXY );
				arraydelete( finalOffsetXY );
				arraydelete( dataDir );
				arraydelete( dataWthoutR );
				return;
			}
		}	
	}
	
	//< step1.3 ����Z�Ჹ���Ͷ�Ӧ��
	qDebug() << "\n\nliuyc step 1.3...";
	RS_Vector * finalOffsetZ = new RS_Vector[g_ZDataCount];   //< checked leak
	if( m_ncLineNumZ != NULL )
	{
		arraydelete( m_ncLineNumZ );
		/*delete [] m_ncLineNumZ;
		m_ncLineNumZ = NULL;*/
	}
	if( g_ZDataCount > 0 )
	{
		m_ncLineNumZ = new int[g_ZDataCount];         //< checked leak
		calculataZOff( finalOffsetZ, g_ZDataCount );
	}
	
	//< step1.4 �Ѳ���������Ӧ�ĵ���ȥ
	qDebug() << "\n\nliuyc step1.4...";

	//< for test: �������֮ǰ�����е�
	/*QFile testFommerPointFile( "/home/Lynuc/Users/NCFiles/Former.txt" );
	testFommerPointFile.open( QIODevice::WriteOnly );
	QTextStream fout( &testFommerPointFile );
	for( int i = 0; i < m_lPointCount; ++i )
	{
		fout << "X" << QString::number( m_NCPoints[i].x, 'd', 6 )
			<< "Y" << QString::number( m_NCPoints[i].y, 'd', 6 )
			<< "Z" << QString::number( m_NCPoints[i].z, 'd', 6 ) << "\n";
	}
	fout.flush();
	testFommerPointFile.close()*/;

	//< ����NC��ͷ�ͽ�β�Ĳ���  
	//< ֮ǰ�ǽ�����һ���ߵĲ�����ƽ��ֵ�ӵ�ͷ����β�ϣ��Ա�֤���������˵��Ĳ��������Ҳ����ֻ���
	//< ���ڽ������˵��Ĳ�����ֱ������Ϊ0  2016.10.13  liu.y.c
	RS_Vector headOff, tailOff;  //< �������˵��Ĳ���������ƫ��
	calculateHeadAndTailOff( finalOffsetXY, g_XYDesDataCount, headOff, tailOff ); 
	cout << "liuyc head and tail off: " << headOff << "  " << tailOff << endl;

	long curPntNum = 0;
	if( g_XYDesDataCount > 0 && m_ncLineNumXY != NULL )
	{
		long offLNumFirXY = m_ncLineNumXY[0];                                 //< ��һ����������к�
		long offLNumLstXY = m_ncLineNumXY[g_XYDesDataCount-1];                //< ���һ����������к�
		long cutLNumFir = -1;
	    long cutLNumLst = -1;
		long actFirOffLine = -1;
		long actLstOffLine = -1;
		if( m_bIsClose )
		{
			cutLNumFir = m_lineNumToDataIndex.key( m_nCutFirstPointIndex, -1 );  //< �������к�
			cutLNumLst = m_lineNumToDataIndex.key( m_nCutLastPointIndex, -1 );   //< �˵����к�
			if( offLNumFirXY < cutLNumFir || offLNumLstXY > cutLNumLst )
			{
				qDebug() << __FUNCTION__ << " : liuyc failed to find cut in point!";
#ifndef Q_OS_WIN
				if( motion_input == inputmd )
					SetMacroVal( g_CIAddress, MACRO_ALL_WRONG, 7. );
				else
					popMessageBox( "Lynuc CAD Warning", tr("δ�ҵ����NC�Ľ�������˵��㣡"), 5, QColor(255,0,0), true );
#endif
				arraydelete( m_ncLineNumZ );
				arraydelete( finalOffsetZ );
				arraydelete( m_ncLineNumXY );
				arraydelete( finalOffsetXY );
				arraydelete( dataDir );
				arraydelete( dataWthoutR );
				return;
			}

			actFirOffLine = m_lineNumToDataIndex.key( m_lActFstOffPnt, -1 );     //< ʵ���ϲ�����ʼ���кţ��ӽ������㣩
			actLstOffLine = m_lineNumToDataIndex.key( m_lActLstOffPnt, -1 );     //< ʵ���ϲ����������к�
		}

		qDebug() << "liuyc: act first offline = " << actFirOffLine;
		qDebug() << "liuyc: act last offline = " << actLstOffLine; 

		if( actFirOffLine <= 0 )
		{
			actFirOffLine = m_nSrcNCPntFirstLine;
			actLstOffLine = m_nSrcNCPntFirstLine + m_lPointCount;
		}

		qDebug() << "liuyc: act first offline = " << actFirOffLine;
		qDebug() << "liuyc: act last offline = " << actLstOffLine; 
		
		RS_Vector tmpVec;
		for( int i = 0; i < m_lNCLineCount; ++i )
		{
			if( m_lineNumToDataIndex.value(i, -1) == -1 )
				continue;

			curPntNum = m_lineNumToDataIndex.value(i, -1);
            if( i <= actFirOffLine )  //< ʵ���ϵ�һ��������֮ǰ��ȫ��ƽ�Ƶ�һ���������ֵ
			{
				m_NCPoints[curPntNum] += headOff;
				continue;
			}
			else if( i >= actLstOffLine )  //< ʵ�ʵ����һ��������֮��
			{
				m_NCPoints[curPntNum] += tailOff;
				continue;
			}
			else if( i <= offLNumFirXY )  //< ��һ��̽���֮ǰ
			{
				m_NCPoints[curPntNum] += ( headOff + ( finalOffsetXY[0] - headOff ) 
					* static_cast<double>( i - actFirOffLine ) / static_cast<double>( offLNumFirXY - actFirOffLine ));
				continue;
			}
			else if( i >= offLNumLstXY )   //< ���һ��̽���֮��
			{
				m_NCPoints[curPntNum] = m_NCPoints[curPntNum] + finalOffsetXY[g_XYDesDataCount-1] 
				  + ( tailOff - finalOffsetXY[g_XYDesDataCount-1] ) * static_cast<double>( i - offLNumLstXY )
					  / static_cast<double>( actLstOffLine - offLNumLstXY );
				continue;
			}
			else  //< ��һ�������㵽���һ��������֮��
			{
				//< ���ҵ���ǰ�кŶ�Ӧ�Ĳ����к�����
				int j = 0;
				for( j = 0 ; j < g_XYDesDataCount - 1; ++j )
				{
					if( i >= m_ncLineNumXY[j] &&  i < m_ncLineNumXY[j+1])
						break;
				}
				m_NCPoints[curPntNum] = m_NCPoints[curPntNum] + finalOffsetXY[j] + ( finalOffsetXY[j+1] - finalOffsetXY[j] )
					* static_cast< double >( i - m_ncLineNumXY[j] ) 
					/ static_cast<double>( m_ncLineNumXY[j+1] - m_ncLineNumXY[j] );

				continue;
			}

		}
	}
	curPntNum = 0;
	if( g_ZDataCount > 0 && m_ncLineNumZ != NULL )
	{
		long offLNumFirZ  = m_ncLineNumZ[0];
		long offLNumLstZ  = m_ncLineNumZ[g_ZDataCount-1];

		for( int i = 0; i < m_lNCLineCount; ++i )
		{
			if( m_lineNumToDataIndex.value(i, -1) == -1 )
				continue;

			curPntNum = m_lineNumToDataIndex.value(i, -1);
			if( i < offLNumFirZ )  //< ��һ���㲹����֮ǰ
			{
				m_NCPoints[curPntNum] += finalOffsetZ[0] * static_cast<double>(i - m_nSrcNCPntFirstLine) 
					/ static_cast<double>( offLNumFirZ - m_nSrcNCPntFirstLine );
				continue;
			}
			else if( i == offLNumFirZ )  //< ��һ��������
			{
				m_NCPoints[curPntNum] += finalOffsetZ[0];
				continue;
			}
			else if( i == offLNumLstZ )  //< ���һ��������
			{
				m_NCPoints[curPntNum] += finalOffsetZ[g_ZDataCount-1];
				continue;
			}
			else if( i > offLNumLstZ )   //< ���һ��������֮��
			{
				m_NCPoints[curPntNum] = m_NCPoints[curPntNum] 
				+ finalOffsetZ[g_ZDataCount-1] * ( 1. -  static_cast<double>( i - offLNumLstZ ) 
					/ static_cast<double>( m_nSrcNCPntFirstLine + m_lPointCount - 1 - offLNumLstZ) );
				continue;
			}
			else  //< ��һ�������㵽���һ��������֮��
			{
				//< ���ҵ���ǰ�кŶ�Ӧ�Ĳ����к�����
				int j = 0;
				for( j = 0 ; j < g_ZDataCount - 1; ++j )
				{
					if( i >= m_ncLineNumZ[j] &&  i < m_ncLineNumZ[j+1])
						break;
				}
				m_NCPoints[curPntNum] = m_NCPoints[curPntNum] + finalOffsetZ[j] + ( finalOffsetZ[j+1] - finalOffsetZ[j] )
					* static_cast< double >( i - m_ncLineNumZ[j] ) / static_cast<double>( m_ncLineNumZ[j+1] - m_ncLineNumZ[j] );

				continue;
			}

		}
	}
	
	qDebug() << "liuyc after sort : ";
	for( int i = 0; i < g_XYDesDataCount; ++i )
	{
		cout << "liuyc nc off line num = " << m_ncLineNumXY[i] << endl;
		cout << "liuyc related point   : " << m_NCPoints[m_lineNumToDataIndex.value( m_ncLineNumXY[i] )] << endl;
	}

	//< for test: �������������е㵽�ļ���
	/*QFile testAfterPointFile( "/home/Lynuc/Users/NCFiles/After.txt" );
	testAfterPointFile.open( QIODevice::WriteOnly );
	QTextStream Aout( &testAfterPointFile );
	for( int i = 0; i < m_lPointCount; ++i )
	{
		Aout << "X" << QString::number( m_NCPoints[i].x, 'd', 6 )
			<< "Y" << QString::number( m_NCPoints[i].y, 'd', 6 )
			<< "Z" << QString::number( m_NCPoints[i].z, 'd', 6 ) << "\n";
	}
	Aout.flush();
	testAfterPointFile.close();*/

#ifdef Q_OS_LINUX
	gettimeofday( &endtime, 0 );
	double timeuse = 1000000. * (endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
	timeuse *= 0.001 ;
	std::cout << "liuyc cauculate new points : " << timeuse << "ms" << endl;

	gettimeofday(&starttime, 0);
#endif
	//< step1.5 д�µ�NC�ļ�
	qDebug() << "\n\nliuyc step1.5...";
	newNCOutput( filename, headOff, tailOff );

#ifdef Q_OS_LINUX
	gettimeofday( &endtime, 0 );
	timeuse = 1000000. * (endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
	timeuse *= 0.001 ;
	std::cout << "liuyc output new nc file : " << timeuse << "ms" << endl;
#endif

    //< step1.6 �������ã��������ļ���
	if( inputmd == ui_input )
	{
		qDebug() << "\n\nliuyc step1.6...";
		saveConfig();
	}
	
	m_NewFNameLbl[FILE_NEW]->setText( "<font color=#0000FF style=\"font-weight:bold;\">"
		+ filename.section( '/', -1, -1 ) + "</font>" );

	if( inputmd == ui_input )
	{
	    popMessageBox( "Lynuc CAD Information", "�µ�NC�ļ������ɳɹ������ڱ༭ģ���²鿴��ִ�У�", 5, QColor(0,0,255), true);
		if( m_NewNCPntDlg == NULL )
			m_NewNCPntDlg = new ShowFinalNcDlg( this );

		m_NewNCPntDlg->set( m_NCPoints, m_lPointCount );
		m_NewNCPntDlg->exec();
	}

	//< end
	arraydelete( m_ncLineNumZ );
	arraydelete( finalOffsetZ );
	arraydelete( m_ncLineNumXY );
    arraydelete( finalOffsetXY );
	arraydelete( dataDir );
	arraydelete( dataWthoutR );
	qDebug() << "\n\nliuyc: generate new nc file succeed!**********************\n";
	return;
}

void OffsetWdt::calculataXYOff(RS_Vector * dataWthoutR, int * dataDir
	, RS_Vector * finalOffsetXY, int size, int inputmd)
{
	if( size <= 0 )
		return;

	qDebug() << __FUNCTION__ << " liuyc : start to calculateXYoff !" ;

	RS_Vector * decPntOnEnt = new RS_Vector[size];

	RS_Entity * tmpEnt = NULL;
	int insertFlag = NOT_INSERT_PNT;
	for( int i = 0; i < size; ++i )
	{
		decPntOnEnt[i] = container->getNearestPointOnEntity( dataWthoutR[i] );
		tmpEnt = container->getNearestEntity( dataWthoutR[i] );

		finalOffsetXY[i] = dataWthoutR[i] - decPntOnEnt[i];
		finalOffsetXY[i].z = 0.;
		if( fabs(finalOffsetXY[i].x) < 10e-6 )
			finalOffsetXY[i].x = 0.;
		else if( fabs(finalOffsetXY[i].y) < 10e-6 )
			finalOffsetXY[i].y = 0.;

		//< ��ȡ��ǰ�Ĳ��������ڻ��Ƕ���
		if( inputmd == motion_input )
			g_nInOrOut = getSrcNCOffDir();

		//qDebug() << "liuyc offdir : " << g_nInOrOut;
		//< 2016.10.20 liu.y.c ֮ǰ�İ汾��ֻ֧�������ߣ�����ѡ�������ַ���������ʵҲû�б�Ҫ���㷨���Ը���
	//	if( tmpEnt->rtti() == RS2::EntityLine )
	//	{
	//		//< ���ҵ���������ȷ�ķ���
	//		RS_Vector normalVec = dynamic_cast<RS_Line *>( tmpEnt )->getNormalVector();
	//		if( ( dataDir[i] == XY_LEFT && normalVec.x > 0.) || (dataDir[i] == XY_RIGHT && normalVec.x < 0.)
	//			|| (dataDir[i] == XY_TOP && normalVec.y < 0.) || (dataDir[i] == XY_BTM && normalVec.y > 0.))
	//		{
	//			normalVec.x = -normalVec.x;
	//			normalVec.y = -normalVec.y;
	//		}
	//		cout << "liuyc dir : " << dataDir[i] << "  normal vec : " << normalVec << endl;
	//		//< ��̽����ϼ���NC������ľ�����������NC��ĵ�λ���Ƚϣ��ҳ���Ӧ��NC�к�
	//		RS_Vector ncVector = decPntOnEnt[i] + normalVec * m_dNCFileROff * g_nInOrOut;
	//		cout << "point with tool R : " << ncVector << endl;

	//		insertFlag = NOT_INSERT_PNT;
	//		m_ncLineNumXY[i] = findNearestNCLineXY( ncVector, insertFlag );  //< �Ƿ�Ϊ����ĵ�
	//		if( m_ncLineNumXY[i] == -1 )
	//		{
	//			qDebug() << "liuyc: never find nc line!";
	//			return;
	//		}

	//		//< ����ǰ��ȡ���кŶ�Ӧ���ǲ�����µ㣬������ LINENUM ��Ҫ�����ƣ�����Z���Ҳ��Ҫ�����ƵĴ���
	//		if( INSERT_PNT == insertFlag )
	//		{
	//			for( int j = 0; j < i; ++j )
	//			{
	//				if( m_ncLineNumXY[i] <= m_ncLineNumXY[j] )  //< �����������ܲ���ͬһ������
	//					m_ncLineNumXY[j]++;
	//			}

	//			if( m_ncLineNumZ != NULL )  //< ͬ��Z���к�Ҳ����Ҫ�����ƶ���
	//			{
	//				for( int j = 0; j < g_ZDataCount; ++j )
	//				{
	//					if( m_ncLineNumXY[i] <= m_ncLineNumZ[j] )
	//						m_ncLineNumZ[j]++;
	//				}
	//			}
	//		}
	//		
	//	}
	//	else
	//	{
	//		qDebug() << "liuyc: it's not a line!";
	//	}

		RS_Vector normalVec = finalOffsetXY[i] / finalOffsetXY[i].magnitude();  //< ��������������ʱû��ȷ�����ڻ�������
		//< �Ƚ����еķ�����������������
		if( ( (dataDir[i] == XY_LEFT || dataDir[i] == XY_LEFT_TOP || dataDir[i] == XY_LEFT_BTM) && normalVec.x > 0.) 
			|| ( (dataDir[i] == XY_RIGHT || dataDir[i] == XY_RIGHT_TOP || dataDir[i] == XY_RIGHT_BTM) && normalVec.x < 0.)
			|| (dataDir[i] == XY_TOP && normalVec.y < 0.) 
			|| (dataDir[i] == XY_BTM && normalVec.y > 0.))
		{
			normalVec.x = -normalVec.x;
			normalVec.y = -normalVec.y;
		}
		cout << "liuyc dir : " << dataDir[i] << "  normal vec : " << normalVec << endl;
		//< ��̽����ϼ���NC������ľ�����������NC��ĵ�λ���Ƚϣ��ҳ���Ӧ��NC�к�
		RS_Vector ncVector = decPntOnEnt[i] + normalVec * m_dNCFileROff * g_nInOrOut;
		//cout << "point with tool R : " << ncVector << endl;

		insertFlag = NOT_INSERT_PNT;
		m_ncLineNumXY[i] = findNearestNCLineXY( ncVector, insertFlag );  //< �Ƿ�Ϊ����ĵ�
		if( m_ncLineNumXY[i] == -1 )
		{
			qDebug() << "liuyc: never find nc line!";
			return;
		}

		//< ����ǰ��ȡ���кŶ�Ӧ���ǲ�����µ㣬������ LINENUM ��Ҫ�����ƣ�����Z���Ҳ��Ҫ�����ƵĴ���
		if( INSERT_PNT == insertFlag )
		{
			for( int j = 0; j < i; ++j )
			{
				if( m_ncLineNumXY[i] <= m_ncLineNumXY[j] )  //< �����������ܲ���ͬһ������
					m_ncLineNumXY[j]++;
			}

			if( m_ncLineNumZ != NULL )  //< ͬ��Z���к�Ҳ����Ҫ�����ƶ���
			{
				for( int j = 0; j < g_ZDataCount; ++j )
				{
					if( m_ncLineNumXY[i] <= m_ncLineNumZ[j] )
						m_ncLineNumZ[j]++;
				}
			}
		}
	}


	for( int i = 0; i < size; ++i )
	{
		cout << "point offset : " << finalOffsetXY[i] << endl;
	}

	//< ��ʱ����һ������MAPΪfinalOffsetXY����
	qDebug() << "liuyc sorted offset: ";
	QMap<int, RS_Vector> lineToOffMap;
	for( int i = 0 ; i < size; ++i )
	{
		lineToOffMap.insert( m_ncLineNumXY[i], finalOffsetXY[i] );
	}
	QList< RS_Vector > sortedFinalOffset = lineToOffMap.values();
	for( int i = 0; i < size; ++i )
	{
		finalOffsetXY[i] = sortedFinalOffset.value( i );
		cout << "point offset : " << finalOffsetXY[i] << endl;
	}
	lineToOffMap.clear();  //< �������Map
	//< ���������к��������򣩣���֤����Ӳ���ʱΪ������β���
	std::sort( m_ncLineNumXY, m_ncLineNumXY + g_XYDesDataCount );

	delete [] decPntOnEnt;
}

/************************************************************************/
/* getSrcNCOffDir
/* function: ��ȡԭNC�Ĳ���������ʵ���ڻ���ʵ���⣩
/* input:    1. ̽����Ӧ��ʵ���ϵĵ�         RS_Vector
             2. ������Ӧ�ıߣ��ϡ��¡����ң�XYZ_DIR(int)
/* output:   NC�ϵĲ������������⻹�����ڣ��� = 1���� = -1  (int)
/************************************************************************/
int OffsetWdt::getSrcNCOffDir( /*RS_Vector & vecOnEnt, XYZ_DIR dir*/ )
{
	if( m_NCPoints == NULL )
		return 0;

#define INSIDE_OFF (-1)
#define OUTSIDE_OFF 1
	
	return getMacroInt( MACRO_P152_INorOUT );
}

/************************************************************************/
/* ncHeadAndTailMod
/* function: �޸�NC��ͷβ����NCͷβ����ƫ��һ��ƫ���������ⲹ�������NC���˵����ֵĸ���
/* input:    1. ��ǰ�е����� QString &
             2. x��ƫ����   double
			 3. y��ƫ����   double
/* output    void
/************************************************************************/
void OffsetWdt::ncHeadAndTailMod( QString & qstrLine, double x /* = 0. */, double y /* = 0. */ )
{
	QRegExp expX( "x|X" );
	QRegExp expY( "y|Y" );
	QRegExp expSplit( "\\s+|[a-z,A-Z]" );

	if( qstrLine.contains('x', Qt::CaseInsensitive)  /* ��һ��������X */
		&& !qstrLine.section( expX, 0, 0).contains( "(" ) )  /* Xǰ��û�������ţ�*/
	{
		QString xStr = qstrLine.section( expX, 1, 1 ).section( expSplit, 0, 0 );
		qDebug() << "liuyc strX = " << xStr;
		qstrLine.replace( "x" + xStr
			, "X" + QString::number( xStr.toDouble() + x, 'd', 6 )
			, Qt::CaseInsensitive );
	}

	if( qstrLine.contains('y', Qt::CaseInsensitive) 
		&& !qstrLine.section( expY, 0, 0).contains( "(" ))
	{
		QString yStr = qstrLine.section( expY, 1, 1 ).section( expSplit, 0, 0 );
		qDebug() << "liuyc strY = " << yStr;
		qstrLine.replace( "y" + yStr
			, "Y" + QString::number( yStr.toDouble() + y, 'd', 6 )
			, Qt::CaseInsensitive );
	}
}

/************************************************************************/
/* newNCOutput
/* function: �Թ̶���ʽ�����µ�NC���������ģ�
/* input:    1. NCͷ�ϵĲ���  RS_Vector
             2. NC��β�Ĳ���  RS_Vector
/* output:   void
/************************************************************************/
void OffsetWdt::newNCOutput(QString filename, RS_Vector headOff, RS_Vector tailoff )
{

	QFile newFile( filename );
	if( !newFile.open( QIODevice::Text | QIODevice::WriteOnly ))
	{
		qDebug() << "liuyc: new file open failed!";
		return;
	}
	QTextStream out( &newFile );
	QStringList contentList = m_SrcContent.split('\n');

	//< ���G01֮ǰ�����ݣ��൱����NC��ͷ
	QString qstrLine;
	for( int i = 0; i < m_nSrcNCPntFirstLine; ++i )
	{
		qstrLine = contentList.at(i);
		if( !(qstrLine.contains("G28", Qt::CaseInsensitive)) )  //< �ҵ���G28��ֱ��������һ�в���������
			ncHeadAndTailMod( qstrLine
			                 , headOff.x + g_ManOff[0]
		                     , headOff.y + g_ManOff[1] );

		out << qstrLine << "\n";
	}

	//< G01��һ�У�Ҫ����F�����ٶ�
	out << "G01" << "X" << QString::number( m_NCPoints[0].x + g_ManOff[0], 'd', 6 ) << " "
		<< "Y" << QString::number( m_NCPoints[0].y + g_ManOff[1], 'd', 6 ) << " "
		<< "Z" << QString::number( m_NCPoints[0].z + g_ManOff[2], 'd', 6 ) << " "
		<< "F" << QString::number( m_dNCFileFspd, 'd', 1) << "\n";

	//< ����һ��G00֮ǰ��N�У�ֻ��Z��仯�Ż����Z������
	double tmpZ = m_NCPoints[0].z;
	for( int i = 1; i < m_lPointCount; ++i )
	{
		out << "X" << QString::number( m_NCPoints[i].x + g_ManOff[0], 'd', 6 )
			<< " Y" << QString::number( m_NCPoints[i].y + g_ManOff[1], 'd', 6 );
		if( fabs( m_NCPoints[i].z - tmpZ ) > 10e-5 )
		{
			tmpZ = m_NCPoints[i].z;
			out << " Z" << QString::number( m_NCPoints[i].z + g_ManOff[2], 'd', 6 ) << "\n";
		}
		else
			out << "\n";
	}

	//< ���NCβ�������һ��G00֮�������
	for( int i = m_nSrcNCPntLastLine + 1; i < contentList.count(); ++i )
	{
		qstrLine = contentList.at(i);
		if( !(qstrLine.contains("G28", Qt::CaseInsensitive)) )  //< �ҵ���G28��ֱ��������һ�в���������
			ncHeadAndTailMod( qstrLine
			                 , tailoff.x + g_ManOff[0]
		                     , tailoff.y + g_ManOff[1] );

		out << qstrLine << "\n";
	}

	//< �ļ�������
	out.flush();
	newFile.close();

#ifndef Q_OS_WIN
	system( "sync" );  //< ͬ��ȷ���ļ�������ɣ�
#endif
}


void OffsetWdt::calculataZOff( RS_Vector * finalOffsetZ, int size )
{
	if( g_ZDesDataBeginNum <= 0 || g_ZDesDataEndNum <= 0 )
		return;

	qDebug() << "liuyc calculate z off start!";

	RS_Vector * detPointXY  = new RS_Vector[size];  //< ��Z��̽����XY���굥���ó���������Z�������ڷ�����û������
	RS_Entity * tmpEnt = NULL;

	int insertFlag = NOT_INSERT_PNT;
	//< �ȼ���Z�����̽���Ĳ���ֵ
	for( int i = 0; i < size; ++i )
	{
		//< �Ƚ�Z��̽���XY �� Z������뿪
		finalOffsetZ[i].set( 0., 0., 0. );
		finalOffsetZ[i].z = g_SavedData[i+g_ZDesDataBeginNum].z;  //< Z�Ჹ����ֻ��Z��XY����0.

		detPointXY[i] = g_SavedData[i+g_ZDesDataBeginNum];
		detPointXY[i].z = 0.;

		tmpEnt = container->getNearestEntity( detPointXY[i] );
		//< 2016/10/20 liu.y.c ͬ���Ǽ���Բ��̽��ʱ�޸��㷨��û�б�Ҫ��ֱ�� 
		//if( tmpEnt->rtti() == RS2::EntityLine )
		//{
		insertFlag = NOT_INSERT_PNT;
		m_ncLineNumZ[i] = findNearestNCLineZ( detPointXY[i], insertFlag, 1. );//< ����������㷨���������⣬��Ҫ�޸ģ���ͣ���޸� 2016.10.20
		if( m_ncLineNumZ[i] == -1 )
		{
			qDebug() << "liuyc: never find nc line_z!";
			delete [] detPointXY;
			return;
		}

		if( INSERT_PNT == insertFlag )
		{
			for( int j = 0; j < i; ++j )
			{
				if( m_ncLineNumZ[i] <= m_ncLineNumZ[j] )
					m_ncLineNumZ[j]++;
			}
			if( m_ncLineNumXY != NULL )  //< XY���к�Ҳ����Ҫ�����ƶ���
			{
				for( int j = 0; j < g_XYDesDataCount; ++j )
				{
					if( m_ncLineNumZ[i] <= m_ncLineNumXY[j] )
						m_ncLineNumXY[j]++;
				}
			}
		}
	}

	for( int i = 0; i < size; ++i )
	{
		cout << "point offset z : " << finalOffsetZ[i] << endl;
	}

	//< ��ʱ����һ������MAPΪfinalOffsetXY����
	qDebug() << "liuyc sorted offset: ";
	QMap<int, RS_Vector> lineToOffMap;
	for( int i = 0 ; i < size; ++i )
	{
		lineToOffMap.insert( m_ncLineNumZ[i], finalOffsetZ[i] );
	}
	QList< RS_Vector > sortedFinalOffset = lineToOffMap.values();
	for( int i = 0; i < size; ++i )
	{
		finalOffsetZ[i] = sortedFinalOffset.value( i );
		cout << "point offset z : " << finalOffsetZ[i] << endl;
	}
	lineToOffMap.clear();  //< �������Map
	//< ���������к��������򣩣���֤����Ӳ���ʱΪ������β���
	std::sort( m_ncLineNumZ, m_ncLineNumZ + size );

	delete [] detPointXY;
}

void OffsetWdt::calculateHeadAndTailOff( RS_Vector * offset, int size, RS_Vector & headOff, RS_Vector & tailOff )
{
	//< step1 ��ֱ���趨����Ϊ0����������Ҫ����ֱ�ӷ��ز���Ϊ0
	headOff.set( 0., 0., 0. );
	tailOff.set( 0., 0., 0. );

	if( size <= 0 || offset == NULL )  //< ���OFFSET����û�У���ֱ�ӷ���
		return;
	//if( m_nCutFirstPointIndex <= 0 || m_nCutLastPointIndex <= 0 )
		//return;

	//< step2 ���Ƿ��ͼ�Σ�����Ҫ�ҵ����˵������غϵĵ㲢�޳�
	if( !m_bIsClose )
	{
		//< ������ͼ�ηǷ�գ���headerʹ�������ߵ�ƽ��offset
		int i = 0;
		for( i = 1; i < size ; ++i )
		{
			if( fabs( offset[i].x - offset[i-1].x) > 10e-8 
				&& fabs( offset[i].y - offset[i-1].y) > 10e-8 )  //< ǰ��������XY����ͬ��ʾ��������һ������
			{
				headOff += offset[i-1];
				break;
			}
			else
				headOff += offset[i-1];
		}
		headOff /= static_cast<double>( i );

		int j = 0;
		for( j = size - 2; j >= 0; j-- )
		{
			if( fabs( offset[j].x - fabs(offset[j+1].x)) > 10e-8 
				&& fabs( offset[j].y - fabs(offset[j+1].y)) > 10e-8 )
			{
				tailOff += offset[j+1];
				break;
			}
			else
				tailOff += offset[j+1];
		}
		tailOff /= ( size - 1 - j);
		return;
	}

	//< ���ͼ�ε����	
	int pntCountLstToFirst = 0;
	RS_Vector offsetVec = offset[0] - offset[size - 1];  //< ���һ�㵽��һ������һ�εĲ�����
	int part1Count = 0;
	int part2Count = 0;
	if( m_NCPoints[m_nCutFirstPointIndex].distanceTo(m_NCPoints[m_nCutLastPointIndex]) < 0.001 ) 
	{
		//< ���˵����غϣ���ʱ���˵����м�ĵ�ͻ��γ�һ����յ�ͼ��
	    part1Count = m_lineNumToDataIndex.value( m_ncLineNumXY[0] ) - m_nCutFirstPointIndex;
		part2Count = m_nCutLastPointIndex - m_lineNumToDataIndex.value( g_XYDesDataCount - 1 );

		m_lActFstOffPnt = m_nCutFirstPointIndex;
		m_lActLstOffPnt = m_nCutLastPointIndex;
	} 
	else 
	{
		//< ���˵��㲻�غ�
		int    index   = -1;
		double minDist = RS_MAXDOUBLE;
		double tmpDist = 0.;
		cout << "liuyc first cut " << m_NCPoints[m_nCutFirstPointIndex] << " last cut " << 
			m_NCPoints[m_nCutLastPointIndex] << endl;
		for( int i = m_nCutFirstPointIndex; i < m_lPointCount * 0.5 && i < m_nCutLastPointIndex; ++i )  
		{
			//< �ӽ����㵽�˵���ǰһ���㣬���ֻ�ҵ�����NC��һ��
			tmpDist = m_NCPoints[i].distanceTo( m_NCPoints[m_nCutLastPointIndex] );
			if( tmpDist < minDist )
			{
				minDist = tmpDist;
				index = i;
			}
		}
		qDebug() << "liuyc find tail! index = " << index << " dist = " << minDist;
		part1Count = index - m_nCutFirstPointIndex;
		part2Count = m_nCutLastPointIndex - m_lineNumToDataIndex.value( g_XYDesDataCount - 1 );

		m_lActFstOffPnt = index;
		m_lActLstOffPnt = m_nCutLastPointIndex;
	}

	pntCountLstToFirst = part2Count + part1Count;

	tailOff = offsetVec / static_cast<double>( pntCountLstToFirst ) * part1Count + offset[size-1];
	headOff = tailOff;

	qDebug() << "liuyc act point index = " << m_lActFstOffPnt << " " << m_lActLstOffPnt;
} 

int OffsetWdt::findNearestNCLineXY( RS_Vector & vec, int & insertFlag, double tollerentDist )
{
	if( m_NCPoints == NULL )
		return -1;
#if 1  //< 2016.09.07 ����֮ǰ���㷨ȱ�ݣ��޸����㷨�������У� ֮ǰ���㷨����һ����֧����
	//< step1 �Ƚ����еĵ㹹����
	int lineNum = -1;
	int dataNum = -1;
	double minDist = RS_MAXDOUBLE;
	double * tmpDist = new double(0.);  //< checked leak
	for( int i = 0; i < m_lNCLineCount - 1; ++i )
	{
		if( m_lineNumToDataIndex.value(i, -1) == -1 || m_lineNumToDataIndex.value(i+1, -1) == -1 )
			continue;

		RS_Vector curStartPoint = m_NCPoints[m_lineNumToDataIndex.value(i)];
		RS_Vector curEndPoint   = m_NCPoints[m_lineNumToDataIndex.value(i+1)];
		curStartPoint.z = curEndPoint.z;
		if( curStartPoint.distanceTo( curEndPoint ) < 10e-6 )
		    continue;

		RS_Line tmpLine( curStartPoint, curEndPoint );
		tmpLine.getNearestPointOnEntity( vec, true, tmpDist );
		if( *tmpDist + 10e-8 < minDist )
		{
			minDist = *tmpDist;
			lineNum = i;
			dataNum = m_lineNumToDataIndex[i];
			vec.z = m_NCPoints[dataNum].z;
		}
	}
	delete tmpDist;

	qDebug() << "liuyc : dataNum = " << dataNum;

	if( dataNum < 0 )
		return -1;

	double distToPnt1 = 0.;
	double distToPnt2 = 0.;
	if( dataNum <= m_lPointCount - 1 )
	    distToPnt1 = vec.distanceTo( m_NCPoints[dataNum] );
	if( dataNum <= m_lPointCount - 2 )
	    distToPnt2 = vec.distanceTo( m_NCPoints[dataNum+1] );

	qDebug() << "liuyc dist1 = " << distToPnt1 << "  dist2 = " << distToPnt2;

	if( distToPnt1 > tollerentDist && distToPnt2 > tollerentDist )
	{
		insertFlag = INSERT_PNT;
		cout << "liuyc insert new point of xy!" << endl;
		//< �½�һ���µ����飬��������֮��ĵ�λ��Ϣ����������֮ǰ��С�� 1
		m_lPointCount++;
		RS_Vector * tmpPoints = new RS_Vector[m_lPointCount];
		if( dataNum == 0 )  //< ��һ����
		{
			tmpPoints[0] = m_NCPoints[0];
			tmpPoints[1] = vec;
			lineNum++;
			for( int i = 2; i < m_lPointCount; ++i )
			{
				tmpPoints[i] = m_NCPoints[i-1];
			}
		}
		else if( dataNum == m_lPointCount - 2 )  //< ��λ����һ֮ǰ������һ��������
		{
			for( int i = 0; i < m_lPointCount - 2; ++i)
			{
				tmpPoints[i] = m_NCPoints[i];
			}
			tmpPoints[m_lPointCount-2] = vec;
			tmpPoints[m_lPointCount-1] = m_NCPoints[m_lPointCount-1];
		}
		else  //< ����ĵ����м�ĵ㣨ȥ����ǰ����β���������
		{
			RS_Vector vecToLineNumP = m_NCPoints[dataNum] - vec;
			RS_Vector vecToNextP    = m_NCPoints[dataNum + 1] - vec;
			RS_Vector lnNumPToNextP = m_NCPoints[dataNum + 1] - m_NCPoints[dataNum];
			if( lnNumPToNextP.magnitude() > vecToLineNumP.magnitude()
				&& lnNumPToNextP.magnitude() > vecToNextP.magnitude() )  //< ��������жϸ��Ͻ�
			{

				//< ���������Ƕ۽ǣ��ӵ�datanum����һ������
				for( int i = 0;i < dataNum + 1; ++i )
				{
					tmpPoints[i] = m_NCPoints[i];
				}
				tmpPoints[dataNum + 1] = vec;
				lineNum++;
				for( int i = dataNum + 2; i < m_lPointCount; ++i )
				{
					tmpPoints[i] = m_NCPoints[i-1];
				}
			} 
			else 
			{
				//< ������������ǣ�����ĵ�����LINENUMǰ���պ��滻��LINENUM��Ӧ�ĵ�
				for( int i = 0; i < dataNum; ++i )
				{
					tmpPoints[i] = m_NCPoints[i];
				}
				tmpPoints[dataNum] = vec;
				for( int i = dataNum + 1; i < m_lPointCount; ++i )
				{
					tmpPoints[i] = m_NCPoints[i-1];
				}
			}
		}
		delete [] m_NCPoints;
		m_NCPoints = tmpPoints;

		//< �����к����ŵ�ӳ���ϵ
		m_lNCLineCount++;
		int lastLineNum = m_lineNumToDataIndex.key( m_lineNumToDataIndex.size() - 1, -1);

		if( lastLineNum != - 1 )
			m_lineNumToDataIndex[lastLineNum + 1] = m_lPointCount - 1;
		else
			return -1;
	}


#else
	//< step1 �ҵ����е��������������ĵ�
	int lineNum = -1;
	int dataNum = -1;
	double minDist = RS_MAXDOUBLE;
	double tmpDist = 0.;
	for( int i = 0; i < m_lNCLineCount; ++i )
	{
		//< �����ǰ��û�ж�Ӧ�ĵ㣬������
		if( m_lineNumToDataIndex.value(i, -1) == -1 )
			continue;

		//< �����ǰ�еĶ�Ӧ����ǰһ����ͬ��ֻ��Z�������������
		RS_Vector curPoint = m_NCPoints[m_lineNumToDataIndex.value(i)];
		if( m_lineNumToDataIndex.value( i-1, -1 ) != -1 )
		{
			RS_Vector prePoint = m_NCPoints[m_lineNumToDataIndex.value(i-1)];
			prePoint.z = curPoint.z;
			if( curPoint.distanceTo( prePoint ) < 10e-6 )
			{
				continue;
			}
		}

		tmpDist = m_NCPoints[m_lineNumToDataIndex.value(i)].distanceTo( vec );
		if( tmpDist + 10e-8 < minDist )  //< ����ıȽ���� 10E-8
		{
			minDist = tmpDist;
			lineNum = i;
			dataNum = m_lineNumToDataIndex[i];
			vec.z = m_NCPoints[m_lineNumToDataIndex[i]].z;
		}
	}

	if( dataNum < 0 )
		return -1;

	//< step2 �����С���뻹�Ǵ�����������㣬����Ҫ�����µĵ�
	if( minDist > tollerentDist )
	{
		insertFlag = INSERT_PNT;
		cout << "liuyc insert new point of xy!" << endl;
		//< �½�һ���µ����飬��������֮��ĵ�λ��Ϣ����������֮ǰ��С�� 1
		m_lPointCount++;
		RS_Vector * tmpPoints = new RS_Vector[m_lPointCount];
		if( dataNum == 0 )  //< ��һ����
		{
			tmpPoints[0] = m_NCPoints[0];
			tmpPoints[1] = vec;
			lineNum++;
			for( int i = 2; i < m_lPointCount; ++i )
			{
				tmpPoints[i] = m_NCPoints[i-1];
			}
		}
		else if( dataNum == m_lPointCount - 2 )  //< ��λ����һ֮ǰ������һ��������
		{
			for( int i = 0; i < m_lPointCount - 2; ++i)
			{
				tmpPoints[i] = m_NCPoints[i];
			}
			tmpPoints[m_lPointCount-2] = vec;
			tmpPoints[m_lPointCount-1] = m_NCPoints[m_lPointCount-1];
		}
		else  //< ����ĵ����м�ĵ㣨ȥ����ǰ����β���������
		{
			RS_Vector vecToLineNumP = m_NCPoints[dataNum] - vec;
			RS_Vector vecToNextP    = m_NCPoints[dataNum + 1] - vec;
			RS_Vector lnNumPToNextP = m_NCPoints[dataNum + 1] - m_NCPoints[dataNum];
			//if( fabs(vecToLineNumP.angle() - vecToNextP.angle()) < M_PI * 0.5 )  
			if( lnNumPToNextP.magnitude() > vecToLineNumP.magnitude()
				&& lnNumPToNextP.magnitude() > vecToNextP.magnitude() )  //< ��������жϸ��Ͻ�
			{
				
				//< ���������Ƕ۽ǣ��ӵ�datanum����һ������
				for( int i = 0;i < dataNum + 1; ++i )
				{
					tmpPoints[i] = m_NCPoints[i];
				}
				tmpPoints[dataNum + 1] = vec;
				lineNum++;
				for( int i = dataNum + 2; i < m_lPointCount; ++i )
				{
					tmpPoints[i] = m_NCPoints[i-1];
				}
			} 
			else 
			{
				//< ������������ǣ�����ĵ�����LINENUMǰ���պ��滻��LINENUM��Ӧ�ĵ�
				for( int i = 0; i < dataNum; ++i )
				{
					tmpPoints[i] = m_NCPoints[i];
				}
				tmpPoints[dataNum] = vec;
				for( int i = dataNum + 1; i < m_lPointCount; ++i )
				{
					tmpPoints[i] = m_NCPoints[i-1];
				}
			}
		}
		delete [] m_NCPoints;
		m_NCPoints = tmpPoints;

		//< �����к����ŵ�ӳ���ϵ
		m_lNCLineCount++;
		int lastLineNum = m_lineNumToDataIndex.key( m_lineNumToDataIndex.size() - 1, -1);

		if( lastLineNum != - 1 )
			m_lineNumToDataIndex[lastLineNum + 1] = m_lPointCount - 1;
		else
			return -1;
	}
#endif

	qDebug() << "liuyc return lineNum = " << lineNum;
	return lineNum;
}

int OffsetWdt::findNearestNCLineZ( RS_Vector & vec, int & insertFlag, double maxDist )
{

	if( m_lPointCount <= 0 && m_NCPoints == NULL )
		return -1;

	// qDebug() << "liuyc find nearest nc line z start!";

	int lineNum = -1;
	int dataNum = -1;

	//< �ҵ�NC����������㣨̽��㣩����ĵ�
	double dist = RS_MAXDOUBLE;
	insertFlag = NOT_INSERT_PNT;
	int  curNum = 0;
	for( int i = 0; i < m_lNCLineCount; ++i )
	{
		curNum = m_lineNumToDataIndex.value( i, -1 );
		if( curNum == -1 )
			continue;
        
		if( vec.distanceTo( m_NCPoints[curNum] ) + 10e-8 < dist )  //< ���Թ��˵���ͬ�ĵ�
		{
			dist = vec.distanceTo( m_NCPoints[curNum] );
			lineNum = i;
			dataNum = curNum;
		}
	}

	//< ���û���ҵ�����ֱ�ӷ���
	if( lineNum == -1 )
		return lineNum;

	int nxtPntNum = 0;
	int prePntNum = 0;
	RS_Line * tmpLine = NULL;
	RS_Line * tmpLine2 = NULL;
    bool isLineValid = true;

	if( 0 == dataNum )
	{
		qDebug() << "liuyc datanum == 0...";
		nxtPntNum = dataNum + 1;
		if( nxtPntNum >= m_lPointCount )
			return -1;
		//< ����������ҵ��뵱ǰ�㲻�غϵĵ㣬ֱ�����һ���㣬���nxtPnt�����Ǻ�����ͬ��N����ĵ�һ��!
		while( m_NCPoints[dataNum].distanceTo( m_NCPoints[nxtPntNum] ) < 10e-8 )
		{
			if( nxtPntNum >= m_lPointCount - 1 )
			{
				isLineValid = false;  //< �ҵ����һ������δ�ҵ���ͬ�ĵ�
				break;
			}

			nxtPntNum++;
		}

		if( !isLineValid )  //< ��ʱ������
			return -1;
		tmpLine = new RS_Line( m_NCPoints[dataNum], m_NCPoints[nxtPntNum] );
		RS_Vector tmpVec = tmpLine->getNearestPointOnEntity( vec );
		if( tmpVec.distanceTo( m_NCPoints[dataNum]) < maxDist )  //< ����ĵ�ֻ�����������߶��ϵĵ㣬���������յ�
		{
			if( tmpLine != NULL )
				delete tmpLine;
			if( tmpLine2 != NULL )
				delete tmpLine2;

			return lineNum;  //< ���ֱ���ҵ��Ǹ��㣬�Ͳ������κ���������
		}
		else
		{
			m_lPointCount++;
			RS_Vector * tmpPoints = new RS_Vector[m_lPointCount];
			for( int i = 0; i < m_lPointCount; ++i )
			{
				if( i < nxtPntNum )
				    tmpPoints[i] = m_NCPoints[i];
				else if( i == nxtPntNum )
					tmpPoints[i] = tmpVec;
				else
					tmpPoints[i] = m_NCPoints[i-1];
			}
			delete [] m_NCPoints;
			m_NCPoints = tmpPoints;

			lineNum = m_lineNumToDataIndex.key( nxtPntNum, -1 );
		}
	}
	else if( m_lPointCount - 1 == dataNum )
	{
		qDebug() << "liuyc datanum == m_lPointCount - 1...";
		prePntNum = dataNum - 1;
		if( prePntNum < 0 )  //< ���ֻ��һ���㣬�����ǰ��û�е㣬���������Խ��
			return -1;
		while( m_NCPoints[dataNum].distanceTo( m_NCPoints[prePntNum] ) < 10e-8 )
		{
			if( prePntNum <= 0 )
			{
				isLineValid = false;
				break;
			}

			prePntNum--;
		}

		if( !isLineValid )
			return -1;
		tmpLine = new RS_Line( m_NCPoints[dataNum], m_NCPoints[prePntNum] );
		RS_Vector tmpVec = tmpLine->getNearestPointOnEntity( vec );
		if( tmpVec.distanceTo( m_NCPoints[dataNum]) < maxDist )
		{
			if( tmpLine != NULL )
				delete tmpLine;
			if( tmpLine2 != NULL )
				delete tmpLine2;

			return lineNum;
		}
		else
		{
			m_lPointCount++;
			RS_Vector * tmpPoints = new RS_Vector[m_lPointCount];
			for( int i = 0; i < m_lPointCount; ++i )
			{
				if( i <= prePntNum )
					tmpPoints[i] = m_NCPoints[i];
				else if( i == prePntNum + 1 )
					tmpPoints[i] = tmpVec;
				else
					tmpPoints[i] = m_NCPoints[i-1];
			}
			delete [] m_NCPoints;
			m_NCPoints = tmpPoints;

			lineNum = m_lineNumToDataIndex.key( prePntNum + 1, -1 );
		}
	}
	else
	{
		//qDebug() << "liuyc: 0 < datanum < m_lPointCount - 1...";

		prePntNum = dataNum - 1;
		nxtPntNum = dataNum + 1;
		if( prePntNum < 0 || nxtPntNum >= m_lPointCount )  //< �����������Խ��
			return -1;

		//< ��ǰ��������ֱ��ҵ�ǰ���һ����ͬ�ĵ�
		while( m_NCPoints[dataNum].distanceTo( m_NCPoints[prePntNum] ) < 10e-8 )
		{
			if( prePntNum <= 0 )
			{
				isLineValid = false;
				break;
			}

			prePntNum--;
		}
		while( m_NCPoints[dataNum].distanceTo( m_NCPoints[nxtPntNum] ) < 10e-8 )
		{
			if( nxtPntNum >= m_lPointCount - 1 )
			{
				isLineValid = isLineValid || false;  //< ǰ��ֻҪ�ҵ�һ����ͬ�ĵ�Ϳ��ԣ�����Ҫ���߶��ҵ���ͬ��
				break;
			}

			nxtPntNum++;
		}

		//cout << "liuyc find nxtPntNum = " << nxtPntNum << " prePntNum = " << prePntNum << endl;
 		if( !isLineValid )
			return -1;

		tmpLine = new RS_Line( m_NCPoints[dataNum], m_NCPoints[prePntNum] );  //< ǰ��һ���߶�
		tmpLine2 = new RS_Line( m_NCPoints[dataNum], m_NCPoints[nxtPntNum] ); //< ����һ���߶�
		//cout << "pre point : " << m_NCPoints[prePntNum] << "\n"
			 //<< "cur point : " << m_NCPoints[dataNum] << "\n"
			// << "nxt point : " << m_NCPoints[nxtPntNum] << endl;
		double * tmpDist1 = new double( 0. );    //< checked leak
		double * tmpDist2 = new double( 0. );    //< checked leak
		RS_Vector tmpVec1 = tmpLine->getNearestPointOnEntity( vec, true, tmpDist1 );
		RS_Vector tmpVec2 = tmpLine2->getNearestPointOnEntity( vec, true, tmpDist2 );
		//cout << tmpVec1 << "    " << tmpVec2 << endl;
#define PRE_LINE  (-1)
#define NXT_LINE  1

		//< ȷ����Ӧ������һ���߶��ϣ��ҵ��������Ǹ��㣬��������Ҫ����ĵ�
		int curLineFlag = 0;
		RS_Vector curVec;
		if( (*tmpDist1) < (*tmpDist2) )
		{
			curLineFlag = PRE_LINE;
			curVec  = tmpVec1;
		}
		else
		{
			curLineFlag = NXT_LINE;
			curVec  = tmpVec2;
		}

		if( curVec.distanceTo( m_NCPoints[dataNum] ) < maxDist )
		{
	        //cout << "branch 1!" << endl;
			if( tmpLine != NULL )
				delete tmpLine;
			if( tmpLine2 != NULL )
				delete tmpLine2;

			delete tmpDist2, tmpDist1;
			return lineNum;  //< �ҵ��ĵ���ԴNC�����Ѿ����ˣ���ֱ�ӷ��أ����账������
		}
		else
		{
			//cout << "branch 2!" << endl;
			m_lPointCount++;
			RS_Vector * tmpPoints = new RS_Vector[m_lPointCount];
			int insertPntNum = -1;
			if( PRE_LINE == curLineFlag )
			    insertPntNum = prePntNum + 1;
			else if( NXT_LINE == curLineFlag )
				insertPntNum = nxtPntNum;

			for( int i = 0; i < m_lPointCount; ++i )
			{
				if( i < insertPntNum )
					tmpPoints[i] = m_NCPoints[i];
				else if( i == insertPntNum )
					tmpPoints[i] = curVec;
				else
					tmpPoints[i] = m_NCPoints[i-1];
			}
			delete [] m_NCPoints;
			m_NCPoints = tmpPoints;

			lineNum = m_lineNumToDataIndex.key( insertPntNum, -1 );
		}
		delete tmpDist2, tmpDist1;
	}

	//< �����к����ŵ�ӳ���ϵ
	cout << "liuyc insert new point of z!" << endl;
	insertFlag = INSERT_PNT;
	m_lNCLineCount++;
	int lastLineNum = m_lineNumToDataIndex.key( m_lineNumToDataIndex.size() - 1, -1);

	if( tmpLine != NULL )
		delete tmpLine;
	if( tmpLine2 != NULL )
		delete tmpLine2;

	//< �м������һ���㣬ӳ���ϵ�����˴�λ�����һ����û�ж�Ӧ���кţ���Ҫ��ӵ�MAP����ȥ
	if( lastLineNum != - 1 )
		m_lineNumToDataIndex[lastLineNum + 1] = m_lPointCount - 1;  
	else
		return -1;

	return lineNum;
}

void OffsetWdt::getSrcPoints( QString filename )
{
	char chFileName[1024];
	char chFileLine[1024];
	memset( chFileName, 0, 1024 ); //< ����ڴ�
	memcpy( chFileName, filename.toLocal8Bit().data(), (filename.toLocal8Bit()).length() );

	FILE * file = fopen( chFileName, "r" );  //< ��FILEָ������ļ���QFILEЧ�ʸߺܶ�
	if( file == NULL )
		return;

	//< ��ȡ��ǰNC����
	m_lNCLineCount = 0;
	while( fgets( chFileLine, 1024, file ) != NULL )
	{
		m_lNCLineCount++;
	}
	fclose( file );

	char chInput[3];
	chInput[0] = 'X';
	chInput[1] = 'Y';
	chInput[2] = 'Z';
	m_NCPoints = new RS_Vector[ m_lNCLineCount ];
	if( m_NCPoints != NULL )
	{
		getPntsFromNC( chFileName, NULL, chInput, m_lNCLineCount);
		qDebug() << "liuyc point first line = " << m_nSrcNCPntFirstLine;
		qDebug() << "liuyc point last line  = " << m_nSrcNCPntLastLine;
		qDebug() << "liuyc points num = " << m_lPointCount;
	}
}

bool OffsetWdt::getPntsFromNC( char *strFileName, char *strOutPutFile, QString chInput, int lTotalLines )
{
#ifdef Q_OS_LINUX
	timeval starttime, endtime;
	gettimeofday(&starttime, 0);
#endif

	bool bLoaded = true;

	//< step1 open the data file
	FILE *fp  = fopen(strFileName, "r");
	if( fp == NULL )
	{
		m_SrcContent.clear();
		m_SrcNCFile.clear();
		return false;
	}

	m_SrcContent.clear();
	m_lineNumToDataIndex.clear();
	LineNCInfo stVarLine,stVarLinePre;
	char charLine[1024];
	memset( charLine, 0, 1024 );

	long lReading   = -1;
	long lReadCount = 0 ; 
	bool bG91Flag   = false;
	bool bParseArc  = false;

	char charG0123   = '0'; // default G00.
	char charG171819 = '7'; // default G17.
	char charClockwise = '1'; // default clockwise.
	char charG404142 = '0';

	char* pcharD = NULL;
	char* pcharF = NULL;
	char* pcharX = NULL;
	char* pcharY = NULL;
	char* pcharZ = NULL;
	char* pcharE = NULL;


	char* pchMTC = NULL;
	bool  bMTCF		  = false;
	bool  bHeadFinish = false;
	bool  bFindX	  = false;
	bool  bFindY	  = false;
	bool  bFindZ      = false;
	bool  bG01Find	  = false;
	bool  bErrHappen1  = false;
	bool  bErrHappen2  = false;

	double dLastF = 0.0;
	double dLastX = 0.0;
	double dLastY = 0.0;
	double dLastZ = 0.0;
	double dLastE = 0.0;
	double dLastPreF = 0.0;
	double dLastPreX = 0.0;
	double dLastPreY = 0.0;
	double dLastPreZ = 0.0;
	double dLastPreE = 0.0;

	double dProgress = 0.0;
	double dARadio   = 1;
	double dMRadio   = 1;
	int  iG0Count   = 0;
	int  iG123Count = 0;
	bool bHead      = true;
	bool bTail		= true;

	int  iPreProgress = 0;
	char cccxx[2],cccyy[2],ccczz[2];
	char cccXX[2],cccYY[2],cccZZ[2];
	cccXX[0] = (chInput.toUpper().toAscii())[0];
	cccYY[0] = (chInput.toUpper().toAscii())[1];
	cccZZ[0] = (chInput.toUpper().toAscii())[2];
	cccxx[0] = (chInput.toLower().toAscii())[0];
	cccyy[0] = (chInput.toLower().toAscii())[1];
	ccczz[0] = (chInput.toLower().toAscii())[2];
	cccXX[1] = '\0';
	cccYY[1] = '\0';
	cccZZ[1] = '\0';
	cccxx[1] = '\0';
	cccyy[1] = '\0';
	ccczz[1] = '\0';

	m_nSrcNCPntFirstLine = 0;
	m_nSrcNCPntLastLine = 0;

	while( fgets( charLine, 1024, fp ) != NULL )
	{
		//< ���ļ��кŴӵ�0�п�ʼ
		lReading++;
		m_SrcContent.append( QString::fromLocal8Bit( charLine ) );

		//< ��(���������ע��, ע�Ͳ�����
		if ((pchMTC = str_srch2(charLine, "(", "(")) != NULL)
		{
			*pchMTC = '\0';
		}

		//< bHeadFinish��ʾ�Ƿ������һ��G01��Ҳ�����Ѷ����ļ�ͷ��֮����� "M"���ҵ�M30��ֱ���������ټ�����
		if( ( pchMTC = str_srch2(charLine, "M", "m") ) != NULL && bHeadFinish)
		{
			//Have M
			pchMTC = str_srch2(charLine, "M30", "m30");
			if(pchMTC == NULL)
			{
				bMTCF = true;
			}
			else 
			{	//M30 NC End
				//< �������������ж�M30ֱ�Ӳ��ټ�������ȥ������������������ܻ����M30֮���һ�����У�����������֮ǰ�����������٣���
				break;
			}
			continue;
		}
		//find "T"
		if( (pchMTC = str_srch2(charLine, "T", "t") ) != NULL && bHeadFinish)
		{
			//Have T
			bMTCF = true;
			continue;
		}
		if( bMTCF )  //< ��ʱ��֧��M/Tָ��
		{
			QMessageBox msgBox;
			char chTimeS[1024];
			memset( chTimeS, 0, 1024);
			/*if( !bErrHappen1 )
			{
				sprintf( chTimeS, "The Current Version Not Support M or T code In The Middle of The File.\n���ڵİ汾��ʱ��֧���ļ��м���T����Mָ��\nLine:%d", lReading);
				msgBox.setText(chTimeS);
				msgBox.exec();
				bErrHappen1 = true;
			}*/
			//break;
		}

		//< ��G28��G28�е�X,Y,Zֵ 
		if( ( pchMTC = str_srch2(charLine, "G28", "g28") ) != NULL)
		{
			pchMTC = str_srch2(charLine, "X", "x");
			if(pchMTC != NULL)
				*pchMTC= 'A';
			pchMTC = str_srch2(charLine, "Y", "y");
			if(pchMTC != NULL)
				*pchMTC= 'A';
			pchMTC = str_srch2(charLine, "Z", "z");
			if(pchMTC != NULL)
				*pchMTC= 'A';
			continue;
		}
		if( bG91Flag )
		{
			bG91Flag = !(str_srch2(charLine, "G90", "g90") != NULL);
		}
		else // NOT G91 flag.
		{
			bG91Flag = (str_srch2(charLine, "G91", "g91") != NULL);
		}

		while(  (pcharD = str_srch2(charLine, "D", "d")) != NULL )
		{
			*pcharD = ' ';
		}

		//< ��G17/18/19 G17��ӦXYƽ�棬G18��ӦZXƽ�棬G19��ӦYZƽ��
		if( str_srch2(charLine, "G17", "g17") != NULL )
		{
			charG171819 = '7'; // plane XY.
		}
		else if( str_srch2(charLine, "G18", "g18") != NULL )
		{
			charG171819 = '8'; // plane ZX.
		}
		else if( str_srch2(charLine, "G19", "g19") != NULL )
		{
			charG171819 = '9'; // plane YZ.
		}

		//< ��G40/G41/G42
		if( str_srch2(charLine, "G40", "g40") != NULL )
		{
			charG404142 = '0';
		}
		else if( str_srch2(charLine, "G41", "g41") != NULL )
		{
			charG404142 = '1';
		}
		else if( str_srch2(charLine, "G42", "g42") != NULL )
		{
			charG404142 = '2';
		}

		//< ��G00/G01/G02/G03 ��ͬ�Ĳ岹��ʽ
		if( CheckG0123(charLine, '0') )
		{
			charG0123 = '0'; //< LINE: Moving(�����ƶ�)
		}
		else if( CheckG0123(charLine, '1') )
		{
			charG0123 = '1'; //< LINE: Cutting(ֱ�߲岹)
		}
		else if( CheckG0123(charLine, '2') )
		{
			charG0123 = '2'; //< ARC: Cutting clockwise.
			charClockwise = '1';
		}
		else if( CheckG0123(charLine, '3') )
		{
			charG0123 = '3'; //< ARC: Cutting anti-clockwise.
			charClockwise = '0';
		}

		//< �ϳ��ٶ�F
		pcharF = str_srch2(charLine, "F", "f");
		if( pcharF != NULL )
		{
			dLastF = atof(pcharF+1);
			m_dNCFileFspd = dLastF;
		}

		//< �ҵ��������X����û����ֱ��������һ�ε�X
		pcharX = str_srch2(charLine, cccXX, cccxx);
		(chInput[0]);
		if( pcharX != NULL )
		{
			bFindX = true;
			if( bG91Flag )
				dLastX += atof(pcharX+1);
			else // NOT G91 flag.
				dLastX = atof(pcharX+1);
		}

		pcharY = str_srch2(charLine, cccYY, cccyy);
		if( pcharY != NULL )
		{
			bFindY = true;
			if( bG91Flag )
				dLastY += atof(pcharY+1);
			else // NOT G91 flag.
				dLastY = atof(pcharY+1);
		}

		//pcharZ = str_srch2(charLine, "Z", "z");
		pcharZ = str_srch2(charLine, cccZZ, ccczz);
		if( pcharZ != NULL )
		{
			bFindZ = true;
			if( bG91Flag )
				dLastZ += atof(pcharZ+1);
			else // NOT G91 flag.
				dLastZ = atof(pcharZ+1);
		}

		pcharE = str_srch2(charLine, "E", "e");
		if( pcharE != NULL )
		{
			if( bG91Flag )
				dLastE += atof(pcharE+1);
			else // NOT G91 flag.
				dLastE = atof(pcharE+1);
		}

		if( charG0123 == '0' )
		{
			iG0Count += 1;
		}
		else // G01, G02, G03.
		{
			iG123Count += 1;
		}

		if( charG0123 == '2' || charG0123 == '3' )
		{
			if(!bErrHappen2)
			{
				QMessageBox msgBox;
				QString msg = tr("The Current Version Not Support Arc.\n���ڵİ汾��ʱ��֧���ļ�����Բ��ָ�\nLine:")
					+ QString::number( lReading );
				msgBox.setText( msg );
				msgBox.exec();
				bErrHappen2 = true;
			}
		}

		stVarLine.vPos[0]  = dLastX;
		stVarLine.vPos[1]  = dLastY;
		stVarLine.vPos[2]  = dLastZ;

		//< ֻ��X,Y,Zȫ����������֮�󣬲��ܿ�ʼ��¼��λ��Ϣ��֮ǰ�ĵ�λ��Ϣ����ȫ
		if(bFindX && bFindY && bFindZ)
		{
			//if( m_nSrcNCPntFirstLine != 0 && charG0123 != '1')  //< �Ѿ��������
			//{
			//	dLastPreX = dLastX;
			//	dLastPreY = dLastY;
			//	dLastPreZ = dLastZ;
			//	dLastPreE = dLastE;
			//	dLastPreF = dLastF;
			//	continue;
			//}

			//< Ŀǰֻ����G00/G01��Բ��ָ�֧��
			if(charG0123 == '1'/* || charG404142 != '0'*/)  //< ��ʱ���Ǳ���G01Ϊ��һ��
			{
				bHeadFinish = true;
				if( m_nSrcNCPntFirstLine == 0 )
					m_nSrcNCPntFirstLine = lReading;
			}
			else// if( charG0123 == '0' && bHeadFinish )
			{
				dLastPreX = dLastX;
				dLastPreY = dLastY;
				dLastPreZ = dLastZ;
				dLastPreE = dLastE;
				dLastPreF = dLastF;
				continue;
			}
		}
		else
		{
			dLastPreX = dLastX;
			dLastPreY = dLastY;
			dLastPreZ = dLastZ;
			dLastPreE = dLastE;
			dLastPreF = dLastF;
			continue;
		}

		stVarLine.lReading = lReading;
		stVarLine.dF       = dLastF;
		stVarLine.chG0123  = charG0123;
		if( (charG0123 == '2' || charG0123 == '3') && lReadCount >= 1 )
		{
			// now try to get the arc center.
			// try to read the 'R' code or 'I'/'J'/'K' code from charLine.
			//bParseArc = ParseTheArc(&stVarLine,charLine, charClockwise, charG171819,stVarLinePre.vPos,stVarLine.vPos);
			//if( bParseArc == false )
			//{
				//stVarLine.chG0123 = '0';
				//stVarLine.dR      = -1.0;
			//}
		}
		else // not an arc.
		{
			stVarLine.dR = -1.0;
		}

		if( stVarLine.dF < 1.0E-3)
		{
			//continue;
		}
		if( stVarLine.chG0123 <= 48 + 1)
		{
			stVarLine.chG0123 = 48 + 1;
		}

		m_NCPoints[lReadCount].x = stVarLine.vPos[0];
		m_NCPoints[lReadCount].y = stVarLine.vPos[1];
		m_NCPoints[lReadCount].z = stVarLine.vPos[2];

		dLastPreX = dLastX;
		dLastPreY = dLastY;
		dLastPreZ = dLastZ;
		dLastPreE = dLastE;
		dLastPreF = dLastF;

		m_lineNumToDataIndex[ lReading ] = lReadCount;
		m_nSrcNCPntLastLine = lReading;

		stVarLinePre = stVarLine;
		lReadCount += 1;
	}

	m_lPointCount = lReadCount;  //< ��λ��
	RS_Vector * tmpPoints = new RS_Vector[m_lPointCount];
	for( int i = 0; i < m_lPointCount; ++i )
	{
		tmpPoints[i] = m_NCPoints[i];
	}
    delete [] m_NCPoints;
	m_NCPoints = tmpPoints;

#ifdef Q_OS_LINUX
	gettimeofday( &endtime, 0 );
	double timeuse = 1000000. * (endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
	timeuse *= 0.001 ;
	std::cout << "liuyc nc get points : " << timeuse << "ms" << endl;
#endif
	// close the data file
	fclose(fp);
	return true;
}


char* OffsetWdt::str_srch2(char* charString, char* charSub1, char* charSub2 )
{
	char* charSearch = strstr(charString, charSub1);
	if( charSearch == NULL )
	{
		charSearch = strstr(charString, charSub2);
	}
	return charSearch;
}

bool OffsetWdt::CheckG0123(char* charString, char char0123)
{
	char charG0123[4];
	charG0123[1] = '0';
	charG0123[2] = char0123;
	charG0123[3] = '\0';

	//< ����G00/G01/G02/G03
	charG0123[0] = 'G';
	if( strstr(charString, charG0123) != NULL )
	{
		return true;
	}
	// try to find g00/1/2/3.
	charG0123[0] = 'g';
	if( strstr(charString, charG0123) != NULL )
	{
		return true;
	}

	// maybe the first '0' has been omitted.
	charG0123[1] = charG0123[2];
	charG0123[2] = charG0123[3];

	// try to find G0/1/2/3.
	charG0123[0] = 'G';
	char* pcharFind = strstr(charString, charG0123);
	if( pcharFind != NULL )
	{ // g0/1/2/3 has been found.
		// check it is "g0/1/2/3"+"0~9" or not.
		if( pcharFind[2] < '0' || pcharFind[2] > '9' )
		{ // it is g0/1/2/3 indeed.
			return true;
		}
		return false;
	}
	// try to find g0/1/2/3.
	charG0123[0] = 'g';
	pcharFind = strstr(charString, charG0123);
	if( pcharFind != NULL )
	{ // g0/1/2/3 has been found.
		// check it is "g0/1/2/3"+"0~9" or not.
		if( pcharFind[2] < '0' || pcharFind[2] > '9' )
		{ // it is g0/1/2/3 indeed.
			return true;
		}
		return false;
	}
	return false;
}

void OffsetWdt::slotRemoveOffsetXY()
{
	//< step1 �ж������Ƿ�Ϸ����Ƿ��������������Χ���غϵ�����
	g_InputMode = ui_input;

	if( !isIndexLegal( RM_Axis_XY ) )
		return;

	qDebug() << "liuyc: des xy : " << g_XYDesDataBeginNum << " : " << g_XYDesDataEndNum;

	//< step2 ����ƫ��
	rmOffsetXY( g_CalOff[0], g_CalOff[1], g_CalRadOff );
	
}

void OffsetWdt::rmOffsetXY( double xoff, double yoff, double angleoff )
{
	for( int i = 0; i < g_XYDesDataCount; ++i )
	{
		//< ��ȥ��X/Yƫ��
		g_SavedData[i+g_XYDesDataBeginNum].x = 
			g_SavedData[i+g_XYSrcDataBeginNum].x + g_QTableExtraDataMap[i+g_XYSrcDataBeginNum].QMicroAdjust.x - xoff;
		g_SavedData[i+g_XYDesDataBeginNum].y = 
			g_SavedData[i+g_XYSrcDataBeginNum].y + g_QTableExtraDataMap[i+g_XYSrcDataBeginNum].QMicroAdjust.y - yoff;

		//< ��ȥ��ƫ��
		g_SavedData[i+g_XYDesDataBeginNum].rotate( -angleoff );
		cout << "points without offset: " << g_SavedData[i+g_XYDesDataBeginNum] << endl;
	}

	//< end
	qDebug() << "liuyc: xy remove offset finished!";
	m_bHasRmXY = true;

	g_bHasRmOff = m_bHasRmXY && m_bHasRmZ;
	return;
}

void OffsetWdt::slotRemoveOffsetZ()
{
	g_InputMode = ui_input;

	if( !isIndexLegal( RM_Axis_Z ) )
		return;

	qDebug() << "liuyc: des z : " << g_ZDesDataBeginNum << " : " << g_ZDesDataEndNum;

	rmOffsetZ( g_CalOff[2] );
	return;
}

void OffsetWdt::rmOffsetZ( double zoff )
{
	for( int i = 0; i < g_ZDataCount; ++i )
	{
		g_SavedData[i+g_ZDesDataBeginNum] = g_SavedData[i+g_ZSrcDataBeginNum];

		g_SavedData[i+g_ZDesDataBeginNum].z = g_SavedData[i+g_ZSrcDataBeginNum].z
			+ g_QTableExtraDataMap[i+g_XYSrcDataBeginNum].QMicroAdjust.z - zoff;  //< ֻ��Z��ȥ��ƫ��������ȡ������
		cout << "points without offset: " << g_SavedData[i+g_ZDesDataBeginNum] << endl;
	}

	qDebug() << "liuyc: z remove offset finished!";
	m_bHasRmZ = true;
	g_bHasRmOff = m_bHasRmXY && m_bHasRmZ;
	return;
}

/************************************************************************/
/* �ۺ��������ý����ϵ�DXF�ļ���
/************************************************************************/
void OffsetWdt::slotSetDXFFile( QString & filename )
{
	m_dxfFilename = filename;
	//qDebug() << "liuyc open dxf file : " << m_dxfFilename;
	if( m_dxfFilename != "" && m_SrcNCFile != "" )
	{
		m_NewBtn[FILE_NEW]->setEnabled( true );
	}
	m_ReadFnameLbl->setText( tr("��ǰ�ļ����� ") 
		+ "<font color=#0000FF style=\"font-weight:bold;\">" + filename.section( '/', -1, -1 ) + "</font>");

	readConfig();
}

void OffsetWdt::slotGetNewContainer(RS_EntityContainer * cont)
{
	container = cont;
	g_ContainerPtr = cont;
	if( container->isEmpty() )
	{
		g_bIsDxfRdy = false;
		return;
	}

	g_bIsDxfRdy = true;
    
	if( isDxfToNC() != 1 )
	{
		m_NewBtn[FILE_NEW]->setEnabled( false );
	}
	else
	{
		m_NewBtn[FILE_NEW]->setEnabled( true );
		emit sglSetMsg( tr("  NC��ȡ��ƥ��ɹ���") );
	}
}

void OffsetWdt::slotCountChanged()
{
	//qDebug() << "liuyc: offset widget show!";
	//qDebug() << "liuyc: xy count = " << g_XYDesDataCount;
	//qDebug() << "liuyc: z  count = " << g_ZDataCount;

	m_RmPntCntLbl[RM_Axis_XY]->setText( tr("������ ") + "<font color=#0000FF style=\"font-weight:bold;\">"
		+ QString::number( g_XYDesDataCount ) + "</font>" + " " );
	m_RmPntCntLbl[RM_Axis_Z]->setText( tr("������ ") + "<font color=#0000FF style=\"font-weight:bold;\">"
		+ QString::number( g_ZDataCount ) + "</font>" + " " );

	m_RmMacroLEdit[RM_Axis_XY]->setEnabled(  g_XYDesDataCount > 0  );
	m_RmBtn[RM_Axis_XY]->setEnabled(  g_XYDesDataCount > 0 );
	//< ���XY������Ϊ0����ֱ���趨���Ƴ��ı�ʶΪ��
	m_bHasRmXY = ( g_XYDesDataCount <= 0 );

	m_RmMacroLEdit[RM_Axis_Z]->setEnabled( g_ZDataCount > 0 );
	m_RmBtn[RM_Axis_Z]->setEnabled( g_ZDataCount > 0 );
	m_bHasRmZ = ( g_ZDataCount <= 0 );

	g_bHasRmOff = m_bHasRmZ && m_bHasRmXY;
}

void OffsetWdt::slotFormatManualOff()
{
	QLineEdit * tmpLineEdit = dynamic_cast<QLineEdit *>( sender() );
	tmpLineEdit->setText( QString::number( tmpLineEdit->text().toFloat(), 'f', 3 ) );
}

int OffsetWdt::isDxfToNC()
{
	double tollerentError = 1.;

	if( !g_bIsDxfRdy || !g_bIsNCRdy )
		return -1;  //< -1��ʾƥ����������

	if( m_NCPoints == NULL )
		return -1;

	//< ����м�70%�ĵ�ľ������Ƿ���ͬ������Ϊԭ������0.7
	int judgePntCnt = static_cast<int>( static_cast<double>( m_lPointCount ) * 0.7 );
	if( judgePntCnt <= 0 )
		return -1;

	RS_Vector tmpVec;
	int headerPntCnt = static_cast<int>( ( m_lPointCount - judgePntCnt ) * 0.5 );
	double * dist = new double[judgePntCnt];
	double distAvg = 0.;
	double * tmpDist = new double( 0. );  //< ����һ����ʱ�ľֲ���������ֹ�����Ǹ���������ջ���
	for( int i = 0; i < judgePntCnt; ++i )
	{
		tmpVec = container->getNearestPointOnEntity( m_NCPoints[i+headerPntCnt], true, tmpDist );
		//cout << "liuyc: nearset point = " << tmpVec << endl;
		//< ��̽����DXFʵ���ϵĵ�����ͬһƽ���ϣ�Ȼ���������Ǵ����XYƽ���ϵľ�����������������ľ�������׼ȷ
		tmpVec.z = m_NCPoints[i+headerPntCnt].z;  
		dist[i] = tmpVec.distanceTo( m_NCPoints[i+headerPntCnt] );
		distAvg += dist[i];
		//qDebug() << "liuyc: dist = " << dist[i];
	}
	delete tmpDist;
	distAvg /= static_cast<double>( judgePntCnt );
	for( int i = 0; i < judgePntCnt; ++i )
	{
		if( fabs( dist[i] - distAvg ) >= tollerentError )
		{
			qDebug() << "liuyc dxf related to NC failed!";
			if( g_InputMode == ui_input )
			    popMessageBox( "Lynuc CAD Warning", tr("DXF�ļ���NC�ļ���ƥ�䣬��ȷ�Ϻ����´�DXF����ԭNC�ļ���"), 5, QColor(255, 0, 0), true );
			return 0;  //< 0��ʾƥ��ʧ��
		}
	}
	delete [] dist;

	qDebug() << "liuyc dxf related to NC succeed!";
	qDebug() << "nc d offset = " << QString::number( distAvg, 'd', 6 );
	m_NewBtn[FILE_NEW]->setEnabled( true );
	m_dNCFileROff = distAvg;

	//< step2 NC��DXFƥ��ɹ�����Ҫ�ҵ����˵���
	for( int i = _leftB; i <= _btmE; ++i )
	{
		if( g_Index[i] == 0 || g_XYIndex[i] == 0 )  //< �м���һ����Ų�û������
		{
			m_bIsClose = false;
			return 1;
		}
	}

	m_bIsClose = true;
	qDebug() << __FUNCTION__ << " : liuyc : finding cut in point!";
	int tmpSum = 0;  //< ��������5���������Ⱦ��ж���һ����ģ����
	tmpDist = new double( 0. );
	double * tmpDist2 = new double( 0. );  //< ��һ�������еľ���
	for( int i = 0; i < m_lPointCount; ++i )
	{
	    tmpVec = container->getNearestPointOnEntity( m_NCPoints[i], true, tmpDist );  //< ��ȡ����
		if( fabs( *tmpDist - m_dNCFileROff ) < 0.001 )  //< ���������DXF�ľ������ƥ��ľ���, 1um
		{
			tmpSum = 1;
			m_nCutFirstPointIndex = i;
			qDebug() << "liuyc : find first point index";
			for( int j = m_nCutFirstPointIndex + 1;
				j < m_nCutFirstPointIndex + 5 && j < m_lPointCount; 
				++j )
			{
				//++i;  //< ������5����
				tmpVec = container->getNearestPointOnEntity( m_NCPoints[j], true, tmpDist2 );  //< ��ȡ����
				qDebug() << "m_dNCFileROff = " << m_dNCFileROff << "tmpDist = " << *tmpDist << "tmpDist2 = "
					 << *tmpDist2;
				if( fabs( *tmpDist2 - m_dNCFileROff ) < 0.001 )
					tmpSum++;

				//qDebug() << "tmpSum = " << tmpSum;
			}
			if( tmpSum >= 5 )  //< ����5��������5�������㣬���϶��ҵ�������
			{
				break;
			}
			else
				m_nCutFirstPointIndex = -1;  //< ���������ң�ֱ���ҵ�����5���㶼����Ҫ���
		}
	}
	if( m_nCutFirstPointIndex < 0 )
	{
		qDebug() << "liuyc: find first cut point failed";
		m_nCutFirstPointIndex = 0;
	}
	else
		qDebug() << "liuyc: m_nCutFirstPointIndex = " << m_nCutFirstPointIndex;

	tmpSum = 0;
	for( int i = m_lPointCount - 1; i >= 0; --i )
	{
		tmpVec = container->getNearestPointOnEntity( m_NCPoints[i], true, tmpDist );  //< ��ȡ����
		if( fabs( *tmpDist - m_dNCFileROff ) < 0.001 )  //< ���������DXF�ľ������ƥ��ľ��� 1um
		{
			tmpSum = 1;
			m_nCutLastPointIndex = i;
			for( int j = m_nCutLastPointIndex - 1; j > m_nCutLastPointIndex - 5 && j >= 0; --j )
			{
				tmpVec = container->getNearestPointOnEntity( m_NCPoints[j], true, tmpDist2 );  //< ��ȡ����
				if( fabs( *tmpDist2 - m_dNCFileROff ) < 0.001 )
					tmpSum++;
			}
			if( tmpSum >= 5 )  //< ����5��������5�������㣬���϶��ҵ��˵���
				break;
			else
				m_nCutFirstPointIndex = -1;
		}
	}
	if( m_nCutLastPointIndex < 0 )
	{
		qDebug() << "liuyc: find last cut point failed";
		m_nCutLastPointIndex = m_lPointCount - 1;
	}
	else
		qDebug() << "liuyc: m_nCutLastPointIndex = " << m_nCutLastPointIndex;

	delete tmpDist2;
	delete tmpDist;

	return 1;  //< 1��ʾƥ��ɹ�
}

bool OffsetWdt::isIndexLegal( int axisIndex, int inputmd )
{
	int tmpBegin = 0;
	int tmpEnd = 0;
	if( inputmd == ui_input)
	{
		tmpBegin = m_RmMacroLEdit[axisIndex]->text().toInt();
		if( axisIndex == RM_Axis_XY )
			tmpEnd   = tmpBegin + g_XYDesDataCount - 1;
		else if( axisIndex == RM_Axis_Z )
			tmpEnd   = tmpBegin + g_ZDataCount - 1;
		else
			return false;
	}
	else if( inputmd == motion_input )  //< �����motion����ģ�����Ҫ�ж�ԭ����index�ĺϷ���
	{
		tmpBegin = getMacroInt( MACRO_P151L60_Q );
		tmpEnd   = getMacroInt( MACRO_P151L60_R );
	}
	else
		return false;

	//< step1 �ж������Ƿ�Ϸ�
	//< ��������������⣺1. ��ʼ������С��1  2.���ϵ�����Ϣ�󣬱�����Χ����
	if( tmpBegin < G100_INDEX_MIN || tmpEnd < G100_INDEX_MIN || tmpEnd > G100_INDEX_MAX )
	{
		qDebug() << "liuyc: des input ilegal! ";
		return false;
	}

	//< ���������������ݸ���ԭ����
	if( g_XYSrcDataCount != 0 && g_XYSrcDataBeginNum != 0 && g_XYSrcDataEndNum != 0 )
	{
		if( (g_XYSrcDataBeginNum >= tmpBegin && g_XYSrcDataBeginNum <= tmpEnd )
			|| ( g_XYSrcDataEndNum >= tmpBegin && g_XYSrcDataEndNum <= tmpEnd )
			|| ( tmpBegin >= g_XYSrcDataBeginNum && tmpBegin <= g_XYSrcDataEndNum )
			|| ( tmpEnd >= g_XYSrcDataBeginNum && tmpEnd <= g_XYSrcDataEndNum ))
		{
			//< ���������ԭ���ݵķ�Χ�г�ͻ
			qDebug() << "liuyc : area is ilegal!";
			return false;
		}
	}

	if( g_ZDataCount != 0 && g_ZSrcDataBeginNum != 0 && g_ZSrcDataEndNum != 0 )  //< ���ж������жϵ�
	{
		if( ( g_ZSrcDataBeginNum >= tmpBegin && g_ZSrcDataBeginNum <= tmpEnd )
			||( g_ZSrcDataEndNum >= tmpBegin && g_ZSrcDataEndNum <= tmpEnd )
			||( tmpBegin >= g_ZSrcDataBeginNum && tmpBegin <= g_ZSrcDataEndNum )
			||( tmpEnd >= g_ZSrcDataBeginNum && tmpEnd <= g_ZSrcDataEndNum ))
		{
			//< ���������ԭ���ݵķ�Χ�г�ͻ
			qDebug() << "liuyc : area is ilegal!";
			return false;
		}
	}

	switch ( axisIndex )
	{
	case RM_Axis_XY:
		if( g_ZDesDataBeginNum != 0 && g_ZDesDataEndNum != 0 )
		{
			if( ( g_ZDesDataBeginNum >= tmpBegin && g_ZDesDataBeginNum <= tmpEnd )
				||( g_ZDesDataEndNum >= tmpBegin && g_ZDesDataEndNum <= tmpEnd )
				||( tmpBegin >= g_ZDesDataBeginNum && tmpBegin <= g_ZDesDataEndNum )
				||( tmpEnd >= g_ZDesDataBeginNum && tmpEnd <= g_ZDesDataEndNum ))
			{
				//< ���������ԭ���ݵķ�Χ�г�ͻ
				qDebug() << "liuyc : area is ilegal!";
				return false;
			}
		}

		g_XYDesDataBeginNum = tmpBegin;
		g_XYDesDataEndNum   = tmpEnd;
		break;

	case RM_Axis_Z:
		if( g_XYDesDataBeginNum != 0 && g_XYDesDataEndNum != 0 )
		{
			if( ( g_XYDesDataBeginNum >= tmpBegin && g_XYDesDataBeginNum <= tmpEnd )
				||( g_XYDesDataEndNum >= tmpBegin && g_XYDesDataEndNum <= tmpEnd )
				||( tmpBegin >= g_XYDesDataBeginNum && tmpBegin <= g_XYDesDataEndNum )
				||( tmpEnd >= g_XYDesDataBeginNum && tmpEnd <= g_XYDesDataEndNum ))
			{
				//< ���������ԭ���ݵķ�Χ�г�ͻ
				qDebug() << "liuyc : area is ilegal!";
				return false;
			}
		}

		g_ZDesDataBeginNum = tmpBegin;
		g_ZDesDataEndNum   = tmpEnd;
		break;
	default:
		return false;
	}

	return true;
}

bool OffsetWdt::isRdyToGen()
{
	if( !g_bIsRadRdy )
	{
		popMessageBox( "Lynuc CAD Warning", tr("û��̽��İ뾶���ݣ�������̽��뾶���س�ȷ�ϣ�"), 5, QColor(255, 0, 0)
			, true );
		return false;
	}
	if( !g_bIsNCRdy )
	{
		popMessageBox( "Lynuc CAD Warning", tr("δѡ��ԭNC�ļ��������������ѡ��ԭNC�ļ�"), 5, QColor(255, 0, 0), true );
		return false;
	}
	if( !g_bIsDxfRdy )
	{
		popMessageBox( "Lynuc CAD Warning", tr("δѡ��DXF�ļ�!"), 5, QColor(255, 0, 0), true );
		return false;
	}
	if( !g_bHasRmOff )
	{
		popMessageBox( "Lynuc CAD Warning", tr("������ƫ�ơ����ߡ�����ƫ�ơ�����δ���!"), 5, QColor(255, 0, 0), true );
		return false;
	}

	if( m_dNCFileROff <= 0. )
	{
		qDebug() << "liuyc: src nc file has a r offset that is less than 0.";
		popMessageBox( "Lynuc CAD Warning", tr("�������ԭʼNC������Ϣ�쳣��"), 5, QColor(255, 0, 0), true );
		return false;
	}
	qDebug() << "liuyc: src nc file has a r offset: " << m_dNCFileROff;

	if( g_XYDesDataCount <= 0 && g_ZDataCount <= 0 )
	{
		popMessageBox( "Lynuc CAD Warning", tr("û������ƫ�ƺ�����ݣ���ȷ������ƫ�ƺ��ٵ�����ɰ�ť!"), 5, QColor(255, 0, 0), true );
		return false;
	}

	return true;
}

void OffsetWdt::saveConfig()
{
	cout << "liuyc: save data start!" << endl;

    QString configFile;
#ifdef Q_OS_WIN
	configFile = "E:/";  //< Ŀ¼
#else
	configFile = "/home/Lynuc/Users/Config/";
#endif
	configFile += "CAD_GGBC_cfg.xml";  //< �ļ���

	QFile file( configFile );
	if( !file.exists() )  //< ���û�д��ļ���ͨ��ֻд�򿪵ķ�ʽ�����ļ�
	{
		if( !file.open( QIODevice::WriteOnly ) )
			return;
		file.close();
	}

	//< ���ļ�
	if( !file.open( QIODevice::ReadOnly ) )
	{
		cout << "liuyc read file failed!" << endl;
		return;
	}
	QDomDocument doc;
	//< �ļ�Ϊ�գ��ļ���СΪ0��
	if( file.size() == 0 )
	{
		qDebug() << "file size = 0 !";
		file.close();
		QString strHeader( "version=\"1.0\" encoding=\"UTF-8\"" );  
		doc.appendChild( doc.createProcessingInstruction("xml", strHeader) );  

		QDomElement root_elem = doc.createElement( "G-Speed" );  
		doc.appendChild( root_elem ); 
	}
	else
	{
		if( !doc.setContent( &file ) )
		{
			cout << "liuyc doc set content failed!" << endl;
			file.close();
			return;
		}

		file.close();
	}

	QDomElement root = doc.documentElement();
	if (root.tagName() != "G-Speed" || root.isNull()) //< ������ǩ����ʽ������ֱ�ӷ���
	{
		return;
	}

	QString tmpDxfFile = NULL;
	QString tmpNCFile  = NULL;
	QString tagName = NULL;
	QDomNodeList nodes_1st = root.childNodes();  //< ��һ���ڵ�(GGBC_SET)
	QDomElement element_1st;
	QList<int> needToDel;
	for(int i = 0; i < nodes_1st.count(); ++i)
	{
		element_1st = nodes_1st.at(i).toElement();
		tagName = element_1st.tagName();
		if(element_1st.tagName() == "GGBC_SET")  //< �ļ�
		{
			tmpDxfFile = element_1st.attribute( "dxf_file" );
			tmpNCFile = element_1st.attribute( "src_nc_file" );
			if( m_dxfFilename.compare( tmpDxfFile, Qt::CaseInsensitive ) != 0 )
			{
				element_1st.setAttribute( "is_cur", "false" );
			}
			else
			{
				//< ������ָ��ļ�����Ϣ��ֱ��ɾ��
				needToDel.prepend( i );
			}
		}
		else if( tagName.compare( "GLOBAL_SET", Qt::CaseInsensitive ) == 0 )
		{
			//qDebug() << "liuyc remove GLOBAL_SET node!";
			needToDel.prepend( i );
		}
		else
		{
			qDebug() << "liuyc : child node is wrong!";
		}
	}

	for( int i = 0; i < needToDel.count(); ++i )
	{
		//qDebug() << "liuyc: need to del " << needToDel.at(i);
		root.removeChild( nodes_1st.at( needToDel.at(i) ) );
	}

	QDomElement newGlbElem = doc.createElement( "GLOBAL_SET" );
	newGlbElem.setAttribute( "det_radius", g_DecRadius );
	root.appendChild( newGlbElem );

	QDomElement newElem = doc.createElement( "GGBC_SET" );
	newElem.setAttribute( "dxf_file", m_dxfFilename );
	newElem.setAttribute( "src_nc_file", m_SrcNCFile );
	newElem.setAttribute( "is_cur" , "true" );
	root.appendChild( newElem );

	QString _xy_dirStr[8] = 
	{
		"left",
		"right",
		"top",
		"btm",
		"left_top",
		"right_top",
		"left_btm",
		"right_btm",
	};
	for( int i = _leftB; i < _rightBtmE; i += 2 )  //< XY
	{
		if( g_XYIndex[i] != 0 && g_XYIndex[i+1] != 0 )
		{
			QDomElement offElement = doc.createElement( "INDEX_SRC" );
			offElement.setAttribute( "dir", _xy_dirStr[i/2] );
			offElement.setAttribute( "index_min", g_XYIndex[i]);
			offElement.setAttribute( "index_max", g_XYIndex[i+1]);
			newElem.appendChild( offElement );
		}
	}
	if( g_ZIndex[0] != 0 && g_ZIndex[1] != 0)    //< Z
	{
		QDomElement offElement = doc.createElement( "INDEX_SRC" );
		offElement.setAttribute( "dir", "z" );
		offElement.setAttribute( "index_min", g_ZIndex[0]);
		offElement.setAttribute( "index_max", g_ZIndex[1]);
		newElem.appendChild( offElement );
	}

	QDomElement desDffElement = doc.createElement( "INDEX_DES" );
	desDffElement.setAttribute( "xy_begin", g_XYDesDataBeginNum);
	desDffElement.setAttribute( "z_begin", g_ZDesDataBeginNum);
	newElem.appendChild( desDffElement );

	QDomElement manOffElement = doc.createElement( "MAN_OFF" );
	manOffElement.setAttribute( "x", QString::number( g_ManOff[0], 'd', 4 ));
	manOffElement.setAttribute( "y", QString::number( g_ManOff[1], 'd', 4 ));
	manOffElement.setAttribute( "z", QString::number( g_ManOff[2], 'd', 4 ));
	newElem.appendChild( manOffElement );
	
	//< ���д���ļ�
	if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
	{
		qDebug() << "liuyc : save config failed!";
		return ;
	}
	QTextStream out(&file);
	doc.save( out, 4 );  //< 4Ϊ��ʽ�����������4���ո�

	out.flush();
	file.close();

	//< end
	qDebug() << "liuyc : save config finished!";
	return;
}

/************************************************************************/
/* �ڶ������ļ�֮ǰ��������б��������ı����Ϣ
/************************************************************************/
void OffsetWdt::clearIndex()
{
	for( int i = 0; i < MEA_MACRO_COUNT; ++i )
	{
		g_Index[i] = 0;
	}
	for( int i = 0; i <= _rightBtmE; ++i )
	{
		g_XYIndex[i] = 0;
	}
	g_ZIndex[0] = g_ZIndex[1] = 0;

	g_XYSrcDataCount = g_XYDesDataCount = 0;
	g_XYSrcDataBeginNum = g_XYSrcDataEndNum = 0;
	g_XYDesDataBeginNum = g_XYDesDataEndNum = 0;
	g_ZSrcDataBeginNum = g_ZSrcDataEndNum = 0;
	g_ZDesDataBeginNum = g_ZDesDataEndNum = 0;
	g_ZDataCount = 0;
}

/************************************************************************/
/* readConfig
/* funtion:  ��ȡ�����ļ�����ȡһ����������Ϣ��ͬʱ����Ϣ�������ؼ�����UI
/* input:    void
/* output:   void
/************************************************************************/
void OffsetWdt::readConfig()
{
	qDebug() << "liuyc: start reading config!";

	QString configFile;
#ifdef Q_OS_WIN
	configFile = "F:/";
#else
	configFile = "/home/Lynuc/Users/Config/";
#endif
	configFile += "CAD_GGBC_cfg.xml";

	//< �����֮ǰ������
	clearIndex();

	//< �жϸ��ֱ���
	QFile file( configFile );
	if( !file.exists() )
	{
		qDebug() << "liuyc: config file does not exist!";
		return;
	}
	if( m_dxfFilename.isEmpty() || m_dxfFilename.isNull() )
	{
		qDebug() << "liuyc: there is no dxf file for config!";
		return;
	}
	if( !file.open( QIODevice::ReadOnly ) )
	{
		qDebug() << "liuyc: openning config file failed!";
		return;
	}

	QDomDocument doc;
	if( !doc.setContent( &file ) )
	{
		qDebug() << "liuyc: read config - failed to set content!";
		file.close();
		return;
	}
	file.close();

	//< �ѻ�ȡ�����ļ���Ϣ�������Ƿ���ڵ�ǰ��DXF��Ӧ������
	QDomElement root = doc.documentElement();
	if (root.tagName() != "G-Speed" || root.isNull()) //< ������ǩ����ʽ������ֱ�ӷ���
	{
		return;
	}

	QString _xy_dirStr[8] = 
	{
		"left",
		"right",
		"top",
		"btm",
		"left_top",
		"right_top",
		"left_btm",
		"right_btm",
	};

	bool bHasConfig = false;
	QDomNodeList nodes_1st = root.childNodes();  //< ��һ���ڵ�(GGBC_SET)
	for( int i = 0; i < nodes_1st.count(); ++i )
	{
		QDomElement elem_1st = nodes_1st.at(i).toElement();
		if( elem_1st.tagName() == "GLOBAL_SET" )
		{
			g_DecRadius = elem_1st.attribute( "det_radius" ).toDouble();
			continue;
		}

		if( elem_1st.tagName() != "GGBC_SET" )
		{
			qDebug() << "liuyc: format of config file is wrong!";  //< �����ڵ�ֻ��GLOBAL_SET��GGBC_SET��ǰ���Ǹ��ж��Ѿ��ų���GLOBAL_SET
			return;
		}
		else
		{
			if( elem_1st.attribute( "dxf_file" ) != m_dxfFilename )  //< ������ǵ�ǰ��DXF�ļ���ֱ��������ֱ���ҵ���ǰ��DXF�ļ�����ȫ��ƥ��
				continue;
		}

		//< ����ҵ��˵�һ��ƥ�������
		QDomNodeList nodes_2st = nodes_1st.at(i).childNodes();
		for( int j = 0; j < nodes_2st.count(); ++j )  //< GGBC_SET����һ���ڵ�  INDEX_SRC | INDEX_DES | MAN_OFF
		{
			QDomElement elem_2st = nodes_2st.at(j).toElement();
			if( "INDEX_SRC" == elem_2st.tagName() )  //< INDEX_SRC
			{
				if( elem_2st.attribute( "dir" ) == "z" )
				{
					g_ZIndex[0] = elem_2st.attribute( "index_min" ).toUShort();
					g_ZIndex[1] = elem_2st.attribute( "index_max" ).toUShort();
					g_ZSrcDataBeginNum = g_ZIndex[0];
					g_ZSrcDataEndNum = g_ZIndex[1];
					g_ZDataCount = g_ZSrcDataEndNum - g_ZSrcDataBeginNum + 1;
					continue;
				}

				for( int ii = 0; ii < 8; ++ii )
				{
					if( _xy_dirStr[ii] != elem_2st.attribute( "dir" ) )
						continue;

					g_XYIndex[ii*2] = elem_2st.attribute( "index_min" ).toUShort();
					g_XYIndex[ii*2+1] = elem_2st.attribute( "index_max" ).toUShort();
					break;
				}

			}
			else if( "INDEX_DES" == elem_2st.tagName() )  //< INDEX_DES
			{
				g_XYDesDataBeginNum = elem_2st.attribute( "xy_begin" ).toUShort();
				g_ZDesDataBeginNum = elem_2st.attribute( "z_begin" ).toUShort();
			}
			else if( "MAN_OFF" == elem_2st.tagName())     //< MAN_OFF
			{
				g_ManOff[0] = elem_2st.attribute( "x" ).toDouble();
				g_ManOff[1] = elem_2st.attribute( "y" ).toDouble();
				g_ManOff[2] = elem_2st.attribute( "z" ).toDouble();
			}
			else
			{
				qDebug() << "liuyc: format of config file is wrong!";
				return;
			}
		}
		
		bHasConfig = true;
	}

	if( !bHasConfig )
		return;

	//< ��ֵӳ�������g_Index��
	for( int i = XY_MIN_LEFT; i <= XY_MAX_BTM; ++i )
	{
		g_Index[i] = g_XYIndex[i];
	}
	g_Index[Z_MIN] = g_ZIndex[0];
	g_Index[Z_MAX] = g_ZIndex[1];

	//< �ҵ�XY��index�е���С���ֵ
	int tmpMin = 501;
	int tmpMax = -1;
	for( int i = 0; i <= _rightBtmE; ++i )
	{
		if( g_XYIndex[i] > 0 )
		{
			tmpMin = tmpMin < g_XYIndex[i] ? tmpMin : g_XYIndex[i];
			tmpMax = tmpMax > g_XYIndex[i] ? tmpMax : g_XYIndex[i];
		}
	}
	g_XYSrcDataBeginNum = tmpMin;
	g_XYSrcDataEndNum = tmpMax;
	g_XYSrcDataCount = g_XYSrcDataEndNum - g_XYSrcDataBeginNum + 1;
	g_XYDesDataCount = g_XYSrcDataCount;

	//< �趨�Ƴ�������������ֵ
	m_RmMacroLEdit[RM_Axis_XY]->setText( QString::number(g_XYDesDataBeginNum) );
	m_RmMacroLEdit[RM_Axis_Z]->setText( QString::number(g_ZDesDataBeginNum) );
	m_RmPntCntLbl[RM_Axis_XY]->setText( tr("������") + QString::number(g_XYDesDataCount) );
	m_RmPntCntLbl[RM_Axis_Z]->setText( tr("������") + QString::number(g_ZDataCount) );

	for( int i = 0; i < 3; ++i )
	{
		m_ManOffLEdit[i]->setText( QString::number( g_ManOff[i], 'd', 3 ) );
	}

	qDebug() << "liuyc read config finished!";
	emit sglConfigRdy();
}

void OffsetWdt::openSrcNcFile( QString filename )
{
	//< ��ʾSRC�ļ���
	m_NewFNameLbl[FILE_SRC]->setText( "<font color=#0000FF style=\"font-weight:bold;\">"
		+ filename.section('/', -1, -1) + "</font>");

	getSrcPoints( filename );

	g_bIsNCRdy = true;
	//< �жϴ�NC�Ͷ����DXF�Ƿ�ƥ��
	if( isDxfToNC() != 1 )
	{
		m_NewBtn[FILE_NEW]->setEnabled( false );
		g_bIsNCRdy = false;
#ifndef Q_OS_WIN
		SetMacroVal( g_CIAddress, MACRO_NcNotMatch_FLAG, 1. );  //< NC���ж����ȶ�DXF�ٶ�NC
#endif
	}
	else
	{
		m_NewBtn[FILE_NEW]->setEnabled( true );
#ifndef Q_OS_WIN
		SetMacroVal( g_CIAddress, MACRO_NcNotMatch_FLAG, 0. );  //< NC���ж����ȶ�DXF�ٶ�NC
#endif
		emit sglSetMsg( tr("  NC��ȡ��ƥ��ɹ���") );
	}
}