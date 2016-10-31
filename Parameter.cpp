/*-----------------------------------
Hotman ȫ�ֱ�����ʼ��Դ�ļ�
������ Liu.y.c
ʱ�䣺 2015.09.01
-----------------------------------*/

#include "Parameter.h"

#include <QString>
#include <QFontDatabase>

#ifdef CONTROLLER_TYPE_C2
	#include "mainwindow.h"
	MainWindow          *g_gMainWindow      = NULL;						// ָ��������ָ��
#endif
							       
//QErrorMessage     *g_QErrorMessage    = new QErrorMessage();						// ������Ϣ��ʾ
QPublicSignal       *g_pQPublicSignal;						// �����ź��� 
ENUM_LOCATION_STATE g_enumLocationState = LOCATION_INIT;							// ��λģ��״̬������λģ����ִ�еĲ���;
ENUM_LOCATION_STATE g_enumLocationStatePre = LOCATION_INIT;							// 
ENUM_PROM_STATE     g_enumProMState     = PROM_INIT;								// ����ӹ�ģ���Ӳ���״̬
ENUM_SYS_MODULE     g_SysModule         = LOCATION_MODULE;							// ϵͳ��ǰ������ģ�飬��ʼֵΪͼ�μӹ�ģ��
ENUM_SYS_MODULE     g_SysModulePre      = LOCATION_MODULE;							// ϵͳ��ǰ����ģ���ǰһ״̬����ʼֵΪͼ�μӹ�ģ��
COLORINFO           g_ColorInfo[8]      = {cl_white, cl_red, cl_orange, cl_yellow, cl_green, cl_cyan_blue, cl_blue, cl_purple};   

int g_CIAddress      = 0;
QMap<int, RS_Vector> g_SavedData;
double g_CalOff[3]   = {0.};
double g_ManOff[3]   = {0.};
double g_CalRadOff = 0.;
double g_DecRadius = 0.;
unsigned short g_Index[MEA_MACRO_COUNT] = { 0 };
unsigned short g_XYIndex[_rightBtmE+1] = { 0 };
unsigned short g_ZIndex[2] = { 0 };
unsigned short g_XYSrcDataBeginNum = 0;
unsigned short g_XYSrcDataEndNum   = 0;
unsigned short g_XYDesDataBeginNum = 0;
unsigned short g_XYDesDataEndNum   = 0;
unsigned short g_XYSrcDataCount    = 0;
unsigned short g_XYDesDataCount    = 0;
unsigned short g_ZSrcDataBeginNum  = 0;
unsigned short g_ZSrcDataEndNum    = 0;
unsigned short g_ZDesDataBeginNum  = 0;
unsigned short g_ZDesDataEndNum    = 0;
unsigned short g_ZDataCount        = 0;

bool g_bIsRadRdy = false;
bool g_bIsDxfRdy = false;
bool g_bHasRmOff = false;
bool g_bIsNCRdy  = false;

int g_nInOrOut = 0;
inputMode g_InputMode = ui_input;
RS_EntityContainer * g_ContainerPtr = NULL;


bool g_bMouseWheel   = true;													// ��Ӧ�������¼���־,false������Ӧ;

bool g_bOffset_NCGte = true;														// ���ɵ�NC�����Ƿ����ò���,fales,������;
bool g_bABSCrdClr    = false;														// ��λģ���еľ�����������ı�־��true:���㣻false:�����㣻
bool g_bG92          = true;														// G92ָ���Ƿ����ɵ�NC������,false,������;
bool g_bSaveNewFile  = false;														// true,ϵͳ�����ļ������,�л����ļ�����ģ����Ҫ�����ļ���ʾ�б�
bool g_bHandle       = false;														// Vdesk�������Ƿ���
double g_dTracingDis       = 5;														// ʵʱ���ʱ����������(ת��Ϊ��Ļ����ľ���)����5�Ż���
double g_fontRatio         = 0.0;													// ȫ������������ű���						   

