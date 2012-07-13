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

#include "util.h"
#include <QSettings>
#include <QPixmap>
#include <QPixmapCache>
#include <QDir>
#include <QDomDocument>
#include <QSysInfo>
#include <QMessageBox>

#if defined(Q_WS_WIN)
#  define _WIN32_IE 0x0500
#  include <qt_windows.h>
#  include <commctrl.h>
#  include <objbase.h>
#elif defined(Q_WS_MAC)
#  include <private/qt_cocoa_helpers_mac_p.h>
#endif

#if defined(Q_WS_WIN)

using namespace xmlrpc;

void util::SetToolBar()
{
    if (QSysInfo::windowsVersion() == QSysInfo::WV_VISTA)
    {
        if (QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS7)
        {

        }
    }
}
#endif

QString util::ConvertNumberToSuffixString(qint64 number)
{
    int iCounter = 0;
    QString sRet;
    double dNumber =(double)number;

    if (dNumber < 0) dNumber = -dNumber;

    while ( (dNumber > 1024.0) && (iCounter <= 3) )
    {
        dNumber /= 1024.0;
        iCounter++;
    }

    sRet = QString::number(dNumber, 'f', (iCounter == 0) ? (0) : (2));
    switch (iCounter)
    {
    case 0:
        sRet += (" B");
        break;
    case 1:
        sRet += (" KiB");
        break;
    case 2:
        sRet += (" MiB");
        break;
    case 3:
        sRet += (" GiB");
        break;
    default:
        sRet += (" TiB");
        break;
    }
    return sRet;
}

QIcon util::getWinIcon(QFileInfo &fileInfo, QString &sDesc)
{
    QIcon retIcon;
    const QString fileExtension = QLatin1Char('.') + fileInfo.suffix().toUpper();

    QString key;
    if (fileInfo.isFile() && !fileInfo.isExecutable() && !fileInfo.isSymLink())
        key = QLatin1String("qt_") + fileExtension;

    QPixmap pixmap;
    if (!key.isEmpty()) {
        QPixmapCache::find(key, pixmap);
    }

    if (!pixmap.isNull()) {
        retIcon.addPixmap(pixmap);
        if (QPixmapCache::find(key + QLatin1Char('l'), pixmap))
            retIcon.addPixmap(pixmap);
        return retIcon;
    }

    SHFILEINFO info;
    unsigned long val = 0;

    //Get the small icon
#ifndef Q_OS_WINCE
    val = SHGetFileInfo((const wchar_t *)QDir::toNativeSeparators(fileInfo.filePath()).utf16(), 0, &info,
                        sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_SYSICONINDEX|SHGFI_TYPENAME);
#else
    val = SHGetFileInfo((const wchar_t *)QDir::toNativeSeparators(fileInfo.filePath()).utf16(), 0, &info,
                        sizeof(SHFILEINFO), SHGFI_SMALLICON|SHGFI_SYSICONINDEX);
#endif
    if (val) {
        if (fileInfo.isDir() && !fileInfo.isRoot()) {
            //using the unique icon index provided by windows save us from duplicate keys
            key = QString::fromLatin1("qt_dir_%1").arg(info.iIcon);
            QPixmapCache::find(key, pixmap);
            if (!pixmap.isNull()) {
                retIcon.addPixmap(pixmap);
                if (QPixmapCache::find(key + QLatin1Char('l'), pixmap))
                    retIcon.addPixmap(pixmap);
                DestroyIcon(info.hIcon);
                return retIcon;
            }
        }
        if (pixmap.isNull()) {
#ifndef Q_OS_WINCE
            pixmap = QPixmap::fromWinHICON(info.hIcon);
#else
            pixmap = QPixmap::fromWinHICON(ImageList_GetIcon((HIMAGELIST) val, info.iIcon, ILD_NORMAL));
#endif
            if (!pixmap.isNull()) {
                retIcon.addPixmap(pixmap);
                if (!key.isEmpty())
                    QPixmapCache::insert(key, pixmap);
            }
            else {
              qWarning("QFileIconProviderPrivate::getWinIcon() no small icon found");
            }
        }
        DestroyIcon(info.hIcon);
    }

    //Get the big icon
#ifndef Q_OS_WINCE
    val = SHGetFileInfo((const wchar_t *)QDir::toNativeSeparators(fileInfo.filePath()).utf16(), 0, &info,
                        sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES|SHGFI_ICON|SHGFI_LARGEICON|SHGFI_SYSICONINDEX|SHGFI_TYPENAME);
#else
    val = SHGetFileInfo((const wchar_t *)QDir::toNativeSeparators(fileInfo.filePath()).utf16(), 0, &info,
                        sizeof(SHFILEINFO), SHGFI_LARGEICON|SHGFI_SYSICONINDEX);
#endif
    if (val) {
        if (fileInfo.isDir() && !fileInfo.isRoot()) {
            //using the unique icon index provided by windows save us from duplicate keys
            key = QString::fromLatin1("qt_dir_%1").arg(info.iIcon);
        }
#ifndef Q_OS_WINCE
        pixmap = QPixmap::fromWinHICON(info.hIcon);
#else
        pixmap = QPixmap::fromWinHICON(ImageList_GetIcon((HIMAGELIST) val, info.iIcon, ILD_NORMAL));
#endif
        sDesc = QString::fromWCharArray(info.szTypeName);
        if (!pixmap.isNull()) {
            retIcon.addPixmap(pixmap);
            if (!key.isEmpty())
                QPixmapCache::insert(key + QLatin1Char('l'), pixmap);
        }
        else {
            qWarning("QFileIconProviderPrivate::getWinIcon() no large icon found");
        }
        DestroyIcon(info.hIcon);
    }
    return retIcon;
}

