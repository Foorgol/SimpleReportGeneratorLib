#include "SimpleReportGenerator.h"

#include <stdexcept>

#include <QGraphicsLineItem>
#include <QHash>
#include <QDateTime>

using namespace std;

namespace SimpleReportLib {

  constexpr char SimpleReportGenerator::DEFAULT_HEADER_STYLE_NAME[];

  constexpr char HeaderFooterStrings::TOKEN_CURPGNUM[];
  constexpr char HeaderFooterStrings::TOKEN_TOTALPGNUM[];
  constexpr char HeaderFooterStrings::TOKEN_CURDATE[];
  constexpr char HeaderFooterStrings::TOKEN_CURTIME[];

//---------------------------------------------------------------------------

  HeaderFooterStrings::HeaderFooterStrings()
  {
  }

//---------------------------------------------------------------------------

  HeaderFooterStrings::~HeaderFooterStrings()
  {
  }

//---------------------------------------------------------------------------

  void HeaderFooterStrings::substTokens(int curPageNum, int totalPageCount)
  {
    // determine local time and date and convert them into strings
    QDateTime localDateTime = QDateTime::currentDateTime();
    QString curTime = localDateTime.toString("HH:mm");
    QString curDate = localDateTime.toString("dd.MM.yyyy");

    // create and fill a substitution table
    QHash<QString, QString> substTab;
    substTab.insert(TOKEN_CURPGNUM, QString::number(curPageNum+1));
    substTab.insert(TOKEN_TOTALPGNUM, QString::number(totalPageCount));
    substTab.insert(TOKEN_CURDATE, curDate);
    substTab.insert(TOKEN_CURTIME, curTime);

    // apply substituion table to all strings
    substTokensInPlace(hl, substTab);
    substTokensInPlace(hc, substTab);
    substTokensInPlace(hr, substTab);
    substTokensInPlace(fl, substTab);
    substTokensInPlace(fc, substTab);
    substTokensInPlace(fr, substTab);
  }

  //---------------------------------------------------------------------------

  void HeaderFooterStrings::substTokensInPlace(QString& s, int idxCurPage, int totalPageCount)
  {
    // determine local time and date and convert them into strings
    QDateTime localDateTime = QDateTime::currentDateTime();
    QString curTime = localDateTime.toString("HH:mm");
    QString curDate = localDateTime.toString("dd.MM.yyyy");

    // create and fill a substitution table
    QHash<QString, QString> substTab;
    substTab.insert(TOKEN_CURPGNUM, QString::number(idxCurPage+1));
    substTab.insert(TOKEN_TOTALPGNUM, QString::number(totalPageCount));
    substTab.insert(TOKEN_CURDATE, curDate);
    substTab.insert(TOKEN_CURTIME, curTime);

    substTokensInPlace(s, substTab);
  }

  //---------------------------------------------------------------------------

  void HeaderFooterStrings::substTokensInPlace(QString& s, const QHash<QString, QString>& substTab)
  {
    for (QString key : substTab.keys())
    {
      QString val = substTab.value(key);
      s.replace(key, val);
    }
  }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

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

    if ((2*_margin) >= _w)
    {
      throw std::invalid_argument("Margin too large for page width");
    }
    if ((2*_margin) >= _h)
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
    globalHeaderFooter.hl = "";
    globalHeaderFooter.hc = "";
    globalHeaderFooter.hr = "";
    globalHeaderFooter.fl = "";
    globalHeaderFooter.fc = "";
    globalHeaderFooter.fr = "";

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
    auto headerStyle = styleLib.createChildStyle(DEFAULT_HEADER_STYLE_NAME);
    headerStyle->setFontSize_MM(PARA_FONT_SIZE__MM * 0.8);
    headerStyle->setItalicsState(true);

