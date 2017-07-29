/*
 * gdip_image_attrs.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tImageAttributes = _MAKE_DATA_TYPE(
    "ImageAttributes", 0, GDIP_OBJ_FREE(ImageAttributes *), NULL, NULL, &cImageAttributes);

static VALUE cColorMatrix;
static const rb_data_type_t tColorMatrix = _MAKE_DATA_TYPE(
    "ColorMatrix", 0, GDIP_DEFAULT_FREE(ColorMatrix *), NULL, NULL, &cColorMatrix);

static VALUE
gdip_colormatrix_alloc(VALUE klass)
{
    dp("<%s> alloc", type_name<ColorMatrix *>());
    ColorMatrix *ptr = static_cast<ColorMatrix *>(RB_ZALLOC(ColorMatrix));
    for (int row = 0; row < 5; ++row) {
        for (int column = 0; column < 5; ++ column) {
            ptr->m[row][column] = row == column ? 1.0f : 0.0f;
        }
    }
    VALUE r = _Data_Wrap_Struct(klass, &tColorMatrix, ptr);
    return r;
}

/**
 * @overload initialize()
 * @overload initialize(matrix)
 *   @param matrix [Array<Float>] The number of elements should be 25. (5x5 matrix)
 * @overload initialize(*floats)
 *   @param floats [Float] The number of elements should be 25.
 * @example
 *   kr = 0.299
 *   kg = 0.587
 *   kb = 0.114
 *   cmatrix = ColorMatrix.new([
 *     kr, kr, kr, 0.0, 0.0,
 *     kg, kg, kg, 0.0, 0.0,
 *     kb, kb, kb, 0.0, 0.0,
 *     0.0, 0.0, 0.0, 1.0, 0.0,
 *     0.0, 0.0, 0.0, 0.0, 1.0,
 *   ])
 */
static VALUE
gdip_colormatrix_init(int argc, VALUE *argv, VALUE self)
{
    float tmp[25];

    if (argc == 0) { return self; }
    else if (argc == 1) {
        if (_RB_ARRAY_P(argv[0])) {
            if (RARRAY_LEN(argv[0]) == 25) {
                for (int i = 0; i < 25; ++i) {
                    VALUE e = rb_ary_entry(argv[0], i);
                    gdip_arg_to_single(e, &tmp[i], "The argument should be Array of Float, the number of elements should be 25.");
                }
            }
            else {
                rb_raise(rb_eTypeError, "The argument should be Array of Float, the number of elements should be 25.");
            }
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Array of Float, the number of elements should be 25.");
        }
    }
    else if (argc == 25) {
        for (int i = 0; i < 25; ++i) {
            gdip_arg_to_single(argv[i], &tmp[i], "The argument should be Array of Float, the number of elements should be 25.");
        }
    }
    else {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1, 25)", argc);
    }

    ColorMatrix *cmatrix = Data_Ptr<ColorMatrix *>(self);
    for (int row = 0; row < 5; ++row) {
        for (int column = 0; column < 5; ++ column) {
            cmatrix->m[row][column] = tmp[row * 5 + column];
        }
    }

    return self;
}

/**
 * @overload [](row, column)
 *   @param row [Integer] 0..4
 *   @param column [Integer] 0..4
 * @example
 *   p cmatrix[1, 1]
 */
static VALUE
gdip_colormatrix_aref(VALUE self, VALUE v_row, VALUE v_column)
{
    if (!Integer_p(v_row, v_column)) {
        rb_raise(rb_eTypeError, "The arguments should be Integer.");
    }

    int row = RB_NUM2INT(v_row);
    int column = RB_NUM2INT(v_column);

    if (row < 0 || 4 < row) {
        rb_raise(rb_eRangeError, "row index out of range (%d for 0..4)", row);
    }
    if (column < 0 || 4 < column) {
        rb_raise(rb_eRangeError, "column index out of range (%d for 0..4)", column);
    }

    ColorMatrix *cmatrix = Data_Ptr<ColorMatrix *>(self);
    
    return SINGLE2NUM(cmatrix->m[row][column]);
}

