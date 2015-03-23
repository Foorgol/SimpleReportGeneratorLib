#include "SimpleReportGenerator.h"

#include <stdexcept>

#include <QGraphicsLineItem>

namespace SimpleReportLib {

  SimpleReportGenerator::SimpleReportGenerator(double _w, double _h, double _margin)
  {
    if (_w <= 0)
    {
      throw std::invalid_argument("Page width can't be less or equal to zero");
    }

    if (_h <= 0)
    {
      throw std::invalid_argument("Page height can't be less or equal to zero");
    }

    if ((2*margin) >= _w)
    {
      throw std::invalid_argument("Margin too large for page width");
    }
    if ((2*margin) >= _h)
    {
      throw std::invalid_argument("Margin too large for page height");
    }

    // store data; convert from "mm" (parameters) to a fictional internal
    // unit that uses higher "numbers" and thus yields nicer text rendering
    w = _w * ACCURACY_FAC;
    h = _h * ACCURACY_FAC;
    margin = _margin * ACCURACY_FAC;

    pageCount = 0;
    for (int i=0; i<MAX_NUM_PAGES; i++) page[i] = nullptr;
    curPage = -1;

    // initialize header and footer strings
    headerLeft = "";
    headerCenter = "";
    headerRight = "";
    footerLeft = "";
    footerCenter = "";
    footerRight = "";

    // initialize some drawing tools
    thinPen = QPen();
    thinPen.setWidthF(THIN_LINE_WIDTH__MM * ACCURACY_FAC);
    mediumPen = QPen();
    mediumPen.setWidthF(MEDIUM_LINE_WIDTH__MM * ACCURACY_FAC);
    thickPen = QPen();
    thickPen.setWidthF(THICK_LINE_WIDTH__MM * ACCURACY_FAC);

    // initialize some styles in the style lib
    auto rootStyle = styleLib.getStyle();   // root is guaranteed to exist
    rootStyle->setFontname(SYS_FONT);
    rootStyle->setFontSize_MM(PARA_FONT_SIZE__MM);
    auto h1Style = styleLib.createChildStyle("H1");
    h1Style->setFontSize_MM(H1_FONT_SIZE__MM);
    h1Style->setBoldState(true);
    auto h2Style = styleLib.createChildStyle("H2");
    h2Style->setFontSize_MM(H2_FONT_SIZE__MM);
    h2Style->setBoldState(true);
    auto headerStyle = styleLib.createChildStyle("Header");
    headerStyle->setFontSize_MM(PARA_FONT_SIZE__MM * 0.8);
    headerStyle->setItalicsState(true);

    curY = -1.0;
  }

  //---------------------------------------------------------------------------

  SimpleReportGenerator::SimpleReportGenerator(const SimpleReportGenerator& orig) {
  }

  //---------------------------------------------------------------------------

