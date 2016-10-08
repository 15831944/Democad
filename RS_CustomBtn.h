
#ifndef RS_CustomBtn_H
#define RS_CustomBtn_H

#include <QPushButton>

class RS_CustomBtn : public QPushButton
{
	Q_OBJECT
public:
    RS_CustomBtn(QWidget * parent = NULL);		//RS_CustomBtn constructor [�ڲ�����]
    ~RS_CustomBtn();					//RS_CustomBtn destructor  [�ڲ�����]
public:
	QColor getButtonUpColor() {return m_clrButtonUp;}
	void setButtonUpColor(const QColor & color);	//set button color
	void setButtonDownColor(const QColor & color);	//set button color
	void setButtonTextColor(const QColor & color);	//set text color
	void setDisableTextColor(const QColor & color);
    void setFontStyle(QFont font);

	QString getPixmapFile() const //get pixmap
	{
		return m_strPixMapFile;
	}

	void setPixmapFile(const QString& strPixmapFile)
	{
		m_strPixMapFile = strPixmapFile;
		update();
	}
	void setButtonPressed(bool bPressed) {m_bButtonPressed = bPressed;}
protected:
	void resizeEvent(QResizeEvent * event);		//process the resize event
	void paintEvent (QPaintEvent * event);		//print value to the sreen
	void focusInEvent (QFocusEvent * event);    //Process the focus in event	
	void focusOutEvent (QFocusEvent * event);	//Process the focus out event
	void keyPressEvent(QKeyEvent * event);		//Process the key press event
	void mousePressEvent(QMouseEvent * event);	//Process the mouse press event
	void mouseReleaseEvent(QMouseEvent * event);//Process the mouse release event
	void mouseMoveEvent(QMouseEvent * event);	//Process the mouse move event 
signals:
	void focusIn(QWidget*);		//signal
	void focusOut(QWidget*);	//signal
	void clickedWithThis(QWidget*);	//signal
	void toggledClicked(QWidget*,bool);	//signal
protected slots:
	void onClicked();			//slot
	void onToggle(bool);			//slot
public:
	static const QColor MC2BUTTON_DARK;			//��
	static const QColor MC2BUTTON_LIGHT;		//��
	static const QColor MC2BUTTON_BLACK;		//��ɫ
	static const QColor MC2BUTTON_GREEN;		//��ɫ
	static const QColor COLOR_DISABLED;			//��ɫ
protected:
	QRect m_rectSize;			//��ť���ο� Ĭ��:(0, 0 ,0, 0)
	QRect m_rectFocus;			//������ο� Ĭ��:(0, 0 ,0, 0)
	bool m_bButtonPressed;		//��ť��ʾ״̬ trueʱ��ʾΪ����״̬ Ĭ��:false
	QColor m_clrButtonUp;		//��ť������ɫ Ĭ��:RGB(192,192,192)
	QColor m_clrButtonDown;		//��ť������ɫ Ĭ��:RGB(204,255,255)
	QColor m_clrButtonText;		//��ť������ɫ Ĭ��:RGB(0,0,0)
	QColor m_clrDisableText;
	QString m_strPixMapFile;		//ͼ���ļ�·��
};

#endif
