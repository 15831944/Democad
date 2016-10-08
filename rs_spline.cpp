/************************************************************************/
/* created by liu.y.c 
/* data: 2016.06.15
/************************************************************************/

#include "rs_spline.h"
#include "rs_painter.h"
#include "rs_painterqt.h"
#include "rs_graphicview.h"
#include "rs_information.h"
#include "rs_linetypepattern.h"

/************************************************************************/
/* RS_Spline constructor
/************************************************************************/
RS_Spline::RS_Spline(RS_EntityContainer* parent, const RS_SplineData & d)
	: RS_AtomicEntity(parent)
	, data( d )
	, m_partsCount( 32 )
	, m_PointsOnBrs( NULL )
	, m_bHasInit( false )
{

	m_PointsOnBrs = new RS_Vector[m_partsCount + 1];  //< RS_Vector��ʼֵ���� 0, 0, 0

	calculatePointsOnBrs();
	m_bHasInit = true;
	calculateEndpoints();
	calculateBorders();

	if( data.nCADID >= 0 ) 
	{
		initId(data.nCADID);
	}

	//< �����ô�ӡ��Ϣ
	/*for( int i = 0; i < data.getSumOfDots(); ++i )
	{
		std::cout << "liuyc data pp = " << data.pPointValue[i] << std::endl;
	}
	for( int i = 0; i < data.getSumOfCtlDots(); ++i )
	{
		std::cout << "liuyc ctl point " << data.pCtlPoints[i] << std::endl;
	}*/

}

/************************************************************************/
/* ����ĳһ�㵽ʵ���ϵ�����Ķ˵㣬���������
/************************************************************************/
RS_Vector RS_Spline::getNearestEndpoint(const RS_Vector& coord, double* dist) const
{
	//< step1 ��ȡ�����������յ㣬����Ӧ����������㵽Ŀ���ľ���
	RS_Vector start = data.getStartPoint();
	RS_Vector end =   data.getEndPoint();
	double dist1 = (start-coord).squared();
	double dist2 = (end-coord).squared();

	//< step2 �ȽϾ���
	if ( dist2 < dist1 ) 
	{
		if ( dist != NULL )
		{
			*dist = sqrt(dist2);
		}
		return end;
	} 
	else
	{
		if ( dist != NULL ) 
		{
			*dist = sqrt(dist1);
		}
		return start;
	}
}

RS_Vector RS_Spline::getFarthestEndpoint(const RS_Vector& coord, double* dist) const
{
	double dist1, dist2;

	dist1 = (startPoint-coord).squared();
	dist2 = (endPoint-coord).squared();

	if (dist2 > dist1) 
	{
		if( dist != NULL ) 
		{
			*dist = sqrt(dist2);
		}
		return endPoint;
	} 
	else 
	{
		if( dist != NULL ) 
		{
			*dist = sqrt(dist1);
		}
		return startPoint;
	}
}

RS_VectorSolutions RS_Spline::getRefPoints() 
{
	RS_VectorSolutions ret(startPoint, endPoint);
	return ret;
}

void RS_Spline::draw(RS_Painter* painter, RS_GraphicView* view, double& patternOffset)
{
	if( painter == NULL || view == NULL )
	{
		return;
	}

	////< ��������Ӵ������軭
	//RS_Pen old_pen = painter->getPen();
	//QVector<RS_Vector> endPoints(0);
	//RS_Vector vpMin( view->toGraph( 0, view->getHeight() ) );
	//RS_Vector vpMax( view->toGraph( view->getWidth(), 0) );
	////���Ӵ���
	//QPolygonF visualBox( QRectF( vpMin.x, vpMin.y, vpMax.x-vpMin.x, vpMax.y-vpMin.y ));

	//if( getStartpoint().isInWindowOrdered(vpMin, vpMax) ) 
	//{
	//	endPoints << getStartpoint();
	//}
	//if( getEndpoint().isInWindowOrdered(vpMin, vpMax) ) 
	//{
	//	endPoints << getEndpoint();
	//}

	RS_Vector * tmpPoints = new RS_Vector[m_partsCount + 1];
	for( int i = 0; i <= m_partsCount; ++i )
	{
		tmpPoints[i] = view->toGui( m_PointsOnBrs[i] );
	}

	RS_LineTypePattern * pat;
	if ( isSelected() )
	{
		pat = &patternSelected;
	} 
	else 
	{
		//pat = &patternSolidLine;
		pat = view->getPattern( getPen().getLineType() );
	}

	//< ���δ��ѡ�У���ֱ�ӻ�ͼ
	for( int i = 0 ; i <= m_partsCount; ++i )
	{
		if( i != 0 )
			painter->drawLine( tmpPoints[i-1], tmpPoints[i] ); //< �ӵ�2���㿪ʼ��ֱ������ǰһ����͵�ǰ��
	}

	delete[] tmpPoints;

	//< ����ֱ���˳�
	return;
}

