#ifndef _GAMEPAD_AUTOCALIBRATION_H
#define _GAMEPAD_AUTOCALIBRATION_H

#include "analog.h"
#include "active_gamepad.h"

namespace gamepad { namespace cal {

template<class HasAnalogsType>
struct stores_analogs_limits: public HasAnalogsType
{
  typedef          HasAnalogsType                     has_analogs_base;
  typedef typename has_analogs_base::analog_type      analog_type;
  typedef typename has_analogs_base::axis_value_type  axis_value_type;
  
  static const uint8_t N_ANALOGS = has_analogs_base::N_ANALOGS; // static_assert(NUMBEROF_ANALOGS>=1);
  static const uint8_t N_AXES = analog_type::N_AXES; // static_assert(N_AXES>=1);
  
  typedef analog_t<axis_value_type,N_AXES>            analog_limits_type; /// store limits
  
  analog_limits_type analogs_mins_[N_ANALOGS];
  analog_limits_type analogs_maxs_[N_ANALOGS];
  
  stores_analogs_limits(const has_analogs_base& base)
  : has_analogs_base(base)
  {}
  
  void reset_limits(axis_value_type avalue=axis_value_type()) {
    for(uint8_t analogindex=0; analogindex<N_ANALOGS; ++analogindex) {
      for(uint8_t axisindex=0; axisindex<N_AXES; ++axisindex) {
        set_max(avalue,axisindex,analogindex);
        set_min(avalue,axisindex,analogindex);
      }
    }
  }
  
  axis_value_type get_max(uint8_t axisindex, uint8_t analogindex=0) const {
    return analogs_maxs_[analogindex][axisindex];
  }
  axis_value_type get_min(uint8_t axisindex, uint8_t analogindex=0) const {
    return analogs_mins_[analogindex][axisindex];
  }
  void set_max(axis_value_type v, uint8_t axisindex, uint8_t analogindex=0) const {
    analogs_maxs_[analogindex].set(axisindex,v);
  }
  void set_min(axis_value_type v, uint8_t axisindex, uint8_t analogindex=0) const {
    analogs_mins_[analogindex].set(axisindex,v);
  }
  
};

template< class HasAnalogsActiveGamepadType, long RANGE_THRESLHOLD=0 >
struct tracks_analogs_limits: public stores_analogs_limits<HasAnalogsActiveGamepadType>
{
  typedef HasAnalogsActiveGamepadType             gamepad_type;
  typedef stores_analogs_limits<gamepad_type>     stores_analogs_limits_base;
  typedef typename gamepad_type::analog_type      analog_type;
  typedef typename gamepad_type::axis_value_type  axis_value_type;
  
  static const uint8_t N_ANALOGS = gamepad_type::N_ANALOGS; // static_assert(NUMBEROF_ANALOGS>=1);
  static const uint8_t N_AXES = analog_type::N_AXES; // static_assert(N_AXES>=1);
  
  typedef analog_t<axis_value_type,N_AXES>            analog_limits_type; /// store limits
 
  tracks_analogs_limits(const gamepad_type& other)
  : stores_analogs_limits_base(other)
  {}
  
  virtual void action_any_state_changed() {
    _GAMEPAD_DEBUG("tracks_analogs_limits::action_any_state_changed");
    gamepad_type::action_any_state_changed();
    axis_value_type aux;
    for(uint8_t analogindex=0; analogindex<N_ANALOGS; ++analogindex)  {
      for(uint8_t axisindex=0; axisindex<N_AXES; ++axisindex) {
        aux = get_analog(analogindex)[axisindex];
        if(aux > get_max(axisindex, analogindex))
          set_max(aux-RANGE_THRESLHOLD,axisindex,analogindex);
        else if(aux < get_min(axisindex, analogindex))
          set_min(aux-RANGE_THRESLHOLD,axisindex,analogindex);
      }
    }
  }  
};




  // todo: this inside calibration helper
  /*
  float get_x_cal() {
    float x = get_x();
    if(x==0 || analog_range.xmax==0 || analog_range.xmin==0)
      return 0.;
    if(x>0)
      return x/float(analog_range.xmax);
    return -x/float(analog_range.xmin);
  }
  float get_y_cal() {
    float y = get_y();
    if(y==0 || analog_range.ymax==0 || analog_range.ymin==0)
      return 0.;
    if(y>0)
      return y/float(analog_range.ymax);
    return -y/float(analog_range.ymin);
  }
  */
  
  
  /*
  
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
  
  /*
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
*/

}}
#endif // _GAMEPAD_AUTOCALIBRATION_H