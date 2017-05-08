/*
 * ruby_gdiplus.h
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#ifndef RUBY_GDIPLUS_H
#define RUBY_GDIPLUS_H 1

#include "ruby.h"
#include "ruby_compatible.h"
#include <windows.h>
#include <rpc.h>
#include <gdiplus.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

using namespace Gdiplus;

extern VALUE mGdiplus;
extern VALUE eGdiplus;

extern VALUE cGuid;
extern VALUE cImageCodecInfo;

extern const rb_data_type_t tGuid;
extern const rb_data_type_t tImageCodecInfo;

void Init_codec();

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

/* Debug */
#define GDIPLUS_DEBUG 1

#if GDIPLUS_DEBUG
void dp(const char *fmt, ...);
#else
#define dp(...)
#endif

/* Utils */
template<typename T>
static inline T 
Data_Ptr(VALUE obj)
{
    if (!RB_TYPE_P(obj, RUBY_T_DATA)) {
        rb_raise(rb_eTypeError, "wrong argument type");
    }
    return static_cast<T>(RTYPEDDATA_DATA(obj));
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
    return TypedData_Wrap_Struct(klass, type, ptr);
}

template<const rb_data_type_t *type>
static VALUE
typeddata_alloc_null(VALUE klass)
{
    return TypedData_Wrap_Struct(klass, type, NULL);
}

/* gdip_utils.cpp */
VALUE util_encode_to_utf8(VALUE str);
VALUE util_associate_utf8(VALUE str);
VALUE util_utf8_sprintf(const char* format, ...);
VALUE util_utf16_str_new(VALUE v);
VALUE util_utf8_str_new_from_wstr(const wchar_t * wstr);

#endif /* RUBY_GDIPLUS_H */
