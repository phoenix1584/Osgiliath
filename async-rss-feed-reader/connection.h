#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include "ts_queue.h"

namespace rssreader{
    namespace networkcore{
        template <typename T>
        class ServerInterface;

        template <typename T>
        class Message;

        template <typename T>
        class AssociatedMessage;

        template <typename T>
        class Connection : public std::enable_shared_from_this<Connection<T>>{
            public:
                enum class Owner{ 
                    server,
                    client
                };

                Connection(Owner parent 
                    ,asio::io_context& asio_context
                    ,asio::ip::tcp::socket socket
                    ,TSQueue<AssociatedMessage<T>>& input_q)
                    {
                      
                    }

            private:
                void WriteHeader(){}
                void WriteBody(){}
                void ReadHeader(){}
                void ReadBody(){}
                uint64_t Scramble(uint64_t input){}
                void ReadValidation(ServerInterface<T>* server = nullptr){}
                void Enqueue(){}

            protected:
                asio::ip::tcp::socket m_socket;
                asio::io_context& m_context;
                TSQueue<rssreader::networkcore::Message<T>> m_out_queue;
                TSQueue<AssociatedMessage<T>>& m_in_queue;
                Message<T> m_transient_message;
                Owner m_owner = Owner::server;
                uint32_t m_id = 0;

                uint64_t m_handshake_out = 0;
                uint64_t m_handshake_in = 0;
                uint64_t m_handshake_check = 0;
        };
    };
};
