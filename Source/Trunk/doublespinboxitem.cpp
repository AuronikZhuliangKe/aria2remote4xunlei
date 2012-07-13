#include "doublespinboxitem.h"

DoubleSpinBoxItem::DoubleSpinBoxItem(QWidget *parent) :
    QDoubleSpinBox(parent)
{
}

DoubleSpinBoxItem::DoubleSpinBoxItem(QTreeWidgetItem *item, int column, const Variant &value)
{
    this->item = item;
    this->column = column;
    this->originalValue = value;
    connect(this, SIGNAL(valueChanged(double)), SLOT(valueChanged(double)));
}

void DoubleSpinBoxItem::valueChanged(double d)
{
    if (originalValue.type() != QVariant::Invalid)
    {
        item->setFont(0, QFont("", 8, (d != originalValue.toDouble()) ? QFont::Bold : QFont::Normal));
    }
    item->setData(this->column, Qt::UserRole, d);
}
