#include "mychart.h"

Mychart::Mychart(QChart *parent) : QChart(parent)
{
    //m_series = new QLineSeries;
    //this->addSeries(m_series);

    this->legend()->hide();
    this->createDefaultAxes();
    this->setTitle("液面高度");
    this->axisX()->setRange(0,700);
    this->axisY()->setRange(-10,10);

    pTimer1 = new QTimer(this);
    connect(pTimer1,SIGNAL(timeout()),this,SLOT(plot()));


    for (int i = 0; i < 700; i++) {
        qreal x = i;
        m_data.append(QPointF(x,qSin(2.0 * 3.141592 * x / 360.0)));
    }
    //m_series->replace(m_data);
}


void Mychart::plot()
{
    qreal x = y_current;
    for (int i=0; i<m_data.size(); ++i)
                m_data[i].setX(m_data.at(i).x() - 1);
    m_data.append(QPointF(700,x));
    m_data.removeFirst();
    //m_series->replace(m_data);
}


void Mychart::get_height(double h)
{
    y_current = h;
}
