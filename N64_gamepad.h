#ifndef _N64_GAMEPAD_H
#define _N64_GAMEPAD_H

#include "gamepad.h"

// this is based on http://www.instructables.com/id/Turn-an-N64-Controller-into-a-USB-Gamepad-using-an/


#define DPORT_HIGH(pinnn) ((DDRD &= ~((pinnn))))
#define DPORT_LOW(pinnn)   ((DDRD |= ((pinnn))))
#define DPORT_QUERY(pinnn)  ((PIND & ((pinnn))))



struct N64_gamepad: public bit_gamepad<uint32_t>
{
  typedef bit_gamepad<uint32_t> gamepad_base;
  static const uint8_t N_BUTTONS = 16;
  static const String names[N_BUTTONS]; // both of these should have in any concrete class of gamepads
  //      0         1         2     3       4     5       6       7
  enum b {A,        B,        Z,    start,  Dup,  Ddown,  Dleft,  Dright,
  //      8         9         10    11      12    13      14      15
          reset,   unkown,    L,    R,      Cup,  Cdown,  Cleft,  Cright};

  int8_t get_x() {
    return buttons >> uint_t(16);
  }
  int8_t get_y() {
    return buttons >> uint_t(24);
  }

  static const uint8_t n_dpads = 2;
  enum d { Dpad, Cpad };

protected:

  directional dpads[2] = {directional(4,5,6,7, *this), directional(12,13,14,15, *this)};
  uint8_t N64_pin;
  
public:

  N64_gamepad(const N64_gamepad& other)
  : N64_gamepad(other.id,other.N64_pin,false)
  {}

  N64_gamepad(uint8_t id, uint8_t N64_pin=3, bool init=true)
  : gamepad_base(id, N_BUTTONS, dpads, n_dpads), N64_pin(N64_pin)
  {
    if(init)
      AndrewBrownInitialize();
  }

  virtual String* get_button_names() const {
    return names;
  }

  void AndrewBrownInitialize();
  void AndrewBrownSend(unsigned char *buffer, char length);
  void AndrewBrownGet();

  struct {
    int8_t xmax=0;
    int8_t xmin=0;
    int8_t ymax=0;
    int8_t ymin=0;
  } analog_range;

  float get_x_cal() {
    float x = get_x();
    if(x==0 || analog_range.xmax==0 || analog_range.xmin==0)
      return 0.;
    if(x>0)
      return x/float(analog_range.xmax);
    return -x/float(analog_range.xmin);
  }
  float get_y_cal() {
    float y = get_y();
    if(y==0 || analog_range.ymax==0 || analog_range.ymin==0)
      return 0.;
    if(y>0)
      return y/float(analog_range.ymax);
    return -y/float(analog_range.ymin);
  }


protected:

  virtual void latch() {
    unsigned char command[] = {0x01};
    AndrewBrownSend(command, 1);
  }
  virtual void read_imp() { // call N64_gamepad::read()!
    AndrewBrownGet();
  }
  virtual void action_after_read() {
    translate_raw_data();

    if(button_is_pressed(b::reset)) {
      analog_range.xmax = analog_range.xmin = analog_range.ymax = analog_range.ymin = 0;
      return;
    }
    
    int8_t aux = get_x();
    if(aux>analog_range.xmax) analog_range.xmax=aux-1;
    if(aux<analog_range.xmin) analog_range.xmin=aux+1;
    aux = get_y();
    if(aux>analog_range.ymax) analog_range.ymax=aux-1;
    if(aux<analog_range.ymin) analog_range.ymin=aux+1;
  }

  void translate_raw_data() {
    // The get_N64_status function sloppily dumps its data 1 bit per byte
    // into the get_status_extended char array. It's our job to go through
    // that and put each piece neatly into the struct N64_status
    //memset(&buttons, 0, sizeof(buttons));
    for (int ii = 0; ii <= 16; ++ii) {
      set_button_state(ii, raw_dump[ii]);
    }
    for (int ii = 0; ii <= 8; ++ii) {
      set_button_state(ii+16, raw_dump[23-ii]);
    }
    for (int ii = 0; ii <= 8; ++ii) {
      set_button_state(ii+24, raw_dump[31-ii]);
    }
  }
  char raw_dump[33]; // 1 received bit per byte // why 33??
};

#ifdef _GAMEPAD_DEFINE_N64_HID
class N64_hid: public gamepad_joystick<N64_gamepad> {
  public:
    typedef gamepad_joystick<N64_gamepad> gamepad_base;
    
    N64_hid(uint8_t id, uint8_t N64_pin=3, bool init=true)
      : gamepad_base(N64_gamepad(id, N64_pin, false))
    {
      if(init)  AndrewBrownInitialize();
      
      this->usb_joystick.setXAxisRange(-127, 127);
      this->usb_joystick.setYAxisRange(-127, 127);
    }

    virtual void action_any_button_changed() {
      this->usb_joystick.setXAxisRange(analog_range.xmin, analog_range.xmax);
      this->usb_joystick.setXAxisRange(analog_range.ymin, analog_range.ymax);
      this->usb_joystick.setXAxis(get_x());//_cal()*127);
      this->usb_joystick.setYAxis(get_y());//_cal()*127);
      gamepad_base::action_any_button_changed();
    }

};
#endif // _GAMEPAD_DEFINE_N64_HID

#endif // _N64_GAMEPAD_H


