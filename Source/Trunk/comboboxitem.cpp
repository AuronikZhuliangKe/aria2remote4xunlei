#include "comboboxitem.h"

ComboBoxItem::ComboBoxItem(QWidget *parent) :
    QComboBox(parent)
{
}

ComboBoxItem::ComboBoxItem(QTreeWidgetItem *item, int column, const Variant &value)
{
    this->item = item;
    this->column = column;
    this->originalValue = value;
    connect(this, SIGNAL(currentIndexChanged(int)), SLOT(changeItem(int)));
}

void ComboBoxItem::changeItem(int index)
{
    if (index >= 0)
    {
        if (originalValue.type() != QVariant::Invalid)
        {
            item->setFont(0, QFont("", 8, (this->itemText(index).compare(originalValue.toString(), Qt::CaseInsensitive) != 0) ? QFont::Bold : QFont::Normal));
        }
        item->setData(this->column, Qt::UserRole, this->itemText(index));
    }
}

void ComboBoxItem::clear()
{
    QComboBox::clear();
}