QLabel *g_messagelabel;				// ��ʾϵͳ��ʾ��������Ϣ
QLabel *g_pLabelNowRow;				// ִ�е�ǰ��
QLabel *g_pLabelTotalRow;				// NC�ļ�������


int g_warnordanger_lock = 0;	  // 0 :Σ����Ϣδ����, 1:Σ����Ϣ�Ѽ���

// ��˿������Ϣ�洢·��
#ifdef _WIN32
	QString g_strDYWConfigPath_m = "./Configuration/DY_EDW_M.txt";	
#else
	QString g_strDYWConfigPath_m = "/home/Lynuc/Users/Config/DY_EDW_M.txt";	
#endif


#ifdef _WIN32
QString G_IMAGESPATH = "./images/";
#else 
QString G_IMAGESPATH = "/home/Lynuc/UI/DYW/images/";
#endif

#ifdef _WIN32
QString G_CONFPATH = "./Configuration/";
#else 
QString G_CONFPATH = "/home/Lynuc/Users/Configuration/";
#endif

#ifdef _WIN32
QString G_INITDATAPATH = "./initdata/";
#else 
QString G_INITDATAPATH = "/home/Lynuc/Users/initdata/";
#endif

#ifdef _WIN32
QString G_MULTILANGUAGEPATH = "./MultiLanguage/";
#else 
QString G_MULTILANGUAGEPATH = "/home/Lynuc/Users/MultiLanguage/";
#endif

//�ӹ���NC���ݱ���·��
#ifdef _WIN32
QString G_EXECUTE_NCPATH = "./NCFiles/";
#else 
QString G_EXECUTE_NCPATH = "/home/Lynuc/Users/NCFiles/";
#endif

#ifdef _WIN32
QString G_DXFPATH = "./DXFfile/";
#else 
QString G_DXFPATH = "/home/Lynuc/Users/DXFfile/";
#endif

#ifdef _WIN32
QString G_NCPATH = "./NCfile/";
#else 
QString G_NCPATH = "/home/Lynuc/Users/NCfile/";
#endif

// Motion����·��
#ifdef _WIN32
QString G_MTPATH = "./Motion/";
#else 
QString G_MTPATH = "/home/Lynuc/Users/Motion/";
#endif

#ifndef _WIN32
// 
int g_FontSize         = 17;				// �����С
QString g_FontName     = "SimHei";			// ��������SimHei��
bool g_FontBold        = false;				// �����Ƿ����

int g_FontSize_MainMenu     = 23;			// �����С
QString g_FontName_MainMenu = "SimHei";		// ��������SimHei��
bool g_FontBold_MainMenu    = false;		// �����Ƿ����

int g_FontSize_ABCCoord1     = 23;			// �����С
QString g_FontName_ABCCoord1 = "SimHei";	// ��������SimHei��
bool g_FontBold_ABCCoord1    = false;		// �����Ƿ����

int g_FontSize_ABCCoord2     = 26;			// �����С
QString g_FontName_ABCCoord2 = "SimHei";		// ��������SimHei��
bool g_FontBold_ABCCoord2    = true;		// �����Ƿ����

int g_FontSize_ABCCoord3     = 17;			// �����С
QString g_FontName_ABCCoord3 = "SimHei";	// ��������SimHei��
bool g_FontBold_ABCCoord3    = true;		// �����Ƿ����

int g_FontSize_INCCoord1     = 23;			// �����С
QString g_FontName_INCCoord1 = "SimHei";	// ��������SimHei��
bool g_FontBold_INCCoord1    = false;		// �����Ƿ����

int g_FontSize_INCCoord2     = 26;			// �����С
QString g_FontName_INCCoord2 = "SimHei";	// ��������SimHei��
bool g_FontBold_INCCoord2    = true;		// �����Ƿ����

