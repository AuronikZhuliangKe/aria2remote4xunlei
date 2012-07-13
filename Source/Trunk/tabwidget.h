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
 
#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include "QTreeWidgetItemEx.h"
#include "bargraph.h"
#include "xmlrpc.h"
#include "util.h"
#include "peerinfo.h"
#include "torrentclient.h"

using namespace xmlrpc;
using namespace util;

namespace Ui {
    class TabWidget_Details;
}

typedef struct Trackers_List
{
    QString name;
    QTreeWidgetItemEx* item;
}TRACKERS_LIST, *PTRACKERS_LIST;

typedef struct Peer_List
{
    QString host;
    quint64 upSpeed;
    quint64 downSpeed;
}PEER_LIST, *PPEER_LIST;

typedef struct Server_List
{
    QString uri;
}SERVER_LIST, *PSERVER_LIST;

typedef struct Peer_Server_List
{
    QTreeWidgetItemEx* item;
    PEER_LIST peer;
    SERVER_LIST server;
}SERVER_PEER_LIST, *PSERVER_PEER_LIST;

typedef struct Files_List
{
    XMLRPC_FILES file;
    QTreeWidgetItemEx* item;
}FILES_LIST, *PFILES_LIST;

class DetailsTabView : public QTabWidget {
    Q_OBJECT
public:
    DetailsTabView(QWidget *parent = 0);
    ~DetailsTabView();

    void setTabPages(xmlrpc::XmlRPC &dw);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TabWidget_Details *ui;

    //bars
    bargraph *m_progress;
    bargraph *m_pieces;
    bargraph *m_availability;

    QMap<QString, SERVER_PEER_LIST> m_tabViewPeersServers;
    QMap<QString, FILES_LIST> m_tabViewFiles;
    QMap<QString, TRACKERS_LIST> m_tabViewTrackers;
    XML_TYPE m_currentType;

    void setTabPageGeneral(xmlrpc::XmlRPC &dw);
    void setTabPageTrackers(xmlrpc::XmlRPC &dw);
    void setTabPagePeers(xmlrpc::XmlRPC &dw);
    void setTabPageFiles(xmlrpc::XmlRPC &dw);

    //Peer info
    PeerInfo m_PeerInfo;
    TorrentClient m_TorrentClient;
};

#endif // TABWIDGET_H
