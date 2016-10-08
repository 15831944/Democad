
#include "graphicview.h"

#include "rs_actionzoomin.h"
#include "rs_actionzoompan.h"
#include "rs_actiondefault.h"
#include "rs_actionzoomauto.h"

#include "rs_actionmodifydelete.h"//����delete��������ɾ��
#include "rs_document.h"
#include "Parameter.h"
#include "rs_grid.h"
#include "global_Values.h"

GraphicView::GraphicView(QWidget* parent)
        :QWidget(parent),RS_GraphicView() {

    setBackground(background);//Ĭ�Ϻ�ɫ����ɫ
    redrawMethod=RS2::RedrawAll;
    PixmapLayer1=PixmapLayer2=PixmapLayer3=NULL;

    layout = new QGridLayout(this);//��ʼ������
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 0);
    layout->setColumnStretch(2, 0);
    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 0);

    hScrollBar = new QG_ScrollBar(Qt::Horizontal, this);//��ʼ��������
    hScrollBar->setSingleStep(50);
    hScrollBar->setCursor(Qt::ArrowCursor);
    layout->addWidget(hScrollBar, 1, 0);
    layout->addItem(new QSpacerItem(0, hScrollBar->sizeHint().height()), 1, 0);
    connect(hScrollBar, SIGNAL(valueChanged(int)),
            this, SLOT(slotHScrolled(int)));

    vScrollBar = new QG_ScrollBar(Qt::Vertical, this);
    vScrollBar->setSingleStep(50);
    vScrollBar->setCursor(Qt::ArrowCursor);
    layout->addWidget(vScrollBar, 0, 2);
    layout->addItem(new QSpacerItem(vScrollBar->sizeHint().width(), 0), 0, 2);
    connect(vScrollBar, SIGNAL(valueChanged(int)),
            this, SLOT(slotVScrolled(int)));


    QPixmap cur1(":ui/cur_cad_bmp.png");//��ʼ�����ͼ��
    QPixmap cur2(":ui/cur_glass_bmp.png");
    QPixmap cur3(":ui/cur_del_bmp.png");
    QPixmap cur4(":ui/cur_select_bmp.png");
    QPixmap cur5(":ui/cur_hand_bmp.png");

    curCad = new QCursor(cur1, 16, 16);
    curMagnifier = new QCursor(cur2, 12, 12);
    curDel = new QCursor(cur3, 16, 16);
    curSelect = new QCursor(cur4, 16, 16);
    curHand = new QCursor(cur5, 15, 15);


    setMouseTracking(true);
    //setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_NoMousePropagation);

    //λ�ó�ʼ��
    setFactorX(4.0);//��ʼ�� ����ϵ��
    setFactorY(4.0);
    setOffset(50, 50);//ƫ��
    setBorders(10, 10, 10, 10);
    setDefaultAction(new RS_ActionDefault(*getEntityContainer(), *this));
}


GraphicView::~GraphicView() {
    cleanUp();//�����ϲ�����eventHandler����Ȼ�ر�ʱ����
}

void GraphicView::setBackground(const RS_Color& bg) {
	RS_GraphicView::setBackground(bg);

	QPalette palette;
	palette.setColor(backgroundRole(), bg);
	setPalette(palette);
}

void GraphicView::redraw(RS2::RedrawMethod method) {
        redrawMethod=(RS2::RedrawMethod ) (redrawMethod | method);
        update();
}

void GraphicView::resizeEvent(QResizeEvent* /*e*/) {
    adjustOffsetControls();
    adjustZoomControls();
    getOverlayContainer(RS2::Snapper)->clear();
    redraw();
}

