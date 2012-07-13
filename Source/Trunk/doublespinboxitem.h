#ifndef DOUBLESPINBOXITEM_H
#define DOUBLESPINBOXITEM_H
#include <QDoubleSpinBox>
#include <QTreeWidgetItem>
#include "xmlrpc.h"

using namespace xmlrpc;
class DoubleSpinBoxItem : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit DoubleSpinBoxItem(QWidget *parent = 0);
    DoubleSpinBoxItem(QTreeWidgetItem *item, int column, const Variant &value = QVariant(QVariant::Invalid));

private:
    QTreeWidgetItem *item;
    int column;
    Variant originalValue;

signals:

public slots:
    void valueChanged(double d);
};
#endif // DOUBLESPINBOXITEM_H
