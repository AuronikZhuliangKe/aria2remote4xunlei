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

#include "adduri.h"
#include "ui_adduri.h"
#include <QMouseEvent>
#include <QMenu>
#include <QFileIconProvider>
#include <QFileDialog>
#include <QClipboard>
#include <QHeaderView>
#include "util.h"
#include "download.h"
#include "globaloptions.h"
#include "selecttorrentfiles.h"
#include "add_http_ftp_magnetlink.h"
#include "localoptions.h"
#include "server.h"
#include "comboboxitem.h"
#include "templates.h"

AddURI::AddURI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddURI),
    m_iCurrentType(URI_TYPE_HTTP_FTP),
    bValidGlobalOption(false)
{
    ui->setupUi(this);
    ui->pushButton_Delete->setEnabled(false);
    ui->pushButton_Modify->setEnabled(false);

    QFileIconProvider fileIconProvider;
    setWindowIcon(fileIconProvider.icon(QFileIconProvider::Folder));

    QByteArray AddUriGeometry = QByteArray::fromBase64(util::LoadSetting("AddUri", "Geometry").toAscii());
    if (AddUriGeometry.size() > 0)
    {
        restoreGeometry(AddUriGeometry);
    }

    bool ok;
    int iType = util::LoadSetting("AddUri", "Type").toInt(&ok);

    //0 - 4 -> HTTP/FTP, Multi HTTP/FTP, Torrent, MagnetLink, MetaLink
    ui->comboBox_Type->setCurrentIndex(ok ? qBound(0, 4, iType) : 0);

    QByteArray AddUriHeaderState = QByteArray::fromBase64(util::LoadSetting("AddUri", "HeaderState").toAscii());
    if (AddUriHeaderState.size() > 0)
    {
        ui->treeWidget->header()->restoreState(AddUriHeaderState);
    }

    //load download templates
    templates = util::LoadTemplates();
   // connect( ui->treeWidget, SIGNAL( customContextMenuRequested( QPoint ) ), this, SLOT( on_treeWidget_customContextMenuRequested( QPoint ) ) );
}

AddURI::~AddURI()
{
   // disconnect(m_rT, SIGNAL( ResponseGetGlobalOptions(QVariant) ), this, SLOT( GetGlobalOptions(QVariant) ));
    util::SaveSetting("AddUri", "Geometry", QString(saveGeometry().toBase64()));
    util::SaveSetting("AddUri", "HeaderState", QString(ui->treeWidget->header()->saveState().toBase64()));
    util::SaveSetting("AddUri", "Type", QString::number(ui->comboBox_Type->currentIndex()));
    delete ui;
}

