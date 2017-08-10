#include "ruby_ext_utils.hpp"
#include "ruby_compatible.h"

void
delete_tmp_obj(volatile VALUE *store)
{
    VALUE s = *store;
    *store = 0;
    if (RB_TYPE_P(s, RUBY_T_DATA)) {
        void *ptr = DATA_PTR(s);
        (RDATA(s)->dfree)(ptr);
        DATA_PTR(s) = ((void*)0);
    }
}

extern "C" 
VALUE 
rxu_run_functor(VALUE p)
{
    typedef std::function<VALUE ()> func_type;
    func_type *t = reinterpret_cast<func_type*>(p);
    return (*t)();
}

extern "C"
int
rxu_run_functor_foreach(VALUE key, VALUE val, VALUE p)
{
    typedef std::function<int (VALUE, VALUE)> func_type;
    func_type *t = reinterpret_cast<func_type*>(p);
    return (*t)(key, val);
}
