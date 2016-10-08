#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <QDialog>
#include "ui_ErrorMessage.h"

class ErrorMessage : public QDialog
{
	Q_OBJECT

public:
	ErrorMessage(QWidget *parent = 0);
	~ErrorMessage();

	int showErrorMessage(int nErrNum, int nErrLevel = 0, QString qstrInfo = "");			// ���ñ���򾯸���Ϣ


protected:
	QString getErrMegDes(int nErrNum, QString qstrInfo = "");								// ͨ������Ż�ȡ����������Ϣ
	QString getWarningMegDes(int nErrNum, QString qstrInfo = "");							// ͨ������Ż�ȡ����������Ϣ
	QString getErrSolution(int nErrNum, QString qstrInfo = "");								// ͨ������Ż�ȡ�������취
	void sysreset();

protected slots:
	void btnYesClicked();
	void btnNoClicked();
	void btnThirdChooseClicked();
	void btnFouthChooseClicked();

private:
	Ui::ErrorMessage ui;
	QEventLoop m_QEventLoop;

	int m_nShowErrMegRtn;

};

#endif // ERRORMESSAGE_H
