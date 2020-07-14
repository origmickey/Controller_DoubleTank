#include "mychart.h"

Mychart::Mychart(QChart *parent) : QChart(parent)
{
    //m_series = new QLineSeries;
    //this->addSeries(m_series);

    this->legend()->hide();
    this->createDefaultAxes();
    this->setTitle("水槽控制曲线");

    axisX = new QValueAxis;
    axisX->setRange(0,700);
    axisX->setTickCount(5);
    axisX->setLabelFormat("%.2f");
    axisX->setTitleText("时间t");
    this->setAxisX(axisX);

    axisY = new QValueAxis;
    axisY->setRange(0,50);
    axisY->setTickCount(5);
    axisY->setLabelFormat("%.2f");
    axisY->setTitleText("液面高度");
    this->setAxisY(axisY);

    //this->axisX();
    //this->axisY();

    //pTimer1 = new QTimer(this);
    //connect(pTimer1,SIGNAL(timeout()),this,SLOT(plot()));


    //for (int i = 0; i < 700; i++) {
    //    qreal x = i;
    //    m_data.append(QPointF(x,qSin(2.0 * 3.141592 * x / 360.0)));
    //}
    //m_series->replace(m_data);
}


