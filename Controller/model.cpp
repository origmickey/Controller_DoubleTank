#include "model.h"

model::model(QObject *parent) : QObject(parent)
{

}

void model::compute(double input)
{
    double u = 0 ;
    //
    //计算过程
    //u = input.last()+1;
    qDebug()<<"hi";
    //
    emit res_u(u);
}
