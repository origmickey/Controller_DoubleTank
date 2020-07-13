#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QQueue>
#include <QDebug>
#include <QQueue>

class model : public QObject
{
    Q_OBJECT
public:
    explicit model(QObject *parent = nullptr);


signals:
    void res_u(double u);       //发送计算结果

public slots:
    void compute(double input);   //计算
    void updata_y(double y);
    double Dalin_CTL(double input,double y);
    double average(double buf[4]);
    double slide_avg(double input,double y);

private:
    QQueue<double> uk;  //存放4个uk
    QQueue<double> ek;   //存放2个ek
    QQueue<double> yk;  //存放2个返回值

};

#endif // MODEL_H
