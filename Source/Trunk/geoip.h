#ifndef GEOIP_H
#define GEOIP_H
#include <QtGui/QApplication>
#include <QString>
#include <QFile>
#include <QHostAddress>
#include <QObject>
#include <QMutex>

class GeoIP: public QObject
{
    Q_OBJECT
    //Q_DECLARE_TR_FUNCTIONS(GeoIP)
private:
    static const quint32 COUNTRY_BEGIN = 16776960;

    static QMutex mutex;

    QFile FileInput;
    quint32 seekCountry(quint32 offset, quint32 ipnum, int depth);
    QString lookupCountryCode(QHostAddress addr);
    QString lookupCountryName(QHostAddress addr);

public:
    GeoIP(QObject *parent = 0) { }
    GeoIP(QString& filename, QObject *parent = 0);
    ~GeoIP();

    QString lookupCountryCode(QString str);
    QString lookupCountryName(QString str);
    bool update(const QByteArray &content);
};

#endif // QGEOIP_H
