#include "getData_Line.h"
#include "global_Values.h"
#include <QTextCodec>
#include "dialogmediator.h"

getData_Line::getData_Line(QWidget *parent, int nOptPraSet, Qt::WFlags flags)
{
	ui.setupUi(this);
	this->setParent(parent);

	QFont font("SimHei", 10, QFont::Bold);//
	this->setFont(font);
	ui.label_X->setFont(font);
	ui.lineEdit_X->setFont(font);
	ui.lineEdit_Y->setFont(font);

	ui.lineEdit_X->installEventFilter(this);
	ui.lineEdit_Y->installEventFilter(this);

	QPalette* pBKG=new QPalette;
	pBKG->setColor(QPalette::Background,QColor(0,0,139));
	ui.label_X->setPalette(*pBKG);
	ui.label_X->setAutoFillBackground(true);

// 	QTextCodec* code=QTextCodec::codecForName("GB2312");
// 	QTextCodec::setCodecForTr(code);

	ui.lineEdit_X->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");
	ui.lineEdit_Y->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");
	ui.label_X->resize(9*10,20);
	ui.lineEdit_X->resize(4*10,20);
	ui.lineEdit_Y->resize(4*10,20);
	m_nTotalLength = 170;
	nMax = 4;
	resetLabelText(nOptPraSet);

	Redisplay(0);

	pvd=new QDoubleValidator(-999999.9999,999999.9999,10,this);
	ui.lineEdit_X->setValidator(pvd);
	ui.lineEdit_Y->setValidator(pvd);
}

getData_Line::~getData_Line(void)
{
	if (pvd != NULL)
	{
		delete pvd;
		pvd=NULL;
	}

}

void getData_Line::resetLabelText(int nOptPraSet)//added by wang
{
	if( nOptPraSet == 0)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ���������:")+"</b></font>");
	}
	else if(nOptPraSet == 1)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ��Բ������:")+"</b></font>");
	}
	else if(nOptPraSet == 2)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ���µ�ԭ��:")+"</b></font>");
	}
	else if(nOptPraSet == 3)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("�������:")+"</b></font>");
		ui.label_X->resize(70,20);
	}
	else if (nOptPraSet == 4)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ�������������:")+"</b></font>");
		ui.label_X->resize(120,20);
	}
	else if (nOptPraSet == 5)		// ƽ��/����/��ת��������
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ����������:")+"</b></font>");
		//ui.label_X->resize(12*10,20);
	}
	else if (nOptPraSet == 6)		// �����������
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ�����������:")+"</b></font>");
		ui.label_X->resize(110,20);
	}
}

void getData_Line::FinshFigure(){
	ui.label_X->setText("<font color=white ><b>"+tr("ָ���������:")+"</b></font>");
}

void getData_Line::DoInput()
{
	g_bIsResponseMouse = true;//���������Ӧ
	double x=0;
	double y=0;

	QString strData = ui.lineEdit_X->text();
	x       = strData.toDouble();
	strData = ui.lineEdit_Y->text();
	y       = strData.toDouble();

	MyPoint mpost = {x, y};
	
	emit commit(mpost);//��õ�����Ž�mpost������źŷ��ͳ�ȥ

	ui.lineEdit_X->setStyleSheet("QLineEdit{background-color:rgba(0,0,139;color:rgb(255,255,255);border-width:0px;border-style:outset}");
	ui.lineEdit_Y->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");
//	this->hide();
	Init();
}

void getData_Line::Init()
{
// 	ui.lineEdit_X->setText("");
// 	ui.lineEdit_Y->setText("");

	ui.lineEdit_X->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");
	ui.lineEdit_Y->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");
	this->setFocus();
}

void getData_Line::setNextFocus()
{
	g_bIsResponseMouse = false;//�ر������Ӧ
	static bool _do=true;

	if(_do)
	{
		ui.lineEdit_X->setFocus();
		ui.lineEdit_X->selectAll();
		ui.lineEdit_X->setStyleSheet("QLineEdit{background-color:rgba(255,255,255);color:rgb(0,0,0);border-width:0px;border-style:outset}");
		ui.lineEdit_Y->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");
	}
	else
	{
		if (ui.lineEdit_Y->isHidden())
		{
			ui.lineEdit_X->setFocus();
			ui.lineEdit_X->selectAll();
			ui.lineEdit_X->setStyleSheet("QLineEdit{background-color:rgba(255,255,255);color:rgb(0,0,0);border-width:0px;border-style:outset}");
			ui.lineEdit_Y->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");
			return;
		}
		ui.lineEdit_Y->setFocus();
		ui.lineEdit_Y->selectAll();
		ui.lineEdit_Y->setStyleSheet("QLineEdit{background-color:rgba(255,255,255);color:rgb(0,0,0);border-width:0px;border-style:outset}");
		ui.lineEdit_X->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");
	}
	_do=!_do;
}

void getData_Line::setXFocus()
{
		ui.lineEdit_X->setFocus();
		ui.lineEdit_X->selectAll();
		ui.lineEdit_X->setStyleSheet("QLineEdit{background-color:rgba(255,255,255);color:rgb(0,0,0);border-width:0px;border-style:outset}");
		ui.lineEdit_Y->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");
}
void getData_Line::setYFocus()
{
	ui.lineEdit_Y->setFocus();
	ui.lineEdit_Y->selectAll();
	ui.lineEdit_Y->setStyleSheet("QLineEdit{background-color:rgba(255,255,255);color:rgb(0,0,0);border-width:0px;border-style:outset}");
	ui.lineEdit_X->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");
}

