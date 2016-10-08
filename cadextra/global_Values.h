#ifndef GLOBAL_VALUES
#define GLOBAL_VALUES

/*
���ڴ���ȫ�ֱ������ļ���
Editor: zhang.y.q
*/

#include"democadSignal.h"
#include"democadSignal.h"
#include"rs_line.h"
#include"en_Line.h"
#include <QMainWindow>
#include "rs_vector.h"

extern double g_dCadCalAccuracy;		// ����
extern double g_dSpacing;				// ��ǰ������
extern const int g_nGridSpacingMax;		// �趨դ�������ֵ1000
extern double g_nGridSpacingMin;		// �趨դ������Сֵ0.001

extern QMainWindow *gMainWindow;
extern DemocadSignal* pDemocadSignal;//Democad ģ���ȫ���źŷ��ͺͽ���

extern En_Line* pLine;           //���ڵ�������enʵ��
extern QList<En_Line*>* pEn_Map; //���ڱ��洦����enʵ�� 
extern unsigned int* G_NUM;      //���ڼ�¼���ͼ�εı��

/*�򵥶���Ļ�����*/
extern bool* G_MUTEX;

extern RS_Vector g_rsvMousePos;				// ��ǰ���λ��(��ת����ʵ������ϵ)

extern RS_Vector g_rsvDemoCadS;				// DemoCad�����λ��
extern RS_Vector g_rsvDemoCadWH;			// DemoCad�Ŀ�(g_rsvDemoCadWH.x)�͸�(g_rsvDemoCadWH.y)	

//extern const int nDemoCadErrNum;

#define G_NDEMO_CAD_ERR_NUM 15000

extern bool g_bTestFlag;
extern bool g_bIsResponseMouse;//����Ƿ���Ӧ��־

#endif