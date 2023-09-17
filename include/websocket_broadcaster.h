#pragma once

#include <set>

#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class BroadcastServer {
 public:
  BroadcastServer() {}

  void start_listening(uint16_t port) {
    m_server.init_asio();

    // set up access channels to only log interesting things
    m_server.clear_access_channels(websocketpp::log::alevel::all);
    m_server.set_access_channels(websocketpp::log::alevel::access_core);
    m_server.set_access_channels(websocketpp::log::alevel::app);

    m_server.set_open_handler(bind(&BroadcastServer::on_open, this, ::_1));
    m_server.set_close_handler(bind(&BroadcastServer::on_close, this, ::_1));

    m_server.set_message_handler(
        bind(&BroadcastServer::on_message, this, ::_1, ::_2));
    m_server.listen(port);
    m_server.start_accept();

    thread_.reset(new websocketpp::lib::thread(&server::run, &m_server));
  }

  void stop_listening() {
    m_server.stop_listening();
    for (connection_hdl con_hdl : m_connections) {
      server::connection_ptr con_ptr = m_server.get_con_from_hdl(con_hdl);
      std::cout << "> Closing connection " << con_ptr << std::endl;

      if (con_ptr->get_state() != websocketpp::session::state::open) {
        continue;
      }

      websocketpp::lib::error_code ec;
      m_server.close(con_ptr, websocketpp::close::status::going_away, "", ec);
      if (ec) {
        std::cout << "> Error closing connection " << con_ptr << ": "
                  << ec.message() << std::endl;
      }
    }

    thread_->join();
  }

  void send_imu_msg(const imu_msgs::ImuMsg& msg) {
    size_t msg_size = msg.ByteSizeLong();
    uint8_t* msg_arr = new uint8_t[msg_size];
    msg.SerializeToArray(msg_arr, msg_size);
    broadcast_server.send_message(msg_arr, msg_size);
    delete[] msg_arr;
  }

  void send_message(const uint8_t* byte_arr_ptr, const size_t& num_bytes) {
    for (auto it : m_connections) {
      m_server.send(it, byte_arr_ptr, num_bytes,
                    websocketpp::frame::opcode::BINARY);
    }
  }

 private:
  void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message (" << msg->get_payload().size()
              << "): " << msg->get_payload() << std::endl;
  }

  void on_open(connection_hdl hdl) {
    std::cout << "got open" << std::endl;
    m_connections.insert(hdl);
  }

  void on_close(connection_hdl hdl) { m_connections.erase(hdl); }

  typedef std::set<connection_hdl, std::owner_less<connection_hdl>> con_list;

  server m_server;
  con_list m_connections;
  websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread_;
};
