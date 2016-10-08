#include "QG_CadToolBar_LeadWire.h"
#include "global_Values.h"
#include "Parameter.h"
#include "dialogmediator.h"

QG_CadToolBar_LeadWire::QG_CadToolBar_LeadWire(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QFont font("SimHei", 12, false);
	this->setFont(font);
	this->setGeometry(0, 0, 65, 100);
	setWindowFlags(Qt::FramelessWindowHint);

	actionHandler  = NULL;
	m_pContainer   = NULL;
	m_pGraphicView = NULL;

	connect(pDemocadSignal, SIGNAL(EditReset()), this, SLOT(LeadWireReset()));
	connect(ui.btnVertical, SIGNAL(clicked()),   this, SLOT(drawLeadLine()));
	//< cxm���������Զ����ɶ�Ӧ���е�����ߣ��ڸ߹�ӹ��в�̫��Ҫ
	//connect(ui.btnVertical, SIGNAL(clicked()),   this, SLOT(drawLeadLineAuto()));  
	connect(ui.btnPoint,    SIGNAL(clicked()),   this, SLOT(drawLeadLineSide()));
	connect(ui.btnAngle1,   SIGNAL(clicked()),   this, SLOT(drawLeadArcLineOblique()));
	connect(ui.btnAngle2,   SIGNAL(clicked()),   this, SLOT(drawLeadArcLineVertical()));
}

QG_CadToolBar_LeadWire::~QG_CadToolBar_LeadWire()
{

}

void QG_CadToolBar_LeadWire::LeadWireReset() 
{
	ui.btnVertical->setChecked(false);
	ui.btnPoint->setChecked(false);
	ui.btnAngle1->setChecked(false);
	ui.btnAngle2->setChecked(false);
}

void QG_CadToolBar_LeadWire::drawLeadLineAuto()
{
	if (!ui.btnVertical->isChecked())
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
		}
	}
	else
	{
		//LeadWireReset();
		g_pQPublicSignal->DemoCadReset();
		pDemocadSignal->sendSignal_EditReset();
		ui.btnVertical->setChecked(true);

		if(actionHandler!=NULL)
		{
			DIALOGMEDIATOR->OperationPromptInfo(tr("ֱ�������߷�ʽ! �뵥��������ȷ���������λ��!"));
			actionHandler->slotAutoDrawLeadLine();
			actionHandler->cancelSelect();
			//m_pGraphicView->showOrgGraph();
		}

	}
}

void QG_CadToolBar_LeadWire::drawLeadLine() 
{
	//actionHandler->ButtonReset();

	if (!ui.btnVertical->isChecked())
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
		}
	}
	else
	{
		//LeadWireReset();
		g_pQPublicSignal->DemoCadReset();
		pDemocadSignal->sendSignal_EditReset();
		ui.btnVertical->setChecked(true);

		if(actionHandler!=NULL)
		{
			DIALOGMEDIATOR->OperationPromptInfo(tr("ֱ�������߷�ʽ! �뵥��������ȷ���������λ��!"));
			actionHandler->slotDrawLeadLine();
			actionHandler->cancelSelect();
			//m_pGraphicView->showOrgGraph();
		}

	}
	 
// 	if(actionHandler!=NULL)
// 	{
// 		actionHandler->slotDrawLeadLine();
// 	}
// 	LeadWireReset();

}

void QG_CadToolBar_LeadWire::drawLeadLineSide() 
{
	if (!ui.btnPoint->isChecked())
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
		}
	}
	else
	{
		g_pQPublicSignal->DemoCadReset();
		//LeadWireReset();
		pDemocadSignal->sendSignal_EditReset();
		ui.btnPoint->setChecked(true);

		if(actionHandler!=NULL)
		{
			DIALOGMEDIATOR->OperationPromptInfo(tr("�˵������߷�ʽ! �뵥��������ȷ���������λ��!"));
			actionHandler->slotDrawLeadLineSide();
			actionHandler->cancelSelect();
			//m_pGraphicView->showOrgGraph();
		}

	}

// 	if(actionHandler!=NULL)
// 	{
// 		actionHandler->slotDrawLeadLineSide();
// 	}

}

void QG_CadToolBar_LeadWire::drawLeadArcLineVertical() 
{
	if (!ui.btnAngle2->isChecked())
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
		}
	}
	else
	{
		g_pQPublicSignal->DemoCadReset();
		//LeadWireReset();
		pDemocadSignal->sendSignal_EditReset();
		ui.btnAngle2->setChecked(true);

		if(actionHandler!=NULL)
		{
			DIALOGMEDIATOR->OperationPromptInfo(tr("Բ��2�����߷�ʽ! �뵥��������ȷ���������λ��!"));
			actionHandler->slotDrawLeadArcLineVertical();
			actionHandler->cancelSelect();
			//m_pGraphicView->showOrgGraph();
		}
	}

// 	if(actionHandler!=NULL)
// 	{
// 		actionHandler->slotDrawLeadArcLineVertical();
// 	}

}

void QG_CadToolBar_LeadWire::drawLeadArcLineOblique()
{
	if (!ui.btnAngle1->isChecked())
	{
		if (actionHandler != NULL)
		{
			actionHandler->reset();
		}
	}
	else
	{
		g_pQPublicSignal->DemoCadReset();
		//LeadWireReset();
		pDemocadSignal->sendSignal_EditReset();
		ui.btnAngle1->setChecked(true);

		if(actionHandler!=NULL)
		{
			DIALOGMEDIATOR->OperationPromptInfo(tr("Բ��1�����߷�ʽ! �뵥��������ȷ���������λ��!"));
			actionHandler->slotDrawLeadArcLineOblique();
			actionHandler->cancelSelect();
			//m_pGraphicView->showOrgGraph();
		}
	}

// 	if(actionHandler!=NULL)
// 	{	
// 		actionHandler->slotDrawLeadArcLineOblique();
// 	}

}