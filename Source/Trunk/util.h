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
 
#ifndef UTIL_H
#define UTIL_H

#include <QtGui/QApplication>
#include <QString>
#include <QIcon>
#include <QFileInfo>
#include <QByteArray>
#include <QDomDocument>
#include "metainfo.h"
#include "gzip/zlib.h"
#include <QMap>
#include "xmlrpc.h"
#include "features.h"
#include "aria2cparameter.h"
#include <QDomElement>

namespace util
{
using namespace xmlrpc;

    enum URI_TYPE {URI_TYPE_NONE = -1, URI_TYPE_HTTP_FTP = 0, URI_TYPE_MULTI_HTTP_FTP, URI_TYPE_TORRENT, URI_TYPE_MAGNETLINK, URI_TYPE_METALINK, URI_TYPE_ALL};

    const static QMap<QString, Aria2cParameter> CreateAria2cParameters()
    {
        QMap<QString, Aria2cParameter> m;
        //Basic
        m["dir"] = Aria2cParameter().addType(QVariant::String);
        m["split"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 256);
        m["max-tries"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 256);
        m["timeout"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 64 * 1024).addSuffix(" s");
        m["lowest-speed-limit"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 1024 * 1024 * 1024).addSuffix(" KiB/s").addDiv(1024);
        m["max-download-limit"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 1024 * 1024 * 1024).addSuffix(" KiB/s").addDiv(1024);
        m["max-file-not-found"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 1024);
        m["max-resume-failure-tries"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 1024);
        m["min-split-size"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 1024 * 1024 * 1024).addSuffix(" KiB").addDiv(1024);
        m["max-connection-per-server"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 1024);
        m["continue"] = Aria2cParameter().addType(QVariant::Bool);
        m["log-level"] = Aria2cParameter().addType(QVariant::StringList).addList(QList<QString> () << QString("debug") << QString("info") << QString("notice") << QString("warn") << QString("error"));
        //FTP
        m["ftp-pasv"] = Aria2cParameter().addType(QVariant::StringList).addList(QList<QString> () << QString("active") << QString("passive"));
        m["connect-timeout"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 64 * 1024).addSuffix(" s");;
        m["ftp-reuse-connection"] = Aria2cParameter().addType(QVariant::Bool);
        m["ftp-type"] = Aria2cParameter().addType(QVariant::StringList).addList(QList<QString> () << QString("binary") << QString("ascii"));
        //HTTP
        m["enable-http-keep-alive"] = Aria2cParameter().addType(QVariant::Bool);
        m["enable-http-pipelining"] = Aria2cParameter().addType(QVariant::Bool);
        m["http-accept-gzip"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_VERSION_191);
        m["http-auth-challenge"] = Aria2cParameter().addType(QVariant::Bool);
        m["http-no-cache"] = Aria2cParameter().addType(QVariant::Bool);
        m["conditional-get"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_VERSION_1100);
        m["user-agent"] = Aria2cParameter().addType(QVariant::String);
        m["connect-timeout"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 64 * 1024).addSuffix(" s");
        //Torrent
        m["bt-enable-lpd"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_VERSION_190 | util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-hash-check-seed"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-metadata-only"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-require-crypto"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-save-metadata"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-seed-unverified"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_VERSION_1100);
        m["follow-torrent"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["enable-peer-exchange"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["seed-ratio"] = Aria2cParameter().addType(QVariant::Double).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-stop-timeout"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 64 * 1024).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["max-upload-limit"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 1024 * 1024 * 1024).addFeatures(util::ARIA2C_VERSION_1100);
        m["bt-max-open-files"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 1024).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-max-peers"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 1024).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["seed-time"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 365*24*3600).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-external-ip"] = Aria2cParameter().addType(QVariant::String).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-prioritize-piece"] = Aria2cParameter().addType(QVariant::String).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-remove-unselected-file"] = Aria2cParameter().addType(QVariant::String).addFeatures(util::ARIA2C_VERSION_1150 | util::ARIA2C_FEATURES_BITTORRENT);

        m["bt-tracker-interval"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 64 * 1024).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-tracker-timeout"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 64 * 1024).addFeatures(util::ARIA2C_VERSION_191 | util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-tracker-connect-timeout"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 64 * 1024).addFeatures(util::ARIA2C_VERSION_191 | util::ARIA2C_FEATURES_BITTORRENT);
        m["peer-id-prefix"] = Aria2cParameter().addType(QVariant::String).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-request-peer-speed-limit"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 1024 * 1024 * 1024).addFeatures(util::ARIA2C_FEATURES_BITTORRENT).addSuffix(" KiB/s").addDiv(1024);

        m["bt-min-crypto-level"] = Aria2cParameter().addType(QVariant::StringList).addList(QList<QString> () << QString("arc4") << QString("plain")).addFeatures(util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-tracker"] = Aria2cParameter().addType(QVariant::String).addFeatures(util::ARIA2C_VERSION_1101 | util::ARIA2C_FEATURES_BITTORRENT);
        m["bt-exclude-tracker"] = Aria2cParameter().addType(QVariant::String).addFeatures(util::ARIA2C_VERSION_1101 | util::ARIA2C_FEATURES_BITTORRENT);

        //Metalink
        m["metalink-preferred-protocol"] = Aria2cParameter().addType(QVariant::StringList).addList(QList<QString> () << QString("http") << QString("https") << QString("ftp") << QString("none")).addFeatures(util::ARIA2C_FEATURES_METALINK);
        m["follow-metalink"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_FEATURES_METALINK);
        m["metalink-enable-unique-protocol"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_FEATURES_METALINK);
        m["metalink-servers"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 1024).addFeatures(util::ARIA2C_FEATURES_METALINK);
        m["metalink-base-uri"] = Aria2cParameter().addType(QVariant::String).addFeatures(util::ARIA2C_VERSION_1112 | util::ARIA2C_FEATURES_METALINK);

        m["metalink-language"] = Aria2cParameter().addType(QVariant::String).addFeatures(util::ARIA2C_FEATURES_METALINK);
        m["metalink-os"] = Aria2cParameter().addType(QVariant::String).addFeatures(util::ARIA2C_FEATURES_METALINK);
        m["metalink-location"] = Aria2cParameter().addType(QVariant::String).addFeatures(util::ARIA2C_FEATURES_METALINK);
        m["metalink-version"] = Aria2cParameter().addType(QVariant::String).addFeatures(util::ARIA2C_FEATURES_METALINK);

        //Advanced
        m["allow-overwrite"] = Aria2cParameter().addType(QVariant::Bool);
        m["allow-piece-length-change"] = Aria2cParameter().addType(QVariant::Bool);
        m["always-resume"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_VERSION_191);
        m["async-dns"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_FEATURES_ASYNCDNS);
        m["async-dns6"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_VERSION_1101 | util::ARIA2C_FEATURES_ASYNCDNS);
        m["auto-file-renaming"] = Aria2cParameter().addType(QVariant::Bool);
        m["check-integrity"] = Aria2cParameter().addType(QVariant::Bool);
        m["dry-run"] = Aria2cParameter().addType(QVariant::Bool);
        m["rpc-allow-origin-all"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_VERSION_1130);

        m["reuse-uri"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_VERSION_190);
        m["remove-control-file"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_VERSION_191);
        m["remote-time"] = Aria2cParameter().addType(QVariant::Bool);
        m["realtime-chunk-checksum"] = Aria2cParameter().addType(QVariant::Bool);
        m["no-netrc"] = Aria2cParameter().addType(QVariant::Bool);
        m["parameterized-uri"] = Aria2cParameter().addType(QVariant::Bool);
        m["use-head"] = Aria2cParameter().addType(QVariant::Bool);
        m["pause"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_VERSION_1120);
        m["hash-check-only"] = Aria2cParameter().addType(QVariant::Bool).addFeatures(util::ARIA2C_VERSION_1130);

        m["no-file-allocation-limit"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 1024 * 1024 * 1024).addSuffix(" KiB/s").addDiv(1024);
        m["piece-length"] = Aria2cParameter().addType(QVariant::Int).addMinMax(1, 1024 * 1024 * 1024).addSuffix(" KiB/s").addFeatures(util::ARIA2C_VERSION_1130).addDiv(1024);
        m["file-allocation"] = Aria2cParameter().addType(QVariant::StringList).addList(QList<QString> () << QString("none") << QString("prealloc") << QString("falloc"));
        m["proxy-method"] = Aria2cParameter().addType(QVariant::StringList).addList(QList<QString> () << QString("get") << QString("tunnel"));

        m["retry-wait"] = Aria2cParameter().addType(QVariant::Int).addMinMax(0, 1024).addFeatures(util::ARIA2C_VERSION_1110);
        m["stream-piece-selector"] = Aria2cParameter().addType(QVariant::StringList).addList(QList<QString> () << QString("default") << QString("inorder")).addFeatures(util::ARIA2C_VERSION_1120);
        m["download-result"] = Aria2cParameter().addType(QVariant::StringList).addList(QList<QString> () << QString("default") << QString("full"));
        m["referer"] = Aria2cParameter().addType(QVariant::String);
        m["header"] = Aria2cParameter().addType(QVariant::String);
        return m;
    }

    const static QMap<QString, Aria2cParameter> Aria2cParams = CreateAria2cParameters();

    QString ConvertNumberToSuffixString(qint64 number);
    QIcon getWinIcon(QFileInfo &fileInfo, QString &sDesc);
    bool ReadTorrentFile(QString sTorrentfile, MetaInfo& metainfo);
    void SetToolBar();
    QByteArray RC4(QByteArray data);
    void deletePrePostSpace(QString &str);


    QString getHomePath();

    QByteArray gzipDecompress(const QByteArray& compressedData);

    //Configuration file load/save
    typedef struct _Templates
    {
        QMap<QString, Variant> value;
        QString name;
        URI_TYPE type;
    }TEMPLATES, *PTEMPLATES;

    QMap<QString, Variant> TemplateFromName(const QList<util::TEMPLATES> &temp, const QString &Name, URI_TYPE type = URI_TYPE_ALL );

    typedef struct _Connection
    {
        QString host;
        QString user;
        QString password;
        int port;
        QString proxyServer;
        QString proxyUser;
        QString proxyPassword;
        int proxyPort;
        bool enableProxy;
    } CONNECTION, *PCONNECTION;

    static QString Server_Type_String[] = {"All Proxy", "HTTP Proxy", "HTTPS Proxy", "FTP Proxy", "HTTP FTP"};
    enum SERVER_TYPE {
        SERVER_ALL_PROXY = 0,
        SERVER_HTTP_PROXY,
        SERVER_HTTPS_PROXY,
        SERVER_FTP_PROXY,
        SERVER_HTTP_FTP
    };

    typedef struct Server_Item
    {
        QString server;
        QString user;
        QString password;
        SERVER_TYPE type;
        int port;
    }SERVER_ITEM, *PSERVER_ITEM;

    static CONNECTION connectionNode;
    static QMap<QString, QMap<QString, QString> > uiSettingNode;
    static QList<SERVER_ITEM> serverListNode;
    static QList<util::TEMPLATES> templateNode;
    static bool AllProxyEnabled = false;

    void LoadConfigurationAll();
    QDomNode LoadConfigurationNode(const QString &node);
    void SaveConfigurationNode();

    //load
    CONNECTION LoadConnectionList();
    const QString LoadSetting(const QString &Element, const QString &name);
    QList<SERVER_ITEM> LoadServerList();
    QList<TEMPLATES> LoadTemplates(URI_TYPE Element = URI_TYPE_ALL);

    //save
    void SaveConnectionList(QDomDocument &doc, QDomElement &root);
    void SaveConnectionList(const CONNECTION &connection);
    void SaveSetting(QDomDocument &doc, QDomElement &root);
    void SaveSetting(const QString &Element, const QString &name, const QString &value);
    void SaveTemplate(QDomDocument &doc, QDomElement &root);
    void SaveTemplate(const QList<util::TEMPLATES> &temp);
    void SaveServerList(QDomDocument &doc, QDomElement &root);
    void SaveServerList(const QList<SERVER_ITEM> &serverlist);
}

#endif // UTIL_H
