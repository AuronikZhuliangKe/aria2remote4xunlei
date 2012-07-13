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

#include "download.h"
#include <QFileDialog>
#include <QMouseEvent>
#include <QMenu>
#include <QMessageBox>
#include "add_http_ftp_magnetlink.h"
#include "selecttorrentfiles.h"
#include "util.h"

Download::Download():
        m_item(NULL)
{
}

Download::~Download()
{
//    if (m_item)
//    {
//        if (m_rootItem)
//        {
//            int index = m_rootItem->indexOfTopLevelItem(m_item);
//            if (index != -1)
//                m_rootItem->takeTopLevelItem(index);
//        }
//        delete m_item;
//    }
}

int Download::addHttpFtp(QString &sUrl, QMap<QString, Variant> &vCurrentParam)
{
    m_type = HTTP_FTP;
    m_sURI = sUrl;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addMagnetLink(QString &sMagnetLink, QMap<QString, Variant> &vCurrentParam)
{
    m_type = MAGNETLINK;
    m_sURI = sMagnetLink;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addTorrent(QString &Torrent, QMap<QString, Variant> &vCurrentParam)
{
    m_type = TORRENT;
    m_sURI = Torrent;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addMetalink(QString &sMetalink, QMap<QString, Variant> &vCurrentParam)
{
    m_type = METALINK;
    m_sURI = sMetalink;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addPeerList(QMap<QString, Variant> &vCurrentParam)
{
    m_type = GET_PEER_LIST;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addVersionInfo(QMap<QString, Variant> &vCurrentParam)
{
    m_type = VERSION_INFO;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addGetGlobalOptions(QMap<QString, Variant> &vCurrentParam)
{
    m_type = GET_GLOBAL_OPTIONS;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addChangeGlobalOptions(QMap<QString, Variant> &vCurrentParam)
{
    m_type = CHANGE_GLOBAL_OPTIONS;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addGetLocalOptions(QMap<QString, Variant> &vCurrentParam)
{
    m_type = GET_LOCAL_OPTIONS;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addChangeLocalOptions(QMap<QString, Variant> &vCurrentParam)
{
    m_type = CHANGE_LOCAL_OPTIONS;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

//Actions
int Download::addUnPause(QMap<QString, Variant> &vCurrentParam)
{
    m_type = ACTION_UNPAUSE;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addUnPauseAll(QMap<QString, Variant> &vCurrentParam)
{
    m_type = ACTION_UNPAUSE_ALL;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addPause(QMap<QString, Variant> &vCurrentParam)
{
    m_type = ACTION_PAUSE;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addPauseAll(QMap<QString, Variant> &vCurrentParam)
{
    m_type = ACTION_PAUSE_ALL;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addRemove(QMap<QString, Variant> &vCurrentParam)
{
    m_type = ACTION_REMOVE;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addPurge(QMap<QString, Variant> &vCurrentParam)
{
    m_type = ACTION_PURGE;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addPowerOff(QMap<QString, Variant> &vCurrentParam)
{
    m_type = ACTION_POWER_OFF;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addForcePause(QMap<QString, Variant> &vCurrentParam)
{
    m_type = ACTION_FORCE_PAUSE;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addForcePauseAll(QMap<QString, Variant> &vCurrentParam)
{
    m_type = ACTION_FORCE_PAUSE_ALL;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addForceRemove(QMap<QString, Variant> &vCurrentParam)
{
    m_type = ACTION_FORCE_REMOVE;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::addForcePowerOff(QMap<QString, Variant> &vCurrentParam)
{
    m_type = ACTION_FORCE_POWER_OFF;
    m_vCurrentParam.unite(vCurrentParam);
    return 0;
}

int Download::toXML()
{
    QVariantList parameters;
    QMap<QString, Variant> methodNameParam, params;
    QString MethodName;

    params = m_vCurrentParam;
    switch(m_type)
    {
    case MAGNETLINK:
    case HTTP_FTP:
        {
            QVariantList Url;
            Url << Variant(m_sURI);

            parameters << Variant(Url) << Variant(params);
            MethodName = QString("aria2.addUri");
        }
        break;

    case TORRENT:
        {
            QFile f(m_sURI);

            if (f.open(QFile::ReadOnly))
            {
                QByteArray array = f.readAll();
                QVariantList vparam;
                f.close();

                parameters << Variant(array) << Variant(vparam) << Variant(params);
                MethodName = QString("aria2.addTorrent");
            }
            else
            {
                //error
            }
        }
        break;

    case METALINK:
        {
            QFile f(m_sURI);

            if (f.open(QFile::ReadOnly))
            {
                QByteArray array = f.readAll();
                f.close();

                parameters << Variant(array) << Variant(params);
                MethodName = QString("aria2.addMetalink");
            }
            else
            {
                //error
            }
        }
        break;

    case GET_PEER_LIST:
        {
            parameters << Variant(params);
            MethodName = QString("aria2.getPeers");
        }
        break;

    case VERSION_INFO:
        {
            MethodName = QString("aria2.getVersion");
        }
        break;

    case GET_GLOBAL_OPTIONS:
        {
            MethodName = QString("aria2.getGlobalOption");
        }
        break;

    case CHANGE_GLOBAL_OPTIONS:
        {
            parameters << Variant(params);
            MethodName = QString("aria2.changeGlobalOption");
        }
        break;

    case GET_LOCAL_OPTIONS:
        {
            parameters << Variant(params);
            MethodName = QString("aria2.getOption");
        }
        break;

    case CHANGE_LOCAL_OPTIONS:
        {
            parameters << Variant(params);
            MethodName = QString("aria2.changeOption");
        }
        break;

        //Actions
    case ACTION_UNPAUSE:
        {
            parameters << params["gid"];
            MethodName = QString("aria2.unpause");
        }
        break;
    case ACTION_UNPAUSE_ALL:
        {
            MethodName = QString("aria2.unpauseAll");
        }
        break;
    case ACTION_PAUSE:
        {
            parameters << params["gid"];
            MethodName = QString("aria2.pause");
        }
        break;
    case ACTION_PAUSE_ALL:
        {
            MethodName = QString("aria2.pauseAll");
        }
        break;
    case ACTION_REMOVE:
        {
            parameters << params["gid"];
            MethodName = QString("aria2.remove");
        }
        break;
    case ACTION_PURGE:
        {
            MethodName = QString("aria2.purgeDownloadResult");
        }
        break;
    case ACTION_POWER_OFF:
        {
            MethodName = QString("aria2.shutdown");
        }
        break;
    case ACTION_FORCE_PAUSE:
        {
            parameters << params["gid"];
            MethodName = QString("aria2.forcePause");
        }
        break;
    case ACTION_FORCE_PAUSE_ALL:
        {
            MethodName = QString("aria2.forcePauseAll");
        }
        break;
    case ACTION_FORCE_REMOVE:
        {
            parameters << params["gid"];
            MethodName = QString("aria2.forceRemove");
        }
        break;
    case ACTION_FORCE_POWER_OFF:
        {
            MethodName = QString("aria2.forceShutdown");
        }
        break;
    case UNKNOWN:
        break;

    default:
        break;
    }
    
    if (MethodName.size() >  0)
    {
        methodNameParam["methodName"] = MethodName;
        methodNameParam["params"] = parameters;
        m_params << Variant(methodNameParam);	
    }
    return 0;
}
