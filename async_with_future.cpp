#include <future>
#include <string>
#include <iostream>

void printSomeData(const std::string &data, int iterations) {
  for (; iterations >= 0; iterations--) {
    std::cout << data << std::endl;
  }
}

int main() {
  std::future<void> thread(std::async(std::launch::async, printSomeData, "thread 1", 10));
  std::future<void> thread1(std::async(std::launch::async, printSomeData, "thread 2", 20));
  return 0;
}
