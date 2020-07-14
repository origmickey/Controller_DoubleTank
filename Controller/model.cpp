#include "model.h"
#include <QThread>
model::model(QObject *parent) : QObject(parent)
{
    //uk和ek初始化
    for (int i = 0; i < 4; ++i) {
        uk.enqueue(0);

    }
    for (int i = 0; i < 2; ++i) {
        ek.enqueue(0);
        yk.enqueue(0);
    }

}

void model::controller(double input, double y, int id)//计算发送到被控对象的u
{
    id = id;
    double u;
    hk = 0.7788*hk+0.5375*uk.last();
    u = Dalin_CTL(input,y);
    emit res_u(u,hk,id);
}

void model::updata_y(double y)
{
    yk.enqueue(y);
    yk.dequeue();
}

double model::Dalin_CTL(double input,double y)
{

    double numDz[3]={0.9621,-1.6344,0.6894};
    double denDz[2]={-0.7788,-0.2212};

    double uk_1;
    double ek_1 = input-y;
/*
    double p = 0.22;
    double ki = 0.02;
    double kd = 0.4;
    uk_1 = p*(ek_1-ek[1])+ki*ek_1+kd*(ek_1-ek[1]+ek[0])+uk[3];
*/

    //死区算法ek
    //double B = 0.1; //限幅值
    //if(ek_1>B||ek_1<-B)
    //{
    //    ek_1 = ek_1;
    //}

    //if(ek_1< B&& ek_1>-B)
    //{
    //    ek_1 = 0;
    //}

    uk_1 = -denDz[0]*uk[3] -denDz[1]*uk[1]
            +numDz[0]*ek_1+numDz[1]*ek[1]+numDz[2]*ek[0];

    //死区算法ek
    double B =input; //限幅值
    if(uk_1>B)
    {
        uk_1 = B;
    }
//
    if(uk_1< -B)
    {
        uk_1 = -B;
    }

    uk.enqueue(uk_1);
    uk.dequeue();
    ek.enqueue(ek_1);
    ek.dequeue();
    return uk_1;
 }


double model::average(double buf[4])
{
    int sum=0;
    for (int count=0;count<4;count++)
    {
        sum+= buf[count];
    }
    double aver=sum/4;
    //可以加上输出限幅
    return aver;
}

double model::slide_avg(double input, double y)
{
    double uk_buf[4];
    int i = 0;
    while(1){
        uk_buf[i] = Dalin_CTL(input,y);
        //可以加上一段延时
        i++;
        i=i%4;
        if(i==0){break;}
    }
    double ave= average(uk_buf);
    return ave;
}
