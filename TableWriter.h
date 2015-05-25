/* 
 * File:   TableWriter.h
 * Author: nyoknvk1
 *
 * Created on 25. August 2014, 08:51
 */

#ifndef TABLEWRITER_H
#define	TABLEWRITER_H

#include <QList>
#include <QPainter>
#include "TabSet.h"

#include "simplereportgenerator_global.h"
#include "SimpleReportGenerator.h"

namespace SimpleReportLib {

  class SIMPLEREPORTGENERATORSHARED_EXPORT TableWriter {
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

