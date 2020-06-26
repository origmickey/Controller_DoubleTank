#include "data_processor.h"

data_processor::data_processor()
{
    this->Init();
}


QByteArray data_processor::packer(QByteArray data2send, QByteArray  id)
{
    int len = id.size()+data2send.size();
    qDebug()<<"len is :"<<len;
    QByteArray len_dataarea = QByteArray::number(len,16);

    qDebug()<<"len_dataarea is :"<<len_dataarea;

    QByteArray data_area=id+data2send;

    qDebug()<<"data_area is :"<<data_area;

    auto crc32 = JQChecksum::crc32( data_area );
    QByteArray crc_code = QByteArray::number(crc32,16);

    qDebug()<<"crc_code is :"<<crc_code;

    QByteArray msg_frame = head+len_dataarea+data_area+crc_code;

    qDebug()<<"msg_frame is :"<<msg_frame;


    return msg_frame;
}

void data_processor::unpacker(QByteArray recv_data)
{
    data_pool.append(recv_data);

    while(data_pool.indexOf(head)!=-1)
    {//while

    int first_head_pos=data_pool.indexOf(head);
    data_pool=data_pool.mid(first_head_pos+2,-1);  //处理索引来到最近的帧头处

    if(data_pool.size()>min_frame)
    {

    bool ok;
    int len_dataarea = data_pool.mid(2,1).toInt(&ok,16);

    if(data_pool.size()-min_frame>len_dataarea)
    {

    QByteArray data_area=data_pool.mid(3,len_dataarea);

    auto crc32 = JQChecksum::crc32(data_area);
    QByteArray judge_crc = QByteArray::number(crc32,16);

    QByteArray recv_crc = data_pool.mid(3+len_dataarea,-1);



    if(recv_crc == judge_crc)
    {
        QByteArray id = data_area.mid(0,1);
        QByteArray valid_data = data_area.mid(1, (data_area.size()-1));

        ParsedResult parsed_result;

        parsed_result.id = id;
        parsed_result.valid_data = valid_data;
        data_pool=data_pool.mid(11+len_dataarea,-1);//数据池排掉处理完的数据帧，向后继续处理

    }
    else
    {
        qDebug()<<"crc校验错误";
        data_pool=data_pool.mid(11+len_dataarea,-1);//数据池排掉处理完的数据帧，向后继续处理
    }

    }//if 数据区长度正确

    else{
        qDebug()<<"数据区长度过短";
    }

    }// if 最小数据帧
    else
    {
        qDebug()<<"当前数据池小于最小数据帧长度";
    }



}//while


    qDebug()<<"当前数据池找不到数据帧";

}



void data_processor::Init()
{
    head=QByteArray::fromHex("FFBE");


    id=QByteArray::fromHex("01");

    data_pool=QByteArray();

    min_frame=11;

}
