#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <QByteArray>
#include <QString>
#include <QDebug>
#include <QObject>
#include "JQChecksum.h"
typedef struct ParseData
{    QByteArray id;
     QByteArray valid_data;
}ParsedResult ;

class data_processor : public QObject
{
    Q_OBJECT
public:
    explicit data_processor(QObject *parent = nullptr);
    QByteArray packer(QByteArray data2send, int id_type);
    void unpacker(QByteArray recv_data);
    QByteArray data_pool;

    QList<QByteArray> id_list;

signals:
    void ValidDataReady(QByteArray id ,QByteArray data);

public slots:
    void ProccessingTask(QByteArray rawdata);

private:
    QByteArray  head;


    int min_frame;

    void Init();


};
#endif // DATA_PROCESSOR_H

