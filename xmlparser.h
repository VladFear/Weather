#ifndef XMLPARSER_H
#define XMLPARSER_H
#include <QDebug>
#include <QXmlStreamReader>
#include <QString>
#include <QFile>
#include <QVector>
#include <QPair>

class XmlParser : public QObject
{
  Q_OBJECT
private:
  QXmlStreamReader* xml_parser;

public:
  XmlParser();
  QPair<QVector<double>, QVector<QString> > parseForecast(const QString& filename);
  QVector<QString> parseCurrent(const QString& filename);

  XmlParser(const XmlParser&) = delete;
  XmlParser& operator = (const XmlParser&) = delete;
  XmlParser(XmlParser&&) = delete;
  XmlParser& operator = (XmlParser&&) = delete;
};

#endif // XMLPARSER_H
