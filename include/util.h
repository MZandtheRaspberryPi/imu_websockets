#pragma once

#include <chrono>
#include <iostream>
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

bool is_calibrated(const imu_msgs::ImuMsg& msg) {
  bool calibrated = true;
  uint32_t calibration_metric = 0;
  if (msg.has_system_calibration()) {
    calibration_metric = msg.system_calibration();
    calibrated &= (calibration_metric == 3 || calibration_metric == 2);
  }

  if (msg.has_gyro_calibration()) {
    calibration_metric = msg.gyro_calibration();
    calibrated &= (calibration_metric == 3);
  }

  if (msg.has_accel_calibration()) {
    calibration_metric = msg.accel_calibration();
    calibrated &= (calibration_metric == 3 || calibration_metric == 2);
  }

  if (msg.has_mag_calibration()) {
    calibration_metric = msg.mag_calibration();
    calibrated &= (calibration_metric == 3);
  }

  return is_calibrated;
}

enum PRINT_MODE {
  ACCEL = 0,
  GYRO = 1,
  MAG = 2,
  ALL_SENSORS = 3,
  BOSCH_EULER = 4,
  CALC_EULER = 5
};

struct PrintSettings {
  PrintSettings()
      : print_msgs(false),
        print_mode(PRINT_MODE::ACCEL),
        messages_per_second(10) {}
  bool print_msgs;
  PRINT_MODE print_mode;
  size_t messages_per_second;
};

void print_accel(const imu_msgs::ImuMsg& msg) {
  std::cout << "linear acceleration" << std::endl;
  std::cout << msg.linear_acceleration().DebugString() << std::endl;
}

void print_gyro(const imu_msgs::ImuMsg& msg) {
  std::cout << "angular acceleration" << std::endl;
  std::cout << msg.angular_acceleration().DebugString() << std::endl;
}

void print_mag(const imu_msgs::ImuMsg& msg) {
  std::cout << "magnetometer" << std::endl;
  std::cout << msg.magnetometer_vector().DebugString() << std::endl;
}

void print_all_sensors(const imu_msgs::ImuMsg& msg) {
  print_accel(msg);
  print_gyro(msg);
  print_mag(msg);
}

void print_euler_bosch(const imu_msgs::ImuMsg& msg) {
  std::cout << "euler angles bosch" << std::endl;
  std::cout << msg.euler_angles().DebugString() << std::endl;
}

void print_euler_calc(const imu_msgs::ImuMsg& msg) {
  std::cout << "euler angles calculated" << std::endl;
  std::cout << msg.euler_angles_filter().DebugString() << std::endl;
}

void print_imu_msg(const imu_msgs::ImuMsg& msg, const PRINT_MODE& print_mode,
                   const size_t& messages_per_second) {
  static uint64_t last_print_time = 0;
  uint64_t ms_between_msg_prints =
      1000. / static_cast<float>(messages_per_second);

  uint64_t cur_timestamp = get_timestamp();

  if (cur_timestamp - last_print_time < ms_between_msg_prints) {
    return;
  }
  last_print_time = cur_timestamp;

  switch (print_mode) {
    case PRINT_MODE::ACCEL:
      print_accel(msg);
      break;
    case PRINT_MODE::GYRO:
      print_gyro(msg);
      break;
    case PRINT_MODE::MAG:
      print_mag(msg);
      break;
    case PRINT_MODE::ALL_SENSORS:
      print_all_sensors(msg);
      break;
    case PRINT_MODE::BOSCH_EULER:
      print_euler_bosch(msg);
      break;
    case PRINT_MODE::CALC_EULER:
      print_euler_calc(msg);
      break;
    default:
      break;
  }
}
