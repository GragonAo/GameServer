#include "global_robots.h"
#include <mutex>

size_t GlobalRobots::GetRobotsCount() {
  std::lock_guard<std::mutex> guard(_mtx);
  return _robotsCnt;
}
void GlobalRobots::SetRobotsCount(size_t count) {
  std::lock_guard<std::mutex> guard(_mtx);
  _robotsCnt = count;
}
void GlobalRobots::CleanRobotsCount() {
  std::lock_guard<std::mutex> guard(_mtx);
  _robotsCnt = 0;
}