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

    ui->spinBox_2->setMinimum(0);
    ui->spinBox_2->setMaximum(50);
    ui->spinBox_2->setSingleStep(1);
    ui->verticalSlider_2->setMinimum(0);
    ui->verticalSlider_2->setMaximum(50);
    ui->verticalSlider_2->setSingleStep(1);
    connect(ui->verticalSlider_2,SIGNAL(valueChanged(int)),ui->spinBox_2,SLOT(setValue(int)));
    connect(ui->spinBox_2,SIGNAL(valueChanged(int)),ui->verticalSlider_2,SLOT(setValue(int)));


    //界面

    m_chart = new Mychart;
    m_chart2 = new Mychart;

    pTimer1 = new QTimer;

    m_series = new QSplineSeries;
    m_chart->addSeries(m_series);
    m_series->attachAxis(m_chart->axisX);
    m_series->attachAxis(m_chart->axisY);
    //m_series->replace(data);
    ui->widget->setChart(m_chart);

    m_series2 = new QSplineSeries;
    m_chart2->addSeries(m_series2);
    m_series2->attachAxis(m_chart2->axisX);
    m_series2->attachAxis(m_chart2->axisY);
    ui->widget_2->setChart(m_chart2);

    //qDebug()<<m_data.data[12];

    connect(pTimer1,SIGNAL(timeout()),&m_data,SLOT(data_update()));
    connect(pTimer1,SIGNAL(timeout()),&m_data2,SLOT(data_update()));

    connect(&m_data,SIGNAL(refresh(QList<QPointF>)),this,SLOT(plot(QList<QPointF>)));
    connect(&m_data2,SIGNAL(refresh(QList<QPointF>)),this,SLOT(plot2(QList<QPointF>)));

    connect(this,SIGNAL(read_new(double)),&m_data,SLOT(get_height(double)));
    connect(this,SIGNAL(read_new2(double)),&m_data2,SLOT(get_height(double)));
    //m_data.moveToThread(&thread2);
    //thread2.start();


    //模型计算
    Model.moveToThread(&thread1);
    Model2.moveToThread(&thread2);
    connect(this,&Widget::get_input,&Model,&model::controller);  //收到返回值后，读取输入发送至控制器计算
    connect(this,&Widget::get_input2,&Model2,&model::controller);

    connect(&Model,&model::res_u,this,&Widget::send_compute_res);  //计算完成发送至被控对象
    connect(&Model2,&model::res_u,this,&Widget::send_compute_res);
    thread1.start();
    thread2.start();


    //通讯线程
    connect(this,&Widget::send_signal,client,&Client::SendMsg);
    //client->moveToThread(&thread2);
    //thread2.start();


    //读取数据
    connect(client,SIGNAL(sig_readyRead(QByteArray)),this,SLOT(SlotReadData(QByteArray)));
    connect(this,SIGNAL(ProccessingCall(QByteArray)),msg_processor,SLOT(ProccessingTask(QByteArray)));
    connect(msg_processor,SIGNAL(ValidDataReady(QByteArray,QByteArray)),this,SLOT(GetValidData(QByteArray,QByteArray)));

    //收到y后读取输入
    connect(this,SIGNAL(read_signal(double,int)),this,SLOT(read_input(double,int)));
    connect(this,SIGNAL(read_signal2(double,int)),this,SLOT(read_input2(double,int)));

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
    //on = 1; //发送允许

   // obj = ui->comboBox->currentIndex();
    //qDebug()<<"control target is :"<<obj;

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


void Widget::on_pushButton_clicked()  //调节控制对象1
{
    //obj = ui->comboBox->currentIndex();
    //qDebug()<<"control target is :"<<obj;

    int u = 10;

    QByteArray data2send = QByteArray::number(u,16);

    //QByteArray id=QByteArray::fromHex("00");

    int id = 5;

    QByteArray  msg = msg_processor->packer(data2send,id);

    //client->SendMsg(msg);
    emit send_signal(msg);  //发送信号
}





