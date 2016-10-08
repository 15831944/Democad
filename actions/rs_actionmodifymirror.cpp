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

#include "rs_actionmodifymirror.h"
#include <QAction>
#include "rs_graphicview.h"
#include "dialogmediator.h"
#include "global_Values.h" //@zhang.y.q
#include "ErrorMessage.h"
#include "rs_actiondefault.h"
#include "rs_overlaybox.h"

RS_ActionModifyMirror::RS_ActionModifyMirror(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Mirror Entities",
                           container, graphicView) {
    mirrorType = Manul;
	m_OldCorner = RS_Vector(false);
	m_NewCorner = RS_Vector(false);
	ss = new RS_Selection(container,&graphicView);
	SelectWindowFlag = false;//Ĭ�Ϲرտ�ѡ
	NeedSelectFlage = true;//�Ƿ���Ҫ����ѡ��ʵ��ı�־��Ϊ��ʱ��
	Init(0);//added by wang.k.h
	mouseLeftButtonDown_Count=0;//@zhang.y.q

	m_bContinueSelt = true;

	/* ���ò����Ի�����Ϣ */
	getDataWindow_Line=new getData_Line(gMainWindow, 6);
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

RS_ActionModifyMirror::~RS_ActionModifyMirror()
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

void RS_ActionModifyMirror::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (getStatus() != SetSelectEntitys)//added by wang.k.h
		{
			if(1 == mouseLeftButtonDown_Count++)
			{
				pDemocadSignal->sendSignal_EditFinish("Mirror");
			}
		}
		else
		{
			if(0 == mouseLeftButtonDown_Count)//��һ�ε������ʱ����õ�һ���ǵ�
			{
				SelectWindowFlag = true;//������ѡ
				m_OldCorner = graphicView->toGraph(e->x(),e->y());
			}
		}
	}
}

QAction* RS_ActionModifyMirror::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        // tr("Mirror"
    QAction* action = new QAction(tr("&Mirror"), NULL);
        action->setIcon(QIcon(":/extui/modifymirror.png"));
    //action->zetStatusTip(tr("Mirror Entities"));
    return action;
}

void RS_ActionModifyMirror::Init(int num) {

	if (num == 1 && document!=NULL)
	{
		//��������
		int SelectResult = ss->HandContainer(document);
		if (4 == SelectResult)//ѡ���ʱ
		{
			//��ʼ��������������ѡ��
			num = 0;
		}	
	}
	
	//��������
	for (RS_Entity *e=container->firstEntity(); e!=NULL; e=container->nextEntity())
	{
		//�ж��������Ƿ���ʵ�屻ѡ��
		if(e->isSelected() && e!=NULL)
		{
			if (num == 0)//��ʼ��ʱ��������������ʵ����Ϊ��ѡ��״̬
			{
				e->setSelected(false);
			}
			else//���ܲ���ʱ����ѡ����̵�״̬����
			{
				NeedSelectFlage = false;	
				break;
			}
		}
	}

	deletePreview();
	graphicView->redraw();

	//
	if(m_bContinueSelt == false && NeedSelectFlage == false)//�У����һ������
	{
		setStatus(SetAxisPoint1);
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

void RS_ActionModifyMirror::init(int status) {
    RS_ActionInterface::init(status);
}

void RS_ActionModifyMirror::trigger() {

    RS_DEBUG->print("RS_ActionModifyMirror::trigger()");

    RS_Modification m(*container, graphicView);
    m.mirror(data);


}



void RS_ActionModifyMirror::mouseMoveEvent(QMouseEvent* e) 
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

	if (getStatus() != ShowDialog)
	{
		getDataWindow_Line->setX(mouse.x);
		getDataWindow_Line->setY(mouse.y);
	}

	getDataWindow_Line->move(X, Y);
	getDataWindow_Line->Redisplay(0);
	/************************************************************************/
	
    if ((getStatus()==SetAxisPoint1 || getStatus()==SetAxisPoint2) && mirrorType == Manul)
	{

        RS_Vector mouse = snapPoint(e);
        switch (getStatus()) 
		{
        case SetAxisPoint1:
            axisPoint1 = mouse;
            break;

        case SetAxisPoint2:
            if (axisPoint1.valid)
			{
                axisPoint2 = mouse;

                deletePreview();
                preview->addSelectionFrom(*container);
                preview->mirror(axisPoint1, axisPoint2);
                preview->addEntity(new RS_Line(preview, RS_LineData(axisPoint1, axisPoint2)));

                drawPreview();
            }
            break;

        default:
            break;
        }
    }
	else if(mirrorType == Manul) //added  by wang.k.h
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

void RS_ActionModifyMirror::mouseReleaseEvent(QMouseEvent* e) 
{
    if (e->button()==Qt::LeftButton) 
	{
		if(getStatus() == SetSelectEntitys && mirrorType == Manul)//added by wang.k.h
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
						SelectWindowFlag = false;//�رտ�ѡ
						deletePreview();
						graphicView->redraw();

						getDataWindow_Line->show();
						DIALOGMEDIATOR->OperationPromptInfo(tr("����! ��ָ�������ߵ����! ע: ��������Ҽ����Լ���ѡ��ͼ��! ����: ˫������Ҽ���ȡ���������!"));
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

					getDataWindow_Line->show();
					DIALOGMEDIATOR->OperationPromptInfo(tr("����! ��ָ�������ߵ����! ע: ��������Ҽ����Լ���ѡ��ͼ��! ����: ˫������Ҽ���ȡ���������!"));
					m_bContinueSelt  = false;

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
				DIALOGMEDIATOR->OperationPromptInfo(tr("����! ��ָ�������ߵ����! ע: ��������Ҽ����Լ���ѡ��ͼ��! ����: ˫������Ҽ���ȡ���������!"));
				m_bContinueSelt  = false;

			}

			Init(1);    //����NeedSelectFlag		
		}
		else if (mirrorType ==  Manul) 
		{
			RS_CoordinateEvent ce(snapPoint(e));
			coordinateEvent(&ce);

		}

    } 
	else if (e->button()==Qt::RightButton) 
	{
        deletePreview();

		if(getStatus()==0)
		{
			pDemocadSignal->sendSignal_EditFinish("Mirror");
		}

        init(getStatus()-1);

		m_bContinueSelt = true;
		DIALOGMEDIATOR->OperationPromptInfo(tr("����! �뵥��������(���ѡ(��ס������ѡ��))����ѡ����Ҫ�����ͼ��!"));
    }
}

