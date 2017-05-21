/*
 * gdip_color.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tColor = _MAKE_DATA_TYPE(
    "Color", 0, RUBY_NEVER_FREE, NULL, NULL, &cColor);

static VALUE
gdip_color_alloc(VALUE klass)
{
    return _Data_Wrap_Struct(klass, &tColor, 0xff000000);
}

static inline void
gdip_color_set_color(VALUE v, BYTE r, BYTE g, BYTE b, BYTE a=0xff)
{
    Data_Ptr_Set_As<ARGB>(v, b | g << 8 | r << 16 | a << 24);
}

static inline void
gdip_color_set_color(VALUE v, ARGB argb)
{
    Data_Ptr_Set_As<ARGB>(v, argb);
}

static VALUE
gdip_color_init(int argc, VALUE *argv, VALUE self)
{
    if (argc == 0) {}
    else if (argc == 1) {
        if (Integer_p(argv[0])) {
            Data_Ptr_Set_As<ARGB>(self, RB_NUM2UINT(argv[0]));
        }
        else {
            rb_raise(rb_eTypeError, "The argument is not an Integer");
        }
    }
    else if (argc == 2) {
        rb_raise(rb_eArgError, "The number of arguments is invalid");
    }
    else if (argc == 3) {
        if (Integer_p(argv[0], argv[1], argv[2])) {
            DWORD r = NUM2UINT(argv[0]) & 0xff;
            DWORD g = NUM2UINT(argv[1]) & 0xff;
            DWORD b = NUM2UINT(argv[2]) & 0xff;
            gdip_color_set_color(self, r, g, b);
        }
        else {
            rb_raise(rb_eTypeError, "All arguments must be Integer");
        }
    }
    else if (argc == 4) {
        if (Integer_p(argv[0], argv[1], argv[2], argv[3])) {
            DWORD r = NUM2UINT(argv[0]) & 0xff;
            DWORD g = NUM2UINT(argv[1]) & 0xff;
            DWORD b = NUM2UINT(argv[2]) & 0xff;
            DWORD a = NUM2UINT(argv[3]) & 0xff;
            gdip_color_set_color(self, r, g, b, a);
        }
        else {
            rb_raise(rb_eTypeError, "All arguments must be Integer");
        }
    }
    else {
        rb_raise(rb_eArgError, "too many arguments");
    }

    return self;
}


void
Init_color()
{
    cColor = rb_define_class_under(mGdiplus, "Color", rb_cObject);
    rb_define_alloc_func(cColor, gdip_color_alloc);
    rb_define_method(cColor, "initialize", RUBY_METHOD_FUNC(gdip_color_init), -1);

}