#include "data_processor.h"
#include <QThread>

data_processor::data_processor(QObject *parent) : QObject(parent)
{
    this->Init();
}

QByteArray data_processor::packer(QByteArray data2send, int id_type)
{
    QByteArray id;
    id = id_list.at(id_type);

    int len = id.size()+data2send.size();
    QByteArray len_dataarea = QByteArray::number(len,16);
    qDebug()<<"len_dataarea in packer is : "<<len;
    QByteArray data_area=id+data2send;
    auto crc32 = JQChecksum::crc32( data_area );
    QByteArray crc_code = QByteArray::number(crc32,16);
    QByteArray msg_frame = head+len_dataarea+data_area+crc_code;
    return msg_frame;
}
void data_processor::unpacker(QByteArray recv_data){
    data_pool.append(recv_data);

    qDebug()<<"data_pool is: "<<data_pool;
    while(data_pool.indexOf(head)!=-1)
    {//while
    int first_head_pos=data_pool.indexOf(head);
    data_pool=data_pool.mid(first_head_pos,-1);  //处理索引来到最近的帧头处
    qDebug()<<"data_pool after head_shorten is: "<<data_pool;
    if(data_pool.size()>min_frame)    {
    bool ok;
    int len_dataarea = data_pool.mid(2,1).toInt(&ok,16);
    qDebug()<<"len_dataarea is: "<<len_dataarea;
    qDebug()<<data_pool;
    qDebug()<<data_pool.size();
    if(data_pool.size()-min_frame>=len_dataarea)
    {
    QByteArray data_area=data_pool.mid(3,len_dataarea);
    qDebug()<<"data_area is: "<<data_area;

    auto crc32 = JQChecksum::crc32(data_area);
    QByteArray judge_crc = QByteArray::number(crc32,16);
    qDebug()<<"judge_crc is: "<<judge_crc;
    QByteArray recv_crc = data_pool.mid(3+len_dataarea,-1);
    qDebug()<<"recv_crc is: "<<recv_crc;


    if(recv_crc == judge_crc)
    {
        QByteArray id = data_area.mid(0,1);
        qDebug()<<"id is: "<<id;

        QByteArray valid_data = data_area.mid(1, (data_area.size()-1));
        qDebug()<<"valid data is :"<< valid_data;

        QString LogInfo;
        LogInfo.sprintf("%p", QThread::currentThread());
        qDebug() <<"threadID : "<<LogInfo;

        emit ValidDataReady(id,valid_data);

        ParsedResult parsed_result;
        parsed_result.id = id;
        parsed_result.valid_data = valid_data;
        data_pool=data_pool.mid(11+len_dataarea,-1);//数据池排掉处理完的数据帧，向后继续处理
        qDebug()<<"data_pool next is :"<< data_pool;
    }
    else
    {
        qDebug()<<"crc校验错误";
        data_pool=data_pool.mid(11+len_dataarea,-1);//数据池排掉处理完的数据帧，向后继续处理
    }

    }//if 数据区长度正确

    else{
        qDebug()<<"数据区长度过短";
        break;
    }

    }// if 最小数据帧
    else
    {
        qDebug()<<"当前数据池小于最小数据帧长度";
        break;
    }



}//while


    qDebug()<<"当前数据池找不到数据帧";

}

void data_processor::ProccessingTask(QByteArray rawdata)
{
    qDebug()<<"rawdata is : "<<rawdata;
    this->unpacker(rawdata);
}

void data_processor::Init()
{
    head=QByteArray::fromHex("FFBE");

    id_list=QList<QByteArray>();

    id_list.append(QByteArray::fromHex("00"));
    id_list.append(QByteArray::fromHex("01"));
    id_list.append(QByteArray::fromHex("02"));

    data_pool=QByteArray();

    min_frame=11;

}
