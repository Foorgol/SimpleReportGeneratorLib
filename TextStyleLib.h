#ifndef TEXTSTYLELIB_H
#define TEXTSTYLELIB_H

#include <memory>
#include <map>

#include <QHash>
#include <QString>

#include "TextStyle.h"

namespace SimpleReportLib {

class SIMPLEREPORTGENERATORSHARED_EXPORT TextStyleLib
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
