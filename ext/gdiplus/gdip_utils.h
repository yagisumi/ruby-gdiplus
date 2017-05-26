/*
 * gdip_utils.h
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */

#define _VERBOSE(...) rb_warning(__VA_ARGS__)
#define _WARNING(...) rb_warn(__VA_ARGS__)
#define _RB_ARRAY_P(v) RB_TYPE_P(v, RUBY_T_ARRAY)
#define _RB_STRING_P(v) RB_TYPE_P(v, RUBY_T_STRING)
#define _RB_INTEGER_P(v) RB_INTEGER_TYPE_P(v)
#define _RB_FLOAT_P(v) RB_TYPE_P(v, RUBY_T_FLOAT)
#define _RB_SYMBOL_P(v) RB_SYMBOL_P(v)

static inline float
NUM2SINGLE(VALUE num)
{
    return static_cast<float>(NUM2DBL(num));
}
/*
static inline VALUE
SINGLE2NUM(float n)
{
    return DBL2NUM(static_cast<double>(n));
}
*/

static inline VALUE
SINGLE2NUM(float n)
{
    return DBL2NUM(round(static_cast<double>(n) * 1000000.0) / 1000000.0);
}

static inline const char * __method__() { return rb_id2name(rb_frame_this_func()); }
static inline const char * __class__(VALUE self) { return rb_class2name(CLASS_OF(self)); }

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

static inline bool
Float_p(VALUE v1)
{
    return (_RB_FLOAT_P(v1));
}

static inline bool
Float_p(VALUE v1, VALUE v2)
{
    return (Float_p(v1) && Float_p(v2));
}

static inline bool
Float_p(VALUE v1, VALUE v2, VALUE v3)
{
    return (Float_p(v1, v2) && Float_p(v3));
}

static inline bool
Float_p(VALUE v1, VALUE v2, VALUE v3, VALUE v4)
{
    return (Float_p(v1, v2, v3) && Float_p(v4));
}
