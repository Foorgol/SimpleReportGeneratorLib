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

#include <stdexcept>

#include <QPrinter>
#include <QPrintDialog>
#include <QDebug>
#include <QWheelEvent>

#include "SimpleReportViewer.h"
#include "ui_SimpleReportViewer.h"

namespace SimpleReportLib {

SimpleReportViewer::SimpleReportViewer(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SimpleReportViewer),
  report(nullptr)
{
  ui->setupUi(this);

  ui->sbPage->setMaximum(MAX_NUM_PAGES);
  //ui->sbPage->setValue(1);

  ui->gv->setBackgroundBrush(QBrush(Qt::lightGray));

  // connect signals and slots for view zoom factor changes
  connect(ui->gv, SIGNAL(viewZoomFactorChanged(int)), this, SLOT(onGraphicsViewZoomFactorChanged(int)), Qt::DirectConnection);

  // start with a default zoom factor of 100% (page fit)
  ui->gv->setZoomFactor(100);

  updateButtons();
}

//---------------------------------------------------------------------------

SimpleReportViewer::~SimpleReportViewer()
{
  delete ui;
}

//---------------------------------------------------------------------------

bool SimpleReportViewer::setReport(SimpleReportGenerator *r)
{
  if (r == nullptr)
  {
    report = nullptr;
    updateButtons();
    return true;
  }

  if (r->getPageCount() == 0) return false;

  report = r;
  ui->sbPage->setMaximum(report->getPageCount());
  showPage(0);
  updateButtons();

  return true;
}

//---------------------------------------------------------------------------

void SimpleReportViewer::refreshDisplayedContent()
{
  if (report == nullptr) return;
  showPage(curPage);
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onBtnPrintClicked()
{
    QPrinter printer;

    // initialize the page ranges for the print dialog
    int numPages = report->getPageCount();
    printer.setFromTo(1, numPages);
    printer.setPrintRange(QPrinter::AllPages);

    // adjust the page orientation, depending
    // on our actual page size
    if (report->getPageWidth() > report->getPageHeight())
    {
      printer.setPageOrientation(QPageLayout::Landscape);
    } else {
      printer.setPageOrientation(QPageLayout::Portrait);
    }

    if (QPrintDialog(&printer, this).exec() == QDialog::Accepted) {
      // get the page range from the printer dialog
      int firstPage = printer.fromPage();
      int lastPage = printer.toPage();

      if (firstPage > 0)
      {
        --firstPage;   // dialog starts counting at 1, we start counting at zero
      }
      if (lastPage > 0)
      {
        --lastPage;
      } else {
        lastPage = numPages-1;
      }

      // the actual printing
      QPainter painter(&printer);
      painter.setRenderHint(QPainter::Antialiasing);
      for (int pg=firstPage; pg <= lastPage; ++pg)
      {
        QGraphicsScene* scene = report->getPage(pg);
        scene->render(&painter);
        if (pg != lastPage)
        {
          printer.newPage();
        }
      }
    }
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onBtnPageNextClicked()
{
  showNextPage();
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onBtnPagePrevClicked()
{
  showPrevPage();
}

//---------------------------------------------------------------------------

bool SimpleReportViewer::showPage(int pgNum)
{
  if (report == nullptr) return false;

  if (pgNum < 0) return false;
  if (pgNum >= report->getPageCount()) return false;

  ui->gv->setScene(report->getPage(pgNum));
  ui->gv->recalcZoom();
  curPage = pgNum;
  updateButtons();
  return true;
}

//---------------------------------------------------------------------------

void SimpleReportViewer::updateButtons()
{
  bool isEnabled = (report != nullptr);
  ui->btnPageNext->setEnabled(isEnabled);
  ui->btnPagePrev->setEnabled(isEnabled);
  ui->btnPrint->setEnabled(isEnabled);
  ui->btnZoomLess->setEnabled(isEnabled);
  ui->btnZoomMore->setEnabled(isEnabled);
  ui->sbPage->setEnabled(isEnabled);
  ui->zoomSlider->setEnabled(isEnabled);
  ui->gv->setEnabled(isEnabled);

  if (!isEnabled) return;

  ui->btnPageNext->setEnabled(curPage < (report->getPageCount() - 1));
  ui->btnPagePrev->setEnabled(curPage > 0);
  ui->sbPage->setValue(curPage + 1);
}

//---------------------------------------------------------------------------

bool SimpleReportViewer::showNextPage()
{
  return showPage(curPage + 1);
}

//---------------------------------------------------------------------------

bool SimpleReportViewer::showPrevPage()
{
  return showPage(curPage - 1);
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onZoomSliderChanged()
{
  // only modify the graphics scene if we have an actual
  // change of the zoom value
  //
  // this also breaks event loops
  int newZoomFactor = ui->zoomSlider->value();
  if (newZoomFactor == ui->gv->getZoomFactor()) return;

  ui->gv->setZoomFactor(newZoomFactor);
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onBtnZoomLessClicked()
{
  int step = ui->zoomSlider->pageStep();
  ui->gv->setZoomFactor_Delta(-step);

  // the slider position will be updated via the viewZoomFactorChanged event
  // issued by the ReportGraphicsView
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onBtnZoomMoreClicked()
{
  int step = ui->zoomSlider->pageStep();
  ui->gv->setZoomFactor_Delta(step);

  // the slider position will be updated via the viewZoomFactorChanged event
  // issued by the ReportGraphicsView
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onSpinBoxPageChanged()
{
  showPage(ui->sbPage->value() - 1);
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onGraphicsViewZoomFactorChanged(int newZoomFactor)
{
  ui->zoomSlider->setValue(newZoomFactor);
}

//---------------------------------------------------------------------------

void SimpleReportViewer::wheelEvent(QWheelEvent* ev)
{
  // ignore / propagate all wheel events without Ctrl-key
  if (ev->modifiers() != Qt::ControlModifier)
  {
    ev->ignore();
    return;
  }

  ev->accept();

  // ignore wheel events without actual rotation
  auto degrees = ev->angleDelta() / 8;
  if (degrees.isNull()) return;

  // increase or decrease the zoom level
  int step = 10;

  if ((degrees.x() <= 0) && (degrees.y() <= 0))
  {
    step *= -1;
  }

  ui->gv->setZoomFactor_Delta(step);
}

//---------------------------------------------------------------------------


}
