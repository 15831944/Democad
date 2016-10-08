#ifndef STRUCTGRAPHM_H
#define STRUCTGRAPHM_H

#include <QApplication>
#include "QVector"

namespace camData {

struct Point3D			// 3ά����
{
	double x;
	double y;
	double z;
	void setValue(double a, double b, double c) {
		x = a; y = b; z = c;
	}
	void setValue(Point3D point3d)
	{
		x = point3d.x;
		y = point3d.y;
		z = point3d.z;
	}
	/*_POINT3D(RS_Vector vec) {//������Ϊ�������Ա�����ܺ��й��캯��
		setValue(vec.x, vec.y, vec.z);
	}*/
};

struct LINE			// �洢ֱ��
{
	Point3D xyStartPoint;
	Point3D xyEndPoint;
};

struct ARC				// Բ��
{
	Point3D xyStartPoint;
	Point3D xyEndPoint;
	Point3D xyCenterPoint;
};


struct SPLINE {
	unsigned short ranks;         //< �������ߵĽ���
	unsigned short sumOfDots;     //< �ڵ���
	unsigned short sumOfCtlDots;  //< ���Ƶ���
	unsigned short sumOfCrvDots;  //< ��ϵ���
	Point3D   normalVector;       //< ��������(��������ƽ���ڲ���Ч)
	Point3D   startVector;        //< ���������
	Point3D   endVector;          //< �յ�������

	Point3D *  pCtlPoints;        //< ���Ƶ�����
	double *   pPointValue;       //< �ڵ�ֵ
};


typedef struct _CAMData			// �洢����CAMͼ�ε�����
{
	int nGphNum;				// ͼ�α��,
	int nCADID;					// CAD�����е�ID�ţ�nCAMID��nCADID�����ж�ͼ���Ƿ��޸�
	int iType;					// �������ͣ�G01:1��ֱ�ߣ�G02:2��˳Բ����G03:3����Բ����
	int iLeadType;				// �������ͣ�0��ֱ�ߣ�1���˵㣻2��Բ��1��3��Բ��2��

	Point3D color;				// ��ʾ����ɫ

	_CAMData() {//Ĭ��ֵ
	nGphNum       = -1;
	nCADID        = -1;
	iType         = 1;
	iLeadType     = -1;
	color.x       = 255.0;
	color.y       = 255.0;
	color.z       = 255.0;
	}

	union
	{
		LINE Line; 
		ARC  Arc;
	}DrawType;

}CAMData;

typedef struct _CAMDataLink		// �洢����CAMͼ�ε�������Ϣ����
{
	CAMData	camData;
	_CAMDataLink *pNext;
	_CAMDataLink *pPre;
	_CAMDataLink() 
	{
		pNext   = NULL; 
		pPre    = NULL;
	}
}CAMDataLink;

inline void appendTo(CAMDataLink **pre, CAMDataLink **cur) {
	if (pre == NULL || cur == NULL || (*pre) == NULL || (*cur) == NULL) {
		return;
	} else {
		(*pre)->pNext = *cur;
		(*cur)->pPre = *pre;
		(*cur)->pNext = NULL;
	}
}


}
using namespace camData; //ʹ�������ռ䣬����ԭ���Ĵ���

//struct CADData
//{
//	int nCADID;
//	int nType;  //< ʵ�������
//	bool isLead;
//
//	enum CAD_entity_type
//	{
//		cad_entity_point,
//		cad_entity_arc,
//		cad_entity_line,
//		cad_antity_spline
//	};
//
//	union
//	{
//		Point3D point;
//		ARC     arc;
//		LINE    line;
//		SPLINE  spline;
//	}entity;
//
//	CADData()
//	{
//		nCADID = -1;
//		nType  = -1;
//		isLead = false;
//	}
//
//};

#endif // STRUCTGRAPHM_H
