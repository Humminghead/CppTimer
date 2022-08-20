#include <boost/system/error_code.hpp>
#include <functional>
#include <thread>

namespace CppTimer {
class TimerBase {
  struct Impl;
  std::unique_ptr<Impl> m_Impl;

public:
  using CallBackType = void(const boost::system::error_code &ec);

  TimerBase(const size_t seconds);
  ~TimerBase();

  void run();

  void stop();

  void setCallback(std::function<CallBackType> &&cb);

  void callOnce();

private:
  void keep(const boost::system::error_code &);
};
} // namespace CppTimer
