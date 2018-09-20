#include "weatherwidget.h"

void WeatherWidget::createInterior()
{
//  QPalette pal(palette());

//  // устанавливаем цвет фона
//  pal.setColor(QPalette::Background, std::move(QColor(63, 62, 57)));
//  this->setAutoFillBackground(true);
//  this->setPalette(std::move(pal));

  QVBoxLayout* main_layout = new QVBoxLayout;
  this->setLayout(main_layout);

  city = new QLabel(tr("City"));
  city->setFont(std::move(QFont( "Arial", 13, QFont::Bold)));
//  city->setStyleSheet("color: white;");

  QVBoxLayout* data_layout = new QVBoxLayout;

  QHBoxLayout* hbx1 = new QHBoxLayout;
  current_icon = new QLabel(this);
  current_icon->setFixedSize(std::move(QSize(75, 75)));
  current_icon->setPixmap(std::move(QPixmap(":/icons/01d.png").scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
  hbx1->addWidget(current_icon);

  QVBoxLayout* vbx1 = new QVBoxLayout;
  cloudness = new QLabel(this);
  temperature = new QLabel(this);
  last_update = new QLabel(this);
//  cloudness->setStyleSheet("color: white;");
//  temperature->setStyleSheet("color: white;");
//  last_update->setStyleSheet("color: white;");
  vbx1->addWidget(cloudness);
  vbx1->addWidget(temperature);
  vbx1->addWidget(last_update);
  hbx1->addLayout(vbx1);

  table = new QTableWidget(7, 1, this);
  QStringList vert_header;
  vert_header << "Temperature" << "Wind" << "Cloudness" << "Pressure" << "Humidity" << "Sunrise" << "Sunset";
  table->setVerticalHeaderLabels(vert_header);
  table->setEditTriggers(QTableWidget::NoEditTriggers); // Disable cell editing
  table->setSelectionMode(QTableWidget::NoSelection);
  table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  table->horizontalHeader()->hide();
//  table->setShowGrid(false);
//  table->setStyleSheet("background-color: rgb(63, 62, 57);");


  QHBoxLayout* hbx2 = new QHBoxLayout;
  update_btn = new QPushButton(this);
  update_btn->setIcon(std::move(QIcon(":/icons/refresh.png")));
  update_btn->setFixedSize(std::move(QSize(30, 30)));
  update_btn->setIconSize(std::move(QSize(20, 20)));

  location_btn = new QPushButton(this);
  location_btn->setIcon(std::move(QIcon(":/icons/gps.png")));
  location_btn->setFixedSize(std::move(QSize(30, 30)));
  location_btn->setIconSize(std::move(QSize(20, 20)));

  hbx2->addWidget(update_btn);
  hbx2->addWidget(location_btn);
  hbx2->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
//  update->setStyleSheet(
//      //"background-color: red;"
//      "border: 1px solid;"  //outline
//      "border-radius: 15px;"     //corners
//      );
//  cities_table = new QTableWidget(this);
//  cities_table->horizontalHeader()->hide();

  data_layout->addLayout(hbx1);
  data_layout->addWidget(table);
  data_layout->addLayout(hbx2);
//  data_layout->addWidget(cities_table);

  QVBoxLayout* plot_layout = new QVBoxLayout;
//  plot = new QCustomPlot(this);
  plot = new MyCustomPlot(this);
  plot->addGraph();
  plot->graph(0)->setScatterStyle( std::move(QCPScatterStyle(QCPScatterStyle::ssDisc, 6)) );
  plot->graph(0)->setPen( std::move(std::move(QPen(std::move(QColor(255, 0, 0))))) );
  plot->graph(0)->setSelectable( QCP::stSingleData );
  plot->setBackground( this->palette().background().color() );
  plot->setInteractions( QCP::iSelectPlottables );
  plot_layout->addWidget(plot);
  // configure bottom axis to show date instead of number:
  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
  dateTicker->setDateTimeFormat("d ddd");
  plot->xAxis->setTickLabelFont(QFont(QFont().family(), 9));
  plot->yAxis->setTickLabelFont(QFont(QFont().family(), 9));
  plot->xAxis->setTicker(dateTicker);

  QHBoxLayout* b1 = new QHBoxLayout;
  b1->addLayout(data_layout, 1);
  b1->addLayout(plot_layout, 2);

  main_layout->addWidget(city, 0, Qt::AlignHCenter);
  main_layout->addLayout(b1);
}

void WeatherWidget::connectSlots()
{
  connect(update_btn, SIGNAL(clicked(bool)), this, SLOT(updateData()));
  connect(access_manager_find, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinishedFind(QNetworkReply*)));
  connect(access_manager_forecast, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinishedForecast(QNetworkReply*)));
  connect(this, SIGNAL(updateTableS(QString)), this, SLOT(updateTable(QString)));
  connect(this, SIGNAL(updatePlotS(QString)), this, SLOT(updatePlot(QString)));
  connect(plot->graph(0), SIGNAL(selectionChanged(QCPDataSelection)), this, SLOT(printSelected(QCPDataSelection)));
}

