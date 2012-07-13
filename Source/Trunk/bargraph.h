/*
 * Bittorrent Client using Qt4 and libtorrent.
 * Copyright (C) 2006  Christophe Dumez
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * In addition, as a special exception, the copyright holders give permission to
 * link this program with the OpenSSL project's "OpenSSL" library (or with
 * modified versions of it that use the same license as the "OpenSSL" library),
 * and distribute the linked executables. You must obey the GNU General Public
 * License in all respects for all of the code used other than "OpenSSL".  If you
 * modify file(s), you may extend this exception to your version of the file(s),
 * but you are not obligated to do so. If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * Contact : chris@qbittorrent.org
 */

#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <numeric>
#include <math.h>

#define BAR_HEIGHT 18

class bargraph: public QWidget {
  Q_OBJECT

private:
  QPixmap pixmap;
  quint64 m_max_value;
  QColor m_BackColor;

public:
  bargraph(QWidget *parent):
          QWidget(parent),
          m_BackColor(Qt::white)
  {
      setFixedHeight(BAR_HEIGHT);
  }

  void setBackColor(QColor backcolor) { m_BackColor = backcolor; }

  void setDoneBar(quint64 totalLength, quint64 completeLength)
  {
      int w = width();
      int c = 0;

      if (totalLength > 0)
      {
          c = completeLength * w / totalLength;
      }

      QPixmap pix = QPixmap(w, 1);
      pix.fill();
      QPainter painter(&pix);
      painter.setPen(Qt::blue);
      painter.drawLine(0, 0, c, 0);
      painter.setPen(Qt::white);
      painter.drawLine(c, 0, w, 0);
      pixmap = pix;
  }

  double setPiecesBar(std::vector<quint64>& pieces)
  {
      return setAvailabilityBar(pieces);
  }

  double setAvailabilityBar(std::vector<quint64>& avail)
  {
      double average = 0;
      m_max_value = 0;

      if (!isEnabled())
      {
          QPixmap pix = QPixmap(1, 1);
          pix.fill();
          QPainter painter(&pix);
          painter.setPen(Qt::lightGray);
          painter.drawLine(0,0,width(), 0);
          pixmap = pix;
      }
      else
      {
          if(avail.empty())
          {
              // Empty bar
              QPixmap pix = QPixmap(1, 1);
              pix.fill();
              pixmap = pix;
          }
          else
          {
              // Look for maximum value
              int nb_pieces = avail.size();
              average = std::accumulate(avail.begin(), avail.end(), 0)/(double)nb_pieces;

              for (int i = 0; i < nb_pieces; ++i)
              {
                  if (m_max_value < avail[i])
                      m_max_value = avail[i];
              }
              // Reduce the number of pieces before creating the pixmap
              // otherwise it can crash when there are too many pieces
              if (nb_pieces > width())
              {
                  int ratio = floor(nb_pieces/(double)width());
                  std::vector<int> scaled_avail;
                  for(int i=0; i<nb_pieces; i+= ratio)
                  {
                      int j = i;
                      int sum = avail[i];
                      for(j = i + 1; j < qMin(i + ratio, nb_pieces); ++j) {
                          sum += avail[j];
                      }
                      scaled_avail.push_back(sum / (qMin(ratio, nb_pieces-i)));
                  }
                  QPixmap pix = QPixmap(scaled_avail.size(), 1);
                  pix.fill();
                  QPainter painter(&pix);
                  for(qulonglong i=0; i < scaled_avail.size(); ++i)
                  {
                      painter.setPen(getPieceColor(scaled_avail[i]));
                      painter.drawPoint(i,0);
                  }
                  pixmap = pix;
              }
              else
              {
                  QPixmap pix = QPixmap(nb_pieces, 1);
                  pix.fill();
                  QPainter painter(&pix);
                  for(int i=0; i < nb_pieces; ++i)
                  {
                      painter.setPen(getPieceColor(avail[i]));
                      painter.drawPoint(i,0);
                  }
                  pixmap = pix;
              }
          }
      }

      update();
      return average;
  }

  void clear()
  {
      pixmap = QPixmap();
      update();
  }

protected:
  void paintEvent(QPaintEvent *)
  {
      if(pixmap.isNull()) return;
      QPainter painter(this);
      painter.drawPixmap(rect(), pixmap);
  }

  QColor getPieceColor(int avail)
  {
      QColor Ret(m_BackColor);

      if (avail > 0)
      {
          Ret = (m_max_value > 0) ? QColor((255 * (m_max_value - avail)) / m_max_value, (255 * (m_max_value - avail)) / m_max_value, 255, 255) : Qt::white;
      }
      return Ret;
  }
};

#endif // BARGRAPH_H