RS_Vector RS_Spline::getRelatedPoint( double u )
{
	return RS_Vector( false );
}

/************************************************************************/
/* ��ȡ��ǰ�״ε�ϵ��ֵ
/************************************************************************/
double RS_Spline::getRatio( double u, unsigned short p, unsigned short i )
{
	//< ���ų�������� u = 0 or 1 ʱ
	if( fabs( u ) < 10E-10 )
	{
		if( i == 0 )
		{
			return 1.;
		}
		else
		{
			return 0.;
		}
	}
	else if( fabs(u - 1.0) < 10E-10 )
	{
		//qDebug() << "liuyc last point!";
		if( i == data.getSumOfCtlDots() - 1 )
		{
			return 1.;
		}
		else
		{
			return 0.;
		}
	}
	else  //< u��0~1֮��ʱ
	{
		if( p == 0 )  //< 0�׻�����
		{
			if( u >= data.pPointValue[i] && u < data.pPointValue[i+1] )
			{
				return 1.;
			}
			else
			{
				return 0.;
			}
		}
		else  //< �߽׻����� p >= 1
		{
			double tmp = 0.;
			//< ���ҳ���ǰ��u���ĸ��ڵ������
			unsigned short k = 0;
			for( k = 0; k < data.getSumOfDots(); ++k )
			{
				if( u >= data.pPointValue[k] && u < data.pPointValue[k+1] )
				{
					break;
				}
			}
			//qDebug() << "liuyc k =" << k;

			//< ������ʽ
			double coefficient = 0.;
			if( i >= k - p && i <= k )
			{
				if( i == k-p )
				{
					coefficient = (data.pPointValue[k+1]-u)/(data.pPointValue[k+1]-data.pPointValue[k-p+1])
						          * getRatio(u, p-1, k-p+1);
				}
				else if( i == k)
				{
					coefficient = (u-data.pPointValue[k])/(data.pPointValue[k+p]-data.pPointValue[k])
								  * getRatio(u, p-1, k);
				}
				else
				{
					coefficient =  (u-data.pPointValue[i])/(data.pPointValue[i+p]-data.pPointValue[i])*getRatio(u,p-1,i) 
						+ (data.pPointValue[i+p+1]-u)/(data.pPointValue[i+p+1]-data.pPointValue[i+1])*getRatio(u,p-1,i+1);
				}
				
			}
            
			return coefficient;
		}
	}

	return 0.;
}

/************************************************************************/
/* ����������������ĺ�����ȡ�˵���Ϣ
/************************************************************************/
void RS_Spline::calculateEndpoints()
{
	startPoint = data.getStartPoint();
	endPoint   = data.getEndPoint();
}

bool RS_Spline::offset(const RS_Vector& position, const double& distance)
{
	return true;
}

/************************************************************************/
/* ƽ�ƹ���
/************************************************************************/
void RS_Spline::move( const RS_Vector & offset )
{
	//< ���Ƶ�ƽ��
	data.moveCtlPoints( offset );
	startPoint.move( offset );
	endPoint.move( offset );
	for( int i = 0; i <= m_partsCount; ++i )
	{
		m_PointsOnBrs[i].move( offset );
	}
	moveBorders( offset );
}

/************************************************************************/
/* ��ת����
/************************************************************************/
void RS_Spline::rotate(const RS_Vector& center, const double& angle)
{
	data.rotateCtlPoints( center, angle );
	calculatePointsOnBrs();
	calculateEndpoints();
	calculateBorders();
}


void RS_Spline::rotate(const RS_Vector& center, const RS_Vector& angleVector)
{
	data.rotateCtlPoints( center, angleVector );
	calculatePointsOnBrs();
	calculateEndpoints();
	calculateBorders();
}

