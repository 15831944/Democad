/*
democadSignal��ľ���ʵ�֡�
Editor: zhang.y.q
*/

#include"democadSignal.h"


DemocadSignal::DemocadSignal(QObject *parent):QObject(parent)
{
}

DemocadSignal::~DemocadSignal()
{
}

void DemocadSignal::sendSignal_EditFinish(QString str){
	emit EditFinish(str);
}

void DemocadSignal::sendSignal_EditReset(){
	emit EditReset();
}
