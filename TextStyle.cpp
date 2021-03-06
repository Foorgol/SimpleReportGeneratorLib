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

#include "TextStyle.h"

#include "SimpleReportGenerator.h"

namespace SimpleReportLib {

TextStyle::TextStyle()
  :fontName("Arial"), fontSize_MM(DEFAULT_FONT_SIZE__MM),
    boldState(0), italicsState(0), fontColor(new QColor(0,0,0)), parent(nullptr)
{
}

//---------------------------------------------------------------------------

QString TextStyle::getFontName() const
{
  if (!(fontName.isEmpty())) return fontName;

  if (parent != nullptr) return parent->getFontName();

  return "Arial";
}

//---------------------------------------------------------------------------

double TextStyle::getFontSize_MM() const
{
  if (fontSize_MM > 0) return fontSize_MM;

  if (parent != nullptr) return parent->getFontSize_MM();

  return DEFAULT_FONT_SIZE__MM;
}

//---------------------------------------------------------------------------

bool TextStyle::isBold() const
{
  if (boldState >= 0) return (boldState == 1);

  if (parent != nullptr) return parent->isBold();

  return false;
}

//---------------------------------------------------------------------------

bool TextStyle::isItalics() const
{
  if (italicsState >= 0) return (italicsState == 1);

  if (parent != nullptr) return parent->isItalics();

  return false;
}

//---------------------------------------------------------------------------

QColor TextStyle::getFontColor() const
{
  if (fontColor != nullptr) return *fontColor;

  if (parent != nullptr) return parent->getFontColor();

  return QColor(0,0,0);
}

//---------------------------------------------------------------------------

void TextStyle::setFontname(QString newFontName)
{
  fontName = newFontName;
}

//---------------------------------------------------------------------------

void TextStyle::setFontSize_MM(double newFontSize_MM)
{
  if (newFontSize_MM < 0) fontSize_MM = -1.0;
  else fontSize_MM = newFontSize_MM;
}

//---------------------------------------------------------------------------

void TextStyle::setBoldState(int newBoldState)
{
  if (newBoldState < 0) boldState = -1;
  else boldState = newBoldState;
}

void TextStyle::setBoldState(bool _isBold)
{
  boldState = _isBold ? 1 : 0;
}

//---------------------------------------------------------------------------

void TextStyle::setItalicsState(int newItalicsState)
{
  if (newItalicsState < 0) italicsState = -1;
  else italicsState = newItalicsState;
}

void TextStyle::setItalicsState(bool _isItalics)
{
  italicsState = _isItalics ? 1 : 0;
}

//---------------------------------------------------------------------------

void TextStyle::setFontColor(const QColor &newCol)
{
  fontColor = std::unique_ptr<QColor>(new QColor(newCol));
}

//---------------------------------------------------------------------------

void TextStyle::setFontColor()
{
  fontColor = nullptr;
}

//---------------------------------------------------------------------------

std::unique_ptr<QFont> TextStyle::getFont() const
{
  QFont* result = new QFont(getFontName());
  result->setPointSizeF(getFontSize_MM() * ACCURACY_FAC);
  result->setBold(isBold());
  result->setItalic(isItalics());

  return std::unique_ptr<QFont>(result);
}

//---------------------------------------------------------------------------

std::unique_ptr<QPen> TextStyle::getPen() const
{
  return std::unique_ptr<QPen>(new QPen(getFontColor()));
}

//---------------------------------------------------------------------------

TextStyle::TextStyle(TextStyle* _parent)
  :fontName(), fontSize_MM(-1.0),
    boldState(-1), italicsState(-1), fontColor(nullptr), parent(_parent)
{
}

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

}
