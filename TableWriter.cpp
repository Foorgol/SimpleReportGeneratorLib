/* 
 * File:   TableWriter.cpp
 * Author: nyoknvk1
 *
 * Created on 25. August 2014, 08:51
 */

#include "TableWriter.h"
#include "TextStyle.h"
#include "TextStyleLib.h"

#include <stdexcept>

namespace SimpleReportLib {

  TableWriter::TableWriter(TabSet& _tabs)
    : tabs(_tabs)
  {
    if (tabs.getTabCount() == 0)
    {
      throw std::invalid_argument("Need at least one tab for the table");
    }

    // set empty initial headers
    int i=0;
    while (i <= tabs.getTabCount())  // "<=" because we assume an implicit tab/column at pos 0
    {
      hdr.append("");
      ++i;
    }
  }

  TableWriter::~TableWriter() {
  }

  void TableWriter::setHeader(const QStringList& lst)
  {
    int i=0;
    while ((i < lst.count()) && (i <= tabs.getTabCount())) // "<=" because we assume an implicit tab/column at pos 0
    {
      hdr.replace(i, cleanupCellText(lst.at(i)));
      ++i;
    }
  }

  bool TableWriter::setHeader(const int col, const QString &txt)
  {
    if ((col < 0) || (col > tabs.getTabCount())) return false; // ">" because we assume an implicit tab/column at pos 0
    hdr.replace(col, cleanupCellText(txt));
    return true;
  }

  bool TableWriter::setCell(const int row, const int col, const QString &txt)
  {
    if ((col < 0) || (col > tabs.getTabCount())) return false;  // ">" because we assume an implicit tab/column at pos 0
    if (row < 0) return false;

    while (data.count() <= row)
    {
      data.append(QStringList());
    }

    QStringList rowData = data.at(row);
    while (rowData.count() <= col)
    {
      rowData.append("");
    }
    rowData.replace(col, cleanupCellText(txt));
    data.replace(row, rowData);

    return true;
  }

  bool TableWriter::setRow(const int row, const QStringList &lst)
  {
    if (row < 0) return false;

    int i=0;
    while ((i < lst.count()) && (i <= tabs.getTabCount())) // "<=" because we assume an implicit tab/column at pos 0
    {
      setCell(row, i, cleanupCellText(lst.at(i)));
      ++i;
    }

    return true;
  }

  QString TableWriter::cleanupCellText(QString inText)
  {
    QString result = inText;

    // cell content shall not contain tabs
    result.replace("\t", " ");

    return result;
  }

  QString TableWriter::genRowFromStringList(const QStringList &lst)
  {
    QString result = "";
    for (QString s : lst)
    {
      result += s + "\t";
    }
    result.remove(result.length()-1, 1); // remove last tab

    return result;
  }

  void TableWriter::write(SimpleReportGenerator *r)
  {
    if (r == nullptr) return;

    // create a special style for the table header, if necessary
    auto headerStyle = r->getTextStyle("TableHeader");
    if (headerStyle == nullptr)
    {
      headerStyle = r->createChildTextStyle("TableHeader");
      headerStyle->setBoldState(true);
    }

    // prepare tabs and add a dummy tab / offset for the left margin
    TabSet finalTabs;
    finalTabs.addTab(leftIndentation, TAB_LEFT);  // here we make the implicitly assumed tab explicit
    int i = 0;
    while (i < tabs.getTabCount())
    {
      TabDef td = tabs.getTabAt(i);
      finalTabs.addTab(td.pos + leftIndentation, td.just);
      ++i;
    }

    // set the tabs
    r->pushTabs(finalTabs);

    // header line
    writeHeader(r);

    // content
    for (QStringList rowData : data)
    {
      // make sure we can fit another line of text on the page.
      // If not, start a new page and repeat the headers
      if (!(r->hasSpaceForAnotherLine(QString())))
      {
        // closing footer line
        r->addHorLine();

        // start a new page and write the continuatin caption, if set
        r->startNextPage();
        if (!(contCaption.isEmpty()))
        {
          TabSet centeredTab;
          centeredTab.addTab(r->getPageWidth() / 2.0, TAB_CENTER);
          r->pushTabs(centeredTab);
          r->writeLine("\t" + contCaption);
          r->popTabs();
        }
        writeHeader(r);
      }

      r->writeLine("\t" + genRowFromStringList(rowData), QString(), extraRowSkip);
    }

    // footer line
    r->addHorLine();

    // restore tabs
    r->popTabs();
  }

  bool TableWriter::appendRow(const QStringList &lst)
  {
    return setRow(data.count(), lst);
  }

  void TableWriter::writeHeader(SimpleReportGenerator *r)
  {
    // assumes to be called from within write() and assumes that all tab positions
    // have been properly set up


    r->addHorLine();
    r->writeLine("\t" + genRowFromStringList(hdr), "TableHeader", extraRowSkip / 2.0, extraRowSkip / 2.0);
    r->addHorLine(THIN);
    r->skip(extraRowSkip);
  }

  void TableWriter::setNextPageContinuationCaption(const QString &cap)
  {
    contCaption = cap;
  }

}
