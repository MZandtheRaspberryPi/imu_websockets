#pragma once

#include <chrono>
#include <thread>

void delay(int64_t sleep_ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
}

uint64_t get_timestamp() {
  std::chrono::milliseconds ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  return ms.count();
}