bool util::ReadTorrentFile(QString sTorrentfile, MetaInfo& metainfo)
{
    if (sTorrentfile.isEmpty()) {
        return false;
    }

    QFile torrent(sTorrentfile);
    if (!torrent.open(QFile::ReadOnly) || !metainfo.parse(torrent.readAll())) {
        return false;
    }
    return true;
}

QByteArray key("axGmYg__ppdVx0iUEFJ__nd8cCC_3okgFBOeF0uujCy_st1_fcyWRRf2C7m1hVkQ");

void swap_RC4( unsigned int *S, unsigned int i, unsigned int j)
{
  unsigned int tmp = S[i];
  S[i] = S[j];
  S[j] = tmp;

  return;
}

QByteArray util::RC4(QByteArray data)
{
  QByteArray Ret;
  unsigned int i, j;
  unsigned int S[256];

  /** HIDDEN PROTOTYPE */
  void swap_RC4( unsigned int *, unsigned int, unsigned int );

  for( i = 0; i < 256; i++ )
          S[i] = i;

  j=0;
  for( i = 0; i < 256; i++ )
  {
     j = (j + S[i] + key[i % key.size()]) % 256;
     swap_RC4( S, i, j );
  }

  i=0; j=0;
  size_t iSize = data.size();
  for( size_t l = 0; l < iSize; l++)
  {
    i = (i + 1) % 256;
    j = (j + S[i]) % 256;
    swap_RC4( S, i, j );
    Ret += S[(S[i] + S[j]) % 256] ^ data[l];
  }
  return Ret;
}

void util::deletePrePostSpace(QString &str)
{
    //pre space
    while ( (str.size() > 0) && (str.at(0) == QChar(' ')) )
        str.remove(0, 1);

    //post space
    while ( (str.size() > 0) && (str.at(str.size() - 1) == QChar(' ')) )
        str.remove(str.size() - 1, 1);
}

