/*
 * gdip_enum.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
 #include "ruby_gdiplus.h"

static ID ID_UNKNOWN;

template <typename KEY>
class TableMapBase {
public:
    virtual bool append(KEY key, ID id) = 0;
    virtual bool lookup(KEY key, ID& id) = 0;
};

template <typename KEY>
class TableMap : public TableMapBase<KEY> {
public:
    int Capa;
    int Len;
    KEY *KeyTable;
    ID *IdTable;
    TableMap(int capa) {
        Capa = capa;
        Len = 0;
        dp("TableMap: new");
        KeyTable = static_cast<KEY *>(RB_ZALLOC_N(KEY, capa));
        IdTable = static_cast<ID *>(RB_ZALLOC_N(ID, capa));
    }
    virtual ~TableMap() {
        dp("TableMap: delete");
        ruby_xfree(KeyTable);
        ruby_xfree(IdTable);
    }
    virtual bool append(KEY key, ID val) {
        if (Len < Capa) {
            KeyTable[Len] = key;
            IdTable[Len] = val;
            Len += 1;
            return true;
        }
        else {
            rb_warning("TableMap: capacity over");
            return false;
        }
    }
    virtual bool lookup(KEY key, ID& id) {
        for (int i = 0; i < Len; ++i) {
            if (key == KeyTable[i]) {
                id = IdTable[i];
                return true;
            }
        }
        id = ID_UNKNOWN;
        return false;
    }
};

template <typename KEY>
class KeyPtrTableMap : public TableMap<KEY> {
public:
    KeyPtrTableMap(int capa) : TableMap<KEY>(capa) {}
    virtual bool lookup(KEY key, ID& id) {
        for (int i = 0; i < this->Len; ++i) {
            if (memcmp(key, this->KeyTable[i], sizeof(*key)) == 0) {
                id = this->IdTable[i];
                return true;
            }
        }
        id = ID_UNKNOWN;
        return false;
    }
};

VALUE
gdip_enum_const_get(VALUE self)
{
    return rb_const_get(self, rb_frame_this_func());
}

static VALUE
gdip_enumint_create(const rb_data_type_t *type, int num)
{
    return _Data_Wrap_Struct(_KLASS(type), type, reinterpret_cast<void*>(num));
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

template <typename T>
static VALUE
gdip_enumptr_create(const rb_data_type_t *type, T data, VALUE klass=Qnil)
{
    T ptr = static_cast<T>(ruby_xcalloc(1, sizeof(*data)));
    *ptr = *data;
    if (RB_NIL_P(klass)) klass = _KLASS(type);
    return _Data_Wrap_Struct(klass, type, ptr);
}

template <typename KEY>
static void
gdip_enum_define(VALUE klass, TableMapBase<KEY> *table, const char *name, KEY data, VALUE v)
{
    ID id = rb_intern(name);
    rb_define_const(klass, name, v);
    rb_define_singleton_method(klass, name, RUBY_METHOD_FUNC(gdip_enum_const_get), 0);
    table->append(data, id);
}

static VALUE
gdip_enumint_inspect(VALUE self, TableMap<int> *table)
{
    ID id;
    int num = Data_Ptr_As<int>(self);
    table->lookup(num, id);
    return util_utf8_sprintf("<%s.%s: %08x>", __class__(self), rb_id2name(id), num);
}

const rb_data_type_t tPixelFormat = _MAKE_DATA_TYPE(
    "PixelFormat", 0, RUBY_NEVER_FREE, NULL, NULL, &cPixelFormat);

static TableMap<int> table_pxlfmt(25);

static VALUE
gdip_pxlfmt_inspect(VALUE self)
{
    return gdip_enumint_inspect(self, &table_pxlfmt);
}

VALUE
gdip_pxlfmt_get(PixelFormat n)
{
    ID id;
    if(table_pxlfmt.lookup(n, id)) {
        return rb_const_get(cPixelFormat, id);
    }
    else {
        return gdip_enumint_create(&tPixelFormat, n);
    }
}

#define define_enumint(_type_, table, name, num) gdip_enum_define<int>(c##_type_, table, name, num, gdip_enumint_create(&t##_type_, num))

static void
Init_PixelFormat()
{
    cPixelFormat = rb_define_class_under(mGdiplus, "PixelFormat", cEnumInt);
    rb_undef_alloc_func(cPixelFormat);
    rb_define_method(cPixelFormat, "inspect", RUBY_METHOD_FUNC(gdip_pxlfmt_inspect), 0);

    define_enumint(PixelFormat, &table_pxlfmt, "Format32bppARGB", PixelFormat32bppARGB);
    define_enumint(PixelFormat, &table_pxlfmt, "Format24bppRGB", PixelFormat24bppRGB);
    define_enumint(PixelFormat, &table_pxlfmt, "Format32bppRGB", PixelFormat32bppRGB);
    define_enumint(PixelFormat, &table_pxlfmt, "Format32bppPARGB", PixelFormat32bppPARGB);
    define_enumint(PixelFormat, &table_pxlfmt, "Format48bppRGB", PixelFormat48bppRGB);
    define_enumint(PixelFormat, &table_pxlfmt, "Format64bppARGB", PixelFormat64bppARGB);
    define_enumint(PixelFormat, &table_pxlfmt, "Format64bppPARGB", PixelFormat64bppPARGB);
    define_enumint(PixelFormat, &table_pxlfmt, "Format32bppCMYK", PixelFormat32bppCMYK);
    define_enumint(PixelFormat, &table_pxlfmt, "Format1bppIndexed", PixelFormat1bppIndexed);
    define_enumint(PixelFormat, &table_pxlfmt, "Format4bppIndexed", PixelFormat4bppIndexed);
    define_enumint(PixelFormat, &table_pxlfmt, "Format8bppIndexed", PixelFormat8bppIndexed);
    define_enumint(PixelFormat, &table_pxlfmt, "Format16bppGrayScale", PixelFormat16bppGrayScale);
    define_enumint(PixelFormat, &table_pxlfmt, "Format16bppRGB555", PixelFormat16bppRGB555);
    define_enumint(PixelFormat, &table_pxlfmt, "Format16bppRGB565", PixelFormat16bppRGB565);
    define_enumint(PixelFormat, &table_pxlfmt, "Format16bppARGB1555", PixelFormat16bppARGB1555);
    define_enumint(PixelFormat, &table_pxlfmt, "Undefined", PixelFormatUndefined);
    define_enumint(PixelFormat, &table_pxlfmt, "Indexed", PixelFormatIndexed);
    define_enumint(PixelFormat, &table_pxlfmt, "GDI", PixelFormatGDI);
    define_enumint(PixelFormat, &table_pxlfmt, "Gdi", PixelFormatGDI);
    define_enumint(PixelFormat, &table_pxlfmt, "Alpha", PixelFormatAlpha);
    define_enumint(PixelFormat, &table_pxlfmt, "PAlpha", PixelFormatPAlpha);
    define_enumint(PixelFormat, &table_pxlfmt, "Extended", PixelFormatExtended);
    define_enumint(PixelFormat, &table_pxlfmt, "Canonical", PixelFormatCanonical);
    define_enumint(PixelFormat, &table_pxlfmt, "Max", PixelFormatMax);
    define_enumint(PixelFormat, &table_pxlfmt, "DontCare", PixelFormatDontCare);
}

const rb_data_type_t tEncoderParameterValueType = _MAKE_DATA_TYPE(
    "EncoderParameterValueType", 0, RUBY_NEVER_FREE, NULL, NULL, &cEncoderParameterValueType);

static TableMap<int> table_eprmvt(25);

static VALUE
gdip_eprmvt_inspect(VALUE self)
{
    return gdip_enumint_inspect(self, &table_eprmvt);
}

VALUE 
gdip_eprmvt_get(int n)
{
    ID id;
    if(table_eprmvt.lookup(n, id)) {
        return rb_const_get(cEncoderParameterValueType, id);
    }
    else {
        return gdip_enumint_create(&tEncoderParameterValueType, n);
    }
}

static void
Init_EncoderParameterValueType()
{
    cEncoderParameterValueType = rb_define_class_under(mGdiplus, "EncoderParameterValueType", cEnumInt);
    rb_undef_alloc_func(cEncoderParameterValueType);
    rb_define_method(cEncoderParameterValueType, "inspect", RUBY_METHOD_FUNC(gdip_eprmvt_inspect), 0);

    define_enumint(EncoderParameterValueType, &table_eprmvt, "ValueTypeByte", EncoderParameterValueTypeByte);
    define_enumint(EncoderParameterValueType, &table_eprmvt, "ValueTypeASCII", EncoderParameterValueTypeASCII);
    define_enumint(EncoderParameterValueType, &table_eprmvt, "ValueTypeShort", EncoderParameterValueTypeShort);
    define_enumint(EncoderParameterValueType, &table_eprmvt, "ValueTypeLong", EncoderParameterValueTypeLong);
    define_enumint(EncoderParameterValueType, &table_eprmvt, "ValueTypeRational", EncoderParameterValueTypeRational);
    define_enumint(EncoderParameterValueType, &table_eprmvt, "ValueTypeLongRange", EncoderParameterValueTypeLongRange);
    define_enumint(EncoderParameterValueType, &table_eprmvt, "ValueTypeUndefined", EncoderParameterValueTypeUndefined);
    define_enumint(EncoderParameterValueType, &table_eprmvt, "ValueTypeRationalRange", EncoderParameterValueTypeRationalRange);
    define_enumint(EncoderParameterValueType, &table_eprmvt, "ValueTypePointer", EncoderParameterValueTypePointer);
}

/* ValueType */
const rb_data_type_t tValueType = _MAKE_DATA_TYPE(
    "ValueType", 0, RUBY_NEVER_FREE, NULL, NULL, &cValueType);

