#ifndef RUBY_COMPATIBLE_H_
#define RUBY_COMPATIBLE_H_ 1

#include <ruby.h>
#ifdef HAVE_RUBY_VERSION_H
#include <ruby/version.h>
#else
#include <version.h>
#define RUBY_API_VERSION_MAJOR RUBY_VERSION_MAJOR
#define RUBY_API_VERSION_MINOR RUBY_VERSION_MINOR
#define RUBY_API_VERSION_TEENY RUBY_VERSION_TEENY
#define RUBY_API_VERSION_CODE (RUBY_API_VERSION_MAJOR*10000+RUBY_API_VERSION_MINOR*100+RUBY_API_VERSION_TEENY)
#endif

#ifndef RB_FIX2INT
#define RB_FIX2INT(x) FIX2INT(x)
#endif
#ifndef RB_NUM2INT
#define RB_NUM2INT(x) NUM2INT(x)
#endif
#ifndef RB_INT2NUM
#define RB_INT2NUM(x) INT2NUM(x)
#endif
#ifndef RB_INT2FIX
#define RB_INT2FIX(x) INT2FIX(x)
#endif
#ifndef RUBY_FIXNUM_MAX
#define RUBY_FIXNUM_MAX FIXNUM_MAX
#endif
#ifndef RUBY_FIXNUM_MIN
#define RUBY_FIXNUM_MIN FIXNUM_MIN
#endif
#ifndef RB_FIX2UINT
#define RB_FIX2UINT(x) FIX2UINT(x)
#endif
#ifndef RB_NUM2UINT
#define RB_NUM2UINT(x) NUM2UINT(x)
#endif
#ifndef RB_UINT2NUM
#define RB_UINT2NUM(x) UINT2NUM(x)
#endif
#ifndef RB_NUM2LONG
#define RB_NUM2LONG(x) NUM2LONG(x)
#endif
#ifndef RB_LONG2NUM
#define RB_LONG2NUM(x) LONG2NUM(x)
#endif
#ifndef RB_NUM2ULONG
#define RB_NUM2ULONG(x) NUM2ULONG(x)
#endif
#ifndef RB_ULONG2NUM
#define RB_ULONG2NUM(x) ULONG2NUM(x)
#endif
#ifndef RB_NUM2LL
#define RB_NUM2LL(x) NUM2LL(x)
#endif
#ifndef RB_NUM2ULL
#define RB_NUM2ULL(x) NUM2ULL(x)
#endif
#ifndef RB_NUM2CHR
#define RB_NUM2CHR(x) NUM2CHR(x)
#endif
#ifndef RB_CHR2FIX
#define RB_CHR2FIX(x) CHR2FIX(x)
#endif
#ifndef RB_ID2SYM
#define RB_ID2SYM(x) ID2SYM(x)
#endif
#ifndef RB_SYM2ID
#define RB_SYM2ID(x) SYM2ID(x)
#endif

#ifdef FIX2SHORT
    #ifndef RB_FIX2SHORT
#ifndef RB_NUM2SHORT
#define RB_NUM2SHORT(x) NUM2SHORT(x)
#endif
#ifndef RB_NUM2USHORT
#define RB_NUM2USHORT(x) NUM2USHORT(x)
#endif
#ifndef RB_FIX2SHORT
#define RB_FIX2SHORT(x) FIX2SHORT(x)
#endif
    #endif /* RB_FIX2SHORT */
#else
/* not yet */
#endif /* FIX2SHORT */

#ifndef DBL2NUM
#define DBL2NUM(dbl)  rb_float_new(dbl)
#endif
#if SIZEOF_LONG == SIZEOF_VOIDP
#define _RB_NUM2ID(x) RB_NUM2ULONG(x)
#elif SIZEOF_LONG_LONG == SIZEOF_VOIDP
#define _RB_NUM2ID(x) RB_NUM2ULL(x)
#endif

#ifndef RB_TEST
#define RB_TEST(v) RTEST(v)
#endif
#ifndef RB_FIXNUM_P
#define RB_FIXNUM_P(f) FIXNUM_P(f)
#endif
#ifndef RB_SYMBOL_P
#define RB_SYMBOL_P(v) SYMBOL_P(v)
#endif
#ifndef RB_SPECIAL_CONST_P
#define RB_SPECIAL_CONST_P(x) SPECIAL_CONST_P(x)
#endif
#ifndef RB_IMMEDIATE_P
#define RB_IMMEDIATE_P(x) IMMEDIATE_P(x)
#endif
#ifndef RB_BUILTIN_TYPE
#define RB_BUILTIN_TYPE(x) BUILTIN_TYPE(x)
#endif
#ifndef RB_NIL_P
#define RB_NIL_P(v) NIL_P(v)
#endif
#ifdef FLONUM_P
#ifndef RB_FLONUM_P
#define RB_FLONUM_P(x) FLONUM_P(x)
#endif
#else
#define RB_FLONUM_P(x) (0)
#endif /* FLONUM_P */
#ifndef RB_OBJ_FREEZE
#define RB_OBJ_FREEZE(x) OBJ_FREEZE(x)
#endif
#ifndef RB_OBJ_FROZEN
#define RB_OBJ_FROZEN(x) OBJ_FROZEN(x)
#endif
#ifndef RB_ALLOC
#define RB_ALLOC(type) ALLOC(type)
#endif

