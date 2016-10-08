
#include "rs_actiondrawleadlineside.h"

#include <QAction>
#include "rs_actioneditundo.h"
#include "rs_graphicview.h"
#include "rs_document.h"
#include "en_Line.h"

#include "dialogmediator.h"

#include"global_Values.h" //@zhang.y.q
#include "global_Operator.h"//@zhang.y.q
//#include <QMessageBox> //use for debug @zhang.y.q
#include "ErrorMessage.h"



RS_ActionDrawLeadLineSide::RS_ActionDrawLeadLineSide(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
    :RS_PreviewActionInterface("Draw lines",
                               container, graphicView) {

    RS_DEBUG->print("RS_ActionDrawLeadLineSide::RS_ActionDrawLeadLineSide");
    reset();
	//Begin Modify @
	getDataWindow_Line=new getData_Line(gMainWindow,4);
	getDataWindow_Line->show();
	getDataWindow_Line->setFocus();
	//getDataWindow_Line->move(320,390);
	getDataWindow_Line->move(80, 43);
	g_bIsResponseMouse = true;
	getDataWindow_Line->installEventFilter(this);

	connect(getDataWindow_Line,SIGNAL(commit(MyPoint)),this,SLOT(OnCommit(MyPoint)));
	connect(getDataWindow_Line,SIGNAL(Right(QString)),this,SLOT(OnRight(QString)));
	connect(getDataWindow_Line,SIGNAL(Left(QString)),this,SLOT(OnLeft(QString)));
	connect(getDataWindow_Line,SIGNAL(Enter(QString)),this,SLOT(OnEnter(QString)));
	//End Modify @
    RS_DEBUG->print("RS_ActionDrawLeadLineSide::RS_ActionDrawLeadLineSide: OK");
}



RS_ActionDrawLeadLineSide::~RS_ActionDrawLeadLineSide()
{
	if (getDataWindow_Line != NULL)
	{
		delete getDataWindow_Line;
		getDataWindow_Line=NULL;
	}
}


QAction* RS_ActionDrawLeadLineSide::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
    //tr("Line: 2 Points")
    QAction* action = new QAction(tr("&2 Points"), NULL);
    action->setIcon(QIcon(":/extui/linesnormal.png"));
    //action->zetStatusTip(tr("Draw lines"));
    return action;
}

void RS_ActionDrawLeadLineSide::OnCommit(MyPoint mpost){//@zhang.y.q

	RS_Vector snapped = RS_Vector(mpost.x,mpost.y,0);	
	RS_CoordinateEvent ce(snapped);
	coordinateEvent(&ce);//����ÿ�εĵ��

	getDataWindow_Line->setFocus();
	/*end*/
}
void RS_ActionDrawLeadLineSide::OnRight(QString f){
	getDataWindow_Line->setNextFocus();
}

void RS_ActionDrawLeadLineSide::OnLeft(QString f){
	getDataWindow_Line->setNextFocus();
}

void RS_ActionDrawLeadLineSide::OnEnter(QString f){
	getDataWindow_Line->DoInput();
}


void RS_ActionDrawLeadLineSide::reset() {
    data = RS_LineData(RS_Vector(false), RS_Vector(false));
    start = RS_Vector(false);
}



void RS_ActionDrawLeadLineSide::init(int status) {
    RS_PreviewActionInterface::init(status);
    reset();
    drawSnapper();
}


