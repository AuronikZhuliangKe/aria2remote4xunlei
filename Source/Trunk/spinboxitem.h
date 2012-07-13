#ifndef SPINBOXITEM_H
#define SPINBOXITEM_H
#include <QSpinBox>
#include <QTreeWidgetItem>
#include "xmlrpc.h"

using namespace xmlrpc;
class SpinBoxItem : public QSpinBox
{
    Q_OBJECT
public:
    explicit SpinBoxItem(QWidget *parent = 0);
    SpinBoxItem(QTreeWidgetItem *item, int column, const Variant &value = QVariant(QVariant::Invalid));

private:
    QTreeWidgetItem *item;
    int column;
    Variant originalValue;

signals:

public slots:
    void valueChanged(int i);
};
#endif // SPINBOXITEM_H
