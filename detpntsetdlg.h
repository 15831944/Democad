#ifndef DETPNTSETDLG_H
#define DETPNTSETDLG_H //< �������ڣ��趨��ֻ��ʾ̽���

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QTextEdit>
#include "RS_CustomBtn.h"
#include "Parameter.h"

class DetPntSetDlg : public QDialog
{
	Q_OBJECT

public:
	DetPntSetDlg(QWidget *parent);
	~DetPntSetDlg();

	bool inputLegal( int inputmode = ui_input );

public slots:
	void slotHasArcChecked( bool );
	void slotUploadAllIndex();

protected:
	void paintEvent(QPaintEvent * event);
	void resizeEvent(QResizeEvent * event);
	void showEvent(QShowEvent * event);

	void paintView( QPainter & painter, int _x, int _y, int wth, int hgt );

private:
    bool m_bReadOnly;
	bool m_bHasArc;

    QLineEdit * m_indexEdit[pnt_set_index_count];
	QLabel    * m_signLbl[9];  //< 9�� - �ţ���ʾ��Χ
	QLabel    * m_ZIndexLbl;   //< Z�᷶Χ�е�������ʾ

	RS_CustomBtn * m_okBtn;
	RS_CustomBtn * m_closeBtn;
	QCheckBox * m_SelectChkBox;  //< ѡ��ѡ��
	QTextEdit * m_TipText;       //< �����ı���
};

#endif // DETPNTSETDLG_H
