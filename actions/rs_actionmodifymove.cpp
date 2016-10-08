/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/

#include "rs_actionmodifymove.h"
#include <QAction>
//#include "rs_dialogfactory.h"
#include "rs_graphicview.h"
#include "dialogmediator.h"
#include "global_Values.h" //@zhang.y.q
#include "ErrorMessage.h"
#include "rs_actiondefault.h"
#include "rs_overlaybox.h"

RS_ActionModifyMove::RS_ActionModifyMove(RS_EntityContainer& container, RS_GraphicView& graphicView):
RS_PreviewActionInterface("Move Entities", container, graphicView)
{
    moveType = Manul;
	//begin
	ss               = new RS_Selection(container,&graphicView);
	m_OldCorner      = RS_Vector(false);
	m_NewCorner      = RS_Vector(false);
	SelectWindowFlag = false;//Ĭ�Ϲرտ�ѡ
	NeedSelectFlag   = true;//�Ƿ���Ҫ����ѡ��ʵ��ı�־��Ϊ��ʱ��
	m_bContinueSelt  = true;
	Init(0);//added by wang.k.h
	//end
	mouseLeftButtonDown_Count=0;//@zhang.y.q

	/* ���ò����Ի�����Ϣ */
	getDataWindow_Line=new getData_Line(gMainWindow, 5);
	getDataWindow_Line->hide();
	getDataWindow_Line->setFocus();
	getDataWindow_Line->move(80, 43);
	g_bIsResponseMouse =  true; //���������Ӧ
	getDataWindow_Line->installEventFilter(this);

	connect(getDataWindow_Line,SIGNAL(commit(MyPoint)),this,SLOT(OnCommit(MyPoint)));
	connect(getDataWindow_Line,SIGNAL(Right(QString)),this,SLOT(OnRight(QString)));
	connect(getDataWindow_Line,SIGNAL(Left(QString)),this,SLOT(OnLeft(QString)));
	connect(getDataWindow_Line,SIGNAL(Enter(QString)),this,SLOT(OnEnter(QString)));
	/* ���ò����Ի�����Ϣ */


}

RS_ActionModifyMove::~RS_ActionModifyMove()
{
	if (getDataWindow_Line != NULL)
	{
		delete getDataWindow_Line;
		getDataWindow_Line = NULL;
	}
	if (ss != NULL)
	{
		delete ss;
		ss = NULL;
	}
	
}

QAction* RS_ActionModifyMove::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) 
{
    // tr("&Move / Copy")
    QAction* action = new QAction(tr("&Move / Copy"),  NULL);
    action->setIcon(QIcon(":/extui/modifymove.png"));
    //action->zetStatusTip(tr("Move or copy entities one or multiple times"));

    return action;

}

void RS_ActionModifyMove::Init(int num)
{
	if (num == 1)
	{
		//��������
		int SelectResult = ss->HandContainer(document);
		if (4 == SelectResult)//ѡ����ȡ��ʱ
		{
			//��ʼ��������������ѡ��
			num = 0;
		}	
	}
	//��������
	for (RS_Entity *e=container->firstEntity(); e!=NULL; e = container->nextEntity())
	{
		//�ж��������Ƿ���ʵ�屻ѡ��
		if(e->isSelected() && e!=NULL)
		{
			if(num == 0)//��ʼ��ʱ��������������ʵ����Ϊ��ѡ��״̬
			{
				e->setSelected(false);
			}
			else//���ܲ���ʱ����ѡ����̵�״̬����
			{
				NeedSelectFlag = false;	
				break;
			}
		}
	}

	deletePreview();
	graphicView->redraw();

	if(m_bContinueSelt == false && NeedSelectFlag == false)//�У����һ������
	{
		setStatus(SetReferencePoint);
		graphicView->setMouseCursor(RS2::CadCursor);
	}
	else			//�ޣ����Ƚ���ѡ�����
	{
		//ErrorMessage err;
		//err.showErrorMessage(15007,2);
		setStatus(SetSelectEntitys);
		graphicView->setMouseCursor(RS2::ArrowCursor);
	}

}

void RS_ActionModifyMove::trigger()
{
    RS_DEBUG->print("RS_ActionModifyMove::trigger()");

    RS_Modification m(*container, graphicView);
    m.move(data);

    //RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected(),container->totalSelectedLength());
    finish();
	
	DIALOGMEDIATOR->requestOptions(this, false, false);
	pDemocadSignal->sendSignal_EditFinish("Move");

}

