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

#include "rs_actionshowangel.h"

#include <QAction>
#include <QToolTip>
#include "rs_actioneditundo.h"
//#include "rs_dialogfactory.h"
#include "rs_graphicview.h"
#include "rs_document.h"

#include "dialogmediator.h"
#include "rs_selection.h"

#include "rs_information.h"

RS_ActionShowAngel::RS_ActionShowAngel(RS_EntityContainer& container,
	RS_GraphicView& graphicView)
	:RS_PreviewActionInterface("Show angel",
	container, graphicView) {

		RS_DEBUG->print("RS_ActionShowAngel::RS_ActionShowAngel");
		RS_DEBUG->print("RS_ActionShowAngel::RS_ActionShowAngel: OK");

		trimEntity = NULL;
		trimCoord = RS_Vector(false);
		limitEntity = NULL;
		limitCoord = RS_Vector(false);
		m_angel = 0.0;

}

RS_ActionShowAngel::~RS_ActionShowAngel() {
	if (graphicView != NULL/* && graphicView->isCleanUp()==false*/){
		if (limitEntity!=NULL) {
			limitEntity->setHighlighted(false);
			graphicView->drawEntity(limitEntity);
		}
		if (trimEntity!=NULL) {
			trimEntity->setHighlighted(false);
			graphicView->drawEntity(trimEntity);
		}
	}
}

QAction* RS_ActionShowAngel::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
	//tr("Line: 2 Points")
	QAction* action = new QAction(tr("Show the angel between two crossed lines"), NULL);
	action->setIcon(QIcon(":/extui/linesnormal.png"));
	//action->zetStatusTip(tr("Draw lines"));
	return action;
}

void RS_ActionShowAngel::init(int status) {
	RS_PreviewActionInterface::init(status);
}



void RS_ActionShowAngel::mouseMoveEvent(QMouseEvent* e) {
	// 	RS_Vector mouse = graphicView->toGraph(e->x(), e->y());
	// 	RS_Entity* se = catchEntity(e);
	// 
	// 	switch (getStatus()) {
	// 	case SetFirstEntity:
	// 		limitCoord = mouse;
	// 		limitEntity = se;
	// 		break;
	// 
	// 	case SetSecondEntity:
	// 		trimCoord = mouse;
	// 		trimEntity = se;
	// 		break;
	// 
	// 	default:
	// 		break;
	// 	}
}

