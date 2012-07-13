#ifndef PROGRESSBARVIEWDELEGATE_H
#define PROGRESSBARVIEWDELEGATE_H
#include <QItemDelegate>
#include "aria2cremote.h"

// TorrentViewDelegate is used to draw the progress bars.
class ProgressBarViewDelegate : public QItemDelegate
{
   // Q_OBJECT
public:
     ProgressBarViewDelegate(Aria2cRemote *mainWindow) : QItemDelegate(mainWindow) {}
     void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};
#endif // PROGRESSBARVIEWDELEGATE_H
