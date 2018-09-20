#include "mygraph.h"

MyGraph::MyGraph(QCPAxis *keyAxis, QCPAxis *valueAxis) : QCPGraph(keyAxis, valueAxis)
{

}

void MyGraph::draw(QCPPainter *painter)
{
    QCPGraph::draw(painter);
}
