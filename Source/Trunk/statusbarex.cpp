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
#include "statusbarex.h"
#include <QtGui>
#include <QString>
#include <QLayout>
#include <QLabel>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include "util.h"

using namespace util;

StatusBarEx::StatusBarEx(QWidget *parent)
 : QWidget(parent)
{
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setMargin(0);
    hbox->setSpacing(6);
    
    iconLabel = new QLabel( this );
    iconLabel->setPixmap(QPixmap::QPixmap(":/icon/down0up0.png"));

    hbox->addWidget(iconLabel, 0, false);

    statusText = new QLabel( tr("<strong>Down:</strong> 0.00 KiB/s | <strong>Up:</strong> 0.00 KiB/s "), this );
    hbox->addWidget(statusText);
    
    setLayout( hbox );
}

StatusBarEx::~StatusBarEx()
{
}

void StatusBarEx::setStatusBar(quint64 downspeed, quint64 upspeed)
{
    QString sText(tr("<strong>Down:</strong> %1 | <strong>Up:</strong> %2").arg(util::ConvertNumberToSuffixString(downspeed) + "/s").arg(util::ConvertNumberToSuffixString(upspeed) + "/s"));

    if (statusText)
          statusText->setText(sText);
    		
    if ( (upspeed > 0) && (downspeed <= 0) )
    {
        iconLabel->setPixmap(QPixmap::QPixmap(":/icon/down0up1.png"));
    }
    else
    {
        if ( (upspeed <= 0) && (downspeed > 0) )
        {
            iconLabel->setPixmap(QPixmap::QPixmap(":/icon/down1up0.png"));
        }
        else
        {
            if ( (upspeed > 0) && (downspeed > 0) )
            {
                iconLabel->setPixmap(QPixmap::QPixmap(":/icon/down1up1.png"));
            }
            else
            {
                iconLabel->setPixmap(QPixmap::QPixmap(":/icon/down0up0.png"));
            }
        }
    }
}


