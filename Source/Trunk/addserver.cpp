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

#include "addserver.h"
#include "ui_addserver.h"
#include <QMessageBox>

AddServer::AddServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddServer)
{
    ui->setupUi(this);
}

AddServer::~AddServer()
{
    delete ui;
}

void AddServer::changeEvent(QEvent *e)
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

void AddServer::setVisiblePort(bool bVisible)
{
    ui->spinBox_Port->setVisible(bVisible);
    ui->label_Port->setVisible(bVisible);
}

QString AddServer::getServer()
{
    return ui->lineEdit_Server->text();
}

void AddServer::setServer(QString s)
{
    ui->lineEdit_Server->setText(s);
}

QString AddServer::getUser()
{
    return ui->lineEdit_User->text();
}

void AddServer::setUser(QString u)
{
    ui->lineEdit_User->setText(u);
}

QString AddServer::getPassword()
{
    return ui->lineEdit_Password->text();
}

void AddServer::setPassword(QString p)
{
    ui->lineEdit_Password->setText(p);
}

int AddServer::getPort()
{
    return ui->spinBox_Port->value();
}
void AddServer::setPort(int p)
{
    ui->spinBox_Port->setValue(p);
}

void AddServer::on_pushButton_OK_clicked()
{
    if (ui->lineEdit_Server->text().size() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("The server name can not be empty!"), QMessageBox::Ok);
        return;
    }
    if (ui->lineEdit_User->text().size() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("The user name can not be empty!"), QMessageBox::Ok);
        return;
    }
    if (ui->lineEdit_Password->text().size() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("The password can not be empty!"), QMessageBox::Ok);
        return;
    }

    foreach (SERVER_ITEM si, m_server)
    {
        if ( (si.server == ui->lineEdit_Server->text()) && (si.user == ui->lineEdit_User->text()) )
        {
            QMessageBox::critical(this, tr("Error"), tr("Server and user name already exists!"), QMessageBox::Ok);
            return;
        }
    }
    accept();
}
