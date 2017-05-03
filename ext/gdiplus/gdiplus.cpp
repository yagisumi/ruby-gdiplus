#include "ruby_gdiplus.h"

VALUE mGdiplus;

extern "C"
void
Init_gdiplus(void)
{
  mGdiplus = rb_define_module("Gdiplus");
}
