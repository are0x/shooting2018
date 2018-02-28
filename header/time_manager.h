#pragma once

namespace are_time {
  class TimeManager{
    double diff;
    double prevTime;
    double nowTime;
  public :
    TimeManager();
    TimeManager(double nowTime);
    void Update(double nowTime);
    double GetDiff();
    double GetPrev();
    double GetNow();
  };
}

extern are_time::TimeManager timeManager;
