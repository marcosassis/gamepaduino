#ifndef _META_MIDI_TYPES_H
#define _META_MIDI_TYPES_H

namespace meta { namespace midi {
  

/// midi basic types are just the message bytes, as defined by MIDI protocol
/// status_byte and data_byte assume that correct first bit is assembled on
/// these next message-specific meta::midi types
typedef uint8_t status_byte;  /// 1_______
typedef uint8_t data_byte;    /// 0_______


/// Channel Voice Messages [nnnn = 0-15 (MIDI Channel Number 1-16)] 
/**
                                                   ----- voice messages
                       ---- channel messages -----|
                      |                            ----- mode messages
                      |
    MIDI messages ----| 
                      |                            ---- common messages         [todo]
                       ----- system messages -----|---- real-time messages      [todo]
                                                   ---- exclusive messages      [todo]
                      
                      [ chart by https://users.cs.cf.ac.uk/Dave.Marshall/Multimedia/node158.html ]
*/
union voice_message_t {
  
  status_byte mmmm____;
  
  enum event {
    note_off                = 0x80, /// B10000000,
    note_on                 = 0x90, /// B10010000,
    polyphonic_key_pressure = 0xA0, /// B10100000, // aftertouch
    control_change          = 0xB0, /// B10110000,
    program_change          = 0xC0, /// B11000000,
    channel_pressure        = 0xD0, /// B11010000, // aftertouch
    pitch_bend              = 0xE0, /// B11100000
  };
  
  /// convert from data_byte mask out last 4 bits
  /// and set first bit to 1
  voice_message_t(data_byte datavm)
  : mmmm____( (datavm & 0xF0) | 0x80 )
  {}
  
  /// as required by USB MIDI
  data_byte midi_event_type() const {
    return mmmm____ >> 4; /// todo: complete table (chap. 4 spec)
    /// todo: put this in a generic usb midi interface
  }
};


/// MIDI channels
union channel_t {
  
  status_byte ____nnnn;
  
  enum MIDI_channel {
    _1, /// ____nnnn = 0
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _10,
    _11,
    _12,
    _13,
    _14,
    _15,
    _16
  };
  
  /// convert from data_byte mask out first 4 bits
  channel_t(data_byte datac)
  : ____nnnn(datac & 0x0F)
  {}
  
  status_byte assemble_status_byte(voice_message_t message) const {
    return message.mmmm____ |
                   ____nnnn;
  }
};


/// http://computermusicresource.com/midikeys.html
union midi_octave_t {
  
  /// 12 * midi_octave_t::value + note_t::note == the midi note data_byte
  uint8_t value; 
  
  enum {
    minus_2, /// value = 0
    minus_1,
    _0,
    _1,
    _2,
    _3,
    _4,      /// value = 6
    _5,
    _6,
    _7,
    _8       /// value = 10
  } C3_60 ;
  
  enum {
    minus_1, /// value = 0
    _0,
    _1,
    _2,
    _3,
    _4,      /// value = 5
    _5,
    _6,
    _7,
    _8,
    _9       /// value = 10
  } C4_60 ;
  
  midi_octave_t(uint8_t val)
  : value(val)
  {}
};


/// western music theory generally divides the octave into a series of twelve tones, called a chromatic scale,
/// within which the interval between adjacent tones is called a half step or semitone. [wikipedia]
union note_t {
  
  data_byte note;
  
  enum chromatic {
    C,                /// 0
    C_sharp,          /// 1
    D_flat = C_sharp, /// 1
    D,                /// 2
    D_sharp,          /// 3
    E_flat = D_sharp, /// 3
    E,                /// 4
    F,                /// 5
    F_sharp,          /// 6
    G_flat = F_sharp, /// 6
    G,                /// 7
    G_sharp,          /// 8
    A_flat = G_sharp, /// 8
    A,                /// 9
    A_sharp,          /// 10
    B_flat = A_sharp, /// 10
    B                 /// 11
  };
  
  /// see midi_octave_t
  data_byte operator()(midi_octave_t octave) {
    return note + (12 * octave.value);
  }
};


/// pitch bend non-coded (decimal 0..8192..16383)
union pitch_bend_t {
  
  uint16_t value;
  
  enum {
    MIN_BEND     = 0x0000,
    CENTER_PITCH = 0x2000, /// DEC: 8192
    MAX_BEND     = 0x3FFF  /// DEC: 16383
  };
  
  pitch_bend_t(uint16_t value)
  : value (value)
  {}
  
  /// -1 .. 0 .. +1
  pitch_bend_t(float normalized_value)
  : value (normalized_value*CENTER_PITCH + CENTER_PITCH)
  {
    if(value>MAX_BEND)
      value = MAX_BEND;
  }
  
  /// -1 .. 0 .. +1
  float get_normalized() const {
    return float(value)/CENTER_PITCH - 1.;
  }
};


/// pitch bend coded
union pitch_bend_coded_t {
  
  struct {
    data_byte _lllllll;
    data_byte _mmmmmmm; // coded
  };
  
  uint16_t coded_value; // coded
  
  pitch_bend_coded_t(pitch_bend_t non_coded)
  : pitch_bend_coded_t(non_coded.value)
  {}
  
  pitch_bend_coded_t(uint16_t non_coded_value)
  : _lllllll( non_coded_value    &0x007F),
    _mmmmmmm((non_coded_value>>8)&0x007F)
  {}
  
  pitch_bend_coded_t(data_byte dataa, data_byte datab)
  : _lllllll(dataa & 0x7F),
    _mmmmmmm(datab & 0x7F)
  {}
  
  pitch_bend_t get_non_coded() const {
    return pitch_bend_t((uint16_t(_mmmmmmm)<<7) | uint16_t(_lllllll));
  }
};


}}

#endif // _META_MIDI_TYPES_H