void RS_ActionModifyMirror::coordinateEvent(RS_CoordinateEvent* e) 
{
    if (e==NULL) 
	{
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) 
	{
    case SetAxisPoint1:
        axisPoint1 = mouse;
        setStatus(SetAxisPoint2);
        graphicView->moveRelativeZero(mouse);
		getDataWindow_Line->setNextLabelText(9);
		DIALOGMEDIATOR->OperationPromptInfo(tr("����! ��ָ�������ߵ��յ�!"));
        break;

    case SetAxisPoint2:
        axisPoint2 = mouse;
        setStatus(ShowDialog);
		graphicView->moveRelativeZero(mouse);
        //�������öԻ��򣬴��޸�

        caculateData();
        deletePreview();
        trigger();
        finish();
		DIALOGMEDIATOR->OperationPromptInfo(tr("�������!"));
        break;

    default:
        break;
    }

}

void RS_ActionModifyMirror::showOptions()
{
    RS_ActionInterface::showOptions();

    DIALOGMEDIATOR->requestOptions(this, true, true);
}



void RS_ActionModifyMirror::hideOptions() {
    RS_ActionInterface::hideOptions();

    DIALOGMEDIATOR->requestOptions(this, false);
}

void RS_ActionModifyMirror::mirrorFixed(int mt) 
{

    axisPoint1 = RS_Vector(0., 0.);

    if (mt == xAxlis) 
	{
        axisPoint2 = RS_Vector(10.0, 0.);
    } 
	else if (mt == yAxlis) 
	{
        axisPoint2 = RS_Vector(0., 10.0);
    }

    caculateData();
    trigger();
    finish();
}

void RS_ActionModifyMirror::caculateData() 
{
    data.axisPoint1 = axisPoint1;
    data.axisPoint2 = axisPoint2;
    data.copy = false;
    data.useCurrentAttributes = false;
    data.useCurrentLayer = false;
}



//void RS_ActionModifyMirror::updateMouseButtonHints() {
//    if (RS_DIALOGFACTORY!=NULL) {
//        switch (getStatus()) {
//            /*case Select:
//                RS_DIALOGFACTORY->updateMouseWidget(tr("Pick entities to move"),
//                                               tr("Cancel"));
//                break;*/
//        case SetAxisPoint1:
//            RS_DIALOGFACTORY->updateMouseWidget(
//                tr("Specify first point of mirror line"),
//                tr("Cancel"));
//            break;
//        case SetAxisPoint2:
//            RS_DIALOGFACTORY->updateMouseWidget(
//                tr("Specify second point of mirror line"),
//                tr("Back"));
//            break;
//        default:
//            RS_DIALOGFACTORY->updateMouseWidget("", "");
//            break;
//        }
//    }
//}



void RS_ActionModifyMirror::updateMouseCursor() {
    //graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyMirror::OnCommit(MyPoint mpost)
{
	RS_Vector pos;
	pos.x = mpost.x;
	pos.y = mpost.y;

	if (! pos.valid ) 
	{
		return;
	}

	switch (getStatus()) 
	{
	case SetAxisPoint1:
		axisPoint1 = pos;
		setStatus(SetAxisPoint2);
		graphicView->moveRelativeZero(pos);
		getDataWindow_Line->setNextLabelText(9);
		DIALOGMEDIATOR->OperationPromptInfo(tr("����! ��ָ�������ߵ��յ�!"));
		break;

	case SetAxisPoint2:
		axisPoint2 = pos;
		setStatus(ShowDialog);
		graphicView->moveRelativeZero(pos);
		//�������öԻ��򣬴��޸�

		caculateData();
		deletePreview();
		trigger();
		finish();
		DIALOGMEDIATOR->OperationPromptInfo(tr("�������!"));
		break;

	default:
		break;
	}

}

void RS_ActionModifyMirror::OnRight(QString f)
{
	getDataWindow_Line->setNextFocus();
}

void RS_ActionModifyMirror::OnLeft(QString f)
{
	getDataWindow_Line->setNextFocus();
}

void RS_ActionModifyMirror::OnEnter(QString f)
{
	getDataWindow_Line->DoInput();
}



// EOF
