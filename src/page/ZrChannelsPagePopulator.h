#pragma once

#include "ZrPage.h"

class ZrChannelsPagePopulator {
 public:
  static void populate(ZrPage& m_currentPage) {
    char temp[50];

    for (int h = 0; h < m_currentPage.m_hCount; h++) {
      for (int v = 0; v < m_currentPage.m_vCount; v++) {
        int channelNumber = h + 1;
        int ind = index(h, v);
        ZrFunction& toPopulate = m_currentPage.m_functions[ind];
        toPopulate.m_hPos = h;
        toPopulate.m_vPos = v;
        if (h < 18) {
          sprintf(temp, "CH%02d", channelNumber);
          toPopulate.m_humanChannelName = temp;
          toPopulate.m_typeDesc =
              ZrFuncTypeDescription::posToFuncTypeDescription(h, v);
          switch (toPopulate.m_typeDesc.type()) {
            case GAIN:
              sprintf(temp, "/headamp/%02d/gain", channelNumber);
              toPopulate.m_oscAddr = temp;
              break;
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
  }

 private:
  static const int index(int h, int v) { return h + v * ZrPage::H_COUNT; }
};