void GraphicView::paintEvent(QPaintEvent *) {
    
	RS_EntityContainer *curCt = NULL;
	curCt = container;
	
    PixmapLayer1=getPixmapForView(PixmapLayer1);
    PixmapLayer2=getPixmapForView(PixmapLayer2);
    PixmapLayer3=getPixmapForView(PixmapLayer3);

    // Draw Layer 1  ������
    if (redrawMethod & RS2::RedrawGrid) 
	{
        PixmapLayer1->fill(background);
        RS_PainterQt painter1(PixmapLayer1);
        drawLayer1((RS_Painter*)&painter1);//����ĺ���������
        painter1.end();
    }

    //��������ʵ��
    if (redrawMethod & RS2::RedrawDrawing) 
	{
		//qDebug() << "liuyc draw all entities!";

        PixmapLayer2->fill(Qt::transparent);
        RS_PainterQt painter2(PixmapLayer2);
        painter2.setDrawingMode(RS2::ModeFull);
        painter2.setDrawSelectedOnly(false);
        drawLayer2((RS_Painter*)&painter2, curCt);//����ʵ��;������
        painter2.setDrawSelectedOnly(true);
        drawLayer2((RS_Painter*)&painter2, curCt);//����ѡ���˵�ʵ��
        painter2.end();

		//qDebug() << "liuyc draw all entities end!";
    }

    if (redrawMethod & RS2::RedrawOverlay) 
	{
		//����������
        PixmapLayer3->fill(Qt::transparent);
        RS_PainterQt painter3(PixmapLayer3);
        drawLayer3((RS_Painter*)&painter3);
        painter3.end();
    }

    RS_PainterQt wPainter(this);
    wPainter.drawPixmap(0,0,*PixmapLayer1);
    wPainter.drawPixmap(0,0,*PixmapLayer2);
    wPainter.drawPixmap(0,0,*PixmapLayer3);
    wPainter.end();
    redrawMethod=RS2::RedrawNone;
}

void GraphicView::slotHScrolled(int value) {
    if (hScrollBar->maximum()==hScrollBar->minimum()) {
        centerOffsetX();
    } else {
        setOffsetX(-value);
    }
    redraw();
}

void GraphicView::slotVScrolled(int value) {
    if (vScrollBar->maximum()==vScrollBar->minimum()) {
        centerOffsetY();
    } else {
        setOffsetY(value);
    }
    redraw();
}

int GraphicView::getWidth() {
    return width() - vScrollBar->sizeHint().width();
}

int GraphicView::getHeight() {
    return height() - hScrollBar->sizeHint().height();
}

//����һ����С���õĿյ�pixmap
QPixmap* GraphicView::getPixmapForView(QPixmap *pm)
{

    if (pm==NULL) {
        return new QPixmap(getWidth(), getHeight());
    } else if (pm->width()!=getWidth() || pm->height()!=getHeight()) {//�ߴ粻ͬ���·���
        delete pm;
        return new QPixmap(getWidth(), getHeight());
    } else {
        return pm;//�ߴ���ͬ�������Լ�����
    }
}


void GraphicView::mouseMoveEvent(QMouseEvent* e) 
{
	if (!g_bIsResponseMouse)//����Ӧ
	{
		return;
	}

    RS_GraphicView::mouseMoveEvent(e);
    QWidget::mouseMoveEvent(e);
	
//#ifdef Q_OS_WIN32
//    // make sure that we can still use hotkeys and the mouse wheel
//    //�̶���������ʱ����ɣ�ʧȥ���㣬��ע�Ϳ���
//    if (parent()!=NULL) {
//        ((QWidget*)parent())->setFocus();
//    }
//#endif
}

