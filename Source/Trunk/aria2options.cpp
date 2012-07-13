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
 
#include "aria2options.h"
#include "ui_aria2options.h"
#include "progressbarviewdelegate.h"
#include "addserver.h"
#include <QFile>

Aria2Options::Aria2Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Aria2Options)
{
    ui->setupUi(this);

    m_connection.host = "";
    m_connection.user = "";
    m_connection.password = "";
    m_connection.port = 6800;
    m_connection.proxyServer = "";
    m_connection.proxyUser = "";
    m_connection.proxyPassword = "";
    m_connection.proxyPort = 8080;

    ui->lineEdit_RemoteHost->setText(m_connection.host);
    ui->lineEdit_UserName->setText(m_connection.user);
    ui->lineEdit_Password->setText(m_connection.password);
    ui->spinBox_Port->setValue(m_connection.port);

    ui->lineEdit_Proxy_Server->setText(m_connection.proxyServer);
    ui->lineEdit_Proxy_UserName->setText(m_connection.proxyUser);
    ui->lineEdit_Proxy_Password->setText(m_connection.proxyPassword);
    ui->spinBox_Proxy_Port->setValue(m_connection.proxyPort);

    m_servers.LoadServerList();
    foreach (SERVER_ITEM si, m_servers.GetServers())
    {
        if (si.type == SERVER_HTTP_FTP)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(0, si.server);
            item->setText(1, si.user);

            ui->treeWidget_Server_List->addTopLevelItem(item);
        }
    }

    ui->pushButton_Servers_Delete->setEnabled(false);
    ui->pushButton_Servers_Modify->setEnabled(false);
}

Aria2Options::~Aria2Options()
{
    delete ui;
}

void Aria2Options::changeEvent(QEvent *e)
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

void Aria2Options::setConnection(const util::CONNECTION &connection)
{
    m_connection = connection;

    ui->lineEdit_RemoteHost->setText(m_connection.host);
    ui->lineEdit_UserName->setText(m_connection.user);
    ui->lineEdit_Password->setText(m_connection.password);
    ui->spinBox_Port->setValue(m_connection.port);

    ui->lineEdit_Proxy_Server->setText(m_connection.proxyServer);
    ui->lineEdit_Proxy_UserName->setText(m_connection.proxyUser);
    ui->lineEdit_Proxy_Password->setText(m_connection.proxyPassword);
    ui->spinBox_Proxy_Port->setValue(m_connection.proxyPort);

    ui->checkBox_Enable_Proxy->setCheckState(m_connection.enableProxy ? (Qt::Checked) :(Qt::Unchecked));

    ui->lineEdit_Proxy_Server->setEnabled(m_connection.enableProxy);
    ui->lineEdit_Proxy_UserName->setEnabled(m_connection.enableProxy);
    ui->lineEdit_Proxy_Password->setEnabled(m_connection.enableProxy);
    ui->spinBox_Proxy_Port->setEnabled(m_connection.enableProxy);
}

void Aria2Options::on_buttonBox_accepted()
{
    m_connection.host = ui->lineEdit_RemoteHost->text();
    m_connection.user = ui->lineEdit_UserName->text();
    m_connection.password = ui->lineEdit_Password->text();
    m_connection.port = ui->spinBox_Port->value();
    m_connection.proxyServer =ui->lineEdit_Proxy_Server->text();
    m_connection.proxyUser = ui->lineEdit_Proxy_UserName->text();
    m_connection.proxyPassword = ui->lineEdit_Proxy_Password->text();
    m_connection.proxyPort = ui->spinBox_Proxy_Port->value();
    m_connection.enableProxy = (ui->checkBox_Enable_Proxy->checkState() == Qt::Checked);
}

void Aria2Options::on_checkBox_Enable_Proxy_stateChanged(int value)
{
    bool isEnabled = (value == Qt::Checked);

    ui->lineEdit_Proxy_Server->setEnabled(isEnabled);
    ui->lineEdit_Proxy_UserName->setEnabled(isEnabled);
    ui->lineEdit_Proxy_Password->setEnabled(isEnabled);
    ui->spinBox_Proxy_Port->setEnabled(isEnabled);
}

void Aria2Options::on_pushButton_Servers_Add_clicked()
{
    AddServer addserver(this);
    addserver.setVisiblePort(false);

    if (addserver.exec() == QDialog::Accepted)
    {
        SERVER_ITEM s;

        s.server = addserver.getServer();
        s.user = addserver.getUser();
        s.password = addserver.getPassword();
        s.type = SERVER_HTTP_FTP;
        s.port = 0;
        m_servers.AddServer(s);

        QTreeWidgetItem *item = new QTreeWidgetItem;

        item->setText(0, s.server);
        item->setText(1, s.user);

        ui->treeWidget_Server_List->addTopLevelItem(item);

        QTreeWidgetItem *itemCurrent = ui->treeWidget_Server_List->currentItem();
        ui->pushButton_Servers_Delete->setEnabled(itemCurrent != NULL);
        ui->pushButton_Servers_Modify->setEnabled(itemCurrent != NULL);
    }
}

void Aria2Options::on_pushButton_Servers_Delete_clicked()
{
    if (QMessageBox::question(this, tr("Delete"), tr("Really want to delete the server?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        int iIndex = ui->treeWidget_Server_List->indexOfTopLevelItem(ui->treeWidget_Server_List->currentItem());

        m_servers.DeleteServer(SERVER_HTTP_FTP, iIndex);
        QTreeWidgetItem *item = ui->treeWidget_Server_List->takeTopLevelItem(iIndex);
        if (item != 0)
        {
            delete item;
        }
        QTreeWidgetItem * itemCurrent = ui->treeWidget_Server_List->currentItem();
        ui->pushButton_Servers_Delete->setEnabled(itemCurrent != NULL);
        ui->pushButton_Servers_Modify->setEnabled(itemCurrent != NULL);
    }
}

void Aria2Options::on_pushButton_Servers_Modify_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget_Server_List->currentItem();
    int iIndex = ui->treeWidget_Server_List->indexOfTopLevelItem(item);
    SERVER_ITEM s = m_servers.GetServers(SERVER_HTTP_FTP)[iIndex];

    AddServer addserver(this);
    addserver.setServer(s.server);
    addserver.setUser(s.user);
    addserver.setPassword(s.password);
    addserver.setPort(s.port);
    addserver.setVisiblePort(false);
    addserver.setServerList(m_servers.GetServers(SERVER_HTTP_FTP));
    addserver.setWindowTitle(tr("Modify %1 server").arg(Server_Type_String[SERVER_HTTP_FTP]));

    if (addserver.exec() == QDialog::Accepted)
    {
        s.server = addserver.getServer();
        s.user = addserver.getUser();
        s.password = addserver.getPassword();
        m_servers.ModifyServer(SERVER_HTTP_FTP, iIndex, s);

        QTreeWidgetItem *item = ui->treeWidget_Server_List->currentItem();

        item->setText(0, s.server);
        item->setText(1, s.user);
    }
}

void Aria2Options::on_treeWidget_Server_List_itemSelectionChanged()
{
    QTreeWidgetItem *item = ui->treeWidget_Server_List->currentItem();
    ui->pushButton_Servers_Delete->setEnabled(item != NULL);
    ui->pushButton_Servers_Modify->setEnabled(item != NULL);
}
