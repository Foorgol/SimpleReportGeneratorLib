/* 
 * File:   TabSet.cpp
 * Author: nyoknvk1
 *
 * Created on 10. September 2014, 16:33
 */

#include <stdexcept>

#include "TabSet.h"

namespace SimpleReportLib {

  TabSet::TabSet() {
  }

  //---------------------------------------------------------------------------

  TabSet::~TabSet()
  {
  }

  //---------------------------------------------------------------------------

  void TabSet::addTab(double pos, TAB_JUSTIFICATION just)
  {
    // do not allow negative tabs
    if (pos < 0) return;
    
    // remove a potentially already existing tab
    if (hasTab(pos)) removeTab(pos);
    
    TabDef newTab(pos, just);
    
    /*
     * find the right position for inserting the tab
     */
    
    // case 1: list is empty
    if (tabList.count() == 0)
    {
      tabList.append(newTab);
    }
    
    // case 2: new tab is after the last "old" tab
    else if (pos > tabList.last().pos)
    {
      tabList.append(newTab);
    }
    
    // case 3: new tab is either the (new) first tab or somewhere before the (old) last tab
    else
    {
      for (int i=0; i < tabList.count(); i++)
      {
        if (pos < tabList.at(i).pos)
        {
          tabList.insert(i, newTab);
          break;
        }
      }
    }
  }

  //---------------------------------------------------------------------------

  void TabSet::clearAllTabs()
  {
    tabList.clear();
  }

  //---------------------------------------------------------------------------

  TabDef TabSet::getTabAt(int i)
  {
    // no range checking here
    // if the caller is too dump to avoid wrong indices, the caller deserves
    // a crash
    return tabList.at(i);
  }

  //---------------------------------------------------------------------------

  int TabSet::getTabCount()
  {
    return tabList.count();
  }

  //---------------------------------------------------------------------------

  bool TabSet::hasTab(double pos)
  {
    for (int i=0; i<tabList.count(); i++)
    {
      if (tabList.at(i).pos == pos) return true;
    }
    
    return false;
  }

  //---------------------------------------------------------------------------

  void TabSet::removeTab(double pos)
  {
    for (int i=0; i<tabList.count(); i++)
    {
      if (tabList.at(i).pos == pos) tabList.removeAt(i);
      return;
    }
  }

  //---------------------------------------------------------------------------

  TabDef::TabDef(double _pos, TAB_JUSTIFICATION _just)
  {
    if (pos < 0)
    {
      throw std::invalid_argument("Tab position can't be negative");
    }
    
    pos = _pos;
    just = _just;
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
