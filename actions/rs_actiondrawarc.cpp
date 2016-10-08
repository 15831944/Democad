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

#include "rs_actiondrawarc.h"

#include <QAction>
#include <QMessageBox>

#include "rs_graphicview.h"
#include "rs_document.h"
//#include "rs_commands.h"
//#include "rs_commandevent.h"
//#include "rs_dialogfactory.h"

#include "dialogmediator.h"

#include "global_Values.h" //ȫ�ֱ��� @zhang.y.q
#include "Parameter.h"
#include "ErrorMessage.h"

RS_ActionDrawArc::RS_ActionDrawArc(RS_EntityContainer& container,
                                   RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw arcs",
                           container, graphicView) {

    reset();
	//Begin Modify @
	getDataWindow_Line=new getData_Line(gMainWindow,1);
	getDataWindow_Line->show();
	getDataWindow_Line->setFocus();
	//getDataWindow_Line->move(320,390);
	getDataWindow_Line->move(80, 43);
	g_bIsResponseMouse = true;//Ĭ�Ͽ�����Ӧ����¼�
	getDataWindow_Line->installEventFilter(this);

	connect(getDataWindow_Line,SIGNAL(commit(MyPoint)),this,SLOT(OnCommit(MyPoint)));
	connect(getDataWindow_Line,SIGNAL(Right(QString)),this,SLOT(OnRight(QString)));
	connect(getDataWindow_Line,SIGNAL(Left(QString)),this,SLOT(OnLeft(QString)));
	connect(getDataWindow_Line,SIGNAL(Enter(QString)),this,SLOT(OnEnter(QString)));
	//End Modify @
}

void RS_ActionDrawArc::OnCommit(MyPoint mpost){//@zhang.y.q
	RS_Vector snapped = RS_Vector(mpost.x,mpost.y,0);
	if (getStatus() == SetRadius)
	{
		snapped = RS_Vector(data.center.x+mpost.x,data.center.y,0);
	}

	RS_CoordinateEvent ce(snapped);
	coordinateEvent(&ce);//����ÿ�εĵ��
	
	getDataWindow_Line->setFocus();
	
	//getDataWindow_Line->setNextLabelText(2);


// 	/*����һ��MOVE���Ķ���*/
// 	if (getStatus()==SetEndpoint && data.startpoint.valid) {
// 		deletePreview();
// 		preview->addEntity(new RS_Line(preview,
// 			RS_LineData(data.startpoint, curMousePoint)));
// 		drawPreview();
// 	}
	/*end*/
}


RS_ActionDrawArc::~RS_ActionDrawArc() 
{
	if (getDataWindow_Line != NULL)
	{
		delete getDataWindow_Line;
		getDataWindow_Line=NULL;
	}
}

QAction* RS_ActionDrawArc::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        // (tr("Arc: Center, Point, Angles")
    QAction* action = new QAction(tr("&Center, Point, Angles"),NULL);
        action->setIcon(QIcon(":/extui/arcscraa.png"));
    //action->zetStatusTip(tr("Draw arcs"));
    return action;
}


void RS_ActionDrawArc::reset() {
    //bool rev = data.reversed;

    if (data.reversed) {
        data = RS_ArcData(RS_Vector(false),
                          0.0,
                          2*M_PI, 0.0,
                          true);
    } else {
        data = RS_ArcData(RS_Vector(false),
                          0.0,
                          0.0, 2*M_PI,
                          false);
    }
}



void RS_ActionDrawArc::init(int status) {
    RS_PreviewActionInterface::init(status);

    reset();
}



void RS_ActionDrawArc::trigger() 
{
    RS_PreviewActionInterface::trigger();

    RS_Arc* arc = new RS_Arc(container, data);

	//arc->setEntityType("Arc");//@zhang.y.q
    //arc->setLayerToActive();

    arc->setPenToActive();
    container->addEntity(arc);

    // upd. undo list:
    if (document!=NULL) 
	{
        document->startUndoCycle();
        document->addUndoable(arc);
        document->endUndoCycle();
    }

    graphicView->redraw(RS2::RedrawDrawing);
    graphicView->moveRelativeZero(arc->getCenter());

    setStatus(SetCenter);
    reset();

}

void RS_ActionDrawArc::keyPressEvent(QKeyEvent* e){
	//NULL	
}
void RS_ActionDrawArc::OnRight(QString f){
	getDataWindow_Line->setNextFocus();
}

