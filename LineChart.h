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

#ifndef LINECHART_H
#define LINECHART_H

#include <vector>
#include <tuple>

//#include "simplereportgenerator_global.h"
#include "SimpleReportGenerator.h"

using namespace std;

namespace SimpleReportLib {

  class LineChart
  {
  public:
    static constexpr double AXIS_WIDTH__MM = 1.5;
    static constexpr double TICK_LENGTH__MM = 1.5;

    LineChart(SimpleReportGenerator* _rep, double _x0, double _y0, double _w, double _h);
    void addTrace(vector<tuple<double, double>>& data);
    void render();
    void render(double xmin, double xmax, double ymin, double ymax);
    tuple<double, double, double, double> get_XMin_XMax_YMin_YMax() const;
    void addLabel_X(double x, const QString& txt);
    void addLabel_Y(double y, const QString& txt);

  protected:
    SimpleReportGenerator* rep;
    double x0;
    double y0;
    double w;
    double h;
    vector<vector<tuple<double, double>>> traces;
    vector<tuple<double, QString>> xLabels;
    vector<tuple<double, QString>> yLabels;
  };
}

#endif // LINECHART_H
