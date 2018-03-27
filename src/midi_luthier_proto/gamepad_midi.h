#ifndef _GAMEPAD_MIDI_H
#define _GAMEPAD_MIDI_H

#include "active_gamepad.h"
#include "midi_interface.h"

namespace gamepad {

typedef meta::midi::data_byte             midi_data_t;
typedef meta::midi::midi_send_interface   midi_send_interface;

template<class gamepad_type>
struct gamepad_midi: public active_gamepad<gamepad_type>
{
  typedef gamepad_type                  gamepad_t; /// base's base, actually the gamepad
  typedef active_gamepad<gamepad_type>  gamepad_base; /// active_gamepad is a "shell", this is another

  midi_send_interface& _midi_;

  gamepad_midi(gamepad_t const& base, midi_send_interface& _midi_instance)
    : gamepad_base(base), _midi_(_midi_instance)
  {}
  
  /// that's when we go midi (with a gamepad)
  midi_send_interface& midi_interface() {
    return _midi_;
  }
  midi_send_interface const& midi_instance() const {
    return _midi_;
  }
  
  /// most important functionss...
  ///
  /// """read everything then do stuff [then flush()]"""
  /// 
  virtual void read() {
    gamepad_base::read();
  }

  /// ... about this info flow =]
  /// 
  /// understand these both and your life will be blessed
  /// 
  virtual void flush() {
    midi_interface().flush();
  }
  
};

}
#endif // _GAMEPAD_MIDI_H


