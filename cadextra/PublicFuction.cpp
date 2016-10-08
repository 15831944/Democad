/*
 *���������ļ�
 *
 */
#include <QFileInfo>
#include <QRegExp>
#include <QRegExpValidator>
#include <QDesktopWidget>
#include <QTime>
#include <time.h>
#include <QTextBlockFormat>
#include <QTextBlock>
#include <QTextCursor>
#include <QScrollBar>
#include "Parameter.h"
#include "PublicFuction.h"
// #include "include/Lynuc2Interface.h"
// #include "include/LynucGui.h"
// #include "include/LyMessageObject.h"
//#include "plcif.h"
#include "QDebug"



/*��������ReSizeCsCal
 *���ܣ�ȫ��
 *��ڲ�����x,y,w,h:��ǰ�ؼ���λ��
 *        nx,ny,nw,nh:��������ȫ���󣬿ؼ���λ��
 *        screenx,screeny:��ǰ��ʾ��ƽ��Ĵ�С
 *        realx,realy:ʵ�ʻ��ƵĴ�С
 *        iMode:�Ƿ�����Ϊ������1��������������1������Ϊ����
 *        Lflag:�Ƿ���Ҫƽ�ơ���Щ�ؼ�����Ϊȫ��ʱ����Ҫƽ�ƣ���Щ��Ҫƽ�ơ�Ϊtrue,��ƽ�ƣ�false��ƽ�ơ�
 *���ز�����nx,ny,nw,nh:��������ȫ���󣬿ؼ���λ��
 *
 */
void ReSizeCsCal(int x, int y, int w, int h, int* nx, int* ny, int* nw, int* nh,
                 int screenx, int screeny, int realx, int realy, int iMode, bool Lflag)
{
	if (0 == realx || 0 == realy)
	{
		return;
	}

    double dratiox = screenx / ((double)realx);
    double dratioy = screeny / ((double)realy);

    if(iMode == 1)
    {
        double dratio;
        double dMovex,dMovey;
        if(dratiox > dratioy)
        {
            dratio = dratioy;
            dMovex = (int)((screenx - realx * dratio) * 0.5);
            dMovey = 0;
        }
        else
        {
            dratio = dratiox;
            dMovey = (int)((screeny - realy * dratio) * 0.5);
            dMovex = 0;
        }

        if ( dratiox-dratioy > -0.0001 && dratiox-dratioy < 0.0001)
        {
            dMovex = 0;
            dMovey = 0;
        }

        if(Lflag)   // �Ƿ��ƶ�
		{
			*nx = (int)(x * dratio);
			*ny = (int)(y * dratio);
        }
        else
        {
            *nx = (int)(x * dratio) + dMovex;
            *ny = (int)(y * dratio) + dMovey;
        }

        *nw = (int)(w * dratio);
        *nh = (int)(h * dratio);
    }
    else
    {
        *nx = (int)(x * dratiox);
        *ny = (int)(y * dratioy);
        *nw = (int)(w * dratiox);
        *nh = (int)(h * dratioy);
    }

    g_fontRatio = dratiox<dratioy?dratiox:dratioy;
}

