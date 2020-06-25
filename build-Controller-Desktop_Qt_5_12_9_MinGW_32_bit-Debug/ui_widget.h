/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *connectserver;
    QPushButton *sendmsg;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 600);
        connectserver = new QPushButton(Widget);
        connectserver->setObjectName(QString::fromUtf8("connectserver"));
        connectserver->setGeometry(QRect(150, 230, 93, 28));
        sendmsg = new QPushButton(Widget);
        sendmsg->setObjectName(QString::fromUtf8("sendmsg"));
        sendmsg->setGeometry(QRect(170, 360, 93, 28));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        connectserver->setText(QApplication::translate("Widget", "connect", nullptr));
        sendmsg->setText(QApplication::translate("Widget", "sendmsg", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
