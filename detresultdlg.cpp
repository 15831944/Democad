#include "detresultdlg.h"
#include <QPainter>

DetResultDlg::DetResultDlg(QWidget *parent)
	: QDialog(parent)
	, m_Factor( 0. )
	, m_bShowXY( true )
	, m_bShowZ( false )
{
	this->resize( 700, 450 );

	this->setWindowTitle( tr("Lynuc CAD - ̽���λ"));
	
	//< ѡ��XY��Z��ʾ̽����ѡ���
	for( int i = 0; i < SHOW_COUNT; ++i )
	{
		m_XYandZCheckBox[i] = new QCheckBox( this );
	}
	m_XYandZCheckBox[SHOW_XY]->setChecked( true );
	m_XYandZCheckBox[SHOW_Z]->setChecked( false );
	connect( m_XYandZCheckBox[SHOW_XY], SIGNAL( toggled(bool) ), this, SLOT( slotChooseToShowXY(bool)) );
	connect( m_XYandZCheckBox[SHOW_Z],  SIGNAL( toggled(bool) ), this, SLOT( slotChooseToShowZ(bool)) );

	//< ��ʾ��
	m_TipsEdit = new QTextEdit( this );
#ifdef __arm__
	m_TipsEdit->setText( qstrToHTML( tr("  \n1. ��ͨ����ѡ���湴ѡ��ѡ��ɼ���̽�����Ϣ��\n\n \
										   2. XY̽�������ϸȷ��L/R/T/Bλ���Ƿ���ȷ��������������������д�ڶ����еı����ź����¼��㣻\n "), 3, QColor(0,0,255), true) );
#else
	m_TipsEdit->setText( qstrToHTML( tr("  \n1. ��ͨ����ѡ���湴ѡ��ѡ��ɼ���̽�����Ϣ��\n\n \
										2. XY̽�������ϸȷ��L/R/T/Bλ���Ƿ���ȷ��������������������д�ڶ����еı����ź����¼��㣻\n "), 4, QColor(0,0,255), true) );
#endif
	m_TipsEdit->setStyleSheet( "border: 0px ;");
	m_TipsEdit->setReadOnly( true );

	//< 
	m_OKBtn = new RS_CustomBtn( this );
	m_OKBtn->setText( tr("ȷ ��") );
	connect( m_OKBtn, SIGNAL(clicked()), this, SLOT(slotOkBtnClicked()));
	m_OKBtn->setFont( FONT_10_SIMHEI_LIGHT );
	m_OKBtn->setFocusPolicy( Qt::NoFocus );
}

DetResultDlg::~DetResultDlg()
{

}

void DetResultDlg::showEvent(QShowEvent * event)
{
	QDialog::showEvent( event );
}

void DetResultDlg::resizeEvent(QResizeEvent * event)
{
	float width = this->width();
	float height = this->height();

	float fShowAreaWthPer = 0.618;
	float fSignAreaHgtPer = 0.22;
	int   nCheckBoxLeft = static_cast<int>( width * fShowAreaWthPer ) + 20/*ͼ���е�����߿��*/ + 10/*С����Ŀ��*/ + 100;
	int   nCheckBoxWth  = 20;
	int   nCheckBoxHgt  = nCheckBoxWth;
	for( int i = 0; i < SHOW_COUNT; ++i )
	{
		m_XYandZCheckBox[i]->setGeometry( nCheckBoxLeft
			                             , static_cast<int>( fSignAreaHgtPer * height * (0.167+0.333*i) - 10 ) /*ͼ��ƽ���ֳ���3��*/
										 , nCheckBoxWth
										 , nCheckBoxWth );
	}

	float fVGapPer = 0.02;
	float fTipHgtPer = 0.5;
	m_TipsEdit->setGeometry( static_cast<int>( fShowAreaWthPer * width )
		                     , static_cast<int>( (fSignAreaHgtPer + fVGapPer) * height )
							 , static_cast<int>( (1. - fShowAreaWthPer) * width + 1)
							 , static_cast<int>( fTipHgtPer * height ) );

	float fBtnLeftPer = 0.85;
	float fBtnTopPer  = 0.91;
	float fBtnWthPer = 0.13;
	float fBtnHgtPer = 0.07;
	m_OKBtn->setGeometry( static_cast<int>( width * fBtnLeftPer), static_cast<int>( height * fBtnTopPer )
		                 , static_cast<int>( width * fBtnWthPer), static_cast<int>( height * fBtnHgtPer));

	QDialog::resizeEvent( event );
}

