/*
 * ruby_gdiplus.h
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#ifndef RUBY_GDIPLUS_H
#define RUBY_GDIPLUS_H

#include <ruby.h>
#include "ruby_compatible.h"
#include "gdip_utils.h"
#include <windows.h>
#include <rpc.h>
#include <gdiplus.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <float.h>

using namespace Gdiplus;

extern VALUE mGdiplus;
extern VALUE mInternals;
extern VALUE cGpObject;
extern VALUE eGdiplus;

extern VALUE cGuid;
extern VALUE cImageCodecInfo;
extern VALUE cImage;
extern VALUE cBitmap;
extern VALUE cPixelFormat;
extern VALUE cEncoderParameterValueType;
extern VALUE cEncoder;
extern VALUE cEncoderValue;
extern VALUE cEnumInt;
extern VALUE cEnumFlags;
extern VALUE cImageFormat;
extern VALUE cEncoderParameter;
extern VALUE cEncoderParameters;
extern VALUE cColor;
extern VALUE cPen;
extern VALUE cBrush;
extern VALUE cSolidBrush;
extern VALUE cBrushType;
extern VALUE cCustomLineCapType;
extern VALUE cDashCap;
extern VALUE cDashStyle;
extern VALUE cLineCap;
extern VALUE cLineJoin;
extern VALUE cMatrixOrder;
extern VALUE cPenAlignment;
extern VALUE cPenType;
extern VALUE cGraphics;
extern VALUE cPoint;
extern VALUE cPointF;
extern VALUE cSize;
extern VALUE cSizeF;
extern VALUE cRectangle;
extern VALUE cRectangleF;
extern VALUE cFontStyle;
extern VALUE cGenericFontFamilies;
extern VALUE cPixelOffsetMode;
extern VALUE cSmoothingMode;
extern VALUE cTextRenderingHint;
extern VALUE cCompositingMode;
extern VALUE cCompositingQuality;
extern VALUE cFillMode;
extern VALUE cGraphicsUnit;
extern VALUE cInterpolationMode;
extern VALUE cDriverStringOptions;
extern VALUE cFlushIntention;
extern VALUE cHotkeyPrefix;
extern VALUE cStringAlignment;
extern VALUE cStringDigitSubstitute;
extern VALUE cStringFormatFlags;
extern VALUE cStringTrimming;
extern VALUE cPathPointType;

extern VALUE cFontFamily;
extern VALUE cFontCollection;
extern VALUE cFont;
extern VALUE cInstalledFontCollection;
extern VALUE cPrivateFontCollection;
extern VALUE cLangId;
extern VALUE cStringFormat;
extern VALUE cGraphicsPath;

extern const rb_data_type_t tGuid;
extern const rb_data_type_t tImageCodecInfo;
extern const rb_data_type_t tImage;
extern const rb_data_type_t tBitmap;
extern const rb_data_type_t tEnumInt;
extern const rb_data_type_t tEncoderParameter;
extern const rb_data_type_t tEncoderParameters;
extern const rb_data_type_t tColor;
extern const rb_data_type_t tPen;
extern const rb_data_type_t tBrush;
extern const rb_data_type_t tGraphics;
extern const rb_data_type_t tPoint;
extern const rb_data_type_t tPointF;
extern const rb_data_type_t tSize;
extern const rb_data_type_t tSizeF;
extern const rb_data_type_t tRectangle;
extern const rb_data_type_t tRectangleF;
extern const rb_data_type_t tFontFamily;
extern const rb_data_type_t tFontCollection;
extern const rb_data_type_t tFont;
extern const rb_data_type_t tInstalledFontCollection;
extern const rb_data_type_t tPrivateFontCollection;
extern const rb_data_type_t tLangId;
extern const rb_data_type_t tStringFormat;
extern const rb_data_type_t tGraphicsPath;
extern const rb_data_type_t tPathData;

void Init_codec();
void Init_image();
void Init_bitmap();
void Init_enum();
void Init_color();
void Init_pen_brush();
void Init_graphics();
void Init_rectangle();
void Init_font();
void Init_langid();
void Init_stringformat();
void Init_graphicspath();

/* gdip_enum.cpp */
extern ID ID_UNKNOWN;
extern ID ID_Compression;
extern ID ID_ColorDepth;
extern ID ID_ScanMethod;
extern ID ID_Version;
extern ID ID_RenderMethod;
extern ID ID_Quality;
extern ID ID_Transformation;
extern ID ID_LuminanceTable;
extern ID ID_ChrominanceTable;
extern ID ID_SaveFlag;
extern ID ID_ColorSpace;
extern ID ID_ImageItems;
extern ID ID_SaveAsCMYK;

extern GUID _EncoderColorSpace;
extern GUID _EncoderImageItems;
extern GUID _EncoderSaveAsCMYK;

