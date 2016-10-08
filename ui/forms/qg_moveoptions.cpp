#include "qg_moveoptions.h"
#include "ui_qg_moveoptions.h"
#include "PublicFuction.h"


QG_MoveOptions::QG_MoveOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QG_MoveOptions)
{
    ui->setupUi(this);

    ui->lineEdit_xValue->setValidator(setInputDataFormat_Digit(this));
	ui->lineEdit_yValue->setValidator(setInputDataFormat_Digit(this));

	QFont font("SimHei", 12, false);//
	this->setFont(font);
	ui->lbInfo->setFont(font);
	ui->radioButton_manual->setFont(font);
	ui->radioButton_fixed->setFont(font);
	ui->pushButton_ok->setFont(font);
	ui->label->setFont(font);
	ui->label_2->setFont(font);
	ui->lineEdit_xValue->setFont(font);
	ui->lineEdit_yValue->setFont(font);
	
	ui->radioButton_manual->setChecked(false);
	ui->radioButton_fixed->setChecked(false);

}

QG_MoveOptions::~QG_MoveOptions()
{
    delete ui;
}


//��������ṹ������ͷ��¼���������ʱaciton�滻�ȣ��������updateOption�Ĵ���
//������setAction�Ĵ��������Բ��������иı�action�����ԣ��������
void QG_MoveOptions::setAction( RS_ActionInterface * a, bool update ) 
{
    if (a!=NULL && a->rtti()==RS2::ActionModifyMove) 
	{
        action = (RS_ActionModifyMove*)a;

        //���ݱ�־���ж��Ƿ���Ҫ����action״̬���¿ؼ���ѡ��״̬
        if (update) 
		{
            //����action��ǰ״̬����ȡ��Ϣ
            //��������ṹ��setAction�ᾭ���ı䣬������½��治��Ӱ�쵽action������
            //��Ϊ�ı�ѡ��Ҫ��λaction״̬������Ҫȷ��action���˵����ť��ɣ������������������ţ�����������
            setCurSelect(action->getCurMoveType());
        } 
		else 
		{
            //Ĭ����Ϣ
            setCurSelect(Manul);
        }

    }
	else 
	{
        RS_DEBUG->print(RS_Debug::D_ERROR, "QG_MoveOptions::setAction: wrong action type");
        action = NULL;
    }
}

int QG_MoveOptions::getCurSelect() 
{
    if (ui->radioButton_manual->isChecked()) 
	{
        return Manul;
    } 
	else if (ui->radioButton_fixed->isChecked()) 
	{
        return Fixed;
    }
	else 
	{
        return Manul;
    }
}

void QG_MoveOptions::setCurSelect(int select) 
{
    switch (select)
    {
    case Manul:
        ui->radioButton_manual->setChecked(true);//�ǲ�����click�źŵ�
        break;
    case Fixed:
        ui->radioButton_fixed->setChecked(true);
        break;
    default:
        break;
    }

    changeShow();

}

void QG_MoveOptions::changeShow() 
{
    curSelect = getCurSelect();

    switch (curSelect)
    {
    case Manul:
        ui->lineEdit_xValue->setEnabled(false);
        ui->lineEdit_yValue->setEnabled(false);
        ui->pushButton_ok->setEnabled(false);
        break;
    case Fixed:
        ui->lineEdit_xValue->setEnabled(true);
        ui->lineEdit_yValue->setEnabled(true);
        ui->pushButton_ok->setEnabled(true);
        break;
    default:
        break;
    }

}


void QG_MoveOptions::changeSelect() 
{
    changeShow();
    action->setCurMoveType(curSelect);
}

void QG_MoveOptions::moveFixed() 
{
    //ȷ����ȷ��������ʽ����
    double x = ui->lineEdit_xValue->text().toDouble();
    double y = ui->lineEdit_yValue->text().toDouble();

    action->moveFixed(x, y);
}

void QG_MoveOptions::updateDataShow() 
{
    if (action != NULL) 
	{

    }
}
