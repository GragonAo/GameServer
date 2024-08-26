#pragma once

#include "component.h"
#include "entity.h"
#include "util_time.h"
#include <queue>

struct Timer {
  timeutil::Time NextTime;
  TimerHandleFunction Handler;
  int DelaySecond;
  int DurationSecond;
  int CallCountTotal;
  int CallCountCur;
  uint64 SN;
};

class TimerComponent : public Entity<TimerComponent>, public IAwakeSystem<> {
public:
  void Awake() override;
  void BackToPool() override;

  uint64 Add(int total, int durations, bool immediateDo,
             int immediateDoDelaySecond, TimerHandleFunction handler);
  void Remove(std::list<uint64> &timers);

  bool CheckTime();
  Timer PopTimeHeap();
  void Update();

protected:
  void Add(Timer &data);

private:
  std::vector<Timer> _heap;
};