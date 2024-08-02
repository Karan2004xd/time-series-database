#include <future>
#include <string>
#include <iostream>

long long int result = 0, test_num = 1000000000;

void printSomeData(const std::string &data, long long int iterations) {
  for (; iterations >= 0; iterations--) {
    result++;
  }
}

int main() {
  std::future<void> thread1(std::async(std::launch::async, printSomeData, "thread 2", test_num));
  std::future<void> thread(std::async(std::launch::async, printSomeData, "thread 1", test_num / 2));

  /* printSomeData("", test_num); */

  /* while (result < test_num) {} */
  std::cout << result << std::endl;
  return 0;
}
