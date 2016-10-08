#include "QPublicSignal.h"


QPublicSignal::QPublicSignal(QObject *parent):QObject(parent)
{

}


QPublicSignal::~QPublicSignal()
{

}

/*
* Description��
*	����Trancing�����߳̿�ʼִ�е��ź�
* Parameters:
*	int nMs : �̵߳�msleepʱ��
* Retrun Values:
*	void 
*/
void QPublicSignal::StartTrancingThreadSignal(int nMs)		// ����Trancing�����߳̿�ʼִ�е��ź�
{
	emit StartTrancingThread(nMs);
}

/*
* Description��
*	���͹ر�Trancing�����߳��ź�
* Parameters��
*	void
* Retrun Values:
*	void 
*/
void QPublicSignal::EndTrancingThreadSignal()			// ���͹ر�Trancing�����߳��ź�
{
	emit EndTrancingThread();
}

/*
* Description��
*	���и���в���ͼ��ID��,
* Parameters��
*	void
* Retrun Values:
*	void 
*/
void QPublicSignal::getGID(int nGid)							// ����ͼ��ID��
{
	emit getGIDSgl(nGid);
}

/*
* Description��
*	����ͼ�α༭ȫ����ʾ
* Parameters��
*	bool bFull: true,ȫ����ʾ
* Retrun Values:
*	void 
*/
void QPublicSignal::SetDemoCadFullScreen(bool bFull)
{
	emit SglDemoCadFullScreen(bFull);
}

/*
* Description��
*	ͼ�α༭ȫ����ʾ�˳�
* Parameters��
*	bool bFull: true,ȫ����ʾ
* Retrun Values:
*	void 
*/
void QPublicSignal::SendDemoCadFullScreenExit()
{
	emit SglDemoCadFullScreenExit();
}

/*
* Description��
*	DemoCad��rs_eventhandler��CleanUp()������Action��������󣬷����ź�
* Parameters��
*	bool bFull: true,ȫ����ʾ
* Retrun Values:
*	void 
*/
void QPublicSignal::SendActionLinkClearUP(QList<RS_ActionInterface*> rs_ai)
{
	emit SglActionLinkClearUP(rs_ai);
}

/*
* Description��
*	DemoCad��qg_actionhandler.cpp����QG_ActionHandler::reset()�У������ѡ�е�ͼ��
* Parameters��
*	bool bFull: true,ȫ����ʾ
* Retrun Values:
*	void 
*/
void QPublicSignal::SendClearChoise()
{
	emit SglSendClearChoise();
}

/*
* Description��
*	���ó���ӹ�ģ��ȫ����ʾ
* Parameters��
*	bool bFull: true,ȫ����ʾ
* Retrun Values:
*	void 
*/
void QPublicSignal::SetPromFullScreen(bool bFull)
{
	emit SglPromFullScreen(bFull);
}

// �ļ�����
void QPublicSignal::SendOpenOpt()
{
	emit SglOpenOpt();
}

void QPublicSignal::SendNewOpt()
{
	emit SglNewOpt();
}

void QPublicSignal::SendSaveOpt()
{
	emit SglSaveOpt();
}

void QPublicSignal::SendSaveAsOpt()
{
	emit SglSaveAsOpt();
}

/*
* Description��
*	DemoCad���и�λ���� 
* Parameters��
*	void
* Retrun Values:
*	void 
*/
void QPublicSignal::DemoCadReset()
{
	emit SglDemoCadReset();
}

void QPublicSignal::SendGetNewContainer( RS_EntityContainer * container)
{
	emit SglGetNewContainer( container );
}