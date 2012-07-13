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

#ifndef ADD_HTTP_FTP_MAGNETLINK_H
#define ADD_HTTP_FTP_MAGNETLINK_H

#include <QDialog>
#include <QString>
#include <QMenu>
#include "xmlrpc.h"
#include <QActionGroup>
#include "util.h"

namespace Ui {
    class add_http_ftp_magnetlink;
}

using namespace xmlrpc;
using namespace util;

class add_http_ftp_magnetlink : public QDialog {
    Q_OBJECT
public:
    add_http_ftp_magnetlink(QWidget *parent = 0);
    ~add_http_ftp_magnetlink();
    QString GetURI();
    QString getTemplateName() {return templateName; }
    void SetURI(QString& s);
    void SetMenu(URI_TYPE t, const QList<util::TEMPLATES> &temp, bool enableTemplateButton = true);

protected:
    void changeEvent(QEvent *e);

private slots:
    void slotMenuChanged(QAction* action);

private:
    Ui::add_http_ftp_magnetlink *ui;
    QMenu *menuTemplate;
    QString templateName;
    QList<util::TEMPLATES> templates;
    URI_TYPE type;
    QActionGroup* MenuGroup;
};

#endif // ADD_HTTP_FTP_MAGNETLINK_H
