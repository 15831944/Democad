#ifndef PUBLICSTRUCT_H
#define PUBLICSTRUCT_H
/************************************************************************
* File Name:
*	PublicStruct.h
* Description:
*	���幫���ṹ��
* Programmer:
*	zhou.g.x
* Make Date:
*	2014-3-28
************************************************************************/

/*
* Description��
*	��λģ��ִ�еĸ��ֲ���
*/
enum ENUM_LOCATION_STATE
{ 		
	LOCATION_INIT,			// ��ʼֵ��Ҳ����MDI�ӹ�״̬
	LOCATION_RETURNTZ,		// ��λ-����
	LOCATION_COORDSET,		// ��λ-��������
	LOCATION_MOVEAXIS_DEF,	// ��λ-����-�Զ�������
	LOCATION_MOVEAXIS_HAF,	// ��λ-����-����ƶ�
	LOCATION_MOVEAXIS_LMT,	// ��λ-����-�����ƶ�
	LOCATION_MOVEAXIS_ORI,	// ��λ-����-��ԭ��
	LOCATION_MOVEAXIS_REF,	// ��λ-����-�زο���
	LOCATION_HANDLE,		// ��λ-����
	LOCATION_VERTICAL,		// ��λ-У��ֱ
	LOCATION_THREEPOINT,	// ��λ-3��ѰԲ��
	LOCATION_LEVELING,		// ��λ-��б����
	LOCATION_ANGLELOCA,		// ��λ-�Ƕ�λ
	LOCATION_WIDTHCENTER,	// ��λ-������
	LOCATION_HOLECENTER,	// ��λ-������
	LOCATION_BATHCENTER,	// ��λ-������
	LOCATION_SURFACE,		// ��λ-����
};

/*
* Description��
*	����ӹ�ģ���Ӳ���״̬
*/
enum ENUM_PROM_STATE
{ 		
	PROM_INIT,				// ��ʼֵ��Ҳ���ǳ���ӹ�״̬
	PROM_SINGLESTEP,		// ����
	PROM_REVERSEMACHING,	// ����
	PROM_REVERSSINGLESTEP,	// ���򵥶�
	PROM_EMPTYMACHING,		// ����
	PROM_ROLLBACK,			// ����
	PROM_ROWMACHING,		// ָ���мӹ�
	PROM_GRAPHMACHING,		// ָ��ͼ�μӹ�
	PROM_RECOVERY			// ��˿�ָ�
};

/*
* Description��
*	ϵͳ��7��ģ��
*/
enum ENUM_SYS_MODULE
{
	GRAPHM_MODULE,				// ͼ�μӹ�
	LOCATION_MODULE,			// ��λ
	MDI_MODULE,					// MDI 
	EDIT_MODULE,				// �༭
	PROM_MODULE,				// ����ӹ�
	PARSET_MODULE,				// �����趨
	HELP_MODULE					// ����
};

/*
* Description��
*	��ʾ��Ϣ�ĵȼ�
*/
enum ERRORORDER
{
	G_MSG_ORDER_DANGEROUS,		// ����
	G_MSG_ORDER_WARNING,		// ����
	G_MSG_ORDER_INFORMATION		// ������Ϣ
};

/*
* Description��
*	��ɫ
*/
enum COLORINFO
{
	cl_red,
	cl_orange,
	cl_yellow,
	cl_green,
	cl_cyan_blue,
	cl_blue,
	cl_purple,
	cl_white,
};

#endif