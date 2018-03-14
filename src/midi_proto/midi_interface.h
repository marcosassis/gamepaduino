#ifndef _MIDI_INTERFACE_H
#define _MIDI_INTERFACE_H

#include "Arduino.h" // this can be more generic, but binary "literals" are useful for readability

namespace meta { namespace midi {

typedef uint8_t data_byte; // maybe union with int8_t (and assert first bit is 0)
typedef uint8_t status_byte; // idem (first bit have to be 1)

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
    MIDI_channel_15,
    MIDI_channel_16
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


/// https://www.midi.org/specifications/item/table-1-summary-of-midi-message
struct midi_send_interface
{
  virtual void flush() = 0;
  
  ///                                       status                data byte              data byte
  ///                                     [D7----D0]              [D7----D0]             [D7----D0]
  virtual void send             ( status_byte data1,       data_byte data2,        data_byte data3 ) = 0;

  /// channel voice messages
  /// 
  ///   [nnnn:                0..15
  ///    MIDI channel number: 1..16]
  ///                                     [    nnnn]              [        ]             [        ]
  ///                                      1000nnnn                0kkkkkkk               0vvvvvvv
  virtual void note_off         ( channel_t channel,  data_byte note_number,    data_byte velocity )
  {
    send( channel.assembly_status_byte(voice_message_t::note_off),
          note_number,  velocity );
  }
  ///                                      1001nnnn                0kkkkkkk               0vvvvvvv
  virtual void note_on          ( channel_t channel,  data_byte note_number,    data_byte velocity )
  {
    send( channel.assembly_status_byte(voice_message_t::note_on),
          note_number,  velocity );
  }
  
  /// coded values version                 1110nnnn                0lllllll               0mmmmmmm
  virtual void pitch_bend       ( channel_t channel,     data_byte _lllllll,    data_byte _mmmmmmm )
  {
    send( channel.assembly_status_byte(voice_message_t::pitch_bend),
          _lllllll,  _mmmmmmm );
  }
  /// non-virtual helper                   1110nnnn                this version is easy, non coded
  void pitch_bend               ( channel_t channel,  pitch_bend_t pb = pitch_bend_t::CENTER_PITCH )
  {
    pitch_bend_coded_t  pbc(pb.value);
    pitch_bend( channel,  pbc._lllllll,  pbc._mmmmmmm );
  }
  
  ///                                      1010nnnn                0lllllll               0vvvvvvv
  virtual void polyphonic_key_pressure
                                ( channel_t channel,  data_byte note_number,    data_byte press_val )
  {
    send( channel.assembly_status_byte(voice_message_t::polyphonic_key_pressure),
          note_number,  press_val );
  }
  ///                                      1101nnnn                0vvvvvvv
  virtual void channel_pressure ( channel_t channel,    data_byte press_val )
  {
    send( channel.assembly_status_byte(voice_message_t::channel_pressure),
          press_val, 0 );
  }
  /// non-virtual aliases                  1010nnnn                0lllllll               0vvvvvvv
  void aftertouch               ( channel_t channel,  data_byte note_number,   data_byte press_val )
  {
    polyphonic_key_pressure(channel,note_number,press_val);
  }
  /// non-virtual aliases                  1101nnnn                0vvvvvvv
  void aftertouch               ( channel_t channel,    data_byte press_val )
  {
    channel_pressure(channel,press_val);
  }
  
  ///                                      1011nnnn                0ccccccc               0vvvvvvv
  virtual void control_change   ( channel_t channel,  data_byte ctrl_number,  data_byte ctrl_value )
  {
    send( channel.assembly_status_byte(voice_message_t::control_change),
          ctrl_number,  ctrl_value );
  }

  ///                                      1100nnnn                0ppppppp                  
  virtual void program_change   ( channel_t channel,  data_byte new_program_number )
  {
    send( channel.assembly_status_byte(voice_message_t::program_change),
          new_program_number,  0 );
  }
};

}}

#endif//_MIDI_INTERFACE_H
