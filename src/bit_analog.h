#ifndef _GAMEPAD_BIT_ANALOG_H
#define _GAMEPAD_BIT_ANALOG_H

#include "bit_gamepad.h"
#include "analog.h"

namespace gamepad {


template< typename uint_Type,
          typename axis_value_Type,
          uint_Type BITMASK1, uint_Type BeginIndex1,
          uint_Type BITMASK2, uint_Type BeginIndex2 >
struct bit_analog_mask_traits_2D
{
  typedef  uint_Type        uint_type;
  typedef  axis_value_Type  axis_value_type;
  
  static const uint8_t    N_AXES = 2;
  static const uint_type  BIT_MASKS   [N_AXES] = { BITMASK1, BITMASK2 };
  static const uint_type  BIT_INDEXES [N_AXES] = { BeginIndex1, BeginIndex2 };

  typedef analog_abstract<axis_value_type, N_AXES>  analog_abstract_type; // -> STATIC_CHECK(N_AXES>=1);
  typedef analog_abstract_t&                        analog_abstract_reference;
  typedef analog_abstract_t*                        analog_abstract_pointer;
};
  

/// bitmap representation for analog types
template<class analog_mask_traits_>
struct bit_analog: public typename analog_mask_traits_::analog_abstract_type
{
  typedef          analog_mask_traits_                      analog_mask_traits;
  typedef typename analog_mask_traits::uint_type            uint_type;
  typedef typename analog_mask_traits::axis_value_type      axis_value_type;
  typedef typename analog_mask_traits::analog_abstract_type analog_abstract_type;
  typedef analog_mask_traits::analog_abstract_pointer       analog_abstract_pointer;
  typedef analog_mask_traits::analog_abstract_reference     analog_abstract_reference;
  static const uint8_t N_AXES = analog_mask_traits::N_AXES;
 
protected:
  bit_gamepad& parent;
  
public:
  
  bit_analog(bit_gamepad& aparent): parent(aparent) {}
  
  virtual value_type get(uint8_t i) const {
    
    return ( parent.get_buttons() & analog_mask_traits::BIT_MASKS[i] ) 
                               >> analog_mask_traits::BIT_INDEXES[i]  ;
  }
  virtual void set(uint8_t i, value_type v) {
    parent.set_buttons( ( v & analog_mask_traits::BIT_MASKS[i] ) 
                         << analog_mask_traits::BIT_INDEXES[i] );
  }
};

}
#endif // _GAMEPAD_BIT_ANALOG_H