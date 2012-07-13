#include "lineedititem.h"

LineEditItem::LineEditItem(QWidget *parent) :
    QLineEdit(parent)
{
}

LineEditItem::LineEditItem(QTreeWidgetItem *item, int column, const Variant &value)
{
    this->item = item;
    this->column = column;
    this->originalValue = value;
    connect(this, SIGNAL(textChanged(QString)), SLOT(textChanged(QString)));
}

void LineEditItem::textChanged(QString str)
{
    if (originalValue.type() != QVariant::Invalid)
    {
        item->setFont(0, QFont("", 8, (this->text().compare(originalValue.toString(), Qt::CaseInsensitive) != 0) ? QFont::Bold : QFont::Normal));
    }
    item->setData(this->column, Qt::UserRole, str);
}
