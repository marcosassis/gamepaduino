#ifndef _GAMEPAD_H
#define _GAMEPAD_H

#include "directional.h"

namespace gamepad {

/// this abstract class is the essential code-interface for gamepads
/**

IMPORTANT: all concrete classes should
               -> have deep copy semantics*
               -> implement all abstract member functions (of all bases)
               -> and define these following public entities:
                      typedef * gamepad_base;                // 1
                      static const uint8_t N_BUTTONS = *;    // 2
                      static const String names[N_BUTTONS];  // 3
                      enum bid {*, *, ..., *};               // 4
                  
*(think it's ok to allow copied objects share buffers,
  since it's same player/controller anyway. todo: study this on N64)

examples from SNES_gamepad.h
                  
1:  you must typedef which is the next base class in the hierarchy.
    ex.:
        typedef bit_gamepad<uint16_t> gamepad_base;

2:  it's useful to have number of buttons in compile time
        (at least if you're not into some dynamically size changing monster - oh no -
        OR something as your controller class allows each instance to have different
        number of buttons/values, as you could do for game-time plug accessories, e.g.)
            (then this could be absent, gamepad interface allows it,
            and buttonset already implement this button counting dynamically)
    ex.:
        static const uint8_t N_BUTTONS = 12;
        
3:  the same way, names are required, so you can implement the first
    abstract function of the hierarchy (remember to check them all):
    
        (virtual) String* (get_button_names::)get_button_names() const = 0;
        
    ex. (you see how static const N_BUTTONS is useful):
    
        static const String names[N_BUTTONS]; // defined in cpp file

4:  following this statically prepared convenience, user could use button ids as
    coherently (possible auto)searching button name in a :: manner.
    ex.:
        //      0  1  2       3      4   5     6     7      8  9  10 11
        enum bid {B, Y, select, start, up, down, left, right, A, X, L, R};
        
        
so **bid** is: button id

the same way we'll use, when makes sense:
    
    **did** is: directional pad (dpad) id
    **aid** is: analog id (analog can be multi-axes: todo)

please follow these names and conventions for consistency

 */
class gamepad: public buttonset
{
protected:

  directional* dpads;
  uint8_t n_dpads;

  virtual void latch() = 0;
  virtual void read_imp() = 0;

public:

  gamepad(uint8_t id, uint8_t n_buttons, directional* dpads=NULL, uint8_t n_dpads=0)
  : buttonset(id,n_buttons), dpads(dpads), n_dpads(n_dpads)
  {}

  /// please check active_gamepad.h before override directly
  /// from gamepad::action_*_read member functions
  virtual void action_before_read() {}
  virtual void action_after_read() {}

  /// the BASIC idea of gamepad library/interface is to READ controllers INPUTS->MCU
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
