// multiplayer.h

#ifndef _MULTIPLAYER_h
#define _MULTIPLAYER_h

#include "gamepad.h"
#include <LinkedList.h>


template<class gamepad_type>
struct multiplayer: public gamepad_type
{
  typedef gamepad_type gamepad_t;
  typedef gamepad_type* gamepad_pointer;
  typedef LinkedList<gamepad_pointer> gamepad_list_t;

  gamepad_list_t players;

  multiplayer(const gamepad_t& base)
  : gamepad_t(base) {
    players.add(this); // first controller = this
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

  // PLEASE derive from this class and override this method for each multiplayer specific protocol (see SNES_multiplayer)
  virtual void read() {
    gamepad_t::read();
    for(uint8_t i = 1; i<players.size(); ++i)
      players.get(i)->read();
  }
};

#endif