static TableMap<int> table_valtype(26);

static VALUE
gdip_valtype_inspect(VALUE self)
{
    return gdip_enumint_inspect(self, &table_valtype);
}

static void
Init_ValueType()
{
    cValueType = rb_define_class_under(mGdiplus, "ValueType", cEnumInt);
    rb_undef_alloc_func(cValueType);
    rb_define_method(cValueType, "inspect", RUBY_METHOD_FUNC(gdip_valtype_inspect), 0);
    
    define_enumint(ValueType, &table_valtype, "ColorTypeCMYK", EncoderValueColorTypeCMYK);
    define_enumint(ValueType, &table_valtype, "ColorTypeYCCK", EncoderValueColorTypeYCCK);
    define_enumint(ValueType, &table_valtype, "CompressionLZW", EncoderValueCompressionLZW);
    define_enumint(ValueType, &table_valtype, "CompressionCCITT3", EncoderValueCompressionCCITT3);
    define_enumint(ValueType, &table_valtype, "CompressionCCITT4", EncoderValueCompressionCCITT4);
    define_enumint(ValueType, &table_valtype, "CompressionRle", EncoderValueCompressionRle);
    define_enumint(ValueType, &table_valtype, "CompressionNone", EncoderValueCompressionNone);
    define_enumint(ValueType, &table_valtype, "ScanMethodInterlaced", EncoderValueScanMethodInterlaced);
    define_enumint(ValueType, &table_valtype, "ScanMethodNonInterlaced", EncoderValueScanMethodNonInterlaced);
    define_enumint(ValueType, &table_valtype, "VersionGif87", EncoderValueVersionGif87);
    define_enumint(ValueType, &table_valtype, "VersionGif89", EncoderValueVersionGif89);
    define_enumint(ValueType, &table_valtype, "RenderProgressive", EncoderValueRenderProgressive);
    define_enumint(ValueType, &table_valtype, "RenderNonProgressive", EncoderValueRenderNonProgressive);
    define_enumint(ValueType, &table_valtype, "TransformRotate90", EncoderValueTransformRotate90);
    define_enumint(ValueType, &table_valtype, "TransformRotate180", EncoderValueTransformRotate180);
    define_enumint(ValueType, &table_valtype, "TransformRotate270", EncoderValueTransformRotate270);
    define_enumint(ValueType, &table_valtype, "TransformFlipHorizontal", EncoderValueTransformFlipHorizontal);
    define_enumint(ValueType, &table_valtype, "TransformFlipVertical", EncoderValueTransformFlipVertical);
    define_enumint(ValueType, &table_valtype, "MultiFrame", EncoderValueMultiFrame);
    define_enumint(ValueType, &table_valtype, "LastFrame", EncoderValueLastFrame);
    define_enumint(ValueType, &table_valtype, "Flush", EncoderValueFlush);
    define_enumint(ValueType, &table_valtype, "FrameDimensionTime", EncoderValueFrameDimensionTime);
    define_enumint(ValueType, &table_valtype, "FrameDimensionResolution", EncoderValueFrameDimensionResolution);
    define_enumint(ValueType, &table_valtype, "FrameDimensionPage", EncoderValueFrameDimensionPage);
    define_enumint(ValueType, &table_valtype, "ColorTypeGray", EncoderValueFrameDimensionPage + 1); // EncoderValueColorTypeGray
    define_enumint(ValueType, &table_valtype, "ColorTypeRGB", EncoderValueFrameDimensionPage + 2); // EncoderValueColorTypeRGB
}



