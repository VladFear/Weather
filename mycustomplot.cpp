#include "mycustomplot.h"

MyCustomPlot::MyCustomPlot(QWidget *parent) : QCustomPlot(parent)
{

}

void MyCustomPlot::mouseMoveEvent(QMouseEvent *event)
{
    cursor_pos_x = event->x();
    cursor_pos_y = event->y();

    QCPGraphDataContainer::const_iterator it = graph(0)->data()->constEnd();
    QVariant details;

    if (graph(0)->selectTest(QPoint(cursor_pos_x, cursor_pos_y), false, &details)) // QPoint could be e.g. event->pos() of a mouse event
    {
      QCPDataSelection dataPoints = details.value<QCPDataSelection>();
      if (dataPoints.dataPointCount() > 0)
        it = graph(0)->data()->at(dataPoints.dataRange().begin());
      y = it->value;
    }
//    qDebug() << y;

    this->repaint();
    QCustomPlot::mouseMoveEvent(event);
}

void MyCustomPlot::paintEvent(QPaintEvent *event)
{
    QCustomPlot::paintEvent(event);

    QPainter painter(this);
//    int x = xAxis->pixelToCoord(cursor_pos_x);
    painter.drawText(cursor_pos_x, cursor_pos_y, QString::number(this->y));
}