/*
*FUNCTION :
*	SetPushButtonSheetStyle
*DESCRIPTION : 
*	���ð�ťԲ����ʾ,����̬,����,���±�����ɫ
*INT : 
*	bgcolor_r,bgcolor_g,bgcolor_b:��ť��̬����RGB;
*	outR: ��ť���Բ�뾶
*	inR: ��ť�ڽ�Բ�뾶
*	bcolor_r,bcolor_r,bcolor_r:����ڰ�ť�ϱ���RGB;
*	pbgcolor_r,pbgcolor_r,pbgcolor_r:��ť���±���RGB;
*OUT :
*RETURN :
*	QString sheetStyle
*/
QString SetPushButtonSheetStyle(int bgcolor_r, int bgcolor_g, int bgcolor_b, int outR, int inR,
									int bcolor_r, int bcolor_g, int bcolor_b,
									int pbgcolor_r, int pbgcolor_g, int pbgcolor_b)
{
	QString ret1 = QString("QPushButton{background-color:rgb(%1,%2,%3);border-radius:%4px;border:%5px groove gray;border-style:outset;border-color:rgb(%6,%7,%8);}").arg(bgcolor_r).arg(bgcolor_g).arg(bgcolor_b).arg(outR).arg(inR).arg(bcolor_r).arg(bcolor_g).arg(bcolor_b);
	QString ret2 = QString("QPushButton:hover{background-color:white;}");
	QString ret3 = QString("QPushButton:pressed{background-color:rgb(%1,%2,%3);border-style:inset;}").arg(pbgcolor_r).arg(pbgcolor_g).arg(pbgcolor_b);
	QString ret4 = QString("QPushButton:focus{padding:-1xp;}"); 
	return ret1 /*+ ret2*/ + ret3 + ret4;
}
/*
* Funciton Name: JudgeMachineCondition()
*	�жϼӹ�������ʽ�Ƿ���ȷ
* INPUT��
*	QString mc���ӹ�����
* OUTPUT��
*	bool 
*/
bool JudgeMachineCondition(QString mc)
{
	QString::iterator i = mc.begin();

	if (mc.capacity() != 5)
	{
		return false;
	}
	else if (*i != 'C')
	{
		return false;
	}

	i++;
	while (i!=mc.end())
	{
		if (*i>'9' || *i<'0')
		{
			return false;
		}

		i++;
	}

	return true;
}

/*
* Funciton Name: checkIllegalChar()
*	�ж��ַ������Ƿ���ڷǷ��ַ�
* INPUT��
*	QString qChar�����ж��ַ�
* OUTPUT��bool
*	false:�����ڷǷ��ַ�
*	true: ���ڷǷ��ַ�
*/
bool checkIllegalChar(QString qStr)
{
	if (qStr.isEmpty())
	{
		return true;
	}

	QString::iterator i = qStr.begin();
	bool bCheck         = false;

	if (*i == '-')
	{
		i++;
	}

	while (i != qStr.end())
	{
		if (!(*i).isDigit() && (*i) != '.')
		{
			bCheck = true;
			break;
		}

		i++;
	}

	return bCheck;
}

/*
* Funciton Name: strFormat()
*	�ַ����ĸ�ʽ����ʾ
* INPUT��
*	QString &qStr������ʽ�����ַ���
*	int iTotal:    �ַ�����ʾ����λ��
*	int iSub��	   С������λ��
* OUTPUT��
*	void
*/
void strFormat(QString &qStr, int iTotal, int iSub)
{
	QString qFrontStr = "";
	QString qBackStr  = "";

	QFileInfo qFileInfo(qStr);
	qFrontStr = qFileInfo.baseName();
	qBackStr  = qFileInfo.suffix();

// 	QByteArray qBa = qFrontStr.toLocal8Bit();
// 	int iFrontStr  = strlen(qBa.data());

//	qBa.clear();
	QByteArray qBa = qBackStr.toLocal8Bit();
	int iBackStr   = strlen(qBa.data());

// 	int iNumF = iTotal-iSub-1-iFrontStr;
// 	for (int i=0; i<iNumF; i++)
// 	{
// 		qFrontStr.push_front('0');
// 	}

	int iNumB = iSub-iBackStr;
	for (int j=0; j<iNumB; j++)
	{
		qBackStr.push_back('0');
	}

// 	if (iNumF<0)
// 	{
// 		return; 
// 	}
	
	if (iNumB<0)
	{
		qBackStr.remove(iSub, iBackStr-iSub);
	}

	qStr = qFrontStr + "." + qBackStr;

}

