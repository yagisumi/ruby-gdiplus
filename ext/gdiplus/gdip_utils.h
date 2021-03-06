/*
 * gdip_utils.h
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */


static inline int
_RB_RANGE_P(VALUE v)
{
    return RB_TEST(rb_obj_is_kind_of(v, rb_cRange));
}

static inline float
NUM2SINGLE(VALUE num)
{
    return static_cast<float>(NUM2DBL(num));
}

static inline VALUE
SINGLE2NUM(float n)
{
    return DBL2NUM(round(static_cast<double>(n) * 1000000.0) / 1000000.0);
}

static inline const char * __method__() { return rb_id2name(rb_frame_this_func()); }
static inline const char * __class__(VALUE self) { return rb_class2name(CLASS_OF(self)); }

template <typename T>
static inline T
clamp(T val, T min, T max)
{
    if (val < min) return min;
    else if (max < val) return max;
    else return val;
}

static inline bool
Integer_p(int argc, VALUE *argv)
{
    for (int i = 0; i < argc; ++i) {
        if (!RB_INTEGER_TYPE_P(argv[i])) return false;
    }
    return true;
}

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
Float_p(int argc, VALUE *argv)
{
    for (int i = 0; i < argc; ++i) {
        if (!_RB_FLOAT_P(argv[i])) return false;
    }
    return true;
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

static inline bool
Typeddata_p(int argc, VALUE *argv, const rb_data_type_t *type)
{
    for (int i = 0; i < argc; ++i) {
        if (!_KIND_OF(argv[i], type)) return false;
    }
    return true;
}