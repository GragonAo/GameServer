#include "timer_component.h"
#include "common.h"
#include "global.h"
#include "update_component.h"
#include "util_time.h"
#include <algorithm>
#include <utility>

struct CompareTimer {
  constexpr bool operator()(const Timer &_Left, const Timer &_Right) const {
    return (_Left.NextTime > _Right.NextTime);
  }
};

void TimerComponent::Add(Timer &data) {
  _heap.emplace_back(data);
  if (_heap.size() == 1) {
    std::make_heap(_heap.begin(), _heap.end(), CompareTimer());
  } else {
    std::push_heap(_heap.begin(), _heap.end(), CompareTimer());
  }
}

void TimerComponent::Awake() {
  auto pUpdateComponent = AddComponent<UpdateComponent>();
  pUpdateComponent->UpdateFunction = BindFunP0(this, &TimerComponent::Update);
}

void TimerComponent::BackToPool() { _heap.clear(); }

uint64 TimerComponent::Add(int total, int durations, bool immediateDo,
                           int immediateDoDelaySecond,
                           TimerHandleFunction handler) {
  Timer data;
  data.SN = Global::GetInstance()->GenerateSN();
  data.CallCountCur = 0;
  data.CallCountTotal = total;
  data.DurationSecond = durations;
  data.Handler = std::move(handler);
  data.NextTime =
      timeutil::AddSeconds(Global::GetInstance()->TimeTick, durations);

  if (immediateDo) {
    data.NextTime = timeutil::AddSeconds(Global::GetInstance()->TimeTick,
                                         immediateDoDelaySecond);
  }
  
  Add(data);
  return data.SN;
}

void TimerComponent::Remove(std::list<uint64> &timers) {
  for (auto sn : timers) {
    auto iter = std::find_if(_heap.begin(), _heap.end(),
                             [sn](const Timer &one) { return one.SN == sn; });
    if (iter == _heap.end())
      return;
    _heap.erase(iter);
  }
}

bool TimerComponent::CheckTime() {
  if (_heap.empty())
    return false;
  const auto data = _heap.front();
  return data.NextTime <= Global::GetInstance()->TimeTick;
}

Timer TimerComponent::PopTimeHeap() {
  pop_heap(_heap.begin(), _heap.end(), CompareTimer());

  Timer data = _heap.back();
  _heap.pop_back();

  return data;
}

void TimerComponent::Update() {
  while (CheckTime()) {
    Timer data = PopTimeHeap();
    data.Handler();
    if (data.CallCountTotal != 0)
      data.CallCountCur++;
    if (data.CallCountTotal != 0 && data.CallCountCur >= data.CallCountTotal) {

    } else {
      data.NextTime = timeutil::AddSeconds(Global::GetInstance()->TimeTick,
                                           data.DurationSecond);
      Add(data);
    }
  }
}

/*

时间堆
实现方式：基于二叉堆的数据结构，堆顶元素是最小的，因此每次取出堆顶元素都能获取到最接近超时的定时任务。
时间复杂度：插入和删除操作的时间复杂度都是 O(log n)，n 是堆中元素的数量。
适用场景：适用于定时任务数量较多且分布较为密集的场景，尤其是在时间跨度较大的情况下，依然能保持较好的性能。

时间轮算法
实现方式：基于环形数组，将时间划分为多个槽，每个槽代表一个时间片。在每个时间片到达时，只需检查对应槽内的任务并执行。
时间复杂度：插入操作的时间复杂度是
O(1)，执行到期任务的时间复杂度取决于槽内任务数量。
适用场景：适用于定时任务数量较多，但时间分布较为均匀的场景。特别是在处理大量短时间间隔的定时任务时，能显著减少时间复杂度。


时间堆 vs 时间轮
效率：时间堆的效率取决于堆的大小，时间复杂度为 O(log
n)。时间轮在任务分布较均匀时插入和执行的效率更高，为 O(1)。
复杂度：时间轮实现较复杂，尤其是在处理任务时间跨度较大或任务数量不均匀时，需要对任务进行再分配。
灵活性：时间堆更适合灵活性要求高的场景，可以轻松处理不同时长的定时任务。而时间轮更适合定时任务密集、时间间隔相对固定的情况。
*/