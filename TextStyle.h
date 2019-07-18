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

#ifndef TEXTSTYLE_H
#define TEXTSTYLE_H

#include <memory>
#include <QString>
#include <QColor>
#include <QFont>

//#include "simplereportgenerator_global.h"

namespace SimpleReportLib {

class TextStyle
{
  friend class TextStyleLib;

public:
  static constexpr double DEFAULT_FONT_SIZE__MM = 2.0;
  static constexpr double ACCURACY_FAC = 50.0;

  // getters
  QString getFontName() const;
  double getFontSize_MM() const;
  bool isBold() const;
  bool isItalics() const;
  QColor getFontColor() const;
  std::unique_ptr<QFont> getFont() const;
  std::unique_ptr<QPen> getPen() const;

  // setters
  void setFontname(QString newFontName);
  void setFontSize_MM(double newFontSize_MM);
  void setBoldState(int newBoldState);
  void setBoldState(bool _isBold);
  void setItalicsState(int newItalicsState);
  void setItalicsState(bool _isItalics);
  void setFontColor(const QColor& newCol);
  void setFontColor();

private:
  QString fontName;  // "" = front parent
  double fontSize_MM;      // -1 = from parent
  int boldState;     // 0 = not bold; 1 = bold; -1 = from parent
  int italicsState;     // 0 = not italics; 1 = italics; -1 = from parent
  std::unique_ptr<QColor> fontColor;   // nullptr = from parent
  TextStyle* parent;   // nullptr = root element

  // private constructor(s) to force using the
  // createXXX-functions of the text style lib
  TextStyle();
  TextStyle(TextStyle* _parent);
};

typedef std::unique_ptr<TextStyle> upTextStyle;

}
#endif // TEXTSTYLE_H
