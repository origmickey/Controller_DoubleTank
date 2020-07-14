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

    void send_compute_res(double res);

    void plot(QList<QPointF> data);

    //void deal_data(double value);
    void read_input(double y);
    void SlotReadData(const QByteArray &data);
    void GetValidData(QByteArray id , QByteArray proccessed_data);

    void on_verticalSlider_valueChanged(int value);

signals:
    void get_input(double,double);
    void start_receive();
    void send_signal(QByteArray msg);
    void ProccessingCall(QByteArray data);
    void read_signal(double y);

private:
    Ui::Widget *ui;
    Mychart *m_chart;
    //QChart *m_chart2;
    QSplineSeries* m_series;
    //QList<QPointF> m_data;//存放数据
    Mydata m_data;
    model Model;
    QTimer *pTimer1;
    double y_current; //当前液面高度

    double m_offset;

protected:
       void paintEvent(QPaintEvent *event);
       void timerEvent(QTimerEvent *event);
       void painttank(double yk,int pointx,int tunky,int width,int height);


};
#endif // WIDGET_H
