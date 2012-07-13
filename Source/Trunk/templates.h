#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <QDialog>
#include "util.h"

namespace Ui {
    class Templates;
}
using namespace util;

extern quint32 g_uiAria2cVersion;

class Templates : public QDialog
{
    Q_OBJECT

public:
    explicit Templates(QWidget *parent = 0);
    ~Templates();
    void setTemplates(QList<util::TEMPLATES> &t);
    QList<util::TEMPLATES> getTemplates() {return temp; }
private slots:
    void on_pushButton_New_clicked();
    void on_pushButton_Save_clicked();
    void on_comboBoxNew_currentIndexChanged(int index);

    void on_pushButton_Delete_clicked();

    void on_buttonBox_accepted();

private:
    Ui::Templates *ui;
    QList<util::TEMPLATES> temp;
    QList<util::TEMPLATES> tempOriginal;
    qint32 lastIndex;

private:
    void setTemplate(const util::TEMPLATES &t);
};

#endif // TEMPLATES_H
