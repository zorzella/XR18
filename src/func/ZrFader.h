#pragma once

class ZrFader {
 private:

  static double myRound(double number) { return round(number * 10.0) / 10.0; }

 public:
  /*
  fader/level parameter:

  dblevel =    40  * idx / (steps - 1) - 30; if idx >= steps / 2
               80  * idx / (steps - 1) - 50; if idx >= steps / 4
              160  * idx / (steps - 1) - 70; if idx >= steps / 16
              480  * idx / (steps - 1) - 90; if idx > 0
             -144                          ; if idx = 0

  */

  /*
    Notes:
      +10 is 1
       +5 is 0.875855
       +1 is 0.775171
       +1 is 0.774194
       +0 is 0.750733
        0 is 0.749756
       -1 is 0.725318
       -5 is 0.625611
      -10 is 0.500489
      -30 is 0.250244
      -75 is 0.0312805
    -89.5 is 0.0009775
      -oo is 0
  */

  static const double humanToOscValue(const double human) {
    if (human > -10) {
      return (human + 30.0) / 40.0;
    }
    if (human > -30) {
      return (human + 50.0) / 80.0;
    }
    if (human > -60) {
      return (human + 70.0) / 160.0;
    }
    if (human > -90) {
      return (human + 90.0) / 480.0;
    }
    return 0;
  }

  static const double oscValueToHuman(const double oscValue) {
    if (oscValue > 0.5) {  // human: -10 to 10
      return myRound(40 * oscValue - 30);
    }

    if (oscValue > 0.25) {  // human: -30 to -10
      return myRound(80 * oscValue - 50);
    }

    if (oscValue > 0.0625) {  // human: -60 to -30
      return myRound(160 * oscValue) - 70;
    }

    if (oscValue > 0) {  // human: -89.9 to -60
      return myRound(480 * oscValue) - 90;
    }

    return -90; // Yes, the manual says this is actually -144dB, but, whatever
  }
};