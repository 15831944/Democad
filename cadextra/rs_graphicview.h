#ifndef RS_GRAPHICVIEW_H
#define RS_GRAPHICVIEW_H

#include "rs_color.h"
#include "rs_painterqt.h"
#include "rs_pen.h"
#include "rs_entity.h"
#include "rs_entitycontainer.h"
#include <QMoveEvent>
#include <QDebug>

#include "rs_snapper.h"
#include "rs_previewactioninterface.h"

//���Ի�ͼʱ����Ҫ
#include "rs_arc.h"
#include "rs_circle.h"

class RS_Grid;
class RS_EventHandler;
class RS_LineTypePattern;

class RS_GraphicView
{
public:
    RS_GraphicView();
    ~RS_GraphicView();
    //����delete eventHandler���������ֹ���ϲ���ã� ԭ��
    //���ϲ��Ǳ��յ�
    void cleanUp();
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    //��ɫ�������
    virtual void setBackground(const RS_Color& bg);
    RS_Color getBackground();
    RS_Color getForeground();
    void setGridColor(const RS_Color& c);
    void setMetaGridColor(const RS_Color& c);
    void setSelectedColor(const RS_Color& c);
    void setHighlightedColor(const RS_Color& c);
    void setStartHandleColor(const RS_Color& c);
    void setHandleColor(const RS_Color& c);
    void setEndHandleColor(const RS_Color& c);

    //��ͼ
    virtual void redraw(RS2::RedrawMethod method=RS2::RedrawAll) = 0;
    virtual void drawLayer1(RS_Painter *painter);//�������������ϵ
    virtual void drawLayer2(RS_Painter *painter, RS_EntityContainer *ct);//��������ʵ��
    virtual void drawLayer3(RS_Painter *painter);//����������
    virtual void drawAbsoluteZero(RS_Painter *painter);//������㣬����ԭ��
    virtual void drawRelativeZero(RS_Painter *painter);//������,�Ǹ���Ȧʮ��

    virtual void drawMetaGrid(RS_Painter *painter);//������ϵ������
    virtual void drawGrid(RS_Painter *painter);//�������

    //�������㣬���粶׽����ʮ�֣���ͼ���м�Ч����
    virtual void drawOverlay(RS_Painter *painter);

    virtual void drawEntity(RS_Painter *painter, RS_Entity* e, double& patternOffset);
    virtual void drawEntity(RS_Painter *painter, RS_Entity* e);
    virtual void drawEntity(RS_Entity* e, double& patternOffset);
    virtual void drawEntity(RS_Entity* e);
    virtual void deleteEntity(RS_Entity* e);//ʵ���������أ��������õģ��������ڼ���ʱ����ʱ����
    //ֱ�ӻ�ͼ
    virtual void drawEntityPlain(RS_Painter *painter, RS_Entity* e);
    virtual void drawEntityPlain(RS_Painter *painter, RS_Entity* e, double& patternOffset);

    virtual void setPenForEntity(RS_Painter *painter, RS_Entity* e );
    virtual RS_LineTypePattern* getPattern(RS2::LineType t);

    //����任
    RS_Vector toGui(RS_Vector v);//ʵ�����굽��Ļ����
    double toGuiX(double x);
    double toGuiY(double y);
    double toGuiDX(double d);
    double toGuiDY(double d);
	RS_Vector toGuiD(RS_Vector v);

    RS_Vector toGraph(RS_Vector v);//��Ļ���굽ʵ������
    RS_Vector toGraph(int x, int y);
    double toGraphX(int x);
    double toGraphY(int y);
    double toGraphDX(int d);
    double toGraphDY(int d);
	RS_Vector toGraphD(RS_Vector v);


	void resetSize() {
		setFactorX(4.0);
		setFactorY(4.0);
		setOffset(50, 50);
		setBorders(10, 10, 10, 10);
	}

    //�¼����
    void back();//����Ҽ��ͷ��¼�
	void mouseDoubleClickEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

    //����ƫ�����
	void addOffset(int ox, int oy) {
		offsetX += ox;
		offsetY += oy;
	}

    virtual void setOffset(int ox, int oy) {
        setOffsetX(ox);
        setOffsetY(oy);
    }
    void setOffsetX(int ox) {
        offsetX = ox;
    }
    void setOffsetY(int oy) {
        offsetY = oy;
    }
    int getOffsetX() {
        return offsetX;
    }
    int getOffsetY() {
        return offsetY;
    }
    void setFactor(double f) {
        setFactorX(f);
        setFactorY(f);
    }
    void setFactorX(double f) {
        factor.x = fabs(f);
    }
    void setFactorY(double f) {
        factor.y = fabs(f);
    }
    RS_Vector getFactor() {
        return factor;
    }

    void centerOffsetX();//ʹ����ƫ�Ƶ���Ļ�м�
    void centerOffsetY();
    void centerX(double x);
    void centerY(double y);

    void setBorders(int left, int top, int right, int bottom) {
        borderLeft = left;
        borderTop = top;
        borderRight = right;
        borderBottom = bottom;
    }

    int getBorderLeft() {
        return borderLeft;
    }
    int getBorderTop() {
        return borderTop;
    }
    int getBorderRight() {
        return borderRight;
    }
    int getBorderBottom() {
        return borderBottom;
    }

