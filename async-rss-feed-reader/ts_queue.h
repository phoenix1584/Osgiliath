#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>

namespace rssreader{
    namespace networkcore{
        template<typename T>
        class tsqueue{
            public:
                tsqueue() = default;
                tsqueue (const tsqueue<T>&) = delete;
                virtual ~tsqueue() {clear();}

                const T& front(){}
                const T& back(){}
                T pop_front(){}
                T pop_back(){}
                T push_back(){}
                T push_front(){}
                bool empty(){}
                size_t count(){}
                void clear(){}
            protected:
                std::mutex m_mux_queue;
                std::deque<T> m_deq;
                std::condition_variable m_cv_blocking;
                std::mutex m_mux_blocking;
        };

    };
};
