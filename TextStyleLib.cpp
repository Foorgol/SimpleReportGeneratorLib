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
