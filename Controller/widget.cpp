#include "widget.h"
#include "ui_widget.h"
#include "JQChecksum.h"
//extern QList<QPointF> data;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->Init();


    //界面

    m_chart = new Mychart;
    pTimer1 = new QTimer;
    m_series = new QLineSeries;
    m_chart->addSeries(m_series);
    m_series->attachAxis(m_chart->axisX);
    m_series->attachAxis(m_chart->axisY);
    //m_series->replace(data);
    ui->widget->setChart(m_chart);
    qDebug()<<m_data.data[12];
    connect(pTimer1,SIGNAL(timeout()),&m_data,SLOT(data_update()));
    connect(&m_data,SIGNAL(refresh(QList<QPointF>)),this,SLOT(plot(QList<QPointF>)));
    connect(this,SIGNAL(read_signal(double)),&m_data,SLOT(get_height(double)));
    //m_data.moveToThread(&thread2);
    //thread2.start();


    //模型计算
    Model.moveToThread(&thread1);
    connect(this,&Widget::get_input,&Model,&model::controller);  //收到返回值后，读取输入发送至控制器计算
    connect(&Model,&model::res_u,this,&Widget::send_compute_res);  //计算完成发送至被控对象
    thread1.start();


    //通讯线程
    connect(this,&Widget::send_signal,client,&Client::SendMsg);
    //client->moveToThread(&thread2);
    //thread2.start();



    //读取数据
    connect(client,SIGNAL(sig_readyRead(QByteArray)),this,SLOT(SlotReadData(QByteArray)));
    connect(this,SIGNAL(ProccessingCall(QByteArray)),msg_processor,SLOT(ProccessingTask(QByteArray)));
    connect(msg_processor,SIGNAL(ValidDataReady(QByteArray,QByteArray)),this,SLOT(GetValidData(QByteArray,QByteArray)));

    //收到y后读取输入
    connect(this,SIGNAL(read_signal(double)),this,SLOT(read_input(double)));

}

Widget::~Widget()
{
    delete ui;
    delete client;

    delete msg_processor;
    thread1.wait();
    thread1.quit();

    thread2.wait();
    thread2.quit();
}

void Widget::Init()
{
    client = new Client;

    msg_processor = new data_processor;


}


void Widget::on_connectserver_clicked()
{
    client->Connect("127.0.0.1",9999);    //连接
    pTimer1->start(50);                   //启动绘图定时器
    //m_chart->pTimer1->start(50);        //启动绘图定时器
    emit start_receive();
}


void Widget::on_sendmsg_clicked()     //启动发送
{


    int u = 10;

    QByteArray data2send = QByteArray::number(u,16);

    //QByteArray id=QByteArray::fromHex("00");

    int id = 2;

    QByteArray  msg = msg_processor->packer(data2send,id);

    //client->SendMsg(msg);
    emit send_signal(msg);  //发送信号

    QString LogInfo;
    LogInfo.sprintf("%p", QThread::currentThread());
    qDebug() <<"threadID : "<<LogInfo;

}


void Widget::send_compute_res(double res)  //发送计算后的uk
//res为计算结果
{
   int u = res*1000;
   QByteArray data2send = QByteArray::number(u,16);
   //QByteArray id = QByteArray::fromHex("00");
   int id = 0;
   QByteArray msg = msg_processor->packer(data2send,id);
   emit send_signal(msg);
}



void Widget::plot(QList<QPointF> data) //绘图
{
    //qDebug()<<data[650];
    m_series->replace(data);
}



void Widget::read_input(double y)  //读取输入框数
{
    double input = ui->lineEdit->text().toDouble();
    emit get_input(input,y);
}


void Widget::SlotReadData(const QByteArray &data)
{
    qDebug()<<" data: "<<QString(data);
    emit ProccessingCall(data);
}



void Widget::GetValidData(QByteArray id, QByteArray proccessed_data)
{
    qDebug()<<"GetValidData id is :"<<id;

    qDebug()<<"proccessed_data is : "<<proccessed_data;

    bool ok;

    int int_yk = proccessed_data.toInt(&ok, 16);

    double times = 1000;
    double double_yk = int_yk;

    double real_yk = double_yk / times;

    qDebug()<<"real_yk is :"<< real_yk;

    y_current = real_yk;


    //发送信号，准备读取文本框
    emit read_signal(y_current);


}
