#ifndef LINEEDITITEM_H
#define LINEEDITITEM_H

#include <QLineEdit>
#include <QTreeWidgetItem>
#include "xmlrpc.h"

using namespace xmlrpc;
class LineEditItem : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEditItem(QWidget *parent = 0);
    LineEditItem(QTreeWidgetItem *item, int column, const Variant &value = QVariant(QVariant::Invalid));

private:
    QTreeWidgetItem *item;
    int column;
    Variant originalValue;

signals:

public slots:
    void textChanged(QString str);
};

#endif // LINEEDITITEM_H
