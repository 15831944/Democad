/********************************************************************************
** Form generated from reading UI file 'qg_snapdistoptions.ui'
**
** Created: Mon Sep 26 14:56:11 2016
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_SNAPDISTOPTIONS_H
#define UI_QG_SNAPDISTOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>
#include "rs_circle.h"
#include "rs_settings.h"

QT_BEGIN_NAMESPACE

class Ui_QG_SnapDistOptions
{
public:
    QHBoxLayout *hboxLayout;
    QLabel *lDist;
    QLineEdit *leDist;
    QFrame *sep1;

    void setupUi(QWidget *QG_SnapDistOptions)
    {
        if (QG_SnapDistOptions->objectName().isEmpty())
            QG_SnapDistOptions->setObjectName(QString::fromUtf8("QG_SnapDistOptions"));
        QG_SnapDistOptions->resize(150, 24);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_SnapDistOptions->sizePolicy().hasHeightForWidth());
        QG_SnapDistOptions->setSizePolicy(sizePolicy);
        QG_SnapDistOptions->setMinimumSize(QSize(130, 22));
        QG_SnapDistOptions->setMaximumSize(QSize(150, 32767));
        QG_SnapDistOptions->setStyleSheet(QString::fromUtf8("background-color: rgb(244, 244, 244);"));
        hboxLayout = new QHBoxLayout(QG_SnapDistOptions);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(1, 1, 1, 1);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lDist = new QLabel(QG_SnapDistOptions);
        lDist->setObjectName(QString::fromUtf8("lDist"));
        lDist->setWordWrap(false);

        hboxLayout->addWidget(lDist);

        leDist = new QLineEdit(QG_SnapDistOptions);
        leDist->setObjectName(QString::fromUtf8("leDist"));

        hboxLayout->addWidget(leDist);

        sep1 = new QFrame(QG_SnapDistOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy1);
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);

        hboxLayout->addWidget(sep1);


        retranslateUi(QG_SnapDistOptions);
        QObject::connect(leDist, SIGNAL(textChanged(QString)), QG_SnapDistOptions, SLOT(updateDist(QString)));

        QMetaObject::connectSlotsByName(QG_SnapDistOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_SnapDistOptions)
    {
        QG_SnapDistOptions->setWindowTitle(QApplication::translate("QG_SnapDistOptions", "Snap Distance Options", 0, QApplication::UnicodeUTF8));
        lDist->setText(QApplication::translate("QG_SnapDistOptions", "\350\267\235\347\246\273", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_SnapDistOptions: public Ui_QG_SnapDistOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_SNAPDISTOPTIONS_H
