#ifndef _ACTIVE_GAMEPAD_H
#define _ACTIVE_GAMEPAD_H

#include "gamepad.h"

namespace gamepad {


template<class gamepad_type>
struct active_gamepad: public gamepad_type {
  
  typedef gamepad_type gamepad_t;

  active_gamepad(const gamepad_t& base) // concrete classes should have deep copy semantics
  : gamepad_t(base)
  {}

  virtual void action_after_read() { // or even this one, maybe
    gamepad_t::action_after_read();
    //Serial.println(get_id());
    if(any_button_state_has_changed()) // if this question is fast (as with bit_gamepad) = ok this imp.
      action_any_button_changed();
  }
  virtual void action_any_button_changed() { // maybe you'd override this
    for(uint8_t i=0; i<get_n_buttons(); ++i) // if this for is really needed = ok this imp.
      if(button_state_has_changed(i))
        action_button_changed(i);
  }
  virtual void action_button_changed(uint8_t i) {} // you have to override this one or one of the others
};

}
#endif // _ACTIVE_GAMEPAD_H