void DetResultDlg::paintEvent(QPaintEvent * event)
{
	int width = this->width();
	int height = this->height();

	int rectWidth = static_cast<double>( width ) * 0.618;

    QPainter painter( this );
	//< step1 ���Ʊ���
	painter.fillRect( 0, 0, rectWidth, height, QBrush( Qt::black ) );

	//< ��ȡԭDXF��С���ҳ����ʵķ�������
	if( g_ContainerPtr != NULL && !g_ContainerPtr->isEmpty())
	{
		painter.setPen( QPen( Qt::white, 1, Qt::SolidLine, Qt::RoundCap ) );
		paintPntsAndEntity( painter, rectWidth, height );
	}
	else
	{
		painter.setPen( QPen( Qt::red, 1, Qt::SolidLine, Qt::RoundCap ) );
		painter.drawText( 10, 20, tr("û��ͼ�����ݣ����DXFͼ�����ټ���"));
	}

	//< step2 �������Ͻ�ͼ��
	double dSignLftPer = 0.618;
	double dSignWthPer = 1. - dSignLftPer;
	double dSignHgtPer = 0.22;
    painter.fillRect( width * dSignLftPer, 0., width * dSignWthPer + 1, height * dSignHgtPer, QBrush( Qt::white ));
	
	paintSigns( painter, width * dSignLftPer, 0, static_cast<int>( width * dSignWthPer ) + 1, static_cast<int>(height * dSignHgtPer) );
	

	QDialog::paintEvent( event );
}

void DetResultDlg::paintPntsAndEntity( QPainter & painter, int width, int height )
{
	//< step1 �������ű���
	double maxWidth = ( g_ContainerPtr->getMax() - g_ContainerPtr->getMin() ).x;
	double maxHeight = ( g_ContainerPtr->getMax() - g_ContainerPtr->getMin() ).y;

	m_GuiCenter = ( g_ContainerPtr->getMax() + g_ContainerPtr->getMin() ) * 0.5;

	double widthPer = width / maxWidth ;
	double heightPer = height / maxHeight;

	double finalPer = widthPer < heightPer ? widthPer : heightPer;
	m_Factor = finalPer * 0.80;  //< ����������յ����ű����ټ�С15%��Ϊ�˲���ͼ�λ����ھ��εı�Ե

	//< step2 ����ʵ��
	for( QList<RS_Entity *>::iterator it = g_ContainerPtr->getEntities().begin();
		it != g_ContainerPtr->getEntities().end();
		it++)
	{
		if( !(*it)->isVisible() ||  (*it)->isLead() )
			continue;

		switch( (*it)->rtti() )
		{
		case RS2::EntityArc:

			break;
		case RS2::EntityLine:
			painter.drawLine( toGuiXY( 0, 0, width, height, (*it)->getStartpoint()),
				              toGuiXY( 0, 0, width, height, (*it)->getEndpoint()) );
			break;
		case RS2::EntitySpline:
			{
				RS_Spline * tmp = reinterpret_cast< RS_Spline * >( *it );
			}
			break;
		default:
			popMessageBox( "Lynuc CAD Warning", tr("DXFͼ����ʵ��ĸ�ʽ��֧�֣���ʹ����ȷ��DXF�ļ���"), 5, QColor(255,0,0), true );
			return;
			break;
		}
	}

	//< step3 ����ԭ��
	painter.setPen( QPen( Qt::red, 1, Qt::SolidLine, Qt::RoundCap ) );
	painter.drawLine( toGuiXY( 0, 0, width, height, RS_Vector( 10, 0)),
		toGuiXY( 0, 0, width, height, RS_Vector(-10 , 0)) );
	painter.drawLine( toGuiXY( 0, 0, width, height, RS_Vector( 0, 10)),
		toGuiXY( 0, 0, width, height, RS_Vector(0 , -10)) );

	//< step4 ����̽���
	if( g_SavedData.isEmpty() )
		return;
	if( m_bShowXY )
	{
		QString dirStr = NULL;
		for( int i = g_XYSrcDataBeginNum; i <= g_XYSrcDataEndNum; ++i )
		{
			painter.fillRect( QRectF( toGuiXY( 0, 0, width, height, g_SavedData.value(i)).x() - 3.
				, toGuiXY( 0, 0, width, height, g_SavedData.value(i)).y() - 3.
				, 6.
				, 6.), QBrush(Qt::yellow) );
			painter.setPen( QPen( Qt::yellow, 1, Qt::SolidLine, Qt::RoundCap ) );
			//< �ж���ŵķ���
			if( i >= g_Index[XY_MIN_LEFT] && i <= g_Index[XY_MAX_LEFT] )
				dirStr = ".L";
			else if( i >= g_Index[XY_MIN_RIGHT] && i <= g_Index[XY_MAX_RIGHT] )
				dirStr = ".R";
			else if( i >= g_Index[XY_MIN_TOP] && i <= g_Index[XY_MAX_TOP] )
				dirStr = ".T";
			else if( i >= g_Index[XY_MIN_BTM] && i <= g_Index[XY_MAX_BTM] )
				dirStr = ".B";
			else if( i >= g_XYIndex[_leftTopB] && i <= g_XYIndex[_leftTopE] )
			    dirStr = ".";
			else if( i >= g_XYIndex[_rightTopB] && i <= g_XYIndex[_rightTopE] )
				dirStr = ".";
			else if( i >= g_XYIndex[_leftBtmB] && i <= g_XYIndex[_leftBtmE] )
				dirStr = ".";
			else if( i >= g_XYIndex[_rightBtmB] && i <= g_XYIndex[_rightBtmE] )
				dirStr = ".";
			else {
				qDebug() << "liuyc: paint index is wrong! _ detresultwdt";
				dirStr = "";
			}
			painter.drawText( toGuiXY( 0, 0, width, height, g_SavedData.value(i)).x() - 3. - 25.
				             , toGuiXY( 0, 0, width, height, g_SavedData.value(i)).y() - 3. + 18.
							 , QString::number( i ) + dirStr);
		}
	}
	if( m_bShowZ && g_ZDataCount > 0)
	{
		for( int i = g_ZSrcDataBeginNum; i <= g_ZSrcDataEndNum; ++i )
		{
			painter.fillRect( QRectF( toGuiXY( 0, 0, width, height, g_SavedData.value(i)).x() - 3.
				, toGuiXY( 0, 0, width, height, g_SavedData.value(i)).y() - 3.
				, 6.
				, 6.), QBrush(Qt::blue) );
			painter.setPen( QPen( Qt::blue, 1, Qt::SolidLine, Qt::RoundCap ) );
			painter.drawText( toGuiXY( 0, 0, width, height, g_SavedData.value(i)).x() - 3. + 7.
				, toGuiXY( 0, 0, width, height, g_SavedData.value(i)).y() - 3.
				, QString::number( i ) + ".Z");
		}
	}
	

}

