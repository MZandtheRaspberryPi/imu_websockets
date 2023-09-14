#pragma once

#include <chrono>
#include <thread>

#include "imu_msgs.pb.h"

void delay(int64_t sleep_ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
}

uint64_t get_timestamp() {
  std::chrono::milliseconds ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  return ms.count();
}


bool is_calibrated(const imu_msgs::ImuMsg& msg)
{
  bool calibrated = true;
  uint32_t calibration_metric = 0;
  if (msg.has_system_calibration())
  {
    calibration_metric = msg.system_calibration();
    calibrated &= (calibration_metric == 3 || calibration_metric == 2);
  }

  if (msg.has_gyro_calibration())
  {
    calibration_metric = msg.gyro_calibration();
    calibrated &= (calibration_metric == 3);
  }

  if (msg.has_accel_calibration())
  {
    calibration_metric = msg.accel_calibration();
    calibrated &= (calibration_metric == 3 || calibration_metric == 2);
  }

  if (msg.has_mag_calibration())
  {
    calibration_metric = msg.mag_calibration();
    calibrated &= (calibration_metric == 3);
  }

  return is_calibrated;
}