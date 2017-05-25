/*
 * gdip_image.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"
#include "simplemap.h"


static StrSortedArrayMap<GUID *> ext_clsid_table(11);
static MemPtrSortedArrayMap<GUID *, CLSID *> imgfmt_clsid_table(5);

enum Extname {
    ExtBmp,
    ExtJpeg,
    ExtGif,
    ExtTiff,
    ExtPng
};

static CLSID clsid_ary[5] = {
    {0x557cf400, 0x1a04, 0x11d3, {0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}},
    {0x557cf401, 0x1a04, 0x11d3, {0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}},
    {0x557cf402, 0x1a04, 0x11d3, {0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}},
    {0x557cf405, 0x1a04, 0x11d3, {0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}},
    {0x557cf406, 0x1a04, 0x11d3, {0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}}
};

static GUID imgfmt_ary[5] = {
    {0xb96b3cab, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}}, 
    {0xb96b3cae, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}}, 
    {0xb96b3cb0, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}}, 
    {0xb96b3cb1, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}}, 
    {0xb96b3caf, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}}
};

struct ext_clsid {
    const char *str;
    CLSID *guid;
};

static ext_clsid ext_clsid_map[] = {
    {".bmp", &clsid_ary[ExtBmp]},
    {".dib", &clsid_ary[ExtBmp]},
    {".rle", &clsid_ary[ExtBmp]},
    {".jpg", &clsid_ary[ExtJpeg]},
    {".jpeg", &clsid_ary[ExtJpeg]},
    {".jpe", &clsid_ary[ExtJpeg]},
    {".jfif", &clsid_ary[ExtJpeg]},
    {".gif", &clsid_ary[ExtGif]},
    {".tif", &clsid_ary[ExtTiff]},
    {".tiff", &clsid_ary[ExtTiff]},
    {".png", &clsid_ary[ExtPng]}
};

static void
ext_clsid_table_init()
{
    for (unsigned int i = 0; i < (sizeof(ext_clsid_map) / sizeof(ext_clsid)); ++i) {
        ext_clsid_table.append(ext_clsid_map[i].str, ext_clsid_map[i].guid);
    }
    for (int i = 0; i < 5; ++i) {
        imgfmt_clsid_table.append(&imgfmt_ary[i], &clsid_ary[i]);
    }
}

const rb_data_type_t tImage = _MAKE_DATA_TYPE(
    "Image", 0, RUBY_NEVER_FREE, NULL, NULL, &cImage);

/**
 *
 * @overload save(filename)
 *   @param filename [String]
 *   @return [self]
 * @overload save(filename, imgfmt)
 *   @param filename [String]
 *   @param imgfmt [ImageFormat] Identifies a file format. (Bmp, Jpeg, Gif, Tiff or Png)
 * @overload save(filename, icinfo)
 *   @param filename [String]
 *   @param icinfo [ImageCodecInfo]
 * @overload save(filename, params)
 *   @param filename [String]
 *   @param params [EncoderParameters]
 * @overload save(filename, imgfmt, params)
 *   @param filename [String]
 *   @param imgfmt [ImageFormat] Identifies a file format. (Bmp, Jpeg, Gif, Tiff or Png)
 *   @param params [EncoderParameters]
 * @overload save(filename, icinfo, params)
 *   @param filename [String]
 *   @param icinfo [ImageCodecInfo]
 *   @param params [EncoderParameters]
 */
static VALUE
gdip_image_save(int argc, VALUE *argv, VALUE self)
{
    Image *image = Data_Ptr<Image *>(self);
    GpStatus status;
    Check_NULL(image, "This image object does not exist.");

    if (argc == 0) {
        rb_raise(rb_eArgError, "too few arguments");
    }
    else if (argc > 3) {
        rb_raise(rb_eArgError, "too many arguments");
    }
    if (!_RB_STRING_P(argv[0])) { // or File
        rb_raise(rb_eTypeError, "a first argument must be String");
    }
    VALUE wstr = util_utf16_str_new(argv[0]);
    if (argc == 1) {
        char ext[6];
        CLSID *clsid;
        if (util_extname(argv[0], ext) && ext_clsid_table.get(ext, clsid)) {
            status = image->Save(RString_Ptr<WCHAR *>(wstr), clsid, NULL);
            Check_Status(status);
        }
        else {
            rb_raise(rb_eArgError, "failed to get an image format from a filename");
        }
    }
    else if (argc == 2) {
        if (_KIND_OF(argv[1], &tGuid)) { // rb_obj_is_kind_of(argv[1], cImageFormat)
            CLSID *clsid;
            if (imgfmt_clsid_table.get(Data_Ptr<GUID *>(argv[1]), clsid)) {
                status = image->Save(RString_Ptr<WCHAR *>(wstr), clsid, NULL);
                Check_Status(status);
            }
            else {
                rb_raise(rb_eArgError, "failed to get an image format from a ImageFormat");
            }
        }
        else if (_KIND_OF(argv[1], &tImageCodecInfo)) {
            ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(argv[1]);
            status = image->Save(RString_Ptr<WCHAR *>(wstr), &icinfo->Clsid, NULL);
            Check_Status(status);
        }
        else if (_KIND_OF(argv[1], &tEncoderParameters)) {
            char ext[6];
            CLSID *clsid;
            if (util_extname(argv[0], ext) && ext_clsid_table.get(ext, clsid)) {
                EncoderParameters *params = gdip_encprms_build_struct(argv[1]);
                status = image->Save(RString_Ptr<WCHAR *>(wstr), clsid, params);
                Check_Status(status);
            }
            else {
                rb_raise(rb_eArgError, "failed to get a image format from a filename");
            }
        }
        else {
            rb_raise(rb_eTypeError, "unexpected type");
        }
    }
    else if (argc == 3) {
        if (_KIND_OF(argv[1], &tGuid) && _KIND_OF(argv[2], &tEncoderParameters)) {
            CLSID *clsid;
            if (imgfmt_clsid_table.get(Data_Ptr<GUID *>(argv[1]), clsid)) {
                status = image->Save(RString_Ptr<WCHAR *>(wstr), clsid, NULL);
                Check_Status(status);
            }
            else {
                rb_raise(rb_eArgError, "failed to get a image format from a ImageFormat");
            }
            EncoderParameters *params = gdip_encprms_build_struct(argv[2]);
            status = image->Save(RString_Ptr<WCHAR *>(wstr), clsid, params);
            Check_Status(status);
        }
        else if (_KIND_OF(argv[1], &tImageCodecInfo) && _KIND_OF(argv[2], &tEncoderParameters)) {
            ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(argv[1]);
            EncoderParameters *params = gdip_encprms_build_struct(argv[2]);
            status = image->Save(RString_Ptr<WCHAR *>(wstr), &icinfo->Clsid, params);
            Check_Status(status);
        }
        else {
            rb_raise(rb_eTypeError, "unexpected type");
        }
    }
    RB_GC_GUARD(wstr);
    return self;
}

