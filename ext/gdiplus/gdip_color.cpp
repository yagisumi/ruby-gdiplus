/*
 * gdip_color.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tColor = _MAKE_DATA_TYPE(
    "Color", 0, RUBY_NEVER_FREE, NULL, NULL, &cColor);


bool
gdip_arg_to_color(VALUE v, Color *color, int option, const char *raise_msg)
{
    if (RB_SYMBOL_P(v)) {
        v = rb_const_get(cColor, RB_SYM2ID(v));
    }

    if (_KIND_OF(v, &tColor)) {
        color->SetValue(Data_Ptr_As<ARGB>(v));
        return true;
    }
    else if ((option & ArgOptionAcceptInt) && _RB_INTEGER_P(v)) {
        color->SetValue(RB_NUM2UINT(v));
        return true;
    }
    else if (option & ArgOptionToInt) {
        VALUE num = rb_to_int(v);
        color->SetValue(RB_NUM2UINT(num));
        return true;
    }
    else if (raise_msg != NULL) {
        rb_raise(rb_eTypeError, raise_msg);
    }
    return false;
}


VALUE
gdip_color_create(ARGB argb)
{
    return _Data_Wrap_Struct(cColor, &tColor, reinterpret_cast<void*>(argb));
}

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
    if (argc == 0) { }
    else if (argc == 1) {
        if (Integer_p(argv[0])) {
            Data_Ptr_Set_As<ARGB>(self, RB_NUM2UINT(argv[0]));
        }
        else {
            Color color;
            gdip_arg_to_color(argv[0], &color);
            Data_Ptr_Set_As<ARGB>(self, color.GetValue());
        }
    }
    else if (argc == 2) {
        if (Integer_p(argv[0])) {
            ARGB argb;
            if (_KIND_OF(argv[1], &tColor)) {
                argb = Data_Ptr_As<ARGB>(argv[1]);
            }
            else if (_RB_SYMBOL_P(argv[1])) {
                Color color;
                gdip_arg_to_color(argv[1], &color);
                argb = color.GetValue();
            }
            else {
                rb_raise(rb_eTypeError, "The second argument should be Color.");
            }
            DWORD a = RB_NUM2UINT(argv[0]) & 0xff;
            argb = argb & 0xffffff | a << 24;
            gdip_color_set_color(self, argb);
        }
        else {
            rb_raise(rb_eTypeError, "The first argument should be Integer.");
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
            rb_raise(rb_eTypeError, "All arguments should be Integer.");
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
            rb_raise(rb_eTypeError, "All arguments should be Integer.");
        }
    }
    else {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..4)", argc);
    }

    return self;
}


/**
 *
 * @overload FromArgb()
 *   default color is black. +0xff000000+
 * @overload FromArgb(argb)
 *   @param argb [Integer] 0xAARRGGBB. e.g. +0xffffff00+
 * @overload FromArgb(a, color)
 *   @param a [Integer] alpha (0-255)
 *   @param color [Color] base color.
 * @overload FromArgb(r, g, b)
 *   @param r [Integer] red (0-255)
 *   @param g [Integer] green (0-255)
 *   @param b [Integer] blue (0-255)
 * @overload FromArgb(a, r, g, b)
 *   @param a [Integer] alpha (0-255)
 *   @param r [Integer] red (0-255)
 *   @param g [Integer] green (0-255)
 *   @param b [Integer] blue (0-255)
 * 
 */
 static VALUE
 gdip_color_s_from_argb(int argc, VALUE *argv, VALUE self)
 {
    VALUE r = gdip_color_alloc(self);
    gdip_color_init(argc, argv, r);
    return r;
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

/**
 *
 *   @return [Integer] alpha
 */
static VALUE
gdip_color_get_a(VALUE self)
{
    ARGB argb = Data_Ptr_As<ARGB>(self);
    return RB_UINT2NUM((argb & 0xff000000) >> 24);
}

/**
 *
 *   @return [Integer] red
 */
static VALUE
gdip_color_get_r(VALUE self)
{
    ARGB argb = Data_Ptr_As<ARGB>(self);
    return RB_UINT2NUM((argb & 0xff0000) >> 16);
}

/**
 *
 *   @return [Integer] green
 */
static VALUE
gdip_color_get_g(VALUE self)
{
    ARGB argb = Data_Ptr_As<ARGB>(self);
    return RB_UINT2NUM((argb & 0xff00) >> 8);
}

/**
 *
 *   @return [Integer] blue
 */
static VALUE
gdip_color_get_b(VALUE self)
{
    ARGB argb = Data_Ptr_As<ARGB>(self);
    return RB_UINT2NUM(argb & 0xff);
}

/**
 *
 * @overload ==(other)
 *   @param other [Object]
 *   @return [Boolean]
 */
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

static VALUE
gdip_color_s_const_get(VALUE self)
{
    return rb_const_get(self, rb_frame_this_func());
}

static inline void
gdip_color_define_const(ARGB argb, const char *name) {
    VALUE color = gdip_color_alloc(cColor);
    Data_Ptr_Set_As<ARGB>(color, argb);
    rb_define_const(cColor, name, color);
    rb_define_singleton_method(cColor, name, RUBY_METHOD_FUNC(gdip_color_s_const_get), 0);
}

static void
Init_color_constants()
{
    gdip_color_define_const(0xFFF0F8FF, "AliceBlue");
    gdip_color_define_const(0xFFFAEBD7, "AntiqueWhite");
    gdip_color_define_const(0xFF00FFFF, "Aqua");
    gdip_color_define_const(0xFF7FFFD4, "Aquamarine");
    gdip_color_define_const(0xFFF0FFFF, "Azure");
    gdip_color_define_const(0xFFF5F5DC, "Beige");
    gdip_color_define_const(0xFFFFE4C4, "Bisque");
    gdip_color_define_const(0xFF000000, "Black");
    gdip_color_define_const(0xFFFFEBCD, "BlanchedAlmond");
    gdip_color_define_const(0xFF0000FF, "Blue");
    gdip_color_define_const(0xFF8A2BE2, "BlueViolet");
    gdip_color_define_const(0xFFA52A2A, "Brown");
    gdip_color_define_const(0xFFDEB887, "BurlyWood");
    gdip_color_define_const(0xFF5F9EA0, "CadetBlue");
    gdip_color_define_const(0xFF7FFF00, "Chartreuse");
    gdip_color_define_const(0xFFD2691E, "Chocolate");
    gdip_color_define_const(0xFFFF7F50, "Coral");
    gdip_color_define_const(0xFF6495ED, "CornflowerBlue");
    gdip_color_define_const(0xFFFFF8DC, "Cornsilk");
    gdip_color_define_const(0xFFDC143C, "Crimson");
    gdip_color_define_const(0xFF00FFFF, "Cyan");
    gdip_color_define_const(0xFF00008B, "DarkBlue");
    gdip_color_define_const(0xFF008B8B, "DarkCyan");
    gdip_color_define_const(0xFFB8860B, "DarkGoldenrod");
    gdip_color_define_const(0xFFA9A9A9, "DarkGray");
    gdip_color_define_const(0xFF006400, "DarkGreen");
    gdip_color_define_const(0xFFBDB76B, "DarkKhaki");
    gdip_color_define_const(0xFF8B008B, "DarkMagenta");
    gdip_color_define_const(0xFF556B2F, "DarkOliveGreen");
    gdip_color_define_const(0xFFFF8C00, "DarkOrange");
    gdip_color_define_const(0xFF9932CC, "DarkOrchid");
    gdip_color_define_const(0xFF8B0000, "DarkRed");
    gdip_color_define_const(0xFFE9967A, "DarkSalmon");
    gdip_color_define_const(0xFF8FBC8B, "DarkSeaGreen");
    gdip_color_define_const(0xFF483D8B, "DarkSlateBlue");
    gdip_color_define_const(0xFF2F4F4F, "DarkSlateGray");
    gdip_color_define_const(0xFF00CED1, "DarkTurquoise");
    gdip_color_define_const(0xFF9400D3, "DarkViolet");
    gdip_color_define_const(0xFFFF1493, "DeepPink");
    gdip_color_define_const(0xFF00BFFF, "DeepSkyBlue");
    gdip_color_define_const(0xFF696969, "DimGray");
    gdip_color_define_const(0xFF1E90FF, "DodgerBlue");
    gdip_color_define_const(0xFFB22222, "Firebrick");
    gdip_color_define_const(0xFFFFFAF0, "FloralWhite");
    gdip_color_define_const(0xFF228B22, "ForestGreen");
    gdip_color_define_const(0xFFFF00FF, "Fuchsia");
    gdip_color_define_const(0xFFDCDCDC, "Gainsboro");
    gdip_color_define_const(0xFFF8F8FF, "GhostWhite");
    gdip_color_define_const(0xFFFFD700, "Gold");
    gdip_color_define_const(0xFFDAA520, "Goldenrod");
    gdip_color_define_const(0xFF808080, "Gray");
    gdip_color_define_const(0xFF008000, "Green");
    gdip_color_define_const(0xFFADFF2F, "GreenYellow");
    gdip_color_define_const(0xFFF0FFF0, "Honeydew");
    gdip_color_define_const(0xFFFF69B4, "HotPink");
    gdip_color_define_const(0xFFCD5C5C, "IndianRed");
    gdip_color_define_const(0xFF4B0082, "Indigo");
    gdip_color_define_const(0xFFFFFFF0, "Ivory");
    gdip_color_define_const(0xFFF0E68C, "Khaki");
    gdip_color_define_const(0xFFE6E6FA, "Lavender");
    gdip_color_define_const(0xFFFFF0F5, "LavenderBlush");
    gdip_color_define_const(0xFF7CFC00, "LawnGreen");
    gdip_color_define_const(0xFFFFFACD, "LemonChiffon");
    gdip_color_define_const(0xFFADD8E6, "LightBlue");
    gdip_color_define_const(0xFFF08080, "LightCoral");
    gdip_color_define_const(0xFFE0FFFF, "LightCyan");
    gdip_color_define_const(0xFFFAFAD2, "LightGoldenrodYellow");
    gdip_color_define_const(0xFFD3D3D3, "LightGray");
    gdip_color_define_const(0xFF90EE90, "LightGreen");
    gdip_color_define_const(0xFFFFB6C1, "LightPink");
    gdip_color_define_const(0xFFFFA07A, "LightSalmon");
    gdip_color_define_const(0xFF20B2AA, "LightSeaGreen");
    gdip_color_define_const(0xFF87CEFA, "LightSkyBlue");
    gdip_color_define_const(0xFF778899, "LightSlateGray");
    gdip_color_define_const(0xFFB0C4DE, "LightSteelBlue");
    gdip_color_define_const(0xFFFFFFE0, "LightYellow");
    gdip_color_define_const(0xFF00FF00, "Lime");
    gdip_color_define_const(0xFF32CD32, "LimeGreen");
    gdip_color_define_const(0xFFFAF0E6, "Linen");
    gdip_color_define_const(0xFFFF00FF, "Magenta");
    gdip_color_define_const(0xFF800000, "Maroon");
    gdip_color_define_const(0xFF66CDAA, "MediumAquamarine");
    gdip_color_define_const(0xFF0000CD, "MediumBlue");
    gdip_color_define_const(0xFFBA55D3, "MediumOrchid");
    gdip_color_define_const(0xFF9370DB, "MediumPurple");
    gdip_color_define_const(0xFF3CB371, "MediumSeaGreen");
    gdip_color_define_const(0xFF7B68EE, "MediumSlateBlue");
    gdip_color_define_const(0xFF00FA9A, "MediumSpringGreen");
    gdip_color_define_const(0xFF48D1CC, "MediumTurquoise");
    gdip_color_define_const(0xFFC71585, "MediumVioletRed");
    gdip_color_define_const(0xFF191970, "MidnightBlue");
    gdip_color_define_const(0xFFF5FFFA, "MintCream");
    gdip_color_define_const(0xFFFFE4E1, "MistyRose");
    gdip_color_define_const(0xFFFFE4B5, "Moccasin");
    gdip_color_define_const(0xFFFFDEAD, "NavajoWhite");
    gdip_color_define_const(0xFF000080, "Navy");
    gdip_color_define_const(0xFFFDF5E6, "OldLace");
    gdip_color_define_const(0xFF808000, "Olive");
    gdip_color_define_const(0xFF6B8E23, "OliveDrab");
    gdip_color_define_const(0xFFFFA500, "Orange");
    gdip_color_define_const(0xFFFF4500, "OrangeRed");
    gdip_color_define_const(0xFFDA70D6, "Orchid");
    gdip_color_define_const(0xFFEEE8AA, "PaleGoldenrod");
    gdip_color_define_const(0xFF98FB98, "PaleGreen");
    gdip_color_define_const(0xFFAFEEEE, "PaleTurquoise");
    gdip_color_define_const(0xFFDB7093, "PaleVioletRed");
    gdip_color_define_const(0xFFFFEFD5, "PapayaWhip");
    gdip_color_define_const(0xFFFFDAB9, "PeachPuff");
    gdip_color_define_const(0xFFCD853F, "Peru");
    gdip_color_define_const(0xFFFFC0CB, "Pink");
    gdip_color_define_const(0xFFDDA0DD, "Plum");
    gdip_color_define_const(0xFFB0E0E6, "PowderBlue");
    gdip_color_define_const(0xFF800080, "Purple");
    gdip_color_define_const(0xFFFF0000, "Red");
    gdip_color_define_const(0xFFBC8F8F, "RosyBrown");
    gdip_color_define_const(0xFF4169E1, "RoyalBlue");
    gdip_color_define_const(0xFF8B4513, "SaddleBrown");
    gdip_color_define_const(0xFFFA8072, "Salmon");
    gdip_color_define_const(0xFFF4A460, "SandyBrown");
    gdip_color_define_const(0xFF2E8B57, "SeaGreen");
    gdip_color_define_const(0xFFFFF5EE, "SeaShell");
    gdip_color_define_const(0xFFA0522D, "Sienna");
    gdip_color_define_const(0xFFC0C0C0, "Silver");
    gdip_color_define_const(0xFF87CEEB, "SkyBlue");
    gdip_color_define_const(0xFF6A5ACD, "SlateBlue");
    gdip_color_define_const(0xFF708090, "SlateGray");
    gdip_color_define_const(0xFFFFFAFA, "Snow");
    gdip_color_define_const(0xFF00FF7F, "SpringGreen");
    gdip_color_define_const(0xFF4682B4, "SteelBlue");
    gdip_color_define_const(0xFFD2B48C, "Tan");
    gdip_color_define_const(0xFF008080, "Teal");
    gdip_color_define_const(0xFFD8BFD8, "Thistle");
    gdip_color_define_const(0xFFFF6347, "Tomato");
    gdip_color_define_const(0x00FFFFFF, "Transparent");
    gdip_color_define_const(0xFF40E0D0, "Turquoise");
    gdip_color_define_const(0xFFEE82EE, "Violet");
    gdip_color_define_const(0xFFF5DEB3, "Wheat");
    gdip_color_define_const(0xFFFFFFFF, "White");
    gdip_color_define_const(0xFFF5F5F5, "WhiteSmoke");
    gdip_color_define_const(0xFFFFFF00, "Yellow");
    gdip_color_define_const(0xFF9ACD32, "YellowGreen");
}

/**
 * Document-class: Gdiplus::Color
 * This class represents 32-bit ARGB.
 * @example
 *   color1 = Color.new(0xffbeedfd)
 *   color2 = Color.new(190, 237, 253)
 *   Color.Red # => 0xFFFF0000
 */
void
Init_color()
{
    cColor = rb_define_class_under(mGdiplus, "Color", rb_cObject);
    rb_define_alloc_func(cColor, gdip_color_alloc);
    rb_define_method(cColor, "initialize", RUBY_METHOD_FUNC(gdip_color_init), -1);
    rb_define_method(cColor, "inspect", RUBY_METHOD_FUNC(gdip_color_inspect), 0);
    rb_define_method(cColor, "to_i", RUBY_METHOD_FUNC(gdip_color_to_i), 0);
    rb_define_method(cColor, "==", RUBY_METHOD_FUNC(gdip_color_equal), 1);
    ATTR_R(cColor, A, a, color);
    ATTR_R(cColor, R, r, color);
    ATTR_R(cColor, G, g, color);
    ATTR_R(cColor, B, b, color);
    rb_define_singleton_method(cColor, "FromArgb", RUBY_METHOD_FUNC(gdip_color_s_from_argb), -1);
    rb_define_alias(rb_singleton_class(cColor), "from_argb", "FromArgb");

    Init_color_constants();
}
