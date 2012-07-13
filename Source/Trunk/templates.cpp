#include "templates.h"
#include "ui_templates.h"
#include "newtemplate.h"
#include "comboboxitem.h"
#include "checkboxitem.h"
#include "lineedititem.h"
#include "spinboxitem.h"
#include "doublespinboxitem.h"
#include <QMessageBox>

Templates::Templates(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Templates),
    lastIndex(-1)
{
    ui->setupUi(this);
    ui->pushButton_Delete->setEnabled(false);
    ui->pushButton_Save->setEnabled(false);
}

Templates::~Templates()
{
    delete ui;
}

void Templates::on_pushButton_New_clicked()
{
    newTemplate nt(this);

    nt.setTemplates(temp);
    if (nt.exec() == QDialog::Accepted)
    {
        temp = nt.getTemplates();
        tempOriginal << temp.back();
        setTemplate(temp.back());
        ui->pushButton_Delete->setEnabled(ui->comboBoxNew->count() != 0);
        ui->pushButton_Save->setEnabled(ui->comboBoxNew->count() != 0);
    }
}

void Templates::setTemplate(const util::TEMPLATES &t)
{
    QIcon icon;
    if (t.type == URI_TYPE_HTTP_FTP)
        icon.addFile(":/icon/uri/ftp.png");
    else if (t.type == URI_TYPE_MULTI_HTTP_FTP)
        icon.addFile(":/icon/uri/http.png");
    else if (t.type == URI_TYPE_TORRENT)
        icon.addFile(":/icon/uri/bittorrent.png");
    else if (t.type == URI_TYPE_MAGNETLINK)
        icon.addFile(":/icon/uri/magnet.png");
    else if (t.type == URI_TYPE_METALINK)
        icon.addFile(":/icon/uri/metalink.png");

    if (!icon.isNull())
        ui->comboBoxNew->addItem(icon, t.name);
}

void Templates::setTemplates(QList<util::TEMPLATES> &t)
{
    tempOriginal = temp = t;
    foreach (TEMPLATES temp, t)
    {
        setTemplate(temp);
    }
    ui->pushButton_Delete->setEnabled(ui->comboBoxNew->count() != 0);
    ui->pushButton_Save->setEnabled(ui->comboBoxNew->count() != 0);
}

void Templates::on_pushButton_Save_clicked()
{
    int index = ui->comboBoxNew->currentIndex();

    int topCount = ui->treeWidget->topLevelItemCount();
    for (int i = 0; i < topCount; i++)
    {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
        temp[index].value[item->text(0)] = item->data(1, Qt::UserRole).toString();
    }

    util::SaveTemplate(temp);
    tempOriginal = temp;
    QMessageBox::information(this, tr("Save"), tr("Modified templates saved"));
    on_comboBoxNew_currentIndexChanged(ui->comboBoxNew->currentIndex());
}

void Templates::on_comboBoxNew_currentIndexChanged(int index)
{
    if ( (index == -1) || (temp.size() == 0) )
        return;
    //save
    if (lastIndex != -1)
    {
        int topCount = ui->treeWidget->topLevelItemCount();
        for (int i = 0; i < topCount; i++)
        {
            QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
            temp[lastIndex].value[item->text(0)] = item->data(1, Qt::UserRole).toString();
        }
    }
    //clear and load
    ui->treeWidget->clear();
    QMapIterator<QString, Variant> m(temp.at(index).value);
    while (m.hasNext())
    {
        m.next();
        QTreeWidgetItem *item = new QTreeWidgetItem;
        ui->treeWidget->addTopLevelItem(item);

        QString key(m.key());
        item->setText(0, key);
        QString originalValue = tempOriginal.at(index).value[key].toString();
        Aria2cParameter params = Aria2cParams[key];
        bool enabledItem = (params.getFeatures() & util::ARIA2C_VERSION_MASK) <= g_uiAria2cVersion;
        if (enabledItem)
            item->setFlags(item->flags() | Qt::ItemIsEnabled);
        else
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        switch (params.getType())
        {
            case QVariant::String:
            {
                LineEditItem *lineedit_template = new LineEditItem(item, 1, originalValue);
                lineedit_template->setText(m.value().toString());
                ui->treeWidget->setItemWidget(item, 1, lineedit_template);
                lineedit_template->setEnabled(enabledItem);
                break;
            }
            case QVariant::Int:
            {
                SpinBoxItem *spinbox_template = new SpinBoxItem(item, 1, originalValue.toLongLong() * params.getMul() / params.getDiv());
                spinbox_template->setRange(params.getMin().toInt(), params.getMax().toInt());
                spinbox_template->setValue(m.value().toLongLong() * params.getMul() / params.getDiv());
                spinbox_template->setSuffix(params.getSuffix());
                ui->treeWidget->setItemWidget(item, 1, spinbox_template);
                spinbox_template->setEnabled(enabledItem);
                break;
            }
            case QVariant::Double:
            {
                DoubleSpinBoxItem *doublespinbox_template = new DoubleSpinBoxItem(item, 1, originalValue);
                doublespinbox_template->setRange(params.getMin().toDouble(), params.getMax().toDouble());
                doublespinbox_template->setValue(m.value().toDouble());
                doublespinbox_template->setSuffix(params.getSuffix());
                ui->treeWidget->setItemWidget(item, 1, doublespinbox_template);
                doublespinbox_template->setEnabled(enabledItem);
                break;
            }
            case QVariant::Bool:
            {
                CheckBoxItem *checkbox_template = new CheckBoxItem(item, 1, originalValue);
                checkbox_template->setCheckState(m.value().toBool() ? Qt::Checked : Qt::Unchecked);
                ui->treeWidget->setItemWidget(item, 1, checkbox_template);
                checkbox_template->setEnabled(enabledItem);
                break;
            }
            case QVariant::StringList:
            {
                QString str(m.value().toString());
                ComboBoxItem *combobox_template = new ComboBoxItem(item, 1, originalValue);
                int currentIndex = -1;
                int i = 0;
                foreach (QString s, params.getList())
                {
                    if (str.compare(s, Qt::CaseInsensitive) == 0)
                        currentIndex = i;
                    combobox_template->addItem(s);
                    i++;
                }
                if (currentIndex != -1)
                    combobox_template->setCurrentIndex(currentIndex);

                ui->treeWidget->setItemWidget(item, 1, combobox_template);
                combobox_template->setEnabled(enabledItem);
                break;
            }
            default:
                item->setFlags(!Qt::ItemIsEnabled);
                break;
        }
    }
    lastIndex = index;
}

void Templates::on_pushButton_Delete_clicked()
{
    int pos = ui->comboBoxNew->currentIndex();
    if ( (pos != -1) && (QMessageBox::question(this, tr("Delete"), tr("Do you really delete to <b>%1</b> template?").arg(temp[pos].name), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) )
    {
        ui->treeWidget->clear();
        ui->comboBoxNew->removeItem(pos);

        temp.removeAt(pos);
        tempOriginal.removeAt(pos);
        int count = ui->comboBoxNew->count();
        lastIndex = -1;
        ui->comboBoxNew->setCurrentIndex(qBound(0, pos, count - 1));
        ui->pushButton_Delete->setEnabled(count != 0);
    }
}

void Templates::on_buttonBox_accepted()
{
    util::SaveTemplate(temp);
}
