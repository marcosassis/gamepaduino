#ifndef _META_MIDI_H
#define _META_MIDI_H

#include "midi_interface.h"

namespace meta { namespace midi {

struct meta_midi: public midi_interface {
  virtual void note_off      (channel_t     nnnn,       data_byte note_number,    data_byte velocity)   {}  
  ///                                   1000nnnn                  0kkkkkkk                  0vvvvvvv
  virtual void note_on       (channel_t     nnnn,       data_byte note_number,    data_byte velocity) {
    
  }

};

}}
#endif//_META_MIDI_H
