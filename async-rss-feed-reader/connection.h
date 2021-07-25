#pragma once

#define ASIO_STANDALONE
#include "ts_queue.h"
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

namespace rssreader {
namespace networkcore {

template <typename T> class ServerInterface;
template <typename T> class Message;
template <typename T> class AssociatedMessage;
template <typename T> class MessageHeader;

template <typename T>
class Connection : public std::enable_shared_from_this<Connection<T>> {
public:
  enum class Owner { server, client };

  Connection(Owner parent, asio::io_context &asio_context,
             asio::ip::tcp::socket socket,
             TSQueue<AssociatedMessage<T>> &input_q)
             : m_asio_context(asio_context),m_socket(std::move(socket)),m_in_queue(input_q){
    m_owner = parent;
    if (m_owner == Owner::server) {
      m_handshake_out =
          uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
      m_handshake_check = Scramble(m_handshake_out);
    }
  }

  virtual ~Connection() {}

  uint32_t GetID() const { return m_id; }

  void ConnectToClient(rssreader::networkcore::ServerInterface<T> *server,
                       uint32_t uid = 0) {
    if (m_owner == Owner::server) {
      if (m_socket.is_open()) {
        m_id = uid;
        WriteValidation();
        ReadValidation(server);
      }
    }
  }

  void ConnectToServer(const asio::ip::tcp::resolver::results_type &endpoints) {
    if (m_owner == Owner::client) {
      asio::async_connect(
          m_socket, endpoints,
          [this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
            if (!ec)
              ReadValidation();
          });
    }
  }

  bool IsConnected() { return m_socket.is_open(); }

  void Disconnect() {
    if (IsConnected())
      asio::post(m_asio_context, [this]() { m_socket.close(); });
  }

  void Send(const Message<T> &msg) {
    asio::post(m_asio_context, [this, msg]() {
      bool writing_in_progress = !m_out_queue.empty();
      m_out_queue.push_back(msg);
      if (writing_in_progress)
        WriteHeader;
    });
  }

private:
  void WriteHeader() {
    asio::async_write(
        m_socket,
        asio::buffer(&m_out_queue.front().m_header, sizeof(MessageHeader<T>))
        ,[this](std::error_code ec, std::size_t length) {
          if (!ec) {
            if (m_out_queue.front().m_body.size() > 0) {
              WriteBody();
            } else {
              m_out_queue.pop_front();
              if (!m_out_queue.empty())
                WriteHeader();
            }
          } else {
            std::cout << "[" << m_id << "] Write Header Fail.\n";
            m_socket.close();
          }
        });
  }

  void WriteBody() {
    asio::async_write(m_socket,
                      asio::buffer(m_out_queue.front().m_body.data(),
                                   m_out_queue.front().m_body.size()),
                      [this](std::error_code ec, std::size_t length) {
                        if (!ec) {
                          m_out_queue.pop_front();
                          if (!m_out_queue.empty())
                            WriteHeader();
                        } else {
                          std::cout << "[" << m_id << "] Write Body Failed.\n";
                          m_socket.close();
                        }
                      });
  }

  void ReadHeader() {
    asio::async_read(
        m_socket,
        asio::buffer(&m_transient_message.m_header, sizeof(MessageHeader<T>)),
        [this](std::error_code ec, std::size_t length) {
          if (!ec) {
            if (m_transient_message.m_header.size > 0) {
              m_transient_message.m_body.resize(
                  m_transient_message.m_header.size);
              ReadBody();
            } else {
              Enqueue();
            }
          } else {
            std::cout << "[" << m_id << "] Read Header Failed.\n";
            m_socket.close();
          }
        });
  }

  void ReadBody() {
      asio::async_read(m_socket
      ,asio::buffer(m_transient_message.m_body.data(),m_transient_message.m_body.size())
      ,[this](std::error_code ec, std::size_t length){
          if(!ec){
              Enqueue();
          }else{
            std::cout << "[" << m_id << "] Read Body Failed.\n";
            m_socket.close();
          }
      });
  }

  // Some random bit magic as an illustration for the handshake.
  uint64_t Scramble(uint64_t input) {
    uint64_t out = input ^ 0xDEADBEEFC0DECAFE;
    out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F) << 4;
    return out ^ 0xC0DEFACE12345678;
  }

  // Faclitates a simple handshake mechanism
  void ReadValidation(ServerInterface<T> *server = nullptr) {
      asio::async_read(m_socket
      ,asio::buffer(&m_handshake_in, sizeof(uint64_t))
      ,[this,server](std::error_code ec,std::size_t length){
          if(!ec){
              if(m_owner == Owner::server){
                  if(m_handshake_in == m_handshake_check){
                      std::cout << "Client Validated successfully.\n";
                      server->OnClientValidated(this->shared_from_this());
                      ReadHeader();
                  }
              }else{
                  std::cout << "Client Failed Validation. Terminating connection.\n";
                  m_socket.close();
              }
          }else{
              m_handshake_out = Scramble(m_handshake_in);
              WriteValidation();
          }
      });
  }
  
  // Faclitates a simple handshake mechanism
  void WriteValidation() {
      asio::async_write(m_socket
      ,asio::buffer(&m_handshake_out,sizeof(uint64_t))
      ,[this](std::error_code ec, std::size_t length){
          if(!ec){
              if(m_owner == Owner::server)
                ReadHeader();
          }else{
              m_socket.close();
          }
      });
  }

  void Enqueue() {
    if (m_owner == Owner::server)
      m_in_queue.push_back({this->shared_from_this(), m_transient_message});
    else
      m_in_queue.push_back({nullptr, m_transient_message});

    ReadHeader();
  }

protected:
  asio::io_context& m_asio_context;
  asio::ip::tcp::socket m_socket;
  TSQueue<rssreader::networkcore::Message<T>> m_out_queue;
  TSQueue<AssociatedMessage<T>>& m_in_queue;
  Message<T> m_transient_message;
  Owner m_owner = Owner::server;
  uint32_t m_id = 0;

  uint64_t m_handshake_out = 0;
  uint64_t m_handshake_in = 0;
  uint64_t m_handshake_check = 0;
};
}; // namespace networkcore
}; // namespace rssreader
