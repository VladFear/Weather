#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QErrorMessage>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTableWidget>
#include <QHeaderView>
#include <QSpacerItem>
#include <QPair>
#include <algorithm>

#include "xmlparser.h"
#include "qcustomplot.h"
#include "mycustomplot.h"

class WeatherWidget : public QWidget
{
  Q_OBJECT
private:
  QNetworkAccessManager* access_manager_find;
  QNetworkAccessManager* access_manager_forecast;
//  QCustomPlot* plot;
  MyCustomPlot* plot;
  QPushButton* update_btn;
  QPushButton* location_btn;
  QTableWidget* table;
//  QTableWidget* cities_table;
  QLabel* current_icon;
  QLabel* city;
  QLabel* cloudness;
  QLabel* temperature;
  QLabel* last_update;
  XmlParser* parser;

private:
  void createInterior();
  void connectSlots();

private slots:
  void updateData();
  void updateTable(const QString& filename);
  void updatePlot(const QString& filename);
  void replyFinishedFind(QNetworkReply *reply);
  void replyFinishedForecast(QNetworkReply *reply);
  void printSelected(QCPDataSelection selected);

signals:
  void updateTableS(const QString& filename);
  void updatePlotS(const QString& filename);

public:
  WeatherWidget();
  ~WeatherWidget();
  WeatherWidget (const WeatherWidget&) = delete;
  WeatherWidget& operator = (const WeatherWidget&) = delete;
  WeatherWidget (WeatherWidget&&) = delete;
  WeatherWidget& operator = (WeatherWidget&&) = delete;
  void startUpdate() const;
};

#endif // WEATHERWIDGET_H
