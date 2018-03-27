#ifndef _META_MIDI_TYPES_H
#define _META_MIDI_TYPES_H

#include "Arduino.h" // this can be more generic, but binary "literals" are useful for readability

namespace meta { namespace midi {

typedef uint8_t data_byte;
typedef uint8_t status_byte;

/// Channel Voice Messages [nnnn = 0-15 (MIDI Channel Number 1-16)] 
union voice_message_t {
  status_byte mmmm____;
  enum event {
    note_off                = B10000000,
    note_on                 = B10010000,
    polyphonic_key_pressure = B10100000, // aftertouch
    control_change          = B10110000,
    program_change          = B11000000,
    channel_pressure        = B11010000, // aftertouch
    pitch_bend              = B11100000
  };
  
  /// convert from data_byte mask out last 4 bits
  voice_message_t(data_byte datavm) {
    mmmm____ = datavm & B11110000;
  }
  
  /// as required by MIDIUSB
  data_byte midi_event_type() const {
    return mmmm____ >> 4;
  }
};

/// MIDI channels
union channel_t {
  status_byte ____nnnn;
  enum {
    MIDI_channel_1,
    MIDI_channel_2,
    MIDI_channel_3,
    MIDI_channel_4,
    MIDI_channel_5,
    MIDI_channel_6,
    MIDI_channel_7,
    MIDI_channel_8,
    MIDI_channel_9,
    MIDI_channel_10,
    MIDI_channel_11,
    MIDI_channel_12,
    MIDI_channel_13,
    MIDI_channel_14,
    MIDI_channel_15
  };
  
  /// convert from data_byte mask out first 4 bits
  channel_t(data_byte datac)
  : ____nnnn(datac & B00001111)
  {}
  
  status_byte assembly_status_byte(voice_message_t message) const {
    return message.mmmm____ |
                   ____nnnn;
  }
};

/// pitch bend coded
union pitch_bend_coded_t {
  struct {
    data_byte _lllllll;
    data_byte _mmmmmmm; // coded
  };
  int16_t coded_value; // coded
  
  pitch_bend_coded_t(int16_t non_coded_value)
  : _lllllll( non_coded_value    &0x007F),
    _mmmmmmm((non_coded_value>>8)&0x007F)
  {}
  
  pitch_bend_coded_t(data_byte dataa, data_byte datab)
  : _lllllll(dataa & B01111111),
    _mmmmmmm(datab & B01111111)
  {}
  
  /// todo: int16_t get_non_coded() const
};

/// pitch bend non-coded (decimal 0..8192..16383)
union pitch_bend_t {
  uint16_t value;
  enum {
    MIN_BEND     = 0x0000,
    CENTER_PITCH = 0x2000,
    MAX_BEND     = 0x3FFF
  };
  
  pitch_bend_t(uint16_t value)
  : value (value)
  {}
};

}}

#endif//_META_MIDI_TYPES_H
