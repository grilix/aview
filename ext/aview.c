#include <ruby.h>

#include "extconf.h"
#include "buffer.h"

void
Init_aview_ext(void) {
  VALUE mod = rb_define_module("AView");
  Init_aview_buffer(mod);
}

