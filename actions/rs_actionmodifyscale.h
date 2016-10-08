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

#ifndef RS_ACTIONMODIFYSCALE_H
#define RS_ACTIONMODIFYSCALE_H

#include "rs_previewactioninterface.h"
#include "rs_modification.h"
#include "rs_selection.h"
#include "getData_Line.h"

/**
 * This action class can handle user events to move entities.
 *
 * @author Andrew Mustun
 */
class RS_ActionModifyScale : public RS_PreviewActionInterface {
	Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
		SetSelectEntitys,     /**< Select Entitys. */
        SetReferencePoint,    /**< Setting the reference point. */
        setTargetPoint, //���һ��״̬���޸ĳ���ȫ������������
        ShowDialog            /**< Showing the options dialog. */
    };

private:
    enum ScaleType{Manul, Fixed};

public:
    RS_ActionModifyScale(RS_EntityContainer& container,
                         RS_GraphicView& graphicView);
    
	~RS_ActionModifyScale();

    static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);
    RS2::ActionType rtti(){
        return RS2::ActionModifyScale;
    }

	void Init(int num);
    virtual void init(int status=0);
    virtual void trigger();
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    //virtual void mousePressEvent(QMouseEvent *e);
    virtual void coordinateEvent(RS_CoordinateEvent* e);
	void mousePressEvent(QMouseEvent *e);//@zhang.y.q

    //virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
//    virtual void updateToolBar();

    virtual void hideOptions();
    virtual void showOptions();

    void setCurScaleType(int st) {
        scaleType = st;
        init(SetReferencePoint);//ѡ��ͬ������ʽʱ��action״̬��ʼ��
        deletePreview();
    }
    int getCurScaleType() {
        return scaleType;
    }

    void scalePrameter(double x, double y, double xf, double yf);

	public slots:
		void OnCommit(MyPoint mpost);
		void OnRight(QString f);
		void OnLeft(QString f);
		void OnEnter(QString f);

private:

	RS_Selection *ss;
	bool NeedSelectFlage;//�������Ƿ��б�ѡ���ʵ���־
	bool SelectWindowFlag;//��ѡ��־
	RS_Vector m_OldCorner;//ѡ����һ���ǵ�
	RS_Vector m_NewCorner;//�ڶ����ǵ�

	int mouseLeftButtonDown_Count;//@zhang.y.q
    RS_ScaleData data;
    RS_Vector referencePoint; //����
    RS_Vector scalePoint;	//���ŵ�
    RS_Vector targetPoint; //Ŀ��㣬���ű�����Ŀ��� - ����  / ���ŵ� - ����
    int scaleType;
    void caculateData();

	getData_Line* getDataWindow_Line;
	bool m_bContinueSelt;		// ����ѡ��ͼ�εı�־

};

#endif
