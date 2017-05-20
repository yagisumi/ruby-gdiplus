/*
 * gdip_utils.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"
#include <windows.h>
#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
#endif

VALUE
util_encode_to_utf8(VALUE str)
{
    #ifdef HAVE_RUBY_ENCODING_H
    str = rb_str_conv_enc(str, rb_enc_get(str), rb_utf8_encoding());
    #endif
    return str;
}

VALUE
util_associate_utf8(VALUE str)
{
    #ifdef HAVE_RUBY_ENCODING_H
    str = rb_enc_associate_index(str, rb_utf8_encindex());
    #endif
    return str;
}

VALUE
util_utf8_sprintf(const char* format, ...)
{
    va_list list;
    va_start(list, format);

    int len = _vscprintf(format, list);
    char *buf = static_cast<char *>(ruby_xmalloc(len + 1));
    vsprintf(buf, format, list);
    VALUE r = rb_utf8_str_new_cstr(buf);
    ruby_xfree(buf);
    va_end(list);
    return r;
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
    //WCHAR *ptr = RB_ZALLOC_N(WCHAR, len);
    VALUE r = rb_str_new(NULL, len * sizeof(WCHAR));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, RString_Ptr<WCHAR *>(r), len);
    RB_GC_GUARD(v);
    //VALUE r = rb_str_new((char *)(ptr), len * sizeof(WCHAR));
    //ruby_xfree(ptr);
    
    return r;
}

VALUE
util_utf8_str_new_from_wstr(const wchar_t * wstr)
{
    if (wstr == NULL) {
        return rb_utf8_str_new_cstr("");
    }
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    //char *str = RB_ZALLOC_N(char, len);
    VALUE r = rb_utf8_str_new(NULL, len - 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, RSTRING_PTR(r), len, NULL, NULL);
    //VALUE r = rb_utf8_str_new_cstr(str);
    //ruby_xfree(str);

    return r;
}

bool
util_extname(VALUE str, char (&ext)[6])
{
    if (!RB_TYPE_P(str, RUBY_T_STRING)) {
        return false;
    }
    str = util_encode_to_utf8(str);
    char *ptr = RSTRING_PTR(str);
    memset(ext, 0, 6);

    int len = RSTRING_LEN(str);
    for (int i = len - 1; i >= 0; i--) {
        if (ptr[i] == '.') {
            for (int j = i; j < len; j ++) {
                if(ptr[j] >= 65 && ptr[j] <= 90) {
                    ext[j - i] = ptr[j] + 32;
                }
                else {
                    ext[j - i] = ptr[j];
                }
            }
            return true;
        }
        if (len - i > 4) {
            break;
        }
    }
    RB_GC_GUARD(str);
    return false;
}