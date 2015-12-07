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

#ifndef TABSET_H
#define	TABSET_H

#include <QList>

#include "simplereportgenerator_global.h"

namespace SimpleReportLib {

  enum SIMPLEREPORTGENERATORSHARED_EXPORT TAB_JUSTIFICATION {
    TAB_LEFT,
    TAB_CENTER,
    TAB_RIGHT
  };

  class SIMPLEREPORTGENERATORSHARED_EXPORT TabDef {
  public:
    TabDef(double _pos, TAB_JUSTIFICATION _just);
    double pos;
    TAB_JUSTIFICATION just;
  };

  class SIMPLEREPORTGENERATORSHARED_EXPORT TabSet {
    
  public:
    TabSet();
    ~TabSet();
    void addTab(double pos, TAB_JUSTIFICATION just);
    void removeTab(double pos);
    void clearAllTabs();
    bool hasTab(double pos);
    int getTabCount();
    TabDef getTabAt(int i);
    
  private:
    QList<TabDef> tabList;
  };

}

#endif	/* TABSET_H */

