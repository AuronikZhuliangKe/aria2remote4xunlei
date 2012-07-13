#ifndef ARIA2CPARAMETER_H
#define PARAMETER_H
#include "xmlrpc.h"
#include "Features.h"
#include <QList>
#include <QString>

using namespace xmlrpc;
using namespace util;

class Aria2cParameter
{
public:
    Aria2cParameter():
        Aria2cFeatures(util::ARIA2C_VERSION_181 | util::ARIA2C_FEATURES_ALL),
        vType(QVariant::Invalid),
        min_value(0),
        max_value(0),
        div(1),
        mul(1){}

    inline Aria2cParameter addFeatures(quint32 f)
    {
        Aria2cFeatures = f;
        return *this;
    }

    inline Aria2cParameter addType(QVariant::Type t)
    {
        vType = t;
        return *this;
    }

    inline Aria2cParameter addMinMax(const Variant &min, const Variant &max)
    {
        min_value = min;
        max_value = max;
        return *this;
    }
    inline Aria2cParameter addList(const QList<QString> &l)
    {
        list = l;
        return *this;
    }
    inline Aria2cParameter addSuffix(const QString &s)
    {
        suffix = s;
        return *this;
    }
    inline Aria2cParameter addDiv(const quint32 &d)
    {
        div = (d == 0) ? (1) : (d);
        return *this;
    }
    inline Aria2cParameter addMul(const quint32 &m)
    {
        mul = m;
        return *this;
    }
    quint32 getFeatures() { return Aria2cFeatures; }
    Variant::Type getType() { return vType; }
    Variant getMin() { return min_value; }
    Variant getMax() { return max_value; }
    QList<QString> getList() { return list; }
    QString getSuffix() {return suffix; }
    quint32 getDiv() { return div; }
    quint32 getMul() { return mul; }

private:
    quint32 Aria2cFeatures;
    Variant::Type vType;
    Variant min_value; //for range
    Variant max_value;
    QList<QString> list; //for ComboBox
    QString suffix;
    quint32 div;
    quint32 mul;
};
#endif // ARIA2CPARAMETER_H
