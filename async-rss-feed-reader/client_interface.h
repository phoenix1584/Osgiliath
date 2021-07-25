#pragma once

#include <cstdint>
#include "message.h"
#include "connection.h"

namespace rssreader{
    namespace networkcore{
        template <typename T>
        class ClientInterface{
            public: 
                ClientInterface(){}
                virtual ~ClientInterface(){}
                bool Connect(const std::string& host, const uint16_t port){}
                void Disconnect(){}
                bool IsConnected(){}
                void Send(){}
                TSQueue<AssociatedMessage<T>>& Incoming(){}
            protected:
                asio::io_context m_asio_context;
                std::thread m_thread_context;
                std::unique_ptr<Connection<T>> m_connection;
            private:
                TSQueue<AssociatedMessage<T>> m_queue_in;
        };        
    };
};