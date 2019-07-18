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

#ifndef TABLEWRITER_H
#define	TABLEWRITER_H

#include <QList>
#include <QPainter>
#include "TabSet.h"

//#include "simplereportgenerator_global.h"
#include "SimpleReportGenerator.h"

namespace SimpleReportLib {

  class TableWriter {
  public:
    TableWriter(TabSet& _tabs);
    virtual ~TableWriter();
    
    bool setCell(const int row, const int col, const QString& txt);
    bool setRow(const int row, const QStringList& lst);
    bool appendRow(const QStringList& lst);
    void setHeader(const QStringList& lst);
    bool setHeader(const int col, const QString& txt);
    void setNextPageContinuationCaption(const QString& cap);
    
    void write(SimpleReportGenerator* r);

  private:
    TabSet tabs;
    QList<QStringList> data;
    QStringList hdr;
    QString contCaption;

    QString cleanupCellText(QString inText);
    QString genRowFromStringList(const QStringList& lst);

    // some layout parameters with default values;
    // can be overridden by the user later
    double extraRowSkip = 1.0;
    double leftIndentation = 2.0;

    void writeHeader(SimpleReportGenerator* r);
  };

}
#endif	/* TABLEWRITER_H */

