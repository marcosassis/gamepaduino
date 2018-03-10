#ifndef _GAMEPAD_MIDI_H
#define _GAMEPAD_MIDI_H

#include "active_gamepad.h"
#include "midi_interface.h"

namespace gamepad {

typedef meta::midi::data_byte             midi_data_t;
typedef meta::midi::midi_send_interface   midi_send_interface;


/*
  // http://www.music-software-development.com/midi-tutorial.html
  // https://en.wikipedia.org/wiki/List_of_musical_symbols#Dynamics
  enum dynamics {
    pppp=8, ppp=20, pp=31, p=42, mp=53, mf=64, f=80, ff=96, fff=112, ffff=127
  };

  // https://en.wikipedia.org/wiki/Dynamics_(music)
  // Letters	                ppp	pp	p	  mp	mf	f	  ff	fff
  // Logic Pro 9 dynamics[2]	16	32	48	64	80	96	112	127
  // Sibelius 5 dynamics[3]	  20	39	61	71	84	98	113	127
  // Sibelius 5 attacks	15	  30	50	60	75	90	105	119
*/

template<class gamepad_type>
struct gamepad_midi: public active_gamepad<gamepad_type>
{
  typedef gamepad_type                      gamepad_t; // base's base, actually the gamepad
  typedef active_gamepad<gamepad_type>      gamepad_base; // active_gamepad is a "shell", this is another
  
  midi_send_interface& _midi_;

  gamepad_midi(gamepad_t const& base, midi_send_interface& _midi_instance)
    : gamepad_base(base), _midi_(_midi_instance)
  {}
  
  midi_send_interface& midi_interface() {
    return _midi_;
  }
  
  midi_send_interface const& midi_instance() const {
    return _midi_;
  }
  
  virtual void flush() {
    midi_interface().flush();
  }
    
  virtual void read() {
    gamepad_base::read();
    //flush();
  }
};

}
#endif // _GAMEPAD_MIDI_H


