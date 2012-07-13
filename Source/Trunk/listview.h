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

#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QTreeWidget>

namespace Ui {
    class ListView;
}

/*
class MyTreeWidget : public QTreeWidget
{
Q_OBJECT

public:
MyTreeWidget(QWidget* parent = 0)
: QTreeWidget(parent)
{
// disable built-in sorting
setSortingEnabled(false);

// use our own sorting method instead
header()->setSortIndicatorShown(true);
header()->setClickable(true);
connect(header(), SIGNAL(sectionClicked(int)), this, SLOT(customSortByColumn(int)));
customSortByColumn(header()->sortIndicatorSection());
}

public slots:
void customSortByColumn(int column)
{
// here you can get the order
Qt::SortOrder order = header()->sortIndicatorOrder();

// and sort the items
sortItems(column, order);

// to get more control over actual sorting of items,
// reimplement QTreeWidgetItemEx::operator<()
}
};
*/
class ListView : public QTreeWidget {
    Q_OBJECT
public:
    ListView(QWidget *parent = 0);
    ~ListView();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ListView *ui;
};

#endif // LISTVIEW_H