/*
* Funciton Name: readFile()
*	��ȡ�ļ���Ϣ
* INPUT��
*	QString Path��	         �ļ�·��
*	QVector<QString> mcBuff: �洢�����ݿ��еļӹ�����
* OUTPUT��bool
*	true:			   ��ȡ�ɹ�
*	false:			   ��ȡʧ��
*/
bool readFile(QString Path, QVector<QString> &mcBuff)
{
	if (Path.isNull() || Path.isEmpty())
	{
		return false;
	}

	FILE *fp;
	QByteArray qb = Path.toLocal8Bit();
	char *pPath   = qb.data();

	if ((fp = fopen(pPath,"r")) == NULL)
	{
		return false;
	}

	while (!feof(fp))
	{
		char buff[100] = {'\0'};
		fgets(buff, 100, fp);
		if (buff[0] == '\0' || buff[0] == ' ')
		{
			continue;
		}
		mcBuff.append(buff);
	}

	fclose(fp);
	return true;
}

/*
* Funciton Name: writeToFile()
*	���ӹ��������浽�ļ���
* INPUT��
*	QString Path��	         �ļ�·��
*	QVector<QString> mcBuff: �洢�����ݿ��еļӹ�����
* OUTPUT��bool
*	true:			      ��ȡ�ɹ�
*	false:			      ��ȡʧ��
*/
bool writeToFile(QString Path, QVector<QString> mcBuff)
{
	if (Path.isNull() || Path.isEmpty())
	{
		return false;
	}

	FILE *fp;
	QByteArray qb = Path.toLocal8Bit();
	char *pPath   = qb.data();

	if ((fp = fopen(pPath,"w")) == NULL)
	{
		return false;
	}

	int iSize = mcBuff.size();
	for (int n=0; n<iSize; n++)
	{
		QByteArray qb = (mcBuff.at(n)).toLocal8Bit();
		char *pBuff   = qb.data();
		fputs(pBuff, fp);
	}
	fclose(fp);
	return true;
}

//��ȡ�ļ���
//input:
//		fullfilename : ȫ·����
//output:
//		filename:	�ļ���
void getFileName(QString fullfilename, char *filename)
{
	int len = 0, si = 0;
	char tmp[128] = {'\0'};

	strcpy(tmp, fullfilename.toAscii());
	len = strlen(tmp);
	si = len - 1;

	while(tmp[si] != '/')
	{
		si--;
	}

	strncpy(filename, &tmp[si + 1], len - si - 1);
}

/*
* Description: 
*	���汾��ת�����ַ�����ʽ
* Parameters:
*	int nVersion:       �����汾��
* Return Value:bool
*	QString strVersion: �����汾��ת���ɵ��ַ����Ͱ汾��
*/
QString VersionIntToStr(int nVersion)
{
	int nTemp          = 0;
	QString strVersion = "";
	
	strVersion.clear();
	for(int i=0; i<4; i++)
	{
		nTemp = (nVersion & 0xFF000000)>>24;
		(nTemp>9) ? (strVersion += QString::number(nTemp)):(strVersion += QString("0") + QString::number(nTemp));

		if (i!=3)
		{
			strVersion += QString(".");
		}

		nVersion <<= 8;
	}
	
	return strVersion;
}