/* Encoder */
static KeyPtrTableMap<GUID *> table_encoder(10);

static VALUE
gdip_encoder_inspect(VALUE self)
{
    ID id;
    VALUE r;
    GUID *guid = Data_Ptr<GUID *>(self);
    table_encoder.lookup(guid, id);
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
    ID id;
    if(table_encoder.lookup(guid, id)) {
        return rb_const_get(cEncoder, id);
    }
    else {
        return gdip_enumptr_create<GUID *>(&tGuid, guid, cEncoder);
    }
}

void
Init_Encoder()
{
    cEncoder = rb_define_class_under(mGdiplus, "Encoder", cGuid);
    rb_undef_alloc_func(cEncoder);
    rb_define_method(cEncoder, "inspect", RUBY_METHOD_FUNC(gdip_encoder_inspect), 0);
    VALUE v;

    GUID guid1 = {0xe09d739d,0xccd4,0x44ee,{0x8e,0xba,0x3f,0xbf,0x8b,0xe4,0xfc,0x58}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid1, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, &table_encoder, "Compression", Data_Ptr<GUID *>(v), v);

    GUID guid2 = {0x66087055,0xad66,0x4c7c,{0x9a,0x18,0x38,0xa2,0x31,0x0b,0x83,0x37}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid2, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, &table_encoder, "ColorDepth", Data_Ptr<GUID *>(v), v);

    GUID guid3 = {0x3a4e2661,0x3109,0x4e56,{0x85,0x36,0x42,0xc1,0x56,0xe7,0xdc,0xfa}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid3, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, &table_encoder, "ScanMethod", Data_Ptr<GUID *>(v), v);

    GUID guid4 = {0x24d18c76,0x814a,0x41a4,{0xbf,0x53,0x1c,0x21,0x9c,0xcc,0xf7,0x97}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid4, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, &table_encoder, "Version", Data_Ptr<GUID *>(v), v);

    GUID guid5 = {0x6d42c53a,0x229a,0x4825,{0x8b,0xb7,0x5c,0x99,0xe2,0xb9,0xa8,0xb8}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid5, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, &table_encoder, "RenderMethod", Data_Ptr<GUID *>(v), v);

    GUID guid6 = {0x1d5be4b5,0xfa4a,0x452d,{0x9c,0xdd,0x5d,0xb3,0x51,0x05,0xe7,0xeb}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid6, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, &table_encoder, "Quality", Data_Ptr<GUID *>(v), v);

    GUID guid7 = {0x8d0eb2d1,0xa58e,0x4ea8,{0xaa,0x14,0x10,0x80,0x74,0xb7,0xb6,0xf9}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid7, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, &table_encoder, "Transformation", Data_Ptr<GUID *>(v), v);

    GUID guid8 = {0xedb33bce,0x0266,0x4a77,{0xb9,0x04,0x27,0x21,0x60,0x99,0xe7,0x17}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid8, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, &table_encoder, "LuminanceTable", Data_Ptr<GUID *>(v), v);

    GUID guid9 = {0xf2e455dc,0x09b3,0x4316,{0x82,0x60,0x67,0x6a,0xda,0x32,0x48,0x1c}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid9, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, &table_encoder, "ChrominanceTable", Data_Ptr<GUID *>(v), v);

    GUID guid10 = {0x292266fc,0xac40,0x47bf,{0x8c, 0xfc, 0xa8, 0x5b, 0x89, 0xa6, 0x55, 0xde}};
    v = gdip_enumptr_create<GUID *>(&tGuid, &guid10, cEncoder);
    gdip_enum_define<GUID *>(cEncoder, &table_encoder, "SaveFlag", Data_Ptr<GUID *>(v), v);
}

const rb_data_type_t tEnumInt = _MAKE_DATA_TYPE(
    "EnumInt", 0, RUBY_NEVER_FREE, NULL, NULL, &cEnumInt);

void
Init_enum() {
    ID_UNKNOWN = rb_intern("__UNKNOWN__");

    cEnumInt = rb_define_class_under(mInternals, "EnumInt", rb_cObject);
    rb_define_method(cEnumInt, "to_i", RUBY_METHOD_FUNC(gdip_enumint_to_i), 0);
    rb_define_method(cEnumInt, "to_int", RUBY_METHOD_FUNC(gdip_enumint_to_i), 0);

    Init_PixelFormat();
    Init_EncoderParameterValueType();
    Init_ValueType();
    Init_Encoder();
}