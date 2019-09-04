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

#ifndef SIMPLEREPORTGENERATOR_H
#define SIMPLEREPORTGENERATOR_H

#include <QPen>
#include <QFont>
#include <QHash>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsScene>
#include <QStack>

//#include "simplereportgenerator_global.h"
#include "TabSet.h"

#include "TextStyle.h"
#include "TextStyleLib.h"

using namespace std;

namespace SimpleReportLib {
  static constexpr double THIN_LINE_WIDTH__MM = 0.1;
  static constexpr double MEDIUM_LINE_WIDTH__MM = 0.35;
  static constexpr double THICK_LINE_WIDTH__MM = 0.7;
  static constexpr double H1_FONT_SIZE__MM = 5.0;
  static constexpr double H2_FONT_SIZE__MM = 3.5;
  static constexpr double PARA_FONT_SIZE__MM = 2.0;
  #define SYS_FONT "Arial"
  static constexpr double DEFAULT_PARSKIP__MM = 1.0;
  static constexpr double DEFAULT_LINESKIP_FAC = 1.1;
  static constexpr double HEADER_FOOTER_SKIP__MM = 3.0;

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

  class HeaderFooterStrings
  {
  public:
    static constexpr char TOKEN_CURPGNUM[] = "$#$";
    static constexpr char TOKEN_TOTALPGNUM[] = "$##$";
    static constexpr char TOKEN_CURDATE[] = "$__DATE__$";
    static constexpr char TOKEN_CURTIME[] = "$__TIME__$";
    void substTokens(int curPageNum, int totalPageCount);
    static void substTokensInPlace(QString& s, int idxCurPage, int totalPageCount);
    static void substTokensInPlace(QString& s, const QHash<QString, QString>& substTab);

    //void setHeader(QString l, QString c, QString r);
    //void setFooter(QString l, QString c, QString r);
    QString hl;
    QString hc;
    QString hr;
    QString fl;
    QString fc;
    QString fr;
  };

  class SimpleReportGenerator
  {

  public:
    static constexpr char DEFAULT_HEADER_STYLE_NAME[] = "Header";

    SimpleReportGenerator(double _w, double _h, double _margin);
    //SimpleReportGenerator(const SimpleReportGenerator& orig);
    virtual ~SimpleReportGenerator();
    //void deleteAllPages();

    void startNextPage();
    int getPageCount();
    bool setActivePage(int idxPage);
    //int getCurrentPageNumber() const;
    QGraphicsScene* getPage(int idxPage);

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
    QRectF drawMultilineText(const QPointF& basePoint, RECT_CORNER basePointAlignment, const QStringList& lines, HOR_TXT_ALIGNMENT horAlign, double lineSpace__MM, const TextStyle* style) const;
    QRectF drawMultilineText(const QPointF& basePoint, RECT_CORNER basePointAlignment, const QString& newlineSepTxt, HOR_TXT_ALIGNMENT horAlign, double lineSpace__MM, const TextStyle* style) const;
    QRectF drawMultilineText(const QRectF& refBox, RECT_CORNER refBoxCorner, RECT_CORNER basePointAlignment, const QString& newlineSepTxt, HOR_TXT_ALIGNMENT horAlign, double lineSpace__MM, const TextStyle* style) const;

    void drawRect(const QRectF& rect, LINE_TYPE lt=MED, const QColor& fillColor = QColor(255, 255, 255)) const;

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
    QRectF moveTextItem(QGraphicsSimpleTextItem* item, const QPointF& targetPoint, RECT_CORNER targetPointAlignment) const;
    QGraphicsSimpleTextItem* addStyledTextItem(const QString& txt, const TextStyle* style=nullptr) const;
    QGraphicsSimpleTextItem* addStyledTextItem(const QString& txt, const QString& styleName=QString()) const;
    QRectF drawMultilineText__internalUnits(const QPointF& basePoint, RECT_CORNER basePointAlignment, const QStringList& lines, HOR_TXT_ALIGNMENT horAlign, double lineSpace, const TextStyle* style) const;
    void drawRect__internalUnits(const QRectF& rect, LINE_TYPE lt=MED, const QColor& fillColor = QColor(255, 255, 255)) const;
    double lineType2Width__internalUnits(LINE_TYPE lt) const;
    QPen lineType2Pen(LINE_TYPE lt, const QColor& penCol = QColor(Qt::black), Qt::PenStyle style = Qt::SolidLine) const;

    double w;
    double h;
    double margin;
    QGraphicsScene* curPagePtr{nullptr};  // not owning, owner is the vector of unique_ptr
    int idxCurPage{0};
    double curY;
    std::vector<std::unique_ptr<QGraphicsScene>> pages;
    std::vector<std::unique_ptr<HeaderFooterStrings>> headerFooter;
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
