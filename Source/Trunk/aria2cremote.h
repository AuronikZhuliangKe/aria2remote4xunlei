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

#ifndef ARIA2CREMOTE_H
#define ARIA2CREMOTE_H

#include <QMainWindow>
#include <QTabWidget>
#include <QIcon>
#include <QSplitter>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QTextEdit>
#include <QSettings>
#include <QMessageBox>
#include <QMenu>
#include <QFileDialog>
#include <QClipboard>
#include <QMutex>
#include <QTreeWidgetItem>
#include <QStatusBar>
#include "tabwidget.h"
#include "listview.h"
#include "mainlistview.h"
#include "adduri.h"
#include "add_http_ftp_magnetlink.h"
#include "globaloptions.h"
#include "selecttorrentfiles.h"
#include "download.h"
#include "requestthread.h"
#include "transfer.h"
#include "QTreeWidgetItemEx.h"
#include "statusbarex.h"
#include "about.h"
#include "geoip.h"
#include "downloadprogressdialog.h"
#ifdef Q_WS_WIN
#include "windows/windows7.h"
#endif

namespace Ui {
    class Aria2cRemote;
}
extern quint32 g_uiAria2cVersion;

typedef struct downloadList
{
    QTreeWidgetItemEx* item;
    xmlrpc::XmlRPC list;
}DOWNLOAD_LIST, *PDOWNLOAD_LIST;

class Aria2cRemote : public QMainWindow {
    Q_OBJECT
public:
    Aria2cRemote(QWidget *parent = 0);
    ~Aria2cRemote();
    void createLanguageMenu(void);

protected:
    void changeEvent(QEvent *e);
    void switchTranslator(QTranslator& translator, const QString& filename);
    void loadLanguage(const QString& rLanguage);

private slots:
    void on_actionGlobal_Options_triggered();
    void on_actionForce_power_off_Aria2c_triggered();
    void on_actionForce_remove_triggered();
    void on_actionForce_pause_all_triggered();
    void on_actionForce_pause_triggered();
    void on_actionPower_off_Aria2c_triggered();
    void on_actionPurge_triggered();
    void on_action_Remove_triggered();
    void on_actionPause_all_triggered();
    void on_action_Pause_triggered();
    void on_actionStart_all_triggered();
    void on_action_Start_triggered();
    void on_actionOption_triggered();
    void ResponseXML(XML_RPC_RESPONSE_MAP tellActive, XML_RPC_RESPONSE_MAP tellStopped, XML_RPC_RESPONSE_MAP tellWaiting);
    void processFaultToUI( int requestId, int errorCode, QString errorString );
    void ResponseVersionInfo(QVariant params);
    void ShowTransferDialog(QString sDescription);
    void HideTransferDialog();
    void GetGlobalOptions(QVariant value);
    void slotLanguageChanged(QAction* action);
    void RequestGID(QList<quint64> gids);
    void RequestFault(QList<FAULT_MESSAGE> faultMessages);
    void GeoIPDBdownloadFinished(QList<QNetworkReply*> listReply);

private:
    Ui::Aria2cRemote *ui;
    DetailsTabView *m_DetailsTab;
    ListView *m_listView;
    MainListView *m_mainListView;
    QSplitter *m_mainSplitter;
    QSplitter *m_mainSplitter1;
    QSystemTrayIcon *m_SystemTrayIcon;

    RequestThread m_requestThread;

    Transfer *m_transfer;

    //global options
    QMap<QString, Variant> m_globalOptions;
    bool bValidGlobalOption;

    //connected to aria2c
    bool m_bConnected;

    //current item
    qint64 m_currentGID;

    //Connection options + proxy
    util::CONNECTION m_connection;

    XML_RPC_RESPONSE_MAP m_tellActive;
    XML_RPC_RESPONSE_MAP m_tellStopped;
    XML_RPC_RESPONSE_MAP m_tellWaiting;

    QMap<qint64, DOWNLOAD_LIST> m_downloadView;

    //Downloads status
    quint64 m_seeding;
    quint64 m_downloading;
    quint64 m_complete;
    quint64 m_pause;
    quint64 m_error;
    quint64 m_waiting;
    quint64 m_remove;

    //status bar
    StatusBarEx *statusBarEx;

    //status bar connect state + Aria2c version
    QLabel m_connectStateIcon;
    QLabel m_connectStateText;


    //language
    QTranslator     m_translator;   // contains the translations for this application
    QTranslator     m_translatorQt; // contains the translations for qt
    QString         m_currLang;     // contains the currently loaded language
    QString         m_langPath;     // Path of language files. This is always fixed to /languages.

    #ifdef Q_WS_WIN
    Windows7 m_Windows7;
    unsigned int m_IDTaskbarButtonCreated;
    #endif

    QMap<quint64, quint32> m_currentDownloadProgress;

    GeoIP geoip;
    DownloadProgressDialog *progressDialog;
private:
    void setTreeWidgetItem(QTreeWidgetItemEx *item, XmlRPC& x);
    void setInitTreeWidgetItem(QTreeWidgetItemEx *item);
    qint64 getCurrentGIDFromListView();
    #ifdef Q_WS_WIN
    bool winEvent(MSG * message, long * result);
    #endif
    void setToolBarIcon(bool bState);

private slots:
    void on_actionAdd_MagnetLink_triggered();
    void on_actionAdd_Metalink_triggered();
    void on_actionAdd_Multi_HTTP_FTP_triggered();
    void on_actionAdd_HTTP_FTP_triggered();
    void on_actionAdd_Torrent_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void on_action_AddURI();
    void ListViewItemClicked(QTreeWidgetItem*, int);
    void mainListViewItemClicked(QListWidgetItem*);
    void on_actionGeoIP_update_triggered();
};

#endif // ARIA2CREMOTE_H
