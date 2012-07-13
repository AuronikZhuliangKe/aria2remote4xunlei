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

#ifndef XMLRPC_H
#define XMLRPC_H

#include "xmlrpc/variant.h"
#include "xmlrpc/request.h"
#include "xmlrpc/response.h"
#include "xmlrpc/client.h"
#include <QString>
#include <QByteArray>

namespace xmlrpc
{
    enum XML_TYPE {UNKNOWN                = 0,
                   HTTP_FTP               = 1,
                   TORRENT                = 2,
                   MAGNETLINK             = 4,
                   METALINK               = 8,
                   GET_PEER_LIST          = 16,
                   VERSION_INFO           = 32,

                   //Options
                   GET_GLOBAL_OPTIONS     = 64,
                   GET_LOCAL_OPTIONS      = 128,
                   CHANGE_GLOBAL_OPTIONS  = 256,
                   CHANGE_LOCAL_OPTIONS   = 512,

                   //Actions
                   ACTION_UNPAUSE         = 1024,
                   ACTION_UNPAUSE_ALL     = 2048,
                   ACTION_PAUSE           = 4096,
                   ACTION_PAUSE_ALL       = 8192,
                   ACTION_REMOVE          = 16384,
                   ACTION_PURGE           = 32768,
                   ACTION_POWER_OFF       = 65536,
                   ACTION_FORCE_PAUSE     = 131072,
                   ACTION_FORCE_PAUSE_ALL = 262144,
                   ACTION_FORCE_REMOVE    = 524288,
                   ACTION_FORCE_POWER_OFF = 1048576,

                   PERIODIC_REQUEST       = 2097152
    };
    enum URI_STATUS {
        STATUS_UNKNOWN  = 0,
        STATUS_ACTIVE   = 1,
        STATUS_WAITING  = 2,
        STATUS_PAUSED   = 3,
        STATUS_ERROR    = 4,
        STATUS_COMPLETE = 5,
        STATUS_REMOVED  = 6
    };

    typedef struct xmlrpc_version_info
    {
        QList<QString> enabledFeatures;
        QString version;
    }XMLRPC_VERSION_INFO, *PXMLRPC_VERSION_INFO;

    typedef struct xmlrpc_server
    {
        QString uri;
        QString currentUri;
        quint64 downloadSpeed;
    }XMLRPC_SERVER, *PXMLRPC_SERVER;

    typedef struct xmlrpc_server_list
    {
        quint64 index;
        QList<XMLRPC_SERVER> servers;
    }XMLRPC_SERVER_LIST, *PXMLRPC_SERVER_LIST;

    typedef struct xmlrpc_files_uris
    {
        QString status;
        QString uri;
    }XMLRPC_FILES_URIS, *PXMLRPC_FILES_URIS;

    typedef struct xmlrpc_files
    {
        quint32 index;
        quint64 length;
        QString path;
        bool selected;
        QList<XMLRPC_FILES_URIS> uris;

    }XMLRPC_FILES, *PXMLRPC_FILES;

    typedef struct xmlrpc_bittorrent_info
    {
        QString name;
    }XMLRPC_BITTORRENT_INFO, *PXMLRPC_BITTORRENT_INFO;

    typedef struct xmlrpc_peer_list
    {
        bool amChoking;
        std::vector<quint64> bitfield;
        quint32 download;
        QString ip;
        bool peerChoking;
        QString peerId;
        quint32 port;
        bool seeder;
        quint32 uploadSpeed;
    }XMLRPC_PEER_LIST, *PXMLRPC_PEER_LIST;

    class XmlRPC
    {
    public:
        XmlRPC();
        int ParseResponseXML(QVariant &params);

        QString getName();
        //Download information
        XML_TYPE getType() { return m_download_type; }
        qint64 getGID() {return m_gid;}
        std::vector<quint64> getBitfield() { return m_bitfield; }
        quint64 getCompletedLength() {return m_completedLength;}
        quint32 getConnections() {return m_connections;}
        QString getDir() {return m_dir;}
        quint32 getDownloadSpeed() {return m_downloadSpeed;}
        QList<XMLRPC_FILES> getFiles() {return m_files;}
        quint32 getNumPieces() {return m_numPieces;}
        quint32 getPieceLength() {return m_pieceLength;}
        QString getStatus() {return m_status;}
        QString getsProcessPercent();
        quint64 getProcessPercent();
        URI_STATUS getiStatus() {return m_iStatus;}
        quint64 getTotalLength();
        quint64 getUploadLength() {return m_uploadLength;}
        quint32 getUploadSpeed() {return m_uploadSpeed;}
        QString getErrorCode() {return m_errorCode;}

        //torrent
        QString getInfoHash() {return m_infoHash;}
        quint32 getNumSeeders() {return m_numSeeders;}
        QString getsSeeders();
        QList<QString> getAnnounceList() {return m_announceList;}
        QDateTime getCreationDateTime() {return m_creationDateTime;}
        QList<XMLRPC_BITTORRENT_INFO> getInfo() {return m_info;}
        QString getMode() {return m_mode;}
        QString getComment() {return m_comment;}
        QString getsRatio();
        double getRatio();
        QString getsETA();
        quint64 getETA();
        QString getsTracker();

        //get Server
        void setServers(const QVariant &value);
        void setPeers(const QVariant &value);

        const QList<XMLRPC_PEER_LIST>& getPeerList(){ return m_peerList;}
        const QList<XMLRPC_SERVER_LIST>& getServerList() {return m_serverList;}

        //version info
        void versionInfo(const QVariant &value);
        XMLRPC_VERSION_INFO getVersionInfo(){ return m_versionInfo; }

        //Options
        QMap<QString, Variant> OptionToMap(const QVariant &value);


    private:
        std::vector<quint64> ConvertBitfieldToBitArray(QString sBitField);
        int GetFiles(const QVariant &value);
        QList<XMLRPC_FILES_URIS> GetFilesUris(const QVariant &value);
        int GetBittorrent(const QVariant &value);
        int GetBittorrentInfo(const QVariant &value);

    private:

        //download type
        XML_TYPE m_download_type;

        //return data
        std::vector<quint64> m_bitfield;
        quint64 m_completedLength;
        quint32 m_connections;
        QString m_dir;
        quint32 m_downloadSpeed;
        QList<XMLRPC_FILES> m_files;
        qint64 m_gid;
        quint32 m_numPieces;
        quint32 m_pieceLength;
        QString m_status;
        URI_STATUS m_iStatus;
        quint64 m_totalLength;
        quint64 m_uploadLength;
        quint32 m_uploadSpeed;
        QString m_errorCode;

        //torrent specific
        QString m_infoHash;
        quint32 m_numSeeders;
        QList<QString> m_announceList;
        QDateTime m_creationDateTime;
        QList<XMLRPC_BITTORRENT_INFO> m_info;
        QString m_mode;
        QString m_comment;

        //Peer list
        QList<XMLRPC_PEER_LIST> m_peerList;

        //Server list
        QList<XMLRPC_SERVER_LIST> m_serverList;

        //error
        QString m_sError;
        quint32 m_iError;

        //version info
        XMLRPC_VERSION_INFO m_versionInfo;
    };
};

#endif // XMLRPC_H
