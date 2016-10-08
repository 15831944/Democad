

#include <QKeyEvent>
#include <QTextCodec>
#include <QPainter>
#include "RS_CustomBtn.h"

const QColor RS_CustomBtn::MC2BUTTON_DARK  = QColor(128,128,128);
const QColor RS_CustomBtn::MC2BUTTON_LIGHT = QColor(255, 255, 255);
const QColor RS_CustomBtn::MC2BUTTON_BLACK = QColor(0, 0, 0);
const QColor RS_CustomBtn::MC2BUTTON_GREEN = QColor(102, 153, 0);
const QColor RS_CustomBtn::COLOR_DISABLED = QColor(192,192,192);
/**Function: RS_CustomBtn 
 *  Description:   ���캯��
  
 */
RS_CustomBtn::RS_CustomBtn(QWidget* parent )
 : QPushButton(parent)
{
 	//Step1���ó�Ա����ΪĬ��ֵ
	m_rectSize			= QRect(0, 0, 0, 0);
	m_rectFocus			= QRect(0, 0, 0, 0);
	m_bButtonPressed	= false;
	m_clrButtonUp		= QColor(192,192,192);
	m_clrButtonDown		= QColor(204,255,255);
	m_clrButtonText		= QColor(0,0,0);
	m_clrDisableText	= QColor(160,160,160);
//	setFont(QGuiConfig::getFont(QGuiConfig::FONT_SMALL));
	//�������ĵ�GBK����Ϊ�����Ĭ������
	QTextCodec* pCodeC = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForCStrings(pCodeC);
	QTextCodec::setCodecForTr(pCodeC);

	connect(this,SIGNAL(clicked()),this,SLOT(onClicked()));
	connect(this,SIGNAL(toggled(bool)),this,SLOT(onToggle(bool)));
}

