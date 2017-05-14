/*
 * gdip_enum.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
 #include "ruby_gdiplus.h"
 #include "simplemap.h"

static ID ID_UNKNOWN;

class KlassTableMap : public SortedArrayMap<VALUE, MapBase *> {
public:
    KlassTableMap(int capa) : SortedArrayMap<VALUE, MapBase *>(capa) {}
    ~KlassTableMap() {
        for (int i = 0; i < this->Len; ++i) {
            dp("KlassTableMap: delete %d", i);
            delete this->ValTable[i];
        }
    }
};

static KlassTableMap klass_table_map(10);

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

static VALUE
gdip_enumint_create(const rb_data_type_t *type, int num, VALUE klass = Qnil)
{
    if (RB_NIL_P(klass)) klass = _KLASS(type);
    VALUE r = _Data_Wrap_Struct(klass, type, reinterpret_cast<void*>(num));
    return r;
}

/*
 * Returns a constant value.
 * @return [Integer]
 */
static VALUE
gdip_enumint_to_i(VALUE self)
{
    return RB_INT2NUM(Data_Ptr_As<int>(self));
}

static VALUE
gdip_enumint_or(VALUE self, VALUE other)
{
    VALUE v = rb_to_int(other);
    int n_other = RB_NUM2INT(v);
    int n_self = Data_Ptr_As<int>(self);
    return RB_INT2NUM(n_self | n_other);
}

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

template <typename T>
static VALUE
gdip_enumptr_create(const rb_data_type_t *type, T data, VALUE klass=Qnil)
{
    T ptr = static_cast<T>(ruby_xcalloc(1, sizeof(*data)));
    *ptr = *data;
    if (RB_NIL_P(klass)) klass = _KLASS(type);
    VALUE r = _Data_Wrap_Struct(klass, type, ptr);
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

static VALUE
gdip_enumint_inspect(VALUE self)
{
    int num = Data_Ptr_As<int>(self);
    IMap<int, ID> *table = gdip_enum_get_table<int>(CLASS_OF(self));
    if (table != NULL) {
        ID id;
        table->get(num, id, ID_UNKNOWN);
        return util_utf8_sprintf("<%s.%s: %08x>", __class__(self), rb_id2name(id), num);
    }
    else {
        return util_utf8_sprintf("<%s: %08x>", __class__(self), num);
    }
    
}

VALUE
gdip_enumint_get(VALUE klass, int n)
{
    return gdip_enumint_create(&tEnumInt, n, klass);
}

#define define_enumint(klass, table, name, num) gdip_enum_define<int>(klass, table, name, num, gdip_enumint_create(&tEnumInt, num, klass))

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
    define_enumint(cPixelFormat, table, "Format32bppCMYK", PixelFormat32bppCMYK);
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
    define_enumint(cPixelFormat, table, "Max", PixelFormatMax);
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
    define_enumint(cEncoderParameterValueType, table, "ValueTypeASCII", EncoderParameterValueTypeASCII);
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
Init_ValueType()
{
    cValueType = rb_define_class_under(mGdiplus, "ValueType", cEnumInt);
    rb_undef_alloc_func(cValueType);
    IndexArrayMap<ID> *table = new IndexArrayMap<ID>(26);
    klass_table_map.set(cValueType, table);
    
    define_enumint(cValueType, table, "ColorTypeCMYK", EncoderValueColorTypeCMYK);
    define_enumint(cValueType, table, "ColorTypeYCCK", EncoderValueColorTypeYCCK);
    define_enumint(cValueType, table, "CompressionLZW", EncoderValueCompressionLZW);
    define_enumint(cValueType, table, "CompressionCCITT3", EncoderValueCompressionCCITT3);
    define_enumint(cValueType, table, "CompressionCCITT4", EncoderValueCompressionCCITT4);
    define_enumint(cValueType, table, "CompressionRle", EncoderValueCompressionRle);
    define_enumint(cValueType, table, "CompressionNone", EncoderValueCompressionNone);
    define_enumint(cValueType, table, "ScanMethodInterlaced", EncoderValueScanMethodInterlaced);
    define_enumint(cValueType, table, "ScanMethodNonInterlaced", EncoderValueScanMethodNonInterlaced);
    define_enumint(cValueType, table, "VersionGif87", EncoderValueVersionGif87);
    define_enumint(cValueType, table, "VersionGif89", EncoderValueVersionGif89);
    define_enumint(cValueType, table, "RenderProgressive", EncoderValueRenderProgressive);
    define_enumint(cValueType, table, "RenderNonProgressive", EncoderValueRenderNonProgressive);
    define_enumint(cValueType, table, "TransformRotate90", EncoderValueTransformRotate90);
    define_enumint(cValueType, table, "TransformRotate180", EncoderValueTransformRotate180);
    define_enumint(cValueType, table, "TransformRotate270", EncoderValueTransformRotate270);
    define_enumint(cValueType, table, "TransformFlipHorizontal", EncoderValueTransformFlipHorizontal);
    define_enumint(cValueType, table, "TransformFlipVertical", EncoderValueTransformFlipVertical);
    define_enumint(cValueType, table, "MultiFrame", EncoderValueMultiFrame);
    define_enumint(cValueType, table, "LastFrame", EncoderValueLastFrame);
    define_enumint(cValueType, table, "Flush", EncoderValueFlush);
    define_enumint(cValueType, table, "FrameDimensionTime", EncoderValueFrameDimensionTime);
    define_enumint(cValueType, table, "FrameDimensionResolution", EncoderValueFrameDimensionResolution);
    define_enumint(cValueType, table, "FrameDimensionPage", EncoderValueFrameDimensionPage);
    define_enumint(cValueType, table, "ColorTypeGray", EncoderValueFrameDimensionPage + 1); // EncoderValueColorTypeGray
    define_enumint(cValueType, table, "ColorTypeRGB", EncoderValueFrameDimensionPage + 2); // EncoderValueColorTypeRGB
}

/* Encoder */

static VALUE
gdip_encoder_inspect(VALUE self)
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
        r = util_utf8_sprintf("<%s.%s: {%s}>", __class__(self), rb_id2name(id), reinterpret_cast<char *>(cstr));
        RpcStringFree(&cstr);
    }
    else {
        r = util_utf8_sprintf("<%s.%s>", __class__(self), rb_id2name(id));
    }
    return r;
}

