#pragma once

#include "ZrPage.h"

class ZrMasterPagePopulator {
 public:
  static const int H_COUNT = 1;
  static const int V_COUNT = 2;

  static void populate(ZrPage& currentPage) {
    char temp[50];

    for (int h = 0; h < currentPage.m_hCount; h++) {
      for (int v = 0; v < currentPage.m_vCount; v++) {
        int channelNumber = h + 1;
        int ind = index(currentPage, h, v);
        ZrFunction& toPopulate = currentPage.m_functions[ind];
        toPopulate.m_hPos = h;
        toPopulate.m_vPos = v;
        sprintf(temp, "CH%02d", channelNumber);
        toPopulate.m_humanChannelName = temp;
        toPopulate.m_typeDesc = posToFuncTypeDescription(h, v);
        switch (toPopulate.m_typeDesc.type()) {
          case EQ:
            sprintf(temp, "/ch/%02d/eq/on", channelNumber);
            toPopulate.m_oscAddr = temp;
            break;
          case FADER:
            sprintf(temp, "/ch/%02d/mix/fader", channelNumber);
            toPopulate.m_oscAddr = temp;
            break;
          default:
            break;
        }
      }
    }
  }

 private:
  static const int index(ZrPage& currentPage, int h, int v) {
    return h + v * currentPage.m_hCount;
  }

  static const ZrFuncType typeForPosition(int h, int v) {
    if (h >= 0 && h <= 17) {
      switch (v) {
        case 0:
          return EQ;
        case 1:
          return FADER;
      }
    }
    return TYPE_UNKNOWN;
  }

  static const ZrFuncTypeDescription posToFuncTypeDescription(int h, int v) {
    return ZrFuncTypeDescription::fromType(typeForPosition(h, v));
  }
};