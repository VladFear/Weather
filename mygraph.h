#ifndef MYGRAPH_H
#define MYGRAPH_H

#include <QObject>
#include "qcustomplot.h"

class MyGraph : public QCPGraph
{
public:
    explicit MyGraph(QCPAxis *keyAxis, QCPAxis *valueAxis);

protected:
    virtual void draw(QCPPainter *painter);
};

#endif // MYGRAPH_H
