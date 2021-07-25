#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <cstring>

namespace rssreader{
    namespace protocol{
        template<typename T>
        struct MessageHeader{
            T m_id{};
            uint32_t size = 0;
        };       
        
        template <typename T>
        struct Message{
            MessageHeader<T> m_header{};
            std::vector<uint8_t> m_body;

            size_t size() const{ 
                return m_body.size();
            }

            friend std::ostream& operator<<(std::ostream& os, const Message<T>& msg){
                os << "ID:" << int(msg.m_header.id) << "Size:" << msg.m_header.size;
                return os;
            }

            template <typename DataType>
            friend Message<T>& operator<< ( Message<T>& msg, const DataType& data){
                static_assert(std::is_standard_layout<DataType>::value, 
                                " Non POD data types are not supported.");
                size_t t = msg.m_body.size();
                msg.m_body.resize(msg.m_body.size() + sizeof(DataType));
                msg.m_header.size = msg.size();
                return msg;
            }

            template <typename DataType>
            friend Message<T>& operator<< ( Message<T>& msg, DataType& data){
                static_assert(std::is_standard_layout<DataType>::value, 
                                " Non POD data types are not supported.");
                size_t t = msg.m_body.size() - sizeof(DataType);
                std::memcpy(&data, msg.m_body.data() + t , sizeof(DataType));
                return msg;
            }
        };

    }
}
