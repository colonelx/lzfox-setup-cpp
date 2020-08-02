#include "clock.h"
#include "lzfoxwindow.h"

Clock::Clock() : m_mutex(), stopped(false) {}

void Clock::stop()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  stopped = true;
}

bool Clock::hasStopped() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return stopped;
}

void Clock::start(LZFoxWindow* caller)
{
    while(!stopped){
        std::lock_guard<std::mutex> lock(m_mutex);
        caller->updateClock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}