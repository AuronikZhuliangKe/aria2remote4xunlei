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

#ifndef REQUESTTHREAD_H
#define REQUESTTHREAD_H

#include <QThread>
#include <QMap>
#include "xmlrpc.h"
#include "download.h"
#include "util.h"

typedef QMap<qint64, xmlrpc::XmlRPC> XML_RPC_RESPONSE_MAP;

typedef struct TFAULT_MESSAGE
{
    quint32 code;
    QString string;
}FAULT_MESSAGE,*PFAULT_MESSAGE;

using namespace xmlrpc;
using namespace util;

class RequestThread : public QThread
{
    Q_OBJECT
public:
    RequestThread();
    ~RequestThread();
    void run();
    void stop(){ m_exit = true; }
    void SetSleep(quint32 sleep) { m_sleep = sleep;}
    void EnablePeriodicRequest() { m_periodicRequest = true; wakeUp(); }
    void setCurrentGID(int gid) { m_currentGID = gid; }
    void setConnection(const util::CONNECTION &connection);
    void SetGZipEnabled() { client.setGZipEnabled(); }
    void wakeUp() {condition.wakeAll();}
    void setDescriptionText(QString s) {m_sDescription = s; }
    void addRequest(Download &d);
    void addRequest(QList<Download> &d);

signals:
    void ResponseGetGlobalOptions(QVariant params);
    void ResponseGetLocalOptions(QVariant params);
    void ResponseVersionInfo(QVariant params);
    void ShowTransferDialog(QString sDescription);
    void HideTransferDialog();
    void RequestGID(QList<quint64>);
    void RequestFault(QList<FAULT_MESSAGE>);
    void Response(XML_RPC_RESPONSE_MAP tellActive, XML_RPC_RESPONSE_MAP tellStopped, XML_RPC_RESPONSE_MAP tellWaiting);
    void processFaultToUI( int requestId, int errorCode, QString errorString );

public slots:
    void processReturnValue( int iTypes, qint64 iGID, int requestId, QVariant value );
    void processFault( int requestId, int errorCode, QString errorString );

private:
    volatile bool m_exit;
    quint32 m_sleep;
    static QWaitCondition condition;
    static QMutex conditionMutex;
    xmlrpc::Client client;

    util::CONNECTION m_connection;

    qint64 m_currentGID;

    QQueue<Download> m_request;
    QString m_sDescription;
    bool m_periodicRequest;
};

#endif // REQUESTTHREAD_H
