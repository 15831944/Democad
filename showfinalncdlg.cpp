#include "showfinalncdlg.h"
#include <QDebug>
#include <QPainter>

#define INIT_WIDTH  700
#define INIT_HEIGHT 450


ShowFinalNcDlg::ShowFinalNcDlg(QWidget *parent)
	: QDialog(parent)
	, m_lPointCount( 0 )
	, m_vecNCPoints( NULL )
	, m_isPressed( false )
	, m_InitFactor( 0. )
{
	this->resize( INIT_WIDTH, INIT_HEIGHT );

	this->setWindowTitle( tr("Lynuc CAD - ��NCԤ��"));

	//< ������ʾ���ʼ��
	m_TipsEdit = new QTextEdit( this );
	m_TipsEdit->setStyleSheet(" border: 0px; background-color: transparent;");
	QString tipStr = tr( "��ǰ�����Ԥ�����ɵ���NC��������ʾ��\n \
					 1. ���������ֶ������ţ�\n \
					 2. ��������ס����϶�ͼ�Σ�\n \
					 3. ���˫��������ʾԭʼ������λ�ã�\n \
					 4. �ٴε����������ʾ��������ʾ��Ϣ��\n \
					 5. ��ɫ������ʾ��λ˳����ɫΪNC��㡣" );
#ifdef __arm__
	m_TipsEdit->setText( qstrToHTML( tipStr, 3, QColor(200,200,200), true) );
#else
	m_TipsEdit->setText( qstrToHTML( tipStr, 4, QColor(200,200,200), true) );
#endif
	m_TipsEdit->setFocusPolicy( Qt::NoFocus );
	m_TipsEdit->setReadOnly( true );
	m_TipsEdit->setVisible( false );
	m_TipsEdit->setAttribute( Qt::WA_TransparentForMouseEvents, true );  //< ��괩͸���ø�������Ӧ����¼�

	//< ��ť��ʼ��
	m_CloseBtn = new RS_CustomBtn( this );
	m_CloseBtn->setText( tr("�ر�Ԥ��") );
	m_CloseBtn->setFocusPolicy( Qt::NoFocus );

	m_TipsBtn = new RS_CustomBtn( this );
	m_TipsBtn->setText( tr("������ʾ") );
	m_TipsBtn->setFocusPolicy( Qt::NoFocus );
	m_TipsBtn->setCheckable( true );

	//< ˫�������еĻ�ͼ��ʼ��
	m_Pixmap = new QPixmap( size() );

	connect( m_TipsBtn, SIGNAL( toggled(bool) ), this, SLOT( slotTipBtnClicked(bool)));
	connect( m_CloseBtn, SIGNAL( clicked()), this, SLOT(close()));
}

ShowFinalNcDlg::~ShowFinalNcDlg()
{
	if( m_Pixmap != NULL )
		delete m_Pixmap;
}

void ShowFinalNcDlg::paintEvent(QPaintEvent * event)
{
	m_Pixmap->fill( this, 0, 0 );
	int width = this->width();
	int height = this->height();

	int rectWidth = width;

	//< step1 �����ͼ
	QPainter painter( m_Pixmap );
	painter.fillRect( rect(), QBrush( Qt::black ) );
	if( m_lPointCount > 0 && m_vecNCPoints != NULL )
	{
		paintAllPoints( painter, rectWidth, height );
	}

	//< step2 �ڴ濽����˫�����ͼ
	QPainter finalPainter( this );
	finalPainter.setRenderHint( QPainter::SmoothPixmapTransform, true );
	finalPainter.drawPixmap( 0, 0, *m_Pixmap );
}

