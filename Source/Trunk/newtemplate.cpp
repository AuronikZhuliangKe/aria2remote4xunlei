#include "newtemplate.h"
#include "ui_newtemplate.h"
#include <QMessageBox>
#include "localoptions.h"

newTemplate::newTemplate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newTemplate)
{
    ui->setupUi(this);
    util::CONNECTION connection = util::LoadConnectionList();
    request.setConnection(connection);
    connect(&request, SIGNAL(ResponseGetGlobalOptions(QVariant)), this, SLOT(GetGlobalOptions(QVariant)));
    request.start();
}

newTemplate::~newTemplate()
{
    delete ui;
}

void newTemplate::on_comboBoxType_currentIndexChanged(int index)
{
    ui->comboBox_cloneFrom->clear();
    ui->comboBox_cloneFrom->addItem(tr("Default"));
    foreach (TEMPLATES t, temp)
    {
        if (t.type == index)
            ui->comboBox_cloneFrom->addItem(t.name);
    }
}

void newTemplate::setTemplates(const QList<util::TEMPLATES> &t)
{
    temp = t;
    on_comboBoxType_currentIndexChanged(0);
}

void newTemplate::on_pushButton_OK_clicked()
{
    if (ui->lineEdit->text().size() == 0)
    {
        QMessageBox::critical(this, tr("Error"),tr("The template name can not be empty!"));
        return;
    }
    foreach (util::TEMPLATES t, temp)
    {
        if (t.name.compare(ui->lineEdit->text(), Qt::CaseInsensitive) == 0)
        {
            QMessageBox::critical(this, tr("Error"),tr("The template name is already exists!"));
            return;
        }
    }
    Download d;
    QMap<QString, Variant> vCurrentParam;

    d.addGetGlobalOptions(vCurrentParam);
    request.addRequest(d);
    request.setDescriptionText(tr("Get global variables"));
    request.wakeUp();
}

void newTemplate::GetGlobalOptions(QVariant value)
{
    XmlRPC x;
    Download d;
    int retValue = QDialog::Accepted;
    QString url;
    QMap<QString, Variant> param;
    int index = ui->comboBoxType->currentIndex();
    QString str = ui->comboBox_cloneFrom->currentText();

    globalOptions = x.OptionToMap(value);

    switch (index)
    {
        case URI_TYPE_HTTP_FTP:
        case URI_TYPE_MULTI_HTTP_FTP: d.addHttpFtp(url, param); break;
        case URI_TYPE_TORRENT: d.addTorrent(url, param); break;
        case URI_TYPE_MAGNETLINK: d.addMagnetLink(url, param); break;
        case URI_TYPE_METALINK: d.addMetalink(url, param); break;
        default: break;
    }

    foreach (TEMPLATES t, temp)
    {
        if ( (t.type == index) && (str.compare(t.name, Qt::CaseInsensitive) == 0) )
        {
            d.setCurrentParam(t.value);
            break;
        }
    }

    LocalOptions lo(this);
    lo.setOptions(globalOptions, d);
    retValue = lo.exec();
    if (retValue == QDialog::Accepted)
    {
        TEMPLATES newTemp;
        newTemp.value = lo.getLocalOption();
        newTemp.name = ui->lineEdit->text();
        newTemp.type = (URI_TYPE)index;
        temp << newTemp;
        accept();
    }
}

