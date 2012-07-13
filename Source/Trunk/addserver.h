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

#ifndef ADDSERVER_H
#define ADDSERVER_H

#include <QDialog>
#include "server.h"

namespace Ui {
    class AddServer;
}

class AddServer : public QDialog {
    Q_OBJECT
public:
    AddServer(QWidget *parent = 0);
    ~AddServer();
    void setVisiblePort(bool bVisible);
    QString getServer();
    void setServer(QString s);

    QString getUser();
    void setUser(QString u);

    QString getPassword();
    void setPassword(QString p);

    int getPort();
    void setPort(int p);

    void setServerList(QList<SERVER_ITEM> si) {m_server = si; }

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AddServer *ui;
    QList<SERVER_ITEM> m_server;

private slots:

    void on_pushButton_OK_clicked();
};

#endif // ADDSERVER_H
