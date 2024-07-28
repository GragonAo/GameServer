#pragma once
#include "common.h"

namespace timeutil {

// 定义 Time 类型为无符号 64 位整数
typedef uint64 Time;

// 将时间值增加指定的秒数，单位为毫秒
inline Time AddSeconds(Time timeValue, int second) {
  return timeValue + second * 1000;
}

// 将时间值增加指定的毫秒数
inline Time AddMilliseconds(Time timeValue, int milliseconds) {
  return timeValue + milliseconds;
}

} // namespace timeutil
