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

#include "rs_actionmodifytrim.h"

#include <QAction>
//#include "rs_dialogfactory.h"
#include "rs_graphicview.h"
#include "rs_modification.h"

#include "global_Values.h" //@zhang.y.q

/**
 * @param both Trim both entities.
 */
RS_ActionModifyTrim::RS_ActionModifyTrim(RS_EntityContainer& container,
        RS_GraphicView& graphicView, bool both)
        :RS_PreviewActionInterface("Trim Entity",
                           container, graphicView) {

    trimEntity = NULL;
    trimCoord = RS_Vector(false);
    limitEntity = NULL;
    limitCoord = RS_Vector(false);
    this->both = both;
}

RS_ActionModifyTrim::~RS_ActionModifyTrim()
{
    if (graphicView != NULL/* && graphicView->isCleanUp()==false*/){
        if (limitEntity!= NULL){
            if(limitEntity->isHighlighted()){
                limitEntity->setHighlighted(false);
                graphicView->drawEntity(limitEntity);
            }
        }
    }
}

QAction* RS_ActionModifyTrim::createGUIAction(RS2::ActionType type, QObject* /*parent*/) {
    QAction* action;

        switch (type) {
        default:
        case RS2::ActionModifyTrim:
                // (tr("Trim")
                action = new QAction(tr("&Trim"), NULL);
                action->setIcon(QIcon(":/extui/modifytrim.png"));
            //action->zetStatusTip(tr("Trim Entities"));
                break;

        case RS2::ActionModifyTrim2:
                // tr("Trim Two")
                action = new QAction(tr("&Trim Two"), NULL);
                action->setIcon(QIcon(":/extui/modifytrim2.png"));
        //action->zetStatusTip(tr("Trim two Entities"));
                break;
        }

    return action;
}


void RS_ActionModifyTrim::init(int status) {

    snapMode.clear();
    snapMode.restriction = RS2::RestrictNothing;
    RS_PreviewActionInterface::init(status);

}



//trimEntity ���޼���ʵ��		trimCoord  ѡ���޼�ʵ��ʱ���������Ӧ��ʵ������
//limitEntity ����ʵ��			limitCoord  ѡ������ʵ��ʱ�����������Ӧ��ʵ������
void RS_ActionModifyTrim::trigger() {

	int flage;
	ErrorMessage err;
    if (trimEntity!=NULL && trimEntity->isAtomic() &&
            limitEntity!=NULL /* && limitEntity->isAtomic()*/) {

        RS_Modification m(*container, graphicView);
        flage = m.trim(trimCoord, (RS_AtomicEntity*)trimEntity,
               limitCoord, /*(RS_AtomicEntity*)*/limitEntity,
               both);

		if (1 == flage)//�޽��㣬�޼�ʧ��
		{		
			DIALOGMEDIATOR->OperationPromptInfo(tr("��ʵ���޽��㣬�޼�ʧ�ܣ�������������ѡ�񣬻��һ��˳���"));		
		}
		else if (2 == flage)//�н��㣬�����޼�ʵ��ֻ��һ�Σ��޼�ʧ��
		{
			DIALOGMEDIATOR->OperationPromptInfo(tr("���޼�ʵ��ֻ��һ�Σ����ܱ��޼���������������ѡ�񣬻��һ��˳���"));
		}
		else if (0 == flage)//�޼��ɹ�
		{
			if (both)
			{
				DIALOGMEDIATOR->OperationPromptInfo(tr(" �޼�2! ��Ҫ����ѡ�������ཻ��ʵ��! ��ѡ���һ���ཻʵ��Ĳü�����!"));
			}
			else
			{
				DIALOGMEDIATOR->OperationPromptInfo(tr(" �޼�1! ��ѡ������ʵ��!"));
			}
		}

        trimEntity = NULL;
		if (limitEntity!=NULL) {
			limitEntity->setHighlighted(false);
			graphicView->drawEntity(limitEntity);
		}
		setStatus(ChooseLimitEntity);

        //RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected(),container->totalSelectedLength());
    }
	return;
}



void RS_ActionModifyTrim::mouseMoveEvent(QMouseEvent* e) {

    RS_Vector mouse = graphicView->toGraph(e->x(), e->y());
    RS_Entity* se = catchEntity(e);

    switch (getStatus()) {
    case ChooseLimitEntity:
        limitCoord = mouse;
        limitEntity = se;
        break;

    case ChooseTrimEntity:
        trimCoord = mouse;
        trimEntity = se;
        break;

    default:
        break;
    }

}



