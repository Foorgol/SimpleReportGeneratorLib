#include "LineChart.h"

namespace SimpleReportLib {

  LineChart::LineChart(SimpleReportGenerator* _rep, double _x0, double _y0, double _w, double _h)
    :rep(_rep), x0(_x0), y0(_y0), w(_w), h(_h)
  {
    if ((rep == nullptr) || (x0 < 0) || (y0 < 0) || (w < 1) || (h < 1))
    {
      throw invalid_argument("Invalid parameters for LineChart ctor!");
    }
  }

  //----------------------------------------------------------------------------

  void LineChart::addTrace(vector<tuple<double, double> >& data)
  {
    if (data.size() == 0) return;
    traces.push_back(data);
  }

  //----------------------------------------------------------------------------

  void LineChart::render()
  {
    if (traces.size() == 0) return;

    // get the extends of the X- and Y-data
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    tie(xmin, xmax, ymin, ymax) = get_XMin_XMax_YMin_YMax();

    render(xmin, xmax, ymin, ymax);
  }

  //----------------------------------------------------------------------------

  void LineChart::render(double xmin, double xmax, double ymin, double ymax)
  {
    if (traces.size() == 0) return;

    // calculate the mapping from "values" to "millimeter" or "coordinates"
    double facX = w / (xmax - xmin);
    double facY = h / (ymax - ymin);

    // a helper function to convert from "value" to "paper" coordinates
    auto coordConversion = [&](const double x, const double y) {
      double xOut = (x - xmin) * facX + x0;
      double yOut = (y0 + h) - (y - ymin) * facY;
      return make_tuple(xOut, yOut);
    };

    // draw the axes
    rep->drawVertLine(x0, y0, h, LINE_TYPE::THICK);
    rep->drawHorLine(x0, y0 + h, w, LINE_TYPE::THICK);

    // draw trace by trace
    for (auto trace : traces)
    {
      double prevX;
      double prevY;
      bool isFirst = true;
      for (auto datapoint : trace)
      {
        double x;
        double y;
        tie(x, y) = datapoint;
        tie(x, y) = coordConversion(x, y);
        if (isFirst)
        {
          prevX = x;
          prevY = y;
          isFirst = false;
        }

        rep->drawLine(prevX, prevY, x, y);
        prevX = x;
        prevY = y;
      }
    }

    // draw axis labels
    for (auto xLabel : xLabels)
    {
      double x;
      QString txt;
      tie(x, txt) = xLabel;
      tie(x, std::ignore) = coordConversion(x, 0);

      // draw the tick
      QPointF tickPos(x, y0 + h);
      rep->drawVertLine(tickPos, TICK_LENGTH__MM);
      tickPos.setY(tickPos.y() + TICK_LENGTH__MM + 0.5);
      rep->drawText(tickPos, RECT_CORNER::TOP_CENTER, txt);
    }
    for (auto yLabel : yLabels)
    {
      double y;
      QString txt;
      tie(y, txt) = yLabel;
      tie(std::ignore, y) = coordConversion(0, y);

      // draw the tick
      QPointF tickPos(x0, y);
      rep->drawHorLine(tickPos, -TICK_LENGTH__MM);
      tickPos.setX(tickPos.x() - TICK_LENGTH__MM - 0.5);
      rep->drawText(tickPos, RECT_CORNER::MID_RIGHT, txt);
    }
  }

  //----------------------------------------------------------------------------

  tuple<double, double, double, double> LineChart::get_XMin_XMax_YMin_YMax() const
  {
    //
    // THIS WILL FAIL IF THERE ISN'T AT LEAST ONE TRACE AVAILABLE!!
    //
    double xmin;
    double xmax;
    double ymin;
    double ymax;

    tie(xmin, ymin) = traces.at(0).at(0);
    tie(xmax, ymax) = traces.at(0).at(0);

    for (auto tr : traces)
    {
      for (auto xy : tr)
      {
        double x;
        double y;
        tie(x,y) = xy;

        if (x < xmin) xmin = x;
        if (y < ymin) ymin = y;
        if (x > xmax) xmax = x;
        if (y > ymax) ymax = y;
      }
    }

    return make_tuple(xmin, xmax, ymin, ymax);
  }

  //----------------------------------------------------------------------------

  void LineChart::addLabel_X(double x, const QString& txt)
  {
    xLabels.push_back(make_tuple(x, txt));
  }

  //----------------------------------------------------------------------------

  void LineChart::addLabel_Y(double y, const QString& txt)
  {
    yLabels.push_back(make_tuple(y, txt));
  }

  //----------------------------------------------------------------------------


  //----------------------------------------------------------------------------


  //----------------------------------------------------------------------------

}
