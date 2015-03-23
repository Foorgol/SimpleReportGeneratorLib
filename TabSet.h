/* 
 * File:   TabSet.h
 * Author: nyoknvk1
 *
 * Created on 10. September 2014, 16:33
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

