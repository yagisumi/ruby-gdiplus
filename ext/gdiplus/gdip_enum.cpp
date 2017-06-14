/*
 * gdip_enum.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
 #include "ruby_gdiplus.h"
 #include "simplemap.h"

ID ID_UNKNOWN;
ID ID_Compression;
ID ID_ColorDepth;
ID ID_ScanMethod;
ID ID_Version;
ID ID_RenderMethod;
ID ID_Quality;
ID ID_Transformation;
ID ID_LuminanceTable;
ID ID_ChrominanceTable;
ID ID_SaveFlag;
ID ID_ColorSpace;
ID ID_ImageItems;
ID ID_SaveAsCMYK;

GUID _EncoderColorSpace;
GUID _EncoderImageItems;
GUID _EncoderSaveAsCMYK;

GUID _ImageFormatEXIF;
GUID _ImageFormatUndefined;

class KlassTableMap : public SortedArrayMap<VALUE, MapBase *> {
public:
    KlassTableMap(int capa) : SortedArrayMap<VALUE, MapBase *>(capa) {}
    virtual ~KlassTableMap() {
        for (int i = 0; i < this->Len; ++i) {
            dp("KlassTableMap: delete %d", i);
            delete this->ValTable[i];
        }
    }
};

static KlassTableMap klass_table_map(30);

template <typename TKey>
static IMap<TKey, ID>*
gdip_enum_get_table(VALUE klass)
{
    MapBase *table = NULL;
    klass_table_map.get(klass, table);
    return static_cast<IMap<TKey, ID>*>(table);
}

VALUE
gdip_enum_const_get(VALUE self)
{
    return rb_const_get(self, rb_frame_this_func());
}

const rb_data_type_t tEnumInt = _MAKE_DATA_TYPE(
    "EnumInt", 0, RUBY_NEVER_FREE, NULL, NULL, &cEnumInt);

VALUE
gdip_enumint_create(VALUE klass, int num)
{
    VALUE r = _Data_Wrap_Struct(klass, &tEnumInt, reinterpret_cast<void*>(num));
    return r;
}

int
gdip_arg_to_enumint(VALUE klass, VALUE arg, int *num, int option, const char *raise_msg)
{
    if (RB_SYMBOL_P(arg) && rb_const_defined_at(klass, RB_SYM2ID(arg))) {
        arg = rb_const_get(klass, RB_SYM2ID(arg));
    }

    if (rb_obj_is_kind_of(arg, klass)) {
        *num = Data_Ptr_As<int>(arg);
        return true;
    }
    else if ((option & ArgOptionAcceptInt) && Integer_p(arg)) {
        *num = RB_NUM2INT(arg);
        return true;
    }
    else if (option & ArgOptionToInt) {
        VALUE v_num = rb_to_int(arg);
        *num = RB_NUM2INT(v_num);
        return true;
    }
    else if (raise_msg != NULL) {
        rb_raise(rb_eTypeError, raise_msg);
    }
    return false;
}


/**
 * @return [String]
 */
static VALUE
gdip_enumint_inspect(VALUE self)
{
    int num = Data_Ptr_As<int>(self);
    IMap<int, ID> *table = gdip_enum_get_table<int>(CLASS_OF(self));
    if (table != NULL) {
        ID id;
        table->get(num, id, ID_UNKNOWN);
        return util_utf8_sprintf("#<%s.%s: 0x%08x>", __class__(self), rb_id2name(id), num);
    }
    else {
        return util_utf8_sprintf("#<%s: 0x%08x>", __class__(self), num);
    }
}

/*
 * Returns a enum value.
 * @return [Integer]
 */
static VALUE
gdip_enumint_to_i(VALUE self)
{
    return RB_INT2NUM(Data_Ptr_As<int>(self));
}

/**
 * @return [Integer]
 */
static VALUE
gdip_enumint_or(VALUE self, VALUE other)
{
    VALUE v = rb_to_int(other);
    int n_other = RB_NUM2INT(v);
    int n_self = Data_Ptr_As<int>(self);
    return RB_INT2NUM(n_self | n_other);
}

/**
 * @return [Integer]
 */
static VALUE
gdip_enumint_and(VALUE self, VALUE other)
{
    VALUE v = rb_to_int(other);
    int n_other = RB_NUM2INT(v);
    int n_self = Data_Ptr_As<int>(self);
    return RB_INT2NUM(n_self & n_other);
}

static VALUE
gdip_enumint_coerce(VALUE self, VALUE other)
{
    if (RB_INTEGER_TYPE_P(other)) {
        int n_self = Data_Ptr_As<int>(self);
        self = RB_INT2NUM(n_self);
    }
    return rb_assoc_new(other, self);
}

/**
 * Compares to other.
 * @return [Boolean]
 */
static VALUE
gdip_enumint_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    if (_KIND_OF(other, &tEnumInt)) {
        int x = Data_Ptr_As<int>(self);
        int y = Data_Ptr_As<int>(other);
        return x == y ? Qtrue : Qfalse;
    }
    else if (_RB_INTEGER_P(other)) {
        int x = Data_Ptr_As<int>(self);
        int y = RB_NUM2INT(other);
        return x == y ? Qtrue : Qfalse;
    }
    else return Qfalse;
}

/**
 * @overload initialize()
 * @overload initialize(arg, ...)
 *   Performs a bitwise OR on arguments.
 *   @param arg [Integer or Symbol] Symbol is replaced by constant value.
 */
