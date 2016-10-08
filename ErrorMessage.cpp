#include "ErrorMessage.h"
#include "Parameter.h"
#include "QFileInfo"
#include "global_Values.h"

#ifdef CONTROLLER_TYPE_C2
	#include "mainwindow.h"
	extern MainWindow *g_gMainWindow;
#endif

ErrorMessage::ErrorMessage(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

#ifdef CONTROLLER_TYPE_C2
	if (parent != NULL)
	{
		this->setParent(parent);
	}
	else
	{
		this->setParent(g_gMainWindow);
	}
#else
	this->setParent(parent);
#endif

	this->raise();

	this->setWindowFlags(windowFlags() &~ Qt::WindowContextHelpButtonHint);			// ȥ���Ի����еİ�����ť
#ifdef CONTROLLER_TYPE_N3	
	this->setWindowFlags(Qt::CustomizeWindowHint);									// ȥ���Ի����е������С����ť
	this->setWindowFlags(Qt::WindowTitleHint);										// ȥ���Ի����еĹرհ�ť
#endif	
	//this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);			// ������ʾ

	connect(ui.btnYes,         SIGNAL(clicked()), this, SLOT(btnYesClicked()));
	connect(ui.btnNo,          SIGNAL(clicked()), this, SLOT(btnNoClicked()));
	connect(ui.btnThirdChoose, SIGNAL(clicked()), this, SLOT(btnThirdChooseClicked()));
	connect(ui.btnFouthChoose, SIGNAL(clicked()), this, SLOT(btnFouthChooseClicked()));

	// ����ֵ����
	m_nShowErrMegRtn = -1;
	
	// ��������
	QFont fontTxt(g_FontName_other, 12, g_FontBold_other);
	ui.lbDescription->setFont(fontTxt);
	QFont font(g_FontName_other, 12, g_FontBold_other);
	ui.btnYes->setFont(font);
	ui.btnNo->setFont(font);
	ui.btnThirdChoose->setFont(font);
	ui.btnFouthChoose->setFont(font);
	ui.checkBox->setFont(font);

}

ErrorMessage::~ErrorMessage()
{

}

void ErrorMessage::btnYesClicked()
{
	m_nShowErrMegRtn = 3;
	m_QEventLoop.quit();
	this->close();
}

void ErrorMessage::btnNoClicked()
{
	m_nShowErrMegRtn = 4;
	m_QEventLoop.quit();
	this->close();
}

void ErrorMessage::btnThirdChooseClicked()
{
	m_nShowErrMegRtn = 2; 
	m_QEventLoop.quit();
	this->close();
}

void ErrorMessage::btnFouthChooseClicked()
{
	m_nShowErrMegRtn = 1;
	m_QEventLoop.quit();
	this->close();
}

