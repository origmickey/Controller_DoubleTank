#include "mydata.h"
#include<QDebug>
//extern QList<QPointF> data;
//QList<QPointF> data ;

Mydata::Mydata(QObject *parent) : QObject(parent)
{
    for (int i = 0; i < 700; i++) {
        qreal x = i;
        data.append(QPointF(x,0));
    }
    qDebug()<<data[12];
}

void Mydata::get_height(double h)
{
    y_current = h;
}

void Mydata::data_update()
{
    qreal x = y_current;

    for (int i=0; i<data.size(); ++i)
                data[i].setX(data.at(i).x() - 1);
    data.append(QPointF(700,x));
    data.removeFirst();

    emit refresh(data);
}
