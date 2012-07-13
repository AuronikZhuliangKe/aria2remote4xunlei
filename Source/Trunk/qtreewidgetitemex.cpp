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

#include "qtreewidgetitemex.h"

QTreeWidgetItemEx::QTreeWidgetItemEx()
{
}

bool QTreeWidgetItemEx::operator<(const QTreeWidgetItem &other) const
{
    int column = treeWidget()->sortColumn();
    QVariant v = data(column, Qt::UserRole);
    QVariant vo = other.data(column, Qt::UserRole);
    bool bRet = false;

    if (v.isValid() && vo.isValid())
    {
        bool ok;
        bRet = (
                ( (v.type() == QVariant::ULongLong) && (v.toULongLong()) < vo.toULongLong() ) ||
                ( (v.type() == QVariant::Double) && (v.toDouble(&ok) < vo.toDouble(&ok)) ) ||
                ( (v.type() == QVariant::LongLong) && (v.toLongLong(&ok) < vo.toLongLong(&ok)) ) ||
                ( (v.type() == QVariant::UInt) && (v.toUInt(&ok) < vo.toUInt(&ok)) )
               );
    }
    else
    {
        bRet = text( column ) < other.text( column );
    }
    return bRet;
}