/**
 * @overload []=(row, column, value)
 *   @param row [Integer] 0..4
 *   @param column [Integer] 0..4
 *   @param value [Float]
 * @example
 *   cmatrix[1, 1]
 */
static VALUE
gdip_colormatrix_aset(VALUE self, VALUE v_row, VALUE v_column, VALUE value)
{
    if (!Integer_p(v_row, v_column)) {
        rb_raise(rb_eTypeError, "The arguments should be Integer.");
    }

    int row = RB_NUM2INT(v_row);
    int column = RB_NUM2INT(v_column);

    if (row < 0 || 4 < row) {
        rb_raise(rb_eRangeError, "row index out of range (%d for 0..4)", row);
    }
    if (column < 0 || 4 < column) {
        rb_raise(rb_eRangeError, "column index out of range (%d for 0..4)", column);
    }

    float num = 0.0f;
    gdip_arg_to_single(value, &num, "The argument should be Float.");

    ColorMatrix *cmatrix = Data_Ptr<ColorMatrix *>(self);
    cmatrix->m[row][column] = num;

    return self;
}


static VALUE
gdip_imgattrs_init(VALUE self)
{
    if (_DATA_PTR(self) != NULL) {
        _VERBOSE("This ImageAttributes object is already initialized.");
        return self;
    }

    _DATA_PTR(self) = gdip_obj_create(new ImageAttributes());

    return self;
}

static VALUE
gdip_imgattrs_clone(VALUE self)
{
    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    VALUE r = typeddata_alloc_null<&tImageAttributes>(cImageAttributes);
    _DATA_PTR(r) = gdip_obj_create(attrs->Clone());

    return r;
}

