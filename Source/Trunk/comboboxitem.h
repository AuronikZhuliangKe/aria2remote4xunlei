#ifndef COMBOBOXITEM_H
#define COMBOBOXITEM_H

#include <QComboBox>
#include <QTreeWidgetItem>
#include "xmlrpc.h"

using namespace xmlrpc;
class ComboBoxItem : public QComboBox
{
    Q_OBJECT
public:
    explicit ComboBoxItem(QWidget *parent = 0);
    ComboBoxItem(QTreeWidgetItem *item, int column, const Variant &value = QVariant(QVariant::Invalid));
    void clear();

private:
    QTreeWidgetItem *item;
    int column;
    Variant originalValue;

signals:

public slots:
    void changeItem(int);
};

#endif // COMBOBOXITEM_H
