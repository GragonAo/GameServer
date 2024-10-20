#include "client_thread.h"
#include "iostream"
#include <chrono>
#include <cstdlib>
#include <thread>
#include <vector>
int main(int argc, char *argv[]) {
  int threadNum = 1, msgCount = 1;
  if (argc >= 2) {
    threadNum = std::atoi(argv[1]);
  }
  if (argc == 3) {
    msgCount = std::atoi(argv[2]);
  }
  std::vector<ClientThread *> threads(threadNum);
  for (auto i = 0; i < threadNum; i++) {
    threads[i] = new ClientThread(msgCount);
    threads[i]->Start();
  }
  auto start = std::chrono::system_clock::now();
  while (true) {
    if (threads.size() <= 0)
      break;
    auto iter = threads.begin();
    while (iter != threads.end()) {
      ClientThread *pThread = (*iter);
      if (!pThread->IsRun()) {
        pThread->Dispose();
        delete pThread;
        iter = threads.erase(iter);
        continue;
      }
      ++iter;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  auto end = std::chrono::system_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "time:"
            << double(duration.count()) *
                   std::chrono::microseconds::period::num /
                   std::chrono::microseconds::period::den
            << "s" << std::endl;
  threads.clear();
  return 0;
}