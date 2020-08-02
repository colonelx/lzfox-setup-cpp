#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <gtkmm.h>
#include <gtkmm/label.h>
#include <thread>
#include <mutex>

class LZFoxWindow;

class Clock {
  public:

    Clock();
    void start(LZFoxWindow *caller);
    void stop();
    bool hasStopped() const;

  protected:
    mutable std::mutex m_mutex;
    bool stopped;
};
#endif