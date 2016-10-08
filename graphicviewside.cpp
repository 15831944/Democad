#include "graphicviewside.h"
#include <iostream>
#include <QPainter>
#include <QDebug>

//< �������������ֵ
#define MAX(a,b,c) (a>b?(a>c?a:c):(b>c?b:c))

GraphicViewSide::GraphicViewSide(QWidget *parent)
	: QDockWidget(parent)
	, m_factor( 0. )
{
	//< ����һ���տؼ�������еı���ؼ����ﵽ���ر�������Ч��
	m_EmptyTtlBar = new QWidget( this );
	this->setTitleBarWidget( m_EmptyTtlBar );

	//< ��ʼ��ʵ������
	container = NULL;

	this->setMinimumHeight( 200 );
}

GraphicViewSide::~GraphicViewSide()
{

}

/************************************************************************/
/* �����е��������룬�����޸�
/************************************************************************/
void GraphicViewSide::setContainer( RS_EntityContainer * container )
{
	this->container = container;
}

/************************************************************************/
/* ���ñ߽�(X,Y,Z)
/************************************************************************/
void GraphicViewSide::setBorder()
{
	if( container != NULL )
	{
		m_MaxV = container->getMax();
		m_MinV = container->getMin();
	}
}

/************************************************************************/
/* ���Ƴ�������TEXT��ʵ�������ͼ�Ͳ���ͼ
/************************************************************************/
void GraphicViewSide::paintEvent(QPaintEvent *event)
{
    int height = this->height();
	int width  = this->width();

	QPainter painter( this );

	//< step1 �Ȼ��Ʊ���
	painter.fillRect( rect(), QBrush( Qt::black ) );

	//< step2 ����������ͼ�򣬰�����ͼ�����
	const int iLeft = 4;
	const int iTop  = 4;
	const int iFrameWth = width * 0.5 - iLeft * 1.5;
	const int iFrameHgt = height - iTop * 2;
	painter.setPen( QPen( Qt::white, 1, Qt::SolidLine, Qt::RoundCap ) );
	painter.drawRect( iLeft, iTop, iFrameWth, iFrameHgt);
	painter.drawRect( width * 0.5 + iLeft * 0.5, iTop, iFrameWth, iFrameHgt);

	const int iTtlHgt = 20;
	const int iTtlWth = static_cast<int>( width * 0.4 );
	painter.setPen( QPen( Qt::yellow, 1, Qt::SolidLine, Qt::RoundCap ) );
	painter.setFont( QFont("SIMHEI", 10, QFont::Light) );
	painter.drawText( QRect( iLeft + 8, iTop + 4, iTtlWth, iTtlHgt)
		            , QString("Main View (x,z)"));
	painter.drawText( QRect( width * 0.5 + iLeft * 0.5 + 8, iTop + 4, iTtlWth, iTtlHgt)
		            , QString("Side View (y,z)"));

	//< step3 ��ʾ�������ʵ��
	if( container == NULL )
		return;

	//painter.save();
	painter.setPen( QPen( Qt::white, 1, Qt::SolidLine ));

	//< step3.1 �������ű���
	setBorder();
	int iViewLeft = 10;
	int iViewWth = iFrameWth - iViewLeft * 2;
	int iViewHgt = iFrameHgt - 40;
	double xTmpFac = ( m_MaxV.x - m_MinV.x ) / static_cast<double>( iViewWth );
	double yTmpFac = ( m_MaxV.y - m_MinV.y ) / static_cast<double>( iViewWth );
	double zTmpFac = ( m_MaxV.z - m_MinV.z ) / static_cast<double>( iViewHgt );
    m_factor = MAX( xTmpFac, yTmpFac, zTmpFac );

	//< step3.2 ʵ�����굽��Ļ����ı任����ʾ
	int iViewLft1 = iLeft + iViewLeft;
	int iViewLft2 = width * 0.5 + iLeft * 0.5 + iViewLeft;
	for( QList<RS_Entity *>::iterator it = container->getEntities().begin();
		it != container->getEntities().end();
		it++)
	{
		//< ʵ��Ϊ���ɼ�ʵ�������������ֱ������
		if( !(*it)->isVisible() || (*it)->isLead() )
			continue;

		//< ���ݲ�ͬ��ʵ�壬���Ʋ�ͬ��ͶӰ
		switch( (*it)->rtti() )
		{
		case RS2::EntityArc:
			painter.drawLine( toGuiXZ( iViewLft1, iTop + 30, iViewWth, iViewHgt, (*it)->getMin())
							, toGuiXZ( iViewLft1, iTop + 30, iViewWth, iViewHgt, (*it)->getMax()) );
			painter.drawLine( toGuiYZ( iViewLft2, iTop + 30, iViewWth, iViewHgt, (*it)->getMin() )
							, toGuiYZ( iViewLft2, iTop + 30, iViewWth, iViewHgt, (*it)->getMax() ));
			break;
		case RS2::EntityLine:
			//std::cout << (*it)->getStartpoint() <<  "  "  << (*it)->getEndpoint() << std::endl;
			painter.drawLine( toGuiXZ( iViewLft1, iTop + 30, iViewWth, iViewHgt, (*it)->getStartpoint() )
				              , toGuiXZ( iViewLft1, iTop + 30, iViewWth, iViewHgt, (*it)->getEndpoint() ));
			painter.drawLine( toGuiYZ( iViewLft2, iTop + 30, iViewWth, iViewHgt,(*it)->getStartpoint() )
				              , toGuiYZ( iViewLft2, iTop + 30, iViewWth, iViewHgt, (*it)->getEndpoint() ));
			break;
		case RS2::EntitySpline:
			{
				RS_Spline * tmp = reinterpret_cast< RS_Spline * >( *it );
				for( int i = 1; i <= tmp->getPartCount() ; ++i )
				{
					painter.drawLine( toGuiXZ( iViewLft1, iTop + 30, iViewWth, iViewHgt, tmp->getPointsOnBrs()[i-1] )
						, toGuiXZ( iViewLft1, iTop + 30, iViewWth, iViewHgt, tmp->getPointsOnBrs()[i] ));
					painter.drawLine( toGuiYZ( iViewLft2, iTop + 30, iViewWth, iViewHgt, tmp->getPointsOnBrs()[i-1] )
						, toGuiYZ( iViewLft2, iTop + 30, iViewWth, iViewHgt, tmp->getPointsOnBrs()[i] ));
				}
			}
			break;
		default:
			break;
		}

	}

#if 0
	//< step4 ��������ϵ
	painter.setPen( QPen( Qt::red, 1, Qt::DashDotDotLine ));
	double centerx = m_MaxV.x >= 0. ? ( m_MinV.x <= 0.? 0. : m_MinV.x ) : m_MaxV.x;
	double centery = m_MaxV.y >= 0. ? ( m_MinV.y <= 0.? 0. : m_MinV.y ) : m_MaxV.y;
	double centerz = m_MaxV.z >= 0. ? ( m_MinV.z <= 0.? 0. : m_MinV.z ) : m_MaxV.z;
	double maxZ = m_MaxV.z;
	double minZ = m_MinV.z;
	if( fabs( maxZ - minZ ) <= 10e-6 )
	{
		maxZ += ( m_MaxV.x - m_MinV.x ) * 0.2;
		minZ -= ( m_MaxV.x - m_MinV.x ) * 0.2;
	}

	//< ������Ӧ�������������
	if( m_MinV.x <= 0. && m_MaxV.x >= 0.)
	{
		painter.drawLine( toGuiXZ( iViewLft1, iTop + 30, iViewWth, iViewHgt, RS_Vector( m_MinV.x, 0., centerz) )
			, toGuiXZ( iViewLft1, iTop + 30, iViewWth, iViewHgt, RS_Vector( m_MaxV.x, 0., centerz) ));
		painter.drawLine( toGuiXZ( iViewLft1, iTop + 30, iViewWth, iViewHgt, RS_Vector( centerx, 0., maxZ) )
			, toGuiXZ( iViewLft1, iTop + 30, iViewWth, iViewHgt, RS_Vector( centerx, 0., minZ) ));
	}
	if( m_MinV.y <= 0. && m_MaxV.y >= 0.)
	{
		painter.drawLine( toGuiYZ( iViewLft2, iTop + 30, iViewWth, iViewHgt, RS_Vector( 0., m_MinV.y, centerz) )
			, toGuiYZ( iViewLft2, iTop + 30, iViewWth, iViewHgt, RS_Vector( 0., m_MaxV.y, centerz) ));
		painter.drawLine( toGuiYZ( iViewLft2, iTop + 30, iViewWth, iViewHgt, RS_Vector( 0., centery, maxZ) )
			, toGuiYZ( iViewLft2, iTop + 30, iViewWth, iViewHgt, RS_Vector( 0., centery, minZ) ));
	}

#endif

}

