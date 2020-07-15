#include "mychart.h"

Mychart::Mychart(QChart *parent) : QChart(parent)
{
    this->legend()->hide();
    this->createDefaultAxes();
    this->setTitle("当前水槽控制曲线");

    axisX = new QValueAxis;
    axisX->setRange(0,700);
    axisX->setTickCount(5);
    axisX->setLabelFormat("%.2f");
    axisX->setTitleText("时间t");
    this->setAxisX(axisX);

    axisY = new QValueAxis;
    axisY->setRange(0,60);
    axisY->setTickCount(5);
    axisY->setLabelFormat("%.2f");
    axisY->setTitleText("液面高度");
    this->setAxisY(axisY);

}


