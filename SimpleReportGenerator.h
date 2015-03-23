#ifndef SIMPLEREPORTGENERATOR_H
#define SIMPLEREPORTGENERATOR_H

#include <QPen>
#include <QFont>
#include <QHash>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsScene>
#include <QStack>

#include "simplereportgenerator_global.h"
#include "TabSet.h"

#include "TextStyle.h"
#include "TextStyleLib.h"

namespace SimpleReportLib {
#define MAX_NUM_PAGES 100
#define THIN_LINE_WIDTH__MM 0.1
#define MEDIUM_LINE_WIDTH__MM 0.35
#define THICK_LINE_WIDTH__MM 0.7
#define H1_FONT_SIZE__MM 5.0
#define H2_FONT_SIZE__MM 3.5
#define PARA_FONT_SIZE__MM 2.0
#define SYS_FONT "Arial"
#define DEFAULT_PARSKIP__MM 1.0
#define DEFAULT_LINESKIP_FAC 1.1
#define HEADER_FOOTER_SKIP__MM 2.0

//#define ACCURACY_FAC 50.0

  static constexpr double ACCURACY_FAC = 50.0;

  enum HOR_TXT_ALIGNMENT {
    LEFT,
    CENTER,
    RIGHT
  };

  enum LINE_TYPE {
    THIN,
    MED,
    THICK
  };

  class SIMPLEREPORTGENERATORSHARED_EXPORT SimpleReportGenerator
  {

  public:
    SimpleReportGenerator(double _w, double _h, double _margin);
    SimpleReportGenerator(const SimpleReportGenerator& orig);
    virtual ~SimpleReportGenerator();
    void deleteAllPages();

    int startNextPage();
    int getPageCount();
    QGraphicsScene* getPage(int pageNum);

    void writeLine(QString txt, const QString& styleName=QString(), double skipAfter = 0.0, double skipBefore = 0.0);
    void writeLine(QString txt, const std::shared_ptr<TextStyle> style, double skipAfter = 0.0, double skipBefore = 0.0);
    void skip(double skipAmount);

    void setHeader(QString left, QString mid, QString right);
    void setFooter(QString left, QString mid, QString right);
    void addTab(double pos, TAB_JUSTIFICATION just);
    void removeTab(double pos);
    void clearAllTabs();
    void pushTabs(TabSet& newTabs);
    void popTabs();

    void addHorLine(LINE_TYPE lt=MED, double skipAfter=0.0, double skipBefore=0.0);

    bool hasSpaceForAnotherLine(const QString& styleName=QString(), double skipBefore = 0.0);
    bool hasSpaceForAnotherLine(std::shared_ptr<TextStyle> style = nullptr, double skipBefore = 0.0);

    double getPageWidth();

    std::shared_ptr<TextStyle> getTextStyle(const QString& styleName=QString()) const;
    std::shared_ptr<TextStyle> createChildTextStyle(const QString &childName, const QString &parentName=QString());


  private:
    double setTextPosAligned(double x, double y, QGraphicsSimpleTextItem* txt, HOR_TXT_ALIGNMENT align=LEFT);
    void insertHeaderAndFooterOnCurrentPage();
    double w;
    double h;
    double margin;
    int pageCount;
    int curPage;
    double curY;
    QGraphicsScene* page[MAX_NUM_PAGES];

    QPen thinPen;
    QPen mediumPen;
    QPen thickPen;

    QString headerLeft;
    QString headerCenter;
    QString headerRight;

    QString footerLeft;
    QString footerCenter;
    QString footerRight;

    double maxY;

    TabSet tabSet;
    QStack<TabSet> tabStack;

    TextStyleLib styleLib;

  };

}

#endif // SIMPLEREPORTGENERATOR_H
