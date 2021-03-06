/*
 * gdip_bitmap.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tBitmap = _MAKE_DATA_TYPE(
    "Bitmap", 0, GDIP_OBJ_FREE(Bitmap *), NULL, &tImage, &cBitmap);

static Bitmap *
gdip_bitmap_init_from_file(VALUE filename, BOOL use_ecm=FALSE)
{
    VALUE wstr = util_utf16_str_new(filename);
    Bitmap *bmp = gdip_obj_create<Bitmap *>(new Bitmap(RString_Ptr<WCHAR *>(wstr), use_ecm));
    RB_GC_GUARD(wstr);
    return bmp;
}

static Bitmap *
gdip_bitmap_init_by_size(VALUE width, VALUE height, VALUE format=Qnil)
{
    int w = RB_NUM2INT(width);
    int h = RB_NUM2INT(height);
    PixelFormat fmt = PixelFormat32bppARGB;
    if (!RB_NIL_P(format)) {
        gdip_arg_to_enumint(cPixelFormat, format, &fmt, "The third argument should be PixelFormat.");
    }
    return gdip_obj_create(new Bitmap(w, h, fmt));
}

static VALUE
gdip_bitmap_create_by_size(VALUE width, VALUE height, VALUE format=Qnil)
{
    VALUE bmp = rb_obj_alloc(cBitmap);
    _DATA_PTR(bmp) = gdip_bitmap_init_by_size(width, height, format);
    return bmp;
}

/*
 * @overload initialize(filename, use_ecm=false)
 *   @param filename [String] File name.
 *   @param use_ecm [Boolean] whether to use embedded color management.
 * @overload initialize(width, height, format=PixelFormat::Format32bppARGB)
 *   @param width [Integer]
 *   @param height [Integer]
 *   @param format [PixelFormat]
 */
static VALUE
gdip_bitmap_init(int argc, VALUE *argv, VALUE self)
{
    Bitmap *bmp = Data_Ptr<Bitmap *>(self);
    if (bmp != NULL) {
        _VERBOSE("Gdiplus::Bitmap already initialized");
        return self;
    }

    if (argc == 0) {
        rb_raise(rb_eArgError, "too few arguments");
    }
    else if (argc > 3) {
        rb_raise(rb_eArgError, "too many arguments");
    }
    else if (argc == 1) {
        if (RB_TYPE_P(argv[0], RUBY_T_STRING)) {
            _DATA_PTR(self) = gdip_bitmap_init_from_file(argv[0]);
        }
        else if (RB_TYPE_P(argv[0], RUBY_T_FILE)) {
            NOT_IMPLEMENTED_ERROR;
        }
        else {
            rb_raise(rb_eArgError, "wrong arguments");
        }
    }
    else if (argc == 2) {
        if (RB_TYPE_P(argv[0], RUBY_T_STRING)) {
            _DATA_PTR(self) = gdip_bitmap_init_from_file(argv[0], RB_TEST(argv[1]));
        }
        else if (RB_TYPE_P(argv[0], RUBY_T_FILE)) {
            NOT_IMPLEMENTED_ERROR;
        }
        else if (Integer_p(argv[0], argv[1])) {
            _DATA_PTR(self) = gdip_bitmap_init_by_size(argv[0], argv[1]);
        }
        else {
            rb_raise(rb_eArgError, "wrong arguments");
        }
    }
    if (argc == 3) {
        _DATA_PTR(self) = gdip_bitmap_init_by_size(argv[0], argv[1], argv[2]);
    }

    return self;
}


void Init_bitmap()
{
    cBitmap = rb_define_class_under(mGdiplus, "Bitmap", cImage);
    rb_define_alloc_func(cBitmap, &typeddata_alloc_null<&tBitmap>);
    rb_define_method(cBitmap, "initialize", RUBY_METHOD_FUNC(gdip_bitmap_init), -1);
}