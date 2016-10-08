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
#include <QContextMenuEvent>
#include <QToolBar>

#include "qg_snaptoolbar.h"
#include "dialogmediator.h"

/*
 *  Constructs a QG_CadToolBarSnap as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
QG_SnapToolBar::QG_SnapToolBar( const QString & title, QG_ActionHandler* ah, QWidget * parent )
    : QToolBar(title, parent)
,m_iActionCounts(0)
{
    actionHandler=ah;
    actionHandler->setSnapToolBar(this);
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
QG_SnapToolBar::~QG_SnapToolBar()
{
    //@Save default snap mode to prefrences.
    //never being called
    saveSnapMode();
    // no need to delete child widgets, Qt does it all for us
}

void QG_SnapToolBar::saveSnapMode()
{

}

void QG_SnapToolBar::setSnaps ( RS_SnapMode s )
{
    snapFree->setChecked(s.snapFree);
    snapGrid->setChecked(s.snapGrid);
    snapEnd->setChecked(s.snapEndpoint);
    snapOnEntity->setChecked(s.snapOnEntity);
    snapCenter->setChecked(s.snapCenter);
    snapMiddle->setChecked(s.snapMiddle);
    snapDistance->setChecked(s.snapDistance);
    snapIntersection->setChecked(s.snapIntersection);

}

RS_SnapMode QG_SnapToolBar::getSnaps ( void )
{
    RS_SnapMode s;

    s.snapFree         = snapFree->isChecked();
    s.snapGrid         = snapGrid->isChecked();
    s.snapEndpoint     = snapEnd->isChecked();
    s.snapOnEntity     = snapOnEntity->isChecked();
    s.snapCenter       = snapCenter->isChecked();
    s.snapMiddle       = snapMiddle->isChecked();
    s.snapDistance     = snapDistance->isChecked();
    s.snapIntersection = snapIntersection->isChecked();

    return s;
}

void QG_SnapToolBar::init()
{
    snapFree = new QAction(QIcon(":/extui/snapfree.png"), tr("���ɲ�׽"), this);
    snapFree->setCheckable(true);
    connect(snapFree, SIGNAL(triggered()), this, SLOT(actionTriggeredSnapFree()));
    m_vSnapActions<<snapFree;
    this->addAction(snapFree);
    snapGrid = new QAction(QIcon(":/extui/snapgrid.png"), tr("����㲶׽"), this);
    snapGrid->setCheckable(true);
    connect(snapGrid, SIGNAL(triggered()), this, SLOT(actionTriggeredSnapGrid()));
    m_vSnapActions<<snapGrid;
    this->addAction(snapGrid);
    snapEnd = new QAction(QIcon(":/extui/snapendpoint.png"), tr("�˵㲶׽"), this);
    snapEnd->setCheckable(true);
    connect(snapEnd, SIGNAL(triggered()), this, SLOT(actionTriggeredSnapEndpoint()));
    m_vSnapActions<<snapEnd;
    this->addAction(snapEnd);
    snapOnEntity = new QAction(QIcon(":/extui/snaponentity.png"), tr("ʵ���ϵ㲶׽"), this);
    snapOnEntity->setCheckable(true);
    connect(snapOnEntity, SIGNAL(triggered()), this, SLOT(actionTriggeredSnapOnEntity()));
    m_vSnapActions<<snapOnEntity;
    this->addAction(snapOnEntity);
    snapCenter = new QAction(QIcon(":/extui/snapcenter.png"), tr("Բ�Ĳ�׽"), this);
    snapCenter->setCheckable(true);
    connect(snapCenter, SIGNAL(triggered()), this, SLOT(actionTriggeredSnapCenter()));
    m_vSnapActions<<snapCenter;
    this->addAction(snapCenter);
    snapMiddle = new QAction(QIcon(":/extui/snapmiddle.png"), tr("�ȷֵ㲶׽"), this);
    snapMiddle->setCheckable(true);
    connect(snapMiddle, SIGNAL(triggered()), this, SLOT(actionTriggeredSnapMiddle()));
    m_vSnapActions<<snapMiddle;
    this->addAction(snapMiddle);
    snapDistance = new QAction(QIcon(":/extui/snapdist.png"), tr("�Ⱦ�㲶׽"), this);
    snapDistance ->setCheckable(true);
    connect(snapDistance, SIGNAL(triggered()), this, SLOT(actionTriggeredSnapDistance()));
    m_vSnapActions<<snapDistance;
    this->addAction(snapDistance);
    snapIntersection = new QAction(QIcon(":/extui/snapintersection.png"), tr("���㲶׽"), this);
    snapIntersection->setCheckable(true);
    connect(snapIntersection, SIGNAL(triggered()), this, SLOT(actionTriggeredSnapIntersection()));
    m_vSnapActions<<snapIntersection;
    this->addAction(snapIntersection);
    m_iActionCounts = m_vSnapActions.size();

}

QVector<QAction*> QG_SnapToolBar::getActions() const
{
    return m_vSnapActions;
}

void QG_SnapToolBar::setActionHandler(QG_ActionHandler* ah)
{
    actionHandler=ah;
}

void QG_SnapToolBar::actionTriggeredSnapFree()
{
	RS_SnapMode rs_Snap = getSnaps();
	actionHandler->slotSetSnaps(rs_Snap);

	if (true == rs_Snap.snapFree)
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("���ɲ�׽���ܿ���!"));
	}
	else
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("���ɲ�׽���ܹر�!"));
	}
	
}

void QG_SnapToolBar::actionTriggeredSnapGrid()
{
	RS_SnapMode rs_Snap = getSnaps();
	actionHandler->slotSetSnaps(rs_Snap);

	if (true == rs_Snap.snapGrid)
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("����㲶׽���ܿ���!"));
	}
	else
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("����㲶׽���ܹر�!"));
	}
}

void QG_SnapToolBar::actionTriggeredSnapEndpoint()
{
	RS_SnapMode rs_Snap = getSnaps();
	actionHandler->slotSetSnaps(rs_Snap);

	if (true == rs_Snap.snapEndpoint)
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("�˵㲶׽���ܿ���!"));
	}
	else
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("�˵㲶׽���ܹر�!"));
	}
}

void QG_SnapToolBar::actionTriggeredSnapOnEntity()
{
	RS_SnapMode rs_Snap = getSnaps();
	actionHandler->slotSetSnaps(rs_Snap);

	if (true == rs_Snap.snapOnEntity)
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("ʵ���ϵ㲶׽���ܿ���!"));
	}
	else
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("ʵ���ϵ㲶׽���ܹر�!"));
	}
}

void QG_SnapToolBar::actionTriggeredSnapCenter()
{
	RS_SnapMode rs_Snap = getSnaps();
	actionHandler->slotSetSnaps(rs_Snap);

	if (true == rs_Snap.snapCenter)
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("Բ�Ĳ�׽���ܿ���!"));
	}
	else
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("Բ�Ĳ�׽���ܹر�!"));
	}
}

void QG_SnapToolBar::actionTriggeredSnapMiddle()
{
	RS_SnapMode rs_Snap = getSnaps();
	actionHandler->slotSetSnaps(rs_Snap);

// 	if (true == rs_Snap.snapMiddle)
// 	{
// 		DIALOGMEDIATOR->OperationPromptInfo(tr("�ȷֵ㲶׽���ܿ���!"));
// 	}
// 	else
// 	{
// 		DIALOGMEDIATOR->OperationPromptInfo(tr("�ȷֵ㲶׽���ܹر�!"));
// 	}
}

void QG_SnapToolBar::actionTriggeredSnapDistance()
{
	RS_SnapMode rs_Snap = getSnaps();
	actionHandler->slotSetSnaps(rs_Snap);

// 	if (true == rs_Snap.snapDistance)
// 	{
// 		DIALOGMEDIATOR->OperationPromptInfo(tr("�Ⱦ���㲶׽���ܿ���!"));
// 	}
// 	else
// 	{
// 		DIALOGMEDIATOR->OperationPromptInfo(tr("�Ⱦ���㲶׽���ܹر�!"));
// 	}
}

void QG_SnapToolBar::actionTriggeredSnapIntersection()
{
	RS_SnapMode rs_Snap = getSnaps();
	actionHandler->slotSetSnaps(rs_Snap);

	if (true == rs_Snap.snapIntersection)
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("���㲶׽���ܿ���!"));
	}
	else
	{
		DIALOGMEDIATOR->OperationPromptInfo(tr("���㲶׽���ܹر�!"));
	}
}