#if RUBY_API_VERSION_CODE < 10900
#define RUBY_Qfalse   Qfalse
#define RUBY_Qtrue    Qtrue
#define RUBY_Qnil     Qnil
#define RUBY_Qundef   Qundef
#define RUBY_T_NONE   T_NONE
#define RUBY_T_NIL    T_NIL
#define RUBY_T_OBJECT T_OBJECT
#define RUBY_T_CLASS  T_CLASS
#define RUBY_T_ICLASS T_ICLASS
#define RUBY_T_MODULE T_MODULE
#define RUBY_T_FLOAT  T_FLOAT
#define RUBY_T_STRING T_STRING
#define RUBY_T_REGEXP T_REGEXP
#define RUBY_T_ARRAY  T_ARRAY
#define RUBY_T_HASH   T_HASH
#define RUBY_T_STRUCT T_STRUCT
#define RUBY_T_BIGNUM T_BIGNUM
#define RUBY_T_FILE   T_FILE
#define RUBY_T_FIXNUM T_FIXNUM
#define RUBY_T_TRUE   T_TRUE
#define RUBY_T_FALSE  T_FALSE
#define RUBY_T_DATA   T_DATA
#define RUBY_T_MATCH  T_MATCH
#define RUBY_T_SYMBOL T_SYMBOL
/* RUBY_T_RATIONAL */
/* RUBY_T_COMPLEX */
/* RUBY_T_IMEMO */
#define RUBY_T_UNDEF  T_UNDEF
#define RUBY_T_NODE   T_NODE
/* RUBY_T_ZOMBIE */
#define RUBY_T_MASK   T_MASK
#endif /* RUBY_API_VERSION_CODE < 10900 */

#ifndef RB_INTEGER_TYPE_P
#define RB_INTEGER_TYPE_P(obj) rb_integer_type_p(obj)
    static inline int
    rb_integer_type_p(VALUE obj)
    {
        return (RB_FIXNUM_P(obj) ||
                (!RB_SPECIAL_CONST_P(obj) &&
                 RB_BUILTIN_TYPE(obj) == RUBY_T_BIGNUM));
    }
#endif /* RB_INTEGER_TYPE_P */

#ifndef RB_FLOAT_TYPE_P
#define RB_FLOAT_TYPE_P(obj) (!RB_SPECIAL_CONST_P(obj) && RB_BUILTIN_TYPE(obj) == RUBY_T_FLOAT)
#endif /* RB_FLOAT_TYPE_P */
#ifndef RB_TYPE_P
#define RB_TYPE_P(obj, type) ( \
        ((type) == RUBY_T_FIXNUM) ? RB_FIXNUM_P(obj) : \
        ((type) == RUBY_T_TRUE) ? ((obj) == RUBY_Qtrue) : \
        ((type) == RUBY_T_FALSE) ? ((obj) == RUBY_Qfalse) : \
        ((type) == RUBY_T_NIL) ? ((obj) == RUBY_Qnil) : \
        ((type) == RUBY_T_UNDEF) ? ((obj) == RUBY_Qundef) : \
        ((type) == RUBY_T_SYMBOL) ? RB_SYMBOL_P(obj) : \
        ((type) == RUBY_T_FLOAT) ? RB_FLOAT_TYPE_P(obj) : \
        (!RB_SPECIAL_CONST_P(obj) && RB_BUILTIN_TYPE(obj) == (type)))
#endif /* RB_TYPE_P */

#ifndef RUBY_DEFAULT_FREE
#define RUBY_DEFAULT_FREE ((RUBY_DATA_FUNC)-1)
#endif
#ifndef RUBY_NEVER_FREE
#define RUBY_NEVER_FREE   ((RUBY_DATA_FUNC)0)
#endif
#ifndef RUBY_TYPED_FREE_IMMEDIATELY
#define RUBY_TYPED_FREE_IMMEDIATELY  1
#endif

