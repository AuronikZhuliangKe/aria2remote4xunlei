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

#include "xmlrpc.h"

namespace xmlrpc
{
    XmlRPC::XmlRPC():
            m_download_type(UNKNOWN),
            m_completedLength(0),
            m_connections(0),
            m_downloadSpeed(0),
            m_gid(0),
            m_numPieces(0),
            m_pieceLength(0),
            m_status(""),
            m_iStatus(STATUS_UNKNOWN),
            m_totalLength(0),
            m_uploadLength(0),
            m_uploadSpeed(0),
            m_numSeeders(0),
            m_iError(0)
    {
    }

    int XmlRPC::ParseResponseXML(QVariant &params)
    {
        QMapIterator<QString,QVariant> it(params.toMap());
        m_download_type = HTTP_FTP;
        while( it.hasNext() )
        {
            it.next();

            QString key = it.key();
            if (key == "bitfield")
            {
                m_bitfield = ConvertBitfieldToBitArray(it.value().toString());
            } else if (key == "completedLength")
            {
                bool ok;
                m_completedLength = it.value().toULongLong(&ok);
            } else if (key == "connections")
            {
                bool ok;
                m_connections = it.value().toUInt(&ok);
            } else if (key == "dir")
            {
                m_dir = it.value().toString();
            } else if (key == "downloadSpeed")
            {
                bool ok;
                m_downloadSpeed = it.value().toUInt(&ok);
            } else if (key == "files")
            {
                GetFiles(it.value());
            } else if (key == "gid")
            {
                bool ok;
                m_gid = it.value().toULongLong(&ok);
            } else if (key == "numPieces")
            {
                bool ok;
                m_numPieces = it.value().toUInt(&ok);

                if ( (m_bitfield.size() > 0) && (m_bitfield.size() > m_numPieces))
                {
                    while (m_bitfield.size() > m_numPieces)
                    {
                        //erase unused piece indicator
                        m_bitfield.pop_back();
                    }
                }
            } else if (key == "pieceLength")
            {
                bool ok;
                m_pieceLength = it.value().toUInt(&ok);
            } else if (key == "status")
            {
                m_iStatus = STATUS_UNKNOWN;
                m_status = it.value().toString();
                if (m_status == "active")
                {
                    m_iStatus = STATUS_ACTIVE;
                } else if (m_status == "waiting")
                {
                    m_iStatus = STATUS_WAITING;
                } else if (m_status == "paused")
                {
                    m_iStatus = STATUS_PAUSED;
                } else if (m_status == "error")
                {
                    m_iStatus = STATUS_ERROR;
                } else if (m_status == "complete")
                {
                    m_iStatus = STATUS_COMPLETE;
                } else if (m_status == "removed")
                {
                    m_iStatus = STATUS_REMOVED;
                }
            } else if (key == "totalLength")
            {
                bool ok;
                m_totalLength = it.value().toULongLong(&ok);
            } else if (key == "uploadLength")
            {
                bool ok;
                m_uploadLength = it.value().toUInt(&ok);
            } else if (key == "uploadSpeed")
            {
                bool ok;
                m_uploadSpeed = it.value().toUInt(&ok);
            } else if (key == "bittorrent")
            {
                GetBittorrent(it.value());
            } else if (key == "infoHash")
            {
                m_infoHash = it.value().toString();
            } else if (key == "numSeeders")
            {
                bool ok;

                //torrent downloaded
                m_download_type = TORRENT;
                m_numSeeders = it.value().toUInt(&ok);
            } else if (key == "errorCode")
            {
                m_errorCode = it.value().toString();
            } /*else if (key == "followedBy")
            {
                m_followedBy = it.value();
            } else if (key == "belongsTo")
            {
                m_belongsTo = it.value();
            }*/
        }
        return 0;
    }

    std::vector<quint64> XmlRPC::ConvertBitfieldToBitArray(QString sBitField)
    {
        size_t iSize = 0;

        if (m_numPieces > 0)
            iSize = m_numPieces;
        else
            iSize = sBitField.size() * 4;

        std::vector<quint64>  bitfield;

        for (size_t i = 0; i < iSize; i++)
        {
            unsigned char cBit = (unsigned char)sBitField[i / 4].toLower().toAscii();
            bool bRet = (((unsigned char) (cBit < 'a') ? (cBit - '0') : (cBit - 'a' + 10) ) & (8 >> (i % 4))) != 0;
            bitfield.push_back(bRet ? (1) : (0));
        }

        return bitfield;
    }

    int XmlRPC::GetFiles(const QVariant &value)
    {
        QList<QVariant> elems = value.toList();
        int size = elems.size();
        for (int i = 0; i < size; ++i)
        {
            QVariant &item = elems[i];
            if (Variant::Map == item.type())
            {
                XMLRPC_FILES f;
                QMapIterator<QString,QVariant> it(item.toMap());

                while( it.hasNext() )
                {
                    it.next();

                    QString key = it.key();
                    if (key == "index")
                    {
                        bool ok;
                        f.index = it.value().toUInt(&ok);
                    } else if (key == "length")
                    {
                        bool ok;
                        f.length = it.value().toUInt(&ok);
                    } else if (key == "path")
                    {
                        f.path = it.value().toString();
                    } else if (key == "selected")
                    {
                        f.selected = it.value().toBool();
                    } else if (key == "uris")
                    {
                        f.uris = GetFilesUris(it.value());
                    }
                }
                m_files << f;
            }
        }
        return 1;
    }