void Widget::send_compute_res(double res, int h ,int id)  //发送计算后的uk
//res为计算结果
{


    if(id==1)
    {
        h_current=h;
    }
    if(id==4)
    {
        h_current2=h;
    }
    int u = res*1000;
    QByteArray data2send = QByteArray::number(u,16);
       //QByteArray id = QByteArray::fromHex("00");
    id = id-1;
    QByteArray msg = msg_processor->packer(data2send,id);
    emit send_signal(msg);

}



void Widget::plot(QList<QPointF> data) //绘图
{
    m_series->replace(data);
}

void Widget::plot2(QList<QPointF> data) //绘图
{
    m_series2->replace(data);
}



void Widget::read_input(double y, int id)  //读取输入框数
{
    //double input = ui->lineEdit->text().toDouble();
    double input = ui->spinBox->text().toDouble();

    if(id == 1)    //若控制对象为0，则发送至1对应的model
    {
        emit get_input(input,y,id);
    }
    if(id == 4)
    {
        emit get_input2(input,y,id);
    }
}

 void Widget::read_input2(double y, int id)
 {
     double input = ui->spinBox_2->text().toDouble();
     emit get_input2(input,y,id);

 }





void Widget::SlotReadData(const QByteArray &data)
{
    qDebug()<<" data: "<<QString(data);

    emit ProccessingCall(data);
}



void Widget::GetValidData(QByteArray id, QByteArray proccessed_data)
{

    int index = msg_processor->id_list.indexOf(id);
    int target = obj*3+1;
    //id_tcp = id;

    qDebug()<<"proccessed_data is : "<<proccessed_data;

    bool ok;

    int int_yk = proccessed_data.toInt(&ok, 16);

    double times = 1000;
    double double_yk = int_yk;

    double real_yk = double_yk / times;

    qDebug()<<"real_yk is :"<< real_yk;



    if(index==1)
    {
         qDebug()<<"got yk0";

         emit read_new(real_yk);

         y_current = real_yk;

         emit read_signal(real_yk,index);

    }
    if(index ==4)
    {
        qDebug()<<"got yk1";

        emit read_new2(real_yk);

        y_current2 = real_yk;

        emit read_signal2(real_yk,index);

    }
    qDebug()<<"target"<<obj;
    //发送信号，准备读取文本框

}


//水槽绘图
void Widget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    ui->height->setText(QString::number(y_current, 10, 4));
    ui->height2->setText(QString::number(y_current2, 10, 4));
    this->update();
}


void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    double h1;
    double h2;
    h1 = h_current;
    h2 = y_current;
    if(h_current<0)
    {
        h1 = 0;
    }
    if(h_current>350/6)
    {
        h1 = 350/6;
    }
    if(y_current<0)
    {
        h2 = 0;
    }
    if(y_current>350/6)
    {
        h2 = 350/6;
    }

    painttank(h1*6,60,150,110,500);
    painttank(h2*6,170,150,220,500);

    double h3;
    double h4;
    h3 = h_current2;
    h4 = y_current2;
    if(h_current2<0)
    {
        h3 = 0;
    }
    if(h_current2>350/6)
    {
        h3 = 350/6;
    }
    if(y_current2<0)
    {
        h4 = 0;
    }
    if(y_current2>350/6)
    {
        h4 = 350/6;
    }

    painttank(h3*6,830,150,880,500);
    painttank(h4*6,920,150,970,500);
}


void Widget::painttank(double yk, int pointx, int tunky, int width, int height)
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

void Widget::on_sendmsg_2_clicked() //停止调节
{
    int id;
    id =6 ;
    int u = 10;

    QByteArray data2send = QByteArray::number(u,16);

    QByteArray  msg = msg_processor->packer(data2send,id);

    emit send_signal(msg);
}

void Widget::on_sendmsg_3_clicked()
{
    int id = 7;
    int u = 10;

    QByteArray data2send = QByteArray::number(u,16);

    QByteArray  msg = msg_processor->packer(data2send,id);

    emit send_signal(msg);
}