QString util::getHomePath()
{
    QString sRet(QDir::homePath() + QDir::separator());

    sRet = QDir::toNativeSeparators(sRet);
#if defined(Q_WS_WIN)
    sRet += (QString("Application Data") + QDir::separator());
#endif
#if defined(Q_WS_LINUX)
     sRet += (QString(".config") + QDir::separator());
#endif

    sRet += (QString("Aria2c Remote Control") + QDir::separator());
    return sRet;
}

QByteArray util::gzipDecompress(const QByteArray& compressedData)
{
    QByteArray uncompressed;
    const quint32 OUTBUF_LENGTH = 1024 * 16;
    quint8 outbuf[OUTBUF_LENGTH];
    z_stream cmpr_stream;

    cmpr_stream.zalloc = Z_NULL;
    cmpr_stream.zfree = Z_NULL;
    cmpr_stream.opaque = Z_NULL;
    cmpr_stream.avail_in = 0;
    cmpr_stream.next_in = Z_NULL;

    // initalize z_stream with gzip/zlib format auto detection enabled.
    if (inflateInit2(&cmpr_stream, 47) != Z_OK)
    {
        return QByteArray();
    }

    cmpr_stream.avail_in = compressedData.size();
    cmpr_stream.next_in = (unsigned char *)(compressedData.data());

    forever
    {
      cmpr_stream.avail_out = OUTBUF_LENGTH;
      cmpr_stream.next_out = outbuf;

      int ret = inflate(&cmpr_stream, Z_NO_FLUSH);
      if ( (ret != Z_OK) && (ret != Z_STREAM_END) )
      {
          return QByteArray();
      }

      uncompressed.append( QByteArray::fromRawData( (char*)outbuf, OUTBUF_LENGTH - cmpr_stream.avail_out) );
      if(cmpr_stream.avail_out > 0) {
        break;
      }
    }

    if (inflateEnd(&cmpr_stream) != Z_OK)
    {
        return QByteArray();
    }

    return uncompressed;
}

QMap<QString, Variant> util::TemplateFromName(const QList<util::TEMPLATES> &temp, const QString &Name, URI_TYPE type)
{
    QMap<QString, Variant> mRet;

    foreach (util::TEMPLATES t, temp)
    {
        if ( ((type == URI_TYPE_ALL) || (type == t.type)) && (t.name.compare(Name, Qt::CaseInsensitive) == 0) )
        {
            mRet = t.value;
            break;
        }
    }

    return mRet;
}

QDomNode util::LoadConfigurationNode(const QString &node)
{
    QDomDocument doc("Aria2cRemoteConfiguration");
    QFile file(util::getHomePath() + "configuration.xml");
    QDomNode retNode;

    if (file.open(QIODevice::ReadOnly))
    {
        QString errorMsg;
        int errorline;
        int errorColumn;
        if (doc.setContent(&file, &errorMsg, &errorline, &errorColumn))
        {
            QDomElement root = doc.documentElement();
            if (root.tagName().compare("Configuration", Qt::CaseInsensitive) == 0)
            {
                QDomNode child = root.firstChild();
                while (!child.isNull())
                {
                    if (child.nodeName().compare(node, Qt::CaseInsensitive) == 0)
                    {
                        retNode = child;
                        break;
                    }
                    child = child.nextSibling();
                }
            }
        }
        file.close();
    }
    return retNode;
}