    QList<XMLRPC_FILES_URIS> XmlRPC::GetFilesUris(const QVariant &value)
    {
        QList<QVariant> elems = value.toList();
        QList<XMLRPC_FILES_URIS> Ret;
        int size = elems.size();

        for (int i = 0; i < size; ++i)
        {
            QVariant &item = elems[i];

            if (Variant::Map == item.type())
            {
                XMLRPC_FILES_URIS u;
                QMapIterator<QString,QVariant> it(item.toMap());

                while( it.hasNext() )
                {
                    it.next();

                    QString key = it.key();
                    if (key == "status")
                    {
                        u.status = it.value().toString();
                    } else if (key == "uri")
                    {
                        u.uri = it.value().toString();
                    }
                }
                Ret << u;
            }
        }
        return Ret;
    }

    int XmlRPC::GetBittorrent(const QVariant &value)
    {
        QMap<QString,QVariant> elems(value.toMap());

        m_mode = elems["mode"].toString();
        m_creationDateTime.setTime_t(elems["creationDate"].toInt());

        QMapIterator<QString,QVariant> it_info(elems["info"].toMap());
        while( it_info.hasNext() )
        {
            it_info.next();
            XMLRPC_BITTORRENT_INFO info;
            info.name = it_info.value().toString();
            m_info << info;
        }

        QList<QVariant> it_announceList(elems["announceList"].toList());
        size_t it_announceList_size = it_announceList.size();

        for (size_t i = 0; i < it_announceList_size; ++i)
        {
            QList<QVariant> it_list = it_announceList[i].toList();

            foreach (QVariant v, it_list)
            {
                m_announceList << v.toString();
            }
        }
        return 1;
    }

    void XmlRPC::setServers(const QVariant &value)
    {
        if (Variant::Map == value.type())
        {
            XMLRPC_SERVER_LIST server;
            QMapIterator<QString,QVariant> it(value.toMap());

            while( it.hasNext() )
            {
                it.next();

                QString key = it.key();
                if (key == "index")
                {
                    bool ok;
                    server.index = it.value().toULongLong(&ok);
                } else if (key == "servers")
                {
                    QList<QVariant> serverItem = it.value().toList();
                    int serverItemCount = serverItem.size();
                    for (int si = 0; si < serverItemCount; si++)
                    {
                        QVariant &sitem = serverItem[si];
                        if (Variant::Map == sitem.type())
                        {
                            QMapIterator<QString,QVariant> sIter(sitem.toMap());
                            XMLRPC_SERVER serverItem;
                            while (sIter.hasNext())
                            {
                                sIter.next();
                                QString key = sIter.key();
                                if (key == "currentUri")
                                {
                                    serverItem.currentUri = sIter.value().toString();
                                } else if (key == "downloadSpeed")
                                {
                                    bool ok;
                                    serverItem.downloadSpeed = sIter.value().toULongLong(&ok);
                                } else if (key == "uri")
                                {
                                    serverItem.uri = sIter.value().toString();
                                }
                            }
                            server.servers << serverItem;
                        }
                    }
                }
            }
            m_serverList << server;
        }
    }

    void XmlRPC::setPeers(const QVariant &value)
    {
        if (Variant::Map == value.type())
        {
            XMLRPC_PEER_LIST peer;
            QMapIterator<QString,QVariant> it(value.toMap());

            while( it.hasNext() )
            {
                it.next();

                QString key = it.key();
                if (key == "amChoking")
                {
                    peer.amChoking = it.value().toBool();
                } else if (key == "bitfield")
                {
                    peer.bitfield = ConvertBitfieldToBitArray(it.value().toString());
                } else if (key == "downloadSpeed")
                {
                    bool ok;
                    peer.download = it.value().toUInt(&ok);
                } else if (key == "ip")
                {
                    peer.ip = it.value().toString();
                } else if (key == "peerChoking")
                {
                    peer.peerChoking = it.value().toBool();
                } else if (key == "peerId")
                {
                    peer.peerId = QString(QByteArray::fromPercentEncoding(it.value().toString().toAscii()));
                } else if (key == "port")
                {
                    bool ok;
                    peer.port = it.value().toUInt(&ok);
                } else if (key == "seeder")
                {
                    peer.seeder =it.value().toBool();
                } else if (key == "uploadSpeed")
                {
                    bool ok;
                    peer.uploadSpeed = it.value().toUInt(&ok);
                }
            }
            m_peerList << peer;
        }
    }
	
