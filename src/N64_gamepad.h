#ifndef _N64_GAMEPAD_H
#define _N64_GAMEPAD_H

// doc https://github.com/marcosassis/gamepaduino/wiki/N64

// this is based on http://www.instructables.com/id/Turn-an-N64-Controller-into-a-USB-Gamepad-using-an/

#include "bit_gamepad.h"
//#include "pins_arduino.h"

namespace gamepad {

// available pins 

// pro micro [ATmega32U4]
// 
// 2,3,4,6
// 
// Arduino Pin Mapping - PD 
// https://www.arduino.cc/en/Hacking/PinMapping32u4
// (OC0B/SCL/INT0)    PD0   Digital pin 3 (SCL)(PWM)
// (SDA/INT1)         PD1   Digital pin 2 (SDA)
// (RX D1/AIN1/INT2)  PD2   Digital pin 0 (RX) // nop
// (TXD1/INT3)        PD3   Digital pin 1 (TX) // no 
//   (please don't use TX/RX on pro micro, I think you'd have to
//   flash with an external programmer if this messes up)
// (ICP1/ADC8)        PD4   Digital pin 4
// (XCK1/#CTS)        PD5   TXLED
// (T1/#OC4D/ADC9)    PD6   Digital pin 12 // ??? unavailable on board
// (T0/OC4D/ADC10)    PD7   Digital Pin 6 (PWM)

// pins_arduino.h [pro micro]
/*
const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(2), // D0 - PD2
	_BV(3),	// D1 - PD3
	_BV(1), // D2 - PD1
	_BV(0),	// D3 - PD0
	_BV(4),	// D4 - PD4
	_BV(6), // D5 - PC6
	_BV(7), // D6 - PD7
  ...
	_BV(6), // D12 - PD6 // ??? unavailable on board
  ...
*/

// arduino 
// 
// 0,1,2,3,4,5,6,7
// (in general arduino boards, digital pins 0..7 correspond to PD bits 0..7)
// 
// ATmega168/328P-Arduino Pin Mapping - PD 
// https://www.arduino.cc/en/Hacking/PinMapping168
//                    PD0   Digital pin 0
//                    PD1   Digital pin 1
//                    PD2   Digital pin 2
//                    PD3   Digital pin 3
//                    PD4   Digital pin 4
//                    PD5   Digital pin 5
//                    PD6   Digital pin 6
//                    PD7   Digital pin 7

struct N64_gamepad: public bit_gamepad<uint32_t>
{
  typedef bit_gamepad<uint32_t> gamepad_base;
  static const uint8_t N_BUTTONS = 16;
  static const String names[N_BUTTONS]; // both of these should have in any concrete class of gamepads
  //      0         1         2     3       4     5       6       7
  enum b {A,        B,        Z,    start,  Dup,  Ddown,  Dleft,  Dright,
  //      8         9         10    11      12    13      14      15
          reset,   unkown,    L,    R,      Cup,  Cdown,  Cleft,  Cright};
       // reset is a virtual pin when pressing start+R+L(at same time)
       //       this will report both R and L pressed(1), but start will report unpressed(0)
       //          ^ watch out this for your application
       //            (this is normal N64 console/controller operation)
       //       AND this will reset controller analog position, as if console has started
       //          ^ the behavior is that zero position is considered from where the stick
       //            was when console started or controller reseted (N64 analog is differential)

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
  char raw_dump[33]; // 1 received bit per byte // why 33??
  uint8_t N64_pin_bit;
  uint8_t N64_pin;
  
public:

  N64_gamepad(const N64_gamepad& other)
  : N64_gamepad(other.id,other.N64_pin,false)
  {}

  N64_gamepad(uint8_t id, uint8_t N64_pin=3, bool init=true)
  : gamepad_base(id, N_BUTTONS, dpads, n_dpads), N64_pin(N64_pin), N64_pin_bit(B10000)//, N64_pin_bit(uint8_t(digital_pin_to_bit_mask_PGM[N64_pin]))
  {
    N64_pin_bit = digitalPinToBitMask(N64_pin);//uint8_t(digital_pin_to_bit_mask_PGM[2]);
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
    // memset(&buttons, 0, sizeof(buttons));
    for (int ii = 0; ii <= 16; ++ii) {
      set_button_state(ii, raw_dump[ii]);
    }
    for (int ii = 0; ii <= 8; ++ii) { // invert byte order for analog values (bit endianness stuff)
      set_button_state(ii+16, raw_dump[23-ii]);
    }
    for (int ii = 0; ii <= 8; ++ii) {
      set_button_state(ii+24, raw_dump[31-ii]);
    }
  }
};

// if you want to TURN OFF this optimization class for SNES controllers protocol,
// just define _GAMEPAD_SNES_SINGLEPLAYER before include this file (if you don't, never mind)
#ifndef _GAMEPAD_N64_SINGLEPLAYER
#define _GAMEPAD_N64_MULTIPLAYER
}//end namespace gamepad
#include "multiplayer.h"
namespace gamepad {



#endif // _GAMEPAD_N64_SINGLEPLAYER


}
#endif // _N64_GAMEPAD_H