static VALUE
gdip_enumflags_init(int argc, VALUE *argv, VALUE self)
{
    unsigned int n_self = 0;
    for (int i = 0; i < argc; ++i) {
        if (Integer_p(argv[0])) {
            n_self = n_self | RB_NUM2UINT(argv[i]);
        }
        else if (RB_SYMBOL_P(argv[0])) {
            VALUE v = rb_const_get(CLASS_OF(self), RB_SYM2ID(argv[i]));
            n_self = n_self | Data_Ptr_As<unsigned int>(v);
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer or Symbol.");
        }
    }
    Data_Ptr_Set_As<unsigned int>(self, n_self);

    return self;
}

/**
 * @return [String]
 */
static VALUE
gdip_enumflags_inspect(VALUE self)
{
    unsigned int num = Data_Ptr_As<unsigned int>(self);
    IMap<unsigned int, ID> *table = gdip_enum_get_table<unsigned int>(CLASS_OF(self));
    VALUE r = Qnil;
    if (table != NULL) {
        unsigned int flags = num;
        if (flags == 0) {
            ID id;
            if (table->get(num, id)) {
                r = util_utf8_sprintf("#<%s 0x%08x: %s>", __class__(self), num, rb_id2name(id));
            }
        } else {
            VALUE symbols = rb_str_new(NULL, 0);
            for (int i = table->length() - 1; i >= 0; --i) {
                std::pair<unsigned int, ID> kv = table->get_key_value(i);
                if (kv.first && (flags & kv.first) == kv.first) {
                    if (RSTRING_LEN(symbols) > 0) {
                        rb_str_cat_cstr(symbols, " | ");
                    }
                    rb_str_cat_cstr(symbols, rb_id2name(kv.second));
                    flags = flags & ~kv.first;
                }
            }
            
            if (flags) {
                if (RSTRING_LEN(symbols) > 0) {
                    rb_str_cat_cstr(symbols, " | __UNKNOWN__");
                }
                else {
                    rb_str_cat_cstr(symbols, "__UNKNOWN__");
                }
            }

            if (RSTRING_LEN(symbols) > 0) {
                r = util_utf8_sprintf("#<%s 0x%08x: %s >", __class__(self), num, RSTRING_PTR(symbols));
            }
            RB_GC_GUARD(symbols);
        }
    }

    if (RB_NIL_P(r)) {
        r = util_utf8_sprintf("#<%s 0x%08x>", __class__(self), num);
    }
    return r;
}

/**
 * @return [Integer]
 */
static VALUE
gdip_enumflags_to_i(VALUE self)
{
    return RB_UINT2NUM(Data_Ptr_As<unsigned int>(self));
}

static VALUE
gdip_enumflags_or(VALUE self, VALUE other)
{
    VALUE klass = CLASS_OF(self);
    if (RB_SYMBOL_P(other)) {
        other = rb_const_get(klass, RB_SYM2ID(other));
    }
    unsigned int n_self = Data_Ptr_As<unsigned int>(self);
    unsigned int n_other = 0;
    if (_KIND_OF(other, &tEnumInt) && rb_obj_is_kind_of(other, klass)) {
        n_other = Data_Ptr_As<unsigned int>(other);
    }
    else if (_RB_INTEGER_P(other)) {
        n_other = RB_NUM2UINT(other);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be %s.", __class__(self));
    }
    return gdip_enumint_create(klass, n_self | n_other);
}

static VALUE
gdip_enumflags_and(VALUE self, VALUE other)
{
    VALUE klass = CLASS_OF(self);
    if (RB_SYMBOL_P(other)) {
        other = rb_const_get(klass, RB_SYM2ID(other));
    }
    unsigned int n_self = Data_Ptr_As<unsigned int>(self);
    unsigned int n_other = 0;
    if (_KIND_OF(other, &tEnumInt) && rb_obj_is_kind_of(other, klass)) {
        n_other = Data_Ptr_As<unsigned int>(other);
    }
    else if (_RB_INTEGER_P(other)) {
        n_other = RB_NUM2UINT(other);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be %s.", __class__(self));
    }
    return gdip_enumint_create(klass, n_self & n_other);
}

/**
 * Compares to other.
 * @return [Boolean]
 */
static VALUE
gdip_enumflags_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    if (_KIND_OF(other, &tEnumInt)) {
        unsigned int x = Data_Ptr_As<unsigned int>(self);
        unsigned int y = Data_Ptr_As<unsigned int>(other);
        return x == y ? Qtrue : Qfalse;
    }
    else if (_RB_INTEGER_P(other)) {
        unsigned int x = Data_Ptr_As<unsigned int>(self);
        unsigned int y = RB_NUM2UINT(other);
        return x == y ? Qtrue : Qfalse;
    }
    else return Qfalse;
}

static VALUE
gdip_enumflags_add_flag(VALUE self)
{
    VALUE klass = CLASS_OF(self);
    VALUE adding = rb_const_get(klass, rb_frame_this_func());
    unsigned int n_self = Data_Ptr_As<unsigned int>(self);
    unsigned int n_adding = Data_Ptr_As<unsigned int>(adding);
    return gdip_enumint_create(klass, n_self | n_adding);
}

template <typename T>
static VALUE
gdip_enumptr_create(const rb_data_type_t *type, T data, VALUE klass=Qnil)
{
    T ptr = static_cast<T>(ruby_xcalloc(1, sizeof(*data)));
    DPT("enum create");
    *ptr = *data;
    if (RB_NIL_P(klass)) klass = _KLASS(type);
    VALUE r = _Data_Wrap_Struct(klass, type, ptr);
    return r;
}

