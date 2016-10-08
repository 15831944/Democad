/****************************************************************************
**
**
**********************************************************************/

#include "rs_actionassignr.h"
#include <QAction>
#include "rs_graphicview.h"
#include "rs_document.h"
#include "global_Operator.h"

RS_ActionAssignR::RS_ActionAssignR(RS_EntityContainer& container,RS_GraphicView& graphicView, 
	bool bAR = false, double dAR = 0.0):RS_PreviewActionInterface("Draw arcs",container, graphicView) 
{
	bAssignR = bAR;
	dAssignR = dAR;
    reset();
}

RS_ActionAssignR::~RS_ActionAssignR() 
{

}

void RS_ActionAssignR::reset() 
{
    return;
}

void RS_ActionAssignR::init(int status) 
{
    RS_PreviewActionInterface::init(status);
    reset();
}

void RS_ActionAssignR::trigger() 
{
    RS_PreviewActionInterface::trigger();
	reset();
}

void RS_ActionAssignR::mouseMoveEvent(QMouseEvent* e) 
{

   // RS_Vector mouse = snapPoint(e);

}

void RS_ActionAssignR::mouseDoubleClickEvent(QMouseEvent* e) 
{
	// ���λ��
	RS_Vector MousePos = graphicView->toGraph(RS_Vector(e->x(), e->y()));

	if (e->button()==Qt::LeftButton) 
	{
		RS_Entity* en = catchEntity(e, RS2::ResolveAll);

		if (en != NULL)
		{

			// ȡ�����λ���������յ������ĵ���������ָ��R
			double ds = MousePos.distanceTo(en->getStartpoint());
			double de = MousePos.distanceTo(en->getEndpoint());

			if (ds < de)
			{
				RS_Entity* penNear     = NULL;		// ʵ��en���ڵ�ʵ��

				getNearEntity(en, &penNear);

				if (penNear != NULL && (penNear->rtti()==RS2::EntityLine || 
					penNear->rtti()==RS2::EntityArc || penNear->rtti()==RS2::EntityCircle))
				{
					graphicView->redraw(RS2::RedrawDrawing);

				}
			}
			else
			{
				if (en->rtti()==RS2::EntityLine || en->rtti()==RS2::EntityArc || en->rtti()==RS2::EntityCircle)
				{
					RS_Entity* penNear = NULL;		// ʵ��en���ڵ�ʵ��

					getNearEntity(en, &penNear, false);

					if (penNear != NULL)
					{
						graphicView->redraw(RS2::RedrawDrawing);
					}

				}
			}

		}

	} 
	else if (e->button()==Qt::RightButton) 
	{
		deletePreview();
		init(getStatus()-1);
	}

}

void RS_ActionAssignR::mouseReleaseEvent(QMouseEvent* e) 
{
	// ���λ��
	RS_Vector MousePos = graphicView->toGraph(RS_Vector(e->x(), e->y()));

    if (e->button()==Qt::LeftButton) 
	{
		RS_Entity* en = catchEntity(e, RS2::ResolveAll);

		if (en != NULL)
		{

			// ȡ�����λ���������յ������ĵ���������ָ��R
			double ds = MousePos.distanceTo(en->getStartpoint());
			double de = MousePos.distanceTo(en->getEndpoint());

			if (ds < de)
			{
				RS_Entity* penNear     = NULL;		// ʵ��en���ڵ�ʵ��

				getNearEntity(en, &penNear);

				if (penNear != NULL && (penNear->rtti()==RS2::EntityLine || 
					penNear->rtti()==RS2::EntityArc || penNear->rtti()==RS2::EntityCircle))
				{
					graphicView->redraw(RS2::RedrawDrawing);

				}
			}
			else
			{
				if (en->rtti()==RS2::EntityLine || en->rtti()==RS2::EntityArc || en->rtti()==RS2::EntityCircle)
				{
					RS_Entity* penNear = NULL;		// ʵ��en���ڵ�ʵ��

					getNearEntity(en, &penNear, false);

					if (penNear != NULL)
					{
						graphicView->redraw(RS2::RedrawDrawing);
					}

				}
			}

		}

	} 
	else if (e->button()==Qt::RightButton) 
	{
        deletePreview();
        init(getStatus()-1);
    }
	
}


void RS_ActionAssignR::coordinateEvent(RS_CoordinateEvent* e) 
{
    if (e==NULL)
	{
        return;
    }

    RS_Vector mouse = e->getCoordinate();
}

void RS_ActionAssignR::showOptions() 
{
    RS_ActionInterface::showOptions();
    DIALOGMEDIATOR->requestOptions(this, true, true);
}

void RS_ActionAssignR::hideOptions() 
{
    RS_ActionInterface::hideOptions();
    DIALOGMEDIATOR->requestOptions(this, false);
}

void RS_ActionAssignR::updateMouseCursor()
{
    graphicView->setMouseCursor(RS2::ArrowCursor);
}

/************************************************************************
* Description:
*	��ȡ�����ʵ��(pRS_Entity)����ʵ��(pRS_Entity)
* Parameters:
*	RS_Entity* pRS_Entity     : ����ʵ�� 
*	RS_Entity** pRS_EntityNear: ���������ʵ��(pRS_Entity)�����յ���ȵ�����ʵ��
*	bool bDir                 : true,���ڵ�ǰһ��ʵ��(pre),Ĭ��Ϊtrue��false,���ڵĺ�һ��ʵ��(next)
* Return Values:
*	void
************************************************************************/
void RS_ActionAssignR::getNearEntity(RS_Entity* pRS_Entity, RS_Entity** pRS_EntityNear, bool bDir)
{
	if (NULL == pRS_Entity)
	{
		return;
	}

	RS_Vector Point;
	bool bDifShape = false;			// �Ƿ�Ϊ��������

	bDir ? (Point = pRS_Entity->getStartpoint()):(Point = pRS_Entity->getEndpoint());

	for (RS_Entity* e = container->firstEntity(); e != NULL; e = container->nextEntity()) 
	{
		if (e == pRS_Entity)
		{
			continue;
		}

		RS_Vector judgePoint;

		bDir ? (judgePoint = e->getEndpoint()):(judgePoint = e->getStartpoint());

		if (judgeTwoPointEqual(Point, judgePoint, 0.00001))
		{
			(*pRS_EntityNear) = e;
			
			if (bDifShape)		// ��������,���ж�һ�Σ���ǰѡ���ı��Ƿ���ͼ�������һ����
			{
				for (RS_Entity* e = container->firstEntity(); e != NULL; e = container->nextEntity()) 
				{
					if (e == pRS_Entity)
					{
						continue;
					}

					RS_Vector judgePoint;

					bDir ? (judgePoint = e->getStartpoint()):(judgePoint = e->getEndpoint());

					if (judgeTwoPointEqual(Point, judgePoint, 0.00001))
					{
						(*pRS_EntityNear) = NULL;
						return;
					}

				}
			}

			break;
		}
	}

}

