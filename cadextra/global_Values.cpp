#include"global_Values.h"

QMainWindow* gMainWindow=NULL;

DemocadSignal* pDemocadSignal;

double g_dCadCalAccuracy  = 0.0001;	// ���㾫��
const int g_nGridSpacingMax = 1000; // added by wang.k.h
double g_nGridSpacingMin = 0.0001;
double g_dSpacing        = 1;		// ��ǰ������

/*
begin
�����ж��Ƿ��Ƿ��ͼ�ε���Ҫ����
*/
unsigned int* G_NUM =new unsigned int(1);
En_Line* pLine = new En_Line(RS_Line(RS_Vector(),RS_Vector()),NULL);
QList<En_Line*>* pEn_Map=NULL;
//end

/*
�򵥶���Ļ�����
*/
bool *G_MUTEX =new bool(false);

RS_Vector g_rsvMousePos;					// ��ǰ���λ��(��ת����ʵ������ϵ)

RS_Vector g_rsvDemoCadS;					// DemoCad�����λ��
RS_Vector g_rsvDemoCadWH;					// DemoCad�Ŀ�(g_rsvDemoCadWH.x)�͸�(g_rsvDemoCadWH.y)	


bool g_bTestFlag = false;
bool g_bIsResponseMouse = true;            ////Ĭ�Ͽ�����Ӧ����¼�



