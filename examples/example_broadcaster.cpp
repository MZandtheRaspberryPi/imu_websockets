#include <chrono>
#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "imu_msgs.pb.h"
#include "websocket_broadcaster.h"

/* Set the delay between fresh samples */
const uint16_t PUB_DELAY_MS = 100;
bool EXIT_FLAG = false;

void my_signal_handler(int s) {
  printf("Caught signal %d\n", s);
  EXIT_FLAG = true;
}

void setup_sigint_handler(struct sigaction &sig_int_handler) {
  sig_int_handler.sa_handler = my_signal_handler;
  sigemptyset(&sig_int_handler.sa_mask);
  sig_int_handler.sa_flags = 0;
  sigaction(SIGINT, &sig_int_handler, NULL);
}

void write_vect_to_triad(const std::vector<double>& vect, imu_msgs::Triad* triad)
{
  triad->set_x(vect[0]);
  triad->set_y(vect[1]);
  triad->set_z(vect[2]);
  return;
}

void delay(int64_t sleep_ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
}

int main(int argc, char *argv[]) {

  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  struct sigaction sig_int_handler;
  setup_sigint_handler(sig_int_handler);

  BroadcastServer broadcast_server = BroadcastServer();
  broadcast_server.start_listening(9000);

  while (!EXIT_FLAG) {

    uint64_t start_time = 1234;

    imu_msgs::ImuMsg imu_msg;
    imu_msg.set_timestamp(start_time);

    // could add VECTOR_ACCELEROMETER, VECTOR_MAGNETOMETER,VECTOR_GRAVITY...
    std::vector<double> sensor_data{1.1, 1.3, 1.5};
    imu_msgs::Triad *euler_angles = imu_msg.mutable_euler_angles();
    write_vect_to_triad(sensor_data, euler_angles);

    std::string debug_str = imu_msg.DebugString();
    std::cout << debug_str << std::endl;

    size_t msg_size = imu_msg.ByteSizeLong();
    uint8_t *msg_arr = new uint8_t[msg_size];
    imu_msg.SerializeToArray(msg_arr, msg_size);
    broadcast_server.send_message(msg_arr, msg_size);
    delete[] msg_arr;

    delay(PUB_DELAY_MS);
  }

  return 0;
}
