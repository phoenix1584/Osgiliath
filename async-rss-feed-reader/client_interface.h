#include <cstdint>
#include <iostream>
#include "connection.h"
#include "message.h"


namespace rssreader {
namespace networkcore {
template <typename T> class ClientInterface {
public:
  ClientInterface() {}

  virtual ~ClientInterface() { Disconnect(); }

  bool Connect(const std::string &host, const uint16_t port) {
    try {
      asio::ip::tcp::resolver resolver(m_asio_context);
      asio::ip::tcp::resolver::results_type endpoints =
          resolver.resolve(host, std::to_string(port));
      m_connection = std::make_unique<Connection<T>>(
          Connection<T>::Owner::client, m_asio_context,
          asio::ip::tcp::socket(m_asio_context), m_in_queue);
      m_connection->ConnectToServer(endpoints);
    } catch (std::exception &e) {
      std::cerr << "Client exception: " << e.what() << "\n";
      return false;
    }
    return true;
  }

  void Disconnect() {
    if (IsConnected())
      m_connection->Disconnect();

    m_asio_context.stop();
    if (m_thread_context.joinable())
      m_thread_context.join();

    m_connection.release();
  }

  bool IsConnected() {
    return m_connection ? m_connection->IsConnected() : false;
  }

  void Send(const Message<T> &message) {
    if (IsConnected())
      m_connection->Send(message);
  }

  TSQueue<AssociatedMessage<T>>& GetQueue() { return m_in_queue; }

protected:
  asio::io_context m_asio_context;
  std::thread m_thread_context;
  std::unique_ptr<Connection<T>> m_connection;

private:
  TSQueue<AssociatedMessage<T>> m_in_queue;
};
}; // namespace networkcore
}; // namespace rssreader