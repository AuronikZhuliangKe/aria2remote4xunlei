#ifndef CHECKBOXITEM_H
#define CHECKBOXITEM_H

#include <QCheckBox>
#include <QTreeWidgetItem>
#include "xmlrpc.h"

using namespace xmlrpc;
class CheckBoxItem : public QCheckBox
{
    Q_OBJECT
public:
    explicit CheckBoxItem(QWidget *parent = 0);
    CheckBoxItem(QTreeWidgetItem *item, int column, const Variant &value = QVariant(QVariant::Invalid));

private:
    QTreeWidgetItem *item;
    int column;
    Variant originalValue;

signals:

public slots:
    void stateChanged(int);
};

#endif // CHECKBOXITEM_H
