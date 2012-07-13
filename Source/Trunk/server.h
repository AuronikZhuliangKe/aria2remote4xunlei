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

#ifndef SERVER_H
#define SERVER_H
#include <QString>
#include <QDomDocument>
#include "util.h"

using namespace util;

class Server
{
    public:
        Server();

        //Server configuration file load/save
        void LoadServerList();
        void SaveServerList();

        QList<SERVER_ITEM> GetServers(SERVER_TYPE type);
        QList<SERVER_ITEM> GetServers() {return m_serverList; }
        void AddServer(SERVER_ITEM &si);
        void DeleteServer(SERVER_TYPE type, QString server, QString user);
        void DeleteServer(SERVER_TYPE type, int iIndex);
        void ModifyServer(SERVER_TYPE type, int iIndex, SERVER_ITEM &si);
        bool MatchedServer(QString sURI, SERVER_ITEM &si);
        void SetAllProxyEnabled(bool enabled) { m_AllProxyEnabled = enabled; }
        bool GetAllProxyEnabled() { return m_AllProxyEnabled; }

    private:
        QList<SERVER_ITEM> m_serverList;
        bool m_AllProxyEnabled;
};

#endif // SERVER_H
