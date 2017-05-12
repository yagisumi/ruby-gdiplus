/*
 * ruby_gdiplus.h
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#ifndef RUBY_GDIPLUS_H
#define RUBY_GDIPLUS_H 1

#include <ruby.h>
#include "ruby_compatible.h"
#include <windows.h>
#include <rpc.h>
#include <gdiplus.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

using namespace Gdiplus;

extern VALUE mGdiplus;
extern VALUE mInternals;
extern VALUE eGdiplus;

extern VALUE cGuid;
extern VALUE cImageCodecInfo;
extern VALUE cImage;
extern VALUE cBitmap;
extern VALUE cPixelFormat;
extern VALUE cEncoderParameterValueType;
extern VALUE cEncoder;
extern VALUE cValueType;
extern VALUE cEnumInt;

extern const rb_data_type_t tGuid;
extern const rb_data_type_t tImageCodecInfo;
extern const rb_data_type_t tImage;
extern const rb_data_type_t tBitmap;
extern const rb_data_type_t tPixelFormat;
extern const rb_data_type_t tEncoderParameterValueType;
extern const rb_data_type_t tValueType;
extern const rb_data_type_t tEnumInt;

void Init_codec();
void Init_image();
void Init_bitmap();
void Init_enum();

/* gdiplus.cpp */
extern const char *GpStatusStrs[22];
extern int gdip_refcount;
extern bool gdip_end_flag;
void gdiplus_shutdown();

static inline void GdiplusAddRef() { ++gdip_refcount; }
static inline void GdiplusRelease() {
    --gdip_refcount;
    if (gdip_end_flag && gdip_refcount == 0) {
        gdiplus_shutdown();
    }
}

/* gdip_codec.cpp */
VALUE gdip_guid_create(GUID *guid);

/* gdip_enum.cpp */
VALUE gdip_enum_const_get(VALUE self);
VALUE gdip_pxlfmt_get(PixelFormat n);
VALUE gdip_valuetype_get(int n);
VALUE gdip_encoder_get(GUID *guid);

#ifndef PixelFormat32bppCMYK
#define PixelFormat32bppCMYK       (15 | (32 << 8))
#endif

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

/* Utils */
static inline const char * __method__() { return rb_id2name(rb_frame_this_func()); }
static inline const char * __class__(VALUE self) { return rb_class2name(CLASS_OF(self)); }

static inline VALUE
_KLASS(const rb_data_type_t *type)
{
    return *static_cast<VALUE *>(type->data);
}

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
typeddata_alloc(VALUE klass)
{
    void *ptr = RB_ZALLOC(T);
    DPT("alloc");
    VALUE r = _Data_Wrap_Struct(klass, type, ptr);
    return r;
}

#if GDIPLUS_DEBUG
    template<typename T, const rb_data_type_t *type>
    static VALUE
    typeddata_alloc_null(VALUE klass)
    {
        DPT("alloc"); // T is used for debug
        VALUE r = _Data_Wrap_Struct(klass, type, NULL);
        return r;
    }
    #define TYPEDDATA_ALLOC_NULL(T, type) &typeddata_alloc_null<T, type>
#else
    template<const rb_data_type_t *type>
    static VALUE
    typeddata_alloc_null(VALUE klass)
    {
        VALUE r = _Data_Wrap_Struct(klass, type, NULL);
        return r;
    }
    #define TYPEDDATA_ALLOC_NULL(T, type) &typeddata_alloc_null<type>
#endif /* GDIPLUS_DEBUG */

template<typename T>
static void
gdip_default_free(void *ptr)
{
    DPT("free");
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

#define NOT_IMPLEMENTED_ERROR rb_raise(rb_eNotImpError, "not implemented yet")

static inline bool
Integer_p(VALUE v1)
{
    return (RB_INTEGER_TYPE_P(v1));
}

static inline bool
Integer_p(VALUE v1, VALUE v2)
{
    return (Integer_p(v1) && Integer_p(v2));
}

static inline bool
Integer_p(VALUE v1, VALUE v2, VALUE v3)
{
    return (Integer_p(v1, v2) && Integer_p(v3));
}

static inline bool
Integer_p(VALUE v1, VALUE v2, VALUE v3, VALUE v4)
{
    return (Integer_p(v1, v2, v3) && Integer_p(v4));
}

/* gdip_utils.cpp */
VALUE util_encode_to_utf8(VALUE str);
VALUE util_associate_utf8(VALUE str);
VALUE util_utf8_sprintf(const char* format, ...);
VALUE util_utf16_str_new(VALUE v);
VALUE util_utf8_str_new_from_wstr(const wchar_t * wstr);

#endif /* RUBY_GDIPLUS_H */
