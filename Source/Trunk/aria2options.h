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

#ifndef ARIA2OPTIONS_H
#define ARIA2OPTIONS_H

#include <QDialog>
#include <QDomDocument>
#include "util.h"
#include "server.h"

using namespace util;
namespace Ui {
    class Aria2Options;
}

class Aria2Options : public QDialog {
    Q_OBJECT
public:
    Aria2Options(QWidget *parent = 0);
    ~Aria2Options();
    void LoadConnectionList();
    void SaveConnectionList();
    util::CONNECTION getConnection() { return m_connection; }
    void setConnection(const util::CONNECTION &connection);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Aria2Options *ui;
    util::CONNECTION m_connection;

    //Server
    Server m_servers;
private slots:
    void on_checkBox_Enable_Proxy_stateChanged(int );
    void on_buttonBox_accepted();

    void on_pushButton_Servers_Add_clicked();
    void on_pushButton_Servers_Delete_clicked();
    void on_pushButton_Servers_Modify_clicked();
    void on_treeWidget_Server_List_itemSelectionChanged();
};

#endif // ARIA2OPTIONS_H