void AddURI::changeEvent(QEvent *e)
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

 void AddURI::setRequestThread(RequestThread *rt)
{
     m_rT = rt;
    // connect(m_rT, SIGNAL( ResponseGetGlobalOptions(QVariant) ), this, SLOT( GetGlobalOptions(QVariant) ));
}

 void AddURI::Add()
{
    QString sUrl;
    QMap<QString, Variant> vCurrentParam;
    Server s;

    s.LoadServerList();

    switch (m_iCurrentType)
    {
    case URI_TYPE_HTTP_FTP:
        {
            const QClipboard *clipboard = QApplication::clipboard();
            QString sStr = clipboard->text(QClipboard::Clipboard);
            add_http_ftp_magnetlink add(this);

            add.SetURI(sStr);
            add.SetMenu(util::URI_TYPE_HTTP_FTP, templates,false);
            if (add.exec() == QDialog::Accepted)
            {
                sUrl = add.GetURI();
                Download d;
                SERVER_ITEM si;

                util::deletePrePostSpace(sUrl);
                if (sUrl.size() > 0)
                {
                    if (s.MatchedServer(sUrl, si))
                    {
                        QUrl url(si.server);
                        QString scheme = url.scheme();
                        if ((scheme == "http") || (scheme == "") )
                        {
                            vCurrentParam["http-user"] = si.user;
                            vCurrentParam["http-passwd"] = si.password;
                        }
                        else if (scheme == "ftp")
                        {
                            vCurrentParam["ftp-user"] = si.user;
                            vCurrentParam["ftp-passwd"] = si.password;
                        }
                    }
                    vCurrentParam.unite(util::TemplateFromName(templates, add.getTemplateName(), util::URI_TYPE_HTTP_FTP));
                    d.addHttpFtp(sUrl, vCurrentParam);

                    //add TreeWidgetitem
                    QTreeWidgetItem *item = new QTreeWidgetItem;

                    d.SetTreeWidgetItemIndex(item);
                    item->setIcon(0, QIcon(":/icon/uri/ftp.png"));
                    item->setText(0, "HTTP/FTP");
                    item->setText(1, sUrl);
                    ui->treeWidget->addTopLevelItem(item);

                    ComboBoxItem *combobox_template = new ComboBoxItem(item, 2);
                    combobox_template->addItem(tr("Default"));
                    foreach (util::TEMPLATES t, templates)
                    {
                        if (t.type == URI_TYPE_HTTP_FTP)
                        {
                            combobox_template->addItem(t.name);
                        }
                    }
                    ui->treeWidget->setItemWidget(item, 2, combobox_template);

                    m_dlist << d;
                }
            }
        }
        break;
    case URI_TYPE_MULTI_HTTP_FTP:
        {
            QFileDialog::Options options;
            QString selectedFilter;
            QList<QString> sUrls;
            QTreeWidgetItem *topItem;
            QIcon childIcon(":/icon/uri/ftp.png");

            QStringList sFileNameList = QFileDialog::getOpenFileNames(this,
                                                                      tr("Open URI link list file"),
                                                                      util::LoadSetting(QString("LastOpenedHTTPFTPDirectory"),QString("Dir")),
                                                                      tr("URI link list Files (*.txt);;All files (*.*)"),
                                                                      &selectedFilter,
                                                                      options);


            if (sFileNameList.size() != 0)
            {
                util::SaveSetting(QString("LastOpenedHTTPFTPDirectory"),QString("Dir"), QFileInfo(sFileNameList.front()).absolutePath());
            }
            foreach(QString sFileName, sFileNameList)
            {
                if (sFileName.size() != 0)
                {
                    topItem = new QTreeWidgetItem;

                    topItem->setIcon(0, QIcon(":/icon/uri/http.png"));
                    topItem->setText(0, "Multi HTTP/FTP");
                    topItem->setText(1, sFileName);
                    ui->treeWidget->addTopLevelItem(topItem);

                    ComboBoxItem *combobox_template = new ComboBoxItem(topItem, 2);
                    combobox_template->addItem(tr("Default"));
                    foreach (util::TEMPLATES t, templates)
                    {
                        if (t.type == URI_TYPE_MULTI_HTTP_FTP)
                        {
                            combobox_template->addItem(t.name);
                        }
                    }

                    ui->treeWidget->setItemWidget(topItem, 2, combobox_template);

                    QFile f(sFileName);

                    if (f.open(QFile::ReadOnly))
                    {
                        QFileInfo fInfo(sFileName);
                        bool bRegExDecode = (fInfo.suffix() != "txt");

                        if (bRegExDecode)
                        {
                            QRegExp regex("(www\\.(?!\\.)|([a-zA-z]+)://)[\\d\\w\\./,:_~\\?=&;#@\\-\\+\\%\\$]+[\\d\\w/]");

                            QString str(f.readAll());
                            f.close();

                            int pos = 0;
                            while ((pos = regex.indexIn(str, pos)) != -1)
                            {
                                QString str1(regex.cap());
                                bool bFound = false;
                                foreach (QString s, sUrls)
                                {
                                    if (s == str1)
                                    {
                                        bFound = true;
                                        break;
                                    }
                                }

                                if (!bFound)
                                {
                                    Download d;

                                    SERVER_ITEM si;

                                    if (s.MatchedServer(str1, si))
                                    {
                                        QUrl url(si.server);
                                        QString scheme = url.scheme();
                                        if ((scheme == "http") || (scheme == "") )
                                        {
                                            vCurrentParam["http-user"] = si.user;
                                            vCurrentParam["http-passwd"] = si.password;
                                        }
                                        else if (scheme == "ftp")
                                        {
                                            vCurrentParam["ftp-user"] = si.user;
                                            vCurrentParam["ftp-passwd"] = si.password;
                                        }
                                    }
                                    d.addHttpFtp(str1, vCurrentParam);

                                    sUrls << str1;
                                    //add TreeWidgetitem
                                    QTreeWidgetItem *item = new QTreeWidgetItem;

                                    d.SetTreeWidgetItemIndex(item);
                                    item->setIcon(0, childIcon);
                                    item->setText(0, "HTTP/FTP");
                                    item->setText(1, str1);
                                    topItem->addChild(item);

                                    ComboBoxItem *combobox_template = new ComboBoxItem(item, 2);
                                    combobox_template->addItem(tr("Default"));
                                    foreach (util::TEMPLATES t, templates)
                                    {
                                        if (t.type == URI_TYPE_HTTP_FTP)
                                        {
                                            combobox_template->addItem(t.name);
                                        }
                                    }
                                    combobox_template->setCurrentIndex(0);
                                    ui->treeWidget->setItemWidget(item, 2, combobox_template);

                                    m_dlist << d;
                                }

                                pos += regex.matchedLength();
                            }
                        }
                        else
                        {
                            QTextStream stream( &f );
                            QString line;
                            while ( !stream.atEnd() )
                            {
                                line = stream.readLine();
                                bool bFound = false;
                                foreach (QString s, sUrls)
                                {
                                    if (s == line)
                                    {
                                        bFound = true;
                                        break;
                                    }
                                }
                                if (!bFound)
                                {
                                    Download d;

                                    SERVER_ITEM si;

                                    util::deletePrePostSpace(line);
                                    if (line.size() > 0)
                                    {
                                        if (s.MatchedServer(line, si))
                                        {
                                            QUrl url(si.server);
                                            QString scheme = url.scheme();
                                            if ((scheme == "http") || (scheme == "") )
                                            {
                                                vCurrentParam["http-user"] = si.user;
                                                vCurrentParam["http-passwd"] = si.password;
                                            }
                                            else if (scheme == "ftp")
                                            {
                                                vCurrentParam["ftp-user"] = si.user;
                                                vCurrentParam["ftp-passwd"] = si.password;
                                            }
                                        }
                                        d.addHttpFtp(line, vCurrentParam);

                                        sUrls << line;
                                        //add TreeWidgetitem
                                        QTreeWidgetItem *item = new QTreeWidgetItem;

                                        d.SetTreeWidgetItemIndex(item);
                                        item->setIcon(0, childIcon);
                                        item->setText(0, "HTTP/FTP");
                                        item->setText(1, line);
                                        topItem->addChild(item);

                                        ComboBoxItem *combobox_template = new ComboBoxItem(item, 2);
                                        combobox_template->addItem(tr("Default"));
                                        foreach (util::TEMPLATES t, templates)
                                        {
                                            if (t.type == URI_TYPE_HTTP_FTP)
                                            {
                                                combobox_template->addItem(t.name);
                                            }
                                        }
                                        combobox_template->setCurrentIndex(0);
                                        ui->treeWidget->setItemWidget(item, 2, combobox_template);

                                        m_dlist << d;
                                    }
                                }
                            }
                        }
                        f.close();
                    }
                    else
                    {
                        QMessageBox::warning(this, tr("Multiple file open"), tr("Unable open to %1").arg(sFileName));
                    }
                }
            }
        }
        break;
    case URI_TYPE_TORRENT:
        {
            QFileDialog::Options options;
            QString selectedFilter;

            QStringList sFileNameList = QFileDialog::getOpenFileNames(this,
                                                                      tr("Open torrent file"),
                                                                      util::LoadSetting(QString("LastOpenedTorrentDirectory"),QString("Dir")),
                                                                      tr("Torrent Files (*.torrent)"),
                                                                      &selectedFilter,
                                                                      options);

            if (sFileNameList.size() > 0)
            {
                //save the selected directory
                util::SaveSetting(QString("LastOpenedTorrentDirectory"),QString("Dir"), QFileInfo(sFileNameList.front()).absolutePath());
            }
            foreach(QString sFileName, sFileNameList)
            {
                if (sFileName.size() != 0)
                {
                    //select files in torrent file
                    SelectTorrentFiles torrent(this);
                    MetaInfo metainfo;

                    if (util::ReadTorrentFile(sFileName, metainfo))
                    {
                        torrent.SetData(metainfo);
                        if (torrent.exec() == QDialog::Accepted)
                        {
                            Download d;

                            vCurrentParam["select-file"] = torrent.getSelectedFileList();
                            d.addTorrent(sFileName, vCurrentParam);

                            //add TreeWidgetitem
                            QTreeWidgetItem *item = new QTreeWidgetItem;

                            d.SetTreeWidgetItemIndex(item);
                            item->setIcon(0, QIcon(":/icon/uri/bittorrent.png"));
                            item->setText(0, "Torrent");
                            item->setText(1, sFileName);
                            ui->treeWidget->addTopLevelItem(item);

                            ComboBoxItem *combobox_template = new ComboBoxItem(item, 2);
                            combobox_template->addItem(tr("Default"));
                            foreach (util::TEMPLATES t, templates)
                            {
                                if (t.type == URI_TYPE_TORRENT)
                                {
                                    combobox_template->addItem(t.name);
                                }
                            }
                            ui->treeWidget->setItemWidget(item, 2, combobox_template);

                            m_dlist << d;
                        }
                    }
                    else
                    {
                        QMessageBox::critical(this, tr("Error"), tr("Torrent file read error"));
                    }
                }
            }
        }
        break;
    case URI_TYPE_MAGNETLINK:
        {
            const QClipboard *clipboard = QApplication::clipboard();
            QString sStr = clipboard->text(QClipboard::Clipboard);
            add_http_ftp_magnetlink add(this);
            QString sMagnetLink;

            add.SetURI(sStr);
            add.SetMenu(URI_TYPE_MAGNETLINK, templates, false);
            if (add.exec() == QDialog::Accepted)
            {
                sMagnetLink = add.GetURI();
                Download d;

                vCurrentParam.unite(util::TemplateFromName(templates, add.getTemplateName(), util::URI_TYPE_MAGNETLINK));
                d.addMagnetLink(sMagnetLink, vCurrentParam);

                QTreeWidgetItem *item = new QTreeWidgetItem;

                d.SetTreeWidgetItemIndex(item);
                item->setIcon(0, QIcon(":/icon/uri/magnet.png"));
                item->setText(0, "Magnet Link");
                item->setText(1, sMagnetLink);
                ui->treeWidget->addTopLevelItem(item);

                ComboBoxItem *combobox_template = new ComboBoxItem(item, 2);
                combobox_template->addItem(tr("Default"));
                foreach (util::TEMPLATES t, templates)
                {
                    if (t.type == URI_TYPE_MAGNETLINK)
                    {
                        combobox_template->addItem(t.name);
                    }
                }
                ui->treeWidget->setItemWidget(item, 2, combobox_template);

                m_dlist << d;
            }
        }
        break;
    case URI_TYPE_METALINK:
        {
            QFileDialog::Options options;
            QString selectedFilter;
            QIcon icon(":/icon/uri/metalink.png");

            QStringList sFileNameList = QFileDialog::getOpenFileNames(this,
                                        tr("Open metalink files"),
                                        util::LoadSetting(QString("LastOpenedMetalinkDirectory"),QString("Dir")),
                                        tr("Metalink Files (*.metalink)"),
                                        &selectedFilter,
                                        options);

            if (sFileNameList.size() > 0)
            {
                util::SaveSetting(QString("LastOpenedMetalinkDirectory"),QString("Dir"), QFileInfo(sFileNameList.front()).absolutePath());
            }
            foreach(QString sFileName, sFileNameList)
            {
                if (sFileName.size() != 0)
                {
                    Download d;

                    d.addMetalink(sFileName, vCurrentParam);

                    QTreeWidgetItem *item = new QTreeWidgetItem;

                    d.SetTreeWidgetItemIndex(item);
                    item->setIcon(0, icon);
                    item->setText(0, "Metalink");
                    item->setText(1, sFileName);
                    ui->treeWidget->addTopLevelItem(item);

                    ComboBoxItem *combobox_template = new ComboBoxItem(item, 2);
                    combobox_template->addItem(tr("Default"));
                    foreach (util::TEMPLATES t, templates)
                    {
                        if (t.type == URI_TYPE_METALINK)
                        {
                            combobox_template->addItem(t.name);
                        }
                    }
                    ui->treeWidget->setItemWidget(item, 2, combobox_template);

                    m_dlist << d;
                }
            }
         }
        break;
    }
}