bool getData_Line::eventFilter(QObject *obj, QEvent *e)
{
	if(e->type()==QEvent::KeyPress)
	{
		QKeyEvent* ke=(QKeyEvent*)e;

		switch(ke->key())
		{
		case Qt::Key_Right:
			emit Right("RIGHT");
			break;
		case Qt::Key_Left:
			emit Left("LEFT");
			break;
// 		case Qt::Key_Enter-1:
// 			emit Enter("ENTER");
// 			break;
		}
	}
	
	if(obj==ui.lineEdit_X)
	{
		return ui.lineEdit_X->event(e);
	}
	else if(ui.lineEdit_Y)
	{
		return ui.lineEdit_Y->event(e);
	}

	return false;
}

void getData_Line::keyPressEvent(QKeyEvent *e)
{
	switch(e->key())
	{
	case Qt::Key_Right:
		emit Right("RIGHT");
		break;
	case Qt::Key_Left:
		emit Left("LEFT");
		break;
	case Qt::Key_Enter-1:
		emit Enter("ENTER");
		break;
	}
}

void getData_Line::setX(double x)
{
	//QString strX   = RS_Math::doubleToString(x, 4);
	QString strX = QString::number(x);

	int num = strX.count();
	if(num <= 4)
	{
		num = 4;
	}
	
	nMax = num;

	ui.lineEdit_X->resize(num*10,20);
	ui.lineEdit_X->setText(strX);//�����X������ʾ��С������
	ui.lineEdit_X->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");
	
}

void getData_Line::setY(double y)
{
	//QString strY   = RS_Math::doubleToString(y, 4);

	QString strY = QString::number(y);

	int num = strY.count();
	if(num<=4)
	{
		num = 4;
	}
	if(num > nMax)
	{
		nMax = num;
	}
	
	ui.lineEdit_Y->resize(nMax*10,20);
	ui.lineEdit_X->resize(nMax*10,20);
	ui.lineEdit_Y->setText(strY);
	ui.lineEdit_Y->setStyleSheet("QLineEdit{background-color:rgba(0,0,139);color:rgb(255,255,255);border-width:0px;border-style:outset}");

}

void getData_Line::setNextLabelText(int nNum)
{
	if(nNum == 0)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ���������:")+"</b></font>");
		ui.label_X->resize(100,20);
	}
	else if(nNum == 1)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ����һ������:")+"</b></font>");
		ui.label_X->resize(110,20);
	}
	else if (nNum == 2)
	{
		ui.lineEdit_Y->show();//�����ص���ָ������ʾ����
		ui.label_X->setText("<font color=white ><b>"+tr("ָ��Բ������:")+"</b></font>");
		ui.label_X->resize(100,20);
	}
	else if (nNum == 3)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ���յ�����:")+"</b></font>");
		ui.label_X->resize(110,20);
	}
	else if (nNum == 4)
	{
		ui.lineEdit_Y->hide();//���ص�һ��ָ����
		ui.label_X->setText("<font color=white ><b>"+tr("Բ(��)�뾶:")+"</b></font>");
		ui.label_X->resize(90,20);
	}
	else if (nNum == 5)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("�������:")+"</b></font>");
		ui.label_X->resize(80,20);
	}
	else if (nNum == 6)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("�յ�����:")+"</b></font>");
		ui.label_X->resize(80,20);
	}
	else if (nNum == 7)
	{
		ui.lineEdit_Y->hide();//���ص�һ��ָ����
		ui.label_X->setText("<font color=white ><b>"+tr("ָ����ת�Ƕ�:")+"</b></font>");
		ui.label_X->resize(100,20);
		setX(0);
	}
	else if (nNum == 8)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ���ο�������:")+"</b></font>");
		ui.label_X->resize(120,20);
	}
	else if (nNum == 9)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ�����ű���:")+"</b></font>");
		ui.label_X->resize(100,20);
		setX(1.0);
		setY(1.0);
	}
	else if (nNum == 10)
	{
		ui.label_X->setText("<font color=white ><b>"+tr("ָ�������ߵ��յ�:")+"</b></font>");
		ui.label_X->resize(120, 20);
	}
	else if (nNum == 11)
	{
		ui.lineEdit_Y->show();//�����ص���ָ������ʾ����
		ui.label_X->setText("<font color=white ><b>"+tr("ָ�������������:")+"</b></font>");
		ui.label_X->resize(120, 20);
	}

	Redisplay(nNum);

}

void getData_Line::Redisplay(int f)
{
	QPoint tpoint=ui.label_X->pos();

	if (f==4)//Ҫָ���뾶ʱ�Ĵ���
	{
		m_nTotalLength = ui.label_X->width()+ nMax*10+ 2;
		ui.lineEdit_X->move(QPoint(tpoint.x()+ui.label_X->width()+2,tpoint.y()));
		return;
	}
	m_nTotalLength = ui.label_X->width()+  nMax*20+ 4;
	ui.lineEdit_X->move(QPoint(tpoint.x()+ui.label_X->width()+2,tpoint.y()));

	tpoint=ui.lineEdit_X->pos();
	ui.lineEdit_Y->move(QPoint(tpoint.x()+ui.lineEdit_X->width()+2,tpoint.y()));
}