/************************************************************************/
/* �������Ͻǵ�ͼ��
/************************************************************************/
void DetResultDlg::paintSigns( QPainter & painter, int nx, int ny, int wth, int hgt)
{
	//< �������int��ȫ������������ͼ��������ֻ���ͼ���λ�ã���resizeEvent�������ֵӦ�ñ���һ��
	int nLeft = 20;
	int nWth  = 10;
	painter.fillRect( nLeft + nx, hgt * 0.167 - nWth * 0.5 + ny, nWth, nWth, QBrush(Qt::yellow));
	painter.fillRect( nLeft + nx, hgt * 0.5 - nWth * 0.5 + ny, nWth, nWth, QBrush(Qt::blue));

	int nTextGap = 10;
	painter.setPen( QPen( Qt::black, 1, Qt::SolidLine, Qt::RoundCap ) );
	painter.drawText( nLeft + nx + nTextGap + nWth, hgt * 0.167 + 5 + ny, tr("X/Y��̽���"));
	painter.drawText( nLeft + nx + nTextGap + nWth, hgt * 0.5 + 5 + ny, tr("Z  ��̽���"));

	int nLeftWth = 20;
	painter.drawText( nx + nLeftWth + wth * 0.25 * 0., hgt * 0.8333 + 5 + ny, tr("L����"));
	painter.drawText( nx + nLeftWth + wth * 0.25 * 1., hgt * 0.8333 + 5 + ny, tr("R����"));
	painter.drawText( nx + nLeftWth + wth * 0.25 * 2., hgt * 0.8333 + 5 + ny, tr("T����"));
	painter.drawText( nx + nLeftWth + wth * 0.25 * 3., hgt * 0.8333 + 5 + ny, tr("B����"));
}

/************************************************************************/
/* ��ʵ���XY������ת������ʾ������㣨����ϵ��ת����
/************************************************************************/
QPointF DetResultDlg::toGuiXY( int nx, int ny, int wth, int hgt, RS_Vector vec )
{
	return QPointF( ( nx + wth ) * 0.5 + (vec.x - m_GuiCenter.x) * m_Factor, ( ny + hgt ) * 0.5 - (vec.y - m_GuiCenter.y) * m_Factor );
}

void DetResultDlg::slotChooseToShowXY( bool flag )
{
	m_bShowXY = flag;
	update();
}

void DetResultDlg::slotChooseToShowZ( bool flag )
{
	m_bShowZ = flag;
	update();
}

void DetResultDlg::slotOkBtnClicked()
{
	qDebug() << "liuyc ok btn clicked!";
	this->close();
}
