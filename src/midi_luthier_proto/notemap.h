#ifndef _META_MIDI_NOTEMAP_H
#define _META_MIDI_NOTEMAP_H

namespace meta { namespace midi {

struct notemap
  
  typedef int8_t interval_type;
  
  enum {
    DONT_PLAY = -128
  };
  
  interval_type*  interval_map;
  
  notemap(interval_type interval_map[])
  : interval_map(interval_map)
  {}
  
  interval_type get_interval(uint8_t i) const {
    return (i>=get_n_buttons()) ? DONT_PLAY : interval_map[i];
  }
  
  void set_interval(uint8_t i, interval_type interv) {
    if(i>=get_n_buttons())
      return;
    interval_map[i] = interv;
  }
};

}}
#endif // _META_MIDI_NOTEMAP_H