template <typename T>
ID
gdip_enum_get_id(VALUE klass, T data) {
    IMap<T, ID> *table = gdip_enum_get_table<T>(klass);
    ID id = ID_UNKNOWN;
    if (table != NULL) {
        table->get(data, id);
    }
    return id;
}

template ID gdip_enum_get_id<GUID *>(VALUE klass, GUID *data);
template ID gdip_enum_get_id<int>(VALUE klass, int data);

template <typename T>
VALUE
gdip_enum_get(VALUE klass, T data) {
    IMap<T, ID> *table = gdip_enum_get_table<T>(klass);
    VALUE v = Qnil;
    ID id = ID_UNKNOWN;
    if (table != NULL) {
        table->get(data, id);
    }
    if (id != ID_UNKNOWN) {
        v = rb_const_get(klass, id);
    }
    return v;
}

template ID gdip_enum_get<GUID *>(VALUE klass, GUID *data);
template ID gdip_enum_get<int>(VALUE klass, int data);

VALUE
gdip_enum_guid_create(VALUE klass, GUID *guid)
{
    VALUE r = gdip_enum_get(klass, guid);
    if (RB_NIL_P(r)) {
        r = typeddata_alloc<GUID, &tGuid>(klass);
        GUID *g = Data_Ptr<GUID *>(r);
        *g = *guid;
    }
    return r;
}

template <typename TKey>
static void
gdip_enum_define(VALUE klass, IMap<TKey, ID> *table, const char *name, TKey data, VALUE v)
{
    ID id = rb_intern(name);
    rb_define_const(klass, name, v);
    rb_define_singleton_method(klass, name, RUBY_METHOD_FUNC(gdip_enum_const_get), 0);
    table->append(data, id);
}


#define define_enumint(klass, table, name, num) gdip_enum_define<int>(klass, table, name, num, gdip_enumint_create(klass, num))
#define define_enumflags(klass, table, name, num) \
    do {\
        gdip_enum_define<unsigned int>(klass, table, name, num, gdip_enumint_create(klass, num));\
        rb_define_method(klass, name, RUBY_METHOD_FUNC(gdip_enumflags_add_flag), 0);\
    } while (0)

static void
Init_PixelFormat()
{
    cPixelFormat = rb_define_class_under(mGdiplus, "PixelFormat", cEnumInt);
    rb_undef_alloc_func(cPixelFormat);
    SortedArrayMap<int, ID> *table = new SortedArrayMap<int, ID>(25);
    klass_table_map.set(cPixelFormat, table);

    define_enumint(cPixelFormat, table, "Format32bppARGB", PixelFormat32bppARGB);
    define_enumint(cPixelFormat, table, "Format24bppRGB", PixelFormat24bppRGB);
    define_enumint(cPixelFormat, table, "Format32bppRGB", PixelFormat32bppRGB);
    define_enumint(cPixelFormat, table, "Format32bppPARGB", PixelFormat32bppPARGB);
    define_enumint(cPixelFormat, table, "Format48bppRGB", PixelFormat48bppRGB);
    define_enumint(cPixelFormat, table, "Format64bppARGB", PixelFormat64bppARGB);
    define_enumint(cPixelFormat, table, "Format64bppPARGB", PixelFormat64bppPARGB);
    define_enumint(cPixelFormat, table, "Format32bppCMYK", (15 | (32 << 8)));
    define_enumint(cPixelFormat, table, "Format1bppIndexed", PixelFormat1bppIndexed);
    define_enumint(cPixelFormat, table, "Format4bppIndexed", PixelFormat4bppIndexed);
    define_enumint(cPixelFormat, table, "Format8bppIndexed", PixelFormat8bppIndexed);
    define_enumint(cPixelFormat, table, "Format16bppGrayScale", PixelFormat16bppGrayScale);
    define_enumint(cPixelFormat, table, "Format16bppRGB555", PixelFormat16bppRGB555);
    define_enumint(cPixelFormat, table, "Format16bppRGB565", PixelFormat16bppRGB565);
    define_enumint(cPixelFormat, table, "Format16bppARGB1555", PixelFormat16bppARGB1555);
    define_enumint(cPixelFormat, table, "Undefined", PixelFormatUndefined);
    define_enumint(cPixelFormat, table, "Indexed", PixelFormatIndexed);
    define_enumint(cPixelFormat, table, "GDI", PixelFormatGDI);
    define_enumint(cPixelFormat, table, "Gdi", PixelFormatGDI);
    define_enumint(cPixelFormat, table, "Alpha", PixelFormatAlpha);
    define_enumint(cPixelFormat, table, "PAlpha", PixelFormatPAlpha);
    define_enumint(cPixelFormat, table, "Extended", PixelFormatExtended);
    define_enumint(cPixelFormat, table, "Canonical", PixelFormatCanonical);
    define_enumint(cPixelFormat, table, "Max", 16);
    define_enumint(cPixelFormat, table, "DontCare", PixelFormatDontCare);
}

