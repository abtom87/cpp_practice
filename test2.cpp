#include <iostream>
#include <mutex>
#include <thread>

std::mutex coutMutex; // Mutex to synchronize output to avoid interleaving
std::uint32_t gCount = 1;

// Function to increment a counter
void counter(int id, int maxCount) {
  for (int i = 1; i <= maxCount; ++i) {
    // Lock mutex to ensure safe access to std::cout
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "Thread " << id << " - Count: " << i << std::endl;
    std::cout << "Global var: " << gCount++ << std::endl;
  }
}

int main() {
  const int maxCount = 10;

  // Create multiple threads
  std::thread t1(counter, 1, maxCount);
  std::thread t2(counter, 2, maxCount);
  std::thread t3(counter, 3, maxCount);

  // Wait for threads to complete
  t1.join();
  t2.join();
  t3.join();

  std::cout << "All threads finished counting.\n";
  return 0;
}
