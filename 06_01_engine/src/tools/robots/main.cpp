#include "global_robots.h"
#include "robots_app.h"
int main(int argc, char *argv[]) {
  GlobalRobots::Instance();
  return MainTemplate<RobotsApp>();
}