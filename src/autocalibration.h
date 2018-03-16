#ifndef _GAMEPAD_AUTOCALIBRATION_H
#define _GAMEPAD_AUTOCALIBRATION_H

//#include "analog.h"

namespace gamepad {

template<typename AxisValueType, uint8_t Dimensions=2>
struct calibration_abstract
{
  typedef AxisValueType axis_value_type;
  static const uint8_t N_AXES = Dimensions;
  static_assert(N_AXES>=1, "number of axis bust be positive");
  typedef float float_t;
  
  virtual  float_t  get_max(uint8_t i) const = 0;
  virtual  void     set_max(uint8_t i, float_t v) = 0;
  virtual  float_t  get_min(uint8_t i) const = 0;
  virtual  void     set_min(uint8_t i, float_t v) = 0;
  
  /// 0.0 == 0% ---- 1.0 == 100%
  virtual  float_t  get_cal(uint8_t i)      const = 0;
  virtual  void     set_cal(uint8_t i, float_t v) = 0;
  
  /*virtual  void copy (const analog_abstract& other) {
    for(uint8_t ii=0; ii<N_AXES; ++ii)
      set(ii, other.get(ii));
  }*/

  /*float_t operator[](uint8_t i) const {
    return get(i);
  }*/
  
  /// all inline, so they're just aliases
  float_t get_x_cal() const {
    return get_cal(0);
  }
  void set_x_cal(float_t x) {
    return set_cal(0,x);
  }
  /// [*et_y_cal and *et_z_cal only compile when N_AXES corresponds]
  float_t get_y_cal() const {
    static_assert(N_AXES>=2, "get_y_cal is not available for class instances with 1 dimension");
    return get_cal(1);
  }
  void set_y_cal(float_t y) {
    static_assert(N_AXES>=2, "set_y_cal is not available for class instances with 1 dimension");
    return set_cal(1,y);
  }
  float_t get_z_cal() const {
    static_assert(N_AXES>=3, "get_z_cal is not available for class instances with N_AXES<3");
    return get_cal(2);
  }
  void set_z_cal(float_t z) {
    static_assert(N_AXES>=3, "set_z_cal is not available for class instances with N_AXES<3");
    return set_cal(2,z);
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
#endif // _GAMEPAD_AUTOCALIBRATION_H