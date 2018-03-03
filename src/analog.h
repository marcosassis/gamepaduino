#ifndef _GAMEPAD_ANALOG_H
#define _GAMEPAD_ANALOG_H

//#include "buttonset.h"
#include "Loki/static_check.h" // thank you, a.alexandrescu!

namespace gamepad {


template<typename value_type, uint8_t Dimensions=2>
struct analog {
  typedef value_type value_t;
  static const uint8_t N_AXES = Dimensions;
  value_type values[N_AXES];
  
  analog() {
    STATIC_CHECK(N_AXES>=1);
  }
  
  value_type get_x() const {
    return values[0];
  }
  void set_x(value_type x) {
    return values[0]=x;
  }
  
  value_type get_y() const {
    STATIC_CHECK(N_AXES>=2);
    return values[1];
  }
  void set_y(value_type y) {
    STATIC_CHECK(N_AXES>=2);
    return values[1]=y;
  }
  
  value_type get_z() const {
    STATIC_CHECK(N_AXES>=3);
    return values[2];
  }
  void set_z(value_type z) {
    STATIC_CHECK(N_AXES>=3);
    return values[2]=z;
  }
  
};

}
#endif // _GAMEPAD_ANALOG_H