/*
* Description: 
*	����QLineEdit���������������ı���ʽ��ֻ����������
* Parameters:
*	QObject *parent  : QLineEdit�������ĸ���
*   bool bInteger    : �Ƿ�Ϊ����,TRUE�ǣ�Ĭ��Ϊ�񣬼�С��
*	bool bUnInt      : ��������־��Ĭ��false,����������
* Return Value:bool
*	QRegExpValidator*: ��ʽ�������ʽ  
*/
QRegExpValidator* setInputDataFormat_Digit(QObject *parent, bool bInteger, bool bUnInt)
{
	// ���������������ʽ
	QString qStrInputFormat;
	if (!bInteger)
	{
		if (bUnInt)
		{
			// 
			QString qStrInt = QString(QObject::tr("0[.][0-9]{1,")) +
				QString::number(g_nDecimal) + QString(QObject::tr("}"));

			// 
			int nTemp = g_nTotal - g_nDecimal - 1;
			QString qStrDecimal = QString(QObject::tr("[1-9]{1}[0-9]{0,")) +
				QString::number(nTemp) + 
				QString(QObject::tr("}[.]{1}[0-9]{0,")) + QString::number(g_nDecimal) +
				QString(QObject::tr("}"));

			qStrInputFormat = qStrInt + QString(QObject::tr("|")) + qStrDecimal;
		}
		else
		{
			// 
			QString qStrInt = QString(QObject::tr("[-]{0,1}0[.][0-9]{1,")) +
				QString::number(g_nDecimal) + QString(QObject::tr("}"));

			// 
			int nTemp = g_nTotal - g_nDecimal - 1;
			QString qStrDecimal = QString(QObject::tr("[-]{0,1}[1-9]{1}[0-9]{0,")) +
				QString::number(nTemp) + 
				QString(QObject::tr("}[.]{1}[0-9]{0,")) + QString::number(g_nDecimal) +
				QString(QObject::tr("}"));

			qStrInputFormat = qStrInt + QString(QObject::tr("|")) + qStrDecimal;
		}


	}
	else
	{
		if (bUnInt)		// ������
		{
			qStrInputFormat = QString(QObject::tr("[1-9]{1}[0-9]{0,9}"));
		}
		else
		{
			qStrInputFormat = QString(QObject::tr("[-]{0,1}[1-9]{1}[0-9]{0,9}"));
		}

	}

	QRegExp qRegExp(qStrInputFormat);
	QRegExpValidator *qRegExpValidator = new QRegExpValidator(qRegExp, parent);

	return qRegExpValidator;

}

/*
* Description: 
*	����QLineEdit���������������ı���ʽ���������ֺ���ĸ�����
* Parameters:
*	QObject *parent: QLineEdit�������ĸ���
* Return Value:bool
*	QRegExpValidator*: ��ʽ�������ʽ
*/
QRegExpValidator* setInputDataFormat_DigitAlphabet(QObject *parent)
{
	// ���������������ʽ
	QString qStrInputFormat = QString(QObject::tr("[A-Za-z0-9_][A-Za-z0-9_-]{0,49}"));  // QString(QObject::tr("[A-Za-z0-9.]{0,20}"))
 
	QRegExp qRegExp(qStrInputFormat);
	QRegExpValidator *qRegExpValidator = new QRegExpValidator(qRegExp, parent);
	
	return qRegExpValidator;

}

/*
* Description:
*	��ͣ��ʱ��
* Parameters:
*	int nT: ��ͣʱ��
* Return Value:
*	void
*/
void HoldTimer(int nT)
{
	QTime qTime;
	qTime.start();

	int n = 0;

	while (n<nT)
	{
		n = qTime.elapsed();
		QCoreApplication::processEvents();
#ifndef _WIN32
		usleep(1);
#endif
		//qWarning() <<"========:"<<n<<"\n";
	}
}

