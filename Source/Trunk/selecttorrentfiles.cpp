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

#include "selecttorrentfiles.h"
#include "ui_selecttorrentfiles.h"
#include "util.h"
#include <QDir>

SelectTorrentFiles::SelectTorrentFiles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectTorrentFiles),
    m_markedCount(0)
{
    ui->setupUi(this);
    QByteArray Geometry = QByteArray::fromBase64(util::LoadSetting("SelectTorrentFiles", "Geometry").toAscii());
    if (Geometry.size() > 0)
    {
        restoreGeometry(Geometry);
    }
    QByteArray SelectTorrentFilesHeaderState = QByteArray::fromBase64(util::LoadSetting("SelectTorrentFiles", "HeaderState").toAscii());
    if (SelectTorrentFilesHeaderState.size() > 0)
    {
        ui->treeWidget->header()->restoreState(SelectTorrentFilesHeaderState);
    }
}

QString SelectTorrentFiles::getSelectedFileList()
{
    QString selectFiles;
    int iFirstItem = 0;
    bool bFoundFirstItem = false;

    int iSize = m_torrentFileState.size();
    for (int i = 0; i < iSize; i++)
    {

        if (m_torrentFileState.at(i)->checkState(0) == Qt::Checked)
        {
            if (i == (iSize - 1))
            {
                if (selectFiles.size() != 0)
                    selectFiles += ",";

                if (!bFoundFirstItem)
                    selectFiles += QString::number(i + 1);
                else
                    selectFiles += (QString::number(iFirstItem + 1) + ( ((i - iFirstItem) < 2) ? "," : "-" ) + QString::number(i + 1));
            }
            else
            {
                if (!bFoundFirstItem)
                {
                    iFirstItem = i;
                    bFoundFirstItem = true;
                }
             }
        }
        else
        {
            if (bFoundFirstItem)
            {
                bFoundFirstItem = false;
                if (selectFiles.size() != 0)
                    selectFiles += ",";

                if ( (iFirstItem + 1) != i)
                    selectFiles += (QString::number(iFirstItem + 1) + ( ((i - (iFirstItem + 1)) < 2) ? "," : "-" ) + QString::number(i));
                else
                    selectFiles += QString::number(i);
             }
        }
    }
    return selectFiles;
}

SelectTorrentFiles::~SelectTorrentFiles()
{
    util::SaveSetting("SelectTorrentFiles", "Geometry", QString(saveGeometry().toBase64()));
    util::SaveSetting("SelectTorrentFiles", "HeaderState", QString(ui->treeWidget->header()->saveState().toBase64()));
    delete ui;
}

