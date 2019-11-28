#include "ZrDebug.h"
#include "ZrPage.h"

ZrPage::ZrPage(int vCount, int hCount)
    : m_vCount{vCount},
      m_hCount{hCount},
      m_indexCount{vCount * hCount} {}

/**
 * @return the ZrFunction to go to when the current function is @a other and
 * the "right" button is pressed.
 */
void ZrPage::goRight() {
  TRACE();
  m_currentHPos++;
  if (m_currentHPos == m_hCount) {
    m_currentHPos = 0;
  }
}

void ZrPage::goLeft() {
  TRACE();
  m_currentHPos--;
  if (m_currentHPos < 0) {
    m_currentHPos = m_hCount - 1;
  }
}

void ZrPage::goDown() {
  TRACE();
  m_currentVPos++;
  if (m_currentVPos == m_vCount) {
    m_currentVPos = 0;
  }
}

void ZrPage::goUp() {
  TRACE();
  m_currentVPos--;
  if (m_currentVPos < 0) {
    m_currentVPos = m_vCount - 1;
  }
}

const int ZrPage::indexOf(int h, int v) const {
  return h + v * ZrPage::H_COUNT;
}

const int ZrPage::index() const {
  return indexOf(m_currentHPos, m_currentVPos);
}

const int ZrPage::indexCount() const { return m_indexCount; }
