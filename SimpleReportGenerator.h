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

using namespace std;

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
#define HEADER_FOOTER_SKIP__MM 3.0

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

  enum class RECT_CORNER {
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    MID_LEFT,
    CENTER,
    MID_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
  };

  class SIMPLEREPORTGENERATORSHARED_EXPORT HeaderFooterStrings
  {
  public:
    static constexpr char TOKEN_CURPGNUM[] = "$#$";
    static constexpr char TOKEN_TOTALPGNUM[] = "$##$";
    static constexpr char TOKEN_CURDATE[] = "$__DATE__$";
    static constexpr char TOKEN_CURTIME[] = "$__TIME__$";
    void substTokens(int curPageNum, int totalPageCount);
    static void substTokensInPlace(QString& s, int idxCurPage, int totalPageCount);
    static void substTokensInPlace(QString& s, const QHash<QString, QString>& substTab);

    HeaderFooterStrings();
    virtual ~HeaderFooterStrings();
    //void setHeader(QString l, QString c, QString r);
    //void setFooter(QString l, QString c, QString r);
    QString hl;
    QString hc;
    QString hr;
    QString fl;
    QString fc;
    QString fr;
  };

  class SIMPLEREPORTGENERATORSHARED_EXPORT SimpleReportGenerator
  {

  public:
    static constexpr char DEFAULT_HEADER_STYLE_NAME[] = "Header";

    SimpleReportGenerator(double _w, double _h, double _margin);
    //SimpleReportGenerator(const SimpleReportGenerator& orig);
    virtual ~SimpleReportGenerator();
    void deleteAllPages();

    int startNextPage();
    int getPageCount();
    bool setActivePage(int idxPage);
    int getCurrentPageNumber() const;
    QGraphicsScene* getPage(int pageNum);

    void writeLine(QString txt, const QString& styleName=QString(), double skipAfter = 0.0, double skipBefore = 0.0);
    void writeLine(QString txt, TextStyle* style, double skipAfter = 0.0, double skipBefore = 0.0);
    void skip(double skipAmount);
    void warpTo(double newAbsYPos);
    QPointF getAbsCursorPos() const;

    void setHeader(QString left, QString mid, QString right, int page=-1);
    void setFooter(QString left, QString mid, QString right, int page=-1);
    void setGlobalHeader(QString left, QString mid, QString right);
    void setGlobalFooter(QString left, QString mid, QString right);
    void addTab(double pos, TAB_JUSTIFICATION just);
    void removeTab(double pos);
    void clearAllTabs();
    void pushTabs(TabSet& newTabs);
    void popTabs();

    void addHorLine(LINE_TYPE lt=MED, double skipAfter=0.0, double skipBefore=0.0);
    void addHorLine_absPos(double yPos, LINE_TYPE lt=MED) const;

    bool hasSpaceForAnotherLine(const QString& styleName=QString(), double skipBefore = 0.0);
    bool hasSpaceForAnotherLine(TextStyle* style = nullptr, double skipBefore = 0.0);

    void insertHeaderAndFooter(int pageNum);
    void applyHeaderAndFooterOnAllPages();

    double getPageWidth();
    double getPageHeight();

    double getUsablePageWidth() const;
    double getUsablePageHeight() const;

    TextStyle* getTextStyle(const QString& styleName=QString()) const;
    TextStyle* createChildTextStyle(const QString &childName, const QString &parentName=QString());

    // functions for free positioning of elements
    inline void drawLine(double x0, double y0, double x1, double y1, LINE_TYPE lt=MED)
    {
      drawLine_internalUnits(x0 * ACCURACY_FAC, y0 * ACCURACY_FAC, x1 * ACCURACY_FAC, y1 * ACCURACY_FAC, lt);
    }
    inline void drawLine(const QPointF& p0, const QPointF& p1, LINE_TYPE lt=MED)
    {
      drawLine__internalUnits(p0 * ACCURACY_FAC, p1 * ACCURACY_FAC, lt);
    }

    inline void drawHorLine(double x0, double y0, double len, LINE_TYPE lt=MED)
    {
      drawLine(x0, y0, x0 + len, y0, lt);
    }
    inline void drawHorLine(const QPointF& p0, double len, LINE_TYPE lt=MED)
    {
      drawLine(p0, p0 + QPointF(len, 0), lt);
    }

    inline void drawVertLine(double x0, double y0, double len, LINE_TYPE lt=MED)
    {
      drawLine(x0, y0, x0, y0 + len, lt);
    }
    inline void drawVertLine(const QPointF& p0, double len, LINE_TYPE lt=MED)
    {
      drawLine(p0, p0 + QPointF(0, len), lt);
    }

    QRectF drawText(double x0, double y0, const QString& txt, const QString& styleName=QString(), HOR_TXT_ALIGNMENT align=LEFT) const;
    QRectF drawText(double x0, double y0, const QString& txt, const TextStyle* style, HOR_TXT_ALIGNMENT align=LEFT) const;
    QRectF drawText(const QPointF& basePoint, RECT_CORNER basePointAlignment, const QString& txt, const QString& styleName=QString()) const;
    QRectF drawText(const QPointF& basePoint, RECT_CORNER basePointAlignment, const QString& txt, const TextStyle* style) const;
    QRectF drawText(const QRectF& refBox, RECT_CORNER refBoxCorner, RECT_CORNER txtBasePointAlignment, const QString& txt, const TextStyle* style) const;

    // disable copy constructor, just for testing
    SimpleReportGenerator(const SimpleReportGenerator &orig) = delete;

    // determine the bounding box of a given text without adding
    // the text to the scene
    QSizeF getTextDimensions_MM(const QString& txt, const TextStyle* style);

    // calculate the position of a corner of a rectangle
    QPointF calcRectCorner(const QRectF& rect, RECT_CORNER corner) const;

  private:
    QRectF setTextPosAligned(double x, double y, QGraphicsSimpleTextItem* txt, HOR_TXT_ALIGNMENT align=LEFT) const;
    double getTextHeightForStyle(const QString& styleName=QString(), const QString& sampleText=QString());
    void drawLine_internalUnits(double x0, double y0, double x1, double y1, LINE_TYPE lt=MED) const;
    void drawLine__internalUnits(const QPointF& p0, const QPointF& p1, LINE_TYPE lt=MED) const;
    QRectF drawText__internalUnits(double x0, double y0, const QString& txt, const QString& styleName=QString(), HOR_TXT_ALIGNMENT align=LEFT) const;
    QRectF drawText__internalUnits(double x0, double y0, const QString& txt, const TextStyle* style, HOR_TXT_ALIGNMENT align=LEFT) const;
    QRectF drawText__internalUnits(const QPointF& basePoint, RECT_CORNER basePointAlignment, const QString& txt, const QString& styleName=QString()) const;
    QRectF drawText__internalUnits(const QPointF& basePoint, RECT_CORNER basePointAlignment, const QString& txt, const TextStyle* style=nullptr) const;
    double lineType2Width(LINE_TYPE lt) const;

    double w;
    double h;
    double margin;
    int pageCount;
    int curPage;
    double curY;
    QGraphicsScene* page[MAX_NUM_PAGES];
    std::unique_ptr<HeaderFooterStrings> headerFooter[MAX_NUM_PAGES];
    HeaderFooterStrings globalHeaderFooter;

    QPen thinPen;
    QPen mediumPen;
    QPen thickPen;

    double maxY;

    TabSet tabSet;
    QStack<TabSet> tabStack;

    TextStyleLib styleLib;

  };

}

#endif // SIMPLEREPORTGENERATOR_H
