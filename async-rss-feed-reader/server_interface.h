#pragma once

#include <cstdint>
#include "message.h"
#include "connection.h"

namespace rssreader{
    namespace networkcore{
        template <typename T>
        class ServerInterface{
            public:
                ServerInterface(uint16_t port){}
                virtual ~ServerInterface(){}
                bool Start(){}
                void Stop(){}
                void WaitForClientConnection(){}
                void OnClientValidate(std::shared_ptr<Connection<T>> client){}
                void MessageClient(std::shared_ptr<T>, const Message<T>& msg){}
                void Broadcast(const Message<T>& msg){}
                void Update(){}

            protected:
                virtual bool OnClientConnect(std::shared_ptr<Connection<T>> client){}
                virtual bool OnClientDisconnect(std::shared_ptr<Connection<T>> client){}    
                virtual bool OnMessage(std::shared_ptr<Connection<T>> client, Message<T>& msg){}    
            protected:
                TSQueue<AssociatedMessage<T>> m_in_queue;
                std::deque<std::shared_ptr<Connection<T>>> m_conn_queue;
                asio::io_context m_asio_context;
                std::thread m_thread_context;
                asio::ip::tcp::acceptor m_asio_accpetor;
                uint32_t m_client_identifier = 10000; // TODO: This has to be random for multiserver.

        };
    };
};