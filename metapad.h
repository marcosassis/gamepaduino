#ifndef _METAPAD_H
#define _METAPAD_H

#include "gamepad.h"
#include "xmidiusb.h"

//                                              0    1    2         3        4     5       6       7        8    9   10   11
//static const String SNES_gamepad::names[] = {"B", "Y", "select", "start", "up", "down", "left", "right", "A", "X", "L", "R"};


template<class gamepad_type>
class metapad: public gamepad_midi<gamepad_type> {
    int8_t** intervals;

  public:

    typedef gamepad_midi<gamepad_type> gamepad_base;

    gamepad_midi(const gamepad_type& basebase,
                 int8_t** intervals, int8_t* channel_map, int8_t* velocity_map,
                 XMIDIUSB_class& XMIDIUSB_instance)
      : gamepad_base(basebase, *intervals, channel_map, velocity_map, XMIDIUSB_instance)
    {}
    
    gamepad_midi(const gamepad_type& basebase,
                 int8_t** intervals, int8_t* channel_map=NULL, int8_t* velocity_map=NULL)
      : gamepad_base(basebase, *intervals, channel_map, velocity_map, XMIDIUSB)
    {}
    
    virtual void action_button_changed(uint8_t i) {
      note_onoroff( i, get_button_state(i) );
    }

    virtual void note_onoroff(uint8_t i, bool isnoteon) {
      Serial.print(i); Serial.print("\t"); Serial.println(note_map[i]);
      uint8_t c = channel_map? channel_map[i] : DEAFULT_CHANNEL;
      uint8_t v = velocity_map? velocity_map[i] : DEAFULT_VELOCITY;
      if(note_map)
        if(isnoteon)
          XMIDIUSB_.note_on(c, note_map[i], v);
        else
          XMIDIUSB_.note_off(c, note_map[i], v);
    }

};


#endif // _METAPAD_H