/************************************************************************/
/* ��ȡ��ӽ���Բ�ģ�������ܲ���������������
/************************************************************************/
RS_Vector RS_Spline::getNearestCenter(const RS_Vector& coord, double* dist) const
{
	return RS_Vector( false );
}

//< ���������ϵĵ�
void RS_Spline::calculatePointsOnBrs()
{
	double uUint = 1. / static_cast<double>( m_partsCount );
	double u     = 0.;  
	unsigned short ranks = data.getRanks();

	for( int i = 0; i <= m_partsCount; ++i )
	{
		m_PointsOnBrs[i].set( 0., 0., 0. );  //< �����֮ǰ�ĵ�λ����
		for( int j = 0; j < data.getSumOfCtlDots(); ++j )
		{
			m_PointsOnBrs[i].x += ( data.pCtlPoints[j].x * getRatio(u, ranks, j) );
			m_PointsOnBrs[i].y += ( data.pCtlPoints[j].y * getRatio(u, ranks, j) );
			m_PointsOnBrs[i].z += ( data.pCtlPoints[j].z * getRatio(u, ranks, j) );
		}
		u += uUint;
	}

	if( m_bHasInit )  //< �Ѿ���ʼ������˵�������Ѿ����ֳ���С�߶Σ��Ժ�û�б�Ҫ�����ж�
		return;

	//< �ж������ϵ�С�߶γ����Ƿ����
	while( true )
	{
		//qDebug() << "liuyc spline parts = " << m_partsCount;
		for( int i = 0; i < m_partsCount; ++i )
		{
			if( m_PointsOnBrs[i].distanceTo(m_PointsOnBrs[i+1]) < 0.5 )  //< ֻҪ��С�߶�С��һ���ض�ֵ�����϶������������̫��
				return;
		}

		delete [] m_PointsOnBrs;  //< ��ɾ��ԭ���飬�ڿ����µ�����ؼ�
		m_partsCount *= 2;
		uUint = 1. / static_cast<double>( m_partsCount );
		u     = 0.;
		m_PointsOnBrs = new RS_Vector[m_partsCount+1];
		for( int i = 0; i <= m_partsCount; ++i )
		{
			m_PointsOnBrs[i].set( 0., 0., 0. );  //< �����֮ǰ�ĵ�λ����
			for( int j = 0; j < data.getSumOfCtlDots(); ++j )
			{
				m_PointsOnBrs[i].x += ( data.pCtlPoints[j].x * getRatio(u, ranks, j) );
				m_PointsOnBrs[i].y += ( data.pCtlPoints[j].y * getRatio(u, ranks, j) );
				m_PointsOnBrs[i].z += ( data.pCtlPoints[j].z * getRatio(u, ranks, j) );
			}
			u += uUint;
		}
	}
	
}

//< ����߽�
void RS_Spline::calculateBorders()
{
	double minX = RS_MAXDOUBLE;
	double minY = minX;
	double minZ = minX;
	double maxX = RS_MINDOUBLE;
	double maxY = maxX;
	double maxZ = maxX;

	for( int i = 0 ; i <= m_partsCount; ++i )
	{
		if( m_PointsOnBrs[i].x < minX )
		{
			minX = m_PointsOnBrs[i].x;
		}
		if( m_PointsOnBrs[i].x > maxX )
		{
			maxX = m_PointsOnBrs[i].x;
		}
		if( m_PointsOnBrs[i].y < minY )
		{
			minY = m_PointsOnBrs[i].y;
		}
		if( m_PointsOnBrs[i].y > maxY )
		{
			maxY = m_PointsOnBrs[i].y;
		}
		if( m_PointsOnBrs[i].z < minZ )
		{
			minZ = m_PointsOnBrs[i].z;
		}
		if( m_PointsOnBrs[i].z > maxZ )
		{
			maxZ = m_PointsOnBrs[i].z;
		}
	}

	minV = RS_Vector( minX, minY, minZ );
	maxV = RS_Vector( maxX, maxY, maxZ );

	return;
}