void AddURI::Delete()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if (item)
    {
        if (QMessageBox::question(this, "Delete", "Do you really delete download?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
           int iIndex = ui->treeWidget->indexOfTopLevelItem(item);

           if (item->childCount() != 0)
           {
               QList<QTreeWidgetItem*> child = item->takeChildren();
               foreach(QTreeWidgetItem *item, child)
               {
                   int iCounter = 0;
                   foreach (Download d, m_dlist)
                   {
                       if (d.GetTreeWidgetItemIndex() == item)
                       {
                           m_dlist.removeAt(iCounter);
                           delete item;
                           break;
                       }
                       iCounter++;
                   }
               }
           }
           else
           {
               ui->treeWidget->takeTopLevelItem(iIndex);
               int iCounter = 0;
               foreach (Download d, m_dlist)
               {
                   if (d.GetTreeWidgetItemIndex() == item)
                   {
                       m_dlist.removeAt(iCounter);
                       break;
                   }

                   iCounter++;
               }
           }

           delete item;
           ui->pushButton_Delete->setEnabled(ui->treeWidget->topLevelItemCount() != 0);
           ui->pushButton_Modify->setEnabled(ui->treeWidget->topLevelItemCount() != 0);
        }
    }
}

void AddURI::Modify()
{
    if (bValidGlobalOption)
    {
        LocalOptions localoptions(this);
        QTreeWidgetItem *item = ui->treeWidget->currentItem();
        if (item)
        {
            if (item->childCount() == 0)
            {
                int iCounter = 0;
                foreach (Download d, m_dlist)
                {
                    if (d.GetTreeWidgetItemIndex() == item)
                    {
                        localoptions.setOptions(m_globalOptions, m_dlist[iCounter]);
                        if (localoptions.exec() == QDialog::Accepted)
                        {
                            m_dlist[iCounter].setCurrentParam(localoptions.getLocalOption());
                            static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(d.GetTreeWidgetItemIndex(), 2))->setCurrentIndex(-1);
                        }
                        break;
                    }
                    iCounter++;
                }
            }
            else
            {
                QTreeWidgetItem *item_Child = item->child(0);
                if (item_Child)
                {
                    int iCounter = 0;
                    foreach (Download d, m_dlist)
                    {
                        if (d.GetTreeWidgetItemIndex() == item_Child)
                        {
                            localoptions.setOptions(m_globalOptions, m_dlist[iCounter]);
                            if (localoptions.exec() == QDialog::Accepted)
                            {
                                int iChildCount = item->childCount();
                                for (int i = 0; i < iChildCount; ++i)
                                {
                                    int iC = 0;
                                    foreach (Download dd, m_dlist)
                                    {
                                        if (dd.GetTreeWidgetItemIndex() == item->child(i))
                                        {
                                            m_dlist[iC].setCurrentParam(localoptions.getLocalOption());
                                            static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(d.GetTreeWidgetItemIndex(), 2))->setCurrentIndex(0);
                                            break;
                                        }
                                        iC++;
                                    }
                                }
                            }
                            break;
                        }
                        iCounter++;
                    }
                }
            }
        }
    }
    else
    {
        Download d;
        QMap<QString, Variant> vCurrentParam;

        d.addGetGlobalOptions(vCurrentParam);
        bGlobalRequest = false;
        m_rT->addRequest(d);
        m_rT->setDescriptionText(tr("Get global variables"));
        m_rT->wakeUp();
    }
}