extern GUID _ImageFormatEXIF;
extern GUID _ImageFormatUndefined;


/* gdiplus.cpp */
enum ArgOption {
    ArgOptionNone = 0,
    ArgOptionAcceptInt = 1,
    ArgOptionToInt = 2,
    ArgOptionColorDefault = ArgOptionAcceptInt | ArgOptionToInt
};
extern const char *GpStatusStrs[22];
extern int gdip_refcount;
extern bool gdip_end_flag;
void gdiplus_shutdown();

bool gdip_arg_to_double(VALUE v, double *dbl, const char *raise_msg=NULL);
bool gdip_arg_to_single(VALUE v, float *flt, const char *raise_msg=NULL);
float *alloc_array_of_single(VALUE ary, int& count);
VALUE gdip_class_const_get(VALUE klass);

static inline void GdiplusAddRef() { ++gdip_refcount; }
static inline void GdiplusRelease() {
    --gdip_refcount;
    if (gdip_end_flag && gdip_refcount == 0) {
        gdiplus_shutdown();
    }
}

/* gdip_codec.cpp */
EncoderParameters *gdip_encprms_build_struct(VALUE v);

/* gdip_enum.cpp */
template <typename T> ID gdip_enum_get_id(VALUE klass, T data);
template <typename T> VALUE gdip_enum_get(VALUE klass, T data);
VALUE gdip_enumint_create(VALUE klass, int num);
int gdip_arg_to_enumint(VALUE klass, VALUE arg, int *num, const char *raise_msg=NULL, int option=ArgOptionAcceptInt);
VALUE gdip_enum_guid_create(VALUE klass, GUID *guid);

/* gdip_graphics.cpp */
VALUE gdip_graphics_create(Graphics *g);

/* gdip_color.cpp */
VALUE gdip_color_create(ARGB argb);
static inline VALUE
gdip_color_create(Color& color)
{
    return gdip_color_create(color.GetValue());
}
bool gdip_arg_to_color(VALUE v, Color *color, const char *raise_msg=NULL, int option=ArgOptionColorDefault);

/* gdip_rectangle.cpp */
VALUE gdip_point_create(int x, int y);
VALUE gdip_pointf_create(float x, float y);
VALUE gdip_size_create(int width, int height);
VALUE gdip_sizef_create(float width, float height);
VALUE gdip_rect_create(int x, int y, int width, int height);
VALUE gdip_rect_create(Rect *rect=NULL);
VALUE gdip_rectf_create(float x, float y, float width, float height);
VALUE gdip_rectf_create(RectF *rect=NULL);

/* gdip_langid.cpp */
VALUE gdip_langid_create(LANGID langage);
bool gdip_arg_to_langid(VALUE arg, LANGID *langid, const char *raise_msg=NULL, int option=ArgOptionAcceptInt);

/* gdip_utils.cpp */
VALUE util_encode_to_utf8(VALUE str);
VALUE util_associate_utf8(VALUE str);
VALUE util_utf8_sprintf(const char* format, ...);
VALUE util_utf16_str_new(VALUE v);
VALUE util_utf8_str_new_from_wstr(const wchar_t * wstr);
bool util_extname(VALUE str, char (&ext)[6]);


