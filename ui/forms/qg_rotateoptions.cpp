#include "qg_rotateoptions.h"
#include "ui_qg_rotateoptions.h"

QG_RotateOptions::QG_RotateOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QG_RotateOptions)
{
    ui->setupUi(this);

	QFont font("SimHei", 12, false);//
	this->setFont(font);
	ui->lbInfo->setFont(font);
	ui->radioButton_manual->setFont(font);
	ui->radioButton_fixed->setFont(font);
	ui->pushButton_ok->setFont(font);
	ui->label_2->setFont(font);
	ui->label_3->setFont(font);
	ui->lineEdit_xValue->setFont(font);
	ui->lineEdit_yValue->setFont(font);
	ui->label_4->setFont(font);
	ui->lineEdit_angle->setFont(font);
}

QG_RotateOptions::~QG_RotateOptions()
{
    delete ui;
}


void QG_RotateOptions::setAction( RS_ActionInterface * a, bool update ) {
    if (a!=NULL && a->rtti()==RS2::ActionModifyRotate) {
        action = (RS_ActionModifyRotate*)a;

        //���ݱ�־���ж��Ƿ���Ҫ����action״̬���¿ؼ���ѡ��״̬
        if (update) {
            //����action��ǰ״̬����ȡ��Ϣ
            //��������ṹ��setAction�ᾭ���ı䣬������½��治��Ӱ�쵽action������
            //��Ϊ�ı�ѡ��Ҫ��λaction״̬������Ҫȷ��action���˵����ť��ɣ������������������ţ�����������
            setCurSelect(action->getCurRotateType());
        } else {
            //Ĭ����Ϣ
            setCurSelect(Manul);
        }


    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, "QG_MoveOptions::setAction: wrong action type");
        action = NULL;
    }
}

int QG_RotateOptions::getCurSelect() {
    if (ui->radioButton_manual->isChecked()) {
        return Manul;
    } else if (ui->radioButton_fixed->isChecked()) {
        return Fixed;
    } else {
        return Manul;
    }
}

void QG_RotateOptions::setCurSelect(int select) {
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

void QG_RotateOptions::changeShow() {
    curSelect =getCurSelect();
    switch (curSelect)
    {
    case Manul:
        ui->lineEdit_xValue->setEnabled(false);
        ui->lineEdit_yValue->setEnabled(false);
        ui->lineEdit_angle->setEnabled(false);
        ui->pushButton_ok->setEnabled(false);
        break;
    case Fixed:
        ui->lineEdit_xValue->setEnabled(true);
        ui->lineEdit_yValue->setEnabled(true);
        ui->lineEdit_angle->setEnabled(true);
        ui->pushButton_ok->setEnabled(true);
        break;
    default:
        break;
    }
}

void QG_RotateOptions::changeSelect() {
    changeShow();
    action->setCurRotateType(curSelect);
}

void QG_RotateOptions::rotateFixed() {
    double x = ui->lineEdit_xValue->text().toDouble();
    double y = ui->lineEdit_yValue->text().toDouble();
    double r = ui->lineEdit_angle->text().toDouble();

    action->rotateFixed(x, y, r);
}
