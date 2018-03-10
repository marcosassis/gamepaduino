#ifndef _MIDI_INSTRUMENT_H
#define _MIDI_INSTRUMENT_H

#include "gamepad_midi.h"

namespace gamepad {

template<class gamepad_type>
struct midi_instrument: public gamepad_midi<gamepad_type>
{
  typedef gamepad_midi<gamepad_type>  gamepad_midi_base;
  typedef gamepad_type                gamepad_t;
  
  typedef  int8_t                 note_t; // todo: generalize in midi interface
  
  static const note_t DONT_PLAY = -128;
  note_t* interval_map; // size == gamepad_t.get_n_buttons();
  midi_data_t tone;
  midi_data_t channel;
  midi_data_t velocity;
  
  midi_instrument(gamepad_t& base,  midi_send_interface& _midi_instance,
                  note_t interval_map[],  midi_data_t tone,  midi_data_t channel=0,  midi_data_t velocity=100)
  : gamepad_midi_base(base, _midi_instance),
    interval_map(interval_map), tone(tone), channel(channel), velocity(velocity)
  {}
  
  int8_t get_interval(uint8_t i) const {
    return (i>=get_n_buttons()) ? DONT_PLAY : interval_map[i];
  }
  
  int8_t get_note(uint8_t i) const {  
    int8_t interv = get_interval(i);
    return (interv==DONT_PLAY) ? DONT_PLAY : (interv+tone);
  }

  virtual void set_interval(uint8_t i, int8_t interv) {
    if(i>=get_n_buttons())
      return;
    interval_map[i] = interv;
  }
  
  virtual void action_button_changed(uint8_t i) {
    int8_t n = get_note(i);
    if(n>=0) {
      if(get_button_state(i))
        midi_interface().note_on(channel,n,velocity);
      else
        midi_interface().note_off(channel,n,velocity);
    }
  }
};

}
#endif // _MIDI_INSTRUMENT_H