//< ����ʵ���������������ĵ�
RS_Vector RS_Spline::getNearestPointOnEntity(const RS_Vector& coord,
	bool onEntity, double* dist , RS_Entity** entity) const
{
	RS_Vector point ;
	double distance = RS_MAXDOUBLE;

	//for( int i = 0; i <= m_partsCount; ++i )
	//{
	//	//< ѭ���ҵ���������ĵ�
	//    if( coord.distanceTo( m_PointsOnBrs[i] ) < distance )
	//	{
	//		distance = coord.distanceTo( m_PointsOnBrs[i] );
	//		point = m_PointsOnBrs[i];
	//	}
	//}
	//< �������ѭ���ҵ�����ĵ������⣬Ӧ�����ҵ�������߶Σ��������߶�������ĵ� //< 2016.09.21 liu.y.c
	RS_Vector nearestPoint;
	double *  tmpDist = new double(0.);
	for( int i = 0; i < m_partsCount; ++i )
	{
		nearestPoint = RS_Line( m_PointsOnBrs[i], m_PointsOnBrs[i+1] ).getNearestPointOnEntity( coord, true, tmpDist );
		if( *tmpDist < distance )
		{
			distance = *tmpDist;
			point = nearestPoint;
		}
	}

	//qDebug() << "nearset point__ distance__" << distance;
	//std::cout << "nearest point : " << point << std::endl;
	*dist = distance;
	return point;
}

RS_Vector RS_Spline::getFarthestPointOnEntity(const RS_Vector& coord,
	bool onEntity, double* dist, RS_Entity** entity) const
{
	RS_Vector point ;
	double distance = RS_MINDOUBLE;

	for( int i = 0; i <= m_partsCount; ++i )
	{
		//< ѭ���ҵ���������ĵ�
		if( coord.distanceTo( m_PointsOnBrs[i] ) > distance )
		{
			distance = coord.distanceTo( m_PointsOnBrs[i] );
			point = m_PointsOnBrs[i];
		}
	}

	*dist = distance;
	return point;
}

//< �����㵽ʵ��ľ���
double RS_Spline::getDistanceToPoint(const RS_Vector& coord,
	RS_Entity** entity,
	RS2::ResolveLevel level,
	double solidDist) const
{
	double dis = 0.;
	//< step1  find the nearest point on entity
    RS_Vector point = getNearestPointOnEntity( coord, false, &dis );
    
	return dis;
}

void RS_Spline::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2)
{
	data.mirror( axisPoint1, axisPoint2 );
	calculatePointsOnBrs();
	calculateEndpoints();
	calculateBorders();
}

//< ����
void RS_Spline::scale(const RS_Vector& center, const RS_Vector& factor)
{
	data.scale( center, factor );
	calculatePointsOnBrs();
	calculateEndpoints();
	calculateBorders();
}

void RS_Spline::moveRef(const RS_Vector& ref, const RS_Vector& offset)
{
	if(  fabs(startPoint.x -ref.x) < 1.0e-4 && fabs(startPoint.y -ref.y) < 1.0e-4 ) 
		startPoint += offset;
	if(  fabs(endPoint.x -ref.x) < 1.0e-4 && fabs(endPoint.y -ref.y) < 1.0e-4 )
		endPoint += offset;
}

//< begin   to be continued...

void RS_Spline::stretch(const RS_Vector& firstCorner,
	const RS_Vector& secondCorner,
	const RS_Vector& offset)
{

	RS_Vector vLow( std::min(firstCorner.x, secondCorner.x), std::min(firstCorner.y, secondCorner.y));
	RS_Vector vHigh( std::max(firstCorner.x, secondCorner.x), std::max(firstCorner.y, secondCorner.y));

	//for( int i = 0; i <= m_partsCount; ++i )
	//{
		//m_PointsOnBrs[i] += offset;
		//if (getStartpoint().isInWindowOrdered(vLow, vHigh)) {
		//	moveStartpoint(getStartpoint() + offset);
		//}
		//if (getEndpoint().isInWindowOrdered(vLow, vHigh)) {
		//	moveEndpoint(getEndpoint() + offset);
		//}
	//}
	
}

RS_Vector RS_Spline::getNearestDist(double distance,
	const RS_Vector& coord,
	double* dist)
{
	return RS_Vector( false );
}

RS_Vector RS_Spline::getNearestOrthTan(const RS_Vector& coord,
	const RS_Line& normal,
	bool onEntity)
{
	return RS_Vector( false );
}
//< end     to be continued...

//< EOF