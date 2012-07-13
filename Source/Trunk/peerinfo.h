#ifndef PEERINFO_H
#define PEERINFO_H
#include <QtGui/QApplication>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QMap>
#include <QQueue>
#include <QWaitCondition>
#include <QHostInfo>
#include "geoip.h"

typedef struct PeerData
{
    QString hostName;
    QString countryCode;
    QString countryName;
} TPeerData, *PPeerData;

class PeerInfo: public QThread
{
    Q_OBJECT
public:
    PeerInfo(QString geoIPPath);
    ~PeerInfo();
    TPeerData getPeerInfo(QString ip);
    void run();
private:
    QString lookUpHost(const QHostInfo &host);
    QMap<QString, TPeerData> peers;
    QQueue<QString> queue;
    QMutex mutex;
    QWaitCondition waitCondition;
    GeoIP geoip;
    bool abort;
};

#endif // PEERINFO_H