    //��׽
    void setDefaultSnapMode(RS_SnapMode sm);
    RS_SnapMode getDefaultSnapMode() {
        return defaultSnapMode;
    }

    //�����������
    void setDefaultAction(RS_ActionInterface* action);
    RS_ActionInterface*  getDefaultAction();
    void setCurrentAction(RS_ActionInterface* action);
    RS_ActionInterface* getCurrentAction();
    void killSelectActions();
    void killAllActions();

    //�������
    //�����ŵ�action�б��ص�
    virtual void adjustOffsetControls() {}//������������ƫ��
    virtual void adjustZoomControls() {}//����������������
    virtual void zoomWindow(RS_Vector v1, RS_Vector v2,bool keepAspectRatio=true);
    virtual void zoomIn(double f=1.5, const RS_Vector& center=RS_Vector(false));
    virtual void zoomInX(double f=1.5);
    virtual void zoomInY(double f=1.5);
    virtual void zoomOut(double f=1.5, const RS_Vector& center=RS_Vector(false));
    virtual void zoomOutX(double f=1.5);
    virtual void zoomOutY(double f=1.5);
    virtual void zoomPan(int dx, int dy);//ƽ��
    //�Զ����ţ�����˫������м䰴���Ĳ���
    virtual void zoomAuto(bool axis=true, bool keepAspectRatio=true);
    virtual void zoomAutoY(bool axis=true);

    //����
    virtual RS_EntityContainer* getOverlayContainer(RS2::OverlayGraphics position);
    virtual void setMouseCursor(RS2::CursorType /*c*/) {}
    RS_Grid* getGrid() {
        return grid;
    }

    bool isGridOn() {
        return isGrid;
    }
    void setGridOn(bool on) {
       isGrid = on;
    }

	/*���õײ�������ʾ���ͺ������ļ������*/
	void updateGrid(RS2::LineType type, double gap);

    //�����㣬��ͼ�����õ����ȼ���
    RS_Vector getRelativeZero() {
        return relativeZero;
    }

    void setRelativeZero(const RS_Vector& pos) {
            relativeZero = pos;
    }
    void moveRelativeZero(const RS_Vector& pos) {
        setRelativeZero(pos);
        redraw(RS2::RedrawGrid);
    }

    RS_EntityContainer* getEntityContainer()//����ӵģ���������ظ��ˣ���ʱ����
    {
        return container;
    }

    void setContainer(RS_EntityContainer* container)
    {
        this->container = container;
    }
    RS_EntityContainer* getContainer() {
        return container;
    }
	RS_EntityContainer* getCamContainer() {
		return camContainer;
	}

    //����deleteģʽ
    void setDeleteMode(bool m) {
        deleteMode = m;
    }

    bool getDeleteMode() {
        return deleteMode;
    }

	/*����������������ϵ��Գ�ʼ����ϵ��ƫ����*/
	void addCoordinateOffset(RS_Vector offset)
	{
		m_offset += offset;
	}

	void setCoordinateOffset(RS_Vector offset) {
		m_offset = offset;
	}

	/*��ȡ��������ϵ��Գ�ʼ����ϵ��ƫ����*/
	RS_Vector getCoordinateOffset()
	{
		return m_offset;
	}

	RS2::LineType getGridLineType() {
		return m_gridtype;
	}

	bool needSelectId() {
		return isNeedSelectId;
	}

	void setNeedSelectId(bool need) {
		isNeedSelectId = need;
	}

	virtual void showCamGraph(CAMDataLink *pData = NULL, bool bTemp = true){};
	virtual void showOrgGraph(){};

protected:
    RS_Color background;//����ɫ
    RS_Color foreground;//ǰ��ɫ
    RS_Color gridColor;//������ɫ
    RS_Color metaGridColor;
    RS_Color selectedColor;
    RS_Color highlightedColor;
    RS_Color startHandleColor;
    RS_Color handleColor;
    RS_Color endHandleColor;

    RS_Grid* grid;//�����
    RS_SnapMode defaultSnapMode;//��׽�������

    RS_EventHandler* eventHandler;//�¼�������
    RS_EntityContainer* container;
	RS_EntityContainer* camContainer;
    int mx;   //!< Last known mouse cursor position
    int my;   //!< Last known mouse cursor position
    RS_Vector relativeZero;//��ͼʱ�����õ����ȼ��ϰ�


    //Ϊ��ʱ������ʵ�壬��ѡ�񱳾�ɫ����������ʵ��
    //��ʵ��������undo��־�����ƣ��Ѿ���������ˣ��ȼ��ϰ�
    //�������õģ��������ڼ���ʱ����ʱ����
    bool deleteMode;

private:
    int borderLeft;//�߽�
    int borderTop;
    int borderRight;
    int borderBottom;

    RS_Vector factor;//����ϵ��
    int offsetX;//ƽ����ص�
    int offsetY;
    //�������������㣬�������ж���
    QMap<int, RS_EntityContainer *> overlayEntities;
    bool isGrid;
	
	/*ʵ������ϵ����ƽ��������������ԭ���������ʵ����ɵ�ƫ������*/
	RS_Vector m_offset;

	/*�ײ������������*/
	RS2::LineType m_gridtype;

	bool isNeedSelectId;
};

#endif // RS_GRAPHICVIEW_H
