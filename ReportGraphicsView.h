/*
 *    This is SimpleReportGenerator, a very basic report generator on top of Qt.
 *    Copyright (C) 2014 - 2015  Volker Knollmann
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef REPORTGRAPHICSVIEW_H
#define REPORTGRAPHICSVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QWheelEvent>

class ReportGraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  ReportGraphicsView(QWidget * parent = 0);
  ReportGraphicsView(QGraphicsScene * scene, QWidget * parent = 0);
  void setZoomFactor(int _zoomPercent);
  void setZoomFactor_Delta(int deltaPercent);
  int getZoomFactor() const;
  void recalcZoom();

  virtual void wheelEvent(QWheelEvent* ev);

signals:
  void viewZoomFactorChanged(int newZoomFactor);

protected:
  int zoomPercent;
};

#endif // REPORTGRAPHICSVIEW_H
