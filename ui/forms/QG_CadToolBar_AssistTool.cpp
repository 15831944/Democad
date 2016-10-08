#include "QG_CadToolBar_AssistTool.h"
#include"global_Values.h"
#include "dialogmediator.h"

QG_CadToolBar_AssistTool::QG_CadToolBar_AssistTool(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QFont font("SimHei", 12, false);
	this->setFont(font);
	this->setGeometry(0, 0, 65, 300);
	setWindowFlags(Qt::FramelessWindowHint);

	actionHandler = NULL;

	connect(ui.btnShowValue,     SIGNAL(clicked()), this, SLOT(ShowValue()));
	connect(ui.btnPointDistance, SIGNAL(clicked()), this, SLOT(Distance_Point()));
	connect(ui.btnCellDistance,  SIGNAL(clicked()), this, SLOT(Distance_Cell()));
	connect(ui.btnAngle,		 SIGNAL(clicked()), this, SLOT(ShowAngle()));
	connect(ui.btnFullScreen,	 SIGNAL(clicked()), this, SLOT(FullScreen()));
	connect(ui.btnGridSet,		 SIGNAL(clicked()), this, SLOT(GridSet()));
	connect(ui.btnSizeSet,		 SIGNAL(clicked()), this, SLOT(SizeSet()));
	connect(ui.btnOriginalSet,   SIGNAL(clicked()), this, SLOT(OriginalSet()));
	connect(ui.btnOriginalInit,  SIGNAL(clicked()), this, SLOT(OriginalInit()));

	connect(pDemocadSignal, SIGNAL(EditReset()), this, SLOT(AssistToolReset()));

}

QG_CadToolBar_AssistTool::~QG_CadToolBar_AssistTool()
{

}

void QG_CadToolBar_AssistTool::ShowValue()
{
	if (!ui.btnShowValue->isChecked())
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
		}
	}
	else
	{
		//pDemocadSignal->sendSignal_EditReset();
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
			DIALOGMEDIATOR->OperationPromptInfo(tr("��ֵ! �뵥��������ѡ��Ҫ��ֵ��Ҫ��!"));
			actionHandler->slotshowPosOfTP();
		}
		ui.btnShowValue->setChecked(true);
	}
	
}

void QG_CadToolBar_AssistTool::Distance_Point()
{
	if (!ui.btnPointDistance->isChecked())
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
		}
	}
	else
	{
		//pDemocadSignal->sendSignal_EditReset();
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
			DIALOGMEDIATOR->OperationPromptInfo(tr("���������ľ���! �뵥��������������/�ҷ������ͨ�����������������!"));
			actionHandler->slotcalDisOfTP();
		}
		ui.btnPointDistance->setChecked(true);
	}
}

void QG_CadToolBar_AssistTool::Distance_Cell()
{
	if (!ui.btnCellDistance->isChecked())
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
		}
	}
	else
	{
		//pDemocadSignal->sendSignal_EditReset();
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
			DIALOGMEDIATOR->OperationPromptInfo(tr("������Ҫ�ؼ�ľ���! �뵥��������ѡ����һ��Ҫ��!"));
			actionHandler->slotshowMinDistance();
		}
		ui.btnCellDistance->setChecked(true);
	}
}

void QG_CadToolBar_AssistTool::ShowAngle(){	if (!ui.btnAngle->isChecked())
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
		}
	}
	else
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
			DIALOGMEDIATOR->OperationPromptInfo(tr("������Ҫ�ؼ�ĽǶ�! �뵥��������ѡ����һ��Ҫ��!"));
			actionHandler->slotshowAngel();
		}
		ui.btnAngle->setChecked(true);
	}}

void QG_CadToolBar_AssistTool::GridSet()
{
	if (!ui.btnGridSet->isChecked())
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
		}
	}
	else
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
			actionHandler->slotsetGridType();
		}
		ui.btnGridSet->setChecked(true);
	}

}

void QG_CadToolBar_AssistTool::SizeSet()
{
// 	if (!ui.btnSizeSet->isChecked())
// 	{
// 		if (actionHandler != NULL)
// 		{
// 			actionHandler->reset();
// 			AssistToolReset();
// 		}
// 	}
// 	else
	{
		ui.btnSizeSet->setChecked(true);
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
			DIALOGMEDIATOR->OperationPromptInfo(tr("ͼ�γߴ��ʼ��!"));
			actionHandler->slotSizeInitial();
		}
		ui.btnSizeSet->setChecked(false);
	}

}

void QG_CadToolBar_AssistTool::OriginalSet()
{
	if (!ui.btnOriginalSet->isChecked())
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
		}
	}
	else
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
			DIALOGMEDIATOR->OperationPromptInfo(tr("ԭ������! �뵥��������(��ʹ����/�ҷ����ͨ����������)ȷ����ԭ��λ��!"));
			actionHandler->slotoriginSet();
		}
		ui.btnOriginalSet->setChecked(true);
	}

}

void QG_CadToolBar_AssistTool::OriginalInit()
{
// 	if (!ui.btnOriginalInit->isChecked())
// 	{
// 		if (actionHandler != NULL)
// 		{
// 			actionHandler->reset();
// 			AssistToolReset();
// 		}
// 	}
// 	else
	{
		ui.btnOriginalInit->setChecked(true);
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
			DIALOGMEDIATOR->OperationPromptInfo(tr("ԭ���ʼ��!"));
			actionHandler->slotoriginInitial();
		}
		ui.btnOriginalInit->setChecked(false);
	}

}

void QG_CadToolBar_AssistTool::FullScreen()
{
// 	if (!ui.btnFullScreen->isChecked())
// 	{
// 		if (actionHandler != NULL)
// 		{
// 			actionHandler->reset();
// 			AssistToolReset();
// 		}
// 	}
// 	else
	{
		ui.btnFullScreen->setChecked(true);
		if (actionHandler != NULL)
		{
			actionHandler->reset();
			AssistToolReset();
			DIALOGMEDIATOR->OperationPromptInfo(tr("�������ܿ���!"));
			actionHandler->slotfullScreen();
		}
		ui.btnFullScreen->setChecked(false);
	}

}

void QG_CadToolBar_AssistTool::AssistToolReset()
{
	ui.btnShowValue->setChecked(false);
	ui.btnPointDistance->setChecked(false);
	ui.btnCellDistance->setChecked(false);
	ui.btnAngle->setChecked(false);
	ui.btnFullScreen->setChecked(false);
	ui.btnGridSet->setChecked(false);
	ui.btnSizeSet->setChecked(false);
	ui.btnOriginalSet->setChecked(false);
	ui.btnOriginalInit->setChecked(false);
}