void RS_ActionShowAngel::mouseReleaseEvent(QMouseEvent* e) 
{
	if (e == NULL)
	{
		return;
	}

	RS_Vector rw_mouse = RS_Vector(e->globalX(), e->globalY());
	
	if (e->button()==Qt::LeftButton) 
	{
		RS_Vector mouse = graphicView->toGraph(e->x(), e->y());
		RS_Entity* se = catchEntity(e);

		switch (getStatus()) 
		{
		case SetFirstEntity:
			limitCoord = mouse;
			limitEntity = se;
			if (limitEntity!=NULL && limitEntity->rtti()==RS2::EntityLine) 
			{
				limitEntity->setHighlighted(true);
				graphicView->drawEntity(limitEntity);
				setStatus(SetSecondEntity);

				DIALOGMEDIATOR->OperationPromptInfo(tr("������Ҫ�ؼ�ĽǶ�! �뵥��������ѡ���ڶ���Ҫ��!"));
			}
			break;

		case SetSecondEntity:
			trimCoord = mouse;
			if (se != limitEntity)
			{
				trimEntity = se;
				if (trimEntity!=NULL 
					&& trimEntity->isAtomic()
					&& trimEntity->rtti()==RS2::EntityLine) 
				{
					trimEntity->setHighlighted(true);
					graphicView->drawEntity(trimEntity);
					trigger(rw_mouse);
					setStatus(TriggerStatus);

					DIALOGMEDIATOR->OperationPromptInfo(tr("��Ҫ�ؼ�ĽǶȲ������! ���������뵥������Ҽ����ٴ�ѡ����һ��Ҫ��! ȡ����˫������Ҽ�!"));
				}
			}
			break;
		case TriggerStatus:
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

		if (trimEntity!=NULL) 
		{
			trimEntity->setHighlighted(false);
			graphicView->drawEntity(trimEntity);
		}

		if (getStatus() == TriggerStatus)
			init(getStatus()-1);

		init(getStatus()-1);
	}
}

//void RS_ActionShowAngel::coordinateEvent(RS_CoordinateEvent* e) {
//	if (e==NULL) {
//		RS_DEBUG->print("RS_ActionShowAngel::coordinateEvent: event was NULL");
//		return;
//	}
//
//	if (container==NULL) {
//		RS_DEBUG->print(RS_Debug::D_WARNING,
//			"RS_Modification::move: no valid container");
//		return ;
//	}
//
//	RS_Vector startpos;
//	RS_Vector endpos;
//
//	for (RS_Entity* e=container->firstEntity();
//			e!=NULL;
//			e=container->nextEntity()) {
//				if (e!=NULL && e->isSelected()) {
//					RS_Entity* ec = e->clone();
//
//					startpos = ec->getStartpoint();
//					endpos = ec->getEndpoint();
//
//					emit s_sendshowposofTP(startpos, endpos);
//		}
//	}
//}

void RS_ActionShowAngel::showOptions() {
	RS_ActionInterface::showOptions();

	DIALOGMEDIATOR->requestOptions(this, true, true);
}

void RS_ActionShowAngel::hideOptions() {
	RS_ActionInterface::hideOptions();

	DIALOGMEDIATOR->requestOptions(this, false);
}

//void RS_ActionShowAngel::updateMouseCursor() {
//	if(graphicView != NULL){
//		graphicView->setMouseCursor(RS2::CadCursor);
//	}
//}

void RS_ActionShowAngel::trigger(RS_Vector rw_mouse) 
{
	if (trimEntity!=NULL && trimEntity->isAtomic() &&
		limitEntity!=NULL /* && limitEntity->isAtomic()*/) 
	{
		RS_Line outline1, outline2;
		m_angel = showAngel(&outline1, &outline2);

		deletePreview();
		RS_Line *line1 = new RS_Line(preview, outline1.getData());
		line1->setPen(RS_Pen(RS_Color(255, 255, 0), RS2::WidthByLayer, RS2::SolidLine)); //RS2::BorderLine2
		preview->addEntity(line1);
		RS_Line *line2 = new RS_Line(preview, outline2.getData());
		line2->setPen(RS_Pen(RS_Color(255, 255, 0), RS2::WidthByLayer, RS2::SolidLine)); //RS2::BorderLine2
		preview->addEntity(line2);
		drawPreview();

		if (m_angel >= 180.0)
		{
			m_angel = 360 - m_angel;
		}
			
		// ��QToolTip����ʾ�Ƕ�ֵ 
		QString qstrShow = QString(tr("�Ƕ�ֵΪ:%1")).arg(m_angel);
		QPoint qPmouse;
		qPmouse.setX(rw_mouse.x);
		qPmouse.setY(rw_mouse.y);
		QToolTip::showText(qPmouse, qstrShow);
		//emit s_sendAngel(m_angel);

	}
}


//trimEntity ���޼���ʵ��		trimCoord  ѡ���޼�ʵ��ʱ���������Ӧ��ʵ������
//limitCoord ����ʵ��			limitCoord  ѡ������ʵ��ʱ�����������Ӧ��ʵ������
double RS_ActionShowAngel::showAngel(RS_Line *outline1, RS_Line *outline2) {

		if (trimEntity==NULL || limitEntity==NULL || outline1 == NULL || outline2 == NULL
			|| trimEntity->rtti() != RS2::EntityLine || limitEntity->rtti() != RS2::EntityLine) {
			RS_DEBUG->print(RS_Debug::D_WARNING,
				"RS_Modification::trim: At least one entity is NULL");
			return -1;
		}

		//���߽���
		RS_VectorSolutions sol;
		sol = RS_Information::getIntersection(trimEntity, limitEntity, false);

		//ƽ���ߣ����н�Ϊ0��
		if (sol.hasValid()==false) {
			return 0.0;
		}

		RS_Line* trimmed1 = NULL;
		RS_Line* trimmed2 = NULL;

		trimmed1 = static_cast<RS_Line*>(trimEntity->clone());//��¡һ�±��޽�ʵ��
 		trimEntity->setHighlighted(false);

		// trim trim entity
		int ind = 0;
		RS_Vector is, is2;
		//���뽻�㼯�ϣ���ѡ���޼�ʵ��ʱ���������꣬׼���޼�
		//�����н�����ѡ���ü���
		is = trimmed1->prepareTrim(trimCoord, sol);//ע���ǿ�¡���Ǹ�ʵ��

		// remove limit entity from view:
		trimmed2 = static_cast<RS_Line*>(limitEntity->clone());//��¡һ������ʵ��
 		limitEntity->setHighlighted(false);

		//����ü��㣬��ѡ���޽�ʵ����������꣬���ݽǶȹ�ϵ
		//�ж��ǲü� ��㵽�ü��� �����յ㵽�ü���
		RS2::Ending ending = trimmed1->getTrimPoint(trimCoord, is);
		switch (ending) {
		case RS2::EndingStart:
			trimmed1->trimStartpoint(is);//���Ǹı�ʵ�����ݣ��������ݵ�����
			break;
		case RS2::EndingEnd:
			trimmed1->trimEndpoint(is);//ͬ��
			trimmed1->reverse();//���ý���Ϊ��ʼ��
			break;
		default:
			break;
		}
		double angel1 = trimmed1->getDirection1();

		// trim limit entity:
		RS_Vector is3 = sol.getClosest(limitCoord);

		//ͬ��������ʵ�������㵽�ü���  ����  �յ㵽�ü���
		ending = trimmed2->getTrimPoint(limitCoord, is3);
		switch (ending) {
		case RS2::EndingStart:
			trimmed2->trimStartpoint(is3);//���Ǹı�ʵ�����ݣ��������ݵ�����
			break;
		case RS2::EndingEnd:
			trimmed2->trimEndpoint(is3);//ͬ��
			trimmed2->reverse();//���ý���Ϊ��ʼ��
			break;
		default:
			break;
		}
		double angel2 = trimmed2->getDirection1();

		*outline1 = *trimmed1;
		*outline2 = *trimmed2;
		delete trimmed1;
		delete trimmed2;
		return fabs(angel1 - angel2)*180/PI;
}


// EOF