void WeatherWidget::updateData()
{
  QNetworkRequest request;
  request.setUrl(QUrl("http://api.openweathermap.org/data/2.5/find?q=kharkiv&appid=76bccad3d71ad0ec90a5acbac6a7e673&units=metric&mode=xml"));

  QNetworkReply* reply_find = access_manager_find->get(request);

  request.setUrl(QUrl("http://api.openweathermap.org/data/2.5/forecast?q=kharkiv&appid=76bccad3d71ad0ec90a5acbac6a7e673&units=metric&mode=xml"));
  QNetworkReply* reply_forecast = access_manager_forecast->get(request);
}

void WeatherWidget::updateTable(const QString &filename)
{
  QVector<QString> cur_data = std::move(parser->parseCurrent(filename));

  table->setItem(0, 0, new QTableWidgetItem(cur_data.at(0) + " °C"));
  for (int i = 1; i < cur_data.size() - 1; i++)
    table->setItem(i, 0, new QTableWidgetItem(cur_data.at(i)));

  city->setText(cur_data.last());
  cloudness->setText(table->item(2, 0)->text());
  current_icon->setPixmap(std::move(QPixmap(":icons/" + cur_data.at(7) + ".png").scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
  temperature->setText(cur_data.at(0) + " °C"); // Temperature label
  last_update->setText(cur_data.at(8)); // Update label
}

void WeatherWidget::updatePlot(const QString &filename)
{
  QPair<QVector<double>, QVector<QString>> res = std::move(parser->parseForecast(filename));
  QVector<double> x;
  for (int i = 0; i < res.first.size(); ++i)
    x.push_back(i);

  QVector<double> date_vec;
  QString format  = "yyyy-MM-ddThh:mm:ss";
  for (int i = 0; i < res.second.size(); ++i)
  {
    QString dateStr = res.second.at(i);
    QDateTime dt = QDateTime::fromString(dateStr, format);
//    QString timeStr = dt.toString("d ddd hh:mm");
//    qDebug() << timeStr;
    double runtime_dbl = dt.toMSecsSinceEpoch() / 1000.0;
    date_vec.push_back(runtime_dbl);
  }

  double* max = std::max_element(res.first.begin(), res.first.end());
  double* min = std::min_element(res.first.begin(), res.first.end());

//  plot->graph(0)->setData(x, res.first);
  plot->graph(0)->setData(date_vec, res.first);
  plot->xAxis->setRange(date_vec.first(), date_vec.last());
  plot->yAxis->setRange(*min - 5, *max + 5);
  plot->replot();
}

void WeatherWidget::replyFinishedFind(QNetworkReply *reply)
{
  QFile file("data_find.xml");
  if (file.open(QFile::WriteOnly))
  {
    file.write(reply->readAll());
  }
  file.close();

  reply->deleteLater();
  emit updateTableS("data_find.xml");
}

void WeatherWidget::replyFinishedForecast(QNetworkReply *reply)
{
  QFile file("data_forecast.xml");
  if (file.open(QFile::WriteOnly))
  {
    file.write(reply->readAll());
  }
  file.close();

  reply->deleteLater();
  emit updatePlotS("data_forecast.xml");
}

void WeatherWidget::printSelected(QCPDataSelection selected)
{
  qDebug() << "select: ";
}

WeatherWidget::WeatherWidget()
{
  this->setWindowTitle(tr("Weather"));
  this->setFixedSize(QSize(700, 400));

  access_manager_find = new QNetworkAccessManager(this);
  access_manager_forecast = new QNetworkAccessManager(this);
  parser = new XmlParser();

  createInterior();
  connectSlots();
}

void WeatherWidget::startUpdate() const
{
  emit update_btn->clicked();
}

WeatherWidget::~WeatherWidget()
{

}