VALUE 
gdip_encoder_get(GUID *guid)
{
    return gdip_enumptr_create<GUID *>(&tGuid, guid, cEncoder);
}

void
Init_Encoder()
{
    cEncoder = rb_define_class_under(mGdiplus, "Encoder", cGuid);
    rb_undef_alloc_func(cEncoder);
    rb_define_method(cEncoder, "inspect", RUBY_METHOD_FUNC(gdip_encoder_inspect), 0);
    MemPtrSortedArrayMap<GUID *, ID> *table = new MemPtrSortedArrayMap<GUID *, ID>(10);
    klass_table_map.set(cEncoder, table);
    
    VALUE v;

    GUID guid1 = {0xe09d739d,0xccd4,0x44ee,{0x8e,0xba,0x3f,0xbf,0x8b,0xe4,0xfc,0x58}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid1, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "Compression", Data_Ptr<GUID *>(v), v);

    GUID guid2 = {0x66087055,0xad66,0x4c7c,{0x9a,0x18,0x38,0xa2,0x31,0x0b,0x83,0x37}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid2, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "ColorDepth", Data_Ptr<GUID *>(v), v);

    GUID guid3 = {0x3a4e2661,0x3109,0x4e56,{0x85,0x36,0x42,0xc1,0x56,0xe7,0xdc,0xfa}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid3, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "ScanMethod", Data_Ptr<GUID *>(v), v);

    GUID guid4 = {0x24d18c76,0x814a,0x41a4,{0xbf,0x53,0x1c,0x21,0x9c,0xcc,0xf7,0x97}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid4, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "Version", Data_Ptr<GUID *>(v), v);

    GUID guid5 = {0x6d42c53a,0x229a,0x4825,{0x8b,0xb7,0x5c,0x99,0xe2,0xb9,0xa8,0xb8}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid5, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "RenderMethod", Data_Ptr<GUID *>(v), v);

    GUID guid6 = {0x1d5be4b5,0xfa4a,0x452d,{0x9c,0xdd,0x5d,0xb3,0x51,0x05,0xe7,0xeb}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid6, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "Quality", Data_Ptr<GUID *>(v), v);

    GUID guid7 = {0x8d0eb2d1,0xa58e,0x4ea8,{0xaa,0x14,0x10,0x80,0x74,0xb7,0xb6,0xf9}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid7, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "Transformation", Data_Ptr<GUID *>(v), v);

    GUID guid8 = {0xedb33bce,0x0266,0x4a77,{0xb9,0x04,0x27,0x21,0x60,0x99,0xe7,0x17}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid8, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "LuminanceTable", Data_Ptr<GUID *>(v), v);

    GUID guid9 = {0xf2e455dc,0x09b3,0x4316,{0x82,0x60,0x67,0x6a,0xda,0x32,0x48,0x1c}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid9, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "ChrominanceTable", Data_Ptr<GUID *>(v), v);

    GUID guid10 = {0x292266fc,0xac40,0x47bf,{0x8c, 0xfc, 0xa8, 0x5b, 0x89, 0xa6, 0x55, 0xde}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid10, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, table, "SaveFlag", Data_Ptr<GUID *>(v), v);
}

void
Init_enum() {
    ID_UNKNOWN = rb_intern("__UNKNOWN__");

    cEnumInt = rb_define_class_under(mInternals, "EnumInt", rb_cObject);
    rb_define_method(cEnumInt, "inspect", RUBY_METHOD_FUNC(gdip_enumint_inspect), 0);
    rb_define_method(cEnumInt, "to_i", RUBY_METHOD_FUNC(gdip_enumint_to_i), 0);
    rb_define_method(cEnumInt, "to_int", RUBY_METHOD_FUNC(gdip_enumint_to_i), 0);
    rb_define_method(cEnumInt, "|", RUBY_METHOD_FUNC(gdip_enumint_or), 1);
    rb_define_method(cEnumInt, "&", RUBY_METHOD_FUNC(gdip_enumint_and), 1);
    rb_define_method(cEnumInt, "coerce", RUBY_METHOD_FUNC(gdip_enumint_coerce), 1);

    Init_PixelFormat();
    Init_EncoderParameterValueType();
    Init_ValueType();
    Init_Encoder();
}