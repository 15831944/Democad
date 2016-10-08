/************************************************************************
* File Name:
*	QPublicSignal.h
* Description��
*	����һ���ź��࣬�����ǣ�����ϵͳ�Ĺ����ź�
* Programmer:
*	zhou.g.x
* Make Date:
*	2014-3-26
************************************************************************/

#ifndef QPUBLICSIGNAL_H
#define QPUBLICSIGNAL_H

#include <QObject>
#include "QList"
#include "rs_actioninterface.h"

class QPublicSignal: public QObject
{
	Q_OBJECT

public:
	explicit QPublicSignal(QObject *parent = 0);
	~QPublicSignal();

	void StartTrancingThreadSignal(int nMs = 1000);							// ����Trancing�����߳̿�ʼִ�е��ź�
	void EndTrancingThreadSignal();											// ���͹ر�Trancing�����߳��ź�
	void getGID(int);														// ����ͼ��ID��
	void SetDemoCadFullScreen(bool bFull);									// ����ͼ�α༭ȫ����ʾ
	void SendDemoCadFullScreenExit();
	void SendActionLinkClearUP(QList<RS_ActionInterface*> rs_ai);			// DemoCad��rs_eventhandler��CleanUp()������Action��������󣬷����ź�
	void SendClearChoise();													// DemoCad��qg_actionhandler.cpp����QG_ActionHandler::reset()�У������ѡ�е�ͼ��
	void SetPromFullScreen(bool bFull);

	// �ļ�����
	void SendOpenOpt();
	void SendNewOpt();
	void SendSaveOpt();
	void SendSaveAsOpt();

	// DemoCad���и�λ���� 
	void DemoCadReset();
	void SendGetNewContainer( RS_EntityContainer * );

signals:
	void SglDemoCadFullScreenExit();
	void StartTrancingThread(int nMs);
	void EndTrancingThread();
	void getGIDSgl(int);													// ����ͼ��ID��
	void SglDemoCadFullScreen(bool);
	void SglPromFullScreen(bool);
	void SglActionLinkClearUP(QList<RS_ActionInterface*> rs_ai);			// DemoCad��rs_eventhandler��CleanUp()������Action��������󣬷����ź�
	void SglSendClearChoise();												// DemoCad��qg_actionhandler.cpp����QG_ActionHandler::reset()�У������ѡ�е�ͼ��

	// �ļ�����
	void SglOpenOpt();
	void SglNewOpt();
	void SglSaveOpt();
	void SglSaveAsOpt();

	void SglDemoCadReset();
	void SglGetNewContainer( RS_EntityContainer * );
};

#endif