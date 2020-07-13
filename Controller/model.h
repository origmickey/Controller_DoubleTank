#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QQueue>
#include <QDebug>
class model : public QObject
{
    Q_OBJECT
public:
    explicit model(QObject *parent = nullptr);


signals:
    void res_u(double u);       //发送计算结果

public slots:
    void compute(double input);   //计算

};

#endif // MODEL_H
