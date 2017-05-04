#ifndef RUBY_GDIPLUS_H
#define RUBY_GDIPLUS_H 1

#include "ruby.h"
#include "ruby_compatible.h"
#include <windows.h>
#include <rpc.h>
#include <gdiplus.h>
using namespace Gdiplus;

extern VALUE mGdiplus;
extern VALUE eGdiplus;

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
#ifdef GDIPLUS_DEBUG
#include <stdio.h>
#include <stdarg.h>
void dp(const char *fmt, ...);
#else
#define dp(...)
#endif

/* Utils */
template<typename T>
static inline T 
Data_Ptr(VALUE obj)
{
    Check_Type(obj, RUBY_T_DATA);
    return static_cast<T>(RTYPEDDATA_DATA(obj));
}

/* gdip_utils.cpp */
VALUE util_encode_to_utf8(VALUE str);
VALUE util_utf16_str_new(VALUE v);
VALUE util_utf8_str_new_from_wstr(const wchar_t * wstr);

#endif /* RUBY_GDIPLUS_H */
