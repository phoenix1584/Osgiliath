#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>

namespace rssreader {
namespace networkcore {
template <typename T> class TSQueue {
public:
  TSQueue() = default;
  TSQueue(const TSQueue<T> &) = delete;
  virtual ~TSQueue() { clear(); }

  const T &front() {
    std::scoped_lock lock(m_mux_queue);
    return m_deq.front();
  }

  const T &back() {
    std::scoped_lock lock(m_mux_queue);
    return m_deq.back();
  }

  T pop_front() {
    std::scoped_lock lock(m_mux_queue);
    auto t = std::move(m_deq.front());
    m_deq.pop_front();
    return t;
  }

  T pop_back() {
    std::scoped_lock lock(m_mux_queue);
    auto t = std::move(m_deq.back());
    m_deq.pop_back();
    return t;
  }

  void push_back(const T &item) {
    std::scoped_lock lock(m_mux_queue);
    m_deq.emplace_back(std::move(item));
    std::unique_lock<std::mutex> ul(m_mux_blocking);
    m_cv_blocking.notify_one();
  }

  void push_front(const T &item) {
    std::scoped_lock lock(m_mux_queue);
    m_deq.emplace_front(std::move(item));
    std::unique_lock<std::mutex> ul(m_mux_blocking);
    m_cv_blocking.notify_one();
  }

  bool empty() {
    std::scoped_lock lock(m_mux_queue);
    return m_deq.empty();
  }

  size_t count() {
    std::scoped_lock lock(m_mux_queue);
    return m_deq.size();
  }

  void clear() {
    std::scoped_lock lock(m_mux_queue);
    m_deq.clear();
  }

  void wait() {
    while (empty()) {
      std::unique_lock<std::mutex> ul(m_mux_blocking);
      m_cv_blocking.wait(ul);
    }
  }

protected:
  std::mutex m_mux_queue;
  std::deque<T> m_deq;
  std::condition_variable m_cv_blocking;
  std::mutex m_mux_blocking;
};

}; // namespace networkcore
}; // namespace rssreader
