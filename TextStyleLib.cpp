#include <exception>

#include "TextStyleLib.h"

namespace SimpleReportLib {

TextStyleLib::TextStyleLib()
  :name2style(QHash<QString, std::shared_ptr<TextStyle>>()),
    root(std::shared_ptr<TextStyle>(new TextStyle))
{
}

//---------------------------------------------------------------------------

std::shared_ptr<TextStyle> TextStyleLib::getStyle(const QString& styleName) const
{
  // root has no name
  if (styleName.isEmpty()) return root;

  // return style if it exists
  if (name2style.keys().contains(styleName)) return name2style.value(styleName);

  // style not found
  return nullptr;
}

//---------------------------------------------------------------------------

std::shared_ptr<TextStyle> TextStyleLib::createChildStyle(const QString& childName, const QString& parentName)
{
  // style names must be unique
  if (name2style.keys().contains(childName)) return nullptr;

  // determine parent. use "root" if parentName is empty
  std::shared_ptr<TextStyle> parentStyle;
  if (!(parentName.isEmpty()))
  {
    parentStyle = getStyle(parentName);
    if (parentStyle == nullptr) return nullptr;  // parent doesn't exist
  }
  if (parentStyle == nullptr) parentStyle = root;

  auto newStyle = std::shared_ptr<TextStyle>(new TextStyle(parentStyle));
  name2style.insert(childName, newStyle);

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
