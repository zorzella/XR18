#pragma once

#include "ZrFunction.h"

class ZrPage {
 public:
  ZrPage() = delete;

  ZrPage(int vCount, int hCount);

  const int m_vCount;
  const int m_hCount;
  const int m_indexCount;

  ZrFunction* m_functions;

  int m_currentHPos;
  int m_currentVPos;

  void goRight();
  void goLeft();
  void goUp();
  void goDown();

  const int index() const;
  const int indexOf(int h, int v) const;
  const int indexCount() const;

 private:
};
