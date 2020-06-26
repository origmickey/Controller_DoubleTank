#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <QByteArray>
#include <QString>
#include <QDebug>

#include "JQChecksum.h"

typedef struct ParseData{
    QByteArray id;
    QByteArray valid_data;
}ParsedResult ;

class data_processor
{
public:    
    data_processor();

    QByteArray packer(QByteArray data2send, QByteArray id);
    void unpacker(QByteArray recv_data);

    QByteArray data_pool;


private:
    QByteArray  head;
    QByteArray  id;

    int min_frame;



    void Init();



};

#endif // DATA_PROCESSOR_H
