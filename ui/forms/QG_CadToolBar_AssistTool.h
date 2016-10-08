#ifndef QG_CADTOOLBAR_ASSISTTOOL_H
#define QG_CADTOOLBAR_ASSISTTOOL_H

#include <QWidget>
#include "ui_QG_CadToolBar_AssistTool.h"
#include "qg_actionhandler.h"

class QG_CadToolBar_AssistTool : public QWidget
{
	Q_OBJECT

public:
	QG_CadToolBar_AssistTool(QWidget *parent = 0);
	~QG_CadToolBar_AssistTool();

	virtual void setActionHandler(QG_ActionHandler *aH) {
		actionHandler = aH;
	}

public slots:
	void ShowValue();			// ��ֵ
	void Distance_Point();		// ��������
	void Distance_Cell();		// Ҫ�ؼ����
	void ShowAngle();			// �Ƕ�
	void GridSet();				// դ������
	void SizeSet();				// �ߴ�����
	void OriginalSet();			// ԭ������
	void OriginalInit();		// ԭ���ʼ��
	void FullScreen();			// ����
	void AssistToolReset();		// 

private:
	Ui::QG_CadToolBar_AssistTool ui;
	QG_ActionHandler* actionHandler;
};

#endif // QG_CADTOOLBAR_ASSISTTOOL_H
