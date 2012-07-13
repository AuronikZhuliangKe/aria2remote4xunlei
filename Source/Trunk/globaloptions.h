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

#ifndef GlobalOptions_H
#define GlobalOptions_H

#include <QDialog>
#include "xmlrpc/variant.h"

using namespace xmlrpc;

namespace Ui {
    class GlobalOptions;
}

class GlobalOptions : public QDialog {
    Q_OBJECT
public:
    GlobalOptions(QWidget *parent = 0);
    ~GlobalOptions();

    void SetGlobalOptions(QMap<QString, Variant> options);
    void SetCurrentAria2cVersion(quint64 uiVersion) { uiAria2cVersion = uiVersion; }
    QMap<QString, Variant> GetNewGlobalOptions() { return m_newGlobalOptions; }
    void SetBasicOptions();
    void SetHTTPOptions();
    void SetFTPOptions();
    void SetBittorrentOptions();
    void SetMetalinkOptions();
    void SetAdvancedOptions();
    void SetAdvanced1Options();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::GlobalOptions *ui;
    QMap<QString, Variant> m_globalOptions;
    QMap<QString, Variant> m_newGlobalOptions;

    quint64 uiAria2cVersion;
private slots:
    void MaxOverallDownloadLimitChanged( int );
    void MaxOverallUploadLimitChanged( int );
    void MaxConcurrentDownloadsChanged( int );
    void LogLevelChanged( QString );
    void MaxDownloadResult( int );
    void DownloadResult( int );
};

#endif // GlobalOptions_H
