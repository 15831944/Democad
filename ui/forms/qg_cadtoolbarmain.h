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
#ifndef QG_CADTOOLBARMAIN_H
#define QG_CADTOOLBARMAIN_H


#include "ui_qg_cadtoolbarmain.h"
#include "qg_actionhandler.h"
#include "rs_entitycontainer.h"
#include "graphicview.h"
#include "rs_actionmodifydelete.h"
#include "rs_eventhandler.h"
#include <QDebug>

class QG_CadToolBar;

class QG_CadToolBarMain : public QWidget, public Ui::QG_CadToolBarMain
{
    Q_OBJECT

public:
    QG_CadToolBarMain(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~QG_CadToolBarMain();
//    void restoreAction(); //restore action from checked button
//    void finishCurrentAction(bool resetToolBar=false); //clear current action
    void resetToolBar();
//    virtual void showCadToolBar(RS2::ActionType actionType);
    virtual void setActionHandler(QG_ActionHandler *aH) {
            actionHandler = aH;
    }

	void getGraphicView_RS_EntityContainer(RS_EntityContainer *pContainer, GraphicView *pGraphicView)
	{
		m_pContainer   = pContainer;
		m_pGraphicView = pGraphicView;
		eventHandler   = new RS_EventHandler(pGraphicView);
	}

    void setCoordinates(const RS_Vector& abs, bool updateFormat = false);
	void setGridGap(double gap);

public slots:
    virtual void init();
//    virtual void setCadToolBar( QG_CadToolBar * tb );
    //����
    virtual void drawLine();
    virtual void drawLineRectangle();//����
    virtual void drawCircle();
    virtual void drawArc();

// 	//��������
// 	void drawLeadLine();
// 	void drawLeadLineSide();
// 	void drawLeadArcLineVertical();
// 	void drawLeadArcLineOblique();

    //ѡ��
    virtual void deselectAll();//ȡ��ȫѡ
    virtual void selectAll();//ȫѡ
    virtual void selectInvert();//����ѡ��

    //�޸�
    virtual void modifyTrim();//�ü�һ��
    virtual void modifyTrim2();//����һ��ü�

    virtual void modifyMove();
    virtual void modifyRotate();
    virtual void modifyScale();
    virtual void modifyMirror();
	virtual void modifydelete();
	void getFullScreenExit();

//	void OnEditFinish(QString str);// ����ͼ��ϵ�ʱ�� @zhang.y.q
	void OnEditReset();
//	bool isOtherButtonPushed(QString str);//����ǲ���������ť�Ѿ����ڱ�����״̬�� @zhang.y.q
//	void setOtherButtonChecked(QString str,bool flag);//����������ťCheck״̬ @zhang.y.q

//protected slots:
    virtual void languageChange();
//    virtual void mouseReleaseEvent(QMouseEvent* e);

private:
    QG_ActionHandler* actionHandler;
	bool bFullScreen;
	RS_EntityContainer *m_pContainer;
	GraphicView *m_pGraphicView;
	RS_EventHandler* eventHandler;//�¼�������

};

#endif
