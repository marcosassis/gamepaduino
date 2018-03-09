#ifndef _MULTIPLAYER_H
#define _MULTIPLAYER_H

#include <LinkedList.h>

namespace gamepad {
  
/// multiplayer class is not a gamepad [anymore], it is a collection of gamepads (simplest)
/// friend this class instantiated for your <gamepad_type> inside gamepad_type definition
/// see SNES_gamepad.h for an example
template<class gamepad_type>
struct multiplayer//: public gamepad_type
{
  typedef gamepad_type gamepad_t;
  typedef gamepad_type* gamepad_pointer;
  typedef LinkedList<gamepad_pointer> gamepad_list_t;

  gamepad_list_t players;

  /*
  multiplayer(const gamepad_t& base)
  : gamepad_t(base) {
    players.add(this); // first controller = this
  }
  */
  
  multiplayer(const gamepad_t& p1) {
    players.add(&p1);
  }
  multiplayer(const gamepad_t& p1, const gamepad_t& p2)
  : multiplayer(p1) {
    players.add(&p2);
  }
  multiplayer(const gamepad_t& p1, const gamepad_t& p2, const gamepad_t& p3)
  : multiplayer(p1,p2) {
    players.add(&p3);
  }
  multiplayer(const gamepad_t& p1, const gamepad_t& p2, const gamepad_t& p3, const gamepad_t& p4)
  : multiplayer(p1,p2,p3) {
    players.add(&p4);
  }

  virtual void read() {
    //naive_read_all();
    latch_all_read_all();
  }
  
  void naive_read_all() {
    //gamepad_t::read();
    for(uint8_t i = 0; i<players.size(); ++i)
      players.get(i)->read();
  }
  
  void latch_all_read_all() {
    for(uint8_t p = 0; p<players.size(); ++p)
      this->players.get(p)->action_before_read();
    
    noInterrupts(); // latch and read all on the same batch
    latch_all();  
    read_all();     // (it's not necessarily on the same latch,
    interrupts();   // but at least without interruptions in the middle)
    
    for(uint8_t p = 0; p<players.size(); ++p)
      this->players.get(p)->action_after_read();
  }
  /* gamepad::read is:
  virtual void read() {
    action_before_read();
    noInterrupts();
    latch();
    read_imp();
    interrupts();
    action_after_read();
  }
  */
  
  /// PLEASE derive from this class and override these methods for each
  /// multiplayer specific protocol (see SNES_multiplayer)
  virtual void latch_all() {
   for(uint8_t p = 0; p<players.size(); ++p)
    this->players.get(p)->latch();   
  }
  virtual void read_all() {
   for(uint8_t p = 0; p<players.size(); ++p)
    this->players.get(p)->read();   
  }
};

}
#endif //_MULTIPLAYER_H



