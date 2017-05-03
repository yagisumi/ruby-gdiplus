#include "ruby_gdiplus.h"

VALUE mGdiplus;

void
Init_gdiplus(void)
{
  mGdiplus = rb_define_module("Gdiplus");
}
