#ifndef PUBLICFUCTION_H
#define PUBLICFUCTION_H

#include "Parameter.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QTextEdit>
#include "QDebug"

void ReSizeCsCal(int x, int y, int w, int h, int* nx, int* ny, int* nw, int* nh,
                 int screenx, int screeny, int realx, int realy, int iMode, bool Lflag);

/*���ð�ť��ʾ��ɫԲ��*/
QString SetPushButtonSheetStyle(int bgcolor_r, int bgcolor_g, int bgcolor_b, int outR, int inR,
									int bcolor_r, int bcolor_g, int bcolor_b,
									int pbgcolor_r, int pbgcolor_g, int pbgcolor_b);

bool checkIllegalChar(QString qStr);															// �ж��ַ������Ƿ���ڷ������ַ�
void strFormat(QString &qStr, int iTotal, int iSub);											// �ַ����ĸ�ʽ����ʾ
bool readFile(QString Path, QVector<QString> &mcBuff);											// ��ȡ�ļ���Ϣ
bool writeToFile(QString Path, QVector<QString> mcBuff);										// ���ӹ��������浽�ļ���

void getFileName(QString fullfilename, char *filename);			// ��ȡ�ļ���


QRegExpValidator* setInputDataFormat_Digit(QObject *parent, bool bInteger = false, bool bUnInt = false);
QRegExpValidator* setInputDataFormat_DigitAlphabet(QObject *parent);

void HoldTimer(int nT = 20);

#endif // PUBLICFUCTION_H