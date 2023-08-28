#pragma once

#include <chrono>
#include <thread>

void delay(int64_t sleep_ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
}