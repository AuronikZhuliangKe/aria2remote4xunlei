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

#include "requestthread.h"
#include <QtGui/QApplication>
#include <QMessageBox>
#include <QByteArray>
#include "util.h"

extern quint32 g_uiAria2cVersion;

QWaitCondition RequestThread::condition;
QMutex RequestThread::conditionMutex;

RequestThread::RequestThread() :
        m_exit(false),
        m_sleep(10000),
        m_currentGID(-1),
        m_periodicRequest(false)
{
    m_connection.host = "";
    m_connection.user = "";
    m_connection.password = "";
    m_connection.port = 6800;
    m_connection.proxyServer = "";
    m_connection.proxyUser = "";
    m_connection.proxyPassword = "";
    m_connection.proxyPort = 8080;

    client.setHost(m_connection.host, m_connection.port, "/rpc");
    client.setUser(m_connection.user, m_connection.password);

    connect( &client, SIGNAL(done( int, qint64, int, QVariant )), this, SLOT(processReturnValue( int, qint64, int, QVariant )) );
    connect( &client, SIGNAL(failed( int, int, QString )), this, SLOT(processFault( int, int, QString )) );
}

RequestThread::~RequestThread()
{
    stop();
    wakeUp();
    wait();
}

void RequestThread::setConnection(const util::CONNECTION &connection)
{
    m_connection = connection;

    client.setHost(m_connection.host, m_connection.port, "/rpc");
    client.setUser(m_connection.user, m_connection.password);
    if (m_connection.enableProxy)
    {
       client.setProxy(m_connection.proxyServer, m_connection.proxyPort, m_connection.proxyUser, m_connection.proxyPassword);
    }
}

void RequestThread::addRequest(Download &d)
{
    conditionMutex.lock();
        m_request.enqueue(d);
    conditionMutex.unlock();
}

void RequestThread::addRequest(QList<Download> &d)
{
    conditionMutex.lock();
    foreach(Download down, d)
    {
        m_request.enqueue(down);
    }
    conditionMutex.unlock();
}

void RequestThread::run()
{
    while(!m_exit)
    {
        conditionMutex.lock();
        condition.wait(&conditionMutex, m_sleep);
        //set request
        QVariantList v;
        int iRequestTypes = UNKNOWN;

        if (m_request.size() > 0)
        {
            while (!m_request.isEmpty())
            {
                Download d = m_request.dequeue();
                d.toXML();
                iRequestTypes |= d.getType();
                v += d.getParamList();
            }
            if ( (v.size() != 0) && (m_sDescription.size() != 0))
            {
                emit ShowTransferDialog(m_sDescription);
            }
        }
        else if (m_periodicRequest)
        {
            QMap<QString, Variant> methodTellActive;
            QVariantList paramsTellActive;
            QMap<QString, Variant> methodTellStopped;
            QMap<QString, Variant> methodTellWaiting;
            QVariantList paramsStruct;

            paramsStruct << Variant(0);

            paramsStruct << Variant(INT_MAX);

            methodTellActive["methodName"] = QString("aria2.tellActive");
            methodTellActive["params"] = paramsTellActive;

            methodTellStopped["methodName"] = QString("aria2.tellStopped");
            methodTellStopped["params"] = paramsStruct;

            methodTellWaiting["methodName"] = QString("aria2.tellWaiting");
            methodTellWaiting["params"] = paramsStruct;

            v += Variant(methodTellActive);
            v += Variant(methodTellStopped);
            v += Variant(methodTellWaiting);
            if (m_currentGID != -1)
            {
                QMap<QString, Variant> methodGetPeers;
                QVariantList paramsGetPeers;
                QMap<QString, Variant> methodGetServers;
                QVariantList paramsGetServers;

                methodGetPeers["methodName"] = QString("aria2.getPeers");
                paramsGetPeers << Variant(QString::number(m_currentGID));
                methodGetPeers["params"] = paramsGetPeers;

                methodGetServers["methodName"] = QString("aria2.getServers");
                paramsGetServers << Variant(QString::number(m_currentGID));
                methodGetServers["params"] = paramsGetServers;

                v += Variant(methodGetPeers);
                v += Variant(methodGetServers);
            }
            iRequestTypes = PERIODIC_REQUEST;
        }

        if (iRequestTypes != UNKNOWN)
        {
            //response from Aria2c
            QList<Variant> multiCall;
            multiCall << Variant(v);
            client.request(multiCall, "system.multicall", iRequestTypes, m_currentGID);
        }
        conditionMutex.unlock();
    }
}

