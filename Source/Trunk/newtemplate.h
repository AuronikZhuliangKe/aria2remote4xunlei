#ifndef NEWTEMPLATE_H
#define NEWTEMPLATE_H

#include <QDialog>
#include "requestthread.h"
#include "util.h"

namespace Ui {
    class newTemplate;
}

using namespace util;

class newTemplate : public QDialog
{
    Q_OBJECT

public:
    explicit newTemplate(QWidget *parent = 0);
    void setTemplates(const QList<util::TEMPLATES> &t);
    QList<util::TEMPLATES> getTemplates() {return temp; }
    ~newTemplate();

private slots:
    void on_comboBoxType_currentIndexChanged(int index);
    void on_pushButton_OK_clicked();
    void GetGlobalOptions(QVariant value);
private:
    Ui::newTemplate *ui;
    QList<util::TEMPLATES> temp;
    RequestThread request;
    QMap<QString, Variant> globalOptions;
};

#endif // NEWTEMPLATE_H