    curY = -1.0;
  }

  //---------------------------------------------------------------------------

  //SimpleReportGenerator::SimpleReportGenerator(const SimpleReportGenerator& orig) {
  //}

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
    curPage = (pageCount - 1);

    // limit the scene size to the paper size
    // and add a "page frame" for zooming as a background
    // color in the viewer
    QPen pen(Qt::white, 0);
    newScene->setSceneRect(0, 0, w, h);
    newScene->addRect(0,0,w,h,pen, QBrush(Qt::white));

    // reserve space for header and footer
    double headerFooterHeight = HEADER_FOOTER_SKIP__MM * ACCURACY_FAC + getTextHeightForStyle(DEFAULT_HEADER_STYLE_NAME);
    curY += headerFooterHeight;
    maxY -= headerFooterHeight;

    // return the ID of the new page
    return curPage;
  }

  //---------------------------------------------------------------------------

  int SimpleReportGenerator::getPageCount()
  {
    return pageCount;
  }

  //---------------------------------------------------------------------------

  bool SimpleReportGenerator::setActivePage(int idxPage)
  {
    if ((idxPage < 0) || (idxPage >= pageCount)) return false;

    curPage = idxPage;
    return true;
  }

  //---------------------------------------------------------------------------

  int SimpleReportGenerator::getCurrentPageNumber() const
  {
    return curPage;
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

  void SimpleReportGenerator::writeLine(QString txt, TextStyle* style, double skipAfter, double skipBefore)
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
      auto bb = setTextPosAligned(margin, curY, txtItem);
      txtHeight = bb.height();
    } else {
      QStringList txtChunk = txt.split("\t");

      // the first chunk is always left-justified, just like regular text
      // and: we always have at least one chunk, even if there is no tab in
      // the text
      QGraphicsSimpleTextItem* txtItem = pg->addSimpleText(txtChunk.at(0).trimmed(), fnt);
      auto bb = setTextPosAligned(margin, curY, txtItem);
      txtHeight = bb.height();
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
        double chunkHeight;
        auto bb = setTextPosAligned(td.pos * ACCURACY_FAC + margin, curY, txtItem, align);
        chunkHeight = bb.height();
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

  QRectF SimpleReportGenerator::setTextPosAligned(double x, double y, QGraphicsSimpleTextItem* txt, HOR_TXT_ALIGNMENT align) const
  {
    double txtWidth = txt->boundingRect().width();

    if (align == LEFT)
    {
      txt->setPos(x, y);
      return txt->boundingRect();
    }

    if (align == RIGHT)
    {
      txt->setPos(x - txtWidth, y);
      return txt->boundingRect();
    }

    txt->setPos(x - txtWidth/2.0, y);
    return txt->boundingRect();
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::setHeader(QString left, QString mid, QString right, int page)
  {
    if (page < 0)
    {
      globalHeaderFooter.hl = left.trimmed();
      globalHeaderFooter.hc = mid.trimmed();
      globalHeaderFooter.hr = right.trimmed();
      return;
    }

    if (page >= MAX_NUM_PAGES) return;

    if (headerFooter[page] == nullptr)
    {
      headerFooter[page] = std::unique_ptr<HeaderFooterStrings>(new HeaderFooterStrings);
    }

    (headerFooter[page])->hl = left.trimmed();
    (headerFooter[page])->hc = mid.trimmed();
    (headerFooter[page])->hr = right.trimmed();
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::setFooter(QString left, QString mid, QString right, int page)
  {
    if (page < 0)
    {
      globalHeaderFooter.fl = left.trimmed();
      globalHeaderFooter.fc = mid.trimmed();
      globalHeaderFooter.fr = right.trimmed();
      return;
    }

    if (page >= MAX_NUM_PAGES) return;

    if (headerFooter[page] == nullptr)
    {
      headerFooter[page] = std::unique_ptr<HeaderFooterStrings>(new HeaderFooterStrings);
    }

    (headerFooter[page])->fl = left.trimmed();
    (headerFooter[page])->fc = mid.trimmed();
    (headerFooter[page])->fr = right.trimmed();
  }

//---------------------------------------------------------------------------

  void SimpleReportGenerator::setGlobalHeader(QString left, QString mid, QString right)
  {
    globalHeaderFooter.hl = left.trimmed();
    globalHeaderFooter.hc = mid.trimmed();
    globalHeaderFooter.hr = right.trimmed();
  }

//---------------------------------------------------------------------------

  void SimpleReportGenerator::setGlobalFooter(QString left, QString mid, QString right)
  {
    globalHeaderFooter.fl = left.trimmed();
    globalHeaderFooter.fc = mid.trimmed();
    globalHeaderFooter.fr = right.trimmed();
  }

//---------------------------------------------------------------------------

  void SimpleReportGenerator::insertHeaderAndFooter(int pageNum)
  {
    if (pageCount < 1) return;

    if (pageNum < 0) pageNum = curPage;

    QGraphicsScene* sc = page[pageNum];

    auto headerStyle = styleLib.getStyle(DEFAULT_HEADER_STYLE_NAME);
    if (headerStyle == nullptr) headerStyle = styleLib.getStyle();

    QFont headerFont = *(headerStyle->getFont());

    // determine the actual text for the header / footer
    HeaderFooterStrings hfStrings;
    if (headerFooter[pageNum] != nullptr)
    {
      hfStrings = *(headerFooter[pageNum]);
    } else {
      hfStrings = globalHeaderFooter;
    }

    // insert actual date, time, page numbers, etc
    hfStrings.substTokens(pageNum, pageCount);

    // write out the text for the header
    if (!(hfStrings.hl.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(hfStrings.hl, headerFont);
      setTextPosAligned(margin, margin, txtItem);
    }
    if (!(hfStrings.hc.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(hfStrings.hc, headerFont);
      setTextPosAligned(w/2.0, margin, txtItem, CENTER);
    }
    if (!(hfStrings.hr.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(hfStrings.hr, headerFont);
      setTextPosAligned(w - margin, margin, txtItem, RIGHT);
    }

    // write out the text for the footer
    if (!(hfStrings.fl.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(hfStrings.fl, headerFont);
      auto bb = setTextPosAligned(margin, h - margin, txtItem);
      double txtHeight = bb.height();
      txtItem->moveBy(0, -txtHeight);
    }
    if (!(hfStrings.fc.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(hfStrings.fc, headerFont);
      auto bb = setTextPosAligned(w/2.0, h - margin, txtItem, CENTER);
      double txtHeight = bb.height();
      txtItem->moveBy(0, -txtHeight);
    }
    if (!(hfStrings.fr.isEmpty()))
    {
      QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(hfStrings.fr, headerFont);
      auto bb = setTextPosAligned(w - margin, h - margin, txtItem, RIGHT);
      double txtHeight = bb.height();
      txtItem->moveBy(0, -txtHeight);
    }
  }

//---------------------------------------------------------------------------

  void SimpleReportGenerator::applyHeaderAndFooterOnAllPages()
  {
    if (pageCount < 0) return;
    for (int pg = 0; pg < pageCount; ++pg) insertHeaderAndFooter(pg);
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

    drawLine_internalUnits(margin, curY, w-margin, curY, lt);

    curY += lineType2Width(lt) * DEFAULT_LINESKIP_FAC;
    curY += skipAfter * ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::addHorLine_absPos(double yPos, LINE_TYPE lt) const
  {
    drawLine_internalUnits(margin, yPos * ACCURACY_FAC, w - margin, yPos * ACCURACY_FAC, lt);
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::skip(double skipAmount)
  {
    curY += skipAmount * ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::warpTo(double newAbsYPos)
  {
    curY = newAbsYPos * ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  QPointF SimpleReportGenerator::getAbsCursorPos() const
  {
    return (QPointF(margin, curY) / ACCURACY_FAC);
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

  bool SimpleReportGenerator::hasSpaceForAnotherLine(TextStyle* style, double skipBefore)
  {
    if (style == nullptr) style = styleLib.getStyle();
    QFont fnt = *(style->getFont());

    return (curY + fnt.pixelSize() + skipBefore * ACCURACY_FAC) < maxY;
  }

  //---------------------------------------------------------------------------

  double SimpleReportGenerator::getPageWidth()
  {
    return w / ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  double SimpleReportGenerator::getPageHeight()
  {
    return h / ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  double SimpleReportGenerator::getUsablePageWidth() const
  {
    return (w - 2 * margin) / ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  double SimpleReportGenerator::getUsablePageHeight() const
  {
    return (h - 2 * margin) / ACCURACY_FAC;
  }

  //---------------------------------------------------------------------------

  TextStyle* SimpleReportGenerator::getTextStyle(const QString &styleName) const
  {
    return styleLib.getStyle(styleName);
  }

  //---------------------------------------------------------------------------

  TextStyle* SimpleReportGenerator::createChildTextStyle(const QString &childName, const QString &parentName)
  {
    return styleLib.createChildStyle(childName, parentName);
  }

  //---------------------------------------------------------------------------

  double SimpleReportGenerator::getTextHeightForStyle(const QString& styleName, const QString& sampleText)
  {
    // we need a QGraphicsScene; if none has been created yet, we return an
    // error value
    if (pageCount < 0) return -1;
    QGraphicsScene* sc = page[curPage];

    // get the style
    auto style = styleLib.getStyle(styleName);
    if (style == nullptr) style = styleLib.getStyle();
    auto fnt = style->getFont();

    // shall we use a specific text for determining the height?
    // Or shall we use a worst case default?
    QString txt = sampleText;
    if (txt.isEmpty()) txt = "X²g^j_";

    // add the text to the scene, determine the heigth and
    // immediately remove it
    QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(txt, *fnt);
    double result = txtItem->boundingRect().height();
    sc->removeItem(txtItem);
    delete txtItem;

    return result;
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::drawLine_internalUnits(double x0, double y0, double x1, double y1, LINE_TYPE lt) const
  {
    double lineWidth = THIN_LINE_WIDTH__MM;
    if (lt == MED) lineWidth = MEDIUM_LINE_WIDTH__MM;
    if (lt == THICK) lineWidth = THICK_LINE_WIDTH__MM;
    lineWidth *= ACCURACY_FAC;

    QGraphicsLineItem* line = page[curPage]->addLine(x0, y0, x1, y1);
    QPen p = QPen();
    p.setWidth(lineWidth);
    line->setPen(p);
  }

  //---------------------------------------------------------------------------

  void SimpleReportGenerator::drawLine__internalUnits(const QPointF &p0, const QPointF &p1, LINE_TYPE lt) const
  {
    drawLine_internalUnits(p0.x(), p0.y(), p1.x(), p1.y());
  }

  //---------------------------------------------------------------------------

  QRectF SimpleReportGenerator::drawText__internalUnits(double x0, double y0, const QString& txt, const QString& styleName, HOR_TXT_ALIGNMENT align) const
  {
    auto style = styleLib.getStyle(styleName);
    if (style == nullptr) style = styleLib.getStyle(); // fallback to root style

    return drawText__internalUnits(x0, y0, txt, style, align);
  }

  //---------------------------------------------------------------------------

  QRectF SimpleReportGenerator::drawText__internalUnits(double x0, double y0, const QString& txt, const TextStyle* style, HOR_TXT_ALIGNMENT align) const
  {
    // Select the right font
    QFont fnt;
    if (style == nullptr)
    {
      fnt = *(styleLib.getStyle()->getFont());
    } else {
      fnt = *(style->getFont());
    }

    QGraphicsScene* pg = page[curPage];
    QGraphicsSimpleTextItem* txtItem = pg->addSimpleText(txt, fnt);

    return setTextPosAligned(x0, y0, txtItem, align);
  }

  //---------------------------------------------------------------------------

  QRectF SimpleReportGenerator::drawText__internalUnits(const QPointF &basePoint, RECT_CORNER basePointAlignment, const QString &txt, const QString &styleName) const
  {
    auto style = styleLib.getStyle(styleName);
    if (style == nullptr) style = styleLib.getStyle(); // fallback to root style

    return drawText__internalUnits(basePoint, basePointAlignment, txt, style);
  }

  //---------------------------------------------------------------------------

  QRectF SimpleReportGenerator::drawText__internalUnits(const QPointF &basePoint, RECT_CORNER basePointAlignment, const QString &txt, const TextStyle *style) const
  {
    // return empty rect if we have no valid page
    if (curPage < 0) return QRectF();

    // Select the right font
    QFont fnt;
    if (style == nullptr)
    {
      fnt = *(styleLib.getStyle()->getFont());
    } else {
      fnt = *(style->getFont());
    }

    QGraphicsScene* pg = page[curPage];
    QGraphicsSimpleTextItem* txtItem = pg->addSimpleText(txt, fnt);

    // calculate the base point position in the text item's current bounding box
    auto txtBox = txtItem->boundingRect();
    QPointF srcBasePoint = calcRectCorner(txtBox, basePointAlignment);

    // determine the offset between the base points to
    // get the translation vector from the source to the target postion
    QPointF translationVector = basePoint - srcBasePoint;

    // apply the translation vector to the txt item's current position
    QPointF srcTxtPos = txtItem->pos();
    QPointF dstTxtPos = srcTxtPos + translationVector;
    txtItem->setPos(dstTxtPos);

    return txtItem->boundingRect();
  }

  //---------------------------------------------------------------------------

  QRectF SimpleReportGenerator::drawText(double x0, double y0, const QString& txt, const QString& styleName, HOR_TXT_ALIGNMENT align) const
  {
    QRectF internalRect = drawText__internalUnits(x0 * ACCURACY_FAC, y0*ACCURACY_FAC, txt, styleName, align);

    double _x0, _y0, _w, _h;
    internalRect.getRect(&_x0, &_y0, &_w, &_h);

    return QRectF(_x0 / ACCURACY_FAC, _y0 / ACCURACY_FAC, _w / ACCURACY_FAC, _h / ACCURACY_FAC);
  }

  //---------------------------------------------------------------------------

  QRectF SimpleReportGenerator::drawText(double x0, double y0, const QString& txt, const TextStyle* style, HOR_TXT_ALIGNMENT align) const
  {
    QRectF internalRect = drawText__internalUnits(x0 * ACCURACY_FAC, y0*ACCURACY_FAC, txt, style, align);

    double _x0, _y0, _w, _h;
    internalRect.getRect(&_x0, &_y0, &_w, &_h);

    return QRectF(_x0 / ACCURACY_FAC, _y0 / ACCURACY_FAC, _w / ACCURACY_FAC, _h / ACCURACY_FAC);
  }

  //---------------------------------------------------------------------------

  QRectF SimpleReportGenerator::drawText(const QPointF &basePoint, RECT_CORNER basePointAlignment, const QString &txt, const QString &styleName) const
  {

  }

  //---------------------------------------------------------------------------

  QRectF SimpleReportGenerator::drawText(const QPointF &basePoint, RECT_CORNER basePointAlignment, const QString &txt, const TextStyle *style) const
  {
    QRectF bbInternalUnits = drawText__internalUnits(basePoint * ACCURACY_FAC, basePointAlignment, txt, style);

    QRectF bbExternalUnits(bbInternalUnits.topLeft() / ACCURACY_FAC, bbInternalUnits.size() / ACCURACY_FAC);
    return bbExternalUnits;
  }

  //---------------------------------------------------------------------------

  QRectF SimpleReportGenerator::drawText(const QRectF &refBox, RECT_CORNER refBoxCorner, RECT_CORNER txtBasePointAlignment, const QString &txt, const TextStyle *style) const
  {
    auto basepoint = calcRectCorner(refBox, refBoxCorner);
    return drawText(basepoint, txtBasePointAlignment, txt, style);
  }

  //---------------------------------------------------------------------------

  QSizeF SimpleReportGenerator::getTextDimensions_MM(const QString& txt, const TextStyle* style)
  {
    // we need a QGraphicsScene; if none has been created yet, we return an
    // error value
    if (pageCount < 0) return QSizeF();
    QGraphicsScene* sc = page[curPage];

    // get the style
    if (style == nullptr) style = styleLib.getStyle();
    auto fnt = style->getFont();

    // add the text to the scene, determine the size and
    // immediately remove it
    QGraphicsSimpleTextItem* txtItem = sc->addSimpleText(txt, *fnt);
    QSizeF result_internalUnits = txtItem->boundingRect().size();
    sc->removeItem(txtItem);
    delete txtItem;

    // convert internal units to external units (mm) and return the result
    return (result_internalUnits / ACCURACY_FAC);
  }

  //---------------------------------------------------------------------------

  QPointF SimpleReportGenerator::calcRectCorner(const QRectF &rect, RECT_CORNER corner) const
  {
    QPointF tmp;
    switch (corner)
    {
      case RECT_CORNER::TOP_LEFT:
        return rect.topLeft();

      case RECT_CORNER::TOP_CENTER:
        tmp = rect.topLeft();
        tmp.setX(tmp.x() + rect.width() / 2.0);
        return tmp;

      case RECT_CORNER::TOP_RIGHT:
        return rect.topRight();

      case RECT_CORNER::MID_LEFT:
        tmp = rect.topLeft();
        tmp.setY(tmp.y() + rect.height() / 2.0);
        return tmp;

      case RECT_CORNER::CENTER:
        return rect.center();

      case RECT_CORNER::MID_RIGHT:
        tmp = rect.topRight();
        tmp.setY(tmp.y() + rect.height() / 2.0);
        return tmp;

      case RECT_CORNER::BOTTOM_LEFT:
        return rect.bottomLeft();
        break;

      case RECT_CORNER::BOTTOM_CENTER:
        tmp = rect.bottomLeft();
        tmp.setX(tmp.x() + rect.width() / 2.0);
        return tmp;

      case RECT_CORNER::BOTTOM_RIGHT:
        return rect.bottomRight();
    }

    return tmp;
  }

  //---------------------------------------------------------------------------

  double SimpleReportGenerator::lineType2Width(LINE_TYPE lt) const
  {
    double lineWidth = THIN_LINE_WIDTH__MM;
    if (lt == MED) lineWidth = MEDIUM_LINE_WIDTH__MM;
    if (lt == THICK) lineWidth = THICK_LINE_WIDTH__MM;
    return lineWidth * ACCURACY_FAC;
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


}
