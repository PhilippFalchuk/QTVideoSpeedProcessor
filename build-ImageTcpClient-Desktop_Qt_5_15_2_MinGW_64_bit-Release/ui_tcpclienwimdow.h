/********************************************************************************
** Form generated from reading UI file 'tcpclienwimdow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENWIMDOW_H
#define UI_TCPCLIENWIMDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpClienWimdow
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TcpClienWimdow)
    {
        if (TcpClienWimdow->objectName().isEmpty())
            TcpClienWimdow->setObjectName(QString::fromUtf8("TcpClienWimdow"));
        TcpClienWimdow->resize(800, 600);
        centralwidget = new QWidget(TcpClienWimdow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        TcpClienWimdow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TcpClienWimdow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        TcpClienWimdow->setMenuBar(menubar);
        statusbar = new QStatusBar(TcpClienWimdow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        TcpClienWimdow->setStatusBar(statusbar);

        retranslateUi(TcpClienWimdow);

        QMetaObject::connectSlotsByName(TcpClienWimdow);
    } // setupUi

    void retranslateUi(QMainWindow *TcpClienWimdow)
    {
        TcpClienWimdow->setWindowTitle(QCoreApplication::translate("TcpClienWimdow", "TcpClienWimdow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpClienWimdow: public Ui_TcpClienWimdow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENWIMDOW_H
