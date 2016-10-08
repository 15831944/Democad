/*********************************************************************
**
**
**********************************************************************/

#ifndef RS_ACTIONASSIGNR_H
#define RS_ACTIONASSIGNR_H

#include "rs_previewactioninterface.h"
//#include "rs_arc.h"

/**
 * This action class can handle user events to set R 
 * 
 * @author zhou.g.x
 */
class RS_ActionAssignR : public RS_PreviewActionInterface {
	Q_OBJECT

public:
    RS_ActionAssignR(RS_EntityContainer& container,
                     RS_GraphicView& graphicView, bool bAR, double dAR);
    ~RS_ActionAssignR();
	
	virtual RS2::ActionType rtti()
	{
		return RS2::ActionPartTaper;
	}

    void reset();

    virtual void init(int status=0);
    virtual void trigger();
	
	virtual void mouseDoubleClickEvent(QMouseEvent* e); 
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);

	virtual void coordinateEvent(RS_CoordinateEvent* e);
	
    //������ʾѡ��ؼ�����������,��setCurrentAction�б�����
    virtual void hideOptions();
    virtual void showOptions();
	
    virtual void updateMouseCursor();

	void getNearEntity(RS_Entity* pRS_Entity, RS_Entity** pRS_EntityNear, bool bDir = true);

protected:
	bool bAssignR;				// ����ָ��R
	double dAssignR;			// ָ��R��ֵ

};

#endif
