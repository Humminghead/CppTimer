#include "timerbase.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>

namespace CppTimer {
struct TimerBase::Impl {
  Impl(const size_t seconds) : m_Interval(seconds), m_Timer(m_Io, m_Interval) {}
  boost::asio::io_service m_Io{};
  boost::posix_time::seconds m_Interval{0}; // 1 second
  boost::asio::deadline_timer m_Timer;
  std::function<TimerBase::CallBackType> m_Callback;

  void keep(const boost::system::error_code &);
};

TimerBase::TimerBase(const size_t seconds)
    : m_Impl(std::make_unique<TimerBase::Impl>(seconds)) {}

void TimerBase::Impl::keep(const boost::system::error_code &e) {
  if (m_Callback)
    m_Callback(e.message());
  m_Timer.expires_at(m_Timer.expires_at() + m_Interval);
  m_Timer.async_wait(
      [&](const boost::system::error_code &e) { this->keep(e); });
}

void TimerBase::run() {
  if (m_Impl->m_Interval.total_seconds() == 0)
    return;

  if (!m_Impl->m_Io.stopped()) {
    m_Impl->m_Timer.async_wait(
        [&](const boost::system::error_code &e) { m_Impl->keep(e); });
    boost::thread(static_cast<std::size_t (boost::asio::io_service::*)()>(
                      &boost::asio::io_service::run),
                  &m_Impl->m_Io);
  }
}

void TimerBase::stop() {
  if (!m_Impl->m_Io.stopped()) {
    m_Impl->m_Io.stop();
  }
}

void TimerBase::setCallback(std::function<CallBackType> &&cb) {
  m_Impl->m_Callback = std::move(cb);
}

void TimerBase::callOnce() {
  if (!m_Impl->m_Io.stopped()) {
    m_Impl->m_Timer.async_wait(
        [&](const boost::system::error_code &ec) { m_Impl->m_Callback(ec.message()); });
    m_Impl->m_Io.run();
  }
}

TimerBase::~TimerBase() {
  stop();
  m_Impl->m_Io.reset();
};

} // namespace CppTimer
