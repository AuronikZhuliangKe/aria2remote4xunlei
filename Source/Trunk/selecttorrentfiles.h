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

#ifndef SELECTTORRENTFILES_H
#define SELECTTORRENTFILES_H

#include <QDialog>
#include <QList>
#include <QFileIconProvider>
#include <QTreeWidgetItem>
#include "metainfo.h"

namespace Ui {
    class SelectTorrentFiles;
}
typedef struct Icon_Colection
{
    QIcon icon;
    QString description;
}ICON_COLLECTION, *PICON_COLLECTION;

typedef struct Dir_Item_Collenction
{
    QTreeWidgetItem* item;
    QList<QString> dir_list;
    qint64 iDirSize;
    QString dir;
}DIR_ITEM_COLLECTION, *PDIR_ITEM_COLLECTION;

class SelectTorrentFiles : public QDialog {
    Q_OBJECT
public:
    SelectTorrentFiles(QWidget *parent = 0);
    ~SelectTorrentFiles();
    void SetData(MetaInfo minfo);
    QString getSelectedFileList();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SelectTorrentFiles *ui;
    MetaInfo metainfo;
    int m_markedCount;
    QList<QTreeWidgetItem*> m_torrentFileState;

private slots:
    void on_treeWidget_itemChanged(QTreeWidgetItem* item, int column);
};

#endif // SELECTTORRENTFILES_H
