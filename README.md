# CppTimer
It is the simple static C++ timer library based on boost thread to make your life easier. The CppTimer provides a high-level programming interface for timers.

## How to build:
Install [boost](www.boost.org) libraries. Pay attention to the version of installed libraries and version of `thread` library in submodule. They must be equal (if it's not, choose the correct version in the `thread` submodule).

```cmd
 git clone https://github.com/Humminghead/CppTimer.git
 cd CppTimer
 git submodule init 
 git submodule update
 cmake CMakeLists.txt 
 make
 make install
```

## How to use:
Create simple cpp project with `cmake`. Add in to your `cmake` next lines:

```cmake
target_link_libraries(${PROJECT_NAME} PRIVATE CppTimer pthread)
```
and replace the `main.cpp` lines with the following text:

```cpp

#include <CppTimer/timer.h>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std;

std::string format(int v)
{
    return v > 9 ? std::to_string(v) : "0" + std::to_string(v);
}

std::string time()
{
    std::time_t t = std::time(0);  // get time now
    std::tm* now = std::localtime(&t);
    return format(now->tm_hour) + "-" + format(now->tm_min) + "-" + format(now->tm_sec);
}

int main()
{
    std::cout << time() << ": start" << std::endl;
    CppTimer::CppTimer timer(1, [] { std::cout << time() << ": event" << std::endl; });
    timer.run();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    timer.stop();
    std::cout << time() << " :stop" << std::endl;
    return 0;
}
```
Out:

```cmd
12-00-20: start
12-00-21: event
12-00-22: event
12-00-23: event
12-00-23 :stop
```
