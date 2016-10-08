#ifndef PARASETWDT_H
#define PARASETWDT_H

#include <QDockWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QRadioButton>
#include "curvefit.h"
#include "RS_CustomBtn.h"
#include "Parameter.h"
#include "CIComm.h"
#include "detresultdlg.h"


class ParaSetWdt : public QDockWidget
{
	Q_OBJECT

public:

	enum DET_OPETATION
	{
		DEMARCATE,
		MEASURE,
		CALCULATE,
		DET_OP_COUNT
	};

	enum offFlag{offX, offY, offZ, offsetCount};


	ParaSetWdt(QWidget *parent);
	~ParaSetWdt();

	bool isInputLegel( int inputmode = ui_input );

signals:
	void sglCountChanged();
	void sglSetMsg( QString msg );

public slots:
	void slotCalAndSave();
	void slotRadSetFinish();
	void slotConfig();
	void slotInOrOut();

protected:
	void resizeEvent( QResizeEvent * event );
	double getOffOfAxis( offFlag );

private:
	QWidget * m_TitleWdt;  //< �յı���ؼ�
	QLabel  * m_HelpLabel;  

	QGroupBox * m_DetGrpBox[DET_OP_COUNT];  //< ���������groupbox
	QLabel  * m_DetHlpLabel[DET_OP_COUNT];  //< ÿ���������涼����Ӧ����ʾ�Ͱ���
	QGridLayout * m_Layout[DET_OP_COUNT];   //< groupbox��Ӧ�Ĳ���

	QLabel * m_DemSharpLbl;    //< �궨ģ��Ŀؼ�
	QLabel * m_RadLbl;
	QLabel * m_DetDirLbl;      //< ̽�ⷽ��
    QLineEdit * m_DetRadLEdit;
	QRadioButton * m_DetDirRadioBtn[2];

	QLabel * m_MeaAxisLbl[5];  //< ̽��ģ��Ŀؼ�
	QLabel * m_MeaSymbalLbl[5];
	QLineEdit * m_MeaLEdit[MEA_MACRO_COUNT];
	
	QLabel * m_CalOffTitleLbl; //< ����ģ��Ŀؼ�
	QLabel * m_CalRotTitleLbl;
	QLabel * m_CalOffData[offsetCount];
	QLabel * m_CalRotData;
	RS_CustomBtn * m_CalBtn;

	DetResultDlg * m_DetPointDlg;
};

#endif // PARASETWDT_H
