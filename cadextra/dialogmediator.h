#ifndef DIALOGMEDIATOR_H
#define DIALOGMEDIATOR_H

#include <QObject>
#include <QToolBar>
#include "qg_cadtoolbarmain.h"

//class QG_CadToolBarMain;

#define DIALOGMEDIATOR DialogMediator::instance()

class DialogMediator : public QObject
{
    Q_OBJECT
public:
	void clearAll();

    static DialogMediator* instance();
    void setCadToolBarMain(QG_CadToolBarMain *tb) {
        cadToolBarMain = tb;
    }

    void setOptionWidget(QToolBar *oW) {
        optionWidget = oW;
    }

    void setUndoButton(QAction *ub) {
        undoButton = ub;
    }

    void setRedoButton(QAction *rb) {
        redoButton = rb;
    }

    void resetToolBar() {
        if (cadToolBarMain != NULL)
            cadToolBarMain->resetToolBar();
    }

    void updateCoordinateWidget(const RS_Vector& abs, bool updateFormat=false) {
        if (cadToolBarMain != NULL)
            cadToolBarMain->setCoordinates(abs, updateFormat);
    }

	void updateGridGapWidget(double gap) {
		if (cadToolBarMain != NULL)
			cadToolBarMain->setGridGap(gap);
	}



    void setRedoEnable(bool enable);
    void setUndoEnable(bool enable);


    //��������action�������atti�ж����ĸ�������Ȼ����þ����requset,ȥ����ѡ��ؼ�
    //����������action��on��־����ʾ����ɾ��
    //update��ʾ�Ƿ���Ҫ���ݵ�ǰaciton�߼���������¿ؼ�ѡ��״̬
    virtual void requestOptions(RS_ActionInterface* action,
                                bool on, bool update = false);
    //==========�����ѡ��ؼ�=========//
    virtual void requestArcOptions(RS_ActionInterface* action,//����
                                   bool on, bool update);
    virtual void requestLineOptions(RS_ActionInterface* action,//��
                                   bool on, bool update);
    virtual void requestCircleOptions(RS_ActionInterface* action,//Բ
                                   bool on, bool update);
    virtual void requestLineRectangleOptions(RS_ActionInterface* action,//����
                                   bool on, bool update);

    virtual void requestMoveOptions(RS_ActionInterface* action,
                                   bool on, bool update);
    virtual void requestRotateOptions(RS_ActionInterface* action,
                                   bool on, bool update);
    virtual void requestScaleOptions(RS_ActionInterface* action,
                                   bool on, bool update);
    virtual void requestMirrorOptions(RS_ActionInterface* action,
                                   bool on, bool update);
	virtual void requestLeadArcRadiusOptions(RS_ActionInterface* action,
		bool on, bool update);
		
/*GraphM supported function*/
	virtual void requestCalDisOfTP(RS_ActionInterface* action, bool on, bool update);
	virtual void requestShowPosOfTP(RS_ActionInterface* action, bool on, bool update);
	virtual void requestShowMinDistance(RS_ActionInterface* action, bool on, bool update);
	virtual void requestShowAngel(RS_ActionInterface* action, bool on, bool update);
	virtual void requestSetOrigin(RS_ActionInterface* action, bool on, bool update);
	virtual void requestSetGridType(RS_ActionInterface* action, bool on, bool update);


//����������
	virtual void requestSnapDistOptions(double& dist, bool on);
	virtual void requestSnapMiddleOptions(int& middlePoints, bool on);
    
	void OperationPromptInfo(QString qstrInfo);			// ������Ϣ��ʾ
    
signals:
public slots:
private:
    explicit DialogMediator(QObject *parent = 0);
    QG_CadToolBarMain *cadToolBarMain;
    QToolBar *optionWidget;
    QAction* undoButton;
    QAction* redoButton;

};

#endif // DIALOGMEDIATOR_H