/************************************************************************/
/* �����¼�
/************************************************************************/
void ShowFinalNcDlg::wheelEvent(QWheelEvent * event)
{
	//< step1 �ж��Ƿ�Ϊ��С����
	int   numOfSteps = event->delta() * 0.125 / 15; //< ����8�ٳ���15����1������ת1��
	double perstep = 0.;
	if( numOfSteps > 0 )
		perstep = 0.18;
	else
	{
		if( m_Factor < m_InitFactor )
			perstep = 0.08;
		else
			perstep = 0.14;
	}
	float numDegrees  = perstep * numOfSteps; 
	double tmpFactor = m_Factor;
	m_Factor *= ( 1. + numDegrees );
	if( m_Factor < m_InitFactor * 0.5 && numDegrees < 0.)  //< �����ʾ������С��������С��������¼�
	{
		m_Factor = tmpFactor;
		event->ignore();
		return;
	}

	//< step2 ���ű�����������λ�õļ���
	QPoint curPos = event->pos();
	RS_Vector tmpVec = RS_Vector( curPos.x(), curPos.y() ) - m_RealCenter;
	tmpVec *= numDegrees;
	m_RealCenter -= tmpVec;
	repaint();
}

void ShowFinalNcDlg::resizeEvent(QResizeEvent * evnnt)
{
	float width  = this->width();
	float height = this->height();

	float fLeftPer = 0.65;
	float fTopPer  = 0.02;
	float fEditWthPer = 0.34;
	float fEditHgtPer = 0.88;
	m_TipsEdit->setGeometry( static_cast<int>( width * fLeftPer ), static_cast<int>( height * fTopPer )
		, static_cast<int>( width * fEditWthPer ), static_cast<int>( height * fEditHgtPer ));

	float fVGapPer   = 0.01;
	float fBtnWthPer = 0.165;
	float fBtnHgtPer = 0.07;
	float fBtnGapPer = 0.01;
	m_TipsBtn->setGeometry( static_cast<int>( width * fLeftPer ), static_cast<int>( height * (fTopPer+fEditHgtPer+fVGapPer))
		, static_cast<int>( width * fBtnWthPer ), static_cast<int>( height * fBtnHgtPer ));
	m_CloseBtn->setGeometry( static_cast<int>( width * (fLeftPer+fBtnWthPer+fBtnGapPer) )
		, static_cast<int>( height * (fTopPer+fEditHgtPer+fVGapPer))
		, static_cast<int>( width * fBtnWthPer )
		, static_cast<int>( height * fBtnHgtPer ));
}

void ShowFinalNcDlg::mousePressEvent(QMouseEvent * event )
{
	if( event->button() == Qt::LeftButton )
	{
		m_MoveLastPoint = event->pos();
		m_isPressed = true;
	}
	else if( event->button() == Qt::RightButton )
	{
		event->ignore();
	}

	QDialog::mousePressEvent( event );
	
}

void ShowFinalNcDlg::mouseReleaseEvent( QMouseEvent * event )
{
	if( event->button() == Qt::LeftButton )
	{
		m_isPressed = false;
	}
	else if( event->button() == Qt::RightButton )
	{
        event->ignore();
	}

	QDialog::mouseReleaseEvent( event );
	
}

void ShowFinalNcDlg::mouseMoveEvent(QMouseEvent * event )
{
	if( !m_isPressed )
		event->ignore();
	else
	{
		RS_Vector moveVec( event->pos().x() - m_MoveLastPoint.x(), event->pos().y() - m_MoveLastPoint.y() );
		m_RealCenter += moveVec;
		repaint();
		m_MoveLastPoint = event->pos();
	}
}

/************************************************************************/
/* ˫���¼����ָ�������ı�����λ��
/************************************************************************/
void ShowFinalNcDlg::mouseDoubleClickEvent(QMouseEvent * event )
{
	m_Factor = m_InitFactor;
	m_RealCenter.set( this->width() * 0.5, this->height() * 0.5 );
	update();
}

