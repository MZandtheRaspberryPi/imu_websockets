#pragma once

#include <exception>
#include <iostream>
#include <mutex>
#include <queue>
#include <vector>
#include <websocketpp/client.hpp>
#include <websocketpp/common/memory.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/connection.hpp>

#include "imu_msgs.pb.h"
#include "util.h"

using websocketpp::lib::bind;

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef std::vector<client::connection_ptr> con_list;

const size_t MAX_QUEUE_SIZE = 50;

class ListenerClient {
 public:
  ListenerClient() {
    my_client_.init_asio();
    my_client_.set_message_handler(bind(&ListenerClient::on_message, this,
                                        std::placeholders::_1,
                                        std::placeholders::_2));
    running_ = false;
  }
  ~ListenerClient() {
    if (running_) {
      this->stop();
    }
  }

  bool connect(const std::string& ip_address, const uint16_t port) {
    std::string uri = "ws://" + ip_address + ":" + std::to_string(port);

    websocketpp::lib::error_code ec;
    client::connection_ptr con = nullptr;

    con = my_client_.get_connection(uri, ec);
    if (ec.value() != 0) {
      std::cout << "could not create connection because: " << ec.message()
                << std::endl;
    }

    std::cout << "actually doing conn" << std::endl;
    // Note that connect here only requests a connection. No network messages
    // are exchanged until the event loop starts running.

    my_client_.connect(con);
    websocketpp::lib::error_code ec2 = con->get_ec();

    std::cout << "connect did'nt return err, status is " << con->get_state()
              << " ec2 is " << ec2.value() << " " << ec2.message() << std::endl;

    connection_list_.push_back(con);

    // have seen that connection needs time to stabilize or sometimes we get
    // errors queryign state isn't helpful in this time. need to build another
    // thread to monitor statuses and test connections and reset if bad
    /*
    imu_websockets-listener-1     | actually doing conn
    imu_websockets-listener-1     | [2023-08-28 21:43:23] [info] asio
        async_connect error: system:111 (Connection refused)
    imu_websockets-listener-1     | [2023-08-28 21:43:23] [info] Error getting
        remote endpoint: system:107 (Transport endpoint is not connected)
    imu_websockets-listener-1     | [2023-08-28 21:43:23] [error] handle_connect
        error: Underlying Transport Error imu_websockets-listener-1     |
    connect did'nt return err, status is 0
    [2023-08-28 21:43:23] [fail] WebSocket
        Connection Unknown - "" / 0 websocketpp.transport:2 Underlying Transport
        Error
    */
    delay(250);
    std::cout << "con end status " << con->get_state() << std::endl;

    return true;
  }

  void start_non_blocking() {
    if (running_) {
      return;
    }
    running_ = true;
    my_client_.start_perpetual();

    thread_.reset(new websocketpp::lib::thread(&client::run, &my_client_));
  }

  void stop() {
    if (!running_) {
      return;
    }
    my_client_.stop_perpetual();

    std::cout << connection_list_.size() << " connections open" << std::endl;
    for (client::connection_ptr con : connection_list_) {
      std::cout << "> Closing connection " << con << std::endl;

      if (con->get_state() != websocketpp::session::state::open) {
        continue;
      }

      websocketpp::lib::error_code ec;
      my_client_.close(con, websocketpp::close::status::going_away, "", ec);
      if (ec) {
        std::cout << "> Error closing connection " << con << ": "
                  << ec.message() << std::endl;
      }
    }

    thread_->join();
    running_ = false;
  }

  bool has_msg() {
    bool queue_has_msg = false;
    while (!msg_queue_mutex_.try_lock()) {
      delay(10);
    }

    queue_has_msg = msg_queue_.size() > 0;
    msg_queue_mutex_.unlock();
    return queue_has_msg;
  }

  imu_msgs::ImuMsg get_msg() {
    imu_msgs::ImuMsg msg;
    if (!this->has_msg()) {
      return msg;
    }

    while (!msg_queue_mutex_.try_lock()) {
      delay(10);
    }
    msg = msg_queue_.front();
    msg_queue_.pop();
    msg_queue_mutex_.unlock();
    return msg;
  }

 private:
  void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg) {
    imu_msgs::ImuMsg imu_msg;
    const std::string& msg_payload = msg->get_payload();
    imu_msg.ParseFromString(msg_payload.c_str());

    while (!msg_queue_mutex_.try_lock()) {
      delay(10);
    }

    if (msg_queue_.size() >= MAX_QUEUE_SIZE) {
      msg_queue_.pop();
    }

    msg_queue_.push(imu_msg);
    msg_queue_mutex_.unlock();
  }

  std::queue<imu_msgs::ImuMsg> msg_queue_;
  std::mutex msg_queue_mutex_;
  client my_client_;
  websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread_;
  con_list connection_list_;
  bool running_;
};