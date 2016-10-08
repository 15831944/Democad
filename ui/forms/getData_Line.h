#pragma once

#include <QWidget>
#include <QKeyEvent>
#include "ui_getData_Line.h"

//��������ʵ�ʵ�������������Ļ����
typedef struct my_point{
	double x;
	double y;
	double r;
}MyPoint;

//��ȡ�ߵ�������
class getData_Line :public QWidget{
	Q_OBJECT

public:
	getData_Line(QWidget *parent = NULL, int num = NULL, Qt::WFlags flags = NULL);
	~getData_Line(void);

	void resetLabelText(int num);//��ʼ��Label_X����ʾ�ַ�
	void setNextFocus();//������һ�������Ľ���
	void setXFocus();//����X���������Ľ���
	void setYFocus();//����Y���������Ľ���

	void setX(double x);//����Xֵ
	void setY(double y);//����Yֵ
	void setR(double r);//����Rֵ

	void setNextLabelText(int f);//������һ����ʾ��Ϣ
	void Redisplay(int f);//���²���

	void FinshFigure();

	int getTotalLength()//�����ʾ���ʵ�ʳ���
	{
		return this->m_nTotalLength;
	}

private:
	Ui::Form ui;

	int nMax;//X,Y������ʾ��Ŀ�����ֵ
	int m_nTotalLength;//��ʾ���ܳ���
	QDoubleValidator* pvd;//����double���͵�������ʽ

protected:
	bool eventFilter(QObject *obj, QEvent *e);//���ڴ��� ����� �Լ������Ӵ��ڵĵ��¼�
	void keyPressEvent(QKeyEvent *e);

signals:
	void commit(MyPoint mpost);//�����ź��ύ����
	void Enter(QString f);
	void Right(QString f);
	void Left(QString f);

public slots:
	void DoInput();//��������
	void Init();//��ʼ����

};

