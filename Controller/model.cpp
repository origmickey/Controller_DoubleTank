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
        yk.enqueue(1);
    }

}

void model::compute(double input)
{
    double u = input+1 ;
    //
    //计算过程
    //
    double ek1 = input - yk.last();
    ek.enqueue(ek1);
    ek.dequeue();
    //slide_avg(input,y)


    qDebug()<<ek;
    //
    QString LogInfo;
    LogInfo.sprintf("%p", QThread::currentThread());
    qDebug() <<"threadID : "<<LogInfo;
    emit res_u(u);
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
    uk_1 = -denDz[0]*uk.last() -denDz[1]*uk[1]
            +numDz[0]*ek_1+numDz[1]*ek.last()+numDz[2]*ek[0];
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
