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





public slots:
    void ReadMsg();
    void SendMsg(QByteArray data);


signals:
    void sig_readyRead(const QByteArray&);

};

#endif // CLIENT_H
