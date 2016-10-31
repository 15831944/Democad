/*-----------------------------------
ȫ�ֱ���ͷ�ļ�
������ Liu.y.c
ʱ�䣺 2015.09.01
-----------------------------------*/

#ifndef PARAMETER_H
#define PARAMETER_H

#include <QTime>
#include <QPalette>
#include <QString>
#include <QLabel>
#include <QFont>
#include <QMessageBox>
#include <QMap>
#include <QDebug>
#include "rs_entitycontainer.h"
#include "RS_CustomBtn.h"
#include "QPublicSignal.h"
#include "./cadextra/PublicStruct.h"
#include "ErrorMessage.h"
#include "structGraphM.h"
#include "qmath.h"
#include "rs_vector.h"
#include "curvefit.h"
#ifndef Q_OS_WIN
#include "CIComm.h"
#endif

enum XYZ_DIR
{
	XY_LEFT,
	XY_RIGHT,
	XY_TOP,
	XY_BTM,
	Z_DIR,
	XY_LEFT_TOP,
	XY_RIGHT_TOP,
	XY_LEFT_BTM,
	XY_RIGHT_BTM
};

enum inputMode
{
	ui_input,
	motion_input
};

extern inputMode g_InputMode;

enum RM_OP
{
	RM_Axis_XY,
	RM_Axis_Z,
	RM_Axis_Count
};

enum MEA_NUM
{
	XY_MIN_LEFT,
	XY_MAX_LEFT,
	XY_MIN_RIGHT,
	XY_MAX_RIGHT,
	XY_MIN_TOP,
	XY_MAX_TOP,
	XY_MIN_BTM,
	XY_MAX_BTM,
	Z_MIN,
	Z_MAX,
	MEA_MACRO_COUNT
};

enum XY_PNT_SET_INDEX
{
	_leftB, _leftE, 
	_rightB, _rightE,
	_topB, _topE,
	_btmB, _btmE,
	_leftTopB, _leftTopE,
	_rightTopB, _rightTopE,
	_leftBtmB, _leftBtmE,
	_rightBtmB, _rightBtmE,
	_zB, _zE,
	pnt_set_index_count
};

extern unsigned short g_XYIndex[_rightBtmE+1];
extern unsigned short g_ZIndex[2];

struct LineNCInfo
{
	double vPos[4];
	double vCent[4];
	double dR;
	double dF;
	long   lReading;
	char   chG171819;
	char   chG0123;
};

# define FONT_12_SIMHEI_LIGHT  QFont("SIMHEI", 12, QFont::Light)
# define FONT_10_SIMHEI_LIGHT  QFont("SIMHEI", 10, QFont::Light)

#ifdef Q_OS_WIN
# define NCFilePath  "f:/"
#else
# define NCFilePath  "/home/Lynuc/Users/NCFiles/"
#endif

//< CI �����ڴ��׵�ַ
extern int g_CIAddress;
//< ���������
extern QMap<int, RS_Vector> g_SavedData;
//< X/Y/Z��ƫ����
extern double g_CalOff[3];
//< �ֶ�����
extern double g_ManOff[3];
//< ƫ��
extern double g_CalRadOff;
//< 
extern double g_DecRadius;
extern unsigned short g_Index[MEA_MACRO_COUNT];

extern unsigned short g_XYSrcDataBeginNum;
extern unsigned short g_XYSrcDataEndNum;
extern unsigned short g_XYDesDataBeginNum;
extern unsigned short g_XYDesDataEndNum;
extern unsigned short g_XYSrcDataCount;
extern unsigned short g_XYDesDataCount;
extern unsigned short g_ZSrcDataBeginNum;
extern unsigned short g_ZSrcDataEndNum;
extern unsigned short g_ZDesDataBeginNum;
extern unsigned short g_ZDesDataEndNum;
extern unsigned short g_ZDataCount;

extern bool g_bIsRadRdy;
extern bool g_bIsDxfRdy;
extern bool g_bHasRmOff;
extern bool g_bIsNCRdy;

extern int g_nInOrOut;

extern RS_EntityContainer * g_ContainerPtr;


//< G100 ���������
#define MACRO_P150L10_FALG       200
#define MACRO_P150L11_FLAG       201
#define MACRO_INDEX              202
#define MACRO_P150L12_FLAG       203
#define MACRO_P150L12_F          204
#define MACRO_P150L12_T          205
#define MACRO_P150L25_FLAG       206
#define MACRO_P150L20_FLAG       207
#define MACRO_P150L22_S          208
#define MACRO_P150L22_DATARDY    209
#define MACRO_P150L21_FLAG       210
#define MACRO_P150L35_FLAG       211
#define MACRO_P150L30_FLAG       212
#define MACRO_P150L32_S          213
#define MACRO_P150L32_FLAG       214
#define MACRO_P150L31_FLAG       215

