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

#ifndef ADDURI_H
#define ADDURI_H

#include <QDialog>
#include <QModelIndex>
#include "QTreeWidgetItemEx.h"
#include <QMessageBox>
#include "metainfo.h"
#include "download.h"
#include "requestthread.h"
#include "util.h"

namespace Ui {
    class AddURI;
}

using namespace util;

class AddURI : public QDialog {
    Q_OBJECT
public:
    AddURI(QWidget *parent = 0);
    QList<Download> getDownloadList() {return m_dlist; }
    void setRequestThread(RequestThread *rt);
    bool isValidGlobalOption() {return bValidGlobalOption; }
    QMap<QString, Variant> getGlobalOption() {return m_globalOptions; }
    ~AddURI();

protected:
    void changeEvent(QEvent *e);

private slots:
    void GetGlobalOptions(QVariant value);
    void processFaultToUI( int requestId, int errorCode, QString errorString );

private:
    Ui::AddURI *ui;
    int m_iCurrentType;
    QList<Download> m_dlist;
    RequestThread *m_rT;
    QMap<QString, Variant> m_globalOptions;
    bool bValidGlobalOption;
    bool bGlobalRequest;
    QList<util::TEMPLATES> templates;

private slots:
    void on_pushButton_GlobalOptions_clicked();
    void on_treeWidget_customContextMenuRequested(QPoint pos);
    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
    void Add();
    void Delete();
    void Modify();
    void Type_Change(int iPos);
    void on_pushButton_Template_clicked();
    void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);
    void on_pushButton_OK_clicked();
};

#endif // ADDURI_H
