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

#include <exception>
#include <memory>

#include "TextStyleLib.h"

namespace SimpleReportLib {

TextStyleLib::TextStyleLib()
  :name2style(std::map<QString, upTextStyle>()),
    root(upTextStyle(new TextStyle()))
{
}

//---------------------------------------------------------------------------

TextStyle* TextStyleLib::getStyle(const QString& styleName) const
{
  // root has no name
  if (styleName.isEmpty()) return root.get();

  // return style if it exists
  auto it = name2style.find(styleName);
  if (it != name2style.end())
  {
    return name2style.at(styleName).get();
  }

  // style not found
  return nullptr;
}

//---------------------------------------------------------------------------

TextStyle* TextStyleLib::createChildStyle(const QString& childName, const QString& parentName)
{
  // style names must be unique
  if (name2style.count(childName) > 0) return nullptr;

  // determine parent. use "root" if parentName is empty
  TextStyle* parentStyle = root.get();
  if (!(parentName.isEmpty()))
  {
    parentStyle = getStyle(parentName);
    if (parentStyle == nullptr) return nullptr;  // parent doesn't exist
  }

  // create a raw pointer to a new (child) style
  TextStyle* newStyle = new TextStyle(parentStyle);

  // transfer ownership to the hash map
  name2style[childName] = upTextStyle(newStyle);

  // return raw handler to the caller
  return newStyle;
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


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

}