CONNECTION util::LoadConnectionList()
{
    CONNECTION connectionReturn = {QString(""), QString(""), QString(""), 6800, QString(""), QString(""), QString(""), 8080, false};
    QDomNode node = LoadConfigurationNode("ConnectionList");

    if (!node.isNull())
    {
        QDomNode child = node.firstChild();
        while(!child.isNull())
        {
            QDomElement element = child.toElement(); // try to convert the node to an element.
            if(!element.isNull())
            {
                connectionReturn.password = QString( RC4( QByteArray::fromBase64( element.attribute("Password", "").toAscii() ) ) );
                connectionReturn.user = QString( QByteArray::fromPercentEncoding(element.attribute("User", "").toAscii()) );
                connectionReturn.host = QString( QByteArray::fromPercentEncoding(element.attribute("Host", "localhost").toAscii()) );
                connectionReturn.port = element.attribute("Port", "6800").toInt();
                connectionReturn.proxyPassword = QString( RC4( QByteArray::fromBase64( element.attribute("ProxyPassword", "").toAscii() ) ) );
                connectionReturn.proxyUser = QString( QByteArray::fromPercentEncoding(element.attribute("ProxyUser", "").toAscii()) );
                connectionReturn.proxyServer = QString( QByteArray::fromPercentEncoding(element.attribute("ProxyServer", "").toAscii()) );
                connectionReturn.proxyPort = element.attribute("ProxyPort", "8080").toInt();
                connectionReturn.enableProxy = element.attribute("EnableProxy", "false").compare("true") == 0;
            }
            child = child.nextSibling();
        }
    }
    connectionNode = connectionReturn;
    return connectionReturn;
}

const QString util::LoadSetting(const QString &Element, const QString &name)
{
    QString sRet;
    if (uiSettingNode.size() > 0)
    {
        sRet = uiSettingNode[Element][name];
    }

    if (sRet.size() == 0)
    {
        uiSettingNode.clear();
        QDomNode node = LoadConfigurationNode("UISetting");
        if (!node.isNull())
        {
            QDomNode child = node.firstChild();

            while(!child.isNull())
            {
                QDomElement elem = child.toElement(); // try to convert the node to an element.
                if(elem.tagName().compare(Element, Qt::CaseInsensitive) == 0)
                {
                    sRet =  elem.attribute(name, "");
                }

                //Attributes to QMap
                QDomNamedNodeMap nodeMap = elem.attributes();
                int itemCount = nodeMap.count();
                QMap<QString, QString> m;
                for (int i = 0; i < itemCount; i++)
                {
                    m[nodeMap.item(i).nodeName()] = nodeMap.item(i).nodeValue();
                }
                uiSettingNode[elem.tagName()] = m;
                child = child.nextSibling();
            }
        }
    }
    return sRet;
}

QList<SERVER_ITEM> util::LoadServerList()
{
    QDomNode node = LoadConfigurationNode("ServerList");
    QDomNode n = node.firstChild();
    QList<SERVER_ITEM> ret;
    AllProxyEnabled = false;
    while(!n.isNull())
    {
        QDomElement element = n.toElement(); // try to convert the node to an element.
        if(!element.isNull())
        {
            if (element.nodeName() == "Server")
            {
                SERVER_ITEM s;

                s.server = QString( QByteArray::fromPercentEncoding(element.attribute("Name", "").toAscii()) );
                s.user = QString( QByteArray::fromPercentEncoding(element.attribute("User", "").toAscii()) );
                s.password = QString( RC4( QByteArray::fromBase64( element.attribute("Password", "").toAscii() ) ) );
                QString sType = element.attribute("Type", "");

                if (sType == Server_Type_String[SERVER_ALL_PROXY])
                    s.type = SERVER_ALL_PROXY;
                else if (sType == Server_Type_String[SERVER_HTTP_PROXY])
                    s.type = SERVER_HTTP_PROXY;
                else if (sType == Server_Type_String[SERVER_HTTPS_PROXY])
                    s.type = SERVER_HTTPS_PROXY;
                else if (sType == Server_Type_String[SERVER_FTP_PROXY])
                    s.type = SERVER_FTP_PROXY;
                else if (sType == Server_Type_String[SERVER_HTTP_FTP])
                    s.type = SERVER_HTTP_FTP;

                s.port = element.attribute("Port", "0").toInt();
                ret << s;
            } else if (element.nodeName() == "Property")
            {
                AllProxyEnabled = element.attribute("AllProxyEnabled", "false").compare("true") == 0;
            }
        }
        n = n.nextSibling();
    }

    serverListNode = ret;
    return ret;
}

