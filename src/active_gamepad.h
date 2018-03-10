#ifndef _ACTIVE_GAMEPAD_H
#define _ACTIVE_GAMEPAD_H

#include "gamepad.h"

namespace gamepad {

/// this template class is an interface intended to all gamepad:: classes that makes OUTPUTS
/// derive from this class is 1 option to use/extend this library in this way
/// user can always monitor changes and read button states inside loop(), action according 
template<class gamepad_type>
struct active_gamepad: public gamepad_type {
  
  /// then this base type is the original input/reader class
  typedef gamepad_type gamepad_t;

  active_gamepad(const gamepad_t& base) // concrete classes should have deep copy semantics
  : gamepad_t(base)
  {}

  /// and any action must be achieved for each button
  virtual void action_button_changed(uint8_t i) {} // you have to override this one or one of the others

  virtual void action_after_read() { // or even this one, maybe
    gamepad_t::action_after_read();   // call base action_after_read!
    if(any_button_state_has_changed()) // if this question is fast (as with bit_gamepad) = ok this imp.
      action_any_button_changed();
  }
  virtual void action_any_button_changed() { // maybe you'd override this
    for(uint8_t i=0; i<get_n_buttons(); ++i)  // if this for is really needed (for all buttons) = ok this imp.
      if(button_state_has_changed(i))          // but you'll have to override action_button_changed
        action_button_changed(i);
  }
  
};

}
#endif // _ACTIVE_GAMEPAD_H
