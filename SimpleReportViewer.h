#ifndef SIMPLEREPORTVIEWER_H
#define SIMPLEREPORTVIEWER_H

#include <QWidget>

#include "simplereportgenerator_global.h"
#include "SimpleReportGenerator.h"

namespace Ui {
  class SimpleReportViewer;
}

namespace SimpleReportLib {

using namespace SimpleReportLib;

class SIMPLEREPORTGENERATORSHARED_EXPORT SimpleReportViewer : public QWidget
{
  Q_OBJECT

public:
  explicit SimpleReportViewer(QWidget *parent = 0);
  ~SimpleReportViewer();

  bool showNextPage();
  bool showPrevPage();
  bool showPage(int pgNum);
  bool setReport(SimpleReportGenerator* r);

public slots:
  void onBtnPrintClicked();
  void onBtnPagePrevClicked();
  void onBtnPageNextClicked();
  void onZoomSliderChanged();
  void onBtnZoomMoreClicked();
  void onBtnZoomLessClicked();
  void onSpinBoxPageChanged();
  virtual void wheelEvent(QWheelEvent* ev) override;

protected:
  void paintEvent(QPaintEvent* event);

private:
  Ui::SimpleReportViewer *ui;
  SimpleReportGenerator* report;
  int curPage = -1;
  void updateButtons();
};

}
#endif // SIMPLEREPORTVIEWER_H
