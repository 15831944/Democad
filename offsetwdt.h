#ifndef OFFESTWDT_H
#define OFFESTWDT_H

#include <QDockWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QGroupBox>
#include <QLayout>
#include <qdom.h>
#include "RS_CustomBtn.h"
#include "Parameter.h"
#include "global_Values.h"
#include "rs_entitycontainer.h"
#include "CIComm.h"
#include <iostream>
#include <QEvent>
#include "showfinalncdlg.h"

#ifdef Q_OS_LINUX
#include <sys/time.h>
#endif

using std::cout;
using std::endl;

class OffsetWdt : public QDockWidget
{
	Q_OBJECT

public:

	enum OFF_GRP
	{
		RM_OFFSET,
		READ_DXF,
		MANUAL,
		NEW_NC,
		OFF_GRP_COUNT
	};

	struct entityWithDir
	{
		XYZ_DIR dir;
		RS_Entity * entity;
	};

	enum FILE_OP
	{
		FILE_SRC,
		FILE_NEW,
		FILE_COUNT
	};

	enum INSERT_FLAG
	{
		NOT_INSERT_PNT,
		INSERT_PNT
	};

	enum ZOOM_DIR
	{
		ZOOM_LEFT,
		ZOOM_RIGHT,
		ZOOM_TOP,
		ZOOM_BTM,
		ZOOM_DIR_COUNT
	};

	OffsetWdt(QWidget *parent);
	~OffsetWdt();

	void openSrcNcFile( QString filename );
	bool isIndexLegal( int axisIndex, int inputmd = ui_input );
	void rmOffsetXY( double xoff, double yoff, double angleoff );
	void rmOffsetZ( double zoff );
	void genNewNC( QString filename, int inputmd = ui_input );

signals:
	void sglConfigRdy();
	void sglSetMsg( QString msg );

public slots:
	void slotBrwSrcFile();
	void slotGenNewNC();
    void slotRemoveOffsetXY();
	void slotRemoveOffsetZ();
	void slotSetDXFFile( QString & filename );
	void slotGetNewContainer(RS_EntityContainer *);
	void slotCountChanged();
	void slotFormatManualOff();

protected:
	void resizeEvent( QResizeEvent * event );
    void showEvent( QShowEvent * event );

	void getSrcPoints( QString filename );
	bool getPntsFromNC( char *strFileName, char *strOutPutFile, QString chInput, int lTotalLines );
	char * str_srch2(char* charString, char* charSub1, char* charSub2 );
	bool CheckG0123(char* charString, char char0123);

	int findNearestNCLineXY( RS_Vector & vec, int & insertFlag, double maxDist = 0.1 );
	int findNearestNCLineZ( RS_Vector & vec, int & insertFlag,  double maxDist = 0.1 );
	int isDxfToNC();
	int getSrcNCOffDir( /*RS_Vector & vec, XYZ_DIR dir*/ );
	bool isRdyToGen();
	void ncHeadAndTailMod( QString & qstrline, double x = 0., double y = 0. );
	void newNCOutput( QString filename, RS_Vector headOff, RS_Vector tailoff );

	void calculataXYOff( RS_Vector * dataWthoutR, int * dataDir
		, RS_Vector * finalOffsetXY, int size, int inputmd = ui_input);
	void calculataZOff( RS_Vector * finalOffsetZ, int size);
	void calculateHeadAndTailOff( RS_Vector * offset, int size, RS_Vector & headOff, RS_Vector & tailOff );
	void showGenNCSteps();

	void saveConfig();
	void readConfig();
	void clearIndex();

	XYZ_DIR getDetDir( int pointIndex );

private:
	QWidget   * m_TitleWdt;
	QLabel    * m_HelpLabel; 
	QGroupBox * m_GrpBox[OFF_GRP_COUNT];
	QGridLayout * m_Layout[OFF_GRP_COUNT];

	QLabel * m_RmHlpLbl;  //< �Ƴ�ƫ����ģ��Ŀؼ�
	QLabel * m_RmAxisLbl[RM_Axis_Count];
	QLabel * m_RmPntCntLbl[RM_Axis_Count];
	QLineEdit * m_RmMacroLEdit[RM_Axis_Count];
	RS_CustomBtn * m_RmBtn[RM_Axis_Count];

	QLabel * m_ReadHlpLbl;              //< ��ȡDXF�ļ�ģ��Ŀؼ�
	QLabel * m_ReadFnameLbl;

	QLabel * m_ManHlpLbl;               //< �ֶ�����ģ��Ŀؼ�
	QLabel * m_ManTipLbl;
	QLabel * m_ManZoomLbl;
	QLineEdit * m_ManOffLEdit[3];       //< x,y,z�Ჹ��ֵ���
	QLineEdit * m_ManOffZoom[ZOOM_DIR_COUNT];  //< x,y������

	bool m_bHasRmXY;
	bool m_bHasRmZ;

	QLabel * m_NewHlpLbl;
	QLabel * m_NewFTtlLbl[FILE_COUNT];
	QLabel * m_NewFNameLbl[FILE_COUNT];
	RS_CustomBtn * m_NewBtn[FILE_COUNT];
	QString  m_SrcNCFile, m_NewNCFile;  //< ԭNC�ļ�������NC�ļ���
	QString  m_dxfFilename;

	QString  m_SrcContent;              //< ԭNC�ļ�����

	RS_Vector *   m_NCPoints;        //< ��λ��Ϣ���ڴ������ܻ����һЩ�㣩
	long          m_lNCLineCount, m_lPointCount;
	int           m_nSrcNCPntFirstLine, m_nSrcNCPntLastLine;
	QMap<int,int> m_lineNumToDataIndex;

	RS_EntityContainer * container;    //< �����ʵ������
	double m_dNCFileROff;			   //< �����NC�ļ����еľ�����
	double m_dNCFileFspd;              //< �����NC�ļ���Ľ����ٶ�

	int * m_ncLineNumXY;  //< �������NC�к�
	int * m_ncLineNumZ;   //< Z�Ჹ�����NC�к�

	//< ***************����˵��ж��йصĲ��ֱ���*************************
	int  m_nCutFirstPointIndex, m_nCutLastPointIndex;  //< ���˵���ı��
	long m_lActFstOffPnt, m_lActLstOffPnt;  //< ʵ���ϼӲ����ĵ�һ��������һ���㣬��Ϊ���˵��ο��ܻ����غϵĲ��֣����Կ��ܽ��˵��㲢��һ����ʵ�ʵĲ�������ʼ���߽����ĵ�
	bool m_bIsClose;  //< �Ƿ��Ƿ�յ�ͼ��(����̽�����жϣ����ܸ���DXF���ж�)

	ShowFinalNcDlg * m_NewNCPntDlg;
};

#endif // OFFESTWDT_H
