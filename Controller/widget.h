#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include "client.h"
#include "model.h"
#include "data_processor.h"
#include <QtCharts>
#include <QThread>
#include <QQueue>
#include "mychart.h"
#include "mydata.h"
#include <QPainter>
#include <QtMath>


QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void Init();

    Client * client;

    QThread thread1;
    QThread thread2;

    data_processor * msg_processor;



private slots:
    void on_connectserver_clicked();

    void on_sendmsg_clicked();

    //void on_pushButton_clicked();

    void send_compute_res(double res, int h, int id);

    void plot(QList<QPointF> data);
    void plot2(QList<QPointF> data);

    //void deal_data(double value);

    void read_input(double y,int id);
    void read_input2(double y,int id);

    void SlotReadData(const QByteArray &data);
    void GetValidData(QByteArray id , QByteArray proccessed_data);

    //void on_verticalSlider_valueChanged(int value);

    void on_sendmsg_2_clicked();

    void on_pushButton_clicked();

    void on_sendmsg_3_clicked();

signals:
    void get_input(double,double,int);
    void get_input2(double,double,int);

    void start_receive();
    void send_signal(QByteArray msg);
    void ProccessingCall(QByteArray data);

    void read_signal(double y,int id);
    void read_signal2(double y,int id);

    void read_new(double y);
    void read_new2(double y);

private:
    Ui::Widget *ui;
    Mychart *m_chart;
    Mychart *m_chart2;
    QSplineSeries* m_series;
    QSplineSeries* m_series2;
    //QList<QPointF> m_data;//存放数据

    Mydata m_data;
    Mydata m_data2;

    model Model;
    model Model2;

    QTimer *pTimer1;

    double y_current; //对象0当前液面高度
    double y_current2;//对象1当前液面高度
    double h_current;
    double h_current2;

    double m_offset;
    int obj;  //被控对象
    int id_tcp ;
    int  on;  //发送开关

protected:

    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void painttank(double yk,int pointx,int tunky,int width,int height);

};
#endif // WIDGET_H
