#include "xmlparser.h"

XmlParser::XmlParser()
{
  xml_parser = new QXmlStreamReader();
}

QPair<QVector<double>, QVector<QString>> XmlParser::parseForecast(const QString &filename)
{
  QFile f(filename);
  if ( !f.open(QFile::ReadOnly) )
  {
    qDebug() << "Failed while opening file!";
    return qMakePair(QVector<double>(), QVector<QString>());
  }

  xml_parser->setDevice(&f);
  xml_parser->readNext();

  QVector<double> value_temp;
  QVector<QString> date;

  while (!xml_parser->atEnd())
  {
    if  (xml_parser->isStartElement())
    {
      if (xml_parser->name() == "temperature")
      {
        foreach (const QXmlStreamAttribute &attr, xml_parser->attributes())
          if (attr.name().toString() == "value")
          {
            QString attribute_value = attr.value().toString();
            value_temp.push_back(attribute_value.toDouble());
          }
      }
      else if (xml_parser->name() == "time")
      {
        foreach (const QXmlStreamAttribute &attr, xml_parser->attributes())
          if (attr.name().toString() == "from")
          {
            QString attribute_value = attr.value().toString();
            date.push_back(attribute_value);
          }
      }
    }
    xml_parser->readNext();
  }

  QPair<QVector<double>, QVector<QString>> result = qMakePair(value_temp, date);
  return result;
}

/*
 * Return vector of string data, which will be used in table,
 * icon and current info.
 * [0] = Temperature
 * [1] = Wind
 * [2] = Cloudness
 * [3] = Pressure
 * [4] = Himidity
 * [5] = Sunrise time
 * [6] = Sunset time
 * [7] = Icon name
 * [8] = Last update
 * [9] = Name of the city
*/
QVector<QString> XmlParser::parseCurrent(const QString &filename)
{
  QFile f(filename);
  if ( !f.open(QFile::ReadOnly) )
  {
    qDebug() << "Failed while opening file!";
    return QVector<QString>();
  }

  xml_parser->setDevice(&f);
  xml_parser->readNext();

  QVector<QString> cur_data;
  cur_data.reserve(10);
  cur_data.resize(10);

  while (!xml_parser->atEnd())
  {
    if (xml_parser->isStartElement())
    {
      if (xml_parser->name() == "temperature")
      {
        foreach (const QXmlStreamAttribute &attr, xml_parser->attributes())
          if (attr.name().toString() == "value")
          {
            QString attribute_value = attr.value().toString();
            cur_data[0] = attribute_value;
          }
      }
      else if (xml_parser->name() == "wind")
      {
        xml_parser->readNext();
        QString data;
        if (xml_parser->name() == "speed")
        {
          foreach (const QXmlStreamAttribute &attr, xml_parser->attributes())
            if (attr.name().toString() == "name")
            {
              QString attribute_value = attr.value().toString();
              data += attribute_value;
            }
        }
        cur_data[1] = data;
      }
//      else if (xml_parser->name() == "clouds")
//      {
//        foreach (const QXmlStreamAttribute &attr, xml_parser->attributes())
//          if (attr.name().toString() == "name")
//          {
//            QString attribute_value = attr.value().toString();
//            cur_data[2] = attribute_value;
//          }
//      }
      else if (xml_parser->name() == "pressure")
      {
        QString data;
        foreach (const QXmlStreamAttribute &attr, xml_parser->attributes())
          if (attr.name().toString() == "value")
          {
            QString attribute_value = attr.value().toString();
            data += attribute_value + " ";
          }
          else if (attr.name().toString() == "unit")
          {
            QString attribute_value = attr.value().toString();
            data += attribute_value;
          }
        cur_data[3] = data;
      }
      else if (xml_parser->name() == "humidity")
      {
        foreach (const QXmlStreamAttribute &attr, xml_parser->attributes())
          if (attr.name().toString() == "value")
          {
            QString attribute_value = attr.value().toString();
            cur_data[4] = attribute_value + "%";
          }
      }
      else if (xml_parser->name() == "sun")
      {
        foreach (const QXmlStreamAttribute &attr, xml_parser->attributes())
        {
          if (attr.name().toString() == "rise")
          {
            QString attribute_value = attr.value().toString();
            int pos = attribute_value.indexOf("T");
            attribute_value.remove(0, pos + 1);
            cur_data[5] = attribute_value;
          }
          else if (attr.name().toString() == "set")
          {
            QString attribute_value = attr.value().toString();
            int pos = attribute_value.indexOf("T");
            attribute_value.remove(0, pos + 1);
            cur_data[6] = attribute_value;
          }
        }
      }
      else if (xml_parser->name() == "weather")
      {
        foreach (const QXmlStreamAttribute &attr, xml_parser->attributes())
        {
          if (attr.name().toString() == "icon")
          {
            QString attribute_value = attr.value().toString();
            cur_data[7] = attribute_value;
          }
          else if (attr.name().toString() == "value")
          {
            QString attribute_value = attr.value().toString();
            cur_data[2] = attribute_value;
          }
        }
      }
      else if (xml_parser->name() == "city")
      {
        foreach (const QXmlStreamAttribute &attr, xml_parser->attributes())
          if (attr.name().toString() == "name")
          {
            QString attribute_value = attr.value().toString();
            cur_data[9] = attribute_value;
          }
      }
      else if (xml_parser->name() == "lastupdate")
      {
        foreach (const QXmlStreamAttribute &attr, xml_parser->attributes())
          if (attr.name().toString() == "value")
            {
              QString attribute_value = attr.value().toString();
              int pos = attribute_value.indexOf("T");
              attribute_value.remove(0, pos + 1);
              cur_data[8] = attribute_value;
            }
      }
    }
    xml_parser->readNext();
  }
  return cur_data;
}