void AddURI::Type_Change(int iPos)
{
    m_iCurrentType = iPos;
}

void AddURI::on_treeWidget_itemClicked(QTreeWidgetItem* item, int column)
{
    ui->pushButton_Delete->setEnabled(true);
    ui->pushButton_Modify->setEnabled(true);
}

void AddURI::on_treeWidget_customContextMenuRequested(QPoint pos)
{

    QMenu contextMnu(this);
    QMouseEvent *mevent = new QMouseEvent( QEvent::MouseButtonPress, pos, Qt::RightButton, Qt::RightButton, Qt::NoModifier );

    QAction *moveAdd = new QAction( tr( "Add ..." ), this );
    QAction *moveupAct = new QAction( tr( "Delete ..." ), this );
    QAction *movedownAct = new QAction( tr( "Modify ..." ), this );

    connect( moveAdd , SIGNAL( triggered() ), this, SLOT( Add() ) );
    connect( moveupAct , SIGNAL( triggered() ), this, SLOT( Delete() ) );
    connect( movedownAct , SIGNAL( triggered() ), this, SLOT( Modify() ) );

    contextMnu.clear();
    contextMnu.addAction( moveAdd );
    contextMnu.addAction( moveupAct );
    contextMnu.addAction( movedownAct );
    contextMnu.exec( mevent->globalPos() );
}

