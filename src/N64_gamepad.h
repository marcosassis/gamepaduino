// https://github.com/marcosassis/gamepaduino/wiki/N64

#ifndef _N64_GAMEPAD_H

#include "bit_analog.h"

#if (!defined(_GAMEPAD_SINGLEPLAYER)) && (!defined(_GAMEPAD_N64_SINGLEPLAYER))
//if not single 
#define _GAMEPAD_N64_MULTIPLAYER
// then multi
#include "multiplayer.h"
// for default this N64 interface is multiplayer compatible
// we have to friend this multiplayer class instance, see below
#endif

namespace gamepad {

///   i was not liking this:        32 bits   axis_t  x bit mask i(x) y bit mask i(y)
//typedef bit_analog_mask_traits_2D<uint32_t, int8_t, 0x00FF0000, 16, 0xFF000000, 24>

/// and that's why:               32 bits   axis_t  x   y
typedef bit_analog_mask_traits_2D<uint32_t, int8_t, 16, 24> // =]
    N64_analog_mask_traits;

//! command and read a N64 controller
/*!

  available pins 

arduino 

  available pins: 0,1,2,3,4,5,6,7
  (in general arduino boards, digital pins 0..7 correspond to PD bits 0..7)

  ATmega168/328P-Arduino Pin Mapping - PD 
  https://www.arduino.cc/en/Hacking/PinMapping168
                     PD0   Digital pin 0
                     PD1   Digital pin 1
                     PD2   Digital pin 2
                     PD3   Digital pin 3
                     PD4   Digital pin 4
                     PD5   Digital pin 5
                     PD6   Digital pin 6
                     PD7   Digital pin 7

pro micro [ATmega32U4]

  available pins: 2,3,4,6

  Arduino Pin Mapping - PD 
  https://www.arduino.cc/en/Hacking/PinMapping32u4
  (OC0B/SCL/INT0)    PD0   Digital pin 3 (SCL)(PWM)
  (SDA/INT1)         PD1   Digital pin 2 (SDA)
  (RX D1/AIN1/INT2)  PD2   Digital pin 0 (RX) // nop
  (TXD1/INT3)        PD3   Digital pin 1 (TX) // no 
    (please don't use TX/RX on pro micro, I think you'd have to
    flash with an external programmer if this messes up. todo: confirm on datasheet)
  (ICP1/ADC8)        PD4   Digital pin 4
  (XCK1/#CTS)        PD5   TXLED
  (T1/#OC4D/ADC9)    PD6   Digital pin 12 // ??? unavailable on board
  (T0/OC4D/ADC10)    PD7   Digital Pin 6 (PWM)

  pins_arduino.h [pro micro]:

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
struct N64_gamepad
: public bit_gamepad<uint32_t>, public has_bitmask_analogs<N64_analog_mask_traits>
{
  typedef bit_gamepad<uint32_t>                       gamepad_base;
  typedef has_bitmask_analogs<N64_analog_mask_traits> has_analogs_base;
  typedef has_analogs_base::bit_analog_type           analog_type;
  
  // number, names and ids of buttons, as demanded by gamepad interface
  static const uint8_t N_BUTTONS = 16;
  static const String names[N_BUTTONS]; // these should have in any concrete class of gamepads
  //        0         1         2     3       4     5       6       7
  enum bid {A,        B,        Z,    start,  Dup,  Ddown,  Dleft,  Dright,
  //        8         9         10    11      12    13      14      15
            reset,   unkown,    L,    R,      Cup,  Cdown,  Cleft,  Cright};
  //        reset is a virtual pin when pressing start+R+L(at same time)
  //              this action will report both R and L pressed(1), but start will report unpressed(0) and
  //              this action will reset controller analog position, as if console has started
  //              more details https://github.com/marcosassis/gamepaduino/wiki/N64#n64-controller-reset
  
  // number and ids of directional pads, for consistency
  static const uint8_t N_DPADS = 2;
  enum did { Dpad, Cpad };

protected:

  directional dpads_[2] = {directional(4,5,6,7, *this), directional(12,13,14,15, *this)};
  analog_type analogs_[2] = {analog_type(*this), analog_type(*this)};
  char raw_dump[33];        /// 1 received bit per byte // why 33??
  uint8_t N64_pin_bit_mask; /// memory(1) vs time(1*N)
  uint8_t N64_pin;          /// arduino pin (not AVR PD)
  
public:

  /// deep copy
  N64_gamepad(const N64_gamepad& other)
  : N64_gamepad(other.id,other.N64_pin,false)
  {}

  /// main constructor
  N64_gamepad(uint8_t id, uint8_t N64_pin=3, bool init=true)
  : gamepad_base(id, N_BUTTONS, dpads_, N_DPADS),
    has_analogs_base(analogs_),
    N64_pin(N64_pin),
    N64_pin_bit_mask(digitalPinToBitMask(N64_pin))
  {
    if(init)  AndrewBrownInitialize();
  }

  virtual String* get_button_names() const {
    return names;
  }
  
  uint8_t get_N64_pin() const {
    return N64_pin;
  }
  
  template<class SerialType=DefaultHardwareSerial>
  void print(int verbose=0, SerialType& theSerialPrinter = Serial) const {
    _GAMEPAD_DEBUG("N64_gamepad::print");
    if(verbose>=3) {
      theSerialPrinter.print("\nx:\t\t");
      theSerialPrinter.print(get_x());
      theSerialPrinter.print("\ny:\t\t");
      theSerialPrinter.print(get_y());
      buttonset::print(verbose, theSerialPrinter);
    }
    else if(verbose==2) {
      theSerialPrinter.print("{ x: ");
      theSerialPrinter.print(get_x());
      theSerialPrinter.print(", y: ");
      theSerialPrinter.print(get_y());
      print_all_buttons(true,", "," }",": ",", ",0,"",theSerialPrinter);
    }
    else
      print_bits(buttons, verbose, theSerialPrinter);
      // same as bit_gamepag::print, print all bits of representation, including analog 
  }
  _GAMEPAD_AUX_IMPLEMENT_PRINTLN

  /// let's expose these, so user can play with protocol without messing with asm
  void AndrewBrownInitialize();
  void AndrewBrownSend(unsigned char *buffer, char length);
  void AndrewBrownGet();

  // todo: this inside calibration traits
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
  
#ifdef _GAMEPAD_N64_MULTIPLAYER
  /// each N64_gamepad has it's own single bidirectional communication/data pin
  /// (and arduino is far from fast enough to read all in parallel), so
  /// default behavior is optimal
  friend struct multiplayer<N64_gamepad>; // see multiplayer.h
#endif

protected:
  
  /// these are called by gamepad_base::read()
  virtual void latch() {
    unsigned char command[] = {0x01};
    AndrewBrownSend(command, 1);
  }
  virtual void read_imp() { // called by gamepad_base::read()
    AndrewBrownGet();
  }
  virtual void action_after_read() { // called by gamepad_base::read()
    translate_raw_data();

    if(button_is_pressed(bid::reset)) { // todo?: generalize reset gamepad function
      analog_range.xmax = analog_range.xmin = analog_range.ymax = analog_range.ymin = 0;
      // and then treat autocalibration, *if* enabled
      return;
    }
    
    int8_t aux = get_x(); // todo: generalize this analog autocalibration (traits?) 
    if(aux>analog_range.xmax) analog_range.xmax=aux-1; // and parametrize this
    if(aux<analog_range.xmin) analog_range.xmin=aux+1;
    aux = get_y();
    if(aux>analog_range.ymax) analog_range.ymax=aux-1;
    if(aux<analog_range.ymin) analog_range.ymin=aux+1;
  }

  /// memory vs time again (time inside AndrewBrown's functions is crucial)
  void translate_raw_data() {
    // "The get_N64_status function sloppily dumps its data 1 bit per byte
    // into the get_status_extended char array. It's our job to go through
    // that and put each piece neatly into the struct N64_status"
    for (int ii = 0; ii <= 16; ++ii) {
      set_button_state(ii, raw_dump[ii]);
    }
    // invert byte order for analog values (bit endianness stuff)
    for (int ii = 0; ii <= 8; ++ii) {
      set_button_state(ii+16, raw_dump[23-ii]);
    }
    for (int ii = 0; ii <= 8; ++ii) {
      set_button_state(ii+24, raw_dump[31-ii]);
    }
  }
};

/// NO custom imp. for N64 multiplayer interface, default is already optimal, see multiplayer.h
#ifdef _GAMEPAD_N64_MULTIPLAYER
typedef multiplayer<N64_gamepad>  N64_multiplayer;
#endif

}
#endif // _N64_GAMEPAD_H