/**
 * @overload SetColorMatrix(color_matrix, mode=ColorMatrixFlag.Default, type=ColorAdjustType.Default)
 *   @param color_matrix [ColorMatrix]
 *   @param mode [ColorMatrixFlag]
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_set_color_matrix(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    VALUE v_matrix, v_mode, v_type;
    rb_scan_args(argc, argv, "12", &v_matrix, &v_mode, &v_type);

    if (!_KIND_OF(v_matrix, &tColorMatrix)) {
        rb_raise(rb_eTypeError, "The first argument should be ColorMatrix.");
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorMatrix *matrix = Data_Ptr<ColorMatrix *>(v_matrix);
    ColorMatrixFlags mode = ColorMatrixFlagsDefault;
    ColorAdjustType type = ColorAdjustTypeDefault;

    if (!RB_NIL_P(v_mode)) {
        gdip_arg_to_enumint(cColorMatrixFlag, v_mode, &mode, "The second argument should be ColorMatrixFlag.");
    }

    if (!RB_NIL_P(v_type)) {
        gdip_arg_to_enumint(cColorAdjustType, v_type, &type, "The third argument should be ColorAdjustType.");
    }

    Status status = attrs->SetColorMatrix(matrix, mode, type);
    Check_Status(status);

    return self;
}

/**
 * @overload ClearColorMatrix(type=ColorAdjustType.Default)
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_clear_color_matrix(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc > 1) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 1) {
        gdip_arg_to_enumint(cColorAdjustType, argv[0], &type, "The argument should be ColorAdjustType.");
    }
    
    Status status = attrs->ClearColorMatrix(type);
    Check_Status(status);

    return self;
}

/**
 * @overload SetColorMatrices(color_matrix, gray_matrix, mode=ColorMatrixFlag.Default, type=ColorAdjustType.Default)
 *   @param color_matrix [ColorMatrix]
 *   @param gray_matrix [ColorMatrix]
 *   @param mode [ColorMatrixFlag]
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_set_color_matrices(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    VALUE v_cmatrix, v_gmatrix, v_mode, v_type;
    rb_scan_args(argc, argv, "22", &v_cmatrix, &v_gmatrix, &v_mode, &v_type);

    if (!_KIND_OF(v_cmatrix, &tColorMatrix)) {
        rb_raise(rb_eTypeError, "The first argument should be ColorMatrix.");
    }

    if (!_KIND_OF(v_gmatrix, &tColorMatrix)) {
        rb_raise(rb_eTypeError, "The second argument should be ColorMatrix.");
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorMatrix *cmatrix = Data_Ptr<ColorMatrix *>(v_cmatrix);
    ColorMatrix *gmatrix = Data_Ptr<ColorMatrix *>(v_gmatrix);
    ColorMatrixFlags mode = ColorMatrixFlagsDefault;
    ColorAdjustType type = ColorAdjustTypeDefault;

    if (!RB_NIL_P(v_mode)) {
        gdip_arg_to_enumint(cColorMatrixFlag, v_mode, &mode, "The third argument should be ColorMatrixFlag.");
    }

    if (!RB_NIL_P(v_type)) {
        gdip_arg_to_enumint(cColorAdjustType, v_type, &type, "The fourth argument should be ColorAdjustType.");
    }

    Status status = attrs->SetColorMatrices(cmatrix, gmatrix, mode, type);
    Check_Status(status);

    return self;
}

/**
 * @overload SetThreshold(threshold, type=ColorAdjustType.Default)
 *   @param threshold [Float]
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_set_threshold(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    float threshold = 1.0f;
    ColorAdjustType type = ColorAdjustTypeDefault;
    
    gdip_arg_to_single(argv[0], &threshold, "The first argument should be Float.");
    threshold = clamp(threshold, 0.0f, 1.0f);

    if (argc == 2) {
        gdip_arg_to_enumint(cColorAdjustType, argv[1], &type, "The second argument should be ColorAdjustType.");
    }

    Status status = attrs->SetThreshold(threshold, type);
    Check_Status(status);

    return self;
}

/**
 * @overload ClearThreshold(type=ColorAdjustType.Default)
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_clear_threshold(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc > 1) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 1) {
        gdip_arg_to_enumint(cColorAdjustType, argv[0], &type, "The argument should be ColorAdjustType.");
    }
    
    Status status = attrs->ClearThreshold(type);
    Check_Status(status);

    return self;
}

/**
 * @overload SetGamma(gamma, type=ColorAdjustType.Default)
 *   @param gamma [Float]
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_set_gamma(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    float gamma = 1.0f;
    ColorAdjustType type = ColorAdjustTypeDefault;
    
    gdip_arg_to_single(argv[0], &gamma, "The first argument should be Float.");

    if (argc == 2) {
        gdip_arg_to_enumint(cColorAdjustType, argv[1], &type, "The second argument should be ColorAdjustType.");
    }

    Status status = attrs->SetGamma(gamma, type);
    Check_Status(status);

    return self;
}

/**
 * @overload ClearGamma(type=ColorAdjustType.Default)
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_clear_gamma(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc > 1) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 1) {
        gdip_arg_to_enumint(cColorAdjustType, argv[0], &type, "The argument should be ColorAdjustType.");
    }
    
    Status status = attrs->ClearGamma(type);
    Check_Status(status);

    return self;
}

/**
 * @overload SetNoOp(type=ColorAdjustType.Default)
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_set_no_op(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc > 1) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 1) {
        gdip_arg_to_enumint(cColorAdjustType, argv[0], &type, "The argument should be ColorAdjustType.");
    }
    
    Status status = attrs->SetNoOp(type);
    Check_Status(status);

    return self;
}

/**
 * @overload ClearNoOp(type=ColorAdjustType.Default)
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_clear_no_op(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc > 1) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 1) {
        gdip_arg_to_enumint(cColorAdjustType, argv[0], &type, "The argument should be ColorAdjustType.");
    }
    
    Status status = attrs->ClearNoOp(type);
    Check_Status(status);

    return self;
}

/**
 * @overload SetColorKey(color_low, color_high, type=ColorAdjustType.Default)
 *   @param color_low [Color]
 *   @param color_high [Color]
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_set_color_key(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc != 2 && argc != 3) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2..3)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    Color color_low;
    Color color_high;
    gdip_arg_to_color(argv[0], &color_low, "The first argument should be Color.");
    gdip_arg_to_color(argv[1], &color_high, "The second argument should be Color.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 3) {
        gdip_arg_to_enumint(cColorAdjustType, argv[2], &type, "The third argument should be ColorAdjustType.");
    }

    Status status = attrs->SetColorKey(color_low, color_high, type);
    Check_Status(status);

    return self;
}
/**
 * @overload ClearColorKey(type=ColorAdjustType.Default)
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_clear_color_key(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc > 1) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 1) {
        gdip_arg_to_enumint(cColorAdjustType, argv[0], &type, "The argument should be ColorAdjustType.");
    }
    
    Status status = attrs->ClearColorKey(type);
    Check_Status(status);

    return self;
}

/**
 * @overload SetOutputChannel(flags, type=ColorAdjustType.Default)
 *   @param flags [ColorChannelFlag]
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_set_output_channel(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorChannelFlags flags = ColorChannelFlagsC;
    gdip_arg_to_enumint(cColorChannelFlag, argv[0], &flags, "The first argument should be ColorChannelFlags.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 2) {
        gdip_arg_to_enumint(cColorAdjustType, argv[1], &type, "The second argument should be ColorAdjustType.");
    }

    Status status = attrs->SetOutputChannel(flags, type);
    Check_Status(status);

    return self;
}

/**
 * @overload ClearOutputChannel(type=ColorAdjustType.Default)
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_clear_output_channel(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc > 1) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 1) {
        gdip_arg_to_enumint(cColorAdjustType, argv[0], &type, "The argument should be ColorAdjustType.");
    }
    
    Status status = attrs->ClearOutputChannel(type);
    Check_Status(status);

    return self;
}

/**
 * @overload SetOutputChannelColorProfile(filename, type=ColorAdjustType.Default)
 *   @param filename [String]
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_set_output_channel_color_profile(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }

    if (!_RB_STRING_P(argv[0])) {
        rb_raise(rb_eTypeError, "The first argument should be String.");
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    VALUE wstr = util_utf16_str_new(argv[0]);

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 2) {
        gdip_arg_to_enumint(cColorAdjustType, argv[1], &type, "The seconde argument should be ColorAdjustType.");
    }

    attrs->SetOutputChannelColorProfile(RString_Ptr<WCHAR *>(wstr), type);
    RB_GC_GUARD(wstr);

    return self;
}

/**
 * @overload ClearOutputChannelColorProfile(type=ColorAdjustType.Default)
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
clear_output_channel_color_profile(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc > 1) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 1) {
        gdip_arg_to_enumint(cColorAdjustType, argv[0], &type, "The argument should be ColorAdjustType.");
    }
    
    Status status = attrs->ClearOutputChannelColorProfile(type);
    Check_Status(status);

    return self;
}

class GdipColorMaps {
public:
    ColorMap *map;
    int count;

    GdipColorMaps(int len) {
        size = len;
        count = 0;
        map = static_cast<ColorMap *>(RB_ZALLOC_N(ColorMap, len));
    }

    ~GdipColorMaps() {
        ruby_xfree(map);
    }

    bool add_map(Color& old_color, Color& new_color) {
        if (count >= size) return false;

        map[count].oldColor = old_color;
        map[count].newColor = new_color;
        count += 1;

        return true;
    }
private:
    int size;
};

/**
 * @overload SetRemapTable(color_map, type=ColorAdjustType.Default)
 *   @param color_map [Hash]
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_set_remap_table(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }

    if (!_RB_HASH_P(argv[0])) {
        rb_raise(rb_eTypeError, "The first argument should be Hash.");
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 2) {
        gdip_arg_to_enumint(cColorAdjustType, argv[1], &type, "The second argument should be ColorAdjustType.");
    }

    int size = RHASH_SIZE(argv[0]);
    if (size <= 0) {
        rb_raise(rb_eTypeError, "The key and value of Hash should be Color.");
    }
    GdipColorMaps *cmaps = new GdipColorMaps(size);
    VALUE v_cmaps = wrap_tmp_obj(cmaps);
    
    _rb_hash_foreach(argv[0], [&cmaps](VALUE key, VALUE val) -> int {
        Color old_color;
        Color new_color;
        if (gdip_arg_to_color(key, &old_color, NULL, ArgOptionAcceptInt) &&
            gdip_arg_to_color(val, &new_color, NULL, ArgOptionAcceptInt)) {
                bool success = cmaps->add_map(old_color, new_color);
                if (!success) return ST_STOP;
        }
        return ST_CONTINUE;
    });

    if (cmaps->count == 0) {
        rb_raise(rb_eTypeError, "The key and value of Hash should be Color.");
    }
    Status status = attrs->SetRemapTable(cmaps->count, cmaps->map, type);
    delete_tmp_obj(&v_cmaps);
    Check_Status(status);

    return self;
}

/**
 * @overload ClearRemapTable(type=ColorAdjustType.Default)
 *   @param type [ColorAdjustType]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_clear_remap_table(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    if (argc > 1) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    ColorAdjustType type = ColorAdjustTypeDefault;
    if (argc == 1) {
        gdip_arg_to_enumint(cColorAdjustType, argv[0], &type, "The argument should be ColorAdjustType.");
    }
    
    Status status = attrs->ClearRemapTable(type);
    Check_Status(status);

    return self;
}

/**
 * @overload SetBrushRemapTable(color_map)
 *   @param color_map [Hash]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_set_brush_remap_table(VALUE self, VALUE color_map)
{
    Check_Frozen(self);

    if (!_RB_HASH_P(color_map)) {
        rb_raise(rb_eTypeError, "The first argument should be Hash.");
    }

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    int size = RHASH_SIZE(color_map);
    GdipColorMaps *cmaps = new GdipColorMaps(size);
    VALUE v_cmaps = wrap_tmp_obj(cmaps);
    
    _rb_hash_foreach(color_map, [&cmaps](VALUE key, VALUE val) -> int {
        Color old_color;
        Color new_color;
        if (gdip_arg_to_color(key, &old_color, NULL, ArgOptionAcceptInt) &&
            gdip_arg_to_color(val, &new_color, NULL, ArgOptionAcceptInt)) {
                bool success = cmaps->add_map(old_color, new_color);
                if (!success) return ST_STOP;
        }
        return ST_CONTINUE;
    });

    if (cmaps->count == 0) {
        rb_raise(rb_eTypeError, "The key and value of Hash should be Color.");
    }

    Status status = attrs->SetBrushRemapTable(cmaps->count, cmaps->map);
    delete_tmp_obj(&v_cmaps);
    Check_Status(status);

    return self;
}

/**
 * @overload ClearBrushRemapTable()
 *   @return [self]
 */
