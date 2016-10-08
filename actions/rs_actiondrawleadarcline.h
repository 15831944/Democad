

#ifndef RS_ACTIONDRAWLEADARCLINE_H
#define RS_ACTIONDRAWLEADARCLINE_H

#include "rs_previewactioninterface.h"
#include "rs_arc.h"
#include "../ui/forms/getData_Line.h"//@


/**
 * This action class can handle user events to draw
 * simple lines with the start- and endpoint given.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawLeadArcLine : public RS_PreviewActionInterface {
        Q_OBJECT
public:
    /**
     * Action States.
     */
    //������״̬����
    enum Status {
        SetStartpoint,   /**< Setting the startpoint.  */
        SetEndpoint      /**< Setting the endpoint. */
    };

public:
    RS_ActionDrawLeadArcLine(RS_EntityContainer& container,
                      RS_GraphicView& graphicView, bool tp);
    virtual ~RS_ActionDrawLeadArcLine();

    virtual RS2::ActionType rtti() {//�����־��Ҫ�ģ���ȻsetCurrentActionʱ��Ҫ����ж��ˣ��������������
        return RS2::ActionDrawLeadArcLineOblique;
    }

    static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

    void reset();

    virtual void init(int status=0);
    virtual void trigger();

    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);

	virtual void mousePressEvent(QMouseEvent* e);//Modify @zhang.y.q

    virtual void coordinateEvent(RS_CoordinateEvent* e);//����ûһ�εĵ���¼�

    virtual void updateMouseCursor();
    //    virtual void updateToolBar();

	//data�������ݣ�bool��־�ǳɹ�����
	bool createLeadLineData(const RS_Vector& coord, RS_Entity* entity, RS_LineData *data);

	virtual void showOptions();
	virtual void hideOptions();

	void setArcRadius(double r) {
		arcRadius = r;
	}

public slots:
	/*Begin Modify @*/
	void OnCommit(MyPoint mpost);
	void OnRight(QString f);
	void OnLeft(QString f);
	void OnEnter(QString f);
	/*end Modify @*/	

private:
	RS_LineData data1;
    RS_LineData data;
	RS_ArcData arcData;
    RS_Vector start;
	double arcRadius;
	bool arcType;		//true  ΪԲ��1 б��    false ΪԲ��2 ����
	RS_Entity *entity; //���浱ǰ��׽�����Ǹ������ߵ�ʵ��

	QPoint curMousePoint;//��ǰ�������� @zhang.y.q
	QPoint prevMousePoint;//֮ǰ�������� @zhang.y.q
	getData_Line* getDataWindow_Line;
};

#endif