static void
Init_EncoderParameterValueType()
{
    cEncoderParameterValueType = rb_define_class_under(mGdiplus, "EncoderParameterValueType", cEnumInt);
    rb_undef_alloc_func(cEncoderParameterValueType);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(10);
    klass_table_map.set(cEncoderParameterValueType, table);

    define_enumint(cEncoderParameterValueType, table, "ValueTypeByte", EncoderParameterValueTypeByte);
    define_enumint(cEncoderParameterValueType, table, "ValueTypeAscii", EncoderParameterValueTypeASCII);
    define_enumint(cEncoderParameterValueType, table, "ValueTypeShort", EncoderParameterValueTypeShort);
    define_enumint(cEncoderParameterValueType, table, "ValueTypeLong", EncoderParameterValueTypeLong);
    define_enumint(cEncoderParameterValueType, table, "ValueTypeRational", EncoderParameterValueTypeRational);
    define_enumint(cEncoderParameterValueType, table, "ValueTypeLongRange", EncoderParameterValueTypeLongRange);
    define_enumint(cEncoderParameterValueType, table, "ValueTypeUndefined", EncoderParameterValueTypeUndefined);
    define_enumint(cEncoderParameterValueType, table, "ValueTypeRationalRange", EncoderParameterValueTypeRationalRange);
    define_enumint(cEncoderParameterValueType, table, "ValueTypePointer", EncoderParameterValueTypePointer);
}

/* ValueType */

static void
Init_EncoderValue()
{
    cEncoderValue = rb_define_class_under(mGdiplus, "EncoderValue", cEnumInt);
    rb_undef_alloc_func(cEncoderValue);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(26);
    klass_table_map.set(cEncoderValue, table);
    
    define_enumint(cEncoderValue, table, "ColorTypeCMYK", 0);
    define_enumint(cEncoderValue, table, "ColorTypeYCCK", 1);
    define_enumint(cEncoderValue, table, "CompressionLZW", 2);
    define_enumint(cEncoderValue, table, "CompressionCCITT3", 3);
    define_enumint(cEncoderValue, table, "CompressionCCITT4", 4);
    define_enumint(cEncoderValue, table, "CompressionRle", 5);
    define_enumint(cEncoderValue, table, "CompressionNone", 6);
    define_enumint(cEncoderValue, table, "ScanMethodInterlaced", 7);
    define_enumint(cEncoderValue, table, "ScanMethodNonInterlaced", 8);
    define_enumint(cEncoderValue, table, "VersionGif87", 9);
    define_enumint(cEncoderValue, table, "VersionGif89", 10);
    define_enumint(cEncoderValue, table, "RenderProgressive", 11);
    define_enumint(cEncoderValue, table, "RenderNonProgressive", 12);
    define_enumint(cEncoderValue, table, "TransformRotate90", 13);
    define_enumint(cEncoderValue, table, "TransformRotate180", 14);
    define_enumint(cEncoderValue, table, "TransformRotate270", 15);
    define_enumint(cEncoderValue, table, "TransformFlipHorizontal", 16);
    define_enumint(cEncoderValue, table, "TransformFlipVertical", 17);
    define_enumint(cEncoderValue, table, "MultiFrame", 18);
    define_enumint(cEncoderValue, table, "LastFrame", 19);
    define_enumint(cEncoderValue, table, "Flush", 20);
    define_enumint(cEncoderValue, table, "FrameDimensionTime", 21);
    define_enumint(cEncoderValue, table, "FrameDimensionResolution", 22);
    define_enumint(cEncoderValue, table, "FrameDimensionPage", 23);
    define_enumint(cEncoderValue, table, "ColorTypeGray", 24);
    define_enumint(cEncoderValue, table, "ColorTypeRGB", 25);
}

static void
Init_BrushType()
{
    cBrushType = rb_define_class_under(mGdiplus, "BrushType", cEnumInt);
    rb_undef_alloc_func(cBrushType);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(5);
    klass_table_map.set(cBrushType, table);

    define_enumint(cBrushType, table, "SolidColor", 0);
    define_enumint(cBrushType, table, "HatchFill", 1);
    define_enumint(cBrushType, table, "TextureFill", 2);
    define_enumint(cBrushType, table, "PathGradient", 3);
    define_enumint(cBrushType, table, "LinearGradient", 4);
}

static void
Init_CustomLineCapType()
{
    cCustomLineCapType = rb_define_class_under(mGdiplus, "CustomLineCapType", cEnumInt);
    rb_undef_alloc_func(cCustomLineCapType);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(2);
    klass_table_map.set(cCustomLineCapType, table);

    define_enumint(cCustomLineCapType, table, "Default", 0);
    define_enumint(cCustomLineCapType, table, "AdjustableArrow", 1);
}

static void
Init_DashCap()
{
    cDashCap = rb_define_class_under(mGdiplus, "DashCap", cEnumInt);
    rb_undef_alloc_func(cDashCap);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(4);
    klass_table_map.set(cDashCap, table);

    define_enumint(cDashCap, table, "Flat", 0);
    define_enumint(cDashCap, table, "Round", 2);
    define_enumint(cDashCap, table, "Triangle", 3);
}

static void
Init_DashStyle()
{
    cDashStyle = rb_define_class_under(mGdiplus, "DashStyle", cEnumInt);
    rb_undef_alloc_func(cDashStyle);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(6);
    klass_table_map.set(cDashStyle, table);

    define_enumint(cDashStyle, table, "Solid", 0);
    define_enumint(cDashStyle, table, "Dash", 1);
    define_enumint(cDashStyle, table, "Dot", 2);
    define_enumint(cDashStyle, table, "DashDot", 3);
    define_enumint(cDashStyle, table, "DashDotDot", 4);
    define_enumint(cDashStyle, table, "Custom", 5);
}

