#include <iostream>
#include <thread>

int main() {
  unsigned int numThreads = std::thread::hardware_concurrency();
  std::cout << numThreads << std::endl;
  return 0;
}
