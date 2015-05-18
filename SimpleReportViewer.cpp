#include <stdexcept>

#include <QPrinter>
#include <QPrintDialog>

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

void SimpleReportViewer::paintEvent(QPaintEvent* event)
{
    if (report == nullptr) return;

    // maximaze the scene on the screen
    QRectF sceneExtends = ui->gv->sceneRect();
    double scaleX = ui->gv->width() / sceneExtends.width();
    double scaleY = ui->gv->height() / sceneExtends.height();
    double scale = std::min(scaleX, scaleY);

    // get an additional zoom factor between 1.0 and 5.0
    // from the slider
    double viewScale = ui->zoomSlider->value() / 10.0;
    ui->gv->resetTransform();
    ui->gv->scale(scale * viewScale, scale * viewScale);
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onBtnPrintClicked()
{
    QPrinter printer;

    // initialize the page ranges for the print dialog
    int numPages = report->getPageCount();
    printer.setFromTo(1, numPages);
    printer.setPrintRange(QPrinter::AllPages);

    if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
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
  this->repaint();
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onBtnZoomLessClicked()
{
  int curVal = ui->zoomSlider->value();

  ui->zoomSlider->setValue( curVal - ui->zoomSlider->pageStep());

  this->repaint();
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onBtnZoomMoreClicked()
{
  int curVal = ui->zoomSlider->value();

  ui->zoomSlider->setValue( curVal + ui->zoomSlider->pageStep());

  this->repaint();
}

//---------------------------------------------------------------------------

void SimpleReportViewer::onSpinBoxPageChanged()
{
  showPage(ui->sbPage->value() - 1);
}

//---------------------------------------------------------------------------


}