#ifndef HAVE_TYPE_RB_DATA_TYPE_T
    typedef struct rb_data_type_struct rb_data_type_t;
    
    struct rb_data_type_struct {
        const char *wrap_struct_name;
        struct {
            void (*dmark)(void*);
            void (*dfree)(void*);
            size_t (*dsize)(const void *);
            void *reserved[2]; /* For future extension.
                                  This array *must* be filled with ZERO. */
        } function;
        const rb_data_type_t *parent;
        void *data;        /* This area can be used for any purpose
                              by a programmer who define the type. */
    };  
    static inline int rb_typeddata_is_kind_of(VALUE obj, const rb_data_type_t *data_type)
    {
        VALUE *klass_ptr = (VALUE *)data_type->data;
        if (RB_TYPE_P(obj, RUBY_T_DATA) && klass_ptr && RB_TYPE_P(*klass_ptr, RUBY_T_CLASS)) {
            return RB_TEST(rb_obj_is_kind_of(obj, *klass_ptr));
        }
        else { return 0; }
    }
#define TypedData_Wrap_Struct(klass,data_type,sval) Data_Wrap_Struct(klass, (data_type)->function.dmark, (data_type)->function.dfree, sval)
#define TypedData_Get_Struct(obj,type,data_type,sval) Data_Get_Struct(obj,type,sval)
#define RTYPEDDATA_DATA(v) DATA_PTR(v)
#define _DATA_PTR(v) DATA_PTR(v)
#else /* HAVE_TYPE_RB_DATA_TYPE_T */
#define _DATA_PTR(v) RTYPEDDATA_DATA(v)
#endif /* HAVE_TYPE_RB_DATA_TYPE_T */
#define _KIND_OF(obj, data_type) rb_typeddata_is_kind_of(obj, data_type)

#if RUBY_API_VERSION_CODE < 20200
    #define _MAKE_DATA_TYPE(name, mark, free, size, parent_type_ptr, klass_ptr) {\
        name,\
        {mark, free, size,},\
        parent_type_ptr, klass_ptr\
    }
#else
    #define _MAKE_DATA_TYPE(name, mark, free, size, parent_type_ptr, klass_ptr) {\
        name,\
        {mark, free, size,},\
        parent_type_ptr, klass_ptr,\
        RUBY_TYPED_FREE_IMMEDIATELY\
    }
#endif /* RUBY_API_VERSION_CODE < 20200 */
    static inline VALUE
    _KLASS(const rb_data_type_t *type)
    {
        return *(VALUE *)(type->data);
    }
#define _Data_Wrap_Struct(klass,data_type,sval) TypedData_Wrap_Struct(klass,data_type,sval)
#define _Data_Get_Struct(obj,type,data_type,sval) TypedData_Get_Struct(obj,type,data_type,sval)
#ifndef RUBY_DEFAULT_FREE
#define RUBY_DEFAULT_FREE ((RUBY_DATA_FUNC)-1)
#endif
#ifndef RUBY_TYPED_DEFAULT_FREE
#define RUBY_TYPED_DEFAULT_FREE RUBY_DEFAULT_FREE
#endif
#ifndef RUBY_NEVER_FREE
#define RUBY_NEVER_FREE   ((RUBY_DATA_FUNC)0)
#endif
#ifndef RUBY_TYPED_NEVER_FREE
#define RUBY_TYPED_NEVER_FREE   RUBY_NEVER_FREE
#endif

#ifndef RB_ALLOC_N
#define RB_ALLOC_N(type,n) ALLOC_N(type,n)
#endif
#ifndef RB_ALLOC
#define RB_ALLOC(type) ALLOC(type)
#endif
#ifndef ZALLOC_N
#define ZALLOC_N(type,n) ((type*)ruby_xcalloc((size_t)(n),sizeof(type)))
#endif
#ifndef ZALLOC
#define ZALLOC(type) (ZALLOC_N(type,1))
#endif
#ifndef RB_ZALLOC_N
#define RB_ZALLOC_N(type,n) ZALLOC_N(type,n)
#endif
#ifndef RB_ZALLOC
#define RB_ZALLOC(type) ZALLOC(type)
#endif
#ifndef RB_REALLOC_N
#define RB_REALLOC_N(var,type,n) REALLOC_N(var,type,n)
#endif

#ifndef rb_ary_new2
#define rb_ary_new_capa rb_ary_new2
#endif
#ifndef rb_ary_new3
#define rb_ary_new_from_args rb_ary_new3
#endif
#ifndef rb_ary_new4
#define rb_ary_new_from_values rb_ary_new4
#endif