static void
Init_LineCap()
{
    cLineCap = rb_define_class_under(mGdiplus, "LineCap", cEnumInt);
    rb_undef_alloc_func(cLineCap);
    SortedArrayMap<int, ID> *table = new SortedArrayMap<int, ID>(11);
    klass_table_map.set(cLineCap, table);

    define_enumint(cLineCap, table, "Flat", 0);
    define_enumint(cLineCap, table, "Square", 1);
    define_enumint(cLineCap, table, "Round", 2);
    define_enumint(cLineCap, table, "Triangle", 3);
    define_enumint(cLineCap, table, "NoAnchor", 16);
    define_enumint(cLineCap, table, "SquareAnchor", 17);
    define_enumint(cLineCap, table, "RoundAnchor", 18);
    define_enumint(cLineCap, table, "DiamondAnchor", 19);
    define_enumint(cLineCap, table, "ArrowAnchor", 20);
    define_enumint(cLineCap, table, "Custom", 255);
    define_enumint(cLineCap, table, "AnchorMask", 240);
}

static void
Init_LineJoin()
{
    cLineJoin = rb_define_class_under(mGdiplus, "LineJoin", cEnumInt);
    rb_undef_alloc_func(cLineJoin);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(4);
    klass_table_map.set(cLineJoin, table);

    define_enumint(cLineJoin, table, "Miter", 0);
    define_enumint(cLineJoin, table, "Bevel", 1);
    define_enumint(cLineJoin, table, "Round", 2);
    define_enumint(cLineJoin, table, "MiterClipped", 3);
}

static void
Init_MatrixOrder()
{
    cMatrixOrder = rb_define_class_under(mGdiplus, "MatrixOrder", cEnumInt);
    rb_undef_alloc_func(cMatrixOrder);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(2);
    klass_table_map.set(cMatrixOrder, table);

    define_enumint(cMatrixOrder, table, "Prepend", 0);
    define_enumint(cMatrixOrder, table, "Append", 1);
}

static void
Init_PenAlignment()
{
    cPenAlignment = rb_define_class_under(mGdiplus, "PenAlignment", cEnumInt);
    rb_undef_alloc_func(cPenAlignment);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(2);
    klass_table_map.set(cPenAlignment, table);

    define_enumint(cPenAlignment, table, "Center", 0);
    define_enumint(cPenAlignment, table, "Inset", 1);
}

static void
Init_PenType()
{
    cPenType = rb_define_class_under(mGdiplus, "PenType", cEnumInt);
    rb_undef_alloc_func(cPenType);
    SortedArrayMap<int, ID> *table = new SortedArrayMap<int, ID>(6);
    klass_table_map.set(cPenType, table);

    define_enumint(cPenType, table, "SolidColor", 0);
    define_enumint(cPenType, table, "HatchFill", 1);
    define_enumint(cPenType, table, "TextureFill", 2);
    define_enumint(cPenType, table, "PathGradient", 3);
    define_enumint(cPenType, table, "LinearGradient", 4);
    define_enumint(cPenType, table, "Unknown", -1);
}

/**
 * Document-class: Gdiplus::FontStyle
 * @example
 *   FontStyle.Bold | FontStyle.Italic
 *   #=> #<Gdiplus::FontStyle 0x00000003: Italic | Bold >
 *   FontStyle.new(0b11)
 *   #=> #<Gdiplus::FontStyle 0x00000003: Italic | Bold >
 *   FontStyle.new(:Bold, :Italic)
 *   #=> #<Gdiplus::FontStyle 0x00000003: Italic | Bold >
 *   FontStyle.new | :Bold | :Italic
 *   #=> #<Gdiplus::FontStyle 0x00000003: Italic | Bold >
 *   FontStyle.Bold.Italic
 *   #=> #<Gdiplus::FontStyle 0x00000003: Italic | Bold >
 */
static void
Init_FontStyle()
{
    //cFontStyle = rb_define_class_under(mGdiplus, "FontStyle", cEnumInt);
    //rb_undef_alloc_func(cFontStyle);
    cFontStyle = rb_define_class_under(mGdiplus, "FontStyle", cEnumFlags);
    SortedArrayMap<unsigned int, ID> *table = new SortedArrayMap<unsigned int, ID>(5);
    klass_table_map.set(cFontStyle, table);

    define_enumflags(cFontStyle, table, "Regular", 0);
    define_enumflags(cFontStyle, table, "Bold", 1);
    define_enumflags(cFontStyle, table, "Italic", 2);
    define_enumflags(cFontStyle, table, "Underline", 4);
    define_enumflags(cFontStyle, table, "Strikeout", 8);
    //define_enumflags(cFontStyle, table, "BoldItalic", 3);
}

static void
Init_GenericFontFamilies()
{
    cGenericFontFamilies = rb_define_class_under(mGdiplus, "GenericFontFamilies", cEnumInt);
    rb_undef_alloc_func(cGenericFontFamilies);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(3);
    klass_table_map.set(cGenericFontFamilies, table);

    define_enumint(cGenericFontFamilies, table, "Serif", 0);
    define_enumint(cGenericFontFamilies, table, "SansSerif", 1);
    define_enumint(cGenericFontFamilies, table, "Monospace", 2);
}

