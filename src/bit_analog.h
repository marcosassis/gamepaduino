#ifndef _GAMEPAD_BIT_ANALOG_H
#define _GAMEPAD_BIT_ANALOG_H

#include "bit_gamepad.h"
#include "analog.h"

namespace gamepad {

/// this bit_analog infrastructure only makes sense if everything is compiler-time accessible...
template< typename uint_Type,
          typename axis_value_Type,
          uint_Type BITMASK1, uint_Type BeginIndex1, /// oh, I'm not liking this, must have a way
          uint_Type BITMASK2, uint_Type BeginIndex2 >
struct bit_analog_mask_traits_2D
{
  typedef  uint_Type        uint_type;
  typedef  axis_value_Type  axis_value_type;
  
  static const uint8_t    N_AXES = 2;
  static const uint_type  BIT_MASKS   [N_AXES];
  static const uint_type  BIT_INDEXES [N_AXES];

  typedef analog_abstract<axis_value_type, N_AXES>  analog_abstract_type; // -> static_assert(N_AXES>=1);
  typedef analog_abstract_type&                     analog_abstract_reference;
  typedef analog_abstract_type*                     analog_abstract_pointer;
};

template< typename U, typename A, U BM1, U I1, U BM2, U I2 >
static const U 
bit_analog_mask_traits_2D<U,A,BM1,I1,BM2,I2>::BIT_MASKS[2]
 = { BM1, BM2 };

template< typename U, typename A, U BM1, U I1, U BM2, U I2 >
static const U 
bit_analog_mask_traits_2D<U,A,BM1,I1,BM2,I2>::BIT_INDEXES[2]
 = { I1, I2 };


/// bitmap representation for analog types
template<class analog_mask_traits_>
struct bit_analog_t: public analog_mask_traits_::analog_abstract_type
{
  typedef          analog_mask_traits_                            analog_mask_traits;
  typedef typename analog_mask_traits::uint_type                  uint_type;
  typedef          bit_gamepad<uint_type>                         bit_gamepad_type;
  typedef typename analog_mask_traits::axis_value_type            axis_value_type;
  typedef typename analog_mask_traits::analog_abstract_type       analog_abstract_type;
  //typedef typename analog_mask_traits::analog_abstract_pointer    analog_abstract_pointer;
  //typedef typename analog_mask_traits::analog_abstract_reference  analog_abstract_reference;
  static const uint8_t N_AXES = analog_mask_traits::N_AXES;
  
  /// ... because we don't store anything in object ;)
  ///     - values are already stored on parent->buttons. since they're always at same place,
  ///       compiler-time accessible values(static const)/types will make everything lean.
  ///       see below how to use bit_analog::analog_mask_traits:: values
  ///     - but if you need to store analog values apart on your object, use something like
  ///       `analog_t myanalogs[N_ANALOGS]`, see analog.h
 
protected:
  bit_gamepad_type& parent;
  
public:
  
  bit_analog_t(bit_gamepad_type& aparent): parent(aparent) {}
  
  virtual axis_value_type get(uint8_t i) const {
    return ( parent.get_buttons() & (analog_mask_traits::BIT_MASKS[i]) ) 
                               >> (analog_mask_traits::BIT_INDEXES[i])  ;
  }
  virtual void set(uint8_t i, axis_value_type v) {
    parent.set_buttons( ( v & (analog_mask_traits::BIT_MASKS[i]) ) 
                         << (analog_mask_traits::BIT_INDEXES[i]) );
  }
};


template<class analog_mask_traits_, uint8_t NUMBEROF_ANALOGS=1> 
struct has_bitmask_analogs
: public has_analogs< bit_analog_t<analog_mask_traits_> , NUMBEROF_ANALOGS >
{
  typedef analog_mask_traits_                         analog_mask_traits;
  typedef bit_analog_t<analog_mask_traits_>           bit_analog_type;
  typedef typename bit_analog_type::axis_value_type   axis_value_type;
  typedef typename bit_analog_type::uint_type         uint_type;
  typedef bit_gamepad<uint_type>                      bit_gamepad_type;
  
  static const uint8_t N_ANALOGS = NUMBEROF_ANALOGS;
  static_assert(NUMBEROF_ANALOGS>=1, "if this class has_analogs, it have to have");
  
  typedef has_analogs<bit_analog_type, N_ANALOGS> has_analogs_base;
  
  has_bitmask_analogs(bit_analog_type* analogs_)
  : has_analogs_base(analogs_)
  {}
};

}
#endif // _GAMEPAD_BIT_ANALOG_H