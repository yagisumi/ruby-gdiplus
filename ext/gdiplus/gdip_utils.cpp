#include "ruby_gdiplus.h"
#include <windows.h>
#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
#endif

VALUE
util_encode_to_utf8(VALUE str) {
    #ifdef HAVE_RUBY_ENCODING_H
    str = rb_str_conv_enc(str, rb_enc_get(str), rb_utf8_encoding());
    #endif
    return str;
}

VALUE
util_utf16_str_new(VALUE v)
{
    v = StringValue(v);
    #ifdef HAVE_RUBY_ENCODING_H
    v = rb_str_conv_enc(v, rb_enc_get(v), rb_utf8_encoding());
    #endif
    char *str = RSTRING_PTR(v);
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    WCHAR *ptr = RB_ZALLOC_N(WCHAR, len);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, ptr, len);
    RB_GC_GUARD(v);
    VALUE r = rb_str_new((char *)(ptr), len * sizeof(WCHAR));
    ruby_xfree(ptr);
    
    return r;
}

VALUE
util_utf8_str_new_from_wstr(const wchar_t * wstr)
{
    int clen = lstrlenW(wstr);
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr, clen, NULL, 0, NULL, NULL);
    VALUE src = rb_utf8_str_new(0, len);
    WideCharToMultiByte(CP_UTF8, 0, wstr, clen, RSTRING_PTR(src), len, NULL, NULL);
    return src;
}
