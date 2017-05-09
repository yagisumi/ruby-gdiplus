/*
 * gdip_image.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tImage = _MAKE_DATA_TYPE(
    "Image", 0, RUBY_NEVER_FREE, NULL, NULL, &cImage);



/*
Document-class: Gdiplus::Image
abstruct class
*/
void Init_image()
{
    cImage = rb_define_class_under(mGdiplus, "Image", rb_cObject);
    rb_undef_alloc_func(cImage);
}