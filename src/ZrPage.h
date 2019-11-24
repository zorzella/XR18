#pragma once

class ZrPage {
 public:
  // TODO!
  static const int H_COUNT = 16;
  static const int V_COUNT = 3;

  int m_hCount = 16;
  int m_vCount = 3;

  int m_currentHPos;
  int m_currentVPos;

  void goRight();
  void goLeft();
  void goUp();
  void goDown();

 private:
};