#ifndef RUBY_EXT_UTILS_HPP_
#define RUBY_EXT_UTILS_HPP_ 1

#include <functional>
#include <ruby.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <typeinfo>
#ifdef __GNUC__
#include <cxxabi.h>
#endif

template <typename T>
static const char *
type_name()
{
#ifdef __GNUC__
    const int buf_size = 32;
    static char tname[buf_size];
    if (tname[0] != 0) {
        return tname;
    }

    const std::type_info& id = typeid(T);
    int status;
    char *name = abi::__cxa_demangle(id.name(), NULL, 0, &status);
    if (name != NULL) {
        if (status == 0) {
            strncpy(tname, name, buf_size - 1);
        }
        else {
            strncpy(tname, id.name(), buf_size - 1);
        }
        free(name);
    }
    
    return tname;
#else
    return typeid(T).name();
#endif
}

/* 
 * undefined reference to ... [g++ (tdm-1) 4.5.2]
 */
// template<typename T>
// static void
// tmp_obj_free(T ptr)
// {
//     if (ptr) {
//         delete ptr;
//     }
// }

// template<typename T>
// static void
// tmp_ary_free(T ptr)
// {
//     if (ptr) {
//         delete[] ptr;
//     }
// }

template<typename T>
static void
tmp_obj_free(void *ptr)
{
    if (ptr) {
        T obj = static_cast<T>(ptr);
        delete obj;
    }
}

template<typename T>
static void
tmp_ary_free(void *ptr)
{
    if (ptr) {
        T obj = static_cast<T>(ptr);
        delete[] obj;
    }
}


template<typename T>
static inline VALUE
wrap_tmp_obj(T ptr)
{
    return Data_Wrap_Struct(rb_cObject, 0, tmp_obj_free<T>, ptr);
}

template<typename T>
static inline VALUE
wrap_tmp_ary(T ptr)
{
    return Data_Wrap_Struct(rb_cObject, 0, tmp_ary_free<T>, ptr);
}

void delete_tmp_obj(volatile VALUE *store);
#define delete_tmp_ary(x) delete_tmp_obj(x)

/*
 * http://masamitsu-murase.blogspot.jp/2013/12/sevenzipruby-2-c-ruby.html
 */
extern "C" VALUE rxu_run_functor(VALUE p);


#define RXU_PROTECT_FUNC(func) ((VALUE (*)(VALUE))(func))

template<typename T>
static inline
VALUE _rb_protect(T func, int *state)
{
    typedef std::function<VALUE ()> func_type;
    func_type f = func;
 
    return rb_protect(rxu_run_functor, reinterpret_cast<VALUE>(&f), state);
}

template<typename T1, typename T2>
static inline VALUE
_rb_rescue(T1 func1, T2 func2)
{
    typedef std::function<VALUE ()> func_type;
    func_type f1 = func1;
    func_type f2 = func2;

    return rb_rescue(
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2));
}

#if defined(_MSC_VER) && _MSC_VER <= 1800
    template<VALUE& e1, typename T1, typename T2>
    static inline VALUE
    _rb_rescue2(T1 func1, T2 func2)
    {
        typedef std::function<VALUE ()> func_type;
        func_type f1 = func1;
        func_type f2 = func2;
        return rb_rescue2(
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2), e1, NULL);
    }

    template<VALUE& e1, VALUE& e2, typename T1, typename T2>
    static inline VALUE
    _rb_rescue2(T1 func1, T2 func2)
    {
        typedef std::function<VALUE ()> func_type;
        func_type f1 = func1;
        func_type f2 = func2;
        return rb_rescue2(
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2), e1, e2, NULL);
    }

    template<VALUE& e1, VALUE& e2, VALUE& e3, typename T1, typename T2>
    static inline VALUE
    _rb_rescue2(T1 func1, T2 func2)
    {
        typedef std::function<VALUE ()> func_type;
        func_type f1 = func1;
        func_type f2 = func2;
        return rb_rescue2(
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2), e1, e2, e3, NULL);
    }

    template<VALUE& e1, VALUE& e2, VALUE& e3, VALUE& e4, typename T1, typename T2>
    static inline VALUE
    _rb_rescue2(T1 func1, T2 func2)
    {
        typedef std::function<VALUE ()> func_type;
        func_type f1 = func1;
        func_type f2 = func2;
        return rb_rescue2(
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2), e1, e2, e3, e4, NULL);
    }

    template<VALUE& e1, VALUE& e2, VALUE& e3, VALUE& e4, VALUE& e5, typename T1, typename T2>
    static inline VALUE
    _rb_rescue2(T1 func1, T2 func2)
    {
        typedef std::function<VALUE ()> func_type;
        func_type f1 = func1;
        func_type f2 = func2;
        return rb_rescue2(
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2), e1, e2, e3, e4, e5, NULL);
    }