RS_CustomBtn::~RS_CustomBtn()
{
	//��
}
/**Function:  resizeEvent
 *  Description:  ���µĵ�����С 
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  event ������С�¼�
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::resizeEvent(QResizeEvent* event)
{
	//Step1 ���µ����߿�Ĵ�С
	m_rectSize = rect();
	m_rectFocus = rect().adjusted(4, 4, -5, -5);

	//Step2 ���ø����resizeEvent
	QPushButton::resizeEvent(event);

	return;
}
/**Function:  paintEvent
 *  Description:    ��ͼ����
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  ��ͼ�¼�
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::paintEvent(QPaintEvent*)
{
	//Step1 ����һ����ͼ�ࣨQPainter painter(this);��
	QPainter painter(this);
	painter.save();
	QPen pen;
	pen.setWidth(2);

	//Step2 m_bButtonpressed ��checkedΪtrue
	if (m_bButtonPressed || isChecked())
	{
		//Step3.1 �ð�ť������ɫ��m_clrbuttonDown����䰴ť������m_rectSize����
		painter.fillRect(m_rectSize, m_clrButtonDown);

		//Step4.1 ��Dark��ɫ(RGB(128,128,128))���ư�ť�߿�����ϲ���
		pen.setColor(MC2BUTTON_DARK);
		painter.setPen(pen);
		painter.drawLine(m_rectSize.bottomLeft(), m_rectSize.topLeft());
		painter.drawLine(m_rectSize.topLeft(), m_rectSize.topRight());

		//Step5.1 ��Light��ɫ(RGB(255,255,255))���ư�ť�߿��Һ��²���
		pen.setColor(MC2BUTTON_LIGHT);
		painter.setPen(pen);
		painter.drawLine(m_rectSize.topRight(), m_rectSize.bottomRight());
		painter.drawLine(m_rectSize.bottomRight(), m_rectSize.bottomLeft());
	}
	else
	{
		//Step3.2 �ð�ťԭʼ��ɫ��m_clrbuttonUp����䰴ť������m_rectSize����
		painter.fillRect(m_rectSize, m_clrButtonUp);

		//Step4.2 ��Light��ɫ(RGB(255,255,255))���ư�ť�߿�����ϲ���
		pen.setColor(MC2BUTTON_LIGHT);
		painter.setPen(pen);
		painter.drawLine(m_rectSize.bottomLeft(), m_rectSize.topLeft());
		painter.drawLine(m_rectSize.topLeft(), m_rectSize.topRight());

		//Step5.2 ��Dark��ɫ(RGB(128,128,128))���ư�ť�߿��Һ��²���
		pen.setColor(MC2BUTTON_DARK);
		painter.setPen(pen);
		painter.drawLine(m_rectSize.topRight(), m_rectSize.bottomRight());
		painter.drawLine(m_rectSize.bottomRight(), m_rectSize.bottomLeft());
	}

	//Step6 �����뽹��
	if (hasFocus())
	{
		//Step6.1 ����ɫRGB(102,153,0)���ƽ�����Σ�m_rectFocus��
		QPen pen(MC2BUTTON_GREEN , 2);
		painter.setPen(pen);
		painter.drawRect(m_rectFocus);
	}

	//Step7 ��ť�Ƿ����
	if (isEnabled())
	{
		//Step7.1 ��m_clrButtonText��ɫ������ư�ť��ǩ
		painter.setPen(m_clrButtonText);
	}
	else
	{
		//Step7.2 �û�ɫ������ư�ť��ǩ
		painter.setPen(m_clrDisableText);
	}

	QString strDrawText = text();
	if (!m_strPixMapFile.isEmpty())
	{
		QImage myImage(m_strPixMapFile);
		QSize sz = myImage.size();

		QRect rcTemp((m_rectSize.width() - sz.width()) / 2, 
			(m_rectSize.height() - sz.height()) / 2, sz.width(), sz.height());
		painter.drawImage(rcTemp, myImage);
	}
	else
	{
		painter.drawText(m_rectSize, Qt::AlignHCenter|Qt::AlignVCenter, strDrawText);
	}

	painter.restore();
	return;
}
/**Function:  mousePressEvent
 *  Description:   ������¼�
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  event ����¼�
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::mousePressEvent(QMouseEvent* event)
{
	//Step1 ����m_bButtonPressedΪtrue
	m_bButtonPressed = true;

	//Step2 ���ø����mousePressEvent
	QPushButton::mousePressEvent(event);

	//Step3 ������ʾ
	update();

	//Step4 ����
	return;
}
/**Function:  mouseReleaseEvent
 *  Description:   ����ͷ��¼�
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  ����¼�
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::mouseReleaseEvent(QMouseEvent* event)
{
	//Step1 ����m_bButtonPressedΪfalse	
	m_bButtonPressed = false;

    //Step2 ���ø����mouseReleaseEvent
	QPushButton::mouseReleaseEvent(event);

	//Step3 ������ʾ
	update();

	//Step4 ����
	return;
}
/**Function:  mouseMoveEvent
 *  Description:   ����ƶ��¼�
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  ����¼�
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::mouseMoveEvent(QMouseEvent* event)
{
	//Step1 �������ڰ�ť��������(m_rectSize.left()<=event->x()<m_rectSize.right()
	//&&m_rectSize.top()<=event->y()< m_rectSize.bottom())��
	//����m_bMousePressedΪtrue������Ϊfalse
	if ((m_rectSize.left() <= event->x())
		&& (m_rectSize.right() > event->x())
		&& (m_rectSize.top() <= event->y())
		&& (m_rectSize.bottom() > event->y()))
	{
		m_bButtonPressed = true;
	}
	else
	{
		m_bButtonPressed = false;
	}

	//Step2���ø����mouseMoveEvent
	QPushButton::mouseMoveEvent(event);

	//Step3 ������ʾ
	update();

	//Step4 ����
	return;
}
/**Function:  keyPressEvent
 *  Description:   ���̰����¼�
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  event �����¼�
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::keyPressEvent(QKeyEvent* event)
{
	if (isCheckable())
	{
		//Step2 ����Enter
		if (Qt::Key_Enter == event->key()||Qt::Key_Return == event->key())
		{
			//Step2.1 ����checkedΪtrue
			setChecked(true);
		}

		//Step3 ����Del
		if (Qt::Key_Delete == event->key())
		{
			//Step3.1 ����checkedΪfalse
			setChecked(false);
		}

		if (Qt::Key_Space == event->key())
		{
			setChecked(!isChecked());
		}
	}
	else
	{
		//Step2 ����Enter
		if (Qt::Key_Enter == event->key()||Qt::Key_Return == event->key()||Qt::Key_Space == event->key())
		{
			//Step2.2 ����clicked(����Ϣ
			emit clicked();//?
		}
	}
	
	QWidget::keyPressEvent(event);
}

/**Function:  setButtonUpColor
 *  Description:   ����̧����ɫ
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  color�¼�
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::setButtonUpColor(const QColor& color)
{
	//Step1 m_clrButtonUp = color
	m_clrButtonUp = color;

	//Step2 ����
	update();

	return;
}
/**Function:  setButtonDownColor
 *  Description:   ���°�����ɫ
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  color�¼�
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::setButtonDownColor(const QColor& color)
{
	//Step1 m_clrButtonDown = color;
	m_clrButtonDown = color;

	//Step2 ���²�����
	update();

	return;
}
/**Function:  setButtonTextColor
 *  Description:   ���ð�ť�༭����ɫ
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  color�¼�
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::setButtonTextColor(const QColor& color)
{
	//Step1 m_clrButtonDown = color;
	m_clrButtonText = color;

	//Step2 ���²�����
	update();

	return;
}
/**Function:  onClicked
 *  Description:  ����¼� 
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  ��
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::onClicked()
{
	//Step1	����clickedWithThis(this)�ź�
	emit clickedWithThis(this);

	return;
}
/**Function:  focusInEvent
 *  Description:  �����ȡ�¼� 
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::focusInEvent ( QFocusEvent * event)
{
	//Step1 ����focusIn(this)��Ϣ
	emit focusIn(this);

	//Step2 ���û����focusInEvent
	QPushButton::focusInEvent(event);

	//Step3 ����
	return;
}
/**Function:  focusOutEvent
 *  Description:   ���㶪ʧ�¼�
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::focusOutEvent (QFocusEvent * event)
{
	
	//Step1 ����focusOut(this)��Ϣ
	emit focusOut(this);

	//Step2 ���û����focusOutEvent
	QPushButton::focusOutEvent(event);

	m_bButtonPressed = false;

	//Step3 ����
	return;

}
/**Function: setDisableTextColor 
 *  Description:   ���� �༭����ɫ
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::setDisableTextColor(const QColor & color)
{
	m_clrDisableText = color;
    return;
}
/**Function:  setFontStyle
 *  Description: ����������  
 *  Calls:  
 *  Called By:  
 *  Table Accessed:   
 *  Table Updated:  
 *  Input:  
 *  Output:   
 *  Return:  
 */
void RS_CustomBtn::setFontStyle(QFont font)
{
    this->setFont(font);
}

void RS_CustomBtn::onToggle(bool bChecked)
{
	emit toggledClicked(this,bChecked);

	return;
}
