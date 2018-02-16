#define _GAMEPAD_JOYSTICK_DEBUG
#include "gamepad_joystick_hid.h"

#define _GAMEPAD_DEFINE_N64_HID
#include "N64_gamepad.h"

#define _GAMEPAD_DEFINE_SNES_HID
#include "SNES_gamepad.h"

//#define _METAPAD_DEBUG
#include "metapad.h"

/*
const uint8_t CLOCK_PIN = 9;//4;
const uint8_t LATCH_PIN = 8;//5;
const uint8_t DATA_PIN1 = 7;
const uint8_t DATA_PIN2 = 6;
const uint8_t DATA_PIN3 = 5;
const uint8_t DATA_PIN4 = 4;

SNES_hid p1hid(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
SNES_hid p2hid(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);
SNES_hid p3hid(3, DATA_PIN3, CLOCK_PIN, LATCH_PIN);
SNES_hid p4hid(4, DATA_PIN4, CLOCK_PIN, LATCH_PIN);

/*
//                      0  1  2       3      4   5     6     7      8  9  10 11
//              enum b {B, Y, select, start, up, down, left, right, A, X, L, R};
const int8_t blues[] = {3, 10, -128, -128, -128, -128, -128, -128, 5, 7, 0, 6}; 
SNES_gamepad p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
SNES_gamepad p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);
SNES_gamepad p3(3, DATA_PIN3, CLOCK_PIN, LATCH_PIN);
SNES_gamepad p4(4, DATA_PIN4, CLOCK_PIN, LATCH_PIN);
metapad<SNES_gamepad> p1midi(p1, blues, 69, 0, 100); // 69=A4 ref.: midi specs
metapad<SNES_gamepad> p2midi(p2, blues, 69, 1, 64); // 69=A4 ref.: midi specs
metapad<SNES_gamepad> p3midi(p3, blues, 69, 2, 100); // 69=A4 ref.: midi specs
metapad<SNES_gamepad> p4midi(p4, blues, 69, 3, 64); // 69=A4 ref.: midi specs
*/

//SNES_multiplayer mp(p1midi, p2midi);
//SNES_multiplayer<SNES_hid> mp(p1hid, p2hid);

//multiplayer<SNES_hid> mp(p1hid, p2hid, p3hid, p4hid);
//multiplayer<metapad<SNES_gamepad> > mp(p1midi, p2midi, p3midi, p4midi);


//N64_gamepad p1n64(1, 3);
N64_hid p1n64(1, 3);

void setup() {
  Serial.begin(9600);
  Serial.println("hello"); // don't know why never can see this
}

void loop() {
  
  //p1hid.read();
  //p2hid.read();
  //p1midi.read();
  //p2midi.read();
  //p3midi.read();
  //p4midi.read();
  //XMIDIUSB.flush();
  //mp.read();

  //XMIDIUSB.note_on(0,69,100);

  p1n64.read();
  /*
  p1n64.print();
  Serial.print("\tx: "); print_bits(p1n64.get_x());
  Serial.print("\ty: "); print_bits(p1n64.get_y());
  Serial.print("\tx: "); Serial.print(p1n64.get_x());  
  Serial.print(" y: "); Serial.print(p1n64.get_y());
  Serial.print("\txmax: "); Serial.print(p1n64.analog_range.xmax);  
  Serial.print(" xmin: "); Serial.print(p1n64.analog_range.xmin);  
  Serial.print(" ymax: "); Serial.print(p1n64.analog_range.ymax);  
  Serial.print(" ymin: "); Serial.print(p1n64.analog_range.ymin);  
  Serial.print("\txcal: "); Serial.print(p1n64.get_x_cal());  
  Serial.print(" ycal: "); Serial.println(p1n64.get_y_cal());  
  delay(25);
  */
  
  //Serial.println(p1n64.get_buttons(),BIN);
  //Serial.print(p1n64.get_x(),BIN); Serial.print(" , "); Serial.println(p1n64.get_y(),BIN); 
  //Serial.print(p1n64.get_x()); Serial.print(" , "); Serial.println(p1n64.get_y()); 
  //Serial.println(p1n64.get_buttons(), BIN);
  
  
/*
  for (uint8_t i = 0; i < p1.get_n_buttons(); ++i) {

    if (p1.button_state_has_changed(i)) {

      Serial.print(i);
      Serial.print("\t:\t");
      Serial.print(p1.get_button_names()[i]);
      Serial.print("\t:\t");
      Serial.print(p1.get_button_state(i));
      Serial.print("\t:\t");
      Serial.print(p1.button_state_has_changed(i));
      Serial.print("\t:\t");
      Serial.print(p1.buttons_changed_mask(), BIN);
      Serial.print("\t:\t");
      Serial.print(p1.get_buttons_last(), BIN);
      Serial.print("\t:\t");
      Serial.println(p1.get_buttons(), BIN);

    }//end if(p1.button_state_has_changed(i))

  }//end for
  */
}


