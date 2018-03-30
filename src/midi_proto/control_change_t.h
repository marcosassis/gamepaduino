#ifndef _META_CONTROL_CHANGE_T_H
#define _META_CONTROL_CHANGE_T_H

#include "midi_types.h"

namespace meta { namespace midi {

/// [1] https://www.midi.org/specifications/item/table-3-control-change-messages-data-bytes-2
/** 

  (Status Bytes 176-191)
    
  [2]:
  > CC is sent when a change is made in a footswitch, expression pedal, slider, or other controller.
  > Continuous Controllers: 00=min, 40=center, 7F=max
  > Switched Controllers: 00-3F=off, 40-7F=on 

[2] http://www.personal.kent.edu/~sbirch/Music_Production/MP-II/MIDI/midi_control_change_messages.htm
[3] https://ask.audio/articles/everything-you-need-to-know-about-midi-messages-but-were-afraid-to-ask
[4] https://www.midikits.net/midi_analyser/control_and_mode_changes.htm
[5] http://computermusicresource.com/Control.Change.html
[6] http://computermusicresource.com/MIDI.Commands.html
[7] http://electronicmusic.wikia.com/wiki/Continuous_controller

*/
union control_change_t {
  
  data_byte _ccccccc;
  
  enum control_number {
                              
/// 00-1F	  Continuous Controllers, MSB 
                              
                                  /// Decimal | Binary | Value | Used As

    bank_select           = 0x00, ///  0     00000000    0-127   MSB
    lever                 = 0x01, ///  1     00000001    0-127   MSB // Modulation Wheel
    breath_controller     = 0x02, ///  2     00000010    0-127   MSB
    undefined_0           = 0x03, ///  3     00000011    0-127   MSB
    foot_controller       = 0x04, ///  4     00000100    0-127   MSB
    portamento_time       = 0x05, ///  5     00000101    0-127   MSB
    data_entry_MSB        = 0x06, ///  6     00000110    0-127   MSB
    channel_volume        = 0x07, ///  7     00000111    0-127   MSB // (formerly Main Volume)

    balance               = 0x08, ///  8     00001000    0-127   MSB
    undefined_1           = 0x09, ///  9     00001001    0-127   MSB
    pan                   = 0x0A, ///  10    00001010    0-127   MSB
    expression_controller = 0x0B, ///  11    00001011    0-127   MSB
    effect_control_1      = 0x0C, ///  12    00001100    0-127   MSB
    effect_control_2      = 0x0D, ///  13    00001101    0-127   MSB
    undefined_2           = 0x0E, ///  14    00001110    0-127   MSB
    undefined_3           = 0x0F, ///  15    00001111    0-127   MSB

    general_purpose_controller_1
                          = 0x10, ///  16    00010000    0-127   MSB
    general_purpose_controller_2
                          = 0x11, ///  17    00010001    0-127   MSB
    general_purpose_controller_3
                          = 0x12, ///  18    00010010    0-127   MSB
    general_purpose_controller_4
                          = 0x13, ///  19    00010011    0-127   MSB
    undefined_4           = 0x14, ///  20    00010100    0-127   MSB
    undefined_5           = 0x15, ///  21    00010101    0-127   MSB
    undefined_6           = 0x16, ///  22    00010110    0-127   MSB
    undefined_7           = 0x17, ///  23    00010111    0-127   MSB
    
    undefined_8           = 0x18, ///  24    00011000    0-127   MSB
    undefined_9           = 0x19, ///  25    00011001    0-127   MSB
    undefined_10          = 0x1A, ///  26    00011010    0-127   MSB
    undefined_11          = 0x1B, ///  27    00011011    0-127   MSB
    undefined_12          = 0x1C, ///  28    00011100    0-127   MSB
    undefined_13          = 0x1D, ///  29    00011101    0-127   MSB
    undefined_14          = 0x1E, ///  30    00011110    0-127   MSB
    undefined_15          = 0x1F, ///  31    00011111    0-127   MSB
    
    
/// 20-3F	  Continuous Controllers, LSB corresponding to controllers 00-3
/**
  [2]:
  > The LSB is optional, and need not be sent if fine control is not required. If not sent, the LSB is assumed to be zero.
  > Furthermore, once both MSB and LSB control messages have been sent, fine adjustments can be made by sending subsequent
  > LSB control messages only. It is not necessary to re-send the MSB if it is unchanged.
*/
                              
