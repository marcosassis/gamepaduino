#ifndef _GAMEPAD_STOPWATCH
#define _GAMEPAD_STOPWATCH

#include "Arduino.h"

namespace gamepad {


struct stopwatch {
  unsigned long start_instant;
  unsigned long last_instant;
  unsigned long stop_instant;
  unsigned long time_to_wait;
  
  stopwatch()
  : start_instant(micros()), time_to_wait(0)
  {}
  
  stopwatch(unsigned long time_to_wait)
  : start_instant(micros()), time_to_wait(time_to_wait)
  {}
  
  void update() {
    last_instant=micros();
  }
  
  void start() {
    start_instant=micros();
  }
  
  void start(unsigned long time_to_wait) {
    start();
    this->time_to_wait = time_to_wait;
  }

  void stop() {
    stop_instant=micros();
  }
  
  bool time_has_elapsed() {
    return elapsed_time() >= time_to_wait; // elapsed_time() updates
  }
  
  unsigned long elapsed_time() {
    update();
    return last_instant-start_instant;
  }
  
  bool stop_if_elapsed() {
    bool b = time_has_elapsed();  // elapsed_time() updates
    if(b)
      stop();
    return b;
  }
  
  // blocking functions, wait until time elapses
  void wait() {
    while(stop_if_elapsed()) {}
  }
  void wait(unsigned long time_to_wait) {
    this->time_to_wait = time_to_wait;
    wait();
  }
  
  // this DON'T update
  unsigned long measure_time() {
    return stop_instant-start_instant;
  }
};

}
#endif//_GAMEPAD_STOPWATCH