void RS_ActionDrawLeadLineSide::mousePressEvent(QMouseEvent* e)
{	//@Modify
	
	entity = catchEntity(e, RS2::ResolveAll);

	if(e->button()==Qt::LeftButton)
	{//@Left Button
		prevMousePoint=curMousePoint;
		curMousePoint=e->pos();
		ErrorMessage err;

		if(data.startpoint.valid == false && getStatus()==SetEndpoint) setStatus(SetStartpoint);
		switch (getStatus()) 
		{
		case SetStartpoint:
			{
				RS_Vector snapped = snapPoint(e);
				RS_CoordinateEvent ce(snapped);
				coordinateEvent(&ce);//@Process Button Press remember the point of the lead line

				DIALOGMEDIATOR->OperationPromptInfo(tr("�˵������߷�ʽ! ���ƶ���굽�����յ�λ�ú󵥻�������ȷ�������յ�λ��!"));
			}
			break;

		case SetEndpoint:
			{
				double dJudge = (data.startpoint).distanceTo(data.endpoint);

				if (dJudge<((g_dSpacing>1) ? 1:g_dSpacing*0.1))
				{
					err.showErrorMessage(G_NDEMO_CAD_ERR_NUM+5,1);
					//QMessageBox::information(this, "����!", "�����յ�̫����");
					reset();
					deletePreview();
					init(getStatus()-1);

					DIALOGMEDIATOR->OperationPromptInfo(tr("������ʧ��! �����������뵥��������ȷ���������λ��! ��ȡ����������˫������Ҽ�!"));
					return ;
				}

				/*Begin Check Close Graph*/
				if (entity!=NULL &&(entity->rtti()==RS2::EntityLine ||entity->rtti()==RS2::EntityArc ||entity->rtti()==RS2::EntityCircle))
				{//@Check Close Graph

					if (entity->isLead() && entity->isVisible())
					{
						err.showErrorMessage(G_NDEMO_CAD_ERR_NUM+2,0);
						//QMessageBox::information(this,tr("����!"),tr("��������������������!"));
						deletePreview();
						init(getStatus()-1);

						DIALOGMEDIATOR->OperationPromptInfo(tr("������ʧ��! �����������뵥��������ȷ���������λ��! ��ȡ����������˫������Ҽ�!"));
						return;
					}

					QList<RS_Entity*> *pHave = new QList<RS_Entity*>;
					int nJudge = checkClosedGraph(entity, container, pHave);

					switch (nJudge)
					{
					case 0:			// ���������ͼ��
						{
							if(!isHasLead(pHave, document, container))
							{
								/*��ʼ �ɹ�����������*/
								//RS_Vector snapped = snapPoint(e);
								RS_Vector snapped = graphicView->toGraph(e->x(), e->y());
								RS_CoordinateEvent ce(snapped);
								coordinateEvent(&ce);//@Process Button Press remember the point of the lead line

								deletePreview();
								init(getStatus()-1);

								DIALOGMEDIATOR->OperationPromptInfo(tr("�����߳ɹ�! �����������뵥��������ȷ���������λ��! ��ȡ����������˫������Ҽ�!"));

							}
							else
							{
								//QMessageBox::information(this,"���棡","���ͼ���Ѿ������ߣ�");
								deletePreview();
								init(getStatus()-1);
								getDataWindow_Line->show();
								DIALOGMEDIATOR->OperationPromptInfo(tr("������ʧ��! �����������뵥��������ȷ���������λ��! ��ȡ����������˫������Ҽ�!"));
								
							}

						}//@Check the figure is closed
						break;
					case 1:			// ���Ƿ��ͼ��
						{
							ErrorMessage err;
							err.showErrorMessage(G_NDEMO_CAD_ERR_NUM+7,1);
							deletePreview();
							init(getStatus()-1);
							DIALOGMEDIATOR->OperationPromptInfo(tr("������ʧ��! �����������뵥��������ȷ���������λ��! ��ȡ����������˫������Ҽ�!"));

						}
						break;
					case 2:			// ͼ��������
						{
							ErrorMessage err;
							err.showErrorMessage(G_NDEMO_CAD_ERR_NUM+10,1);
							deletePreview();
							init(getStatus()-1);
							DIALOGMEDIATOR->OperationPromptInfo(tr("������ʧ��! �����������뵥��������ȷ���������λ��! ��ȡ����������˫������Ҽ�!"));

						}
						break;
					default:
						break;
					}

					if (pHave != NULL)
					{
						delete pHave;
						pHave = NULL;
					}

				}//@Check Close Graph
				else
				{
					err.showErrorMessage(G_NDEMO_CAD_ERR_NUM+5,0);
					//QMessageBox::information(this,"���棡","�������ȷ�������յ����ڵ�ֱ�ߡ�Բ��Բ����");
					deletePreview();
					init(getStatus()-1);
					return ;
				}
			}
			break;

		default:
			break;
		}

	}//@Left Button
	else if(e->button()==Qt::RightButton){//@Right Button
		deletePreview();
		//��aciont��status��ʼΪ0�������õڶ���Ľڵ㣬statusΪ1

		//Begin Modify @zhang.y.q
		if(getStatus()==0){
			//pDemocadSignal->sendSignal_EditFinish("Angle");
			pDemocadSignal->sendSignal_EditReset();
		}
		//End Modify @zhang.y.q

		init(getStatus()-1);
	}//@Right Button

	/*End New @zhang.y.q*/
	return ;

}


//û����һ�λ����һ���ߵ�����
void RS_ActionDrawLeadLineSide::trigger() 
{

//Modify @zhang.y.q
// 	if (NULL == entity || entity->GetLeadLinkFlag())			// add by zhou.g.x ����һ��ʵ����ֻ����һ������
// 	{
// 		return;
// 	}

	RS_PreviewActionInterface::trigger();

    RS_Line* line = new RS_Line(container, data);
	line->setLead(true);					// ����Ӱ�߱�־�������û���Ϊ��ɫ
	line->setLeadType(RS2::LeadSide);
	line->setLeadLinkEntity(entity);
	entity->addRelativeEntity(line);
	entity->SetLeadLinkFlag(true);			// add by zhou.g.x ����һ��ʵ����ֻ����һ������
	line->setPen(RS_Pen(RS_Color(255, 0, 0), RS2::WidthByLayer, RS2::SolidLine));
	container->prependEntity(line);			// ��֤ǰ��ŵĶ�������

    // upd. undo list:
    //��������undo�ṹ����ʱ����
    if (document!=NULL) {
        document->startUndoCycle();
        document->addUndoable(line);
        document->endUndoCycle();
    }

//	document->setModified(true);
    graphicView->redraw(RS2::RedrawDrawing);
	graphicView->showOrgGraph();
    //    graphicView->moveRelativeZero(line->getEndpoint());

	//��aciont��status��ʼΪ0�������õڶ���Ľڵ㣬statusΪ1
// 	deletePreview();
// 	init(getStatus()-1);     //add by wang.k.h

}




