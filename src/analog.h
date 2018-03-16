#ifndef _GAMEPAD_ANALOG_H
#define _GAMEPAD_ANALOG_H

//#include "buttonset.h"

namespace gamepad {

/// everything that is not binary is called analog [in this gamepad context]
template<typename value_type, uint8_t Dimensions=2>
struct analog_abstract
{
  /// representation type for each dimension/axis of a analog value
  typedef value_type value_t;
  
  /// multiaxis (or just 1), default is (x,y)
  static const uint8_t N_AXES = Dimensions;
  
  static_assert(N_AXES>=1, "number of axis bust be positive");
  
  virtual  value_type   get(uint8_t i)                const = 0;
  virtual  void         set(uint8_t i, value_type v)        = 0;
  
  virtual  void copy (const analog_abstract& other) {
    for(uint8_t ii=0; ii<N_AXES; ++ii)
      set(ii, other.get(ii));
  }

  /// operator and specific names
  /// all inline, so they're just aliases
  value_type operator[](uint8_t i) const {
    return get(i);
  }
  
  value_type get_x() const {
    return get(0);
  }
  void set_x(value_type x) {
    return set(0,x);
  }
  /// [get_y and get_z only compile when N_AXES corresponds]
  value_type get_y() const {
    static_assert(N_AXES>=2, "get_y is not available for class instances with 1 dimension");
    return get(1);
  }
  void set_y(value_type y) {
    static_assert(N_AXES>=2, "set_y is not available for class instances with 1 dimension");
    return set(1,y);
  }
  value_type get_z() const {
    static_assert(N_AXES>=3, "get_z is not available for class instances with N_AXES<3");
    return get(2);
  }
  void set_z(value_type z) {
    static_assert(N_AXES>=3, "set_z is not available for class instances with N_AXES<3");
    return set(2,z);
  }
};


/// concrete implementation for when you want to (statically) store
/// (separate) values for your analogs
/** 
    this is very common, but is also common analog bits to be already
    represented/masked within a gamepad common representation for buttons
    and analog values inside a same type (as e.g. in the same word or dword)
    
    so, this implementation is kept apart from interface(analog_abstract)
    and you must use references/pointers to analog_abstract when dealing
    directly with analog entities (see also bit_analog.h)
 */
template<typename value_type, uint8_t Dimensions=2>
struct analog_t: public analog_abstract<value_type,Dimensions>
{
  /// representation type for each dimension/axis of a analog value
  typedef value_type axis_value_type;

  /// multiaxis (or just 1), default is (x,y)
  static const uint8_t N_AXES = Dimensions;
  
  /// values statically [const sized array] stored
  axis_value_type values[N_AXES];

  typedef analog_abstract<value_type,N_AXES> analog_abstract_type;
    
  analog_t(axis_value_type values[]): values(values) {}
  
  /// i don't think we can afford to check index range every time, so user must be conscious
  virtual axis_value_type get(uint8_t i) const {
    return values[i];
  }
  virtual void set(uint8_t i, axis_value_type v) {
    values[i]=v;
  }
};


/// helper "shell" class so we don't have to obligate non-analog capable 
/// gamepad to have analog interface
/**
    if your class needs to store separate values/members for analogs, use analog_t
    
    if your class/protocol represents analogs values together with buttons
    values, let's say in specific bits of an integer (for buttons, or
    bytes/words for each analog axis), then you need to check bit_analog.h
    
    helpful: get_x(), get_y() and get_z() (when makes sense by AnalogType::N_AXIS)
    
    as with gamepad with directionals, this does not store AnalogType objects
    (both lists are OO enabled, that's because)
    
      - but if a class uses `has_analogs` name/semantics, so it has to have at least 1
      - and:
        - for directionals it makes sense that button mappings (directional is a subset)
          may be cerated dynamically
        - but this doesn't makes so much sense for multiple bit values
          - so, to optimize it all, everything on this analog interface is static
            (see also bit_analog.h)
          
 */
template<typename AnalogType, uint8_t NUMBEROF_ANALOGS=1> 
struct has_analogs
{
  typedef AnalogType                              analog_type;
  typedef typename analog_type::axis_value_type   axis_value_type;
  
  static const uint8_t N_ANALOGS = NUMBEROF_ANALOGS;
  static_assert(NUMBEROF_ANALOGS>=1, "if this class has_analogs, it have to have");
  
protected:
  analog_type* analogs;
  
public:

  has_analogs(analog_type* instancedanalogs)
  : analogs(instancedanalogs)
  {}
  
  /// i don't think we can afford to check index range every time, so user must be conscious
  const analog_type& get_analog(uint8_t i=0) const
  {
    return analogs[i];
  }
  void set_analog(analog_type& newanalog, uint8_t i=0)
  {
    analogs[i].copy(newanalog);
  }

  axis_value_type get_x(uint8_t ai=0) const {
    return get_analog(ai).get_x();
  }
  void set_x(axis_value_type x, uint8_t ai=0) {
    return get_analog(ai).get_x(x);
  }
  
  axis_value_type get_y(uint8_t ai=0) const {
    return get_analog(ai).get_y();
  }
  void set_y(axis_value_type y, uint8_t ai=0) {
    return get_analog(ai).get_y(y);
  }
  
  axis_value_type get_z(uint8_t ai=0) const {
    return get_analog(ai).get_z();
  }
  void set_z(axis_value_type z, uint8_t ai=0) {
    return get_analog(ai).get_z(z);
  }
  
  // ** enum your 'aid' [see gamepad.h] **
};

}
#endif // _GAMEPAD_ANALOG_H