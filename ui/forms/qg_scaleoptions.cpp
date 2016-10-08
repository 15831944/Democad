#include "qg_scaleoptions.h"
#include "ui_qg_scaleoptions.h"

QG_ScaleOptions::QG_ScaleOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QG_ScaleOptions)
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
	ui->label_4->setFont(font);
	ui->label_5->setFont(font);
	ui->label_6->setFont(font);
	ui->label_7->setFont(font);
	ui->lineEdit_xBaseValue->setFont(font);
	ui->lineEdit_yBaseValue->setFont(font);
	ui->lineEdit_xFactorValue->setFont(font);
	ui->lineEdit_yFactorValue->setFont(font);

}

QG_ScaleOptions::~QG_ScaleOptions()
{
    delete ui;
}

void QG_ScaleOptions::setAction( RS_ActionInterface * a, bool update ) {
    if (a!=NULL && a->rtti()==RS2::ActionModifyScale) {
        action = (RS_ActionModifyScale*)a;

        //���ݱ�־���ж��Ƿ���Ҫ����action״̬���¿ؼ���ѡ��״̬
        if (update) {
            //����action��ǰ״̬����ȡ��Ϣ
            //��������ṹ��setAction�ᾭ���ı䣬������½��治��Ӱ�쵽action������
            //��Ϊ�ı�ѡ��Ҫ��λaction״̬������Ҫȷ��action���˵����ť��ɣ������������������ţ�����������
            setCurSelect(action->getCurScaleType());
        } else {
            //Ĭ����Ϣ
            setCurSelect(Manul);
        }


    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, "QG_MoveOptions::setAction: wrong action type");
        action = NULL;
    }
}

int QG_ScaleOptions::getCurSelect() {
    if (ui->radioButton_manual->isChecked()) {
        return Manul;
    } else if (ui->radioButton_fixed->isChecked()) {
        return Fixed;
    } else {
        return Manul;
    }
}

void QG_ScaleOptions::setCurSelect(int select) {
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

void QG_ScaleOptions::changeShow() {
    curSelect =getCurSelect();
    switch (curSelect)
    {
    case Manul:
        ui->lineEdit_xFactorValue->setEnabled(false);
        ui->lineEdit_yFactorValue->setEnabled(false);
        ui->lineEdit_xBaseValue->setEnabled(false);
        ui->lineEdit_yBaseValue->setEnabled(false);
        ui->pushButton_ok->setEnabled(false);
        break;
    case Fixed:
        ui->lineEdit_xFactorValue->setEnabled(true);
        ui->lineEdit_yFactorValue->setEnabled(true);
        ui->lineEdit_xBaseValue->setEnabled(true);
        ui->lineEdit_yBaseValue->setEnabled(true);
        ui->pushButton_ok->setEnabled(true);
        break;
    default:
        break;
    }
}

void QG_ScaleOptions::changeSelect() {
    changeShow();
    action->setCurScaleType(curSelect);
}

void QG_ScaleOptions::scaleFixed() {
    double bx = ui->lineEdit_xBaseValue->text().toDouble();
    double by = ui->lineEdit_yBaseValue->text().toDouble();
    double fx = ui->lineEdit_xFactorValue->text().toDouble();
    double fy = ui->lineEdit_yFactorValue->text().toDouble();

    action->scalePrameter(bx, by, fx, fy);
}