void RS_ActionDrawArc::OnLeft(QString f){
	getDataWindow_Line->setNextFocus();
}

void RS_ActionDrawArc::OnEnter(QString f){
	getDataWindow_Line->DoInput();
}

void RS_ActionDrawArc::mouseMoveEvent(QMouseEvent* e) 
{
    RS_Vector mouse = snapPoint(e);

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

	if (getStatus() == SetRadius)
	{
		getDataWindow_Line->setX(data.center.distanceTo(mouse));//added by wang.k.h
		getDataWindow_Line->Redisplay(4);
	}

	if(getStatus() == SetCenter)
	{
		getDataWindow_Line->setX(mouse.x);
		getDataWindow_Line->setY(mouse.y);
		getDataWindow_Line->Redisplay(0);
	}
	getDataWindow_Line->move(X, Y);
	//getDataWindow_Line->Redisplay();

    switch (getStatus()) {
    case SetCenter:
        data.center = mouse;
        break;

    case SetRadius:
        if (data.center.valid) {
            data.radius = data.center.distanceTo(mouse);
            deletePreview();
            preview->addEntity(new RS_Circle(preview,
                                             RS_CircleData(data.center,
                                                           data.radius)));
            drawPreview();
        }
        break;

    case SetAngle1:
        data.angle1 = data.center.angleTo(mouse);
        if (data.reversed) {
            data.angle2 = RS_Math::correctAngle(data.angle1-M_PI/3);
        } else {
            data.angle2 = RS_Math::correctAngle(data.angle1+M_PI/3);
        }
        deletePreview();
        preview->addEntity(new RS_Arc(preview,
                                      data));
        drawPreview();
        break;

    case SetAngle2:
        data.angle2 = data.center.angleTo(mouse);
        deletePreview();
        preview->addEntity(new RS_Arc(preview,
                                      data));
        drawPreview();
        break;

    case SetIncAngle:
        data.angle2 = data.angle1 + data.center.angleTo(mouse);
        deletePreview();
        preview->addEntity(new RS_Arc(preview,
                                      data));
        drawPreview();
        break;

    case SetChordLength: {
            double x = data.center.distanceTo(mouse);
            if (fabs(x/(2*data.radius))<=1.0) {
                data.angle2 = data.angle1 + asin(x/(2*data.radius)) * 2;
                deletePreview();
                preview->addEntity(new RS_Arc(preview,
                                              data));
                drawPreview();
            }
        }
        break;

    default:
        break;

    }

}



void RS_ActionDrawArc::mouseReleaseEvent(QMouseEvent* e) 
{
    if (e->button()==Qt::LeftButton) 
	{
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);

    }
	else if (e->button()==Qt::RightButton) 
	{
        deletePreview();

		//@zhang.y.q getStatus() ==0 ˵��û�ˡ�
		//Begin Modify @zhang.y.q 
		getDataWindow_Line->Init();

		if(getStatus()!=0)
		{
			getDataWindow_Line->setNextLabelText(2);
		}

		/*��getStatus()==-1��ʱ�����źű�ʾ��ͼ����*/
		if(getStatus()==0)
		{
			pDemocadSignal->sendSignal_EditFinish("Arc");
			g_pQPublicSignal->DemoCadReset();

		}
		//End Modify @zhang.y.q

        init(getStatus()-1);
    }
}