void RS_ActionModifyTrim::mouseReleaseEvent(QMouseEvent* e) 
{
    if (e->button()==Qt::LeftButton) 
	{
        RS_Vector mouse = graphicView->toGraph(e->x(), e->y());
        RS_Entity* se = catchEntity(e);

        switch (getStatus()) 
		{
        case ChooseLimitEntity:
            limitCoord = mouse;
            limitEntity = se;
            if (limitEntity!=NULL /*&& limitEntity->isAtomic()*/) 
			{
                limitEntity->setHighlighted(true);
                graphicView->drawEntity(limitEntity);
                setStatus(ChooseTrimEntity);

				if (both)
				{
					DIALOGMEDIATOR->OperationPromptInfo(tr(" �޼�2! ��ѡ����һ���ཻʵ��Ĳü�����!"));
				}
				else
				{
					DIALOGMEDIATOR->OperationPromptInfo(tr(" �޼�1! ��ѡ��ü�ʵ��Ĳü�����!"));
				}
            }
            break;
        case ChooseTrimEntity:
            trimCoord = mouse;
            trimEntity = se;
            if (trimEntity!=NULL && trimEntity->isAtomic()) 
			{
                trigger();

				// ��ɲü�
// 				if (both)
// 				{
// 					DIALOGMEDIATOR->OperationPromptInfo(tr(" �޼�2! ��Ҫ����ѡ�������ཻ��ʵ��! ��ѡ���һ���ཻʵ��Ĳü�����!"));
// 				}
// 				else
// 				{
// 					deletePreview();
// 					if (limitEntity!=NULL) {
// 						limitEntity->setHighlighted(false);
// 						graphicView->drawEntity(limitEntity);
// 					}
// 					init(getStatus()-1);
// 
// 					DIALOGMEDIATOR->OperationPromptInfo(tr(" �޼�1! ��ѡ������ʵ��!"));
// 				}
            }
            break;

        default:
            break;
        }
    } 
	else if (e->button()==Qt::RightButton)
	{
        deletePreview();
		if (limitEntity!=NULL) 
		{
			limitEntity->setHighlighted(false);
			graphicView->drawEntity(limitEntity);
        }

		//Begin Modify @zhang.y.q
		if(getStatus()==0)
		{
			if(both)
			{
				pDemocadSignal->sendSignal_EditFinish("Trim2");
			}		
			else
			{
				pDemocadSignal->sendSignal_EditFinish("Trim");
			}
		}
		//End Modify @zhang.y.q

        init(getStatus()-1);

		if (both)
		{
			DIALOGMEDIATOR->OperationPromptInfo(tr(" �޼�2! ��Ҫ����ѡ�������ཻ��ʵ��! ��ѡ���һ���ཻʵ��Ĳü�����!"));
		}
		else
		{
			DIALOGMEDIATOR->OperationPromptInfo(tr(" �޼�1! ��ѡ������ʵ��!"));
		}
    }
}

//void RS_ActionModifyTrim::finish(bool updateTB) {
//    if (limitEntity->isHighlighted()){
//        limitEntity->setHighlighted(false);
//        graphicView->drawEntity(limitEntity);
//    }
//    RS_PreviewActionInterface::finish(updateTB);
//}

//void RS_ActionModifyTrim::updateMouseButtonHints() {
//    switch (getStatus()) {
//    case ChooseLimitEntity:
//        if (both) {
//            RS_DIALOGFACTORY->updateMouseWidget(tr("Select first trim entity"),
//                                                tr("Cancel"));
//        } else {
//            RS_DIALOGFACTORY->updateMouseWidget(tr("Select limiting entity"),
//                                                tr("Back"));
//        }
//        break;
//    case ChooseTrimEntity:
//        if (both) {
//            RS_DIALOGFACTORY->updateMouseWidget(tr("Select second trim entity"),
//                                                tr("Cancel"));
//        } else {
//            RS_DIALOGFACTORY->updateMouseWidget(tr("Select entity to trim"),
//                                                tr("Back"));
//        }
//        break;
//    default:
//        RS_DIALOGFACTORY->updateMouseWidget("", "");
//        break;
//    }
//}



void RS_ActionModifyTrim::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



//void RS_ActionModifyTrim::updateToolBar() {
//    //not needed any more with new snap
//    return;
//    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
//}