  SimpleReportGenerator::~SimpleReportGenerator() {
    deleteAllPages();
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::deleteAllPages()
  {
    for (int i=0; i < MAX_NUM_PAGES; i++)
    {
      QGraphicsScene* sc = page[i];
      if (sc != 0)
      {
        delete sc;
        page[i] = 0;
      }
    }
  }

  //---------------------------------------------------------------------------

  int SimpleReportGenerator::startNextPage()
  {
    if (pageCount >= MAX_NUM_PAGES)
    {
      return -1;
    }

    // start a new scene ( = page) and initialize it accordingly
    auto newScene = new QGraphicsScene(0, 0, w, h);
    page[pageCount] = newScene;
    ++pageCount;
    curY = margin;
    maxY = h - margin;
    ++curPage;

    // limit the scene size to the paper size
    // and add a "page frame"
    newScene->setSceneRect(0, 0, w, h);
    newScene->addRect(0,0,w,h,QPen(), QBrush(Qt::white));

    // set header and footer
    insertHeaderAndFooterOnCurrentPage();


    // return the ID of the new page
    return pageCount - 1;
  }

  //---------------------------------------------------------------------------

  int SimpleReportGenerator::getPageCount()
  {
    return pageCount;
  }

  //---------------------------------------------------------------------------

  QGraphicsScene* SimpleReportGenerator::getPage(int pageNum)
  {
    if (pageNum >= pageCount) return 0;

    return page[pageNum];
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::writeLine(QString txt, const QString& styleName, double skipAfter, double skipBefore)
  {
    auto style = styleLib.getStyle(styleName);
    if (style == nullptr) style = styleLib.getStyle(); // fallback to root style

    writeLine(txt, style, skipAfter, skipBefore);
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::writeLine(QString txt, std::shared_ptr<TextStyle> style, double skipAfter, double skipBefore)
  {
    if (style == nullptr) style = styleLib.getStyle();

    if (!(hasSpaceForAnotherLine(style, skipBefore)))
    {
      startNextPage();
    } else {
      curY += skipBefore * ACCURACY_FAC;
    }

    // Select the right font
    QFont fnt = *(style->getFont());

    QGraphicsScene* pg = page[curPage];

    // add the text to the scene (separated by tabs if applicable)
    double txtHeight = 0;
    if (tabSet.getTabCount() == 0)
    {
      // no tabs defined, simply write out the text
      QGraphicsSimpleTextItem* txtItem = pg->addSimpleText(txt, fnt);
      txtHeight = setTextPosAligned(margin, curY, txtItem);
    } else {
      QStringList txtChunk = txt.split("\t");

      // the first chunk is always left-justified, just like regular text
      // and: we always have at least one chunk, even if there is no tab in
      // the text
      QGraphicsSimpleTextItem* txtItem = pg->addSimpleText(txtChunk.at(0).trimmed(), fnt);
      txtHeight = setTextPosAligned(margin, curY, txtItem);
      txtChunk.removeFirst();

      // if we have more chunks than tabs, simply merge the last chunks back together
      while (txtChunk.count() > tabSet.getTabCount())
      {
        QString lastChunk = txtChunk.last();
        txtChunk.removeLast();
        lastChunk = txtChunk.last().trimmed() + " " + lastChunk.trimmed();
        txtChunk.removeLast();
        txtChunk.append(lastChunk);
      }

      // now simply write out the chunks according to the tab definitions
      for (int tabIndex=0; tabIndex < txtChunk.count(); tabIndex++)
      {
        TabDef td = tabSet.getTabAt(tabIndex);
        HOR_TXT_ALIGNMENT align = LEFT;
        if (td.just == TAB_CENTER) align = CENTER;
        if (td.just == TAB_RIGHT) align = RIGHT;
        txtItem = pg->addSimpleText(txtChunk.at(tabIndex).trimmed(), fnt);
        double chunkHeight = setTextPosAligned(td.pos * ACCURACY_FAC + margin, curY, txtItem, align);
        txtHeight = qMax(txtHeight, chunkHeight);
      }
    }

    // increment the cursor position by the actual text height
    // plus line skip
    curY += txtHeight * DEFAULT_LINESKIP_FAC;

    // add a potential space after the text
    curY += skipAfter * ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  double SimpleReportGenerator::setTextPosAligned(double x, double y, QGraphicsSimpleTextItem* txt, HOR_TXT_ALIGNMENT align)
  {
    QRectF bb = txt->boundingRect();
    double txtWidth = bb.width();

    if (align == LEFT)
    {
      txt->setPos(x, y);
      return bb.height();
    }



    if (align == RIGHT)
    {
      txt->setPos(x - txtWidth, y);
      return bb.height();
    }

    txt->setPos(x - txtWidth/2.0, y);
    return bb.height();
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::setHeader(QString left, QString mid, QString right)
  {
    headerLeft = left.trimmed();
    headerCenter = mid.trimmed();
    headerRight = right.trimmed();
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::setFooter(QString left, QString mid, QString right)
  {
    footerLeft = left.trimmed();
    footerCenter = mid.trimmed();
    footerRight = right.trimmed();
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::insertHeaderAndFooterOnCurrentPage()
  {
    if (pageCount < 1) return;

    QGraphicsScene* sc = page[curPage];

    auto headerStyle = styleLib.getStyle("Header");
    if (headerStyle == nullptr) headerStyle = styleLib.getStyle();

    QFont headerFont = *(headerStyle->getFont());

    double headerSkip = 0.0;
    if (!(headerLeft.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(headerLeft, headerFont);
      double txtHeight = setTextPosAligned(margin, margin, txtItem);
      headerSkip = txtHeight * DEFAULT_LINESKIP_FAC;
    }
    if (!(headerCenter.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(headerCenter, headerFont);
      double txtHeight = setTextPosAligned(w/2.0, margin, txtItem, CENTER);
      headerSkip = txtHeight * DEFAULT_LINESKIP_FAC;
    }
    if (!(headerRight.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(headerRight, headerFont);
      double txtHeight = setTextPosAligned(w - margin, margin, txtItem, RIGHT);
      headerSkip = txtHeight * DEFAULT_LINESKIP_FAC;
    }
    curY += headerSkip;
    curY += HEADER_FOOTER_SKIP__MM * ACCURACY_FAC;

    double footerSkip = 0.0;
    if (!(footerLeft.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(footerLeft, headerFont);
      double txtHeight = setTextPosAligned(margin, maxY, txtItem);
      footerSkip = txtHeight * DEFAULT_LINESKIP_FAC;
      txtItem->moveBy(0, -txtHeight);
    }

    if (!(footerCenter.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(footerCenter, headerFont);
      double txtHeight = setTextPosAligned(w/2.0, maxY, txtItem, CENTER);
      footerSkip = txtHeight * DEFAULT_LINESKIP_FAC;
      txtItem->moveBy(0, -txtHeight);
    }
    if (!(footerRight.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(footerRight, headerFont);
      double txtHeight = setTextPosAligned(w - margin, maxY, txtItem, RIGHT);
      footerSkip = txtHeight * DEFAULT_LINESKIP_FAC;
      txtItem->moveBy(0, -txtHeight);
    }

    maxY -= footerSkip;
    maxY -= HEADER_FOOTER_SKIP__MM * ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::addTab(double pos, TAB_JUSTIFICATION just)
  {
    tabSet.addTab(pos, just);
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::removeTab(double pos)
  {
    tabSet.removeTab(pos);
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::clearAllTabs()
  {
    tabSet.clearAllTabs();
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::addHorLine(LINE_TYPE lt, double skipAfter, double skipBefore)
  {
    curY += skipBefore * ACCURACY_FAC;

    double lineWidth = THIN_LINE_WIDTH__MM;
    if (lt == MED) lineWidth = MEDIUM_LINE_WIDTH__MM;
    if (lt == THICK) lineWidth = THICK_LINE_WIDTH__MM;
    lineWidth *= ACCURACY_FAC;

    curY += lineWidth * DEFAULT_LINESKIP_FAC;
    QGraphicsLineItem* line = page[curPage]->addLine(margin, curY, w - margin, curY);
    QPen p = QPen();
    p.setWidth(lineWidth);
    line->setPen(p);
    curY += lineWidth * DEFAULT_LINESKIP_FAC;

    curY += skipAfter * ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::skip(double skipAmount)
  {
    curY += skipAmount * ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::pushTabs(TabSet &newTabs)
  {
    tabStack.push(tabSet);
    tabSet = newTabs;
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::popTabs()
  {
    if (tabStack.count() > 0)
    {
      tabSet = tabStack.pop();
    }
  }

  //---------------------------------------------------------------------------

  bool SimpleReportGenerator::hasSpaceForAnotherLine(const QString &styleName, double skipBefore)
  {
    auto style = styleLib.getStyle(styleName);
    if (style == nullptr) style = styleLib.getStyle();
    return hasSpaceForAnotherLine(style, skipBefore);
  }

  bool SimpleReportGenerator::hasSpaceForAnotherLine(std::shared_ptr<TextStyle> style, double skipBefore)
  {
    if (style == nullptr) style = styleLib.getStyle();
    QFont fnt = *(style->getFont());

    return (curY + fnt.pixelSize() + skipBefore * ACCURACY_FAC) <= maxY;
  }

  //---------------------------------------------------------------------------

  double SimpleReportGenerator::getPageWidth()
  {
    return w / ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  std::shared_ptr<TextStyle> SimpleReportGenerator::getTextStyle(const QString &styleName) const
  {
    return styleLib.getStyle(styleName);
  }

  //---------------------------------------------------------------------------

  std::shared_ptr<TextStyle> SimpleReportGenerator::createChildTextStyle(const QString &childName, const QString &parentName)
  {
    return styleLib.createChildStyle(childName, parentName);
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


}
