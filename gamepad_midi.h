#ifndef _GAMEPAD_MIDI_H
#define _GAMEPAD_MIDI_H

#include "gamepad.h"
#include "xmidiusb.h"


template<class gamepad_type, class XMIDIUSB_type=XMIDIUSB_class>
class gamepad_midi: public gamepad_type {
protected:
  uint8_t channel;
  uint8_t velocity;

public:

  typedef gamepad_type gamepad_base;
  typedef XMIDIUSB_type XMIDIUSB_t;
  XMIDIUSB_t& XMIDIUSB_;



  gamepad_midi(gamepad_base& base, XMIDIUSB_t& XMIDIUSB_instance, uint8_t channel=0, uint8_t velocity=100)
    : gamepad_base(base), XMIDIUSB_(XMIDIUSB_instance), channel(channel), velocity(velocity)
  {}

  gamepad_midi(gamepad_base& base, uint8_t channel=0, uint8_t velocity=100)
    : gamepad_base(base), XMIDIUSB_(XMIDIUSB), channel(channel), velocity(velocity)
  {}

  virtual void flush_usb() {
    XMIDIUSB_.flush();
  }
    
  virtual void read() {
    gamepad_base::read();
    for(uint8_t i=0; i<get_n_buttons(); ++i)
      if(button_state_has_changed(i))
        action_button_changed(i);
          
    flush_usb();
  }

  virtual void action_button_changed(uint8_t i) = 0;

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