/************************************************************************/
/* ��������������NC��
/************************************************************************/
void ShowFinalNcDlg::paintAllPoints( QPainter & painter, int wth, int hgt )
{
	//< step1 ����NC�����
	int red = 0;
	int blue = 255;
	for( int i = 0; i < m_lPointCount - 1; ++i )
	{
		red = static_cast<int>( (double)i / m_lPointCount * 200. );
		blue = static_cast<int>( (double)i / m_lPointCount * 200. );
		painter.setPen( QPen( QColor(red, 200, blue), 1, Qt::SolidLine, Qt::RoundCap ) );
		painter.drawLine( toGuiXY( 0, 0, wth, hgt, m_vecNCPoints[i] )
			, toGuiXY( 0, 0, wth, hgt, m_vecNCPoints[i+1] ) );
		painter.fillRect( toGuiXY( 0, 0, wth, hgt, m_vecNCPoints[i]).x(), toGuiXY( 0, 0, wth, hgt, m_vecNCPoints[i]).y(), 3, 3
			, QBrush( QColor(red, 200, blue) ));
	}

	//< step2 ����ԭ��
	painter.setPen( QPen( Qt::red, 1, Qt::SolidLine, Qt::RoundCap ) );
	QPointF tmpCenter = toGuiXY( 0, 0, wth, hgt, RS_Vector( 0, 0) );
	painter.drawLine( tmpCenter - QPointF(5., 0), tmpCenter + QPointF(5., 0) );
	painter.drawLine( tmpCenter - QPointF(0, 5.), tmpCenter + QPointF(0., 5.));
}

void ShowFinalNcDlg::set( RS_Vector * vecPtr, long lcount )
{
	m_vecNCPoints = vecPtr;
	if( lcount < 0 )
	{
		qDebug() << "liuyc : count less than 0! _ showfinalncdialog";
		lcount = 0;
	}
	m_lPointCount = lcount;

    //< ����NC������Χ
	m_MinV.set( RS_MAXDOUBLE, RS_MAXDOUBLE );
	m_MaxV.set( RS_MINDOUBLE, RS_MINDOUBLE );
	for( int i = 0; i < m_lPointCount - 1; ++i )  //< �ҵ����
	{
		if( m_vecNCPoints[i].x > m_MaxV.x )
			m_MaxV.x = m_vecNCPoints[i].x;
		if( m_vecNCPoints[i].x < m_MinV.x )
			m_MinV.x = m_vecNCPoints[i].x;

		if( m_vecNCPoints[i].y > m_MaxV.y )
			m_MaxV.y = m_vecNCPoints[i].y;
		if( m_vecNCPoints[i].y < m_MinV.y )
			m_MinV.y = m_vecNCPoints[i].y;
	}

	double maxWidth = ( m_MaxV - m_MinV ).x;
	double maxHeight = ( m_MaxV - m_MinV ).y;

	double widthPer  = INIT_WIDTH / maxWidth ;
	double heightPer = INIT_HEIGHT / maxHeight;

	double finalPer = widthPer < heightPer ? widthPer : heightPer;
	m_Factor = finalPer * 0.80;  //< ����������յ����ű����ټ�С20%��Ϊ�˲���ͼ�λ����ھ��εı�Ե
	m_InitFactor = m_Factor;

	m_GuiCenter = ( m_MaxV + m_MinV ) * 0.5;
	m_RealCenter.set( INIT_WIDTH * 0.5, INIT_HEIGHT * 0.5 );
}

QPointF ShowFinalNcDlg::toGuiXY( int nx, int ny, int wth, int hgt, RS_Vector vec )
{
	return QPointF( nx + m_RealCenter.x + (vec.x - m_GuiCenter.x) * m_Factor,  ny + m_RealCenter.y - (vec.y - m_GuiCenter.y) * m_Factor );
}

RS_Vector ShowFinalNcDlg::toRealXY(int nx, int ny, int wth, int hgt, QPointF point)
{
	return RS_Vector( (point.x() - nx - m_RealCenter.x) / m_Factor + m_GuiCenter.x
		            , -(point.y() - ny - m_RealCenter.x) / m_Factor + m_GuiCenter.y );
}

void ShowFinalNcDlg::slotTipBtnClicked( bool flag )
{
	m_TipsEdit->setVisible( flag );
}
