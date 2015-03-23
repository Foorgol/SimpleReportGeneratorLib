#ifndef TEXTSTYLELIB_H
#define TEXTSTYLELIB_H

#include <QHash>
#include <QString>

#include "TextStyle.h"

namespace SimpleReportLib {

class TextStyleLib
{
public:
  TextStyleLib();

  std::shared_ptr<TextStyle> getStyle(const QString &styleName=QString()) const;
  std::shared_ptr<TextStyle> createChildStyle(const QString &childName, const QString &parentName=QString());

private:
  QHash<QString, std::shared_ptr<TextStyle>> name2style;
  std::shared_ptr<TextStyle> root;
};

}
#endif // TEXTSTYLELIB_H