void SelectTorrentFiles::SetData(MetaInfo minfo)
{
    QTreeWidgetItem *rootItem, *child;
    QFileIconProvider fileIconProvider;
    QIcon iconFolder(fileIconProvider.icon(QFileIconProvider::Folder));
    QIcon iconFile(fileIconProvider.icon(QFileIconProvider::File));

    metainfo = minfo;

    ui->label_Torrent_Name->setText(metainfo.name());
    ui->label_Torrent_Size->setText(util::ConvertNumberToSuffixString(metainfo.totalSize()));
    ui->label_Torrent_Comment->setText(metainfo.comment());
    ui->label_Torrent_CreatedBy->setText(metainfo.createdBy());
    ui->label_Torrent_Date->setText(metainfo.creationDate().toString(Qt::DefaultLocaleShortDate));
    ui->treeWidget->clear();

    //root item
    rootItem = new QTreeWidgetItem;
    rootItem->setText(0, (metainfo.name().size() == 0) ? (QString("/")) : (metainfo.name()) );
    rootItem->setText(1, tr("Directory") );
    rootItem->setText(2, util::ConvertNumberToSuffixString(metainfo.totalSize()));
    rootItem->setCheckState(0, Qt::Checked);
    rootItem->setIcon(0, iconFolder);

    if (metainfo.fileForm() == metainfo.MultiFileForm)
    {
        QList<QString> m_lsDir;
        QList<DIR_ITEM_COLLECTION> items;

        foreach (MetaInfoMultiFile file, metainfo.multiFiles())
        {
            QString s;
            QString path;
            QTreeWidgetItem *currentDirectory = rootItem;

            foreach(QString s, file.Directories)
            {
                if (path.size() != 0)
                {
                    path += "/";
                }
                path += s;

                int iFound = -1;
                for (int itr = 0; itr < items.size(); itr++)
                {
                    if (items[itr].dir == path)
                    {
                        iFound = itr;
                        break;
                    }
                }

                if (iFound == -1)
                {
                    DIR_ITEM_COLLECTION dic;

                    dic.dir_list = file.Directories;
                    dic.item = new QTreeWidgetItem(currentDirectory);
                    dic.item->setText(0, s );
                    dic.item->setText(1, tr("Directory") );
                    dic.iDirSize = file.length;
                    dic.item->setCheckState(0, Qt::Checked);
                    dic.item->setIcon(0, iconFolder);
                    dic.dir = path;
                    items.push_back(dic);
                    currentDirectory = dic.item;
                }
                else
                {
                    DIR_ITEM_COLLECTION &d = items[iFound];
                    d.iDirSize += file.length;
                    currentDirectory = d.item;
                }
            }

            QFileInfo fi(file.path + file.filename);
            QString sDesc;
            QIcon icon = util::getWinIcon(fi, sDesc);

            child = new QTreeWidgetItem(currentDirectory);
            child->setText(0, file.filename);
            child->setText(1, sDesc);
            child->setText(2, util::ConvertNumberToSuffixString(file.length));
            child->setCheckState(0, Qt::Checked);
            child->setIcon(0, icon);

            m_torrentFileState << child;


            //set DirSize
            size_t size = items.size();

            for (size_t i = 0; i < size; ++i)
            {
                DIR_ITEM_COLLECTION &d = items[i];
                d.item->setText(2, util::ConvertNumberToSuffixString(d.iDirSize));
            }
            rootItem->addChild(child);
        }
    }
    else
    {
        QString sDesc;
        QFileInfo fi(metainfo.singleFile().name);
        QIcon icon = util::getWinIcon(fi, sDesc);
        QString name = metainfo.singleFile().name;

        child = new QTreeWidgetItem;
        child->setText(0, name);
        child->setText(1, sDesc);
        child->setText(2, util::ConvertNumberToSuffixString(metainfo.singleFile().length));
        child->setCheckState(0, Qt::Checked);
        child->setIcon(0, icon);

        rootItem->addChild(child);
    }

    ui->treeWidget->addTopLevelItem(rootItem);
}

void SelectTorrentFiles::changeEvent(QEvent *e)
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

void SelectTorrentFiles::on_treeWidget_itemChanged(QTreeWidgetItem* item, int column)
{
    if (!item || (column != 0))
        return;

    Qt::CheckState state = item->checkState(0);
    QTreeWidgetItem *parent = item->parent();

    if (parent)
    {
        Qt::CheckState parentstate = parent->checkState(0);

        if (state == Qt::Checked &&
            parentstate == Qt::Unchecked)
        {
            // Mark parent items when child items are checked.
            parent->setCheckState(0, Qt::Checked);

        } else if (state == Qt::Unchecked &&
                   parentstate == Qt::Checked)
        {
            int ChildCount = parent->childCount();
            int number = 0;
            for (int row = 0; row < ChildCount; ++row)
            {
                if (parent->child(row)->checkState(0) == Qt::Checked)
                    ++number;
            }
            // Unmark parent items when all child items are unchecked or partiallyChecked.
            if ( number == 0 )
                    parent->setCheckState(0, Qt::Unchecked);
            else
                    parent->setCheckState(0, Qt::Checked);
        }

    }

    int number = 0;
    int ChildCount = item->childCount();
    for (int row = 0; row < ChildCount; ++row)
    {
        if (item->child(row)->checkState(0) == Qt::Checked)
            ++number;
    }

    // Mark/unmark all child items when marking/unmarking top-level
    // items.
    if (state == Qt::Checked && number == 0)
    {
        for (int row = 0; row < ChildCount; ++row)
        {
            if (item->child(row)->checkState(0) == Qt::Unchecked)
                item->child(row)->setCheckState(0, Qt::Checked);
        }
    } else if (state == Qt::Unchecked && number > 0)
    {
        for (int row = 0; row < ChildCount; ++row)
        {
            if (item->child(row)->checkState(0) == Qt::Checked)
                item->child(row)->setCheckState(0, Qt::Unchecked);
        }
    }
}