void RS_ActionModifyMove::mouseMoveEvent(QMouseEvent* e) 
{
	if (e == NULL)
	{
		return;
	}
	
	/************************************************************************/
	/* �������öԻ����������ƶ� */
	RS_Vector mouse = snapPoint(e);
	getDataWindow_Line->setFocus();

	int X = e->pos().x() + 80;
	int Y = e->pos().y() + 50;

	if(e->pos().x() + 50 > g_rsvDemoCadWH.x + g_rsvDemoCadS.x)
	{
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
	/************************************************************************/


    //���ƣ������Ի���״̬ʱ����Ӧ�¼���ֻ��ѡ�ο���ʱ��Ӧ
    if (moveType == Manul) 
	{
        if (getStatus()==SetReferencePoint || getStatus()==SetTargetPoint)
		{

            RS_Vector mouse = snapPoint(e);
            switch (getStatus())
			{
            case SetReferencePoint:
                referencePoint = mouse;
                break;

            case SetTargetPoint:
                if (referencePoint.valid)
				{
                    targetPoint = mouse;

                    deletePreview();
                    preview->addSelectionFrom( *container );
                    preview->move( targetPoint - referencePoint );
                    drawPreview();

					//DIALOGMEDIATOR->requestOptions(this, false, false);
                }
                break;

            default:
                break;
            }
        }
		else  //ѡ�����  @added by wang.k.h
		{
			 m_NewCorner = graphicView->toGraph(e->x(),e->y());

			 //���ƶ�̬���ο�
			 if(SelectWindowFlag)
			 {
				 deletePreview();
				 //����һ��������ĺ��ӣ���������ʾѡ���
				 RS_OverlayBox* ob = new RS_OverlayBox(preview, RS_OverlayBoxData(m_OldCorner, m_NewCorner));
				 preview->addEntity(ob);
				 drawPreview();

			 }
		}
		
    }

}

void RS_ActionModifyMove::mousePressEvent(QMouseEvent* e)
{
	//��Ҫѡ�����������ѡ��
	if(getStatus() == SetSelectEntitys && e->button()==Qt::LeftButton)//added by wang.k.h
	{
		if(0 == mouseLeftButtonDown_Count)//��һ�ε������ʱ����õ�һ���ǵ�
		{
			SelectWindowFlag = true;//������ѡ
			m_OldCorner = graphicView->toGraph(e->x(),e->y());
		}
		
	}
	else if(e->button()==Qt::LeftButton)
	{
		if(1==mouseLeftButtonDown_Count++)
		{
			mouseLeftButtonDown_Count = 0;
			pDemocadSignal->sendSignal_EditFinish("Move");
		}
	}
}


void RS_ActionModifyMove::mouseReleaseEvent(QMouseEvent* e)
{
	if (e == NULL)
	{
		return;
	}
	
    if (e->button()==Qt::LeftButton)
	{
		//��Ҫѡ�����������ѡ��
		if(getStatus() == SetSelectEntitys && moveType == Manul)//added by wang.k.h
		{
			m_NewCorner = graphicView->toGraph(e->x(),e->y());

			if(0 == mouseLeftButtonDown_Count)//��һ�ε���ͷ�ʱ����õڶ����ǵ�
			{
				if (m_NewCorner == m_OldCorner)//������ͬ������ѡ�¼�
				{
					RS_Entity* en = catchEntity(e, RS2::ResolveAll);
					if(en != NULL)//��׽��ʵ��
					{
						en->setSelected(true);	
						//setStatus(SetReferencePoint);
						//NeedSelectFlag = false;
						SelectWindowFlag = false;//�رտ�ѡ
						deletePreview();
						graphicView->redraw();

						//DIALOGMEDIATOR->requestOptions(this, true, true);
						getDataWindow_Line->show();
						DIALOGMEDIATOR->OperationPromptInfo(tr("ƽ��! ��ָ����������! ע: ��������Ҽ����Լ���ѡ��ͼ��! ����: ˫������Ҽ���ȡ��ƽ�Ʋ���!"));

						m_bContinueSelt  = false;
					}	
					else//û�в�׽��ʵ��
					{
						mouseLeftButtonDown_Count++;
					}
				}
				else  //��ͬ������ѡ�¼�
				{
				    //��ѡ������ʵ�����ѡ����
					bool cross = (m_OldCorner.x > m_NewCorner.x);
					ss->selectWindow(m_OldCorner,m_NewCorner,true,cross);
					SelectWindowFlag = false;//�رտ�ѡ
					deletePreview();

					//DIALOGMEDIATOR->requestOptions(this, true, true);
					getDataWindow_Line->show();
					DIALOGMEDIATOR->OperationPromptInfo(tr("ƽ��! ��ָ����������! ע: ��������Ҽ����Լ���ѡ��ͼ��! ����: ˫������Ҽ���ȡ��ƽ�Ʋ���!"));

					m_bContinueSelt = false;
				}
			}
			else	  //�ڶ��ε���ͷ�,���µڶ����ǵ�
			{
				bool cross = (m_OldCorner.x > m_NewCorner.x);
				ss->selectWindow(m_OldCorner,m_NewCorner,true,cross);
				mouseLeftButtonDown_Count--;
				SelectWindowFlag = false;//�رտ�ѡ
				deletePreview();

				getDataWindow_Line->show();
				DIALOGMEDIATOR->OperationPromptInfo(tr("ƽ��! ��ָ����������! ע: ��������Ҽ����Լ���ѡ��ͼ��! ����: ˫������Ҽ���ȡ��ƽ�Ʋ���!"));

				m_bContinueSelt = false;
			}

			Init(1);    //����NeedSelectFlag		
			
		}
        else if (moveType == Manul) 
		{//���ֶ�ʱ��֧��������
            
			qDebug() << moveType;
            RS_CoordinateEvent ce(snapPoint(e));
            coordinateEvent(&ce);
        }		

    }
	else if (e->button()==Qt::RightButton) //��֧������Ҽ�
	{
		deletePreview();

		if(getStatus() == 0)
		{
			pDemocadSignal->sendSignal_EditFinish("Move");
		}

        init(getStatus()-1);

		m_bContinueSelt = true;
		DIALOGMEDIATOR->OperationPromptInfo(tr("ƽ��! �뵥��������(���ѡ(��ס������ѡ��))����ѡ����Ҫƽ�Ƶ�ͼ��!"));
    }

}

void RS_ActionModifyMove::coordinateEvent(RS_CoordinateEvent* e) {

    if (e==NULL) {
        return;
    }

    RS_Vector pos = e->getCoordinate();

    switch (getStatus()) {
    case SetReferencePoint:
        referencePoint = pos;
        graphicView->moveRelativeZero(referencePoint);
        setStatus(SetTargetPoint);
		getDataWindow_Line->setNextLabelText(1);
		DIALOGMEDIATOR->OperationPromptInfo(tr("ƽ��! ��ָ��Ŀ�������!"));
        break;

    case SetTargetPoint:
        targetPoint = pos;
        graphicView->moveRelativeZero(targetPoint);
        setStatus(ShowDialog);

        //���� = 0, ��ֵ = 1 , ���ؿ���ʱ��Ҫ���������������Ϊ����
        caculateData();
        trigger();
		DIALOGMEDIATOR->OperationPromptInfo(tr("ƽ�����!"));
		m_bContinueSelt = true;
        break;

    default:
        break;
    }
}

void RS_ActionModifyMove::showOptions() 
{
//     RS_ActionInterface::showOptions();
//     DIALOGMEDIATOR->requestOptions(this, true, true);
}

void RS_ActionModifyMove::hideOptions() 
{
//     RS_ActionInterface::hideOptions();
//     DIALOGMEDIATOR->requestOptions(this, false);
}

void RS_ActionModifyMove::setCurMoveType(int mt)  
{
    moveType = mt;
    init(SetReferencePoint);//ѡ��ͬ������ʽʱ��action״̬��ʼ��
    deletePreview();
}

void RS_ActionModifyMove::caculateData() 
{
    data.number = 0;
    data.offset = targetPoint - referencePoint;
    data.useCurrentAttributes = false;
    data.useCurrentLayer = false;
}


void RS_ActionModifyMove::moveFixed(double x, double y) 
{
    referencePoint = RS_Vector(0., 0.);
    targetPoint = RS_Vector(x, y);
    caculateData();
    trigger();
}



//void RS_ActionModifyMove::updateMouseButtonHints() {
//    if(RS_DIALOGFACTORY != NULL) {
//        switch (getStatus()) {
//        /*case Select:
//            RS_DIALOGFACTORY->updateMouseWidget(tr("Pick entities to move"),
//                                           tr("Cancel"));
//            break;*/
//        case SetReferencePoint:
//            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
//                                                tr("Cancel"));
//            break;
//        case SetTargetPoint:
//            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify target point"),
//                                                tr("Back"));
//            break;
//        default:
//            RS_DIALOGFACTORY->updateMouseWidget("", "");
//            break;
//        }
//    }
//}



void RS_ActionModifyMove::updateMouseCursor() {
        if(graphicView != NULL){
		//	graphicView->setMouseCursor(RS2::CadCursor);
        }
}

void RS_ActionModifyMove::OnCommit(MyPoint mpost)
{
	switch (getStatus()) 
	{
	case SetReferencePoint:
		referencePoint.x = mpost.x;
		referencePoint.y = mpost.y;
		graphicView->moveRelativeZero(referencePoint);
		setStatus(SetTargetPoint);
		getDataWindow_Line->setNextLabelText(1);
		DIALOGMEDIATOR->OperationPromptInfo(tr("ƽ��! ��ָ��Ŀ�������!"));
		break;

	case SetTargetPoint:
		targetPoint.x = mpost.x;
		targetPoint.y = mpost.y;
		graphicView->moveRelativeZero(targetPoint);
		//getDataWindow_Line->hide();
		setStatus(ShowDialog);
		//���� = 0, ��ֵ = 1 , ���ؿ���ʱ��Ҫ���������������Ϊ����
		caculateData();
		trigger();
		DIALOGMEDIATOR->OperationPromptInfo(tr("ƽ�����!"));
		m_bContinueSelt = true;
		break;

	default:
		break;
	}

}

void RS_ActionModifyMove::OnRight(QString f)
{
	getDataWindow_Line->setNextFocus();
}

void RS_ActionModifyMove::OnLeft(QString f)
{
	getDataWindow_Line->setNextFocus();
}

void RS_ActionModifyMove::OnEnter(QString f)
{
	getDataWindow_Line->DoInput();
}

// EOF
