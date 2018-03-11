#ifndef _PITCH_WHEEL_H
#define _PITCH_WHEEL_H

#include "midi_instrument.h"

namespace meta { namespace midi { namespace luthier {

template<class gamepad_midi_type>
struct pitch_wheel: public gamepad_midi_type
{
  typedef          gamepad_midi_type            gamepad_midi_base;
  typedef typename gamepad_midi_base::gamepad_t gamepad_t;
  
  typedef  int8_t pitch_bend_t; // todo: generalize pitch_bend_t in midi interface
  
  
  pitch_wheel(gamepad_midi_base& base)
  : gamepad_midi_base(base)
  {}
  
  virtual void action_any_button_changed() {
    gamepad_midi_base::action_any_button_changed();
    
    Serial.print(gamepad_t::get_x_cal()); Serial.print("\t");
    Serial.println(gamepad_t::get_x());
  }
};

}}}
#endif // _PITCH_WHEEL_H


