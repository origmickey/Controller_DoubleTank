#ifndef MYCHART_H
#define MYCHART_H
#include <QTimer>
#include <QObject>
#include <QtCharts>
#include <QValueAxis>
class Mychart : public QChart
{
    Q_OBJECT
public:
    explicit Mychart(QChart *parent = nullptr);
    //QLineSeries *m_series;
    //QTimer *pTimer1;
    //QList<QPointF> m_data;
    double y_current;
    QValueAxis *axisX;
    QValueAxis *axisY;

signals:

public slots:
    //void plot();
    //void get_height(double h);
private:


};

#endif // MYCHART_H
