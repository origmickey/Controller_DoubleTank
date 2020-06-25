#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->Init();

}

Widget::~Widget()
{
    delete ui;
    delete client;
}

void Widget::Init()
{
    client = new Client;
}

void Widget::on_connectserver_clicked()
{
    client->Connect("127.0.0.1",9999);
}

void Widget::on_sendmsg_clicked()
{
    char * msg = "hi! it is client request!";
    client->SendMsg(msg);
}