static VALUE
gdip_imgattrs_clear_brush_remap_table(VALUE self)
{
    Check_Frozen(self);

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    Status status = attrs->ClearBrushRemapTable();
    Check_Status(status);

    return self;
}

/**
 * @overload SetWrapMode(wrap, color=Color.Black, clamp=false)
 *   @param wrap [WrapMode]
 *   @param color [Color]
 *   @param clamp [Boolean]
 *   @return [self]
 */
static VALUE
gdip_imgattrs_set_wrap_mode(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);

    VALUE v_wrap, v_color, v_clamp;
    rb_scan_args(argc, argv, "12", &v_wrap, &v_color, &v_clamp);

    ImageAttributes *attrs = Data_Ptr<ImageAttributes *>(self);
    Check_NULL(attrs, "This ImageAttributes object does not exist.");

    WrapMode wrap = WrapModeTile;
    gdip_arg_to_enumint(cWrapMode, v_wrap, &wrap, "The first argument should be WrapMode.");

    Color color;
    if (!RB_NIL_P(v_color)) {
        gdip_arg_to_color(v_color, &color);
    }

    BOOL clamp = RB_TEST(v_clamp);

    Status status = attrs->SetWrapMode(wrap, color, clamp);
    Check_Status(status);

    return self;
}

/**
 * @overload GetAdjustedPalette(type=ColorAdjustType.Default)
 *   @param palette [ColorPalette]
 *   @param type [ColorAdjustType]
 *   @return [ColorPalette]
 */