static void
Init_PixelOffsetMode()
{
    cPixelOffsetMode = rb_define_class_under(mGdiplus, "PixelOffsetMode", cEnumInt);
    rb_undef_alloc_func(cPixelOffsetMode);
    SortedArrayMap<int, ID> *table = new SortedArrayMap<int, ID>(6);
    klass_table_map.set(cPixelOffsetMode, table);

    define_enumint(cPixelOffsetMode, table, "Invalid", -1);
    define_enumint(cPixelOffsetMode, table, "Default", 0);
    define_enumint(cPixelOffsetMode, table, "HighSpeed", 1);
    define_enumint(cPixelOffsetMode, table, "HighQuality", 2);
    define_enumint(cPixelOffsetMode, table, "None", 3);
    define_enumint(cPixelOffsetMode, table, "Half", 4);
}

static void
Init_SmoothingMode()
{
    cSmoothingMode = rb_define_class_under(mGdiplus, "SmoothingMode", cEnumInt);
    rb_undef_alloc_func(cSmoothingMode);
    SortedArrayMap<int, ID> *table = new SortedArrayMap<int, ID>(8);
    klass_table_map.set(cSmoothingMode, table);

    define_enumint(cSmoothingMode, table, "Invalid", -1);
    define_enumint(cSmoothingMode, table, "Default", 0);
    define_enumint(cSmoothingMode, table, "HighSpeed", 1);
    define_enumint(cSmoothingMode, table, "HighQuality", 2);
    define_enumint(cSmoothingMode, table, "None", 3);
    define_enumint(cSmoothingMode, table, "AntiAlias", 4);
    define_enumint(cSmoothingMode, table, "AntiAlias8x4", 4);
    define_enumint(cSmoothingMode, table, "AntiAlias8x8", 5);
}

static void
Init_TextRenderingHint()
{
    cTextRenderingHint = rb_define_class_under(mGdiplus, "TextRenderingHint", cEnumInt);
    rb_undef_alloc_func(cTextRenderingHint);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(6);
    klass_table_map.set(cTextRenderingHint, table);

    define_enumint(cTextRenderingHint, table, "SystemDefault", 0);
    define_enumint(cTextRenderingHint, table, "SingleBitPerPixelGridFit", 1);
    define_enumint(cTextRenderingHint, table, "SingleBitPerPixel", 2);
    define_enumint(cTextRenderingHint, table, "AntiAliasGridFit", 3);
    define_enumint(cTextRenderingHint, table, "AntiAlias", 4);
    define_enumint(cTextRenderingHint, table, "ClearTypeGridFit", 5);
}

static void
Init_CompositingMode()
{
    cCompositingMode = rb_define_class_under(mGdiplus, "CompositingMode", cEnumInt);
    rb_undef_alloc_func(cCompositingMode);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(2);
    klass_table_map.set(cCompositingMode, table);

    define_enumint(cCompositingMode, table, "SourceOver", 0);
    define_enumint(cCompositingMode, table, "SourceCopy", 1);
}

static void
Init_CompositingQuality()
{
    cCompositingQuality = rb_define_class_under(mGdiplus, "CompositingQuality", cEnumInt);
    rb_undef_alloc_func(cCompositingQuality);
    SortedArrayMap<int, ID> *table = new SortedArrayMap<int, ID>(6);
    klass_table_map.set(cCompositingQuality, table);

    define_enumint(cCompositingQuality, table, "Invalid", -1);
    define_enumint(cCompositingQuality, table, "Default", 0);
    define_enumint(cCompositingQuality, table, "HighSpeed", 1);
    define_enumint(cCompositingQuality, table, "HighQuality", 2);
    define_enumint(cCompositingQuality, table, "GammaCorrected", 3);
    define_enumint(cCompositingQuality, table, "AssumeLinear", 4);
}

static void
Init_FillMode()
{
    cFillMode = rb_define_class_under(mGdiplus, "FillMode", cEnumInt);
    rb_undef_alloc_func(cFillMode);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(2);
    klass_table_map.set(cFillMode, table);

    define_enumint(cFillMode, table, "Alternate", 0);
    define_enumint(cFillMode, table, "Winding", 1);
}

static void
Init_GraphicsUnit()
{
    cGraphicsUnit = rb_define_class_under(mGdiplus, "GraphicsUnit", cEnumInt);
    rb_undef_alloc_func(cGraphicsUnit);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(7);
    klass_table_map.set(cGraphicsUnit, table);

    define_enumint(cGraphicsUnit, table, "World", 0);
    define_enumint(cGraphicsUnit, table, "Display", 1);
    define_enumint(cGraphicsUnit, table, "Pixel", 2);
    define_enumint(cGraphicsUnit, table, "Point", 3);
    define_enumint(cGraphicsUnit, table, "Inch", 4);
    define_enumint(cGraphicsUnit, table, "Document", 5);
    define_enumint(cGraphicsUnit, table, "Millimeter", 6);
}

static void
Init_InterpolationMode()
{
    cInterpolationMode = rb_define_class_under(mGdiplus, "InterpolationMode", cEnumInt);
    rb_undef_alloc_func(cInterpolationMode);
    SortedArrayMap<int, ID> *table = new SortedArrayMap<int, ID>(9);
    klass_table_map.set(cInterpolationMode, table);

    define_enumint(cInterpolationMode, table, "Invalid", -1);
    define_enumint(cInterpolationMode, table, "Default", 0);
    define_enumint(cInterpolationMode, table, "LowQuality", 1);
    define_enumint(cInterpolationMode, table, "HighQuality", 2);
    define_enumint(cInterpolationMode, table, "Bilinear", 3);
    define_enumint(cInterpolationMode, table, "Bicubic", 4);
    define_enumint(cInterpolationMode, table, "NearestNeighbor", 5);
    define_enumint(cInterpolationMode, table, "HighQualityBilinear", 6);
    define_enumint(cInterpolationMode, table, "HighQualityBicubic", 7);
}