#define MACRO_P151L51_I          216
#define MACRO_P151L51_J          217
#define MACRO_P151L51_FALG       218
#define MACRO_P151L51_K          219

#define MACRO_P151L10L11_I       220
#define MACRO_P151L10L11_J       221
#define MACRO_P151L10L11_A       222
#define MACRO_P151L10L11_B       223
#define MACRO_P151L10L11_Q       224
#define MACRO_P151L10L11_R       225
#define MACRO_P151L10L11_U       226
#define MACRO_P151L10L11_V       227
#define MACRO_P151L10_FLAG       228
#define MACRO_P151L11_K          229 
#define MACRO_P151L11_FLAG       230
#define MACRO_ALL_WRONG          231
#define MACRO_P151L11_W          232
#define MACRO_P151L12_FLAG       233

#define MACRO_X_DATA             234
#define MACRO_Y_DATA             235
#define MACRO_Z_DATA             236

#define MACRO_P155L10_FLAG       240
#define MACRO_P155L11_Q          241
#define MACRO_P155L11_FLAG       242
#define MACRO_P155L12_I          243
#define MACRO_P155L12_J          244
#define MACRO_P155L12_FLAG       245
#define MACRO_P155L31_Q          246
#define MACRO_P155L31_FLAG       247
#define MACRO_P155L50_Q          248
#define MACRO_P155L50_FLAG       249


#define MACRO_P151L60_A          260
#define MACRO_P151L61_FLAG       261
#define MACRO_P151L21_I          262
#define MACRO_P151L21_J          263
#define MACRO_P151L21_FLAG       264
#define MACRO_P152L11_FLAG       265
#define MACRO_P152L11_I          266
#define MACRO_P152L11_J          267
#define MACRO_NcNotMatch_FLAG    268

#define MACRO_P152_XYBEGIN       271
#define MACRO_P152_XYEND         272
#define MACRO_P152_ZBEGIN        275
#define MACRO_P152_ZEND          276
#define MACRO_P152_INorOUT       290
#define MACRO_P152_DET_RAD       291

#define MACRO_X_GET_DATA         281
#define MACRO_Y_GET_DATA         282
#define MACRO_Z_GET_DATA         283
#define MACRO_RAD_OFF            284  //< ��������Ļ���ֵ
#define MACRO_ANGLE_OFF          285  //< ��������ĽǶ�ֵ

#define MACRO_P151L60_I          294
#define MACRO_P151L60_J          295
#define MACRO_P151L60_Q          296
#define MACRO_P151L60_R          297
#define MACRO_P151L60_FLAG       298

#define MACRO_CAD_START_SUC      299

//< G100���ݱ�ŷ�Χ
#define G100_INDEX_MAX  500
#define G100_INDEX_MIN  1

extern QPublicSignal *g_pQPublicSignal;				// �����ź��� 
extern ENUM_LOCATION_STATE g_enumLocationState;		// ��λ��ǰ״̬
extern ENUM_LOCATION_STATE g_enumLocationStatePre;	//
extern ENUM_PROM_STATE g_enumProMState;				// ����ӹ�ģ���Ӳ���״̬
extern ENUM_SYS_MODULE g_SysModule;					// ϵͳ��ǰ������ģ�飬��ʼֵΪͼ�μӹ�ģ��
extern ENUM_SYS_MODULE g_SysModulePre;				// 
extern double g_fontRatio;							// ȫ������������ű��� 
extern int g_nDecimal;  							// С������ʾλ��
extern int g_nTotal;
extern bool g_bABSCrdClr;
extern bool g_bMouseWheel;
extern bool g_bOffset_NCGte;
extern bool g_bG92;
extern COLORINFO g_ColorInfo[8];
extern bool g_bSaveNewFile;
extern QString g_strDYWConfigPath_m;
extern bool g_bHandle;

extern double g_dTracingDis;

enum EDM_MODETYPE {
	MT_CODELESS = 1,
	MT_MEM,
	MT_REMOTE,
	MT_HANDLE,
	MT_JOG,
	MT_HOME,
	MT_DRNC,
	MT_MDI
};

enum MACHINE_STATE
{
	MACHINE_STATE_READY = 0,      // ����
	MACHINE_STATE_RUNNING,        // ����
	MACHINE_STATE_PAUSE,          // ��ͣ
	MACHINE_STATE_STOP            // ֹͣ
};


//��ʾϵͳ��ʾ��������Ϣ
extern QLabel *g_messagelabel;
// NC�ļ���
//extern QLabel *g_pLabelNCname;
// ִ�е�ǰ��
extern QLabel *g_pLabelNowRow;
// NC�ļ�������
extern QLabel *g_pLabelTotalRow;