/*
* Description:
*	����ϵͳ�����С
* Parameters:
*	int nScreenResolution: ��Ļ�ֱ���0,1024*768�� 1,800*600;
* Return Value:
*	int:	// ����ϵͳ��ǰ״̬
*		0: ����
*		1: ����
*		2: ��ͣ
*		3: ֹͣ
*/
void SetFontPara(int nScreenResolution)
{

	if (0 == nScreenResolution)
	{
#ifndef _WIN32
		// 
		g_FontSize         = 17;				// �����С
		g_FontName     = "SimHei";			// ��������SimHei��
		g_FontBold        = false;				// �����Ƿ����

		g_FontSize_MainMenu     = 23;			// �����С
		g_FontName_MainMenu = "SimHei";		// ��������SimHei��
		g_FontBold_MainMenu    = false;		// �����Ƿ����

		g_FontSize_ABCCoord1     = 23;			// �����С
		g_FontName_ABCCoord1 = "SimHei";	// ��������SimHei��
		g_FontBold_ABCCoord1    = false;		// �����Ƿ����

		g_FontSize_ABCCoord2     = 26;			// �����С
		g_FontName_ABCCoord2 = "SimHei";		// ��������SimHei��
		g_FontBold_ABCCoord2    = true;		// �����Ƿ����

		g_FontSize_ABCCoord3     = 17;			// �����С
		g_FontName_ABCCoord3 = "SimHei";	// ��������SimHei��
		g_FontBold_ABCCoord3    = true;		// �����Ƿ����

		g_FontSize_INCCoord1     = 23;			// �����С
		g_FontName_INCCoord1 = "SimHei";	// ��������SimHei��
		g_FontBold_INCCoord1    = false;		// �����Ƿ����

		g_FontSize_INCCoord2     = 26;			// �����С
		g_FontName_INCCoord2 = "SimHei";	// ��������SimHei��
		g_FontBold_INCCoord2    = true;		// �����Ƿ����

		g_FontSize_INCCoord3     = 17;			// �����С
		g_FontName_INCCoord3 = "SimHei";		// ��������SimHei��
		g_FontBold_INCCoord3    = true;		// �����Ƿ����

		g_FontSize_Tool     = 12;				// �����С
		g_FontName_Tool = "SimHei";			// ��������SimHei��
		g_FontBold_Tool    = false;			// �����Ƿ����

		g_FontSize_State     = 14;				// �����С
		g_FontName_State = "SimHei";		// ��������SimHei��
		g_FontBold_State    = false;			// �����Ƿ����

		g_FontSize_other     = 14;				// �����С
		g_FontName_other = "SimHei";		// ��������SimHei��
		g_FontBold_other    = false;			// �����Ƿ����

#else
		// 
		g_FontSize         = 13;				// �����С
		g_FontName     = "SimHei";			// ��������SimHei��
		g_FontBold        = false;				// �����Ƿ����

		g_FontSize_MainMenu     = 17;			// �����С
		g_FontName_MainMenu = "SimHei";		// ��������SimHei��
		g_FontBold_MainMenu    = false;		// �����Ƿ����

		g_FontSize_ABCCoord1     = 17;			// �����С
		g_FontName_ABCCoord1 = "SimHei";	// ��������SimHei��
		g_FontBold_ABCCoord1    = false;		// �����Ƿ����

		g_FontSize_ABCCoord2     = 19;			// �����С
		g_FontName_ABCCoord2 = "SimHei";		// ��������SimHei��
		g_FontBold_ABCCoord2    = true;		// �����Ƿ����

		g_FontSize_ABCCoord3     = 10;			// �����С
		g_FontName_ABCCoord3 = "SimHei";	// ��������SimHei��
		g_FontBold_ABCCoord3    = true;		// �����Ƿ����

		g_FontSize_INCCoord1     = 17;			// �����С
		g_FontName_INCCoord1 = "SimHei";	// ��������SimHei��
		g_FontBold_INCCoord1    = false;		// �����Ƿ����

		g_FontSize_INCCoord2     = 19;			// �����С
		g_FontName_INCCoord2 = "SimHei";	// ��������SimHei��
		g_FontBold_INCCoord2    = true;		// �����Ƿ����

		g_FontSize_INCCoord3     = 10;			// �����С
		g_FontName_INCCoord3 = "SimHei";		// ��������SimHei��
		g_FontBold_INCCoord3    = true;		// �����Ƿ����

		g_FontSize_Tool     = 11;				// �����С
		g_FontName_Tool = "SimHei";			// ��������SimHei��
		g_FontBold_Tool    = false;			// �����Ƿ����

		g_FontSize_State     = 11;				// �����С
		g_FontName_State = "SimHei";		// ��������SimHei��
		g_FontBold_State    = false;			// �����Ƿ����

		g_FontSize_other     = 11;				// �����С
		g_FontName_other = "SimHei";		// ��������SimHei��
		g_FontBold_other    = false;			// �����Ƿ����

#endif

	}
	else if (1 == nScreenResolution)
	{
#ifdef CONTROLLER_TYPE_C2
		g_FontSize         = 10;				// �����С
		g_FontName     = "SimHei";			// ��������SimHei��
		g_FontBold        = false;				// �����Ƿ����

		g_FontSize_MainMenu     = 14;			// �����С
		g_FontName_MainMenu = "SimHei";		// ��������SimHei��
		g_FontBold_MainMenu    = false;		// �����Ƿ����

		g_FontSize_ABCCoord1     = 16;			// �����С
		g_FontName_ABCCoord1 = "SimHei";	// ��������SimHei��
		g_FontBold_ABCCoord1    = false;		// �����Ƿ����

		g_FontSize_ABCCoord2     = 18;			// �����С
		g_FontName_ABCCoord2 = "SimHei";		// ��������SimHei��
		g_FontBold_ABCCoord2    = true;		// �����Ƿ����

		g_FontSize_ABCCoord3     = 10;			// �����С
		g_FontName_ABCCoord3 = "SimHei";	// ��������SimHei��
		g_FontBold_ABCCoord3    = true;		// �����Ƿ����

		g_FontSize_INCCoord1     = 16;			// �����С
		g_FontName_INCCoord1 = "SimHei";	// ��������SimHei��
		g_FontBold_INCCoord1    = false;		// �����Ƿ����

		g_FontSize_INCCoord2     = 18;			// �����С
		g_FontName_INCCoord2 = "SimHei";	// ��������SimHei��
		g_FontBold_INCCoord2    = true;		// �����Ƿ����

		g_FontSize_INCCoord3     = 10;			// �����С
		g_FontName_INCCoord3 = "SimHei";		// ��������SimHei��
		g_FontBold_INCCoord3    = true;		// �����Ƿ����

		g_FontSize_Tool     = 9;				// �����С
		g_FontName_Tool = "SimHei";			// ��������SimHei��
		g_FontBold_Tool    = false;			// �����Ƿ����

		g_FontSize_State     = 9;				// �����С
		g_FontName_State = "SimHei";		// ��������SimHei��
		g_FontBold_State    = false;			// �����Ƿ����

		g_FontSize_other     = 9;				// �����С
		g_FontName_other = "SimHei";		// ��������SimHei��
		g_FontBold_other    = false;			// �����Ƿ����

		qWarning()<< "define C2!";
		return;
#endif

#ifndef _WIN32
		// 
		g_FontSize         = 14;				// �����С
		g_FontName     = "SimHei";			// ��������SimHei��
		g_FontBold        = false;				// �����Ƿ����

		g_FontSize_MainMenu     = 19;			// �����С
		g_FontName_MainMenu = "SimHei";		// ��������SimHei��
		g_FontBold_MainMenu    = false;		// �����Ƿ����

		g_FontSize_ABCCoord1     = 20;			// �����С
		g_FontName_ABCCoord1 = "SimHei";	// ��������SimHei��
		g_FontBold_ABCCoord1    = false;		// �����Ƿ����

		g_FontSize_ABCCoord2     = 21;			// �����С
		g_FontName_ABCCoord2 = "SimHei";		// ��������SimHei��
		g_FontBold_ABCCoord2    = true;		// �����Ƿ����

		g_FontSize_ABCCoord3     = 14;			// �����С
		g_FontName_ABCCoord3 = "SimHei";	// ��������SimHei��
		g_FontBold_ABCCoord3    = true;		// �����Ƿ����

		g_FontSize_INCCoord1     = 20;			// �����С
		g_FontName_INCCoord1 = "SimHei";	// ��������SimHei��
		g_FontBold_INCCoord1    = false;		// �����Ƿ����

		g_FontSize_INCCoord2     = 21;			// �����С
		g_FontName_INCCoord2 = "SimHei";	// ��������SimHei��
		g_FontBold_INCCoord2    = true;		// �����Ƿ����

		g_FontSize_INCCoord3     = 14;			// �����С
		g_FontName_INCCoord3 = "SimHei";		// ��������SimHei��
		g_FontBold_INCCoord3    = true;		// �����Ƿ����

		g_FontSize_Tool     = 12;				// �����С
		g_FontName_Tool = "SimHei";			// ��������SimHei��
		g_FontBold_Tool    = false;			// �����Ƿ����

		g_FontSize_State     = 12;				// �����С
		g_FontName_State = "SimHei";		// ��������SimHei��
		g_FontBold_State    = false;			// �����Ƿ����

		g_FontSize_other     = 12;				// �����С
		g_FontName_other = "SimHei";		// ��������SimHei��
		g_FontBold_other    = false;			// �����Ƿ����

#else
		// 
		g_FontSize         = 13;				// �����С
		g_FontName     = "SimHei";			// ��������SimHei��
		g_FontBold        = false;				// �����Ƿ����

		g_FontSize_MainMenu     = 17;			// �����С
		g_FontName_MainMenu = "SimHei";		// ��������SimHei��
		g_FontBold_MainMenu    = false;		// �����Ƿ����

		g_FontSize_ABCCoord1     = 17;			// �����С
		g_FontName_ABCCoord1 = "SimHei";	// ��������SimHei��
		g_FontBold_ABCCoord1    = false;		// �����Ƿ����

		g_FontSize_ABCCoord2     = 19;			// �����С
		g_FontName_ABCCoord2 = "SimHei";		// ��������SimHei��
		g_FontBold_ABCCoord2    = true;		// �����Ƿ����

		g_FontSize_ABCCoord3     = 10;			// �����С
		g_FontName_ABCCoord3 = "SimHei";	// ��������SimHei��
		g_FontBold_ABCCoord3    = true;		// �����Ƿ����

		g_FontSize_INCCoord1     = 17;			// �����С
		g_FontName_INCCoord1 = "SimHei";	// ��������SimHei��
		g_FontBold_INCCoord1    = false;		// �����Ƿ����

		g_FontSize_INCCoord2     = 19;			// �����С
		g_FontName_INCCoord2 = "SimHei";	// ��������SimHei��
		g_FontBold_INCCoord2    = true;		// �����Ƿ����

		g_FontSize_INCCoord3     = 10;			// �����С
		g_FontName_INCCoord3 = "SimHei";		// ��������SimHei��
		g_FontBold_INCCoord3    = true;		// �����Ƿ����

		g_FontSize_Tool     = 11;				// �����С
		g_FontName_Tool = "SimHei";			// ��������SimHei��
		g_FontBold_Tool    = false;			// �����Ƿ����

		g_FontSize_State     = 11;				// �����С
		g_FontName_State = "SimHei";		// ��������SimHei��
		g_FontBold_State    = false;			// �����Ƿ����

		g_FontSize_other     = 11;				// �����С
		g_FontName_other = "SimHei";		// ��������SimHei��
		g_FontBold_other    = false;			// �����Ƿ����

#endif
	}

}

