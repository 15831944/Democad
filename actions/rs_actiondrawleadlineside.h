

#ifndef RS_ACTIONDRAWLEADLINESIDE_H
#define RS_ACTIONDRAWLEADLINESIDE_H

#include "rs_previewactioninterface.h"
#include "getData_Line.h"


/**
 * This action class can handle user events to draw
 * simple lines with the start- and endpoint given.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawLeadLineSide : public RS_PreviewActionInterface {
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
    RS_ActionDrawLeadLineSide(RS_EntityContainer& container,
                      RS_GraphicView& graphicView);
    virtual ~RS_ActionDrawLeadLineSide();

    virtual RS2::ActionType rtti() {//�����־��Ҫ�ģ���ȻsetCurrentActionʱ��Ҫ����ж��ˣ��������������
        return RS2::ActionDrawLine;
    }

    static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

    void reset();

    virtual void init(int status=0);
    virtual void trigger();

    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);

	void mousePressEvent(QMouseEvent* e);//Modify @zhang.y.q

    virtual void coordinateEvent(RS_CoordinateEvent* e);//����ûһ�εĵ���¼�

    virtual void updateMouseCursor();
    //    virtual void updateToolBar();

	//data�������ݣ�bool��־�ǳɹ�����
	bool createLeadLineData(const RS_Vector& coord, RS_Entity* entity, RS_LineData *data);
	
public slots:
	/*Begin Modify @*/
	void OnCommit(MyPoint mpost);
	void OnRight(QString f);
	void OnLeft(QString f);
	void OnEnter(QString f);
	/*end Modify @*/	


private:
    RS_LineData data;
    RS_Vector start;
	RS_Entity *entity; //���浱ǰ��׽�����Ǹ������ߵ�ʵ��

	QPoint curMousePoint;//��ǰ�������� @zhang.y.q
	QPoint prevMousePoint;//֮ǰ�������� @zhang.y.q
	getData_Line* getDataWindow_Line;
};

#endif
