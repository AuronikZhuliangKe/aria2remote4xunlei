/*************************************************************************
 *                                                                       *
 * Aria2c Remote Control - UI for Aria2c                                 *
 * Copyright (C) 2010 Zoltan Molnar                                      *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                       *
 * Contact : <molnarzoltan@users.sourceforge.net>                        *
 *                                                                       *
 *************************************************************************/

#include "add_http_ftp_magnetlink.h"
#include "ui_add_http_ftp_magnetlink.h"

add_http_ftp_magnetlink::add_http_ftp_magnetlink(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_http_ftp_magnetlink)
{
    ui->setupUi(this);

    QByteArray DialogSize = QByteArray::fromBase64(util::LoadSetting("AddHttpFtpMagnetlink", "Geometry").toAscii());
    if (DialogSize.size() > 0)
    {
        restoreGeometry(DialogSize);
    }
}

add_http_ftp_magnetlink::~add_http_ftp_magnetlink()
{
    util::SaveSetting("AddHttpFtpMagnetlink", "Geometry", QString( saveGeometry().toBase64()));
    delete ui;
    delete menuTemplate;
    delete MenuGroup;
}

void add_http_ftp_magnetlink::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString add_http_ftp_magnetlink::GetURI()
{
    return ui->lineEdit->text();
}

void add_http_ftp_magnetlink::SetURI(QString& s)
{
    ui->lineEdit->setText(s);
}

void add_http_ftp_magnetlink::slotMenuChanged(QAction* action)
{
    if (action != NULL)
    {
        templateName = action->data().toString();
    }
}

void add_http_ftp_magnetlink::SetMenu(URI_TYPE t, const QList<util::TEMPLATES> &temp, bool enableTemplateButton)
{
    type = t;
    templates = temp;
    setWindowIcon( (t == URI_TYPE_HTTP_FTP) ? QIcon(":/icon/uri/ftp.png") : QIcon(":/icon/uri/magnet.png") );
    ui->label->setText( (t == URI_TYPE_HTTP_FTP) ? ("URL:") : ("Magnet Link:") );
    setWindowTitle( (t == URI_TYPE_HTTP_FTP) ? tr("Add URL") : tr("Add Magnet Link") );
    ui->pushButton_template->setEnabled(enableTemplateButton);

    MenuGroup = new QActionGroup(ui->pushButton_template);
    MenuGroup->setExclusive(true);

    connect(MenuGroup, SIGNAL(triggered(QAction *)), this, SLOT(slotMenuChanged(QAction *)));
    menuTemplate = new QMenu(this);

    //add default menu item
    QAction *menuDefault = new QAction(tr("Default"), this);
    menuDefault->setCheckable(true);
    menuDefault->setChecked(true);
    menuDefault->setData("Default_item");
    menuTemplate->addAction( menuDefault );
    MenuGroup->addAction(menuDefault);

    foreach (util::TEMPLATES temp, templates)
    {
        if ( ( (t == URI_TYPE_HTTP_FTP) && (temp.type == URI_TYPE_HTTP_FTP) ) ||
             ( (t == URI_TYPE_MAGNETLINK) && (temp.type == URI_TYPE_MAGNETLINK) ) )
        {
            QAction *menuItem = new QAction(temp.name, this);
            menuItem->setCheckable(true);
            menuItem->setData(temp.name);
            menuTemplate->addAction(menuItem);
            MenuGroup->addAction(menuItem);
        }
    }
    ui->pushButton_template->setMenu(menuTemplate);
}