#ifndef rb_str_new2
#define rb_str_new_cstr rb_str_new2
#endif
#ifndef rb_tainted_str_new2
#define rb_tainted_str_new_cstr rb_tainted_str_new2
#endif
#ifndef rb_str_new3
#define rb_str_new_shared rb_str_new3
#endif
#ifndef rb_str_new4
#define rb_str_new_frozen rb_str_new4
#endif
#ifndef rb_str_new5
#define rb_str_new_with_class rb_str_new5
#endif
#ifndef rb_str_buf_new2
#define rb_str_buf_new_cstr rb_str_buf_new2
#endif
#if RUBY_API_VERSION_CODE < 20200
#define rb_str_cat_cstr rb_str_cat2
#endif

#if RUBY_API_VERSION_CODE < 10900
    static inline VALUE
    rb_str_length(VALUE str)
    {
        return LONG2NUM(RSTRING(str)->len);
    }
#endif

#if !defined(HAVE_RUBY_ENCODING_H)
#define rb_usascii_str_new rb_str_new
#define rb_usascii_str_new_cstr rb_str_new2
#define rb_utf8_str_new rb_str_new
#define rb_utf8_str_new_cstr rb_str_new2
#define rb_enc_reg_new(s, len, enc, options) rb_reg_new(s, len, options)
#elif RUBY_API_VERSION_CODE < 20200
#include <ruby/encoding.h>
    //#define rb_utf8_str_new(ptr, len) rb_enc_str_new(ptr, len, rb_utf8_encoding())
    static inline VALUE 
    rb_utf8_str_new(const char *ptr, long len)
    {
        return rb_enc_str_new(ptr, len, rb_utf8_encoding());
    }
    //#define rb_utf8_str_new_cstr(ptr) rb_enc_str_new(ptr, strlen(ptr), rb_utf8_encoding())
    static inline VALUE
    rb_utf8_str_new_cstr(const char *ptr)
    {
        return rb_enc_str_new(ptr, strlen(ptr), rb_utf8_encoding());
    }
#endif /* HAVE_RUBY_ENCODING_H */

#ifdef HAVE_RUBY_ONIGURUMA_H
#include <ruby/oniguruma.h>
#define RE_OPTION_NONE       ONIG_OPTION_NONE
#define RE_OPTION_IGNORECASE ONIG_OPTION_IGNORECASE
#define RE_OPTION_EXTENDED   ONIG_OPTION_EXTEND
#define RE_OPTION_MULTILINE  ONIG_OPTION_MULTILINE
#define RE_OPTION_SINGLELINE ONIG_OPTION_SINGLELINE
#define RE_OPTION_LONGEST    ONIG_OPTION_FIND_LONGEST
#else
#include <regex.h>
#define RE_OPTION_NONE           (0L)
#define ONIG_OPTION_NONE         (0L)
#define ONIG_OPTION_IGNORECASE   RE_OPTION_IGNORECASE
#define ONIG_OPTION_EXTEND       RE_OPTION_EXTENDED
#define ONIG_OPTION_MULTILINE    RE_OPTION_MULTILINE
#define ONIG_OPTION_SINGLELINE   RE_OPTION_SINGLELINE
#define ONIG_OPTION_FIND_LONGEST RE_OPTION_LONGEST
#endif /* HAVE_RUBY_ONIGURUMA_H */

#if RUBY_API_VERSION_CODE < 10900
    #if defined(__cplusplus)
    extern "C" { 
    #endif
        #include <st.h>
    #if defined(__cplusplus)
    }
    #endif
    typedef int st_index_t;
    static inline st_index_t
    rb_memhash(const void *ptr, long len)
    {
        VALUE str = rb_str_new((const char *)ptr, len);
        return rb_str_hash(str);
    }
#endif /* RUBY_API_VERSION_CODE < 10900 */
#define _FOREACH_FUNC(func) ((int (*)(ANYARGS))(func))

// rb_frame_callee is rb_frame_caller in 1.9 ( ? rb_frame_this_func)

#if RUBY_API_VERSION_CODE < 10900
  
    static inline VALUE rb_rational_new(VALUE x, VALUE y)
    {
      rb_f_require(rb_mKernel, rb_str_new_cstr("rational"));
      return rb_funcall(rb_mKernel, rb_intern("Rational"), 2, x, y);
    }
    #include <string.h>
    static inline int _RB_RATIONAL_P(VALUE x)
    {
        VALUE path = rb_class_path(CLASS_OF(x));
        if (strcmp("Rational", RSTRING_PTR(path)) == 0) {
            return 1;
        }
        else {
            return 0;
        }
    }
    static inline VALUE rb_rational_den(VALUE x)
    {
        return rb_funcall(x, rb_intern("denominator"), 0);
    }
    static inline VALUE rb_rational_num(VALUE x)
    {
        return rb_funcall(x, rb_intern("numerator"), 0);
    }
