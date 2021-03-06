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

#ifndef SIMPLEREPORTVIEWER_H
#define SIMPLEREPORTVIEWER_H

#include <QWidget>

//#include "simplereportgenerator_global.h"
#include "SimpleReportGenerator.h"

namespace Ui {
  class SimpleReportViewer;
}

namespace SimpleReportLib {

using namespace SimpleReportLib;

class SimpleReportViewer : public QWidget
{
  Q_OBJECT

public:
  explicit SimpleReportViewer(QWidget *parent = 0);
  ~SimpleReportViewer();

  bool showNextPage();
  bool showPrevPage();
  bool showPage(int pgNum);
  bool setReport(SimpleReportGenerator* r);
  void refreshDisplayedContent();

public slots:
  void onBtnPrintClicked();
  void onBtnPagePrevClicked();
  void onBtnPageNextClicked();
  void onZoomSliderChanged();
  void onBtnZoomMoreClicked();
  void onBtnZoomLessClicked();
  void onSpinBoxPageChanged();
  void onGraphicsViewZoomFactorChanged(int newZoomFactor);
  virtual void wheelEvent(QWheelEvent* ev) override;

protected:
  //void paintEvent(QPaintEvent* event);

private:
  Ui::SimpleReportViewer *ui;
  SimpleReportGenerator* report;
  int curPage = -1;
  void updateButtons();
};

}
#endif // SIMPLEREPORTVIEWER_H