//����,Σ����Ϣ����
extern int g_warnordanger_lock;		   // 0 :Σ����Ϣδ����, 1:Σ����Ϣ�Ѽ���
//DXF���ݱ���·��
extern QString G_DXFPATH;
//NC���ݱ���·��
extern QString G_NCPATH;
// Motion����·��
extern QString G_MTPATH;
//�ӹ���NC���ݱ���·��
extern QString G_EXECUTE_NCPATH;
//�����ʼ�����ϵ�����
extern QString G_INITDATAPATH;
//
extern QString G_CONFPATH;
// 
extern QString G_IMAGESPATH;
//�����Ա���·��
extern QString G_MULTILANGUAGEPATH;

extern double g_dCalAccuracy;	   // ���㾫��

extern int g_FontSize;
extern QString g_FontName;				// ������
extern bool g_FontBold;	

extern int g_FontSize_MainMenu;			// �����С
extern QString g_FontName_MainMenu;		// ��������SimHei��
extern bool g_FontBold_MainMenu;		// �����Ƿ����

extern int g_FontSize_ABCCoord1;		// �����С
extern QString g_FontName_ABCCoord1;	// ��������SimHei��
extern bool g_FontBold_ABCCoord1;		// �����Ƿ����

extern int g_FontSize_ABCCoord2;		// �����С
extern QString g_FontName_ABCCoord2;	// ��������SimHei��
extern bool g_FontBold_ABCCoord2;		// �����Ƿ����

extern int g_FontSize_ABCCoord3;		// �����С
extern QString g_FontName_ABCCoord3;	// ��������SimHei��
extern bool g_FontBold_ABCCoord3;		// �����Ƿ����1

extern int g_FontSize_INCCoord1;		// �����С
extern QString g_FontName_INCCoord1;	// ��������SimHei��
extern bool g_FontBold_INCCoord1;		// �����Ƿ����

extern int g_FontSize_INCCoord2;		// �����С
extern QString g_FontName_INCCoord2;	// ��������SimHei��
extern bool g_FontBold_INCCoord2;		// �����Ƿ����

extern int g_FontSize_INCCoord3;		// �����С
extern QString g_FontName_INCCoord3;	// ��������SimHei��
extern bool g_FontBold_INCCoord3;		// �����Ƿ����

extern int g_FontSize_Tool;				// �����С
extern QString g_FontName_Tool;			// ��������SimHei��
extern bool g_FontBold_Tool;			// �����Ƿ����

extern int g_FontSize_State;			// �����С
extern QString g_FontName_State;		// ��������SimHei��
extern bool g_FontBold_State;			// �����Ƿ����

extern int g_FontSize_other;			// �����С
extern QString g_FontName_other;		// ��������SimHei��
extern bool g_FontBold_other;			// �����Ƿ����

extern int g_FontSizeTextEdit;			// �����С
extern QString g_FontNameTextEdit;		// ��������SimHei��
extern bool g_FontBoldTextEdit;			// �����Ƿ����

//��ť�߿����
#define G_COLOR_DGRAY_R		50
#define G_COLOR_DGRAY_G		50
#define G_COLOR_DGRAY_B		50

//��ť����ī�ڱ���ɫ
#define G_COLOR_INKY_R		90
#define G_COLOR_INKY_G		90
#define G_COLOR_INKY_B		90

//��ť�߿�����
#define G_COLOR_NAVY_R		11
#define G_COLOR_NAVY_G		97
#define G_COLOR_NAVY_B		255

//��ť��������ɫ
#define G_COLOR_LBLUE_R		175
#define G_COLOR_LBLUE_G		202
#define G_COLOR_LBLUE_B		255

//����
//#define G_COLOR_WHITE		255	

//error message
#define SYS_ERROR_NONE		"ϵͳ״̬����"

#ifdef _WIN32
extern int g_nMotion;							// Motion���Թ����Ƿ���,1,����
#endif

//< ���Ƕ�ת����[-PI, PI]֮��
extern double correctAngleMPIToPI( double agl );
//< ���Ƕ�ת����[0�� 2PI]֮��
extern double correctAngle0To2PI( double agl );
//< ����ת�����Ƕ�
inline double radToDeg( double a );
//< ���ÿؼ�����ɫ
extern void fillWidgetBackground( QWidget *, QColor);
//< ��������ʱ����
extern void delayMsecNoWait(int msec);
//< ������Ӧ����ʾ
extern void popMessageBox( QString title, QString content, int fontSize, QColor contentColor, bool islight
	, QMessageBox::StandardButton btn = QMessageBox::Ok );
//< 
extern QString qstrToHTML( QString content, int fontSize, QColor contentColor, bool isLight );

enum lineFlag{ LineX, LineY };
extern double getAngleOfLine( int indexB, int indexE, int flag );

//< ��ȡ�����������ֵ
extern int getMacroInt( int nMacro );



#endif // PARAMETER_H
