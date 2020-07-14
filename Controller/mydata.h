#ifndef MYDATA_H
#define MYDATA_H

#include <QObject>
#include<QtCharts>

class Mydata : public QObject
{
    Q_OBJECT
public:
    explicit Mydata(QObject *parent = nullptr);
    QList<QPointF> data;
    double y_current;
signals:
    void refresh(QList<QPointF>);
public slots:
    void get_height(double h);
    void data_update();

};

#endif // MYDATA_H
