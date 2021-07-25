#pragma once

#include "connection.h"
#include "message.h"
#include <cstdint>

namespace rssreader {
namespace networkcore {
template <typename T> class ServerInterface {
public:
  ServerInterface(uint16_t port)
      : m_asio_acceptor(m_asio_context,
                        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

  virtual ~ServerInterface() { Stop(); }

  bool Start() {
    try {
      WaitForClientConnection();
      m_thread_context = std::thread([this]() { m_asio_context.run(); });
    } catch (std::exception &e) {
      std::cerr << "[SERVER] Exception: " << e.what() << "\n";
      return false;
    }

    std::cout << "[SERVER] Started!\n";
    return true;
  }

  void Stop() {
    m_asio_context.stop();
    if (m_thread_context.joinable())
      m_thread_context.join();
    std::cout << "[SERVER] Stopped.\n";
  }

  void WaitForClientConnection() {
    m_asio_acceptor.async_accept([this](std::error_code ec,
                                        asio::ip::tcp::socket socket) {
      if (!ec) {
        std::cout << "[SERVER] New connection" << socket.remote_endpoint()
                  << "\n";
        std::shared_ptr<Connection<T>> new_conn =
            std::make_shared<Connection<T>>(Connection<T>::Owner::server,
                                            m_asio_context, std::move(socket),
                                            m_in_queue);
        if (OnClientConnect(new_conn)) {
          m_conn_queue.push_back(std::move(new_conn));
          m_conn_queue.back()->ConnectToClient(this, m_client_identifier++);
          std::cout << "[" << m_conn_queue.back()->GetID()
                    << "] Connection Approved\n";
        } else {
          std::cout << "[XXXX] Connection Denied";
        }
      } else {
        std::cout << "[SERVER] New Connection ERROR : " << ec.message() << "\n";
      }
      WaitForClientConnection();
    });
  }

  void MessageClient(std::shared_ptr<Connection<T>> client,
                     const Message<T> &msg) {
    if (client && client->IsConnected()) {
      client->Send(msg);
    } else {
      OnClientDisconnect(client);
      client.reset();
      m_conn_queue.erase(
          std::remove(m_conn_queue.begin(), m_conn_queue.end(), client),
          m_conn_queue.end());
    }
  }

  void Broadcast(const Message<T> &msg,
                 std::shared_ptr<Connection<T>> ignore_client = nullptr) {
    bool invalid_client_exists = false;
    for (auto &client : m_conn_queue) {
      if (client && client->IsConnected()) {
        if (client != ignore_client)
          client->Send(msg);
      } else {
        OnClientDisconnect(client);
        client.reset();
        invalid_client_exists = true;
      }
    }
    if (invalid_client_exists)
      m_conn_queue.erase(
          std::remove(m_conn_queue.begin(), m_conn_queue.end(), nullptr),
          m_conn_queue.end());
  }

  void Update(size_t max_messages = -1, bool should_wait = false) {
    if (should_wait)
      m_in_queue.wait();

    size_t msg_count = 0;
    while (msg_count < max_messages && m_in_queue.empty()) {
      auto msg = m_in_queue.pop_front();
      OnMessage(msg.remote, msg.m_message);
      msg_count++;
    }
  }

  virtual void OnClientValidated(std::shared_ptr<Connection<T>> client) {}

protected:
  virtual bool OnClientConnect(std::shared_ptr<Connection<T>> client) {
    return false;
  }
  virtual void OnClientDisconnect(std::shared_ptr<Connection<T>> client) {}
  virtual void OnMessage(std::shared_ptr<Connection<T>> client,
                         Message<T> &msg) {}

protected:
  TSQueue<AssociatedMessage<T>> m_in_queue;
  std::deque<std::shared_ptr<Connection<T>>> m_conn_queue;
  asio::io_context m_asio_context;
  std::thread m_thread_context;
  asio::ip::tcp::acceptor m_asio_acceptor;
  uint32_t m_client_identifier =
      12345; // TODO: This has to be random for multiserver.
};
}; // namespace networkcore
}; // namespace rssreader