void GraphicView::mousePressEvent(QMouseEvent* e) 
{
	if (!g_bIsResponseMouse)//����Ӧ
	{
		if (e->button()==Qt::RightButton || e->button()==Qt::LeftButton)
		{
			g_bIsResponseMouse = true;//���������Ӧ
		}
	}
    // pan zoom with middle mouse button
#if QT_VERSION < 0x040700
    if (e->button()==Qt::MidButton /*|| (e->state()==Qt::LeftButton|Qt::AltButton)*/) {
#else
    if (e->button()==Qt::MiddleButton /*|| (e->state()==Qt::LeftButton|Qt::AltButton)*/) {
#endif
        setCurrentAction(new RS_ActionZoomPan(*container, *this));
    }

    RS_GraphicView::mousePressEvent(e);
    QWidget::mousePressEvent(e);
}

void GraphicView::mouseReleaseEvent(QMouseEvent* e) 
{
	if (!g_bIsResponseMouse)//����Ӧ
	{
		if (e->button()==Qt::RightButton || e->button()==Qt::LeftButton)
		{
			g_bIsResponseMouse = true;//���������Ӧ
		}
	}
    RS_GraphicView::mouseReleaseEvent(e);
    //QWidget::mouseReleaseEvent(e);

    if (!e->isAccepted()) {
//        if (QG_DIALOGFACTORY!=NULL && QG_DIALOGFACTORY->getCadToolBar()!=NULL) {
//            QG_DIALOGFACTORY->getCadToolBar()->mouseReleaseEvent(e);
//        }
    }
}

void GraphicView::mouseDoubleClickEvent(QMouseEvent* e) 
{
	if (!g_bIsResponseMouse)//����Ӧ
	{
		if (e->button()==Qt::RightButton || e->button()==Qt::LeftButton)
		{
			g_bIsResponseMouse = true;//���������Ӧ
		}
	}
    // zoom auto with double click middle mouse button
#if QT_VERSION < 0x040700
    if (e->button()==Qt::MidButton) {
#else
	if (e->button()==Qt::LeftButton) { //ԭ����˫���м�
#endif
        setCurrentAction(new RS_ActionZoomAuto(*container, *this));
        e->accept();
    }
	// ˫������Ҽ���ȡ�����и���еĲ��� add by @zhou.g.x
	if (Qt::RightButton == e->button())
	{
		double dTemp = 0.0;
		int nTemp = 0;
		setNeedSelectId(false);
		pDemocadSignal->sendSignal_EditReset();
		DIALOGMEDIATOR->requestArcOptions(NULL, false, false);
		DIALOGMEDIATOR->requestSnapDistOptions(dTemp, false);
		DIALOGMEDIATOR->requestSnapMiddleOptions(nTemp, false);
		//g_pQPublicSignal->DemoCadReset();
	}

	RS_GraphicView::mouseDoubleClickEvent(e);
	QWidget::mouseDoubleClickEvent(e);
	
}

void GraphicView::setMouseCursor(RS2::CursorType c) {

    switch (c) {
    default:
    case RS2::ArrowCursor:
        setCursor(Qt::ArrowCursor);
        break;
    case RS2::UpArrowCursor:
        setCursor(Qt::UpArrowCursor);
        break;
    case RS2::CrossCursor:
        setCursor(Qt::CrossCursor);
        break;
    case RS2::WaitCursor:
        setCursor(Qt::WaitCursor);
        break;
    case RS2::IbeamCursor:
        setCursor(Qt::IBeamCursor);
        break;
    case RS2::SizeVerCursor:
        setCursor(Qt::SizeVerCursor);
        break;
    case RS2::SizeHorCursor:
        setCursor(Qt::SizeHorCursor);
        break;
    case RS2::SizeBDiagCursor:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case RS2::SizeFDiagCursor:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case RS2::SizeAllCursor:
        setCursor(Qt::SizeAllCursor);
        break;
    case RS2::BlankCursor:
        setCursor(Qt::BlankCursor);
        break;
    case RS2::SplitVCursor:
        setCursor(Qt::SplitVCursor);
        break;
    case RS2::SplitHCursor:
        setCursor(Qt::SplitHCursor);
        break;
    case RS2::PointingHandCursor:
        setCursor(Qt::PointingHandCursor);
        break;
    case RS2::ForbiddenCursor:
        setCursor(Qt::ForbiddenCursor);
        break;
    case RS2::WhatsThisCursor:
        setCursor(Qt::WhatsThisCursor);
        break;
    case RS2::OpenHandCursor:
        setCursor(Qt::OpenHandCursor);
        break;
    case RS2::ClosedHandCursor:
        setCursor(Qt::ClosedHandCursor);
        break;
    case RS2::CadCursor:
        setCursor(*curCad);
        break;
    case RS2::DelCursor:
        setCursor(*curDel);
        break;
    case RS2::SelectCursor:
        setCursor(*curSelect);
        break;
    case RS2::MagnifierCursor:
        setCursor(*curMagnifier);
        break;
    case RS2::MovingHandCursor:
        setCursor(*curHand);
        break;
    }

}

void GraphicView::wheelEvent(QWheelEvent *e) {

	if (!g_bMouseWheel)			// ����Ӧ�������¼�
	{
		return;
	}
	if (!g_bIsResponseMouse)
	{
		g_bIsResponseMouse = true;//���������Ӧ
	}
    if (container==NULL) {
        return;
    }

    RS_Vector mouse = toGraph(RS_Vector(e->x(), e->y()));

	grid->updatePointArray();//���������� added by wang.k.h
	g_dSpacing = grid->getSpacing();//��ȡ��ǰ������
    //û�а�������������
	if (e->delta()>0 && g_dSpacing>g_nGridSpacingMin)//@modified by wang.k.h
	{
			setCurrentAction(new RS_ActionZoomIn(*container, *this,
                                             RS2::In, RS2::Both,
                                             mouse));
    }
	else if(e->delta()<0 && g_dSpacing<g_nGridSpacingMax)//@modified by wang.k.h
	{
			setCurrentAction(new RS_ActionZoomIn(*container, *this,
                                             RS2::Out, RS2::Both,
                                             mouse));
    }
	else
		return;
    redraw();
    e->accept();
}

void GraphicView::keyPressEvent(QKeyEvent* e) {

    if (container==NULL) {
        return;
    }

    switch (e->key()) {
    case Qt::Key_Delete:
        setCurrentAction(new RS_ActionModifyDelete(*container, *this));
        break;
    default:
        break;
    }

    QWidget::keyPressEvent(e);
}

void GraphicView::adjustOffsetControls() {
        static bool running = false;

        if (running) {
                return;
        }

        running = true;


    if (container==NULL || hScrollBar==NULL || vScrollBar==NULL) {
        return;
    }

    int ox = getOffsetX();
    int oy = getOffsetY();

    RS_Vector min = container->getMin();
    RS_Vector max = container->getMax();

    // no drawing yet - still allow to scroll
    if (max.x < min.x+1.0e-6 ||
            max.y < min.y+1.0e-6 ||
                max.x > RS_MAXDOUBLE ||
                max.x < RS_MINDOUBLE ||
                min.x > RS_MAXDOUBLE ||
                min.x < RS_MINDOUBLE ||
                max.y > RS_MAXDOUBLE ||
                max.y < RS_MINDOUBLE ||
                min.y > RS_MAXDOUBLE ||
                min.y < RS_MINDOUBLE ) {
        min = RS_Vector(-10,-10);
        max = RS_Vector(100,100);
    }

        int minVal = (int)(-ox-toGuiDX(getWidth())*0.5
                        - QG_SCROLLMARGIN - getBorderLeft());
        int maxVal = (int)(-ox+toGuiDX(getWidth())*0.5
                        + QG_SCROLLMARGIN + getBorderRight());

        hScrollBar->setValue(0);
        if (minVal<=maxVal) {
                hScrollBar->setRange(minVal, maxVal);
        }

        minVal = (int)(oy-toGuiDY(getHeight())*0.5
                        - QG_SCROLLMARGIN - getBorderTop());
        maxVal = (int)(oy+toGuiDY(getHeight())*0.5
                       +QG_SCROLLMARGIN + getBorderBottom());

        if (minVal<=maxVal) {
                vScrollBar->setRange(minVal, maxVal);
        }

    hScrollBar->setPageStep((int)(getWidth()));
    vScrollBar->setPageStep((int)(getHeight()));

    hScrollBar->setValue(-ox);
    vScrollBar->setValue(oy);

    slotHScrolled(-ox);
    slotVScrolled(oy);

    running = false;
}

void GraphicView::adjustZoomControls() {}


void GraphicView::showCamGraph(CAMDataLink *pData, bool bTemp) {
	if (pData != NULL) {
		camContainer->addEntityWithCamList(pData, bTemp);
	}
	setCurrentAction(new RS_ActionZoomAuto(*camContainer, *this));
}

void GraphicView::showOrgGraph() {
	setCurrentAction(new RS_ActionZoomAuto(*container, *this));
}


// EOF