                                  /// Decimal | Binary | Value | Used As

    bank_select_LSB       = 0x20, ///  32    00100000    0-127   LSB [all LSB are optional]
    lever_LSB             = 0x21, ///  33    00100001    0-127   LSB // Modulation Wheel
    breath_controller_LSB = 0x22, ///  34    00100010    0-127   LSB 
    undefined_0_LSB       = 0x23, ///  35    00100011    0-127   LSB 
    foot_controller_LSB   = 0x24, ///  36    00100100    0-127   LSB 
    portamento_time_LSB   = 0x25, ///  37    00100101    0-127   LSB 
    data_entry_LSB        = 0x26, ///  38    00100110    0-127   LSB 
    channel_volume_LSB    = 0x27, ///  39    00100111    0-127   LSB  // (formerly Main Volume)

    balance_LSB           = 0x28, ///  40    00101000    0-127   LSB 
    undefined_1_LSB       = 0x29, ///  41    00101001    0-127   LSB 
    pan_LSB               = 0x2A, ///  42    00101010    0-127   LSB 
    expression_controller_LSB
                          = 0x2B, ///  43    00101011    0-127   LSB 
    effect_control_1_LSB  = 0x2C, ///  44    00101100    0-127   LSB 
    effect_control_2_LSB  = 0x2D, ///  45    00101101    0-127   LSB 
    undefined_2_LSB       = 0x2E, ///  46    00101110    0-127   LSB 
    undefined_3_LSB       = 0x2F, ///  47    00101111    0-127   LSB 

    general_purpose_controller_1_LSB
                          = 0x30, ///  48    00110000    0-127   LSB 
    general_purpose_controller_2_LSB
                          = 0x31, ///  49    00110001    0-127   LSB 
    general_purpose_controller_3_LSB
                          = 0x32, ///  50    00110010    0-127   LSB 
    general_purpose_controller_4_LSB
                          = 0x33, ///  51    00110011    0-127   LSB 
    undefined_4_LSB       = 0x34, ///  52    00110100    0-127   LSB 
    undefined_5_LSB       = 0x35, ///  53    00110101    0-127   LSB 
    undefined_6_LSB       = 0x36, ///  54    00110110    0-127   LSB 
    undefined_7_LSB       = 0x37, ///  55    00110111    0-127   LSB 
    
    undefined_8_LSB       = 0x38, ///  56    00111000    0-127   LSB 
    undefined_9_LSB       = 0x39, ///  57    00111001    0-127   LSB 
    undefined_10_LSB      = 0x3A, ///  58    00111010    0-127   LSB 
    undefined_11_LSB      = 0x3B, ///  59    00111011    0-127   LSB 
    undefined_12_LSB      = 0x3C, ///  60    00111100    0-127   LSB 
    undefined_13_LSB      = 0x3D, ///  61    00111101    0-127   LSB
    undefined_14_LSB      = 0x3E, ///  62    00111110    0-127   LSB
    undefined_15_LSB      = 0x3F, ///  63    00111111    0-127   LSB
                              
                              
/// 40-5F   Single-byte controllers
                              
                                  /// Decimal | Binary | Value | Used As

    sustain               = 0x40, ///  64    01000000    ≤63 off, ≥64 on // Damper Pedal
    portamento            = 0x41, ///  65    01000001    ≤63 off, ≥64 on
    sostenuto             = 0x42, ///  66    01000010    ≤63 off, ≥64 on
    soft_pedal            = 0x43, ///  67    01000011    ≤63 off, ≥64 on
    legato_footswitch     = 0x44, ///  68    01000100    ≤63 Normal, ≥64 Legato
    hold_2                = 0x45, ///  69    01000101    ≤63 off, ≥64 on
                                  
    sound_controllerl_1   = 0x46, ///  70    01000110    0-127   LSB // (default: Sound Variation)
    sound_controllerl_2   = 0x47, ///  71    01000111    0-127   LSB // (default: Timbre/Harmonic Intens.)
    
