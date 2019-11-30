#pragma once

#include "ZrPage.h"

class ZrSnapshotPagePopulator {
 public:
  static const int H_COUNT = 1;
  static const int V_COUNT = 64;

  static void populate(ZrPage& currentPage) {
    char temp[50];

    for (int h = 0; h < currentPage.m_hCount; h++) {
      for (int v = 0; v < currentPage.m_vCount; v++) {
        int snapshotNumber = v + 1;
        int ind = index(currentPage, h, v);
        ZrFunction& toPopulate = currentPage.m_functions[ind];
        toPopulate.m_hPos = h;
        toPopulate.m_vPos = v;
        sprintf(temp, "Snapshot %02d", snapshotNumber);
        toPopulate.m_humanChannelName = temp;
        toPopulate.m_typeDesc = posToFuncTypeDescription(h, v);
        switch (toPopulate.m_typeDesc.type()) {
          case SNAPSHOT:
            sprintf(temp, "/-snap/%02d/name", snapshotNumber);
            toPopulate.m_oscAddr = temp;
            break;
          default:
            Serial.print("Unimplemented type: ");
            Serial.println(toPopulate.m_typeDesc.type());
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
    return SNAPSHOT;
  }

  static const ZrFuncTypeDescription posToFuncTypeDescription(int h, int v) {
    return ZrFuncTypeDescription::fromType(typeForPosition(h, v));
  }
};