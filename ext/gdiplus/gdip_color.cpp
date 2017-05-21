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
    return _Data_Wrap_Struct(klass, &tColor, (void*)0xff000000);
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

/**
 *
 * @overload initialize()
 *   default color is black. +0xff000000+
 * @overload initialize(argb)
 *   @param argb [Integer] 0xAARRGGBB. e.g. +0xffffff00+
 * @overload initialize(a, color)
 *   @param a [Integer] alpha (0-255)
 *   @param color [Color] base color.
 * @overload initialize(r, g, b)
 *   @param r [Integer] red (0-255)
 *   @param g [Integer] green (0-255)
 *   @param b [Integer] blue (0-255)
 * @overload initialize(a, r, g, b)
 *   @param a [Integer] alpha (0-255)
 *   @param r [Integer] red (0-255)
 *   @param g [Integer] green (0-255)
 *   @param b [Integer] blue (0-255)
 * 
 */
static VALUE
gdip_color_init(int argc, VALUE *argv, VALUE self)
{
    if (argc == 0) {}
    else if (argc == 1) {
        if (Integer_p(argv[0])) {
            Data_Ptr_Set_As<ARGB>(self, RB_NUM2UINT(argv[0]));
        }
        else {
            rb_raise(rb_eTypeError, "The argument must be an Integer");
        }
    }
    else if (argc == 2) {
        if (Integer_p(argv[0]) && _KIND_OF(argv[1], &tColor)) {
            ARGB argb = Data_Ptr_As<ARGB>(argv[1]);
            DWORD a = NUM2UINT(argv[0]) & 0xff;
            argb = argb & 0xffffff | a << 24;
            gdip_color_set_color(self, argb);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
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
            DWORD a = NUM2UINT(argv[0]) & 0xff;
            DWORD r = NUM2UINT(argv[1]) & 0xff;
            DWORD g = NUM2UINT(argv[2]) & 0xff;
            DWORD b = NUM2UINT(argv[3]) & 0xff;
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

/**
 *
 * @overload inspect()
 *   @return [String]
 */
static VALUE
gdip_color_inspect(VALUE self)
{
    VALUE r = util_utf8_sprintf("#<%s 0x%08x>", __class__(self), Data_Ptr_As<ARGB>(self));
    return r;
}

/**
 *
 * @overload to_i()
 *   @return [Integer]
 */
static VALUE
gdip_color_to_i(VALUE self)
{
    ARGB argb = Data_Ptr_As<ARGB>(self);
    return RB_UINT2NUM(argb);
}

static VALUE
gdip_color_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    else if (_KIND_OF(other, &tColor)) {
        return Data_Ptr_As<ARGB>(self) == Data_Ptr_As<ARGB>(other) ? Qtrue : Qfalse;
    }
    else if (_RB_INTEGER_P(other)) {
        return Data_Ptr_As<UINT>(self) == RB_NUM2UINT(other) ? Qtrue : Qfalse;
    }
    else {
        return Qfalse;
    }
}



void
Init_color()
{
    cColor = rb_define_class_under(mGdiplus, "Color", rb_cObject);
    rb_define_alloc_func(cColor, gdip_color_alloc);
    rb_define_method(cColor, "initialize", RUBY_METHOD_FUNC(gdip_color_init), -1);
    rb_define_method(cColor, "inspect", RUBY_METHOD_FUNC(gdip_color_inspect), 0);
    rb_define_method(cColor, "to_i", RUBY_METHOD_FUNC(gdip_color_to_i), 0);
    rb_define_method(cColor, "==", RUBY_METHOD_FUNC(gdip_color_equal), 1);
}