/**
 *
 * @return [Integer]
 * 
 */
static VALUE
gdip_image_width(VALUE self)
{
    Image *image = Data_Ptr<Image *>(self);
    Check_NULL(image, "The image object does not exist.");
    return RB_UINT2NUM(image->GetWidth());
}

/**
 *
 * @return [Integer]
 * 
 */
static VALUE
gdip_image_height(VALUE self)
{
    Image *image = Data_Ptr<Image *>(self);
    Check_NULL(image, "The image object does not exist.");
    return RB_UINT2NUM(image->GetHeight());
}

/**
 *
 * @return [Float]
 * 
 */
static VALUE
gdip_image_horizontal_resolution(VALUE self)
{
    Image *image = Data_Ptr<Image *>(self);
    Check_NULL(image, "The image object does not exist.");
    return SINGLE2NUM(image->GetHorizontalResolution());
}

/**
 *
 * @return [Float]
 * 
 */
static VALUE
gdip_image_vertical_resolution(VALUE self)
{
    Image *image = Data_Ptr<Image *>(self);
    Check_NULL(image, "The image object does not exist.");
    return SINGLE2NUM(image->GetVerticalResolution());
}

/**
 *
 * @return [PixelFormat]
 * 
 */
static VALUE
gdip_image_pixel_format(VALUE self)
{
    Image *image = Data_Ptr<Image *>(self);
    Check_NULL(image, "The image object does not exist.");
    PixelFormat format = image->GetPixelFormat();
    return gdip_enumint_create(cPixelFormat, format);
}

/**
 *
 * @return [PixelFormat]
 * 
 */
static VALUE
gdip_image_raw_format(VALUE self)
{
    Image *image = Data_Ptr<Image *>(self);
    Check_NULL(image, "The image object does not exist.");
    GUID guid;
    PixelFormat format = image->GetRawFormat(&guid);
    VALUE r = gdip_enum_get(cImageFormat, &guid);
    if (RB_NIL_P(r)) {
        r = typeddata_alloc<GUID, &tGuid>(cImageFormat);
    }
    return r;
}
/*
Document-class: Gdiplus::Image
abstruct class
*/
void Init_image()
{
    ext_clsid_table_init();

    cImage = rb_define_class_under(mGdiplus, "Image", rb_cObject);
    rb_undef_alloc_func(cImage);
    rb_define_method(cImage, "save", RUBY_METHOD_FUNC(gdip_image_save), -1);
    rb_define_method(cImage, "Width", RUBY_METHOD_FUNC(gdip_image_width), 0);
    rb_define_method(cImage, "width", RUBY_METHOD_FUNC(gdip_image_width), 0);
    rb_define_method(cImage, "Height", RUBY_METHOD_FUNC(gdip_image_height), 0);
    rb_define_method(cImage, "height", RUBY_METHOD_FUNC(gdip_image_height), 0);
    rb_define_method(cImage, "HorizontalResolution", RUBY_METHOD_FUNC(gdip_image_horizontal_resolution), 0);
    rb_define_method(cImage, "horizontal_resolution", RUBY_METHOD_FUNC(gdip_image_horizontal_resolution), 0);
    rb_define_method(cImage, "VerticalResolution", RUBY_METHOD_FUNC(gdip_image_vertical_resolution), 0);
    rb_define_method(cImage, "vertical_resolution", RUBY_METHOD_FUNC(gdip_image_vertical_resolution), 0);
    rb_define_method(cImage, "PixelFormat", RUBY_METHOD_FUNC(gdip_image_pixel_format), 0);
    rb_define_method(cImage, "pixel_format", RUBY_METHOD_FUNC(gdip_image_pixel_format), 0);
    rb_define_method(cImage, "RawFormat", RUBY_METHOD_FUNC(gdip_image_raw_format), 0);
    rb_define_method(cImage, "raw_format", RUBY_METHOD_FUNC(gdip_image_raw_format), 0);
    
}
