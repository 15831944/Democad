#include "hidelabeln3.h"
#include <QMouseEvent>
#include <QDebug>

/************************************************************************/
/* ���캯��
/************************************************************************/
hideLabelN3::hideLabelN3(QWidget *parent)
	: QLabel(parent)
	, m_bIsVis( true )
	, m_bIsPressed( false )
{
	setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );  //< �����ޱ߿���ǰ��ʾ
	setAttribute( Qt::WA_TranslucentBackground, true );                    //< ����͸��
	setAttribute( Qt::WA_ShowWithoutActivating, true );                    //< ʹ������������������¼�
	resize( 50, 50 );
}

/************************************************************************/
/* ��������
/************************************************************************/
hideLabelN3::~hideLabelN3()
{
	//< Qt�Զ�����
}

void hideLabelN3::paintEvent(QPaintEvent * event)
{
	QPainter painter( this );
	painter.fillRect( rect(), QBrush( QColor(30,30,30,70)));

	int nWidth = width();
	int nHeight = height();

	//< �������Ǽ�ͷ
	QPointF tranglePoints[3];
    if( m_bIsVis )
	{
		tranglePoints[0].setX( nWidth * 0.1 );
		tranglePoints[0].setY( nHeight * 0.5 );
		tranglePoints[1].setX( nWidth * 0.9 );
		tranglePoints[1].setY( nHeight * 0.5 );
		tranglePoints[2].setX( nWidth * 0.5 );
		tranglePoints[2].setY( nHeight * 0.9 );
	}
	else
	{
		tranglePoints[0].setX( nWidth * 0.1 );
		tranglePoints[0].setY( nHeight * 0.9 );
		tranglePoints[1].setX( nWidth * 0.9 );
		tranglePoints[1].setY( nHeight * 0.9 );
		tranglePoints[2].setX( nWidth * 0.5 );
		tranglePoints[2].setY( nHeight * 0.5 );
	}
	
	painter.setPen( QPen( QColor( Qt::green ), 1, Qt::SolidLine, Qt::RoundCap ) );
	painter.setBrush( QColor( Qt::green ) );
	painter.drawPolygon( tranglePoints, 3, Qt::WindingFill );

	//< ����CAD����
    painter.setFont( QFont("SIMHEI", 20, QFont::Bold ) );
	painter.setPen( QPen( QColor( Qt::red ), 1, Qt::SolidLine, Qt::RoundCap ) );
	painter.drawText( static_cast<int>( nWidth * 0.1 ), static_cast<int>( nHeight * 0.1 )
		, static_cast<int>( nWidth * 0.8 ), static_cast<int>( height() * 0.4 ) , Qt::AlignCenter
		, "CAD" );

	QLabel::paintEvent( event );
}

void hideLabelN3::mousePressEvent( QMouseEvent * event )
{
	//< ��¼֮ǰ����Ļλ��
	if( event->button() == Qt::LeftButton )
	{
		m_FommerCursorPos = event->globalPos();
		m_CurPos = pos();
		m_LastPos = m_CurPos;
		m_bIsPressed = true;
	}
	else
		event->ignore();
	
	QLabel::mousePressEvent( event );
}

void hideLabelN3::mouseReleaseEvent( QMouseEvent * event )
{
	if( m_CurPos == m_LastPos && event->button() == Qt::LeftButton )  //< �����ǰλ����֮ǰ��¼��λ����ͬ����ζ��û���ƶ���ֻ���л���ʾ
	{
		m_bIsVis = !m_bIsVis;
		update();
		emit sglSetVisible( m_bIsVis );
	}

	m_bIsPressed = false;
	QLabel::mouseReleaseEvent( event );
}

void hideLabelN3::mouseMoveEvent(QMouseEvent *event)
{
	if( m_bIsPressed )  //< �ƶ�LAEBL
	{
		m_CurPos += ( event->globalPos() - m_FommerCursorPos );
		move( m_CurPos );
	}
	else
		event->ignore();

	m_FommerCursorPos = event->globalPos();
	QLabel::mouseMoveEvent( event );
}