void AddURI::on_pushButton_GlobalOptions_clicked()
{
    Download d;
    QMap<QString, Variant> vCurrentParam;

    d.addGetGlobalOptions(vCurrentParam);
    bGlobalRequest = true;
    m_rT->addRequest(d);
    m_rT->setDescriptionText(tr("Get global variables"));
    m_rT->wakeUp();
}

void AddURI::GetGlobalOptions(QVariant value)
{
    XmlRPC x;
    m_globalOptions = x.OptionToMap(value);
    bValidGlobalOption = true;

    if (bGlobalRequest)
    {
        GlobalOptions g(this);
        g.SetGlobalOptions(m_globalOptions);
        if (g.exec() == QDialog::Accepted)
        {
            QMap<QString, Variant> m_newGlobalOptions = g.GetNewGlobalOptions();
            QMap<QString, Variant> vCurrentParam;

            bool bFoundChangeParams = false;
            for (size_t i = 0; i < sizeof(GlobalChangeableOption) / sizeof(GlobalChangeableOption[0]); ++i)
            {
                bool ok;
                if (m_globalOptions[GlobalChangeableOption[i]].toInt(&ok) != m_newGlobalOptions[GlobalChangeableOption[i]].toInt(&ok))
                {
                    bFoundChangeParams = true;
                    vCurrentParam[GlobalChangeableOption[i]] = m_newGlobalOptions[GlobalChangeableOption[i]].toString();
                }
            }

            if (bFoundChangeParams)
            {
                Download d;
                d.addChangeGlobalOptions(vCurrentParam);
                m_rT->addRequest(d);
                m_rT->setDescriptionText(tr("Change global variables"));
                m_rT->wakeUp();
            }
        }
    }
    else
    {
        LocalOptions localoptions(this);

        QTreeWidgetItem *item = ui->treeWidget->currentItem();
        if (item)
        {
            if (item->childCount() == 0)
            {
                int iCounter = 0;
                foreach (Download d, m_dlist)
                {
                    if (d.GetTreeWidgetItemIndex() == item)
                    {
                        localoptions.setOptions(m_globalOptions, m_dlist[iCounter]);
                        if (localoptions.exec() == QDialog::Accepted)
                        {
                            m_dlist[iCounter].setCurrentParam(localoptions.getLocalOption());
                            static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(d.GetTreeWidgetItemIndex(), 2))->setCurrentIndex(-1);
                        }
                        break;
                    }
                    iCounter++;
                }
            }
            else
            {
                QTreeWidgetItem *item_Child = item->child(0);
                if (item_Child)
                {
                    int iCounter = 0;
                    foreach (Download d, m_dlist)
                    {
                        if (d.GetTreeWidgetItemIndex() == item_Child)
                        {
                            localoptions.setOptions(m_globalOptions, m_dlist[iCounter]);
                            if (localoptions.exec() == QDialog::Accepted)
                            {
                                int iChildCount = item->childCount();
                                for (int i = 0; i < iChildCount; ++i)
                                {
                                    int iC = 0;
                                    foreach (Download dd, m_dlist)
                                    {
                                        if (dd.GetTreeWidgetItemIndex() == item->child(i))
                                        {
                                            m_dlist[iC].setCurrentParam(localoptions.getLocalOption());
                                            static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(d.GetTreeWidgetItemIndex(), 2))->setCurrentIndex(-1);
                                            break;
                                        }
                                        iC++;
                                    }
                                }
                            }
                            break;
                        }
                        iCounter++;
                    }
                }
            }
        }
    }
}

