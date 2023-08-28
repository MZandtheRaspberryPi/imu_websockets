#pragma once

#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class BroadcastServer {
public:
    BroadcastServer() {
        m_server.init_asio();

        m_server.set_open_handler(bind(&BroadcastServer::on_open,this,::_1));
        m_server.set_close_handler(bind(&BroadcastServer::on_close,this,::_1));
    }

    void on_open(connection_hdl hdl) {
        m_connections.insert(hdl);
    }

    void on_close(connection_hdl hdl) {
        m_connections.erase(hdl);
    }

    void start_listening(uint16_t port) {
        m_server.listen(port);
        m_server.start_accept();
    }

    void send_message(const uint8_t* byte_arr_ptr, const size_t& num_bytes)
    {
        for (auto it : m_connections) {
            m_server.send(it, byte_arr_ptr, num_bytes, websocketpp::frame::opcode::BINARY);
        }
    }
private:
    typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

    server m_server;
    con_list m_connections;
};
