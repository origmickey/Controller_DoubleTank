#include "client.h"
#include <QThread>
Client::Client()
{
    connect(this,SIGNAL(readyRead()),this,SLOT(ReadMsg()));
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
    qDebug()<<data;
    this->write(data);

    QString LogInfo;
    LogInfo.sprintf("%p", QThread::currentThread());
    qDebug() <<"threadID : "<<LogInfo;

}

void Client::ReadMsg()
{
    QString LogInfo;
    LogInfo.sprintf("%p", QThread::currentThread());

    qDebug() <<"threadID : "<<LogInfo;
    QString msg=QString(this->readAll());
    qDebug()<<msg;

    double value = 1;
    emit new_data(value);
}
