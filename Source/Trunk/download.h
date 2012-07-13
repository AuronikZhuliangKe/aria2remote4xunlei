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

#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "xmlrpc.h"
#include "QTreeWidgetItemEx.h"
#include <QMap>

using namespace xmlrpc;

const QString GlobalChangeableOption[]= {
    "max-overall-download-limit",
    "max-overall-upload-limit",
    "max-concurrent-downloads"
};

class Download
{
public:
    Download();
    Download(QTreeWidget *rootItem) {m_rootItem = rootItem; }
    ~Download();
    void setRootItem(QTreeWidget *rootItem) {m_rootItem = rootItem; }

    //TreeWidgetItem
    QTreeWidgetItem* GetTreeWidgetItemIndex() {return m_item; }
    void SetTreeWidgetItemIndex(QTreeWidgetItem* item) { m_item = item; }
    QVariantList getParamList() { return m_params; }
    QString getURI() {return m_sURI;}
    QMap<QString, Variant> getCurrentParam() {return m_vCurrentParam;}
    void setCurrentParam(const QMap<QString, Variant> &param, bool Mode = true)
    {
        if (Mode)
        {
            QMap<QString, Variant>::const_iterator i = param.constBegin();
            while (i != param.constEnd())
            {
                m_vCurrentParam.insert(i.key(), i.value());
                ++i;
            }
        }else
        {
            m_vCurrentParam = param;
        }
    }

    //add download
    int addHttpFtp(QString &sUrl, QMap<QString, Variant> &vCurrentParam);
    int addMagnetLink(QString &sMagnetLink, QMap<QString, Variant> &vCurrentParam);
    int addTorrent(QString &Torrent, QMap<QString, Variant> &vCurrentParam);
    int addMetalink(QString &sMetalink, QMap<QString, Variant> &vCurrentParam);

    //add other request
    int addPeerList(QMap<QString, Variant> &vCurrentParam);
    int addVersionInfo(QMap<QString, Variant> &vCurrentParam);
    int addGetGlobalOptions(QMap<QString, Variant> &vCurrentParam);
    int addChangeGlobalOptions(QMap<QString, Variant> &vCurrentParam);
    int addGetLocalOptions(QMap<QString, Variant> &vCurrentParam);
    int addChangeLocalOptions(QMap<QString, Variant> &vCurrentParam);

    //add Actions
    int addUnPause(QMap<QString, Variant> &vCurrentParam);
    int addUnPauseAll(QMap<QString, Variant> &vCurrentParam);
    int addPause(QMap<QString, Variant> &vCurrentParam);
    int addPauseAll(QMap<QString, Variant> &vCurrentParam);
    int addRemove(QMap<QString, Variant> &vCurrentParam);
    int addPurge(QMap<QString, Variant> &vCurrentParam);
    int addPowerOff(QMap<QString, Variant> &vCurrentParam);
    int addForcePause(QMap<QString, Variant> &vCurrentParam);
    int addForcePauseAll(QMap<QString, Variant> &vCurrentParam);
    int addForceRemove(QMap<QString, Variant> &vCurrentParam);
    int addForcePowerOff(QMap<QString, Variant> &vCurrentParam);

    //xml
    int toXML();
    XML_TYPE getType() { return m_type; }

private:
    QString m_sURI;
    QMap<QString, Variant> m_vCurrentParam;
    QVariantList m_params;
    QTreeWidgetItem *m_item;
    QTreeWidget *m_rootItem;
    XML_TYPE m_type;

};

#endif // DOWNLOAD_H