int g_FontSize_INCCoord3     = 17;			// �����С
QString g_FontName_INCCoord3 = "SimHei";		// ��������SimHei��
bool g_FontBold_INCCoord3    = true;		// �����Ƿ����

int g_FontSize_Tool     = 12;				// �����С
QString g_FontName_Tool = "SimHei";			// ��������SimHei��
bool g_FontBold_Tool    = false;			// �����Ƿ����

int g_FontSize_State     = 14;				// �����С
QString g_FontName_State = "SimHei";		// ��������SimHei��
bool g_FontBold_State    = false;			// �����Ƿ����

int g_FontSize_other     = 14;				// �����С
QString g_FontName_other = "SimHei";		// ��������SimHei��
bool g_FontBold_other    = false;			// �����Ƿ����

int g_FontSizeTextEdit     = 17;				// �����С
QString g_FontNameTextEdit = "SimHei";		// ��������SimHei��
bool g_FontBoldTextEdit    = false;			// �����Ƿ����

#else
// 
int g_FontSize         = 13;				// �����С
QString g_FontName     = "SimHei";			// ��������SimHei��
bool g_FontBold        = false;				// �����Ƿ����

int g_FontSize_MainMenu     = 17;			// �����С
QString g_FontName_MainMenu = "SimHei";		// ��������SimHei��
bool g_FontBold_MainMenu    = false;		// �����Ƿ����

int g_FontSize_ABCCoord1     = 17;			// �����С
QString g_FontName_ABCCoord1 = "SimHei";	// ��������SimHei��
bool g_FontBold_ABCCoord1    = false;		// �����Ƿ����

int g_FontSize_ABCCoord2     = 19;			// �����С
QString g_FontName_ABCCoord2 = "SimHei";		// ��������SimHei��
bool g_FontBold_ABCCoord2    = true;		// �����Ƿ����

int g_FontSize_ABCCoord3     = 10;			// �����С
QString g_FontName_ABCCoord3 = "SimHei";	// ��������SimHei��
bool g_FontBold_ABCCoord3    = true;		// �����Ƿ����

int g_FontSize_INCCoord1     = 17;			// �����С
QString g_FontName_INCCoord1 = "SimHei";	// ��������SimHei��
bool g_FontBold_INCCoord1    = false;		// �����Ƿ����

int g_FontSize_INCCoord2     = 19;			// �����С
QString g_FontName_INCCoord2 = "SimHei";	// ��������SimHei��
bool g_FontBold_INCCoord2    = true;		// �����Ƿ����

int g_FontSize_INCCoord3     = 10;			// �����С
QString g_FontName_INCCoord3 = "SimHei";		// ��������SimHei��
bool g_FontBold_INCCoord3    = true;		// �����Ƿ����

int g_FontSize_Tool     = 11;				// �����С
QString g_FontName_Tool = "SimHei";			// ��������SimHei��
bool g_FontBold_Tool    = false;			// �����Ƿ����

int g_FontSize_State     = 11;				// �����С
QString g_FontName_State = "SimHei";		// ��������SimHei��
bool g_FontBold_State    = false;			// �����Ƿ����

int g_FontSize_other     = 11;				// �����С
QString g_FontName_other = "SimHei";		// ��������SimHei��
bool g_FontBold_other    = false;			// �����Ƿ����

int g_FontSizeTextEdit     = 11;			// �����С
QString g_FontNameTextEdit = "SimHei";		// ��������SimHei��
bool g_FontBoldTextEdit    = false;			// �����Ƿ����

#endif

double g_dCalAccuracy  = 0.0001;			// ���㾫��
int g_nTotal           = 9;					// ����ϵͳ��һ����ʾ9λ��	
int g_nDecimal         = 4;					// С������ʾλ��

#ifdef _WIN32
int g_nMotion = 0;							// Motion���Թ����Ƿ���,1,����
#endif