/*
* Description:
*	���ӹ������е�IP,STPI,STPII������ת����С����ʾ��
*	ת�������������� = nValue/8;
*			  С������ = nValue%8/10;
* Parameters:
*	int nValue: ��ת��������;
* Return Value:
*	QString:    ת������ַ���
*/
QString ShowMachiningCondition_IP(int nValue)
{
	int nInt       = nValue/8;
	float fDecimal = nValue%8/10.0;

	QString qstrRtn = QString::number(nInt + fDecimal, 'f', 1);
	return qstrRtn;

}

/*
* Description:
*	���ӹ������е�IP,STPI,STPII��С��ת����������ʾ��
*	ת������nValue = ��������*8 + С������*10;
* Parameters:
*	QString qstrValue: 
* Return Value:
*	int:    ת���������;
*/
int SetMachiningCondition_IP(QString qstrValue)
{
	if (qstrValue.isEmpty() || qstrValue == NULL)
	{
		return 0;
	}

	QFileInfo qFileInfo(qstrValue);
	QString qstrInt     = qFileInfo.baseName();
	QString qstrDecimal = qFileInfo.suffix();

	int nInt     = 0;
	int nDecimal = 0;

	if (qstrInt.isEmpty() || qstrInt.isNull())
	{
		nInt = 0;
	}
	else
	{
		nInt = qstrInt.toInt();
	}

	if (qstrDecimal.isEmpty() || qstrDecimal.isNull())
	{
		nDecimal = 0;
	}
	else
	{
		nDecimal = QString(qstrDecimal.at(0)).toInt(); 

		if (nDecimal < 0)
		{
			nDecimal = 0;
		}
		else if (nDecimal > 7)
		{
			nDecimal = 7;
		}

	}

	int nRtn = nInt*8 + nDecimal;

	return nRtn;

}