#else
    template<VALUE&... exceptions, typename T1, typename T2>
    static inline VALUE
    _rb_rescue2(T1 func1, T2 func2)
    {
        typedef std::function<VALUE ()> func_type;
        func_type f1 = func1;
        func_type f2 = func2;
        return rb_rescue2(
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
            RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2), exceptions..., NULL);
    }
#endif

template<typename T1, typename T2>
static inline VALUE
_rb_rescue2(T1 func1, T2 func2, VALUE e1)
{
    typedef std::function<VALUE ()> func_type;
    func_type f1 = func1;
    func_type f2 = func2;
    return rb_rescue2(
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2), e1, NULL);
}

template<typename T1, typename T2>
static inline VALUE
_rb_rescue2(T1 func1, T2 func2, VALUE e1, VALUE e2)
{
    typedef std::function<VALUE ()> func_type;
    func_type f1 = func1;
    func_type f2 = func2;
    return rb_rescue2(
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2), e1, e2, NULL);
}

template<typename T1, typename T2>
static inline VALUE
_rb_rescue2(T1 func1, T2 func2, VALUE e1, VALUE e2, VALUE e3)
{
    typedef std::function<VALUE ()> func_type;
    func_type f1 = func1;
    func_type f2 = func2;
    return rb_rescue2(
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2), e1, e2, e3, NULL);
}

template<typename T1, typename T2>
static inline VALUE
_rb_rescue2(T1 func1, T2 func2, VALUE e1, VALUE e2, VALUE e3, VALUE e4)
{
    typedef std::function<VALUE ()> func_type;
    func_type f1 = func1;
    func_type f2 = func2;
    return rb_rescue2(
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2), e1, e2, e3, e4, NULL);
}

template<typename T1, typename T2>
static inline VALUE
_rb_rescue2(T1 func1, T2 func2, VALUE e1, VALUE e2, VALUE e3, VALUE e4, VALUE e5)
{
    typedef std::function<VALUE ()> func_type;
    func_type f1 = func1;
    func_type f2 = func2;
    return rb_rescue2(
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2), e1, e2, e3, e4, e5, NULL);
}

template<typename T1, typename T2>
static inline VALUE
_rb_ensure(T1 func1, T2 func2)
{
    typedef std::function<VALUE ()> func_type;
    func_type f1 = func1;
    func_type f2 = func2;

    return rb_ensure(
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f1), 
        RUBY_METHOD_FUNC(rxu_run_functor), reinterpret_cast<VALUE>(&f2));
}

extern "C" int rxu_run_functor_foreach(VALUE key, VALUE val, VALUE p);

#define RXU_FOREACH_FUNC(func) ((int (*)(ANYARGS))(func))

template<typename T>
static inline void
_rb_hash_foreach(VALUE obj, T func)
{
    typedef std::function<int (VALUE, VALUE)> func_type;
    func_type f = func;
    rb_hash_foreach(obj, RXU_FOREACH_FUNC(rxu_run_functor_foreach), reinterpret_cast<VALUE>(&f));
}

#endif /* RUBY_EXT_UTILS_HPP_ */
