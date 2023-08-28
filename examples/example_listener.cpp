#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <chrono>
#include <cstdlib>
#include <iostream>

#include "imu_msgs.pb.h"
#include "util.h"
#include "websocket_listener.h"

const uint16_t LOOP_DELAY_MS = 10;
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

int main(int argc, char *argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  struct sigaction sig_int_handler;
  setup_sigint_handler(sig_int_handler);

  ListenerClient listener_server = ListenerClient();
  std::string ip_address = "broadcaster";
  uint16_t port = 9000;
  listener_server.connect(ip_address, port);
  listener_server.start_non_blocking();

  while (!EXIT_FLAG) {
    if (listener_server.has_msg()) {
      imu_msgs::ImuMsg msg = listener_server.get_msg();
      if (!msg.IsInitialized()) {
        continue;
      }
      std::string debug_str = msg.DebugString();
      std::cout << debug_str << std::endl;
    }

    delay(LOOP_DELAY_MS);
  }

  listener_server.stop();

  return 0;
}
