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

#ifndef TEXTSTYLELIB_H
#define TEXTSTYLELIB_H

#include <memory>
#include <map>

#include <QHash>
#include <QString>

#include "TextStyle.h"

namespace SimpleReportLib {

class TextStyleLib
{
public:
  TextStyleLib();

  TextStyle* getStyle(const QString &styleName=QString()) const;
  TextStyle* createChildStyle(const QString &childName, const QString &parentName=QString());

private:
  std::map<QString, upTextStyle> name2style;
  upTextStyle root;
};

}
#endif // TEXTSTYLELIB_H
