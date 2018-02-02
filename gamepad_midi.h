#ifndef _GAMEPAD_MIDI_H
#define _GAMEPAD_MIDI_H

#include "gamepad.h"
#include "xmidiusb.h"


template<class gamepad_type>
class gamepad_midi: public gamepad_type {
    int8_t* note_map;
    int8_t* channel_map;
    int8_t* velocity_map;
    XMIDIUSB_class& XMIDIUSB_;

  public:

    typedef gamepad_type gamepad_base;
    static const int8_t DEAFULT_CHANNEL = 0;
    static const int8_t DEAFULT_VELOCITY = 100;

    gamepad_midi(const gamepad_base& base,
                 int8_t* note_map, int8_t* channel_map, int8_t* velocity_map,
                 XMIDIUSB_class& XMIDIUSB_instance)
      : gamepad_base(base),
        note_map(note_map), note_map(note_map), note_map(note_map),
        XMIDIUSB_(XMIDIUSB_instance)
    {}

    gamepad_midi(gamepad_base& base,
                 int8_t* note_map, int8_t* channel_map=NULL, int8_t* velocity_map=NULL)
      : gamepad_base(base),
        note_map(note_map), channel_map(channel_map), velocity_map(velocity_map),
        XMIDIUSB_(XMIDIUSB)
    {}

    virtual void flush_usb() {
      XMIDIUSB_.flush();
    }
    
    virtual void read() {
      gamepad_base::read();
      for(uint8_t i=0; i<get_n_buttons(); ++i)
        if(note_map[i]>-128 && button_state_has_changed(i))
          action_button_changed(i);
          
      flush_usb();
    }

    virtual void action_button_changed(uint8_t i) {
      note_onoroff(i,get_button_state(i));
    }

    virtual void note_onoroff(uint8_t i, bool isnoteon) {
      if(i>=get_n_buttons())
        return;
      uint8_t c = channel_map? channel_map[i] : DEAFULT_CHANNEL;
      uint8_t v = velocity_map? velocity_map[i] : DEAFULT_VELOCITY;
      if(note_map)
        if(isnoteon)
          XMIDIUSB_.note_on(c, note_map[i], v);
        else
          XMIDIUSB_.note_off(c, note_map[i], v);
    }

    virtual void note_off(uint8_t i) {
      note_onoroff(i,false);
    }
    virtual void note_on(uint8_t i) {
      note_onoroff(i,true);
    }

    const int8_t get_note(uint8_t i) const {
      if(i>=get_n_buttons())
        return -128;
      
      return note_map[i];
    }

    void set_note(uint8_t i, uint8_t note) {
      if(i>=get_n_buttons())
        return;
      
      note_map[i] = note;
    }
};


#endif // _GAMEPAD_MIDI_H
