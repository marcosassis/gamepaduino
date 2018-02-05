#include "gamepad.h"


int get_id_by_name(String aname, String* names, unsigned max_names) {
  int ret = -1;
  for (unsigned i = 0; i < max_names; ++i) {
    if (names[i] == aname) {
      ret = i;
      break;
    }
  }
  return ret;
}



