#pragma once

class ZrFader {
 private:
  static constexpr float HUMAN_MIN = -90.0;
  static constexpr float HUMAN_MAX = 10.0;

 public:
  // [0.0,1.0] fader(1024), -oo..10 dB
  //                        around -90.0dB to 10.0dB
  // values -89.8 and under are -oo

/*

fader/level parameter: 

dblevel =    40  * idx / (steps - 1) - 30; if idx >= steps / 2 
             80  * idx / (steps - 1) - 50; if idx >= steps / 4
            160  * idx / (steps - 1) - 70; if idx >= steps / 16
            480  * idx / (steps - 1) - 90; if idx > 0
           -144                          ; if idx = 0

*/

  // TODO: this is wrong
  static const float humanToOscValue(const float human) {
    // HUMAN_MIN + (HUMAN_MAX - HUMAN_MIN) * result = human
    //             (HUMAN_MAX - HUMAN_MIN) * result = human - HUMAN_MIN
    //                                       result = (human - HUMAN_MIN) /
    //                                       (HUMAN_MAX - HUMAN_MIN)
    float result = (human - HUMAN_MIN) / (HUMAN_MAX - HUMAN_MIN);
    return result;
  }

  // TODO: this is wrong
  static const float oscValueToHuman(const float oscValue) {
    float result = HUMAN_MIN + (HUMAN_MAX - HUMAN_MIN) * oscValue;
    return result;
  }
};