    sound_controllerl_3   = 0x48, ///  72    01001000    0-127   LSB // (default: Release Time)
    sound_controllerl_4   = 0x49, ///  73    01001001    0-127   LSB // (default: Attack Time)
    sound_controllerl_5   = 0x4A, ///  74    01001010    0-127   LSB // (default: Brightness)
    sound_controllerl_6   = 0x4B, ///  75    01001011    0-127   LSB // (default: Decay Time - see MMA RP-021)
    sound_controllerl_7   = 0x4C, ///  76    01001100    0-127   LSB // (default: Vibrato Rate - see MMA RP-021)
    sound_controllerl_8   = 0x4D, ///  77    01001101    0-127   LSB // (default: Vibrato Depth - see MMA RP-021)
    sound_controllerl_9   = 0x4E, ///  78    01001110    0-127   LSB // (default: Vibrato Delay - see MMA RP-021)
    sound_controllerl_10  = 0x4F, ///  79    01001111    0-127   LSB // (default undefined - see MMA RP-021)
    
    general_purpose_controller_5
                          = 0x50, ///  80    01010000    0-127   LSB 
    general_purpose_controller_6
                          = 0x51, ///  81    01010001    0-127   LSB 
    general_purpose_controller_7
                          = 0x52, ///  82    01010010    0-127   LSB 
    general_purpose_controller_8
                          = 0x53, ///  83    01010011    0-127   LSB 
                          
    portamento_control    = 0x54, ///  84    01010100    0-127   LSB 
    
    undefined_16          = 0x55, ///  85    01010101     ---    --- 
    undefined_17          = 0x56, ///  86    01010110     ---    ---
    undefined_18          = 0x57, ///  87    01010111     ---    --- 
    
    high_resolution_velocity_prefix
                          = 0x58, ///  88    01011000    0-127   LSB 
                          
    undefined_19          = 0x59, ///  89    01011001     ---    --- 
    undefined_20          = 0x5A, ///  90    01011010     ---    ---

    /// Depth (default: Reverb Send Level - see MMA RP-023)
    ///       (formerly External effects_Depth)
    effects_1             = 0x5B, ///  91    01011011    0-127   ---

    /// Depth (formerly Tremolo Depth)
    effects_2             = 0x5C, ///  92    01011100    0-127   ---

    /// Depth (default: Chorus Send Level - see MMA RP-023)
    ///       (formerly Chorus Depth)
    effects_3             = 0x5D, ///  93    01011101    0-127   ---

    /// Depth (formerly Celeste [Detune] Depth)
    effects_4             = 0x5E, ///  94    01011110    0-127   ---

    /// Depth (formerly Phaser Depth)
    effects_5             = 0x5F, ///  95    01011111    0-127   ---

    data_increment        = 0x60, ///  96    01100000     N/A    --- // (Data Entry +1) (see MMA RP-018)
    data_decrement        = 0x61, ///  97    01100001     N/A    --- // (Data Entry -1) (see MMA RP-018)
    
    /// Non-Registered Parameter Number (NRPN)
    /// may be used as needed by manufacturers. They should be described in the owner's manual
    /// The parameter value is specified by a subsequent Data entry, Data Increment or Data Decrement message.
    NRPN_LSB              = 0x62, ///  98    01100010    0-127	 LSB
    NRPN_MSB              = 0x63, ///  99    01100011    0-127	 MSB
    
    /// Registered Parameter Number (RPN)
    /// are an extension to the Control Change message for setting additional parameters.
    RPN_LSB               = 0x64, ///  100   01100010    0-127	 LSB
    RPN_MSB               = 0x65, ///  101   01100011    0-127	 MSB
    
    undefined_16          = 0x66, ///  102   01100110     ---    --- 
    undefined_17          = 0x67, ///  103   01010110     ---    ---
    
    undefined_18          = 0x68, ///  104   01010111     ---    --- 
    undefined_16          = 0x69, ///  105   01010101     ---    --- 
    undefined_17          = 0x6A, ///  106   01010110     ---    ---
    undefined_18          = 0x6B, ///  107   01010111     ---    --- 
    undefined_16          = 0x6C, ///  108   01010101     ---    --- 
    undefined_17          = 0x6D, ///  109   01010110     ---    ---
    undefined_18          = 0x6E, ///  110   01010111     ---    --- 
    undefined_16          = 0x6F, ///  111   01010101     ---    --- 
    
