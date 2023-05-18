#include <memory>
#include <functional>
#include <string_view>

namespace CppTimer {
class TimerBase {
  struct Impl;
  std::unique_ptr<Impl> m_Impl;

public:
  using CallBackType = void(const std::string_view ec);

  TimerBase(const size_t seconds);
  ~TimerBase();

  void run();

  void stop();

  void setCallback(std::function<CallBackType> &&cb);

  void callOnce();

  bool isStoped();
};
} // namespace CppTimer