QList<TEMPLATES> util::LoadTemplates(URI_TYPE Element)
{
    QList<TEMPLATES> lRet;
    QDomNode node = LoadConfigurationNode("templateList");
    QDomNode n = node.firstChild();
    while(!n.isNull())
    {
        QDomElement item = n.toElement();
        if(!item.isNull())
        {
            TEMPLATES itemMap;
            itemMap.type = URI_TYPE_NONE;
            QString type = item.attribute("type", "");
            if (type.compare("HTTP/FTP", Qt::CaseInsensitive) == 0)
                itemMap.type = URI_TYPE_HTTP_FTP;
            else if (type.compare("Multi HTTP/FTP", Qt::CaseInsensitive) == 0)
                itemMap.type = URI_TYPE_MULTI_HTTP_FTP;
            else if (type.compare("Torrent", Qt::CaseInsensitive) == 0)
                itemMap.type = URI_TYPE_TORRENT;
            else if (type.compare("Magnetlink", Qt::CaseInsensitive) == 0)
                itemMap.type = URI_TYPE_MAGNETLINK;
            else if (type.compare("Metalink", Qt::CaseInsensitive) == 0)
                itemMap.type = URI_TYPE_METALINK;

            if (itemMap.type != URI_TYPE_NONE)
            {
                itemMap.name = QString::fromUtf8(QByteArray::fromPercentEncoding(item.attribute("name", "").toAscii()));
                if ( (Element == URI_TYPE_ALL) || (itemMap.type == Element) )
                {
                    QDomNode NodeChild = n.firstChild();
                    while (!NodeChild.isNull())
                    {
                        QDomElement itemChild = NodeChild.toElement();
                        if (!itemChild.isNull())
                        {
                            QDomNamedNodeMap nodeMap = itemChild.attributes();
                            int itemCount = nodeMap.count();
                            for (int i = 0; i < itemCount; i++)
                            {
                                itemMap.value[nodeMap.item(i).nodeName()] = QString(QByteArray::fromPercentEncoding(nodeMap.item(i).nodeValue().toUtf8()));
                            }
                        }
                        NodeChild = NodeChild.nextSibling();
                    }
                    lRet << itemMap;
                }
            }
        }
        n = n.nextSibling();
    }
    if (Element == URI_TYPE_ALL)
    {
        templateNode = lRet;
    }
    return lRet;
}

void util::LoadConfigurationAll()
{
    connectionNode = LoadConnectionList();
    LoadSetting("", "");
    templateNode = LoadTemplates();
    serverListNode = LoadServerList();
}

void util::SaveConfigurationNode()
{
    QDomDocument doc("Aria2cRemoteConfiguration");
    QDomElement root = doc.createElement("Configuration");
    doc.appendChild(root);

    SaveConnectionList(doc, root);
    SaveSetting(doc, root);
    SaveTemplate(doc,root);
    SaveServerList(doc,root);

    QFile file(util::getHomePath() + "configuration.xml");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toByteArray());
        file.close();
    }
}

void util::SaveConnectionList(const CONNECTION &connection)
{
    connectionNode = connection;
    SaveConfigurationNode();
}

void util::SaveConnectionList(QDomDocument &doc, QDomElement &root)
{
    QDomElement nodeRoot = doc.createElement("ConnectionList");
    root.appendChild(nodeRoot);

    QDomElement elem = doc.createElement("Connection");
    elem.setAttribute("Password", QString( RC4(connectionNode.password.toAscii()).toBase64()) );
    elem.setAttribute("User", QString( connectionNode.user.toAscii().toPercentEncoding() ) );
    elem.setAttribute("Host", QString( connectionNode.host.toAscii().toPercentEncoding() ) );
    elem.setAttribute("Port", QString::number( connectionNode.port ));
    elem.setAttribute("EnableProxy", QVariant( connectionNode.enableProxy).toString() );
    elem.setAttribute("ProxyPassword", QString( RC4( connectionNode.proxyPassword.toAscii()).toBase64()) );
    elem.setAttribute("ProxyUser", QString( connectionNode.proxyUser.toAscii().toPercentEncoding() ) );
    elem.setAttribute("ProxyServer", QString( connectionNode.proxyServer.toAscii().toPercentEncoding() ) );
    elem.setAttribute("ProxyPort", QString::number( connectionNode.proxyPort ));
    nodeRoot.appendChild(elem);
}

