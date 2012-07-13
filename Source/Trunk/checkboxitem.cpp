#include "checkboxitem.h"

CheckBoxItem::CheckBoxItem(QWidget *parent) :
    QCheckBox(parent)
{
}

CheckBoxItem::CheckBoxItem(QTreeWidgetItem *item, int column, const Variant &value)
{
    this->item = item;
    this->column = column;
    this->originalValue = value;
    connect(this, SIGNAL(stateChanged(int)), SLOT(stateChanged(int)));
}

void CheckBoxItem::stateChanged(int index)
{
    if (originalValue.type() != QVariant::Invalid)
    {
        Qt::CheckState origState = this->originalValue.toBool() ? Qt::Checked : Qt::Unchecked;
        item->setFont(0, QFont("", 8, (index != origState) ? QFont::Bold : QFont::Normal));
    }
    item->setData(this->column, Qt::UserRole, (index == Qt::Checked));
}
