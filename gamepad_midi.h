#ifndef _GAMEPAD_MIDI_H
#define _GAMEPAD_MIDI_H

//#include "gamepad.h"
#include "SNES_gamepad.h"
#include "xmidiusb.h"






template<class gamepad_type, class XMIDIUSB_type=XMIDIUSB_class>
class gamepad_midi: public active_gamepad<gamepad_type> {
protected:
  uint8_t channel;
  uint8_t velocity;

public:
  
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


  typedef gamepad_type gamepad_t;
  typedef active_gamepad<gamepad_type> gamepad_base;
  typedef XMIDIUSB_type XMIDIUSB_t;
  XMIDIUSB_t& XMIDIUSB_;


  gamepad_midi(const gamepad_base& base, XMIDIUSB_t& XMIDIUSB_instance, uint8_t channel=0, uint8_t velocity=dynamics::mf)
    : gamepad_base(base), XMIDIUSB_(XMIDIUSB_instance), channel(channel), velocity(velocity)
  {}

  gamepad_midi(const gamepad_base& base, uint8_t channel=0, uint8_t velocity=dynamics::mf)
    : gamepad_base(base), XMIDIUSB_(XMIDIUSB), channel(channel), velocity(velocity)
  {}

  virtual void flush_usb() {
    XMIDIUSB_.flush();
  }
    
  virtual void read() {
    gamepad_base::read();
    //flush_usb();
  }

  virtual void note_on(uint8_t c, uint8_t n, uint8_t v) {
    if(c<128 && n<128 && v<128)
      XMIDIUSB_.note_on(c, n, v);
  }
  virtual void note_off(uint8_t c, uint8_t n, uint8_t v) {
    if(c<128 && n<128 && v<128)
      XMIDIUSB_.note_off(c, n, v);
  }

  virtual void note_on(uint8_t n, uint8_t v) {
    note_on(channel, n, v);
  }
  virtual void note_off(uint8_t n, uint8_t v) {
    note_off(channel, n, v);
  }
  virtual void note_on(uint8_t n) {
    note_on(channel, n, velocity);
  }
  virtual void note_off(uint8_t n) {
    note_off(channel, n, velocity);
  }

  uint8_t get_channel() const {
    return channel;
  }
  void set_channel(uint8_t newchannel) {
    channel=newchannel;
  }
  uint8_t get_velocity() const {
    return velocity;
  }
  void set_velocity(uint8_t newvelocity) {
    velocity=newvelocity;
  }

  virtual XMIDIUSB_t& XMIDIUSB_instance() {
    return XMIDIUSB_;
  }
};



template<class gamepad_type, class XMIDIUSB_type=XMIDIUSB_class>
struct note_map: public gamepad_midi<gamepad_type,XMIDIUSB_type> {
  typedef XMIDIUSB_type XMIDIUSB_t;
  typedef gamepad_midi<gamepad_type,XMIDIUSB_type> gamepad_midi_base;
  typedef gamepad_type gamepad_t;
  
  int8_t* pitch_map;

  note_map(gamepad_type& base, XMIDIUSB_t& XMIDIUSB_instance,
           uint8_t pitch_map[], uint8_t channel=0, uint8_t velocity=100)
  : gamepad_midi_base(base, XMIDIUSB_instance, channel, velocity), pitch_map(pitch_map)
  {}
  note_map(gamepad_type& base, int8_t pitch_map[], uint8_t channel=0, uint8_t velocity=100)
  : gamepad_midi_base(base, channel, velocity), pitch_map(pitch_map)
  {}
      
    
  virtual int8_t get_note(uint8_t i) const {
    if(i>=get_n_buttons())
      return -128;
    
    return pitch_map[i];
  }

  virtual void set_note(uint8_t i, int8_t note) {
    if(i>=get_n_buttons())
      return;
      
    pitch_map[i] = note;
  }
  
  virtual void action_button_changed(uint8_t i) {
    int8_t n = get_note(i);
    if(n>=0) {
      if(get_button_state(i))
        note_on(n);
      else
        note_off(n);
    }
  }
};



#endif // _GAMEPAD_MIDI_H


