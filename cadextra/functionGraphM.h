#ifndef FUNCTIONGRAPHM_H
#define FUNCTIONGRAPHM_H

#include "structGraphM.h"
#include "entityDefine.h"
#include <QFileDialog>
#include <QFile>
#include <QtCore/qmath.h>

bool judgeLinePoint(Point3D startP, Point3D endP, Point3D judgeP);									// �ж��Ƿ����߶����˵���һ��
bool judgeArcPoint(Point3D, Point3D, Point3D, Point3D, int);										// �ж��Ƿ���Բ����Բ�ϵ�һ��
double CheckDirect(Point3D p0, Point3D p1, Point3D p2);
bool   judgeTwoPointEqual(Point3D firP, Point3D secP, double dToleranceError);
bool   judgeTwoPointEqual(double dFir, double dSec, double dToleranceError);
bool   ChkPotInline(Point3D p0, Point3D p1, Point3D p2);
double calAngle(Point3D Point,Point3D cPoint);

#endif // FUNCTIONGRAPHM_H