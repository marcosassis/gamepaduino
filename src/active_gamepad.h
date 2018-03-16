#ifndef _ACTIVE_GAMEPAD_H
#define _ACTIVE_GAMEPAD_H

#include "gamepad.h"

namespace gamepad {

/// this template class is an interface intended to all gamepad:: classes that makes OUTPUTS
/// derive from this class is 1 option to use/extend this library in this way, or
/// user can always monitor changes and read button states inside loop(), action according 
template<class gamepad_type>
struct active_gamepad: public gamepad_type {
  
  /// then this base type is the original input/reader class
  typedef gamepad_type gamepad_t;

  /// concrete classes should have deep copy semantics
  active_gamepad(const gamepad_t& base) 
  : gamepad_t(base)
  {}

  /// and any action must be achieved for each button
  ///
  /// you have to override this one or one of the others
  ///
  virtual void action_button_changed(uint8_t i) {
    _GAMEPAD_DEBUG("active_gamepad::action_button_changed");
  } 

  /// maybe you'd override this
  virtual void action_any_button_changed() {
    _GAMEPAD_DEBUG("active_gamepad::action_any_button_changed");
    for(uint8_t i=0; i<get_n_buttons(); ++i) // if this for is really needed (for all buttons) = ok this imp.
      if(button_state_has_changed(i))        // but you'll have to override action_button_changed
        action_button_changed(i);
  }  
  
  /// or even this one, maybe
  virtual void action_after_read() { 
    _GAMEPAD_DEBUG("active_gamepad::action_after_read");
    gamepad_t::action_after_read();    // call base action_after_read!
    if(any_button_state_has_changed()) // if this question is fast (as with bit_gamepad) = ok this imp.
      action_any_button_changed();
  }
  
  /// [optional] [these flush() semantics depends on protocol/case]
  ///
  /// to be used when communication demands an 'a posteriori' action to commit message
  /// use whenever makes sense, it can be most important in some cases
  /*! 
      use this and read in loop():
          {
            mygamepad.read();
            // do my stuff
            mygamepad.flush();
            // then flush to proceed actions commanded
          }          
          
      example of do_my_stuf():
      {
          mygamepadmidi.read();
          if(mygamepadmidi.button_state_has_changed(mygamepadmidi_type::bid::my_button_id))
          {
            mygamepadmidi.midi_interface().play_some_kinda_bulshit();
                // we don't have this interface yet, it's illustrative
          }
          mygamepadmidi.flush();
      }
      
      button_state_has_changed(i) is buttonset:: interface (check out)     
  */
  virtual void flush() {
    _GAMEPAD_DEBUG("active_gamepad::flush");
  }
};

}
#endif // _ACTIVE_GAMEPAD_H
