#include "functionGraphM.h"
#include <QMessageBox>
#include "Parameter.h"

/*
* Funciton Name: calAngle() 
*	����Բ������ʼ��Ƕ�
* INPUT��
*	Point3D  Point��Բ����ʼ��
*	Point3D cPoint��Բ��
* OUTPUT��
*   double�����ػ���
*/
double calAngle(Point3D Point,Point3D cPoint)
{
	double r     = sqrt((Point.x-cPoint.x)*(Point.x-cPoint.x) + (Point.y-cPoint.y)*(Point.y-cPoint.y));
	double x0    = cPoint.x + r;
	double y0    = cPoint.y;
	double angle = acos((2*r*r - ((Point.x-x0)*(Point.x-x0) + (Point.y-y0)*(Point.y-y0)))/(2*r*r));

	if (Point.x>=cPoint.x && Point.y>=cPoint.y)				// 1����
	{
		return angle;
	}
	else if (Point.x<=cPoint.x && Point.y>=cPoint.y)		// 2����	
	{
		return angle;
	}
	else if (Point.x<=cPoint.x && Point.y<=cPoint.y)		// 3����
	{
		return (2*3.14159 - angle);
	}
	else if (Point.x>=cPoint.x && Point.y<=cPoint.y)		// 4����
	{
		return (2*3.14159 - angle);
	}

	return 0.0;
}


/*
* Description��
*	�ж�p2λ��p0p1��Χ��
* Parameters��
*	Point3D p0: 
*	Point3D p1:
*	Point3D p2: 
* Retrun Values:
*	bool: 
*		true: �ڷ�Χ��
*		false:���ڷ�Χ��
*/
bool ChkPotInline(Point3D p0, Point3D p1, Point3D p2)
{
	double dMinx = (p0.x<p1.x) ? p0.x:p1.x;
	double dMaxx = (p0.x>p1.x) ? p0.x:p1.x;
	double dMiny = (p0.y<p1.y) ? p0.y:p1.y;
	double dMaxy = (p0.y>p1.y) ? p0.y:p1.y;

	if ((p2.x<=dMaxx && p2.x>=dMinx) && (p2.y<=dMaxy && p2.y>=dMiny))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
* Funciton Name: judgeLinePoint()
*	�ж�ĳ���Ƿ����߶����˵���һ��
* INPUT��
*	Point3D startP:ֱ�ߵ���� 
*	Point3D endP:  ֱ�ߵ��յ�
*	Point3D judgeP:���жϵĵ�
* OUTPUT��
*   bool: �����жϽ����true:���߶����˵���һ�㣻false:�����߶����˵���һ��
*
*/
bool judgeLinePoint(Point3D startP, Point3D endP, Point3D judgeP)		// �ж��Ƿ����߶����˵���һ��
{
	double dRtn = CheckDirect(startP, endP, judgeP);

	// ���㲻����
	if (!judgeTwoPointEqual(dRtn, 0.0, g_dCalAccuracy))
	{
		return false;
	}

	// ���㹲�ߺ��жϸõ��Ƿ����߶����˵���һ��
	bool bRtn = ChkPotInline(startP, endP, judgeP);

	return bRtn;
}

/*
* Funciton Name: judgeArcPoint()
*	�ж��Ƿ���Բ����Բ�ϵ�һ��
* INPUT��
*	Point3D startP:Բ������� 
*	Point3D endP:  Բ�����յ�
*	Point3D center:Բ������
*	Point3D judgeP:���жϵĵ�
*	int iType:     Բ������
* OUTPUT��
*   bool: �����жϽ����true:��Բ����Բ�ϵ�һ�㣻false:����Բ����Բ�ϵ�һ��
*
*/
bool judgeArcPoint(Point3D startP, Point3D endP, Point3D centerP, Point3D judgeP, int iType)			// �ж��Ƿ���Բ����Բ�ϵ�һ��
{
	double dR    = sqrt((startP.x - centerP.x)*(startP.x - centerP.x) + (startP.y - centerP.y)*(startP.y - centerP.y));
	double dJud  = sqrt((judgeP.x - centerP.x)*(judgeP.x - centerP.x) + (judgeP.y - centerP.y)*(judgeP.y - centerP.y));

	bool bInC    = judgeTwoPointEqual(dJud, dR, g_dCalAccuracy);

	// ���жϵĵ㲻��Բ��
	if (!bInC)
	{
		return false;
	}

	double dSA = calAngle(startP, centerP);
	double dEA = calAngle(endP,   centerP);
	double dJA = calAngle(judgeP, centerP);

	// ��ʼ�ǵ�����ֹ�ǣ���Բ
	if (judgeTwoPointEqual(dSA, dEA, g_dCalAccuracy))
	{
		return true;
	}
	// ��ʼԲ�Ľ�С����ֹ��
	else if (dSA<dEA)
	{
		if (3 == iType)
		{
			if (dJA>=dSA && dJA<=dEA)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		else if (2 == iType)
		{
			if (dJA<=dSA || dJA>=dEA)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
	}
	// ��ʼ�Ǵ�����ֹ��
	else if (dSA>dEA)
	{
		if (3 == iType)
		{
			if (dJA<=dEA || dJA>=dSA)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		else if (2 == iType)
		{
			if (dJA>=dEA && dJA<=dSA)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
	}

	return true;
}

/*
* Description��
*	�ж�p0p2λ��p0p1��˳ʱ�뷽�򣬻�����ʱ�뷽��
* Parameters��
*	Point3D p0: 
*	Point3D p1:
*	Point3D p2: 
* Retrun Values:
*	double: 
*		����0: ��ʱ�뷽��
*		С��0: ˳ʱ�뷽��
*		����0: ����
*/
double CheckDirect(Point3D p0, Point3D p1, Point3D p2)
{
	return ((p1.x-p0.x)*(p2.y-p0.y) - (p2.x-p0.x)*(p1.y-p0.y));
}

/*
* Funciton Name: judgeTwoPointEqual()
*	�ж��������Ƿ����
* INPUT��
*	Point3D firP:��һ���� 
*	Point3D secP:�ڶ�����
*	double dToleranceError: �������
* OUTPUT��
*   bool
*/
bool judgeTwoPointEqual(Point3D firP, Point3D secP, double dToleranceError)		
{
	if (dToleranceError<0)
	{
		dToleranceError = -dToleranceError;
	}

	if (firP.x - secP.x > -dToleranceError && firP.x - secP.x < dToleranceError &&
		firP.y - secP.y > -dToleranceError && firP.y - secP.y < dToleranceError &&
		firP.z - secP.z > -dToleranceError && firP.z - secP.z < dToleranceError)
	{
		return true;
	}

	return false;
}

/*
* Description��
*	�ж������������Ƿ����
* Parameters��
*	double dFir:
*	double dSec:
*	double dToleranceError: �������
* Retrun Values:
*	double: ����ͼ��·���ĳ���
*/
bool judgeTwoPointEqual(double dFir, double dSec, double dToleranceError)
{
	if (dToleranceError<0)
	{
		dToleranceError = -dToleranceError;
	}

	if (dFir - dSec > -dToleranceError && dFir - dSec < dToleranceError)
	{
		return true;
	}

	return false;
}