void RS_ActionDrawArc::coordinateEvent(RS_CoordinateEvent* e) 
{
    if (e==NULL) 
	{
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) 
	{
    case SetCenter:
        data.center = mouse;
        graphicView->moveRelativeZero(mouse);
        setStatus(SetRadius);
		getDataWindow_Line->setNextLabelText(4);//Modify @wang.k.h

		DIALOGMEDIATOR->OperationPromptInfo(tr("����Բ��! ���ƶ����(������/�ҷ������ͨ����������)Բ���뾶!"));
        break;

    case SetRadius:
        if (data.center.valid) 
		{
            data.radius = data.center.distanceTo(mouse);
        
			if (data.radius > 0.000001)      //added by wang.k.h
			{
				getDataWindow_Line->hide();
// 				ErrorMessage err;
// 				err.showErrorMessage(15001,2);
// 				QMessageBox::information(this, "Error!", "ȷ�������յ㣡",
// 				QMessageBox::Ok, QMessageBox::Ok);
				setStatus(SetAngle1);
				getDataWindow_Line->setNextLabelText(0);

				DIALOGMEDIATOR->OperationPromptInfo(tr("����Բ��! ���ƶ����󵥻����ȷ��Բ���������!"));
			}
			else 
			{
				//�뾶Ϊ0ʱ��Ӧ�ñ�������ʾ�����趨�뾶  added by wang.k.h
				ErrorMessage err;
				err.showErrorMessage(G_NDEMO_CAD_ERR_NUM+1,1);
// 				QMessageBox::information(this, "Error!", "Բ���뾶����Ϊ0��\n������ȷ���뾶��",
// 				QMessageBox::Ok, QMessageBox::Ok);
				setStatus(SetRadius);
				getDataWindow_Line->setNextLabelText(4);

				DIALOGMEDIATOR->OperationPromptInfo(tr("����Բ������: Բ���뾶����Ϊ0! ������ȷ���뾶!"));
			}
		}
        break;

    case SetAngle1:
        data.angle1 = data.center.angleTo(mouse);
        setStatus(SetAngle2);
		//getDataWindow_Line->setNextLabelText(3);//Modify @zhang.y.q
		DIALOGMEDIATOR->requestOptions(this, true, true);
		//DIALOGMEDIATOR->OperationPromptInfo(tr("����Բ��! ���ƶ����󵥻����ȷ��Բ���յ�����!"));
        break;

    case SetAngle2:
		if (data.center.valid /*&& data.angle1*/ && data.radius)
		{
			data.angle2 = data.center.angleTo(mouse);
			//Բ�������յ��غ�ʱ��Ӧ��������ʾ�����趨�յ�
			if (data.angle1 != data.angle2)    //added by wang.k.h
			{
				trigger();
				DIALOGMEDIATOR->OperationPromptInfo(tr("����Բ���ɹ�! ���������뵥��������������/�ҷ������ͨ����������Բ������! ȡ����˫���Ҽ�!"));
				getDataWindow_Line->show();
				getDataWindow_Line->setNextLabelText(2);//Modify @zhang.y.q
				DIALOGMEDIATOR->requestArcOptions(NULL, false, false);
			}
			else
			{
				ErrorMessage err;
				err.showErrorMessage(15002,1);
	// 			QMessageBox::information(this, "Error!", "Բ���յ����㲻���غϣ�\n������ȷ���յ㣡",
	// 				QMessageBox::Ok, QMessageBox::Ok);
				setStatus(SetAngle2);
					//getDataWindow_Line->setNextLabelText(3);//Modify @

				DIALOGMEDIATOR->OperationPromptInfo(tr("����Բ������: Բ���յ����㲻���غ�! ������ȷ���յ�!"));
			}

		}
        break;

    case SetIncAngle:
        data.angle2 = data.angle1 + data.center.angleTo(mouse);
        trigger();
        break;

    case SetChordLength: 
		{
            double x = data.center.distanceTo(mouse);

            if (fabs(x/(2*data.radius))<=1.0) 
			{
                data.angle2 = data.angle1 + asin(x/(2*data.radius)) * 2;
                trigger();
            }

        }
        break;

    default:
        break;
    }

}

void RS_ActionDrawArc::slotNext(RS_Vector start, RS_Vector end, double radius) 
{
    RS_ArcData tdata;
//    RS_Vector center;//�е�
//    double radius;//�뾶
//    double angle1;//���
//    double angle2;//�ս�
//    bool reversed;//���ƻ��ķ����Ƿ���

    RS_Arc* arc = new RS_Arc(container, data);
    arc->setPenToActive();
    container->addEntity(arc);

    // upd. undo list:
    if (document!=NULL) {
        document->startUndoCycle();
        document->addUndoable(arc);
        document->endUndoCycle();
    }

    graphicView->redraw(RS2::RedrawDrawing);

}



//void RS_ActionDrawArc::commandEvent(RS_CommandEvent* e) {
//    QString c = e->getCommand().toLower();

//    if (RS_COMMANDS->checkCommand("help", c)) {
//        if (RS_DIALOGFACTORY!=NULL) {
//            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
//                                             + getAvailableCommands().join(", "));
//        }
//        return;
//    }

//    if (RS_COMMANDS->checkCommand("reversed", c)) {
//        e->accept();
//        setReversed(!isReversed());

