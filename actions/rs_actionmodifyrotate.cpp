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

#include "rs_actionmodifyrotate.h"

#include <QAction>
//#include "rs_dialogfactory.h"
#include "rs_graphicview.h"

#include "dialogmediator.h"
#include "rs_math.h"

#include "global_Values.h" //@zhang.y.q
#include "rs_overlaybox.h"



RS_ActionModifyRotate::RS_ActionModifyRotate(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
    :RS_PreviewActionInterface("Rotate Entities",
                               container, graphicView) {
    rotateType =Manul;
	m_OldCorner = RS_Vector(false);
	m_NewCorner = RS_Vector(false);
	ss = new RS_Selection(container,&graphicView);
	SelectWindowFlag = false;//Ĭ�Ϲرտ�ѡ
	NeedSelectFlage = true;//�Ƿ���Ҫ����ѡ��ʵ��ı�־��Ϊ��ʱ��
	Init(0);//added by wang.k.h
	mouseLeftButtonDown_Count=0;//@zhang.y.q

	m_bContinueSelt = true;

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

RS_ActionModifyRotate::~RS_ActionModifyRotate()
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

void RS_ActionModifyRotate::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (getStatus() != setSelectEntitys)//added by wang.k.h
		{
			if(1 == mouseLeftButtonDown_Count++)
			{
				mouseLeftButtonDown_Count=0;
				pDemocadSignal->sendSignal_EditFinish("Rotate");
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



QAction* RS_ActionModifyRotate::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) 
{
    // tr("Rotate")
    QAction* action = new QAction(tr("&Rotate"), NULL);
    action->setIcon(QIcon(":/extui/modifyrotate.png"));
    //action->zetStatusTip(tr("Rotate Entities"));
    return action;
}

void RS_ActionModifyRotate::Init(int num)//added by wang.k.h
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
	for (RS_Entity *e=container->firstEntity();//added by.wang.k.h
		e!=NULL;
		e=container->nextEntity())
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
		setStatus(setReferencePoint);
		graphicView->setMouseCursor(RS2::CadCursor);
	}
	else			//�ޣ����Ƚ���ѡ�����
	{
		//ErrorMessage err;
		//err.showErrorMessage(15007,2);
		setStatus(setSelectEntitys);
		graphicView->setMouseCursor(RS2::ArrowCursor);
	}

}
void RS_ActionModifyRotate::init(int status) 
{
    RS_ActionInterface::init(status);
}



void RS_ActionModifyRotate::trigger() 
{
    RS_Modification m(*container, graphicView);
    m.rotate(data);

    //RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected(),container->totalSelectedLength());
}



void RS_ActionModifyRotate::mouseMoveEvent(QMouseEvent* e) 
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

	if (getStatus() == setRotateAngle)
	{
		getDataWindow_Line->setX(0);
		getDataWindow_Line->setY(0);
	}
	else
	{
		getDataWindow_Line->setX(mouse.x);
		getDataWindow_Line->setY(mouse.y);
	}

	getDataWindow_Line->move(X, Y);
	getDataWindow_Line->Redisplay(0);
	/************************************************************************/


    if (rotateType == Manul && getStatus() != setSelectEntitys) 
	{
        RS_Vector mouse = snapPoint(e);
        switch (getStatus()) {
//        case setCenterPoint:
        case setReferencePoint:
            break;

        case setRotateAngle:
            if( ! mouse.valid ) return;
            deletePreview();
            preview->addSelectionFrom(*container);
            //preview->rotate(data.center,RS_Math::correctAngle((mouse - data.center).angle() - data.angle));
			preview->rotate(data.center,(mouse-data.center).angle());
            drawPreview();
        }
    }
	else if(rotateType == Manul)//added by wang.k.h
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



void RS_ActionModifyRotate::mouseReleaseEvent(QMouseEvent* e) 
{
	if (e == NULL)
	{
		return;
	}

    if (e->button()==Qt::LeftButton) 
	{
		if (rotateType ==  Manul && getStatus() == setSelectEntitys)//added by wang.k.h
		{
			m_NewCorner = graphicView->toGraph(e->x(),e->y());

			if(0 == mouseLeftButtonDown_Count)//��һ�ε���ͷ�ʱ����õڶ����ǵ�
			{
				if (checkSamePoint(m_NewCorner, m_OldCorner))//������ͬ������ѡ�¼�
				{
					RS_Entity* en = catchEntity(e, RS2::ResolveAll);
					if(en != NULL)//��׽��ʵ��
					{
						en->setSelected(true);	
						setStatus(setReferencePoint);
						SelectWindowFlag = false;//�رտ�ѡ
						deletePreview();
						graphicView->redraw();

						getDataWindow_Line->show();
						DIALOGMEDIATOR->OperationPromptInfo(tr("��ת! ��ָ����������! ע: ��������Ҽ����Լ���ѡ��ͼ��! ����: ˫������Ҽ���ȡ����ת����!"));

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
					DIALOGMEDIATOR->OperationPromptInfo(tr("��ת! ��ָ����������! ע: ��������Ҽ����Լ���ѡ��ͼ��! ����: ˫������Ҽ���ȡ����ת����!"));

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
				DIALOGMEDIATOR->OperationPromptInfo(tr("��ת! ��ָ����������! ע: ��������Ҽ����Լ���ѡ��ͼ��! ����: ˫������Ҽ���ȡ����ת����!"));

				m_bContinueSelt = false;

			}

			Init(1);    //����NeedSelectFlag	
		}
        else if (rotateType ==  Manul) 
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
			pDemocadSignal->sendSignal_EditFinish("Rotate");
		}

        init(getStatus()-1);

		m_bContinueSelt = true;
		DIALOGMEDIATOR->OperationPromptInfo(tr("��ת! �뵥��������(���ѡ(��ס������ѡ��))����ѡ����Ҫ��ת��ͼ��!"));

    }
}



void RS_ActionModifyRotate::coordinateEvent(RS_CoordinateEvent* e) 
{
    if (e==NULL) 
	{
        return;
    }

    RS_Vector pos = e->getCoordinate();

    if (!pos.valid ) 
	{
        return;
    }

    switch (getStatus()) 
	{
    case setReferencePoint:
        pos -= data.center;
		data.center = pos;
        if ( pos.squared()< RS_TOLERANCE2 ) 
		{
            //��ֹ�ο���ͻ���ͬһ���㣬������ѧ�������
            data.angle = 0.;//angle not well defined, go direct to dialog
            QMessageBox::warning(NULL, tr("Warning"), tr("Reference point can not be the same starting point"));
        } 
		else 
		{
            data.angle = pos.angle();//�����������ĽǶ�
            setStatus(setRotateAngle);
        }

		setStatus(setRotateAngle);
		getDataWindow_Line->setNextLabelText(7);
		DIALOGMEDIATOR->OperationPromptInfo(tr("��ת! ��ָ����ת�Ƕ�!"));
        break;

    case setRotateAngle:
        pos -= data.center;
        if ( pos.squared()< RS_TOLERANCE2 ) //δ��������������
		{
            data.angle = 0.;//angle not well defined
        }
		else 
		{
            //data.angle = RS_Math::correctAngle(pos.angle() - data.angle);
			data.angle = pos.angle();
        }

        caculateData();
        trigger();
        finish();
		DIALOGMEDIATOR->OperationPromptInfo(tr("��ת���!"));
		m_bContinueSelt = true;
        break;

    default:
        break;
    }
}

void RS_ActionModifyRotate::caculateData() 
{
    data.useCurrentAttributes = false;
    data.useCurrentLayer = false;
    data.number = 0;
}



void RS_ActionModifyRotate::showOptions() {
    RS_ActionInterface::showOptions();

    DIALOGMEDIATOR->requestOptions(this, true, true);
}



void RS_ActionModifyRotate::hideOptions() {
    RS_ActionInterface::hideOptions();

    DIALOGMEDIATOR->requestOptions(this, false);
}

void RS_ActionModifyRotate::rotateFixed(double x, double y, double r) {
    data.center.x = x;
    data.center.y = y;
    data.angle = RS_Math::deg2rad(r);//�ѽǶ�ת���ɻ�����
    caculateData();
    trigger();
    finish();
}



//void RS_ActionModifyRotate::updateMouseButtonHints() {
//    switch (getStatus()) {
//    case setCenterPoint:
//        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify rotation center"),
//                                            tr("Back"));
//        break;

//    case setReferencePoint:
//        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
//                                            tr("Back"));
//        break;
//    case setTargetPoint:
//        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify target point to rotate to"),
//                                            tr("Back"));
//        break;
//    default:
//        RS_DIALOGFACTORY->updateMouseWidget("", "");
//        break;
//    }
//}



void RS_ActionModifyRotate::updateMouseCursor() {
    //graphicView->setMouseCursor(RS2::CadCursor);
}




void RS_ActionModifyRotate::OnCommit(MyPoint mpost)
{
	RS_Vector pos;
	pos.x = mpost.x;
	pos.y = mpost.y;

	if (!pos.valid ) 
	{
		return;
	}

	switch (getStatus()) 
	{
	case setReferencePoint:
		pos -= data.center;
		data.center = pos;
// 		if ( pos.squared()< RS_TOLERANCE2 ) 
// 		{
// 			//��ֹ�ο���ͻ���ͬһ���㣬������ѧ�������
// 			data.angle = 0.;//angle not well defined, go direct to dialog
// 			QMessageBox::warning(NULL, tr("Warning"), tr("Reference point can not be the same starting point"));
// 		} 
// 		else 
		{
			data.angle = pos.angle();//�����������ĽǶ�
			setStatus(setRotateAngle);
		}

		setStatus(setRotateAngle);
		getDataWindow_Line->setNextLabelText(7);
		DIALOGMEDIATOR->OperationPromptInfo(tr("��ת! ��ָ����ת�Ƕ�!"));
		break;

	case setRotateAngle:
		//pos -= data.center;
		//data.angle = RS_Math::correctAngle(pos.x*M_PI/180);
		if (pos.x >= 360 || pos.x <= -360)
		{
			//pos.x = (pos.x)%360;
			int nTemp = pos.x/360;
			pos.x -= nTemp*360;
		}
		if (pos.x > 180)
		{
			pos.x -= 360;
		}
		if (pos.x < -180)
		{
			pos.x += 360;
		}
		
			
		data.angle = pos.x*M_PI/180;

		caculateData();
		trigger();
		finish();
		DIALOGMEDIATOR->OperationPromptInfo(tr("��ת���!"));
		m_bContinueSelt = true;
		pDemocadSignal->sendSignal_EditFinish("Rotate");
		break;

	default:
		break;
	}

}

void RS_ActionModifyRotate::OnRight(QString f)
{
	getDataWindow_Line->setNextFocus();
}

void RS_ActionModifyRotate::OnLeft(QString f)
{
	getDataWindow_Line->setNextFocus();
}

void RS_ActionModifyRotate::OnEnter(QString f)
{
	getDataWindow_Line->DoInput();
}


// EOF
