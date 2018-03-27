#ifndef _META_MIDI_INTERFACE_H
#define _META_MIDI_INTERFACE_H

#include "midi_types.h"

namespace meta { namespace midi {

/// https://www.midi.org/specifications/item/table-1-summary-of-midi-message
struct midi_send_interface
{
  virtual void flush() = 0;
  
  ///                                       status                data byte              data byte
  ///                                     [D7----D0]              [D7----D0]             [D7----D0]
  virtual void send             ( status_byte data1,       data_byte data2,        data_byte data3 ) = 0;
  ///                                      1_______                0_______               0_______

  /// channel voice messages
  /// 
  ///   [nnnn:                0..15
  ///    MIDI channel number: 1..16]
  ///                                     [    nnnn]              [        ]             [        ]
  ///                                      1000nnnn                0kkkkkkk               0vvvvvvv
  virtual void note_off         ( channel_t channel,  data_byte note_number,    data_byte velocity )  {
    send( channel.assembly_status_byte(voice_message_t::note_off), note_number,  velocity );
  }
  ///                                      1001nnnn                0kkkkkkk               0vvvvvvv
  virtual void note_on          ( channel_t channel,  data_byte note_number,    data_byte velocity )  {
    send( channel.assembly_status_byte(voice_message_t::note_on), note_number,  velocity );
  }
  
  /// coded values version                 1110nnnn                0lllllll               0mmmmmmm
  virtual void pitch_bend       ( channel_t channel,     data_byte _lllllll,    data_byte _mmmmmmm )  {
    send( channel.assembly_status_byte(voice_message_t::pitch_bend), _lllllll,  _mmmmmmm );
  }
  /// non-virtual helper                   1110nnnn                this version is easy, non coded
  void pitch_bend               ( channel_t channel,  pitch_bend_t pb = pitch_bend_t::CENTER_PITCH )  {
    pitch_bend_coded_t  pbc(pb.value);
    pitch_bend( channel,  pbc._lllllll,  pbc._mmmmmmm );
  }
  
  ///                                      1010nnnn                0lllllll               0vvvvvvv
  virtual void polyphonic_key_pressure
                                ( channel_t channel,  data_byte note_number,    data_byte press_val )  {
    send( channel.assembly_status_byte(voice_message_t::polyphonic_key_pressure), note_number,  press_val );
  }
  ///                                      1101nnnn                0vvvvvvv
  virtual void channel_pressure ( channel_t channel,    data_byte press_val )  {
    send( channel.assembly_status_byte(voice_message_t::channel_pressure), press_val, 0 );
  }
  /// non-virtual aliases                  1010nnnn                0lllllll               0vvvvvvv
  void aftertouch               ( channel_t channel,  data_byte note_number,   data_byte press_val )  {
    polyphonic_key_pressure(channel, note_number, press_val);
  }
  /// non-virtual aliases                  1101nnnn                0vvvvvvv
  void aftertouch               ( channel_t channel,    data_byte press_val )  {
    channel_pressure(channel, press_val);
  }
  
  ///                                      1011nnnn                0ccccccc               0vvvvvvv
  virtual void control_change   ( channel_t channel,  data_byte ctrl_number,  data_byte ctrl_value )  {
    send( channel.assembly_status_byte(voice_message_t::control_change), ctrl_number,  ctrl_value );
  }
  ///                                      1100nnnn                0ppppppp                  
  virtual void program_change   ( channel_t channel,  data_byte new_program_number )  {
    send( channel.assembly_status_byte(voice_message_t::program_change), new_program_number,  0 );
  }
};

}}

#endif//_META_MIDI_INTERFACE_H
