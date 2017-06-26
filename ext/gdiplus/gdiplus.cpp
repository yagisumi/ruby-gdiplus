/*
 * gdiplus.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

VALUE mGdiplus;
VALUE mInternals;
VALUE cGpObject;
VALUE eGdiplus;

VALUE cGuid;
VALUE cImageCodecInfo;
VALUE cImage;
VALUE cBitmap;
VALUE cPixelFormat;
VALUE cEncoderParameterValueType;
VALUE cBrushType;
VALUE cCustomLineCapType;
VALUE cDashCap;
VALUE cDashStyle;
VALUE cLineCap;
VALUE cLineJoin;
VALUE cMatrixOrder;
VALUE cPenAlignment;
VALUE cPenType;
VALUE cFontStyle;
VALUE cGenericFontFamilies;
VALUE cPixelOffsetMode;
VALUE cSmoothingMode;
VALUE cTextRenderingHint;
VALUE cCompositingMode;
VALUE cCompositingQuality;
VALUE cFillMode;
VALUE cGraphicsUnit;
VALUE cInterpolationMode;
VALUE cDriverStringOptions;
VALUE cFlushIntention;
VALUE cHotkeyPrefix;
VALUE cStringAlignment;
VALUE cStringDigitSubstitute;
VALUE cStringFormatFlags;
VALUE cStringTrimming;
VALUE cPathPointType;

VALUE cEncoder;
VALUE cEncoderValue;
VALUE cEnumInt;
VALUE cEnumFlags;
VALUE cImageFormat;
VALUE cEncoderParameter;
VALUE cEncoderParameters;
VALUE cColor;
VALUE cPen;
VALUE cBrush;
VALUE cSolidBrush;
VALUE cGraphics;
VALUE cPoint;
VALUE cPointF;
VALUE cSize;
VALUE cSizeF;
VALUE cRectangle;
VALUE cRectangleF;
VALUE cFontFamily;
VALUE cFontCollection;
VALUE cFont;
VALUE cInstalledFontCollection;
VALUE cPrivateFontCollection;
VALUE cLangId;
VALUE cStringFormat;
VALUE cGraphicsPath;
VALUE cMatrix;

int gdip_refcount = 0;
bool gdip_end_flag = false;
static ULONG_PTR gdiplus_token = 0;

const char *GpStatusStrs[22] = {
    "Ok",
    "GenericError",
    "InvalidParameter",
    "OutOfMemory",
    "ObjectBusy",
    "InsufficientBuffer",
    "NotImplemented",
    "Win32Error",
    "WrongState",
    "Aborted",
    "FileNotFound",
    "ValueOverflow",
    "AccessDenied",
    "UnknownImageFormat",
    "FontFamilyNotFound",
    "FontStyleNotFound",
    "NotTrueTypeFont",
    "UnsupportedGdiplusVersion",
    "GdiplusNotInitialized",
    "PropertyNotFound",
    "PropertyNotSupported",
    "ProfileNotFound"
};

#if GDIPLUS_DEBUG
void
dp(const char *fmt, ...)
{
    char buf[128] = {0};
    va_list list;
    va_start(list, fmt);
    _vsnprintf(buf, 127, fmt, list);
    OutputDebugString(buf);
    va_end(list);
}

class LastCheck {
    public:
    LastCheck() {
        dp("===(%d)==================================================", RUBY_API_VERSION_CODE);
    }
    ~LastCheck() {
        if (gdip_refcount != 0) {
            dp("LastCheck Error: gdip_refcount = %d", gdip_refcount);
        }
        else {
            dp("LastCheck: OK");
        }
        dp("--------------------------------------------------(%d)---", RUBY_API_VERSION_CODE);
    }
};

static LastCheck lastcheck;
#endif

static void
gdiplus_init()
{
    GdiplusStartupInput gsi;
    Status status = GdiplusStartup(&gdiplus_token, &gsi, NULL);
    dp("GdiplusStartup status: %d, gdiplus_token: %p", status, gdiplus_token);
    if (gdiplus_token == 0 || status != Ok) {
        rb_raise(eGdiplus, "GdiplusStartup error");
    }
}

void
gdiplus_shutdown(){
    dp("%s (gdip_refcount: %d)", __func__, gdip_refcount);
    if (gdip_end_flag && gdip_refcount == 0) {
        dp("GdiplusShutdown");
        GdiplusShutdown(gdiplus_token);
    }
}

static void
gdiplus_end(VALUE self)
{
    gdip_end_flag = true;
    gdiplus_shutdown();
}

bool
gdip_arg_to_double(VALUE v, double *dbl, const char *raise_msg)
{
    if (_RB_FLOAT_P(v)) {
        *dbl = NUM2DBL(v);
        return true;
    }
    else if (_RB_INTEGER_P(v)) {
        *dbl = 1.0 * RB_NUM2INT(v);
        return true;
    }
    else if (raise_msg != NULL) {
        rb_raise(rb_eTypeError, raise_msg);
    }
    return false;
}

bool
gdip_arg_to_single(VALUE v, float *flt, const char *raise_msg)
{
    if (_RB_FLOAT_P(v)) {
        *flt = NUM2SINGLE(v);
        return true;
    }
    else if (_RB_INTEGER_P(v)) {
        *flt = 1.0f * RB_NUM2INT(v);
        return true;
    }
    else if (raise_msg != NULL) {
        rb_raise(rb_eTypeError, raise_msg);
    }
    return false;
}

float *
alloc_array_of_single(VALUE ary, int& count)
{
    count = 0;
    for (long i = 0; i < RARRAY_LEN(ary); ++i) {
        VALUE elem = rb_ary_entry(ary, i);
        if (Float_p(elem)) {
            count += 1;
        }
    }
    if (count == 0) return NULL;

    int idx = 0;
    float *tary = static_cast<float *>(ruby_xcalloc(count, sizeof(float)));
    for (long i = 0; i < RARRAY_LEN(ary); ++i) {
        VALUE elem = rb_ary_entry(ary, i);
        if (Float_p(elem)) {
            tary[idx] = NUM2SINGLE(elem);
            idx += 1;
        }
    }
    return tary;
}

/**
 * @return [Integer]
 */
static VALUE
gdip_gpobject_object_id(VALUE self)
{
    if (RB_TYPE_P(self, RUBY_T_DATA)) {
        return _RB_ID2NUM(Data_Ptr_As<ID>(self) >> 1);
    }
    else {
        return _RB_ID2NUM(-1);
    }
}

VALUE
gdip_class_const_get(VALUE klass)
{
    return rb_const_get(klass, rb_frame_this_func());
}

extern "C" void
Init_gdiplus(void)
{
    mGdiplus = rb_define_module("Gdiplus");
    eGdiplus = rb_define_class_under(mGdiplus, "GdiplusError", rb_eException);
    mInternals = rb_define_module_under(mGdiplus, "Internals");
    cGpObject = rb_define_class_under(mInternals, "GpObject", rb_cObject);
    rb_define_method(cGpObject, "gdiplus_id", RUBY_METHOD_FUNC(gdip_gpobject_object_id), 0);

    rb_set_end_proc(gdiplus_end, Qnil);
    gdiplus_init();

    Init_codec();
    Init_image();
    Init_bitmap();
    Init_enum();
    Init_color();
    Init_pen_brush();
    Init_graphics();
    Init_rectangle();
    Init_font();
    Init_langid();
    Init_stringformat();
    Init_graphicspath();
    Init_matrix();
}