double correctAngleMPIToPI( double agl )
{
	while( agl >= M_PI )
	{
		agl -= ( M_PI * 2. );
	}

	while( agl < -M_PI )
	{
		agl += ( M_PI * 2. );
	}

	return agl;
}

double correctAngle0To2PI( double agl )
{
	while( agl >= M_PI * 2. )
	{
		agl -= ( M_PI * 2. );
	}
	while( agl < 0. )
	{
		agl += ( M_PI * 2. );
	}

	return agl;
}

//< ����ת�Ƕ�
double radToDeg( double a )
{
	return ( a * 180.0 / M_PI );
}

void fillWidgetBackground(QWidget *widget, QColor color)
{
	QPalette palette( widget->palette() );
	palette.setBrush( widget->backgroundRole(), QBrush( color ) );
	widget->setAutoFillBackground(true);

	widget->setPalette(palette);
}

void delayMsecNoWait(int msec)
{
	QTime dieTime = QTime::currentTime().addMSecs(msec);

	while( QTime::currentTime() < dieTime )
	{
		QCoreApplication::processEvents();
	}

	return;
}

//< ������Ϣ��ͬʱ����������Ϣ�������������ʽ
void popMessageBox(QString title, QString content, int fontSize, QColor contentColor, bool islight, QMessageBox::StandardButton btn)
{
	QMessageBox msgBox( QMessageBox::NoIcon, title, qstrToHTML(content, fontSize, contentColor, islight) );

	msgBox.exec();

}

QString qstrToHTML( QString content, int fontSize, QColor contentColor, bool isLight )
{
	QString strColor = contentColor.name();
#ifdef __arm__   //< N3����������⣬��һ����������ʾ�ĺܳ�ֻ�����ƴ�С
	if( fontSize > 3 )
		fontSize = 3;
#endif

	QString contentFont = "<font color=" + strColor + " size=\"" + QString::number(fontSize) + "\"";
	if( isLight )
		contentFont += " style=\"font-weight:light;font-family:simhei\">";
	else
		contentFont += " style=\"font-weight:bold;font-family:simhei\">";

	//< HTML�в�ʶ��C++���з����滻��HTML�Ļ��з�
	content.replace( '\n', "<br />");
	QString contentInHtml = contentFont + content + "</font>";

	return contentInHtml;
}

double getAngleOfLine( int indexB, int indexE, int flag )
{
	double angle = 0.;

	//< ��ȡ��С���������������в���
	int size  = indexE - indexB + 1;
	if( size <= 1 )
		return 0.;

	double * x = new double[size];
	double * y = new double[size];
	const int order = 1;
	double *a = new double[order+1];
	for( int i = indexB; i <= indexE; ++i )
	{
		if( !g_SavedData.contains( i ) )
		{
			delete [] a;
			delete [] y;
			delete [] x;
			return 0.;
		}
		x[i-indexB] = g_SavedData[i].x;
		y[i-indexB] = g_SavedData[i].y;
	}
	FIT::curvefit( size, x, y, order, a );

	//< ��ϵĽ���Ի��ȵ���ʽ������
	double rad = atan( a[order] );  //< -pi/2 ~ pi/2
	if( flag == LineY )
		angle = ( rad >= 0. ) ? ( rad - M_PI * 0.5 ) : ( rad + M_PI * 0.5 );
	else if( flag == LineX )
		angle = rad;

	delete [] a;
	delete [] y;
	delete [] x;

	return angle;
}

int getMacroInt( int nMacro )
{
	double tmp = 0.;
#ifndef Q_OS_WIN
	GetMacroVal( g_CIAddress, nMacro, tmp );
#endif
	if( fabs( tmp ) < 10e-6 )  //< 0����
		return 0;
	else if( tmp > 10e-6 )  //< ����0
		return static_cast<int>( tmp + 10e-6 );
	else  //< С��0
		return static_cast<int>( tmp - 10e-6 );
}