#include "client.h"

Client::Client()
{

}

Client::~Client()
{

}


void Client::Connect(QString addr,quint16 port)
{
    this->connectToHost(addr,port);
}

void Client::SendMsg(QByteArray  data)
{
    this->write(data);

    connect(this,SIGNAL(readyRead()),this,SLOT(ReadMsg()));
}

void Client::ReadMsg()
{
    QString msg=QString(this->readAll());
    qDebug()<<msg;
}
