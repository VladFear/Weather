#ifndef MYCUSTOMPLOT_H
#define MYCUSTOMPLOT_H

#include <QObject>
#include <QDebug>

#include "qcustomplot.h"

class MyCustomPlot : public QCustomPlot
{
  Q_OBJECT
private:
    int cursor_pos_x;
    int cursor_pos_y;
    double y;

public:
    explicit MyCustomPlot(QWidget* parent);

protected:
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
};

#endif // MYCUSTOMPLOT_H
