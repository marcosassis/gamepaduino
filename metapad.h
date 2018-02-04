#ifndef _METAPAD_H
#define _METAPAD_H

#include "gamepad_midi.h"

template<class gamepad_type, class XMIDIUSB_type=XMIDIUSB_class>
struct metapad: public note_map<gamepad_type,XMIDIUSB_type> {
  typedef XMIDIUSB_type XMIDIUSB_t;
  typedef note_map<gamepad_type,XMIDIUSB_type> gamepad_midi_base;
  typedef gamepad_type gamepad_t;

  uint8_t base_pitch;
  uint8_t tone_mod;
  int8_t octave_shift;

  metapad(gamepad_type& base, XMIDIUSB_t& XMIDIUSB_instance,
          uint8_t intervals[], uint8_t base_pitch=69, uint8_t channel=0, uint8_t velocity=100)
  : gamepad_midi_base(base, XMIDIUSB_instance, intervals, channel, velocity),
    base_pitch(base_pitch), tone_mod(0), octave_shift(0)
  {}
  metapad(gamepad_type& base, uint8_t intervals[], uint8_t base_pitch=69, uint8_t channel=0, uint8_t velocity=100)
  : gamepad_midi_base(base, intervals, channel, velocity), base_pitch(base_pitch), tone_mod(0), octave_shift(0)
  {}
  
  
  virtual int8_t get_note(uint8_t i) const {
    //Serial.println(abs(int(n + base_pitch + tone_mod + octave_shift*12))%128);
    int8_t n = gamepad_midi_base::get_note(i);
    if(n>-128)
      return (120+int(n + base_pitch + tone_mod + octave_shift*12)%120)%120; //%128
  }

  virtual void action_button_changed(uint8_t i)
  { 
    if(get_dpad()) {
      if(get_dpad()->is_up()) {
        tone_mod = (tone_mod >= 11) ? 0 : tone_mod+1;
      }
      if(get_dpad()->is_down()) {
        tone_mod = (tone_mod <= 0) ? 11 : tone_mod-1;
      }
      if(get_dpad()->is_left()) {
        --octave_shift;
        //base_pitch = (base_pitch-12)%128;
      }
      if(get_dpad()->is_right()) {
        ++octave_shift;
        //base_pitch = (base_pitch+12)%128;
      }
    }
    if(i==2) { // b::select
      XMIDIUSB_instance().all_sounds_off(get_channel());
      XMIDIUSB_instance().all_notes_off(get_channel());
    }
    gamepad_midi_base::action_button_changed(i);

#ifdef _METAPAD_DEBUG
    Serial.print(get_note(i)); Serial.print("\t");
    Serial.print(base_pitch); Serial.print("\t");
    Serial.print(tone_mod); Serial.print("\t");
    Serial.println(octave_shift);
#endif

  }
};


#endif // _METAPAD_H
