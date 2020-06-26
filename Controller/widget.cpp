#include "widget.h"
#include "ui_widget.h"

#include "JQChecksum.h"

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

    delete msg_processor;
}

void Widget::Init()
{
    client = new Client;

    msg_processor = new data_processor;


//    QByteArray head=QByteArray::fromHex("FFBE");
//    QByteArray data("a");
//    data=head+data;

//    qDebug()<<data;

//    qDebug()<<data.indexOf(head);

}

void Widget::on_connectserver_clicked()
{
    client->Connect("127.0.0.1",9999);
}

void Widget::on_sendmsg_clicked()
{

    int u=10;

    QByteArray data2send = QByteArray::number(u,16);

    QByteArray id=QByteArray::fromHex("01");


    QByteArray  msg = msg_processor->packer(data2send,id);

    client->SendMsg(msg);


}
