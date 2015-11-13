#include <QGraphicsView>
#include "ReportGraphicsView.h"


ReportGraphicsView::ReportGraphicsView(QWidget* parent)
  :QGraphicsView(parent), zoomPercent(100)
{

}

//---------------------------------------------------------------------------

ReportGraphicsView::ReportGraphicsView(QGraphicsScene* scene, QWidget* parent)
  :QGraphicsView(scene, parent), zoomPercent(100)
{

}

//---------------------------------------------------------------------------

void ReportGraphicsView::setZoomFactor(int _zoomPercent)
{
  // ensure valid values
  if (_zoomPercent <= 0) return;

  // store the new zoom factor and repaint the widget
  zoomPercent = _zoomPercent;

  // maximize the scene on the screen
  QRectF sceneExtends = sceneRect();
  double scaleX = width() / sceneExtends.width();
  double scaleY = height() / sceneExtends.height();
  double viewScale = std::min(scaleX, scaleY);

  // apply an additional internal zoom factor
  // a value of "100" means "100 %" means "fit all on screen"
  viewScale *= zoomPercent / 100.0;

  // scale the scene by applying the zoom factor
  resetTransform();
  scale(viewScale, viewScale);

  // tell the world that the view's zoom has changed
  emit viewZoomFactorChanged(zoomPercent);

  // repaint
  this->repaint();
}

//---------------------------------------------------------------------------

void ReportGraphicsView::setZoomFactor_Delta(int deltaPercent)
{
  setZoomFactor(zoomPercent + deltaPercent);  // a bit ugly, but...
}

//---------------------------------------------------------------------------

int ReportGraphicsView::getZoomFactor() const
{
  return zoomPercent;
}

//---------------------------------------------------------------------------

void ReportGraphicsView::recalcZoom()
{
  // triggers a recalculation of the view's scaling factor
  // by re-applying the old zoom factor
  //
  // Reason: if the widget was initialized without a report,
  // the scaling factor can't be established. If then subsequently
  // a report is assigned, the scaling factors need to be recalculated
  // based on the new report's dimensions
  setZoomFactor(zoomPercent);
}

//---------------------------------------------------------------------------

void ReportGraphicsView::wheelEvent(QWheelEvent* ev)
{
  // ignore / propagate all wheel events without Ctrl-key
  if (ev->modifiers() != Qt::ControlModifier)
  {
    QGraphicsView::wheelEvent(ev);  // keep scrolling etc. alive
    return;
  }

  ev->accept();

  // ignore wheel events without actual rotation
  auto degrees = ev->angleDelta() / 8;
  if (degrees.isNull()) return;

  // increase or decrease the zoom level by 10%
  int step = 10;

  if ((degrees.x() <= 0) && (degrees.y() <= 0))
  {
    step *= -1;
  }

  setZoomFactor_Delta(step);
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

