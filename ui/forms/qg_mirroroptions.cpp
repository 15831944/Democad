#include "qg_mirroroptions.h"
#include "ui_qg_mirroroptions.h"

QG_MirrorOptions::QG_MirrorOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QG_MirrorOptions)
{
    ui->setupUi(this);

	QFont font("SimHei", 12, false);//
	this->setFont(font);
	ui->lbInfo->setFont(font);
	ui->radioButton_manual->setFont(font);
	ui->radioButton_xAxlis->setFont(font);
	ui->pushButton_ok->setFont(font);
	ui->radioButton_yAxlis->setFont(font);

}

QG_MirrorOptions::~QG_MirrorOptions()
{
    delete ui;
}


void QG_MirrorOptions::setAction( RS_ActionInterface * a, bool update ) {
    if (a!=NULL && a->rtti()==RS2::ActionModifyMirror) {
        action = (RS_ActionModifyMirror*)a;

        //���ݱ�־���ж��Ƿ���Ҫ����action״̬���¿ؼ���ѡ��״̬
        if (update) {
            //����action��ǰ״̬����ȡ��Ϣ
            //��������ṹ��setAction�ᾭ���ı䣬������½��治��Ӱ�쵽action������
            //��Ϊ�ı�ѡ��Ҫ��λaction״̬������Ҫȷ��action���˵����ť��ɣ������������������ţ�����������
            setCurSelect(action->getCurMirrorType());
        } else {
            //Ĭ����Ϣ
            setCurSelect(Manul);
        }


    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, "QG_MoveOptions::setAction: wrong action type");
        action = NULL;
    }
}

int QG_MirrorOptions::getCurSelect() {
    if (ui->radioButton_manual->isChecked()) {
        return Manul;
    } else if (ui->radioButton_xAxlis->isChecked()) {
        return xAxlis;
    } else if (ui->radioButton_yAxlis->isChecked()) {
        return yAxlis;
    } else {
        return Manul;
    }
}

void QG_MirrorOptions::setCurSelect(int select) {
    switch (select)
    {
    case Manul:
        ui->radioButton_manual->setChecked(true);//�ǲ�����click�źŵ�
        break;
    case xAxlis:
        ui->radioButton_xAxlis->setChecked(true);
        break;
    case yAxlis:
        ui->radioButton_yAxlis->setChecked(true);
        break;
    default:
        break;
    }
    changeShow();
}

void QG_MirrorOptions::changeShow() {
    curSelect =getCurSelect();
    switch (curSelect)
    {
    case Manul:
        ui->pushButton_ok->setEnabled(false);
        break;
    case xAxlis:
    case yAxlis:
        ui->pushButton_ok->setEnabled(true);
        break;
    default:
        break;
    }
}

void QG_MirrorOptions::changeSelect() {
    changeShow();
    action->setCurMirrorType(curSelect);
}

void QG_MirrorOptions::mirrorFixed() {
    curSelect =getCurSelect();
    action->mirrorFixed(curSelect);
}