static VALUE
gdip_imgattrs_get_adjusted_palette(int argc, VALUE *argv, VALUE self)
{
    NOT_IMPLEMENTED_ERROR;
    return Qnil;
}

void
Init_image_attrs()
{
    cColorMatrix = rb_define_class_under(mGdiplus, "ColorMatrix", rb_cObject);
    rb_define_alloc_func(cColorMatrix, gdip_colormatrix_alloc);
    rb_define_method(cColorMatrix, "initialize", RUBY_METHOD_FUNC(gdip_colormatrix_init), -1);
    rb_define_method(cColorMatrix, "[]", RUBY_METHOD_FUNC(gdip_colormatrix_aref), 2);
    rb_define_method(cColorMatrix, "[]=", RUBY_METHOD_FUNC(gdip_colormatrix_aset), 3);

    cImageAttributes = rb_define_class_under(mGdiplus, "ImageAttributes", cGpObject);
    rb_define_alloc_func(cImageAttributes, &typeddata_alloc_null<&tImageAttributes>);
    rb_define_method(cImageAttributes, "initialize", RUBY_METHOD_FUNC(gdip_imgattrs_init), 0);
    rb_define_method(cImageAttributes, "Clone", RUBY_METHOD_FUNC(gdip_imgattrs_clone), 0);
    rb_define_alias(cImageAttributes, "clone", "Clone");
    rb_define_alias(cImageAttributes, "dup", "Clone");
    rb_define_method(cImageAttributes, "SetColorMatrix", RUBY_METHOD_FUNC(gdip_imgattrs_set_color_matrix), -1);
    rb_define_alias(cImageAttributes, "set_color_matrix", "SetColorMatrix");
    rb_define_method(cImageAttributes, "ClearColorMatrix", RUBY_METHOD_FUNC(gdip_imgattrs_clear_color_matrix), -1);
    rb_define_alias(cImageAttributes, "clear_color_matrix", "ClearColorMatrix");
    rb_define_alias(cImageAttributes, "ClearColorMatrices", "ClearColorMatrix");
    rb_define_alias(cImageAttributes, "clear_color_matrices", "ClearColorMatrix");
    rb_define_method(cImageAttributes, "SetColorMatrices", RUBY_METHOD_FUNC(gdip_imgattrs_set_color_matrices), -1);
    rb_define_alias(cImageAttributes, "set_color_matrices", "SetColorMatrices");
    rb_define_method(cImageAttributes, "SetThreshold", RUBY_METHOD_FUNC(gdip_imgattrs_set_threshold), -1);
    rb_define_alias(cImageAttributes, "set_threshold", "SetThreshold");
    rb_define_method(cImageAttributes, "ClearThreshold", RUBY_METHOD_FUNC(gdip_imgattrs_clear_threshold), -1);
    rb_define_alias(cImageAttributes, "clear_threshold", "ClearThreshold");
    rb_define_method(cImageAttributes, "SetGamma", RUBY_METHOD_FUNC(gdip_imgattrs_set_gamma), -1);
    rb_define_alias(cImageAttributes, "set_gamma", "SetGamma");
    rb_define_method(cImageAttributes, "ClearGamma", RUBY_METHOD_FUNC(gdip_imgattrs_clear_gamma), -1);
    rb_define_alias(cImageAttributes, "clear_gamma", "ClearGamma");
    rb_define_method(cImageAttributes, "SetNoOp", RUBY_METHOD_FUNC(gdip_imgattrs_set_no_op), -1);
    rb_define_alias(cImageAttributes, "set_no_op", "SetNoOp");
    rb_define_method(cImageAttributes, "ClearNoOp", RUBY_METHOD_FUNC(gdip_imgattrs_clear_no_op), -1);
    rb_define_alias(cImageAttributes, "clear_no_op", "ClearNoOp");
    rb_define_method(cImageAttributes, "SetColorKey", RUBY_METHOD_FUNC(gdip_imgattrs_set_color_key), -1);
    rb_define_alias(cImageAttributes, "set_color_key", "SetColorKey");
    rb_define_method(cImageAttributes, "ClearColorKey", RUBY_METHOD_FUNC(gdip_imgattrs_clear_color_key), -1);
    rb_define_alias(cImageAttributes, "clear_color_key", "ClearColorKey");
    rb_define_method(cImageAttributes, "SetOutputChannel", RUBY_METHOD_FUNC(gdip_imgattrs_set_output_channel), -1);
    rb_define_alias(cImageAttributes, "set_output_channel", "SetOutputChannel");
    rb_define_method(cImageAttributes, "ClearOutputChannel", RUBY_METHOD_FUNC(gdip_imgattrs_clear_output_channel), -1);
    rb_define_alias(cImageAttributes, "clear_output_channel", "ClearOutputChannel");
    rb_define_method(cImageAttributes, "SetOutputChannelColorProfile", RUBY_METHOD_FUNC(gdip_imgattrs_set_output_channel_color_profile), -1);
    rb_define_alias(cImageAttributes, "set_output_channel_color_profile", "SetOutputChannelColorProfile");
    rb_define_method(cImageAttributes, "ClearOutputChannelColorProfile", RUBY_METHOD_FUNC(clear_output_channel_color_profile), -1);
    rb_define_alias(cImageAttributes, "clear_output_channel_color_profile", "ClearOutputChannelColorProfile");
    rb_define_method(cImageAttributes, "SetRemapTable", RUBY_METHOD_FUNC(gdip_imgattrs_set_remap_table), -1);
    rb_define_alias(cImageAttributes, "set_remap_table", "SetRemapTable");
    rb_define_method(cImageAttributes, "ClearRemapTable", RUBY_METHOD_FUNC(gdip_imgattrs_clear_remap_table), -1);
    rb_define_alias(cImageAttributes, "clear_remap_table", "ClearRemapTable");
    rb_define_method(cImageAttributes, "SetBrushRemapTable", RUBY_METHOD_FUNC(gdip_imgattrs_set_brush_remap_table), 1);
    rb_define_alias(cImageAttributes, "set_brush_remap_table", "SetBrushRemapTable");
    rb_define_method(cImageAttributes, "ClearBrushRemapTable", RUBY_METHOD_FUNC(gdip_imgattrs_clear_brush_remap_table), 0);
    rb_define_alias(cImageAttributes, "clear_brush_remap_table", "ClearBrushRemapTable");
    rb_define_method(cImageAttributes, "SetWrapMode", RUBY_METHOD_FUNC(gdip_imgattrs_set_wrap_mode), -1);
    rb_define_alias(cImageAttributes, "set_wrap_mode", "SetWrapMode");
    rb_define_method(cImageAttributes, "GetAdjustedPalette", RUBY_METHOD_FUNC(gdip_imgattrs_get_adjusted_palette), -1);
    rb_define_alias(cImageAttributes, "get_adjusted_palette", "GetAdjustedPalette");


}