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