/*
* Description:
*	����QTextEdit��ĳһ�еı���ɫ
* Parameters:
*	QTextEdit *pQTextEdit:
*	int nRowNum          :
* Return Value:
*	void;
*/
void setQTextEditRowBackgroudColor(QTextEdit *pQTextEdit, int nRowNum, int nPreNum, bool bRest)
{
	if (pQTextEdit == NULL || pQTextEdit->toPlainText().isEmpty() == true || nRowNum < 0)
	{
		return;
	}

	static int s_nSliderOffsetValue = 0;

	if (bRest == true || nRowNum == 0)
	{
		int nTotal    = pQTextEdit->document()->blockCount();
		int nMin      = pQTextEdit->verticalScrollBar()->minimum();
		int nMax      = pQTextEdit->verticalScrollBar()->maximum();
		int nPageStep = pQTextEdit->verticalScrollBar()->pageStep();

		if (nTotal == 0)
		{
			s_nSliderOffsetValue = 0;
		}
		else
		{
			s_nSliderOffsetValue = (nMax - nMin + nPageStep)/nTotal; 
		}

		qWarning() <<"=======s_nSliderOffsetValue:========"<<s_nSliderOffsetValue;
		if (bRest == true)
		{
			return;
		}

	}

	// �����һ��λ��
	if (nPreNum == -1)
	{
		QTextBlockFormat filebfPre;
		filebfPre.setBackground(Qt::NoBrush);
		QTextCursor filecursorPre = pQTextEdit->textCursor();
		filecursorPre.clearSelection();
		filecursorPre.setBlockFormat(filebfPre);
	}
	else
	{
		QTextBlockFormat filebfPre;
		filebfPre.setBackground(Qt::NoBrush);
		QTextCursor filecursorPre = pQTextEdit->textCursor();
		filecursorPre.clearSelection();
		QTextBlock fileblockPre   = pQTextEdit->document()->findBlockByLineNumber(nPreNum);
		filecursorPre.setPosition(fileblockPre.position());
		filecursorPre.setBlockFormat(filebfPre);
	}

	// ˢ�±���
	QTextBlockFormat filebf;
	filebf.setBackground(Qt::cyan);
	QTextBlock fileblock   = pQTextEdit->document()->findBlockByLineNumber(nRowNum);
	QTextCursor filecursor = pQTextEdit->textCursor();
	filecursor.setPosition(fileblock.position());
	filecursor.setBlockFormat(filebf);

	pQTextEdit->setTextCursor(filecursor);

#ifdef _WIN32
	if (nRowNum > 14)
#else
	if (nRowNum > 10)
#endif
	{
		int nVal = pQTextEdit->verticalScrollBar()->value();

#ifdef _WIN32
// 		if (nPreNum < 14)
// 		{
// 			nPreNum = 14;
// 		}
#else
		if (nPreNum < 10)
		{
			nPreNum = 10;
		}
#endif

		int nAdd = abs((nRowNum - nPreNum))*s_nSliderOffsetValue; //s_nSliderOffsetValue

		nVal = nVal + nAdd;
		pQTextEdit->verticalScrollBar()->setSliderPosition(nVal);

	}

}
