#include <QApplication>
#include <QDebug>

#include "weatherwidget.h"

int main(int argc, char** argv)
{
  QApplication a(argc, argv);

  WeatherWidget weather_widget;
  weather_widget.show();
  weather_widget.startUpdate();

  return a.exec();
}