    undefined_17          = 0x70, ///  112   01010110     ---    ---
    undefined_18          = 0x71, ///  113   01010111     ---    --- 
    undefined_16          = 0x72, ///  114   01010101     ---    --- 
    undefined_17          = 0x73, ///  115   01010110     ---    ---
    undefined_18          = 0x74, ///  116   01010111     ---    --- 
    undefined_16          = 0x75, ///  117   01010101     ---    --- 
    undefined_17          = 0x76, ///  118   01010110     ---    ---
    undefined_18          = 0x77, ///  119   01010111     ---    --- 
    
    
    /// Note:	Controller numbers 120-127 are reserved for Channel Mode Messages,
    /// which rather than controlling sound parameters, affect the channel's operating mode.
    
                                  /// Decimal | Binary | Value | Used As
    
    all_sound_off         = 0x78, ///  120   01111000      0     ---
    reset_all_controllers = 0x79, ///  121   01111001      0     --- // (See MMA RP-015)
    local_control         = 0x7A, ///  122   01111010   0 off, 127 on	---
    all_notes_off         = 0x7B, ///  123   01111011      0     ---
    omni_mode_off         = 0x7C, ///  124   01111100      0     --- // (+ all notes off)
    omni_mode_on          = 0x7D, ///  125   01111101      0     --- // (+ all notes off)
    
    /// This equals the number of channels, or zero if the number of channels equals the number of voices in the receiver.
    mono_mode_on          = 0x7E, ///  126   01111110      *     --- // (+ poly off, + all notes off)
    
    poly_mode_on          = 0x7F, ///  127   01111111      0     --- // (+ mono off, + all notes off)

    

Table 3a: Registered Parameter Numbers

To set or change the value of a Registered Parameter:

1. Send two Control Change messages using Control Numbers 101 (65H) and 100 (64H) to select the desired Registered Parameter Number, as per the following table.

2. To set the selected Registered Parameter to a specific value, send a Control Change messages to the Data Entry MSB controller (Control Number 6). If the selected Registered Parameter requires the LSB to be set, send another Control Change message to the Data Entry LSB controller (Control Number 38).

3. To make a relative adjustment to the selected Registered Parameter's current value, use the Data Increment or Data Decrement controllers (Control Numbers 96 and 97).

Parameter Number	Parameter Function	Data Entry Value
MSB: Control 101 (65H) Value	LSB: Control 100 (64H) Value
00H	00H	Pitch Bend Sensitivity	MSB = +/- semitones
LSB =+/--cents
01H	Channel Fine Tuning 
(formerly Fine Tuning - see MMA RP-022)	Resolution 100/8192 cents 
00H 00H = -100 cents 
40H 00H = A440 
7FH 7FH = +100 cents
02H	Channel Coarse Tuning 
(formerly Coarse Tuning - see MMA RP-022)	Only MSB used 
Resolution 100 cents
00H = -6400 cents 
40H = A440 
7FH = +6300 cents
03H	Tuning Program Change	Tuning Program Number
04H	Tuning Bank Select	Tuning Bank Number
05H 	Modulation Depth Range 
(see MMA General MIDI Level 2 Specification)	For GM2, defined in GM2 Specification. 
For other systems, defined by manufacturer
 ...	 ...	 All RESERVED for future MMA Definition	 ...
3DH (61)	Three Dimensional Sound Controllers
00H	AZIMUTH ANGLE 	See RP-049
01H	ELEVATION ANGLE 	See RP-049
02H	GAIN 	See RP-049
03H	DISTANCE RATIO 	See RP-049
04H	MAXIMUM DISTANCE 	See RP-049
05H	GAIN AT MAXIMUM DISTANCE 	See RP-049
06H	REFERENCE DISTANCE RATIO 	See RP-049
07H	PAN SPREAD ANGLE 	See RP-049
08H	ROLL ANGLE 	See RP-049
 ...	 ...	 All RESERVED for future MMA Definition	 ...
7FH	7FH	Null Function Number for RPN/NRPN	Setting RPN to 7FH,7FH will disable the data entry, data increment, and data decrement controllers until a new RPN or NRPN is selected.
    
}}
#endif // _META_CONTROL_CHANGE_T_H