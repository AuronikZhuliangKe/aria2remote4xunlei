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

#include "GlobalOptions.h"
#include "ui_GlobalOptions.h"
#include "util.h"

extern quint32 g_uiAria2cVersion;

GlobalOptions::GlobalOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GlobalOptions)
{
    ui->setupUi(this);
}

GlobalOptions::~GlobalOptions()
{
    delete ui;
}

void GlobalOptions::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GlobalOptions::SetGlobalOptions(QMap<QString, Variant> options)
{
    m_newGlobalOptions = m_globalOptions = options;

    SetBasicOptions();
    SetFTPOptions();
    SetHTTPOptions();
    SetBittorrentOptions();
    SetMetalinkOptions();
    SetAdvancedOptions();
    SetAdvanced1Options();

    if (g_uiAria2cVersion >= util::ARIA2C_VERSION_1140)
    {
        ui->comboBox_LogLevel->setEnabled(true);
        ui->spinBox_MaxDownloadResult->setEnabled(true);
        ui->comboBox_DownloadResult->setEnabled(true);
    }
}

void GlobalOptions::MaxOverallDownloadLimitChanged(int iValue)
{
    m_newGlobalOptions["max-overall-download-limit"] = Variant(iValue * 1024);
}

void GlobalOptions::MaxOverallUploadLimitChanged(int iValue)
{
    m_newGlobalOptions["max-overall-upload-limit"] = Variant(iValue * 1024);
}

void GlobalOptions::MaxConcurrentDownloadsChanged(int iValue)
{
    m_newGlobalOptions["max-concurrent-downloads"] = Variant(iValue);
}

void GlobalOptions::LogLevelChanged(QString sLogLevel)
{
    m_newGlobalOptions["log-level"] = Variant(sLogLevel);
}

void GlobalOptions::MaxDownloadResult(int iValue)
{
    m_newGlobalOptions["max-download-result"] = Variant(iValue);
}

void GlobalOptions::DownloadResult(int iValue)
{
    Aria2cParameter param = Aria2cParams["download-result"];
    m_newGlobalOptions["download-result"] = Variant(param.getList()[iValue]);
}

void GlobalOptions::SetBasicOptions()
{
    int iValue;

    ui->lineEdit_Dir->setText(m_globalOptions.value("dir", QString("")).toString());
    ui->lineEdit_Help->setText(m_globalOptions.value("help", QString("#basic")).toString());

    ui->lineEdit_LogFile->setText(m_globalOptions.value("log", QString("")).toString());

    Aria2cParameter param = Aria2cParams["log-level"];
    ui->comboBox_LogLevel->setCurrentIndex(param.getList().indexOf(m_globalOptions.value("log-level", QString("debug")).toString()));

    iValue = m_globalOptions.value("max-overall-download-limit", 0).toInt() / 1024;
    ui->spinBox_MaxOverallDownloadLimit->setValue(iValue);

    iValue = m_globalOptions.value("max-overall-upload-limit", 0).toInt() / 1024;
    ui->spinBox_MaxOverallUploadLimit->setValue(iValue);

    ui->spinBox_MaxConcurrentDownloads->setValue(m_globalOptions.value("max-concurrent-downloads", 5).toInt());
    ui->checkBox_ContinueDownload->setCheckState(m_globalOptions.value("continue", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));

    ui->spinBox_Split->setValue(m_globalOptions.value("split", 5).toInt());
    ui->spinBox_LowestSpeedLimit->setValue(m_globalOptions.value("lowest-speed-limit", 0).toInt() / 1024);
    ui->spinBox_MaxDownloadLimit->setValue(m_globalOptions.value("max-download-limit", 0).toInt() / 1024);
    ui->spinBox_MaxFileNotFound->setValue(m_globalOptions.value("max-file-not-found", 0).toInt());
    ui->spinBox_MaxResumeFailureTries->setValue(m_globalOptions.value("max-resume-failure-tries", 0).toInt());
    ui->spinBox_MaxTries->setValue(m_globalOptions.value("max-tries", 5).toInt());
    ui->spinBox_Timeout->setValue(m_globalOptions.value("timeout", 60).toInt());
    ui->spinBox_MaxConnectionPerServer->setValue(m_globalOptions.value("max-connection-per-server", 1).toInt());
    ui->spinBox_MinSplitSize->setValue(m_globalOptions.value("min-split-size", 20971520).toInt() / 1024);
    ui->spinBox_MaxDownloadResult->setValue(m_globalOptions.value("max-download-result", 1000).toInt());
}