#else
    static inline int _RB_RATIONAL_P(VALUE x)
    {
        return rb_cRational == CLASS_OF(x);
    }
    #if RUBY_API_VERSION_CODE < 20200
        static inline VALUE rb_rational_den(VALUE x)
        {
            return ((struct RRational *)(x))->den;
        }
        static inline VALUE rb_rational_num(VALUE x)
        {
            return ((struct RRational *)(x))->num;
        }
    #endif
#endif

#if RUBY_API_VERSION_CODE >= 10900 && RUBY_API_VERSION_CODE < 20100
    #define RSTRUCT_GET(st, idx)    rb_struct_aref(st, INT2NUM(idx))
#endif

#if RUBY_API_VERSION_CODE < 10900
    static inline VALUE
    _rb_range_beg(VALUE range)
    {
        return rb_iv_get(range, "begin");
    }
    static inline VALUE
    _rb_range_end(VALUE range)
    {
        return rb_iv_get(range, "end");
    }
    static inline int
    _rb_range_excl_p(VALUE range)
    {
        return RB_TEST(rb_iv_get(range, "excl"));
    }
#else
    static inline VALUE
    _rb_range_beg(VALUE range)
    {
        return RSTRUCT_GET(range, 0);
    }
    static inline VALUE
    _rb_range_end(VALUE range)
    {
        return RSTRUCT_GET(range, 1);
    }
    static inline int
    _rb_range_excl_p(VALUE range)
    {
        return RB_TEST(RSTRUCT_GET(range, 2));
    }
#endif

#if RUBY_API_VERSION_CODE < 20000
    static inline void
    rb_error_frozen_object(VALUE frozen_obj)
    {
        rb_error_frozen(rb_class2name(CLASS_OF(frozen_obj)));
    }
#elif RUBY_API_VERSION_CODE < 20200
    static inline void
    rb_error_frozen_object(VALUE frozen_obj)
    {
        rb_raise(rb_eRuntimeError, "can't modify frozen %" PRIsVALUE, CLASS_OF(frozen_obj));
    }
#endif

#define _PROTECT_FUNC(func) ((VALUE (*)(VALUE))(func))

static inline VALUE
_wrap_tmp_buffer(void *ptr)
{
    return Data_Wrap_Struct(rb_cObject, 0, RUBY_DEFAULT_FREE, ptr);
}

static inline void *
_rb_alloc_tmp_buffer(volatile VALUE *store, long len)
{
    void *ptr;
    ptr = ruby_xmalloc(len);
    *store = Data_Wrap_Struct(rb_cObject, 0, RUBY_DEFAULT_FREE, ptr);
    return ptr;
}

static inline void
_rb_free_tmp_buffer(volatile VALUE *store)
{
    VALUE s = *store;
    *store = 0;
    if (RB_TYPE_P(s, RUBY_T_DATA)) {
        void *ptr = DATA_PTR(s);
        if (ptr) {
            ruby_xfree(ptr);
            DATA_PTR(s) = ((void*)0);
        }
    }
}

#if RUBY_API_VERSION_CODE < 10900
    static inline void *
    rb_alloc_tmp_buffer(volatile VALUE *store, long len)
    {
        return _rb_alloc_tmp_buffer(store, len);
    }

    static inline void
    rb_free_tmp_buffer(volatile VALUE *store)
    {
        _rb_free_tmp_buffer(store);
    }
#endif

/* Utils */
#define _VERBOSE(...) rb_warning(__VA_ARGS__)
#define _WARNING(...) rb_warn(__VA_ARGS__)
#define _RB_ARRAY_P(v) RB_TYPE_P(v, RUBY_T_ARRAY)
#define _RB_STRING_P(v) RB_TYPE_P(v, RUBY_T_STRING)
#define _RB_INTEGER_P(v) RB_INTEGER_TYPE_P(v)
#define _RB_FLOAT_P(v) RB_TYPE_P(v, RUBY_T_FLOAT)
#define _RB_SYMBOL_P(v) RB_SYMBOL_P(v)
#define _RB_HASH_P(v) RB_TYPE_P(v, RUBY_T_HASH)

#if SIZEOF_LONG_LONG == SIZEOF_VOIDP
#define _RB_ID2NUM(v) ULL2NUM(v)
#else
#define _RB_ID2NUM(v) RB_ULONG2NUM(v)
#endif

#endif /* RUBY_COMPATIBLE_H_ */

