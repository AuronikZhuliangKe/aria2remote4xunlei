#include "spinboxitem.h"

SpinBoxItem::SpinBoxItem(QWidget *parent) :
    QSpinBox(parent)
{
}

SpinBoxItem::SpinBoxItem(QTreeWidgetItem *item, int column, const Variant &value)
{
    this->item = item;
    this->column = column;
    this->originalValue = value;
    connect(this, SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));
}

void SpinBoxItem::valueChanged(int i)
{
    if (originalValue.type() != QVariant::Invalid)
    {
        item->setFont(0, QFont("", 8, (i != originalValue.toInt()) ? QFont::Bold : QFont::Normal));
    }
    item->setData(this->column, Qt::UserRole, i);
}
