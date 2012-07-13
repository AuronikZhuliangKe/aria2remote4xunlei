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

#include "server.h"
#include <QUrl>
#include <QDir>

Server::Server():
    m_AllProxyEnabled(false)
{
}

void Server::LoadServerList()
{
    m_serverList = util::LoadServerList();
    m_AllProxyEnabled = util::AllProxyEnabled;
}

void Server::SaveServerList()
{
    util::AllProxyEnabled = m_AllProxyEnabled;
    util::SaveServerList(m_serverList);
}

QList<SERVER_ITEM> Server::GetServers(SERVER_TYPE type)
{
    QList<SERVER_ITEM> ret;

    foreach (SERVER_ITEM si, m_serverList)
    {
        if (si.type == type)
            ret << si;
    }

    return ret;
}

void Server::AddServer(SERVER_ITEM &si)
{
    m_serverList << si;
    SaveServerList();
}

void Server::DeleteServer(SERVER_TYPE type, QString server, QString user)
{
    QList<SERVER_ITEM>::iterator i = m_serverList.begin();
    while (i != m_serverList.end())
    {
        SERVER_ITEM si = *i;
        if ( (si.type == type) && (si.server == server) && (si.user == user) )
        {
            i = m_serverList.erase(i);
        }
        else
            ++i;
    }
    SaveServerList();
}

void Server::DeleteServer(SERVER_TYPE type, int iIndex)
{
    int iCounter = 0;
    QList<SERVER_ITEM>::iterator i = m_serverList.begin();
    while (i != m_serverList.end())
    {
        SERVER_ITEM si = *i;
        if (si.type == type)
        {
            if (iCounter == iIndex )
            {
                m_serverList.erase(i);
                break;
            }
            iCounter++;
        }
        ++i;
    }
    SaveServerList();
}

void Server::ModifyServer(SERVER_TYPE type, int iIndex, SERVER_ITEM &si)
{
    int iCounter = 0;
    QList<SERVER_ITEM>::iterator i = m_serverList.begin();
    while (i != m_serverList.end())
    {
        SERVER_ITEM &sitem = *i;
        if (sitem.type == type)
        {
            if (iCounter == iIndex )
            {
                sitem = si;
                break;
            }
            iCounter++;
        }
        ++i;
    }
    SaveServerList();
}

bool Server::MatchedServer(QString sURI, SERVER_ITEM &si)
{
    bool bRet = false;
    QUrl sURL(sURI);

    QList<SERVER_ITEM> list = GetServers(SERVER_HTTP_FTP);
    foreach (SERVER_ITEM sitem, list)
    {
        QUrl serverURL(sitem.server);
        if ( (sURL.scheme().size() != 0) && (sURL.host().size() != 0) && (sURL.scheme() == serverURL.scheme()) && (sURL.host() == serverURL.host()) )
        {
            si = sitem;
            bRet = true;
            break;
        }
    }
    return bRet;
}
