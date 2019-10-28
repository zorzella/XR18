#pragma once

class ZrGain {
 private:
  static constexpr float HUMAN_MIN = -12.0;
  static constexpr float HUMAN_MAX = 60.0;

 public:
  //   GAIN, // [0.0,1.0] lin(145), -12..60 dB

  static const float humanToOscValue(const float human) {
    // HUMAN_MIN + (HUMAN_MAX - HUMAN_MIN) * result = human
    //             (HUMAN_MAX - HUMAN_MIN) * result = human - HUMAN_MIN
    //                                       result = (human - HUMAN_MIN) /
    //                                       (HUMAN_MAX - HUMAN_MIN)
    float result = (human - HUMAN_MIN) / (HUMAN_MAX - HUMAN_MIN);
    return result;
  }

  static const float oscValueToHuman(const float oscValue) {
    float result = HUMAN_MIN + (HUMAN_MAX - HUMAN_MIN) * oscValue;
    return result;
  }
};