/* Encoder */

/**
 *
 * @return [String]
 * 
 */
static VALUE
gdip_enumguid_inspect(VALUE self)
{
    ID id = ID_UNKNOWN;
    VALUE r;
    GUID *guid = Data_Ptr<GUID *>(self);
    IMap<GUID *, ID> *table = gdip_enum_get_table<GUID *>(CLASS_OF(self));
    if (table != NULL) {
        table->get(guid, id);
    }
    unsigned char *cstr; // RPC_CSTR
    if (RPC_S_OK == ::UuidToStringA(guid, &cstr)) {
        r = util_utf8_sprintf("#<%s.%s: {%s}>", __class__(self), rb_id2name(id), reinterpret_cast<char *>(cstr));
        RpcStringFree(&cstr);
    }
    else {
        r = util_utf8_sprintf("#<%s.%s>", __class__(self), rb_id2name(id));
    }
    return r;
}

void
Init_Encoder()
{
    cEncoder = rb_define_class_under(mGdiplus, "Encoder", cGuid);
    rb_undef_alloc_func(cEncoder);
    rb_define_method(cEncoder, "inspect", RUBY_METHOD_FUNC(gdip_enumguid_inspect), 0);
    MemPtrSortedArrayMap<GUID *, ID> *table = new MemPtrSortedArrayMap<GUID *, ID>(13);
    klass_table_map.set(cEncoder, table);
    
    VALUE v;
    GUID guid;

    ID_Compression = rb_intern("Compression");
    guid = EncoderCompression;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "Compression", Data_Ptr<GUID *>(v), v);

    ID_ColorDepth = rb_intern("ColorDepth");
    guid = EncoderColorDepth;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "ColorDepth", Data_Ptr<GUID *>(v), v);

    ID_ScanMethod = rb_intern("ScanMethod");
    guid = EncoderScanMethod;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "ScanMethod", Data_Ptr<GUID *>(v), v);

    ID_Version = rb_intern("Version");
    guid = EncoderVersion;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "Version", Data_Ptr<GUID *>(v), v);

    ID_RenderMethod = rb_intern("RenderMethod");
    guid = EncoderRenderMethod;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "RenderMethod", Data_Ptr<GUID *>(v), v);

    ID_Quality = rb_intern("Quality");
    guid = EncoderQuality;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "Quality", Data_Ptr<GUID *>(v), v);

    ID_Transformation = rb_intern("Transformation");
    guid = EncoderTransformation;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "Transformation", Data_Ptr<GUID *>(v), v);

    ID_LuminanceTable = rb_intern("LuminanceTable");
    guid = EncoderLuminanceTable;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "LuminanceTable", Data_Ptr<GUID *>(v), v);

    ID_ChrominanceTable = rb_intern("ChrominanceTable");
    guid = EncoderChrominanceTable;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "ChrominanceTable", Data_Ptr<GUID *>(v), v);

    ID_SaveFlag = rb_intern("SaveFlag");
    guid = EncoderSaveFlag;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "SaveFlag", Data_Ptr<GUID *>(v), v);

    ID_ColorSpace = rb_intern("ColorSpace");
    GUID guid_cs = {0xae7a62a0,0xee2c,0x49d8,{0x9d,0x7,0x1b,0xa8,0xa9,0x27,0x59,0x6e}};
    _EncoderColorSpace = guid_cs;
    v = gdip_enumptr_create<GUID *>(&tGuid, &_EncoderColorSpace, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "ColorSpace", Data_Ptr<GUID *>(v), v);

    ID_ImageItems = rb_intern("ImageItems");
    GUID guid_ii = {0x63875e13,0x1f1d,0x45ab,{0x91, 0x95, 0xa2, 0x9b, 0x60, 0x66, 0xa6, 0x50}};
    _EncoderImageItems = guid_ii;
    v = gdip_enumptr_create<GUID *>(&tGuid, &_EncoderImageItems, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "ImageItems", Data_Ptr<GUID *>(v), v);

    ID_SaveAsCMYK = rb_intern("SaveAsCMYK");
    GUID guid_sacmyk = {0xa219bbc9, 0xa9d, 0x4005, {0xa3, 0xee, 0x3a, 0x42, 0x1b, 0x8b, 0xb0, 0x6c}};
    _EncoderSaveAsCMYK = guid_sacmyk;
    v = gdip_enumptr_create<GUID *>(&tGuid, &_EncoderSaveAsCMYK, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "SaveAsCMYK", Data_Ptr<GUID *>(v), v);

}

