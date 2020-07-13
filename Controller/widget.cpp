#include "widget.h"
#include "ui_widget.h"

#include "JQChecksum.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->Init();

    /*
    //uk和ek初始化
    for (int i = 0; i < 4; ++i) {
        uk.enqueue(0);

    }
    for (int i = 0; i < 2; ++i) {
        ek.enqueue(0);
        yk.enqueue(1);
    }
    //qDebug()<<ek;
    */

    //界面
    m_series = new QLineSeries;
    m_chart = new QChart;
    m_chart->legend()->hide();
    m_chart->addSeries(m_series);
    m_chart->createDefaultAxes();
    m_chart->setTitle("曲线图");

    m_chart->axisX()->setRange(0,700);
    m_chart->axisY()->setRange(-10,10);

    ui->widget->setChart(m_chart);

    //绑定
    Model.moveToThread(&thread1);
    connect(this,&Widget::get_input,&Model,&model::compute);
    connect(&Model,&model::res_u,this,&Widget::send_compute_res);
    //connect(&thread1, &QThread::finished, &Model, &QObject::deleteLater);
    thread1.start();

    //定时器
    pTimer1 = new QTimer(this);
    for (int i = 0; i < 700; i++) {
        qreal x = i;
        m_data.append(QPointF(x,qSin(2.0 * 3.141592 * x / 360.0)));
    }
    //m_series->append(m_data);
    connect(pTimer1,SIGNAL(timeout()),this,SLOT(plot()));

    //通讯线程
    //connect(this,SIGNAL(start_receive()),client,SLOT(ReadMsg()));
    connect(client,SIGNAL(readyRead()),client,SLOT(ReadMsg()));
    connect(client,&Client::new_data,this,&Widget::deal_data);
    client->moveToThread(&thread2);
    thread2.start();

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


//    QByteArray head=QByteArray::fromHex("FFBE");
//    QByteArray data("a");
//    data=head+data;

//    qDebug()<<data;

//    qDebug()<<data.indexOf(head);

}

void Widget::on_connectserver_clicked()
{
    client->Connect("127.0.0.1",9999);    //连接
    pTimer1->start(50);                   //启动绘图定时器
    emit start_receive();
}

void Widget::on_sendmsg_clicked()
{

    int u=10;

    QByteArray data2send = QByteArray::number(u,16);

    QByteArray id=QByteArray::fromHex("01");

    QByteArray  msg = msg_processor->packer(data2send,id);

    client->SendMsg(msg);


}

void Widget::on_pushButton_clicked()
{
    double input = ui->lineEdit->text().toDouble(); //读取当前输入ek
    double ek1 = input - yk.last();
    //ek.enqueue(ek1);
    //ek.dequeue();
    //qDebug()<<ek;
    emit get_input(ek1);
}


void Widget::send_compute_res(double res)  //发送计算后的控制量
{
    qDebug()<<res;//待补充???

    //qreal x = res;
    //for (int i=0; i<m_data.size(); ++i)
    //            m_data[i].setX(m_data.at(i).x() - 1);
    //m_data.append(QPointF(700,x));
    //m_data.removeFirst();

}



void Widget::plot() //绘图
{
    m_series->replace(m_data);
}

void Widget::deal_data(double value)
{
    qDebug()<<"new value is:"<<value;
    //更新m_data
    qreal x = value;
    for (int i=0; i<m_data.size(); ++i)
                m_data[i].setX(m_data.at(i).x() - 1);
    m_data.append(QPointF(700,x));
    m_data.removeFirst();
}










