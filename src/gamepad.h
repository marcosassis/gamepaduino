#ifndef _GAMEPAD_H
#define _GAMEPAD_H

#include "directional.h"

namespace gamepad {


// IMPORTANT: all concrete classes should have
//              deep copy semantics
//              and define two 

/// gamepad
class gamepad: public buttonset
{
protected:

  directional* dpads;
  uint8_t n_dpads;

  virtual void latch() = 0;
  virtual void read_imp() = 0;

  virtual void action_before_read() {}
  virtual void action_after_read() {}

public:

  gamepad(uint8_t id, uint8_t n_buttons, directional* dpads=NULL, uint8_t n_dpads=0)
  : buttonset(id,n_buttons), dpads(dpads), n_dpads(n_dpads)
  {}

  virtual void read() {
    action_before_read();
    noInterrupts();
    latch();
    read_imp();
    interrupts();
    action_after_read();
  }

  virtual const directional* get_dpad(uint8_t i=0)
  {
    if(dpads && n_dpads && i<n_dpads)
      return dpads+i;

    return dpads;
  }
  virtual void set_dpad(directional& newdpad, uint8_t i=0)
  {
    if(dpads && n_dpads && i<n_dpads)
      dpads[i] = newdpad;
  }
};

}
#endif // _GAMEPAD_H
