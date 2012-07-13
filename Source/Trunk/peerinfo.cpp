#include "peerinfo.h"

PeerInfo::PeerInfo(QString geoIPPath):
    geoip(geoIPPath)
  , abort(false)
{
}

PeerInfo::~PeerInfo()
{
    mutex.lock();
    abort = true;
    waitCondition.wakeOne();
    mutex.unlock();

    wait();
}

void PeerInfo::run()
{
    while (!abort)
    {
        mutex.lock();
        waitCondition.wait(&mutex);

        while (!queue.isEmpty())
        {
            QString ip = queue.dequeue();
            mutex.unlock();

            TPeerData pd;
            //GeoIP
            pd.countryCode = geoip.lookupCountryCode(ip);
            pd.countryName = geoip.lookupCountryName(ip);

            //Resolve host name
            pd.hostName = lookUpHost( QHostInfo::fromName( QHostAddress(ip).toString() ));
            mutex.lock();
            peers[ip] = pd;
        }
        mutex.unlock();
    }
}

TPeerData PeerInfo::getPeerInfo(QString ip)
{
    TPeerData pd = {QString(ip), QString(""), QString("")};
    mutex.lock();
    if (!peers.contains(ip))
    {
        queue.enqueue(ip);
        waitCondition.wakeOne();
    }
    else
    {
        pd = peers[ip];
    }
    mutex.unlock();
    return pd;
}

QString PeerInfo::lookUpHost(const QHostInfo &host)
{
    if (host.error() != QHostInfo::NoError)
    {
        return "";
    }

    return host.hostName();
}
