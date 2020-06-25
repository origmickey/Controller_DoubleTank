#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>

#include "client.h"



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void Init();

    Client * client;

private slots:
    void on_connectserver_clicked();

    void on_sendmsg_clicked();

private:
    Ui::Widget *ui;

};
#endif // WIDGET_H