void RS_ActionDrawLeadLineSide::mouseMoveEvent(QMouseEvent* e) {
    RS_Vector mouse = snapPoint(e);
	if (getStatus() == SetEndpoint)
	{
		mouse = graphicView->toGraph(e->x(), e->y());
	}
	//begin
	getDataWindow_Line->setFocus();

	int X = e->pos().x() + 80;
	int Y = e->pos().y() + 50;

	if(e->pos().x() + 50 > g_rsvDemoCadWH.x + g_rsvDemoCadS.x)
	{
		//X = X - getDataWindow_Line->width()-20;
		X = X - getDataWindow_Line->getTotalLength()-20;
	}
	if(e->pos().y() + 150 > g_rsvDemoCadWH.y + g_rsvDemoCadS.y)
	{
		Y = Y - getDataWindow_Line->height()*2;
	}
	getDataWindow_Line->setX(mouse.x);
	getDataWindow_Line->setY(mouse.y);

	getDataWindow_Line->move(X, Y);
	getDataWindow_Line->Redisplay(0);
	//end
    if (getStatus()==SetEndpoint && start.valid) {

		RS_Entity* en = catchEntity(e, RS2::ResolveAll);

		if (en!=NULL && (en->rtti()==RS2::EntityLine || en->rtti()==RS2::EntityArc || en->rtti()==RS2::EntityCircle))
		{
			entity = en;
			RS_LineData dt;
			if (en->createLeadLineSideData(start, mouse, &dt))
			{
				deletePreview();					
				data = dt;
				RS_Line *line = new RS_Line(preview, dt);
				line->setPen(RS_Pen(RS_Color(255, 0, 0), RS2::WidthByLayer, RS2::SolidLine));
				preview->addEntity(line);
				drawPreview();

			}
			else
			{
				deletePreview();
				if(getStatus()==0)
				{
				 	pDemocadSignal->sendSignal_EditFinish("");

				}
				//init(getStatus()-1);     //add by wang.k.h
			}
		}
           
    }
}



void RS_ActionDrawLeadLineSide::mouseReleaseEvent(QMouseEvent* e) 
{
//     if (e->button()==Qt::LeftButton) {
//         RS_Vector snapped = snapPoint(e);
//         RS_CoordinateEvent ce(snapped);
//         coordinateEvent(&ce);//����ÿ�εĵ��
//     } else if (e->button()==Qt::RightButton) {
//         deletePreview();
//         //��aciont��status��ʼΪ0�������õڶ���Ľڵ㣬statusΪ1
// 		//Begin Modify @zhang.y.q
// 		if(getStatus()==0){
// 			pDemocadSignal->sendSignal_EditFinish("Angle");
// 		}
// 		//End Modify @zhang.y.q
//         init(getStatus()-1);
//     }

//	deleteLeadByG_NUM(0,container);
//	deleteLeadByLeadLinkEntity(container);
}



void RS_ActionDrawLeadLineSide::coordinateEvent(RS_CoordinateEvent* e) 
{
    if (e==NULL) {
        RS_DEBUG->print("RS_ActionDrawLeadLineSide::coordinateEvent: event was NULL");
        return;
    }


    RS_Vector mouse = e->getCoordinate();
    if(data.startpoint.valid == false && getStatus()==SetEndpoint) setStatus(SetStartpoint);
    switch (getStatus()) {
    case SetStartpoint:
        start = mouse;
        setStatus(SetEndpoint);//SetEndpointΪ1�����������õڶ���Ľڵ㣬statusΪ1
        graphicView->moveRelativeZero(mouse);
        updateMouseButtonHints();
		getDataWindow_Line->hide();
        break;

    case SetEndpoint:
        if((mouse-data.startpoint).squared() > RS_TOLERANCE2 && entity != NULL) {//����̫С��������̫�����ͺ���
            //refuse zero length lines
            trigger();//������ʹһ���߶���ӽ�����
			getDataWindow_Line->show();
        }
        break;

    default:
        break;
    }
}



void RS_ActionDrawLeadLineSide::updateMouseCursor() {
	graphicView->setMouseCursor(RS2::CadCursor);
}


//void RS_ActionDrawLeadLineSide::updateToolBar() {
//    if (RS_DIALOGFACTORY!=NULL) {
//        if (isFinished()) {
//            RS_DIALOGFACTORY->resetToolBar();
//        }
//    }
//}


// EOF