void RequestThread::processReturnValue( int iTypes, qint64 iGID, int requestId, QVariant value )
{
    QVariantList vl;
    QString errorMessage;

    vl << value;
    QByteArray array(xmlrpc::Response(vl).composeResponse());
    xmlrpc::Response re;

#ifdef QT_DEBUG
    QFile ResposeWriteToFile(util::getHomePath() + "aria_response.xml");
    if (ResposeWriteToFile.open(QIODevice::ReadWrite))
    {
        ResposeWriteToFile.seek(ResposeWriteToFile.size());
        ResposeWriteToFile.write(array);
        ResposeWriteToFile.close();
    }
#endif
    emit HideTransferDialog();

    if (iTypes & GET_GLOBAL_OPTIONS)
    {
        emit ResponseGetGlobalOptions(value);
    }
    else if (iTypes & GET_LOCAL_OPTIONS)
    {
        emit ResponseGetLocalOptions(value);
    }
    else if (iTypes & VERSION_INFO)
    {
        emit ResponseVersionInfo(value);
    } else if (iTypes & PERIODIC_REQUEST)
    {
        if (re.setContent(array, &errorMessage))
        {
            XML_RPC_RESPONSE_MAP responseTellActive;
            XML_RPC_RESPONSE_MAP responseTellStopped;
            XML_RPC_RESPONSE_MAP responseTellWaiting;
            QVariantList values = value.toList();

            QVariantList tmpList = values[0].toList();
            if (tmpList.size() > 0)
            {
                QVariantList itemtellActiveList(tmpList[0].toList());
                int itemtellActiveListSize = itemtellActiveList.size();
                for (int i = 0; i < itemtellActiveListSize; i++)
                {
                     QVariant &item(itemtellActiveList[i]);
                     if (Variant::Map == item.type())
                     {
                         xmlrpc::XmlRPC x;
                         x.ParseResponseXML(item);
                         qint64 gid = x.getGID();
                         responseTellActive[gid] = x;
                     }
                }
            }

            tmpList = values[1].toList();
            if (tmpList.size() > 0)
            {
                QVariantList itemtellStoppedList(tmpList[0].toList());
                int itemtellStoppedListSize = itemtellStoppedList.size();
                for (int i = 0; i < itemtellStoppedListSize; i++)
                {
                     QVariant &item(itemtellStoppedList[i]);
                     if (Variant::Map == item.type())
                     {
                         xmlrpc::XmlRPC x;
                         x.ParseResponseXML(item);
                         qint64 gid = x.getGID();
                         responseTellStopped[gid] = x;
                     }
                }
            }

            tmpList = values[2].toList();
            if (tmpList.size() > 0)
            {
                QVariantList itemtellWaitingList(tmpList[0].toList());
                int itemtellWaitingListSize = itemtellWaitingList.size();
                for (int i = 0; i < itemtellWaitingListSize; i++)
                {
                     QVariant &item(itemtellWaitingList[i]);
                     if (Variant::Map == item.type())
                     {
                         xmlrpc::XmlRPC x;
                         x.ParseResponseXML(item);
                         qint64 gid = x.getGID();
                         responseTellWaiting[gid] = x;
                     }
                }
            }

            if (values.size() == 5)
            {
                if (Variant::Map == values[3].type())
                {
                    //there is no peer list
                }
                else
                {
                    QVariantList itemtellPeersList(values[3].toList()[0].toList());
                    int itemtellPeersListSize = itemtellPeersList.size();

                    for (int i = 0; i < itemtellPeersListSize; i++)
                    {
                         QVariant &item(itemtellPeersList[i]);
                         if (Variant::Map == item.type())
                         {
                             if (responseTellActive.contains(iGID))
                             {
                                 responseTellActive[iGID].setPeers(item);
                             } else if (responseTellStopped.contains(iGID))
                             {
                                 responseTellStopped[iGID].setPeers(item);
                             } else if (responseTellWaiting.contains(iGID))
                             {
                                 responseTellWaiting[iGID].setPeers(item);
                             }
                         }
                    }
                }

                if (Variant::Map == values[4].type())
                {
                    //there is no server list
                }
                else
                {
                    QVariantList itemtellServersList(values[4].toList()[0].toList());
                    int itemtellServersListSize = itemtellServersList.size();
                    for (int i = 0; i < itemtellServersListSize; i++)
                    {
                         QVariant &item(itemtellServersList[i]);
                         if (Variant::Map == item.type())
                         {
                             if (responseTellActive.contains(iGID))
                             {
                                 responseTellActive[iGID].setServers(item);
                             } else if (responseTellStopped.contains(iGID))
                             {
                                 responseTellStopped[iGID].setServers(item);
                             } else if (responseTellWaiting.contains(iGID))
                             {
                                 responseTellWaiting[iGID].setServers(item);
                             }
                         }
                    }
                }
            }
            emit Response(responseTellActive, responseTellStopped, responseTellWaiting);
        }
    } else
    {
        QList<quint64> gids;
        QList<FAULT_MESSAGE> faultMessages;
        foreach(QVariant elem, value.toList())
        {
            if (elem.type() == Variant::Map)
            {
                QMapIterator<QString,QVariant> fault(elem.toMap());
                FAULT_MESSAGE faultMessage;
                int ParamCount = 0;
                while( fault.hasNext() )
                {
                    fault.next();
                    QString key = fault.key();
                    if (key.compare("faultCode", Qt::CaseInsensitive) == 0)
                    {
                        bool ok;
                        faultMessage.code = fault.value().toInt(&ok);
                        if (ok)
                            ParamCount++;
                    } else if (key.compare("faultString", Qt::CaseInsensitive) == 0)
                    {
                        faultMessage.string = fault.value().toString();
                        ParamCount++;
                    }
                }
                if (ParamCount == 2)
                    faultMessages.push_back(faultMessage);
            } else if (elem.type() == Variant::List)
            {
                foreach(QVariant subElem, elem.toList())
                {
                    if (subElem.toString().compare("OK") == 0)
                    {
                    }
                    else
                    {
                        bool ok;
                        quint64 gid = subElem.toULongLong(&ok);
                        if (ok)
                            gids.push_back(gid);
                    }
                }
            }
        }
        if (gids.size() > 0)
            emit RequestGID(gids);

        if (faultMessages.size() > 0)
            emit RequestFault(faultMessages);
    }
}

void RequestThread::processFault( int requestId, int errorCode, QString errorString )
{
    emit HideTransferDialog();
    if (!m_exit)
    {
        emit processFaultToUI(requestId, errorCode, errorString );
    }
}