void AddURI::processFaultToUI( int requestId, int errorCode, QString errorString )
{
    m_globalOptions.clear();
    bValidGlobalOption = false;
    QMessageBox::warning(this, tr("Request failed"), QString("XML-RPC request  failed.\n\nFault code: %1\n%2\n").arg(errorCode).arg(errorString), QMessageBox::Ok );
}

void AddURI::on_pushButton_Template_clicked()
{
    Templates temp(this);
    temp.setTemplates(templates);
    if (temp.exec() == QDialog::Accepted)
    {
        templates = temp.getTemplates();
        foreach (Download d, m_dlist)
        {
            QTreeWidgetItem *item = d.GetTreeWidgetItemIndex();
            QString temp_name = static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(item, 2))->currentText();
            int pos = static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(item, 2))->currentIndex();
            XML_TYPE t = d.getType();
            URI_TYPE u = (t == HTTP_FTP) ? (URI_TYPE_HTTP_FTP) :(
                         (t == TORRENT) ? (URI_TYPE_TORRENT) :(
                         (t == MAGNETLINK) ? (URI_TYPE_MAGNETLINK) : (
                         (t == METALINK) ? (URI_TYPE_METALINK) :(URI_TYPE_NONE)
                         ) ) );
            static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(item, 2))->clear();
            static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(item, 2))->addItem(tr("Default"));

            foreach (TEMPLATES t, templates)
            {
                if (t.type == u)
                {
                    static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(item, 2))->addItem(t.name);
                    if ( (pos != -1) && (temp_name.compare(t.name, Qt::CaseInsensitive) == 0) )
                        static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(item, 2))->setCurrentIndex(pos);
                }
            }
            if (pos == -1)
                static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(item, 2))->setCurrentIndex(pos);
        }
    }
}

