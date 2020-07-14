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
    this->startTimer(50);
    //
    ui->spinBox->setMinimum(0);
    ui->spinBox->setMaximum(50);
    ui->spinBox->setSingleStep(1);
    ui->verticalSlider->setMinimum(0);
    ui->verticalSlider->setMaximum(50);
    ui->verticalSlider->setSingleStep(1);
    connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->verticalSlider,SLOT(setValue(int)));
    connect(ui->verticalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));



    //界面

    m_chart = new Mychart;
    pTimer1 = new QTimer;
    m_series = new QSplineSeries;
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
    pTimer1->start(100);                   //启动绘图定时器
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
    //double input = ui->lineEdit->text().toDouble();
    double input = ui->spinBox->text().toDouble();
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

//水槽绘图
void Widget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    this->update();
}


void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    painttank(100,60,300,110,500);
    painttank(y_current*4,170,300,220,500);
}

void Widget::painttank(double yk, int pointx, int tunky, int width, int height)
//double yk：液面高度
//int pointx：水箱液面初始点横坐标
//int tunky：水箱左上角角点纵坐标
//int width：水箱右下角角点横坐标
//int height：水箱右下角角点纵坐标
{
    QPainter painter(this);  //QWidget为绘图设备，创建一个画刷对象，主要用到设置颜色和填充模式，brush，setBrush
    //正弦曲线公式 y = A * sin(ωx + φ) + k
    double w = M_PI/100;  //w为角速度 ，可以理解为波浪的密度，越大密度越大
    double A = 10;    //  A表示振幅,可以理解为水波的高度,越大高度越高
    double k = height-yk;    //  k表示y轴偏移

    painter.drawRect(pointx, tunky, width-pointx, height-tunky);//绘制水箱
    QPainterPath wave; //波浪区域
    wave.moveTo(pointx, height);//第一点坐标为（0,height）;
    m_offset += 6; //全局变量
    //初相位每次加6，每次曲线的起始点不一样，加上定时器的刷新，从而产生动态的效果
    for(int x = pointx; x <= width; x+=1)  //x从0~w的值而改变，从而得到正弦曲线
    {
         double waveY = (double)(A * sin(w * x + m_offset))/2 + k;// waveY随着x的值改变而改变，从而得到正弦曲线
         wave.lineTo(x, waveY);   //从上一个绘制点画一条线到（x，waveY）；
    }
    wave.lineTo(width, height); //右下角，坐标（width, height），移动到右下角结束点,整体形成一个闭合路径
    painter.setBrush(Qt::darkBlue); //填充绿色
    painter.drawPath(wave);      //绘制出图形
}