/*
* Description��
*	���ñ���򾯸���Ϣ
* Parameters��
*	int nErrNum      : �����
*	int nErrLevel    : ����ȼ�;0,Ĭ��ֵ�����棻1:����! 2:��ʾ; 3:��ʾCI������Ϣ; 
*	QString qstrInfo : ���������Ϣ	
* Retrun Values:
*	int:	������ȡ�Ĵ����ʩ;
*		0: btnYes;
*		1: btnNo;
*		2: btnThirdChoose;
*		3: btnFouthChoose;
*/
int ErrorMessage::showErrorMessage(int nErrNum, int nErrLevel, QString qstrInfo)			
{
	// ����⣬��ʾ����Ա���ô�����Ϣ������
	if (nErrNum <= 10000 && (nErrLevel < 0 && nErrLevel > 2))
	{
		nErrNum   = 10000;
		nErrLevel = 1;
		qstrInfo  = QString("");
	}

	sysreset();

	switch (nErrLevel)
	{
	case 0:			// ����
		{
			//ui.lbError->hide();
			ui.lbErrorNum->hide();
			ui.lbErrorValue->hide();
			QString qstrErrNum = QString::number(nErrNum);
			QString qstrErrDes = getWarningMegDes(nErrNum, qstrInfo);
			QString qstrErr    = QString(tr("����!"));
			this->setWindowTitle(qstrErr);
			ui.lbDescription->setText(qstrErrDes);

		}
		break;
	case 1:			// ����
		{
			//ui.lbError->show();
			ui.lbErrorNum->show();
			ui.lbErrorValue->show();
			QString qstrErrNum = QString::number(nErrNum);
			QString qstrErrDes = QString(tr("��������:\n")) + getErrMegDes(nErrNum, qstrInfo)+ QString("\n");
			QString qstrErrSln = QString(tr("\n����취:\n")) + getErrSolution(nErrNum, qstrInfo);// + QString("\n")
			QString qstrErr    = QString(tr("����!"));
			this->setWindowTitle(qstrErr);
			ui.lbErrorValue->setText(qstrErrNum);
			ui.lbDescription->setText(qstrErrDes + qstrErrSln);
		}
		break;
	case 2:			// ��ʾ
		{
			//ui.lbError->hide();
			ui.lbErrorNum->hide();
			ui.lbErrorValue->hide();
			QString qstrErrNum = QString::number(nErrNum);
			QString qstrErrDes = getWarningMegDes(nErrNum, qstrInfo);
			QString qstrErr    = QString(tr("��ʾ!"));
			this->setWindowTitle(qstrErr);
			ui.lbDescription->setText(qstrErrDes);

		}
		break;
	case 3:			// 3:��ʾCI������Ϣ;
		{
			ui.lbErrorNum->show();
			ui.lbErrorValue->show();
			ui.lbErrorValue->setText(QString::number(nErrNum));
			ui.btnNo->setText(tr("ȷ��"));
			ui.btnNo->show();
			QString qstrErr = QString(tr("����!"));
			this->setWindowTitle(qstrErr);

			ui.lbDescription->setText(qstrInfo);

		}
		break;
	default:
		break;
	}

	this->exec();
	//m_QEventLoop.exec();

	return m_nShowErrMegRtn;
}