#define ATTR_R4(klass, Name, name_, prefix) \
    do { \
        rb_define_method(klass, #Name, RUBY_METHOD_FUNC(gdip_ ## prefix ## _get_ ## name_), 0); \
        rb_define_method(klass, #name_, RUBY_METHOD_FUNC(gdip_ ## prefix ## _get_ ## name_), 0); \
    } \
    while (0)
#define ATTR_R5(klass, Name, name_, prefix, fname) \
    do { \
        rb_define_method(klass, #Name, RUBY_METHOD_FUNC(gdip_ ## prefix ## _get_ ## fname), 0); \
        rb_define_method(klass, #name_, RUBY_METHOD_FUNC(gdip_ ## prefix ## _get_ ## fname), 0); \
    } \
    while (0)
#define ATTR_R_Q(klass, Name, name_, prefix) \
    do { \
        rb_define_method(klass, #Name, RUBY_METHOD_FUNC(gdip_ ## prefix ## _get_ ## name_), 0); \
        rb_define_method(klass, #name_"?", RUBY_METHOD_FUNC(gdip_ ## prefix ## _get_ ## name_), 0); \
    } \
    while (0)
#define ATTR_RW(klass, Name, name_, prefix) \
    do { \
        rb_define_method(klass, #Name, RUBY_METHOD_FUNC(gdip_ ## prefix ## _get_ ## name_), 0); \
        rb_define_method(klass, #name_, RUBY_METHOD_FUNC(gdip_ ## prefix ## _get_ ## name_), 0); \
        rb_define_method(klass, #Name"=", RUBY_METHOD_FUNC(gdip_ ## prefix ## _set_ ## name_), 1); \
        rb_define_method(klass, #name_"=", RUBY_METHOD_FUNC(gdip_ ## prefix ## _set_ ## name_), 1); \
    } \
    while (0)

#define GET_MACRO(a) GET_MACRO2 a
#define GET_MACRO2(_1,_2,_3,_4,_5,NAME,...) NAME
#define ATTR_R(...) GET_MACRO((__VA_ARGS__, ATTR_R5, ATTR_R4))(__VA_ARGS__)

#define CLASS_ATTR_R4(klass, Name, name_, prefix) \
    do { \
        rb_define_singleton_method(klass, #Name, RUBY_METHOD_FUNC(gdip_ ## prefix ## _s_get_ ## name_), 0); \
        rb_define_singleton_method(klass, #name_, RUBY_METHOD_FUNC(gdip_ ## prefix ## _s_get_ ## name_), 0); \
    } \
    while (0)
#define CLASS_ATTR_R5(klass, Name, name_, prefix, fname) \
    do { \
        rb_define_singleton_method(klass, #Name, RUBY_METHOD_FUNC(gdip_ ## prefix ## _s_get_ ## fname), 0); \
        rb_define_singleton_method(klass, #name_, RUBY_METHOD_FUNC(gdip_ ## prefix ## _s_get_ ## fname), 0); \
    } \
    while (0)
#define CLASS_ATTR_R_Q(klass, Name, name_, prefix) \
    do { \
        rb_define_singleton_method(klass, #Name, RUBY_METHOD_FUNC(gdip_ ## prefix ## _s_get_ ## name_), 0); \
        rb_define_singleton_method(klass, #name_"?", RUBY_METHOD_FUNC(gdip_ ## prefix ## _s_get_ ## name_), 0); \
    } \
    while (0)
#define CLASS_ATTR_RW(klass, Name, name_, prefix) \
    do { \
        rb_define_singleton_method(klass, #Name, RUBY_METHOD_FUNC(gdip_ ## prefix ## _s_get_ ## name_), 0); \
        rb_define_singleton_method(klass, #name_, RUBY_METHOD_FUNC(gdip_ ## prefix ## _s_get_ ## name_), 0); \
        rb_define_singleton_method(klass, #Name"=", RUBY_METHOD_FUNC(gdip_ ## prefix ## _s_set_ ## name_), 1); \
        rb_define_singleton_method(klass, #name_"=", RUBY_METHOD_FUNC(gdip_ ## prefix ## _s_set_ ## name_), 1); \
    } \
    while (0)

#define CLASS_ATTR_R(...) GET_MACRO((__VA_ARGS__, CLASS_ATTR_R5, CLASS_ATTR_R4))(__VA_ARGS__)

/* Debug */
// #define GDIPLUS_DEBUG 1 // moved in extconf.rb

#ifndef GDIPLUS_DEBUG
#define GDIPLUS_DEBUG 0
#endif
#if GDIPLUS_DEBUG
    #include <typeinfo>
    #if defined(__MINGW32__) || defined(__MINGW64__)
        #define GDIPLUS_MINGW 1
        #include <cxxabi.h>
    #else
        #define GDIPLUS_MINGW 0
    #endif
    void dp(const char *fmt, ...);
#else
    #define dp(...)
#endif

#if GDIPLUS_DEBUG
template <typename T>
static inline void
dp_type(const char *msg)
{
    #if GDIPLUS_MINGW
        const std::type_info& id = typeid(T);
        int status;
        char *name = abi::__cxa_demangle(id.name(), NULL, 0, &status);
        if (name != NULL) {
            if (status == 0) {
                dp("%s: %s", name, msg);
            }
            else {
                dp("%s: %s", id.name(), msg);
            }
            free(name);
        }
    #else
        dp("%s: %s", typeid(T).name(), msg);
    #endif /* GDIPLUS_MINGW */
}
#define DPT(msg) dp_type<T>(msg);
#else
#define DPT(msg)
#endif

template<typename T>
static inline T 
Data_Ptr(VALUE obj)
{
    if (!RB_TYPE_P(obj, RUBY_T_DATA)) {
        rb_raise(rb_eTypeError, "wrong argument type");
    }
    return static_cast<T>(_DATA_PTR(obj));
}

template<typename T>
union castunion {
    T t;
    void *ptr;
};

template<typename T>
static inline T 
Data_Ptr_As(VALUE obj)
{
    if (!RB_TYPE_P(obj, RUBY_T_DATA)) {
        rb_raise(rb_eTypeError, "wrong argument type");
    }
    //return *reinterpret_cast<T *>(&_DATA_PTR(obj)); // warning: dereferencing type-punned pointer will break strict-aliasing rules
    castunion<T> uni;
    uni.ptr = _DATA_PTR(obj);
    return uni.t;
}

template<typename T>
static inline void
Data_Ptr_Set_As(VALUE v, T arg)
{
    _DATA_PTR(v) = reinterpret_cast<void *>(arg);
}

template<typename T>
static inline T
RString_Ptr(VALUE str)
{
    return reinterpret_cast<T>(RSTRING_PTR(str));
}

template<typename T>
static inline void
Data_Ptr_Set_Data(VALUE v, T ptr)
{
    *static_cast<T>(_DATA_PTR(v)) = *ptr;
}

template<typename T>
static size_t
typeddata_size(const void *p)
{
    return sizeof(T);
}

template<typename T, const rb_data_type_t *type>
static VALUE
typeddata_alloc(VALUE klass=Qnil)
{
    if (RB_NIL_P(klass)) klass = *static_cast<VALUE *>(type->data);
    void *ptr = RB_ZALLOC(T);
    DPT("RData alloc");
    VALUE r = _Data_Wrap_Struct(klass, type, ptr);
    return r;
}

template<const rb_data_type_t *type>
static VALUE
typeddata_alloc_null(VALUE klass=Qnil)
{
    if (RB_NIL_P(klass)) klass = *static_cast<VALUE *>(type->data);
    dp("%s: RData alloc", type->wrap_struct_name);
    VALUE r = _Data_Wrap_Struct(klass, type, NULL);
    return r;
}


template<typename T, const rb_data_type_t *type>
static T *
alloc_array_of(VALUE ary, int& count)
{
    count = 0;
    for (long i = 0; i < RARRAY_LEN(ary); ++i) {
        VALUE elem = rb_ary_entry(ary, i);
        if (_KIND_OF(elem, type)) {
            count += 1;
        }
    }
    if (count == 0) return NULL;

    int idx = 0;
    T *tary = static_cast<T *>(ruby_xcalloc(count, sizeof(T)));
    for (long i = 0; i < RARRAY_LEN(ary); ++i) {
        VALUE elem = rb_ary_entry(ary, i);
        if (_KIND_OF(elem, type)) {
            T *t = Data_Ptr<T *>(elem);
            tary[idx] = *t;
            idx += 1;
        }
    }
    return tary;
}



template<typename T>
static void
gdip_default_free(void *ptr)
{
    DPT("RData free");
    ruby_xfree(ptr);
}
#if GDIPLUS_DEBUG
#define GDIP_DEFAULT_FREE(T) (&gdip_default_free<T>)
#else
#define GDIP_DEFAULT_FREE(T) RUBY_DEFAULT_FREE
#endif

template<typename T>
static inline T
gdip_obj_create(T obj)
{
    if (obj == NULL) {
        DPT("error obj == NULL");
        rb_raise(eGdiplus, "Object creation error");
        return NULL;
    }
    Status status = obj->GetLastStatus();
    if (status == Ok) {
        DPT("new");
        GdiplusAddRef();
        return obj;
    }
    DPT("error and delete");
    delete obj;
    if (status < 22) {
        rb_raise(eGdiplus, "Status.%s, Some error occurred.", GpStatusStrs[status]);
    }
    else {
        rb_raise(eGdiplus, "Unknown error occurred. (Status: %d)", status);
    }
    return NULL;
}

template<typename T>
static inline void
gdip_obj_free(void *ptr)
{
    T obj = static_cast<T>(ptr);
    if (obj != NULL) {
        delete obj;
        DPT("delete");
        GdiplusRelease();
    }
}
#define GDIP_OBJ_FREE(T) (&gdip_obj_free<T>)

static inline void
Check_NULL(void *p, const char *msg)
{
    if (p == NULL) {
        rb_raise(eGdiplus, msg);
    }
}

static inline void
Check_Frozen(VALUE v)
{
    if (RB_OBJ_FROZEN(v)) {
        rb_error_frozen_object(v);
    }
}

template<typename T>
static inline void
Check_LastStatus(T obj)
{
    Status status = obj->GetLastStatus();
    if (status == Ok) { return; }

    delete obj;
    if (status < 22) {
        rb_raise(eGdiplus, "Status.%s, Some error occurred.", GpStatusStrs[status]);
    }
    else {
        rb_raise(eGdiplus, "Unknown error occurred. (Status: %d)", status);
    }
}

static inline void
Check_Status(Status status) {
    if (status == Ok) { return; }
    if (status < 22) {
        rb_raise(eGdiplus, "Status.%s, Some error occurred.", GpStatusStrs[status]);
    }
    else {
        rb_raise(eGdiplus, "Unknown error occurred. (GpStatus: %d)", status);
    }
}

#define NOT_IMPLEMENTED_ERROR rb_raise(rb_eNotImpError, "not implemented yet")


#endif /* RUBY_GDIPLUS_H */