QPointF GraphicViewSide::toGuiXZ( int nx, int ny, int wth, int hgt, RS_Vector vec )
{
	if( fabs( m_factor ) < 10e-10 )
	{
		qDebug() << "liuyc : factor is to small !";
		return QPointF();
	}

	double xMid = ( m_MaxV.x + m_MinV.x ) * 0.5;
	double zMid = ( m_MinV.z + m_MaxV.z ) * 0.5;
	double xReal = nx + wth * 0.5 + ( vec.x - xMid ) / m_factor;
	double zReal = ny + hgt * 0.5 - ( vec.z - zMid ) / m_factor;

	return QPointF( xReal, zReal );
}

QPointF GraphicViewSide::toGuiYZ( int nx, int ny, int wth, int hgt, RS_Vector vec )
{
	if( fabs( m_factor ) < 10e-10 )
	{
		qDebug() << "liuyc : factor is to small !";
		return QPointF();
	}

	double yMid = ( m_MaxV.y + m_MinV.y ) * 0.5;
	double zMid = ( m_MinV.z + m_MaxV.z ) * 0.5;
	double yReal = nx + wth * 0.5 + ( vec.y - yMid ) / m_factor;
	double zReal = ny + hgt * 0.5 - ( vec.z - zMid ) / m_factor;

	return QPointF( yReal, zReal );
}

void GraphicViewSide::doGetNewContainer( RS_EntityContainer * cont )
{
	//qDebug() << "liuyc update side view !";
	//< �����µ�ʵ������
	setContainer( cont );

	//< ���»�ͼ
	update();
}
