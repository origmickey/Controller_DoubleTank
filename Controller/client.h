#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>



class Client : public QTcpSocket
{
    Q_OBJECT

public:
    Client();

    ~Client();

    void Connect(QString addr,quint16 port);

    void SendMsg(char* data);

public slots:
    void ReadMsg();

};

#endif // CLIENT_H