void GlobalOptions::SetFTPOptions()
{
    ui->radioButton_Mode_Passive->setChecked(m_globalOptions.value("ftp-pasv", true).toBool());
    ui->radioButton_Mode_Active->setChecked(!m_globalOptions.value("ftp-pasv", true).toBool());
    ui->spinBox_FTP_ConnectTimeout->setValue(m_globalOptions.value("connect-timeout", 60).toInt());
    ui->checkBox_ReuseConnection->setCheckState(m_globalOptions.value("ftp-reuse-connection", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->radioButton_Type_Binary->setChecked(m_globalOptions.value("ftp-type", QString("binary")).toString() == QString("binary"));
    ui->radioButton_Type_ASCII->setChecked(m_globalOptions.value("ftp-type", QString("binary")).toString() == QString("ascii"));
}

void GlobalOptions::SetHTTPOptions()
{
    ui->checkBox_HTTPKeepAlive->setCheckState(m_globalOptions.value("enable-http-keep-alive", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_HTTPPipelining->setCheckState(m_globalOptions.value("enable-http-pipelining", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_AcceptGzip->setCheckState(m_globalOptions.value("http-accept-gzip", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_AuthChallenge->setCheckState(m_globalOptions.value("http-auth-challenge", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_HTTPNoCache->setCheckState(m_globalOptions.value("http-no-cache", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_ConditionalGet->setCheckState(m_globalOptions.value("conditional-get", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->lineEdit_UserAgent->setText(m_globalOptions.value("user-agent", QString("aria2/")).toString());
    ui->spinBox_ConnectTimeout->setValue(m_globalOptions.value("connect-timeout", 60).toInt());
}

void GlobalOptions::SetBittorrentOptions()
{
    ui->checkBox_EnableLpd->setCheckState(m_globalOptions.value("bt-enable-lpd", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_HashCheckSeed->setCheckState(m_globalOptions.value("bt-hash-check-seed", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_MetadataOnly->setCheckState(m_globalOptions.value("bt-metadata-only", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_RequireCrypto->setCheckState(m_globalOptions.value("bt-require-crypto", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_SaveMetadata->setCheckState(m_globalOptions.value("bt-save-metadata", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_SeedUnverified->setCheckState(m_globalOptions.value("bt-seed-unverified", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_FollowTorrent->setCheckState(m_globalOptions.value("follow-torrent", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_PeerExchange->setCheckState(m_globalOptions.value("enable-peer-exchange", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_RemoveUnselectedFile->setCheckState(m_globalOptions.value("bt-remove-unselected-file", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));

    bool ok;
    ui->doubleSpinBox_SeedRatio->setValue(m_globalOptions.value("seed-ratio", 1.0).toDouble(&ok));
    ui->spinBox_StopTimeout->setValue(m_globalOptions.value("bt-stop-timeout", 0).toInt());
    ui->spinBox_MaxUploadLimit->setValue(m_globalOptions.value("max-upload-limit", 0).toInt());
    ui->spinBox_MaxOpenFiles->setValue(m_globalOptions.value("bt-max-open-files", 100).toInt());
    ui->spinBox_MaxPeers->setValue(m_globalOptions.value("bt-max-peers", 55).toInt());
    ui->spinBox_TrackerInterval->setValue(m_globalOptions.value("bt-tracker-interval", 0).toInt());
    ui->spinBox_RequestPeerSpeedLimit->setValue(m_globalOptions.value("bt-request-peer-speed-limit", 51200).toInt() / 1024);
    ui->spinBox_TrackerTimeout->setValue(m_globalOptions.value("bt-tracker-timeout", 60).toInt());
    ui->spinBox_TrackerConnectTimeout->setValue(m_globalOptions.value("bt-tracker-connect-timeout", 60).toInt());

    ui->lineEdit_PeerIDPrefix->setText(m_globalOptions.value("peer-id-prefix", QString("aria2/1.9.1-")).toString());
    ui->lineEdit_listenPort->setText(m_globalOptions.value("listen-port", QString("6881-6999")).toString());

    QString s = m_globalOptions.value("bt-min-crypto-level", QString("arc4")).toString();
    int iPos = -1;
    if (s == "arc4")
        iPos = 0;
    else if (s == "plain")
        iPos = 1;

    ui->comboBox_MinCryptoLevel->setCurrentIndex(iPos);
}

void GlobalOptions::SetMetalinkOptions()
{
    ui->radioButton_PreferredProtocol_HTTP->setChecked(m_globalOptions.value("metalink-preferred-protocol", QString("none")).toString() == QString("http"));
    ui->radioButton_PreferredProtocol_HTTPS->setChecked(m_globalOptions.value("metalink-preferred-protocol", QString("none")).toString() == QString("https"));
    ui->radioButton_PreferredProtocol_FTP->setChecked(m_globalOptions.value("metalink-preferred-protocol", QString("none")).toString() == QString("ftp"));
    ui->radioButton_PreferredProtocol_None->setChecked(m_globalOptions.value("metalink-preferred-protocol", QString("none")).toString() == QString("none"));

    ui->checkBox_FollowMetalink->setCheckState(m_globalOptions.value("follow-metalink", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_UniqueProtocol->setCheckState(m_globalOptions.value("metalink-enable-unique-protocol", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->spinBox_MetalinkServers->setValue(m_globalOptions.value("metalink-servers", 5).toInt());
}

void GlobalOptions::SetAdvancedOptions()
{
    ui->checkBox_AllowOverwrite->setCheckState(m_globalOptions.value("allow-overwrite", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_AllowPieceLengthChange->setCheckState(m_globalOptions.value("allow-piece-length-change", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_AlwayResume->setCheckState(m_globalOptions.value("always-resume", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_AsyncDns->setCheckState(m_globalOptions.value("async-dns", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_AsyncDns6->setCheckState(m_globalOptions.value("enable-async-dns6", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_AutoFileRenaming->setCheckState(m_globalOptions.value("auto-file-renaming", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_DisableIPV6->setCheckState(m_globalOptions.value("disable-ipv6", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_DryRun->setCheckState(m_globalOptions.value("dry-run", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_CheckCertificate->setCheckState(m_globalOptions.value("check-certificate", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_CheckIntegrity->setCheckState(m_globalOptions.value("check-integrity", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_Daemon->setCheckState(m_globalOptions.value("daemon", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_HumanReadable->setCheckState(m_globalOptions.value("human-readable", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_HashCheckOnly->setCheckState(m_globalOptions.value("hash-check-only", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));

    ui->spinBox_AutoSaveInterval->setValue(m_globalOptions.value("auto-save-interval", 60).toInt());

    ui->radioButton_EventPoll_Epoll->setChecked(m_globalOptions.value("event-poll", QString("select")).toString() == QString("epoll"));
    ui->radioButton_EventPoll_Select->setChecked(m_globalOptions.value("event-poll", QString("select")).toString() == QString("select"));

    int iPos = -1;

    QString s = m_globalOptions.value("file-allocation", QString("none")).toString();
    if (s == "none")
        iPos = 0;
    else if (s == "prealloc")
        iPos = 1;
    else if (s == "falloc")
        iPos = 2;

    ui->comboBox_FileAllocation->setCurrentIndex(iPos);

    ui->lineEdit_DHT_FilePath->setText(m_globalOptions.value("dht-file-path", QString("$HOME/.aria2/dht.dat")).toString());
    ui->lineEdit_DHT_FilePath6->setText(m_globalOptions.value("dht-file-path6", QString("$HOME/.aria2/dht6.dat")).toString());
    ui->lineEdit_DHT_ListenPort->setText(m_globalOptions.value("dht-listen-port", QString("6881-6999")).toString());
    ui->lineEdit_ConfPath->setText(m_globalOptions.value("conf-path", QString("$HOME/.aria2/aria2.conf")).toString());
    ui->checkBox_EnableDHT->setCheckState(m_globalOptions.value("enable-dht", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_EnableDHT6->setCheckState(m_globalOptions.value("enable-dht6", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->spinBox_DHT_MessageTimeout->setValue(m_globalOptions.value("dht-message-timeout", 10).toInt());
    ui->spinBox_RetryWait->setValue(m_globalOptions.value("retry-wait", 0).toInt());

    s = m_globalOptions.value("stream-piece-selector", QString("default")).toString();
    if (s == "default")
        iPos = 0;
    else if (s == "inorder")
        iPos = 1;

    ui->comboBox_StreamPieceSelector->setCurrentIndex(iPos);

    s = m_globalOptions.value("download-result", QString("default")).toString();
    if (s == "default")
        iPos = 0;
    else if (s == "full")
        iPos = 1;

    ui->comboBox_DownloadResult->setCurrentIndex(iPos);
}

void GlobalOptions::SetAdvanced1Options()
{
    ui->checkBox_NoConf->setCheckState(m_globalOptions.value("no-conf", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_UseHead->setCheckState(m_globalOptions.value("use-head", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_NoNetrc->setCheckState(m_globalOptions.value("no-netrc", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_ParameterizedURI->setCheckState(m_globalOptions.value("parameterized-uri", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_Quiet->setCheckState(m_globalOptions.value("quiet", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_RealtimeChunkChecksum->setCheckState(m_globalOptions.value("realtime-chunk-checksum", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_RemoteTime->setCheckState(m_globalOptions.value("remote-time", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_RemoveControlFile->setCheckState(m_globalOptions.value("remove-control-file", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_ReuseURI->setCheckState(m_globalOptions.value("reuse-uri", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_ShowFiles->setCheckState(m_globalOptions.value("show-files", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_ShowConsoleReadout->setCheckState(m_globalOptions.value("show-console-readout", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_TruncateConsoleReadout->setCheckState(m_globalOptions.value("truncate-console-readout", true).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_RpcAllowOriginAll->setCheckState(m_globalOptions.value("rpc-allow-origin-all", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_DeferredInput->setCheckState(m_globalOptions.value("deferred-input", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->spinBox_SummaryInterval->setValue(m_globalOptions.value("summary-interval", 60).toInt());
    ui->spinBox_ServerStatTimeout->setValue(m_globalOptions.value("server-stat-timeout", 86400).toInt());
    ui->spinBox_NoFileAllocationLimit->setValue(m_globalOptions.value("no-file-allocation-limit", 5242880).toInt() / 1024);
    ui->spinBox_PieceLength->setValue(m_globalOptions.value("piece-length", 1048576).toInt() / 1024);
    ui->spinBox_Stop->setValue(m_globalOptions.value("stop", 0).toInt());

    int iPos = -1;

    QString s = m_globalOptions.value("uri-selector", QString("feedback")).toString();
    if (s == "inorder")
        iPos = 0;
    else if (s == "feedback")
        iPos = 1;
    else if (s == "adaptive")
        iPos = 2;

    ui->comboBox_URISelector->setCurrentIndex(iPos);

    iPos = -1;
    s = m_globalOptions.value("proxy-method", QString("get")).toString();
    if (s == "get")
        iPos = 0;
    else if (s == "tunnel")
        iPos = 1;

    ui->comboBox_ProxyMethod->setCurrentIndex(iPos);

    ui->checkBox_XMLRPC_Enable->setCheckState(m_globalOptions.value((g_uiAria2cVersion >= util::ARIA2C_VERSION_1110) ? "enable-rpc" : "enable-xml-rpc", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->checkBox_XMLRPC_ListenAll->setCheckState(m_globalOptions.value((g_uiAria2cVersion >= util::ARIA2C_VERSION_1110) ? "xml-rpc-listen-all" : "rpc-listen-all", false).toBool() ? (Qt::Checked) : (Qt::Unchecked));
    ui->spinBox_XMLRPC_ListenPort->setValue(m_globalOptions.value((g_uiAria2cVersion >= util::ARIA2C_VERSION_1110) ? "xml-rpc-listen-port" : "rpc-listen-port", 6800).toInt());
    ui->spinBox_XMLRPC_MaxRequestSize->setValue(m_globalOptions.value((g_uiAria2cVersion >= util::ARIA2C_VERSION_1110) ? "xml-rpc-max-request-size" : "rpc-max-request-size", 2097152).toInt() / 1024);
}