/*
* Description��
*	ͨ������Ż�ȡ����������Ϣ
* Parameters��
*	int nErrNum     : �����
*	QString qstrInfo: ���������Ϣ
* Retrun Values:
*	QString    : ����������Ϣ
*/
QString ErrorMessage::getErrMegDes(int nErrNum, QString qstrInfo)
{
	QString qstrErrMeg;
	switch (nErrNum)
	{
	case 10000:
		qstrErrMeg = QString(tr("ϵͳָ��Ŀ¼��û���ҵ�O0100.NC���޷�ִ��G100����!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	// 10001��19999: ͼ�μӹ�ģ�鱨��� 
	case 10001:
		qstrErrMeg = QString(tr("�������ζ����У��ϱ�������������±���������㲻һ��!"));
		ui.btnYes->setText(tr("ѡ��3"));
		ui.btnNo->setText(tr("ȡ��"));
		ui.btnThirdChoose->setText(tr("ѡ��2"));
		ui.btnFouthChoose->setText(tr("ѡ��1"));
		ui.btnYes->show();
		ui.btnNo->show();
		ui.btnThirdChoose->show();
		ui.btnFouthChoose->show();
		break;
	case 10002:
		qstrErrMeg = QString(tr("��ǰû�п���ִ�����и�����%1��ͼ��!")).arg(QString("(") + qstrInfo + QString(tr("����)")));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10003:
		qstrErrMeg = QString(tr("��ǰͼ��û��ִ�����и��(��ģ��͹ģ������״�Լ���������)����!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10004:
		qstrErrMeg = QString(tr("��ǰͼ��ִ�е����и�����Ϊ�������Σ��������ζ����в����趨����׶��ֵ!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10005:
		qstrErrMeg = QString(tr("��ǰ�ӹ�ͼ�δ�������, �޷�ִ�����и������е�%1����!")).arg(qstrInfo);
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10006:
		qstrErrMeg = QString(tr("��ǰѡ���ļӹ�ͼ��û��������!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10007:
		qstrErrMeg = QString(tr("���п���״�������ʱ, ͼ��·�����ܴ���Բ��!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10008:
		qstrErrMeg = QString(tr("�����߷�ʽ�趨�����������β���ʱ, ���±���������߷�ʽ��ֻ��Ϊֱ�������߷�ʽ!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10009:
		qstrErrMeg = QString(tr("��ǰ������ͼ�β��Ƿ��ͼ��!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10010:
		qstrErrMeg = QString(tr("��ǰ�༭�ļӹ�ͼ��û�м����ӹ�����!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10011:
		qstrErrMeg = QString(tr("�ļ���ʧ��! ͼ���ļ���ȡʧ��!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10012:
		qstrErrMeg = QString(tr("���п���״����ʱ���ӹ�ͼ�ε��׶β���ΪԲ��!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10013:
		qstrErrMeg = QString(tr("���п���״����ʱ���ӹ�ͼ�ε�ĩ�β���ΪԲ��!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10014:
		qstrErrMeg = QString(tr("�������β���ʱ, ���±���Ҫ�ظ��������,�޷�ִ��!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;

	// DemoCadģ��///////////////////added by wang.k.h
	case G_NDEMO_CAD_ERR_NUM+1:
		qstrErrMeg = QString(tr("Բ�������뾶����Ϊ0!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+2:
		qstrErrMeg = QString(tr("Բ���յ����㲻���غ�!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+3:
		qstrErrMeg = QString(tr("�߶ε��յ�����㲻���غ�!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+4:
		qstrErrMeg = QString(tr("���εĳ��Ϳ������Ϊ0!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+5:
		qstrErrMeg = QString(tr("�����յ�̫��!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+6:
		qstrErrMeg = QString(tr("�����������!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+7:
		qstrErrMeg = QString(tr("���ͼ�β��Ƿ��ͼ��!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+8:
		qstrErrMeg = QString(tr("���޼�ʵ��Ϊ����ʵ�壬����ִ�вü�����!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+9:
		qstrErrMeg = QString(tr("��ʵ���޽��㣬����ִ�вü�����!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+10:
		qstrErrMeg = QString(tr("���ͼ��������!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+11:
		qstrErrMeg = QString(tr("Բ�������߷�ʽ�У�Բ���뾶���ù���"));
		ui.btnYes->setText(tr("ѡ��2"));
		ui.btnNo->setText(tr("ȡ��"));
		ui.btnThirdChoose->setText(tr("ѡ��1"));
		ui.btnYes->show();
		ui.btnNo->show();
		ui.btnThirdChoose->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+12:
		qstrErrMeg = QString(tr("��ͼ���޷�����PVT�ļ���"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+13:
		qstrErrMeg = QString(tr("δ�趨���ɵ�PVT�ļ����������ļ�ʧ��!"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+14:
		qstrErrMeg = QString(tr("���ͼ���⻹������ͼ�Σ�"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+15:
		qstrErrMeg = QString(tr("ͼ���д��ڰ��㣡"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+16:
		qstrErrMeg = QString(tr("��֧�ִ��ּӹ��������ϣ�"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM+17:
		qstrErrMeg = QString(tr("�ӹ�������ϴ���"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	case G_NDEMO_CAD_ERR_NUM + 18:
		qstrErrMeg = QString(tr("û�����ߣ��޷���ȡģ�ͽ����㣡"));
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	// 20001��29999: ��λģ�鱨���
	case 20001:
		qstrErrMeg = QString(tr(""));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;

	// 30001��39999: MDIģ�鱨���
	case 30001:
// 		qstrErrMeg = QString(tr("��ǰ�ӹ�ģʽ����\"MDI\"ģʽ! ��MDIģ���з�\"MDI\"�ӹ�ģʽ����ִ�мӹ�����!"));
// 		//ui.btnYes->setText(tr("ִ��"));
// 		ui.btnNo->setText(tr("ȷ��"));
// 		//ui.btnYes->show();
// 		ui.btnNo->show();
		break;

	// 40001��49999: �༭ģ�鱨���
	case 40001:
		qstrErrMeg = QString(tr(""));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;

	// 50001��59999: ����ӹ�ģ�鱨���
	case 50001:
		qstrErrMeg = QString(tr("\"��˿�ָ�\"��������ʧ��!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
		break;

	// 60001��69999: �����趨ģ�鱨���
	case 60001:
		qstrErrMeg = QString(tr(""));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 60002:
		qstrErrMeg = qstrInfo;
		ui.btnNo->setText(tr("ȷ��"));
		ui.btnNo->show();
		break;
	// 70001��79999: ����ģ�鱨���
	case 70001:
		qstrErrMeg = QString(tr(""));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;

	// 80001��89999: ϵͳ���������
	case 80001:
		qstrErrMeg = QString(tr("����\"MDI\"ģʽʧ��!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 80002:
		qstrErrMeg = QString(tr("����\"����ӹ�\"ģʽʧ��!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 80003:
		qstrErrMeg = QString(tr("����\"����\"ģʽʧ��!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 80004:
		qstrErrMeg = QString(tr("��ǰģʽ����\"")) + qstrInfo + QString(tr("\"!"))
			+ qstrInfo + QString(tr("\"����ִ�мӹ�����!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 80005:
		qstrErrMeg = QString(tr("����\"CODELESS\"ģʽʧ��!"));
		//ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȷ��"));
		//ui.btnYes->show();
		ui.btnNo->show();
		break;
	default:
		break;
	}

	return qstrErrMeg;
}

/*
* Description��
*	ͨ������Ż�ȡ�������취
* Parameters��
*	int nErrNum     : �����
*	QString qstrInfo: ���������Ϣ	
* Retrun Values:
*	*	QString: �������취
*/
QString ErrorMessage::getErrSolution(int nErrNum, QString qstrInfo)
{
	QString qstrErrSolution;
	switch (nErrNum)
	{
	case 10000:
		qstrErrSolution = QString(tr("����ϵ���̻�ȡ��ȷ��MOTION����O0100.NC�������û�Motion�ļ�����!\n"));
		break;
	case 10002:
		qstrErrSolution = QString(tr("�밴������ʾ���±༭ͼ��:\n")) + 
			QString(tr("1: ѡ��[�ļ�]���ܣ��򿪻��ƺõ�DXF�ļ�����ִ�д˲���!\n")) + 
			QString(tr("2: ѡ��[ͼ�α༭]�����е�ͼ�α༭���ߣ��༭��ͼ�κ���ִ�д˲���!\n"));
		break;
	case 10003:
		qstrErrSolution = QString(tr("����ִ�����и��(��ģ��͹ģ������״�Լ���������)��������ִ�д˲���!\n"));
		break;
	case 10004:
		qstrErrSolution = QString(tr("���޸����и�����Ϊ: ��ģ��͹ģ����״����ִ�д˲���!\n"));
		break;
	case 10005:
		qstrErrSolution = QString(tr("�밴������ʾ���±༭ͼ��:\n")) + 
			QString(tr("1: ͼ�α���Ϊ���ͼ��!\n")) + 
			QString(tr("2: ���ͼ�α����ǵ�һ��(����ѡ����ͼ�β��ܴ��ڶ������·��)!\n"));
		break;
	case 10006:
		qstrErrSolution = QString(tr("��ѡ����������������߷�ʽ, ���ú����ߺ���ִ�б�����!"));
		break;
	case 10007:
		qstrErrSolution = QString(tr("ȷ��ִ�п���״��������е�ͼ���в�����Բ����������!\n"));
		break;
	case 10008:
		qstrErrSolution = QString(tr("�������������±���������߷�ʽΪֱ�������߷�ʽ!\n"));
		break;
	case 10009:
		qstrErrSolution = QString(tr("ѡ��[ͼ�α༭]�����е�ͼ�α༭���ߣ��༭��ͼ��Ϊ������ķ��ͼ�κ���ִ�д˲���!\n"));
		break;
	case 10010:
		qstrErrSolution = QString(tr("����[���и��]������ִ�а�ģ(��͹ģ,����״����������)����ʱ���ڵ�����\"��������\"�Ի����У�"))
			+ QString(tr("��[�������]��������Ϊ\"��\", �����ӹ�������ɺ���ִ�б�����!\n"));
		break;
	case 10011:
		qstrErrSolution = QString(tr("ѡ��򿪵�ͼ���ļ�����! ���������ͼ���ļ�(��ʹ��[ͼ�α༭]�������±༭ͼ���ļ�)!\n")); 
		break;
	case 10012:
		qstrErrSolution = QString(tr("ѡ��[ͼ�α༭]�����е�ͼ�α༭���ߣ�ȥ���ӹ�ͼ����ΪԲ�����׶κ���ִ�д˲���!\n"));
		break;
	case 10013:
		qstrErrSolution = QString(tr("ѡ��[ͼ�α༭]�����е�ͼ�α༭���ߣ�ȥ���ӹ�ͼ����ΪԲ����ĩ�κ���ִ�д˲���!\n"));
		break;
	case 10014:
		qstrErrSolution = QString(tr("ѡ��[ͼ�α༭]�����е�ͼ�α༭���ߣ��޸�ͼ�Σ�ʹ�����±���Ҫ�ظ�����Ⱥ�,��ִ�д˲���!\n"));
		break;

		// DemoCadģ��////////////////////////added by wang.k.h
	case G_NDEMO_CAD_ERR_NUM+1:
		qstrErrSolution = QString(tr("������ȷ���뾶!\n"));
		break;
	case G_NDEMO_CAD_ERR_NUM+2:
		qstrErrSolution = QString(tr("������ȷ���յ�!\n"));
		break;
	case G_NDEMO_CAD_ERR_NUM+3:
		qstrErrSolution = QString(tr("������ȷ���յ�!\n"));
		break;
	case G_NDEMO_CAD_ERR_NUM+4:
		qstrErrSolution = QString(tr("������ȷ���յ�(����X�����Y���궼�������)!\n"));
		break;
	case G_NDEMO_CAD_ERR_NUM+5:
		qstrErrSolution = QString(tr("������ȷ�����������յ�!\n"));
		break;
	case G_NDEMO_CAD_ERR_NUM+6:
		break;
	case G_NDEMO_CAD_ERR_NUM+7:
		qstrErrSolution = QString(tr("�����޸�ͼ�Σ��ٽ��иò���!\n"));
		break;
	case G_NDEMO_CAD_ERR_NUM+8:
		qstrErrSolution = QString(tr("������ѡ��ʵ�壬�ٽ��вü�����!\n"));
		break;
	case G_NDEMO_CAD_ERR_NUM+9:
		qstrErrSolution = QString(tr("������ѡ��ʵ�壬�ٽ��вü�����!\n"));
		break;
	case G_NDEMO_CAD_ERR_NUM+10:
		qstrErrSolution = QString(tr("�밴������ʾ���±༭ͼ��:\n")) + 
			QString(tr("1: ��ȷ��ͼ��·��ֻ��ȷ��Ψһ��ͼ��!\n")) + 
			QString(tr("2: ��ȷ��\"NC��������\"�еĲ���\"����������\"�Ƿ�����ǡ��!\n"));
		break;
	case G_NDEMO_CAD_ERR_NUM+11:
		qstrErrSolution = QString(tr(" 1.��������Բ���뾶�������뾶��СһЩ!\n")) +
			QString(tr(" 2.����ѡ����㣬ʹ���������յ�����λ�õľ�����!\n"));
		break;
	case G_NDEMO_CAD_ERR_NUM+12:
		qstrErrSolution = QString(tr("�����»���ͼ�λ��ȡDXF�ļ����ٽ�������PVT����!\n"));
		break;
	case G_NDEMO_CAD_ERR_NUM+16:
		qstrErrSolution = QString(tr("֧�ֵļ��ּӹ�������ϣ�\n")) + 
			QString(tr("1: �����ӹ�����\n")) + 
			QString(tr("2: ��ĥ+��ĥ ���� ��ĥ+��ĥ\n")) + 
			QString(tr("3: ���в����ܺ�"));
		break;
	case G_NDEMO_CAD_ERR_NUM+17:
		qstrErrSolution = QString(tr("��ĥǰ�Զ�����ɰ�ֹ���ֻ�������ӹ���֧�֣�����Ҫ�˹��ܣ���ѡ���мӹ����裻��ȡ�����ܹ�ѡ��\n"));
		break;
	// 30001��39999: MDIģ�鱨���
	case 30001:
		//qstrErrSolution = QString(tr("����ԭ��:ʹ���ֿغл������мӹ�����ʱ, �ӹ�ģʽû������Ϊ\"MDI\"ģʽ!\n")) + 
			//QString(tr("����취: �����õ�ǰ�ӹ�ģʽΪ\"MDI\"ģʽ��,��ִ��MDI�ӹ�����!\n"));
		break;

	// 80001��89999: ϵͳ���������
	case 80001:
		qstrErrSolution = QString(tr("���ȡ���°취���в���:\n")) + 
			QString(tr("1: ���ٴΰ���\"ִ��\"���ܰ�ť!\n")) + 
			QString(tr("2: ������1���޷����������ִ��ϵͳ\"��λ\"����!\n")) +
			QString(tr("3: �����ϲ������޷��������!������ϵͳ!\n"));
		break;
	case 80002:
		qstrErrSolution = QString(tr("���ȡ���°취���в���:\n")) + 
			QString(tr("1: ���ٴΰ���\"�ӹ�\"���ܰ�ť!\n")) + 
			QString(tr("2: ������1���޷����������ִ��ϵͳ\"��λ\"����!\n")) +
			QString(tr("3: �����ϲ������޷��������!������ϵͳ!\n"));
		break;
	case 80003:
		qstrErrSolution = QString(tr("���ȡ���°취���в���:\n")) + 
			QString(tr("1: ���ٴΰ���\"�ӹ�\"���ܰ�ť!\n")) + 
			QString(tr("2: ������1���޷����������ִ��ϵͳ\"��λ\"����!\n")) +
			QString(tr("3: �����ϲ������޷��������!������ϵͳ!\n"));
		break;
	case 80004:
		qstrErrSolution = QString(tr("����ԭ��:ʹ���ֿغл������мӹ�����ʱ, �ӹ�ģʽû������Ϊ\""))
			+ qstrInfo + QString(tr("\"ģʽ!\n"))
			+ QString(tr("����취: �뽫��ǰģʽ����Ϊ\""))
			+ qstrInfo
			+ QString(tr("\"��,��ִ�мӹ�����!\n"));
		break;
	case 80005:
		qstrErrSolution = QString(tr("���ȡ���°취���в���:\n")) + 
			QString(tr("1: ���ٴΰ���\"ִ��\"���ܰ�ť!\n")) + 
			QString(tr("2: ������1���޷����������ִ��ϵͳ\"��λ\"����!\n")) +
			QString(tr("3: �����ϲ������޷��������!������ϵͳ!\n"));
		break;
	default:
		break;
	}

	return qstrErrSolution;
}

/*
* Description��
*	ͨ������Ż�ȡ����������Ϣ
* Parameters��
*	int nErrNum     : �����
*	QString qstrInfo: ���������Ϣ	
* Retrun Values:
*	QString    : ����������Ϣ
*/
QString ErrorMessage::getWarningMegDes(int nErrNum, QString qstrInfo)
{
	QString qstrErrMeg;
	switch (nErrNum)
	{
	case 10001:
		qstrErrMeg = QString(tr("��ǰѡ�е�ͼ���Ѿ�����������ζ���!ִ�б��β�����ȡ���������ζ��壬�Ƿ�ִ�б��β���?"));
		ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȡ��"));
		ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10002:
		qstrErrMeg = QString(tr("��ǰѡ�е�ͼ���Ѿ���ɿ���״����!ִ�б��β�����ȡ������״���壬�Ƿ�ִ�б��β���?"));
		ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȡ��"));
		ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10003:
		qstrErrMeg = QString(tr("��ǰѡ�е�ͼ���Ѿ����͹ģ����!ִ�б��β�����ȡ��͹ģ���壬�Ƿ�ִ�б��β���?"));
		ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȡ��"));
		ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10004:
		qstrErrMeg = QString(tr("��ǰѡ�е�ͼ���Ѿ���ɰ�ģ����!ִ�б��β�����ȡ����ģ���壬�Ƿ�ִ�б��β���?"));
		ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȡ��"));
		ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10005:
		qstrErrMeg = QString(tr("��ǰѡ�е�ͼ���Ѿ���ɰ�ģ����!ִ�б��β�����ȡ����ģ���壬�Ƿ�ִ�б��β���?"));
		ui.btnYes->setText(tr("ִ��"));
		ui.btnNo->setText(tr("ȡ��"));
		ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 10006:			// �ļ���������ǰ�Ƿ񱣴棿
		{
			QFileInfo qFInfo(qstrInfo);
			QString qbaseName = qFInfo.baseName();
			qstrErrMeg = QString(tr("�Ƿ񽫸��ı��浽�ļ�\"")) + qbaseName + QString(tr(".DXF\"��?"));
			ui.btnThirdChoose->setText(tr("��"));
			ui.btnYes->setText(tr("��"));
			ui.btnNo->setText(tr("ȡ��"));
			ui.btnThirdChoose->show();
			ui.btnYes->show();
			ui.btnNo->show();
		}
		break;
	case 10007:			// �ļ���������ǰ�Ƿ񱣴棿
		{
			qstrErrMeg = QString(tr("��ǰ�ļ��Ѹ��ģ��Ƿ񽫸��ı���?"));
			ui.btnThirdChoose->setText(tr("��"));
			ui.btnYes->setText(tr("��"));
			ui.btnNo->setText(tr("ȡ��"));
			ui.btnThirdChoose->show();
			ui.btnYes->show();
			ui.btnNo->show();
		}
		break;
	case 10008:			// 
		{
			qstrErrMeg = QString(tr("�ļ�����ɹ�,���\"ȷ��\"��ִ��\"��\"�ļ�����!"));
			ui.btnNo->setText(tr("ȷ��"));
			ui.btnNo->show();
		}
		break;
	case 10009:			// 
		{
			qstrErrMeg = QString(tr("��ע�⵱ǰ�򿪵��ļ�Ϊ��!"));
			ui.btnNo->setText(tr("ȷ��"));
			ui.btnNo->show();
		}
		break;

	//ͼ�α༭ģ��////////////////added by wang.k.h
	case G_NDEMO_CAD_ERR_NUM+1:
		{
			qstrErrMeg = QString(tr("��ȷ��Բ���������յ�!"));
			ui.btnNo->setText(tr("ȷ��"));
			ui.btnNo->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+2:
		{
			qstrErrMeg = QString(tr("��������������������!"));
			ui.btnNo->setText(tr("ȷ��"));
			ui.btnNo->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+3:
		{
			qstrErrMeg = QString(tr("���ͼ�β��Ƿ��ͼ�λ�������!"));
			ui.btnNo->setText(tr("ȷ��"));
			ui.btnNo->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+4:
		{
			qstrErrMeg = QString(tr("��ǰͼ���Ѿ��������ߣ��Ƿ��滻ԭ������?"));
			ui.btnYes->setText(tr("��"));
			ui.btnNo->setText(tr("��"));
			ui.btnNo->show();
			ui.btnYes->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+5:
		{
			qstrErrMeg = QString(tr("�������ȷ�������յ����ڵ�ֱ�ߡ�Բ��Բ��!"));
			ui.btnNo->setText(tr("ȷ��"));
			ui.btnNo->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+6:
		{
			qstrErrMeg = QString(tr("ɾ��ʵ��ʱ�����ϵ�����Ҳ�ᱻɾ��!"));
			ui.btnNo->setText(tr("ȷ��"));
			ui.btnNo->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+7:
		{
			qstrErrMeg = QString(tr("����ѡ������!"));
			ui.btnNo->setText(tr("ȷ��"));
			ui.btnNo->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+8:
		{
			qstrErrMeg = QString(tr("�������ߵ�ʵ�����ƽ�ƣ���ת�����Ż������ʱ�����߻ᱻɾ��!�Ƿ��������?"));
			ui.btnYes->setText(tr("��"));
			ui.btnNo->setText(tr("��"));
			ui.btnNo->show();
			ui.btnYes->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+9:
		{
			qstrErrMeg = QString(tr("!"));
			ui.btnNo->setText(tr("ȷ��"));
			ui.btnNo->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+10:
		{
		    qstrErrMeg = QString(tr("ͼ�����нӵ㲻���е�������Ƿ����?"));
			ui.btnYes->setText(tr("��"));
			ui.btnNo->setText(tr("��"));
			ui.btnNo->show();
			ui.btnYes->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+11:
		{
			qstrErrMeg = QString(tr("��ͼ��û�����ߣ����Ȼ������ߣ�"));
			ui.btnYes->setText(tr("ȷ��"));
			ui.btnYes->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+12:
		{
			qstrErrMeg = QString(tr("δѡ���κβ��裬�����������ļ���"));
			ui.btnYes->setText(tr("ȷ��"));
			ui.btnYes->show();
		}
		break;
	case G_NDEMO_CAD_ERR_NUM+13:
		{
			qstrErrMeg = qstrInfo;
			ui.btnYes->setText(tr("ȷ��"));
			ui.btnYes->show();
		}
		break;
	// 20001��29999: ��λģ��
	case 20001:
		break;

	// 30001��39999: MDIģ��
	case 30001:
		break;

	// 40001��49999: �༭ģ��
	case 40001:
		qstrErrMeg = QString(tr("�����豸���Ѱ���ͬ���ļ�: %1. �Ƿ��滻���ļ�?").arg(qstrInfo));
		ui.btnYes->setText(tr("����"));
		ui.btnNo->setText(tr("ȡ��"));
		ui.btnThirdChoose->setText(tr("�滻"));
		ui.btnYes->show();
		ui.btnNo->show();
		ui.btnThirdChoose->show();
		break;

	// 50001��59999: ����ӹ�ģ��
	case 50001:
		qstrErrMeg = QString(tr("��ǰ�Ѿ����ڴ򿪵��ļ����Ƿ����ִ��\"���ļ�\"����?(ע, ����ִ��\"���ļ�\"�������滻ԭ���ļ�)"));
		ui.btnYes->setText(tr("��"));
		ui.btnNo->setText(tr("��"));
		ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 50002:
		qstrErrMeg = QString(tr("�Ƿ�ȷ��ִ�ж�˿�ָ�����?"));
		ui.btnYes->setText(tr("��"));
		ui.btnNo->setText(tr("��"));
		ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 50003:
		qstrErrMeg = QString(tr("�µ缫˿��˿�����Ƿ��Ѿ����?(ע, ����밴��ʵ�����ѡ�����, ���򽫿�����ɶ�˿!)"));
		ui.btnYes->setText(tr("��"));
		ui.btnNo->setText(tr("��"));
		ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 50004:
		qstrErrMeg = QString(tr("�µ缫˿�Ƿ��ѻص���˿��λ��?(ע, ����밴��ʵ�����ѡ�����, ���򽫿�����ɶ�˿!)"));
		ui.btnYes->setText(tr("��"));
		ui.btnNo->setText(tr("��"));
		ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 50005:
		qstrErrMeg = QString(tr("\"��˿�ָ�\"�����Ƿ���ɣ����\"��\"�󽫴Ӷ�˿λ�ÿ�ʼ�������мӹ�����!"));
		ui.btnYes->setText(tr("��"));
		ui.btnNo->setText(tr("��"));
		ui.btnYes->show();
		ui.btnNo->show();
		break;
	case 50006:
		break;
	// 60001��69999: �����趨ģ��
	case 60001:
		break;

	// 70001��79999: ����ģ�鱨
	case 70001:
		break;

	// 80001��89999: ϵͳ����
	case 80001:
		break;

	default:
		break;
	}

	return qstrErrMeg;
}

void ErrorMessage::sysreset()
{
	QString qstrErrNum = QString("");
	QString qstrErrDes = QString("");
	QString qstrErrSln = QString("");
	QString qstrErr    = QString("");

	this->setWindowTitle(qstrErr);
	ui.lbErrorValue->setText(qstrErrNum);
	ui.lbDescription->setText(qstrErrDes);
	//ui.lbSolution->setText(qstrErrSln);
	//ui.lbDescription->setTextColor(QColor(255, 0, 0));

	ui.btnYes->hide();
	ui.btnNo->hide();
	ui.btnThirdChoose->hide();
	ui.btnFouthChoose->hide();
	ui.checkBox->hide();

}