    void XmlRPC::versionInfo(const QVariant &value)
    {
        QVariantList values = value.toList();
        QVariantList itemVersionInfoList(values[0].toList());
        int itemVersioninfoSize = itemVersionInfoList.size();

        for (int i = 0; i < itemVersioninfoSize; i++)
        {
             QVariant &item(itemVersionInfoList[i]);
             if (Variant::Map == item.type())
             {
                 QMapIterator<QString,QVariant> it(item.toMap());
                 while( it.hasNext() )
                 {
                     it.next();

                     QString key = it.key();
                     if (key == "version")
                     {
                         m_versionInfo.version = it.value().toString();
                     } else if (key == "enabledFeatures")
                     {
                         m_versionInfo.enabledFeatures = it.value().toStringList();
                     }
                 }
             }
        }
    }

    QString XmlRPC::getName()
    {
        QString sRet;

        switch (m_download_type)
        {
        case HTTP_FTP:
            {
                if (m_files.size() > 0)
                {
                    sRet = m_files[0].path;
                    if ( (sRet.size() == 0) && (m_files[0].uris.size() > 0) )
                    {
                        sRet = m_files[0].uris[0].uri;
                    }
                }
            }
            break;
        case TORRENT:
        case METALINK:
            sRet = (m_mode == "multi") ? (m_info[0].name) : (m_files[0].path);
            break;
        default:
            break;
        }
        QFileInfo info(sRet);
        return info.fileName();
    }

    quint64 XmlRPC::getTotalLength()
    {
        quint64 uiRet = m_totalLength;
        if ( (uiRet == 0) && (m_files.size() > 0))
        {
            uiRet = m_files[0].length;
        }
        return uiRet;
    }

    QString XmlRPC::getsProcessPercent()
    {
        quint64 uiTotalLength = getTotalLength();
        QString sRet;

        if (uiTotalLength != 0)
        {
            sRet = QString::number(m_completedLength * 100 / uiTotalLength) + "%";
        }
        else
        {
            sRet = "0%";
        }
        return sRet;
    }

    quint64 XmlRPC::getProcessPercent()
    {
        quint64 uiTotalLength = getTotalLength();
        quint64 uRet = 0;

        if (uiTotalLength != 0)
        {
            uRet = m_completedLength * 100 / uiTotalLength;
        }
        return uRet;
    }
    QString XmlRPC::getsSeeders()
    {
        QString sRet;

        if ( (m_download_type == TORRENT) || (m_download_type == MAGNETLINK) )
        {
            sRet = QString::number(m_numSeeders);
        }
        return sRet;
    }

    QString XmlRPC::getsTracker()
    {
        QString sRet;

        if ( (m_download_type == TORRENT) || (m_download_type == MAGNETLINK) )
        {
            foreach (QString tracker, m_announceList)
            {
                if (sRet.size() > 0)
                {
                    sRet += ";";
                }
                sRet += tracker;
            }
        }

        return sRet;
    }

    QString XmlRPC::getsRatio()
    {
        QString sRet;

        if ( (m_download_type == TORRENT) || (m_download_type == MAGNETLINK) )
        {
            if (m_completedLength > 0)
            {
                sRet =  QString::number( (double)m_uploadLength / (double)m_completedLength, 'f', 3) + "%";
            }
            else
            {
                sRet = "0.000%";
            }
        }
        return sRet;
    }

    double XmlRPC::getRatio()
    {
        double dRet = 0.00;

        if ( ((m_download_type == TORRENT) || (m_download_type == MAGNETLINK)) && (m_completedLength > 0) )
        {
            dRet = (double)m_uploadLength / (double)m_completedLength;
        }
        return dRet;
    }

    QString XmlRPC::getsETA()
    {
        QString sRet("n/a");
        if (m_downloadSpeed != 0)
        {
            quint64 remsec = (getTotalLength() - m_completedLength) / m_downloadSpeed;
            quint64 hr = remsec / 3600;
            remsec %= 3600;
            quint64 min = remsec / 60;
            remsec %= 60;

            sRet = "";
            sRet += ( (hr > 0) ? (QString::number(hr) + "h") : ("") );
            sRet += ( (min > 0) ? (QString::number(min) + "m") : ("") );
            sRet += ( (remsec > 0) ? (QString::number(remsec) + "s") : ("") );
        }
        return sRet;
    }

    quint64 XmlRPC::getETA()
    {
        #ifdef Q_CC_MSVC
        quint64 uRet = _UI64_MAX;
        #else
        quint64 uRet = LONG_LONG_MAX;
        #endif
        if (m_downloadSpeed != 0)
        {
            uRet = (getTotalLength() - m_completedLength) / m_downloadSpeed;
        }
        return uRet;
    }

    QMap<QString, Variant> XmlRPC::OptionToMap(const QVariant &value)
    {
        QMap<QString, Variant> mRet;
        QList<QVariant> elems = value.toList()[0].toList();
        QVariant &item = elems[0];
        if (Variant::Map == item.type())
        {
            QMapIterator<QString, QVariant> it(item.toMap());
            while( it.hasNext() )
            {
                it.next();
                QString key = it.key();
                Variant v = Variant(it.value().toString());
                mRet[key] = v;
            }
        }
        return mRet;
    }
};