void AddURI::on_treeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    if (item != NULL)
    {
        int childCount = item->childCount();
        if ( (column == 2) && (childCount > 0) )
        {
            for (int i = 0; i < childCount; i++)
            {
                static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(item->child(i), 2))->setCurrentIndex(0);
            }
        }
    }
}

void AddURI::on_pushButton_OK_clicked()
{
    int iCounter = 0;
    foreach(Download d, m_dlist)
    {
        QMap<QString, Variant> param = d.getCurrentParam();
        QMap<QString, Variant> paramMerge;
        QString name = static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(d.GetTreeWidgetItemIndex(), 2))->currentText();
        int index = static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(d.GetTreeWidgetItemIndex(), 2))->currentIndex();
        if (index == -1)
        {
            //custom
            paramMerge = param;
        }
        else
        {
            //Template
            XML_TYPE t = d.getType();
            if (d.GetTreeWidgetItemIndex()->parent())
            {
                //multi download
                if (name.compare(tr("Default"), Qt::CaseInsensitive) == 0)
                {
                    QString parentName = static_cast<ComboBoxItem*>(ui->treeWidget->itemWidget(d.GetTreeWidgetItemIndex()->parent(), 2))->currentText();
                    paramMerge = util::TemplateFromName(templates, parentName, URI_TYPE_MULTI_HTTP_FTP);
                }
                else
                {
                    paramMerge = util::TemplateFromName(templates, name, URI_TYPE_HTTP_FTP);
                }
            }
            else
            {
                //single download
                if (name.compare(tr("Default"), Qt::CaseInsensitive) == 0)
                {
                    paramMerge = param;
                }
                else
                {
                    URI_TYPE u = (t == HTTP_FTP) ? (URI_TYPE_HTTP_FTP) :(
                                 (t == TORRENT) ? (URI_TYPE_TORRENT) :(
                                 (t == MAGNETLINK) ? (URI_TYPE_MAGNETLINK) : (
                                 (t == METALINK) ? (URI_TYPE_METALINK) :(URI_TYPE_NONE)
                                 ) ) );
                    paramMerge = util::TemplateFromName(templates, name, u);
                }
            }
        }
        m_dlist[iCounter].setCurrentParam(paramMerge, false);
        iCounter++;
    }
}
