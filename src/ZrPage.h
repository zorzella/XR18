#pragma once

#include "ZrFunction.h"

class ZrPage {
 public:
  // TODO!
  static const int H_COUNT = 16;
  static const int V_COUNT = 3;

  ZrFunction m_functions[ZrPage::H_COUNT * ZrPage::V_COUNT];

  int m_hCount = 16;
  int m_vCount = 3;

  int m_currentHPos;
  int m_currentVPos;

  void goRight();
  void goLeft();
  void goUp();
  void goDown();

  const int index() const;
  const int indexOf(int h, int v) const;

 private:
};