void util::SaveSetting(QDomDocument &doc, QDomElement &root)
{
    QDomElement nodeRoot = doc.createElement("UISetting");
    root.appendChild(nodeRoot);

    QMapIterator<QString, QMap<QString, QString> > m(uiSettingNode);
    while (m.hasNext())
    {
        m.next();
        QDomElement elem = doc.createElement(m.key());
        QMapIterator<QString, QString> v(m.value());
        while (v.hasNext())
        {
            v.next();
            elem.setAttribute(v.key(), v.value());
        }
        nodeRoot.appendChild(elem);
    }
}

void util::SaveSetting(const QString &Element, const QString &name, const QString &value)
{
    uiSettingNode[Element][name] = value;
    SaveConfigurationNode();
}

void util::SaveTemplate(QDomDocument &doc, QDomElement &root)
{
    QDomElement nodeRoot = doc.createElement("TemplateList");
    root.appendChild(nodeRoot);

    foreach (util::TEMPLATES t, templateNode)
    {
        QDomElement elem = doc.createElement("item");
        elem.setAttribute("name", QString(t.name.toUtf8().toPercentEncoding()));
        QString itemType;

        if (t.type == URI_TYPE_HTTP_FTP)
            itemType = "HTTP/FTP";
        else if (t.type == URI_TYPE_MULTI_HTTP_FTP)
            itemType = "Multi HTTP/FTP";
        else if (t.type == URI_TYPE_TORRENT)
            itemType = "Torrent";
        else if (t.type == URI_TYPE_MAGNETLINK)
            itemType = "Magnetlink";
        else if (t.type == URI_TYPE_METALINK)
            itemType = "Metalink";

        if (itemType.size() > 0)
        {
            elem.setAttribute("type", itemType);

            QMapIterator<QString, Variant> m(t.value);
            while (m.hasNext())
            {
                m.next();
                QDomElement e = doc.createElement("value");
                e.setAttribute(m.key(), QString(m.value().toString().toUtf8().toPercentEncoding()));
                elem.appendChild(e);
            }
            nodeRoot.appendChild(elem);
        }
    }
}

void util::SaveTemplate(const QList<util::TEMPLATES> &temp)
{
    templateNode = temp;
    SaveConfigurationNode();
}

void util::SaveServerList(QDomDocument &doc, QDomElement &root)
{
    QDomElement nodeRoot = doc.createElement("ServerList");
    root.appendChild(nodeRoot);

    foreach (SERVER_ITEM si, serverListNode)
    {
        QDomElement elem = doc.createElement("Server");

        elem.setAttribute("Password", QString( RC4(si.password.toAscii()).toBase64()) );
        elem.setAttribute("User", QString( si.user.toAscii().toPercentEncoding() ) );
        elem.setAttribute("Name", QString( si.server.toAscii().toPercentEncoding() ) );
        elem.setAttribute("Type", Server_Type_String[si.type]);
        if (si.port != 0)
        {
            elem.setAttribute("Port", QString::number(si.port));
        }

        nodeRoot.appendChild(elem);
    }

    QDomElement elemProperty = doc.createElement("Property");

    elemProperty.setAttribute("AllProxyEnabled", QVariant(AllProxyEnabled).toString());
    nodeRoot.appendChild(elemProperty);
}

void util::SaveServerList(const QList<SERVER_ITEM> &serverlist)
{
    serverListNode = serverlist;
    SaveConfigurationNode();

}
