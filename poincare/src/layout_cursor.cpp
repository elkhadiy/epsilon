#include <poincare/layout_cursor.h>
#include <poincare/layout_reference.h>
#include <stdio.h>

namespace Poincare {

/* Getters and setters */

KDCoordinate LayoutCursor::cursorHeight() {
  KDCoordinate height = layoutHeight();
  return height == 0 ? k_cursorHeight : height;
}

KDCoordinate LayoutCursor::baseline() {
  if (layoutHeight() == 0) {
    return k_cursorHeight/2;
  }
  KDCoordinate layoutBaseline = m_layoutRef.baseline();
  LayoutRef equivalentLayoutRef = m_layoutRef.equivalentCursor(this).layoutReference();
  if (m_layoutRef.hasChild(equivalentLayoutRef)) {
    return equivalentLayoutRef.baseline();
  } else if (m_layoutRef.hasSibling(equivalentLayoutRef)) {
    return max(layoutBaseline, equivalentLayoutRef.baseline());
  }
  return layoutBaseline;
}

/* Comparison */

bool LayoutCursor::isEquivalentTo(LayoutCursor cursor) {
  assert(isDefined());
  assert(cursor.isDefined());
  return middleLeftPoint() == cursor.middleLeftPoint();
}

/* Position */

KDPoint LayoutCursor::middleLeftPoint() {
  KDPoint layoutOrigin = layoutReference().absoluteOrigin();
  return layoutOrigin;
}

/* Move */

void LayoutCursor::moveLeft(bool * shouldRecomputeLayout) {
  layoutReference().typedNode()->moveCursorLeft(this, shouldRecomputeLayout);
}

void LayoutCursor::moveRight(bool * shouldRecomputeLayout) {
  layoutReference().typedNode()->moveCursorRight(this, shouldRecomputeLayout);
}

void LayoutCursor::moveAbove(bool * shouldRecomputeLayout) {
  layoutReference().typedNode()->moveCursorUp(this, shouldRecomputeLayout);
}

void LayoutCursor::moveUnder(bool * shouldRecomputeLayout) {
  layoutReference().typedNode()->moveCursorDown(this, shouldRecomputeLayout);
}

/* Layout modification */
void LayoutCursor::addLayoutAndMoveCursor(LayoutRef l) {
  bool layoutWillBeMerged = l.isHorizontal();
  m_layoutRef.addSiblingAndMoveCursor(this, l); //TODO
  if (!layoutWillBeMerged) {
    l.collapseSiblingsAndMoveCursor(this);
  }
}

void LayoutCursor::clearLayout() {
  LayoutRef rootLayoutR = m_layoutRef.root();
  assert(rootLayoutR.isHorizontal());
  rootLayoutR.removeChildren();
  m_layoutRef = rootLayoutR;
}

/* Private */

KDCoordinate LayoutCursor::layoutHeight() {
  LayoutRef equivalentLayoutRef = m_layoutRef.equivalentCursor(this).layoutReference();
  if (m_layoutRef.hasChild(equivalentLayoutRef)) {
    return equivalentLayoutRef.layoutSize().height();
  }
  KDCoordinate pointedLayoutHeight = m_layoutRef.layoutSize().height();
  if (m_layoutRef.hasSibling(equivalentLayoutRef)) {
    KDCoordinate equivalentLayoutHeight = equivalentLayoutRef.layoutSize().height();
    KDCoordinate pointedLayoutBaseline = m_layoutRef.baseline();
    KDCoordinate equivalentLayoutBaseline = equivalentLayoutRef.baseline();
    return max(pointedLayoutBaseline, equivalentLayoutBaseline)
      + max(pointedLayoutHeight - pointedLayoutBaseline, equivalentLayoutHeight - equivalentLayoutBaseline);
  }
  return pointedLayoutHeight;

}

}