//        if (RS_DIALOGFACTORY!=NULL) {
//            RS_DIALOGFACTORY->requestOptions(this, true, true);
//        }
//        return;
//    }

//    switch (getStatus()) {

//    case SetRadius: {
//            bool ok;
//            double r = RS_Math::eval(c, &ok);
//            if (ok==true) {
//                data.radius = r;
//                setStatus(SetAngle1);
//            } else {
//                if (RS_DIALOGFACTORY!=NULL) {
//                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
//                }
//            }
//        }
//        break;

//    case SetAngle1: {
//            bool ok;
//            double a = RS_Math::eval(c, &ok);
//            if (ok==true) {
//                data.angle1 = RS_Math::deg2rad(a);
//                setStatus(SetAngle2);
//            } else {
//                if (RS_DIALOGFACTORY!=NULL) {
//                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
//                }
//            }
//        }
//        break;

//    case SetAngle2: {
//            if (RS_COMMANDS->checkCommand("angle", c)) {
//                setStatus(SetIncAngle);
//            } else if (RS_COMMANDS->checkCommand("chord length", c)) {
//                setStatus(SetChordLength);
//            } else {
//                bool ok;
//                double a = RS_Math::eval(c, &ok);
//                if (ok==true) {
//                    data.angle2 = RS_Math::deg2rad(a);
//                    trigger();
//                } else {
//                    if (RS_DIALOGFACTORY!=NULL) {
//                        RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
//                    }
//                }
//            }
//        }
//        break;

//    case SetIncAngle: {
//            bool ok;
//            double a = RS_Math::eval(c, &ok);
//            if (ok==true) {
//                data.angle2 = data.angle1 + RS_Math::deg2rad(a);
//                trigger();
//            } else {
//                if (RS_DIALOGFACTORY!=NULL) {
//                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
//                }
//            }
//        }
//        break;

//    case SetChordLength: {
//            bool ok;
//            double l = RS_Math::eval(c, &ok);
//            if (ok==true) {
//                if (fabs(l/(2*data.radius))<=1.0) {
//                    data.angle2 = data.angle1 + asin(l/(2*data.radius)) * 2;
//                    trigger();
//                } else {
//                    if (RS_DIALOGFACTORY!=NULL) {
//                        RS_DIALOGFACTORY->commandMessage(
//                            tr("Not a valid chord length"));
//                    }
//                }
//            } else {
//                if (RS_DIALOGFACTORY!=NULL) {
//                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
//                }
//            }
//        }
//        break;

//    default:
//        break;
//    }
//}



//QStringList RS_ActionDrawArc::getAvailableCommands() {
//    QStringList cmd;
//    cmd += RS_COMMANDS->command("reversed");
//    return cmd;
//}


//void RS_ActionDrawArc::updateMouseButtonHints() {
//    if (RS_DIALOGFACTORY!=NULL) {
//        switch (getStatus()) {
//        case SetCenter:
//            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify center"), tr("Cancel"));
//            break;
//        case SetRadius:
//            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify radius"), tr("Back"));
//            break;
//        case SetAngle1:
//            RS_DIALOGFACTORY->updateMouseWidget(
//                tr("Specify start angle:"), tr("Back"));
//            break;
//        case SetAngle2:
//            RS_DIALOGFACTORY->updateMouseWidget(
//                tr("Specify end angle or [angle/chord length]"),
//                tr("Back"));
//            break;
//        case SetIncAngle:
//            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify included angle:"),
//                                                tr("Back"));
//            break;
//        case SetChordLength:
//            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify chord length:"),
//                                                tr("Back"));
//            break;
//        default:
//            RS_DIALOGFACTORY->updateMouseWidget("", "");
//            break;
//        }
//    }
//}



void RS_ActionDrawArc::showOptions() {
    RS_ActionInterface::showOptions();

 //   DIALOGMEDIATOR->requestOptions(this, true, true);
}



void RS_ActionDrawArc::hideOptions() {
    RS_ActionInterface::hideOptions();

 //   DIALOGMEDIATOR->requestOptions(this, false);
}



void RS_ActionDrawArc::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



//void RS_ActionDrawArc::updateToolBar() {
//    if (RS_DIALOGFACTORY!=NULL) {
//        if (isFinished()) {
//            RS_DIALOGFACTORY->resetToolBar();
//        }else{
//            RS_DIALOGFACTORY->showCadToolBar(rtti());
//        }
//    }
//}


// EOF