static void
Init_imageformat()
{
    cImageFormat = rb_define_class_under(mGdiplus, "ImageFormat", cGuid);
    rb_undef_alloc_func(cImageFormat);
    rb_define_method(cImageFormat, "inspect", RUBY_METHOD_FUNC(gdip_enumguid_inspect), 0);
    MemPtrSortedArrayMap<GUID *, ID> *table = new MemPtrSortedArrayMap<GUID *, ID>(11);
    klass_table_map.set(cImageFormat, table);
    
    VALUE v;
    GUID guid;

    GUID guid_undef = {0xb96b3ca9,0x0728,0x11d3,{0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e}};
    _ImageFormatUndefined = guid_undef;
    v = gdip_enumptr_create<GUID *>(&tGuid, &_ImageFormatUndefined, cImageFormat);
    gdip_enum_define<GUID *>(cImageFormat, table, "Undefined", Data_Ptr<GUID *>(v), v);

    guid = ImageFormatMemoryBMP;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cImageFormat);
    gdip_enum_define<GUID *>(cImageFormat, table, "MemoryBmp", Data_Ptr<GUID *>(v), v);

    guid = ImageFormatBMP;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cImageFormat);
    gdip_enum_define<GUID *>(cImageFormat, table, "Bmp", Data_Ptr<GUID *>(v), v);

    guid = ImageFormatEMF;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cImageFormat);
    gdip_enum_define<GUID *>(cImageFormat, table, "Emf", Data_Ptr<GUID *>(v), v);

    guid = ImageFormatWMF;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cImageFormat);
    gdip_enum_define<GUID *>(cImageFormat, table, "Wmf", Data_Ptr<GUID *>(v), v);

    guid = ImageFormatJPEG;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cImageFormat);
    gdip_enum_define<GUID *>(cImageFormat, table, "Jpeg", Data_Ptr<GUID *>(v), v);

    guid = ImageFormatPNG;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cImageFormat);
    gdip_enum_define<GUID *>(cImageFormat, table, "Png", Data_Ptr<GUID *>(v), v);

    guid = ImageFormatGIF;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cImageFormat);
    gdip_enum_define<GUID *>(cImageFormat, table, "Gif", Data_Ptr<GUID *>(v), v);

    guid = ImageFormatTIFF;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cImageFormat);
    gdip_enum_define<GUID *>(cImageFormat, table, "Tiff", Data_Ptr<GUID *>(v), v);


    GUID guid_exif = {0xb96b3cb2,0x0728,0x11d3,{0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e}};
    _ImageFormatEXIF = guid_exif;
    v = gdip_enumptr_create<GUID *>(&tGuid, &_ImageFormatEXIF, cImageFormat);
    gdip_enum_define<GUID *>(cImageFormat, table, "Exif", Data_Ptr<GUID *>(v), v);

    guid = ImageFormatIcon;
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid, cImageFormat);
    gdip_enum_define<GUID *>(cImageFormat, table, "Icon", Data_Ptr<GUID *>(v), v);
}

void
Init_enum() {
    ID_UNKNOWN = rb_intern("__UNKNOWN__");

    cEnumInt = rb_define_class_under(mInternals, "EnumInt", rb_cObject);
    rb_undef_alloc_func(cEnumInt);
    rb_define_method(cEnumInt, "inspect", RUBY_METHOD_FUNC(gdip_enumint_inspect), 0);
    rb_define_method(cEnumInt, "to_i", RUBY_METHOD_FUNC(gdip_enumint_to_i), 0);
    rb_define_method(cEnumInt, "to_int", RUBY_METHOD_FUNC(gdip_enumint_to_i), 0);
    rb_define_method(cEnumInt, "|", RUBY_METHOD_FUNC(gdip_enumint_or), 1);
    rb_define_method(cEnumInt, "&", RUBY_METHOD_FUNC(gdip_enumint_and), 1);
    rb_define_method(cEnumInt, "coerce", RUBY_METHOD_FUNC(gdip_enumint_coerce), 1);
    rb_define_method(cEnumInt, "==", RUBY_METHOD_FUNC(gdip_enumint_equal), 1);

    cEnumFlags = rb_define_class_under(mInternals, "EnumFlags", cEnumInt);
    rb_define_alloc_func(cEnumFlags, &typeddata_alloc_null<&tEnumInt>);
    rb_define_method(cEnumFlags, "initialize", RUBY_METHOD_FUNC(gdip_enumflags_init), -1);
    rb_define_method(cEnumFlags, "inspect", RUBY_METHOD_FUNC(gdip_enumflags_inspect), 0);
    rb_define_method(cEnumFlags, "to_i", RUBY_METHOD_FUNC(gdip_enumflags_to_i), 0);
    rb_define_method(cEnumFlags, "to_int", RUBY_METHOD_FUNC(gdip_enumflags_to_i), 0);
    rb_define_method(cEnumFlags, "|", RUBY_METHOD_FUNC(gdip_enumflags_or), 1);
    rb_define_method(cEnumFlags, "&", RUBY_METHOD_FUNC(gdip_enumflags_and), 1);
    rb_define_method(cEnumFlags, "==", RUBY_METHOD_FUNC(gdip_enumflags_equal), 1);

    Init_PixelFormat();
    Init_EncoderParameterValueType();
    Init_EncoderValue();
    Init_BrushType();
    Init_CustomLineCapType();
    Init_DashCap();
    Init_DashStyle();
    Init_LineCap();
    Init_LineJoin();
    Init_MatrixOrder();
    Init_PenAlignment();
    Init_PenType();
    Init_FontStyle();
    Init_GenericFontFamilies();
    Init_PixelOffsetMode();
    Init_SmoothingMode();
    Init_TextRenderingHint();
    Init_CompositingMode();
    Init_CompositingQuality();
    Init_FillMode();
    Init_GraphicsUnit();
    Init_InterpolationMode();
    
    Init_Encoder();
    Init_imageformat();
}