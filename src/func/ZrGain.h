#pragma once

class ZrGain {
 private:
  static constexpr double HUMAN_MIN = -12.0;
  static constexpr double HUMAN_MAX = 60.0;

 public:
  //   GAIN, // [0.0,1.0] lin(145), -12..60 dB

  static const double humanToOscValue(const double human) {
    // HUMAN_MIN + (HUMAN_MAX - HUMAN_MIN) * result = human
    //             (HUMAN_MAX - HUMAN_MIN) * result = human - HUMAN_MIN
    //                                       result = (human - HUMAN_MIN) /
    //                                       (HUMAN_MAX - HUMAN_MIN)
    double result = (human - HUMAN_MIN) / (HUMAN_MAX - HUMAN_MIN);
    return result;
  }

  static const double oscValueToRoundedHuman(const double oscValue) {
    double result = HUMAN_MIN + (HUMAN_MAX - HUMAN_MIN) * oscValue;
    // TODO: myRound
    result = round(result * 10.0) / 10.0;
    return result;
  }
};