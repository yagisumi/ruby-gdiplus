/*
 * gdip_enum.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

static st_table *enum_int_maps = NULL;
static ID id_enum_unknown = 0;

struct GdipEnumInt {
    ID id;
    int value;
};

const rb_data_type_t tEnumInt = _MAKE_DATA_TYPE(
    "EnumInt", 0, GDIP_DEFAULT_FREE(GdipEnumInt), &typeddata_size<GdipEnumInt>, NULL, &cEnumInt);

static void
gdip_enum_int_init()
{
    id_enum_unknown = rb_intern("__UNKNOWN__");
    dp("enum_int_maps: init");
    enum_int_maps = st_init_numtable();
}

static int
gdip_enum_int_free_i(VALUE klass, st_table* table, VALUE arg)
{
    if (table) {
        dp("enum_int_maps: inner table free");
        st_free_table(table);
    }
    return ST_CONTINUE;
}

static st_table *
gdip_enum_int_get_table(VALUE klass)
{
    st_table *table;
    if (!st_lookup(enum_int_maps, klass, (st_data_t*)&table)) {
        dp("enum_int_maps: inner table init");
        table = st_init_numtable();
        st_insert(enum_int_maps, klass, (st_data_t)table);
    }
    return table;
}

static void
gdip_enum_int_end(VALUE self)
{
    if (enum_int_maps) {
        st_foreach(enum_int_maps, _FOREACH_FUNC(gdip_enum_int_free_i), Qnil);
        dp("enum_int_maps: free");
        st_free_table(enum_int_maps);
    }
}

VALUE
gdip_enum_const_get(VALUE self)
{
    return rb_const_get(self, rb_frame_this_func());
}

/*
 * @return [String]
 */
static VALUE
gdip_enum_int_inspect(VALUE self)
{
    GdipEnumInt *enumint = Data_Ptr<GdipEnumInt *>(self);
    return util_utf8_sprintf("<%s.%s: 0x%08x>", rb_class2name(CLASS_OF(self)), rb_id2name(enumint->id), enumint->value);
}

/*
 * @return [Integer]
 */
static VALUE
gdip_enum_int_to_int(VALUE self)
{
    GdipEnumInt *enumint = Data_Ptr<GdipEnumInt *>(self);
    return RB_INT2NUM(enumint->value);
}

static VALUE
gdip_enum_int_create(VALUE klass, ID id, int num)
{
    GdipEnumInt *p = static_cast<GdipEnumInt*>(RB_ZALLOC(GdipEnumInt));
    dp("GdipEnumInt: alloc");
    p->id = id;
    p->value = num;
    return _Data_Wrap_Struct(klass, &tEnumInt, p);
}

VALUE
gdip_enum_int_num2value(VALUE klass, int num)
{
    st_table *table = gdip_enum_int_get_table(klass);
    ID id;
    if (st_lookup(table, (st_data_t)num, &id)) {
        return rb_const_get(klass, id);
    }
    else {
        return gdip_enum_int_create(klass, id_enum_unknown, num);
    }
}

int
gdip_enum_int_value2num(VALUE v)
{
    GdipEnumInt *enumint = Data_Ptr<GdipEnumInt *>(v);
    return enumint->value;
}


static void 
gdip_enum_int_define(VALUE klass, st_table *table, const char *name, int num)
{
    ID id = rb_intern(name);
    VALUE v = gdip_enum_int_create(klass, id, num);
    rb_define_const(klass, name, v);
    rb_define_singleton_method(klass, name, RUBY_METHOD_FUNC(gdip_enum_const_get), 0);
    st_insert(table, (st_data_t)num, id);
}

static void
Init_PixelFormat()
{
    cPixelFormat = rb_define_class_under(mGdiplus, "PixelFormat", cEnumInt);
    st_table *table = gdip_enum_int_get_table(cPixelFormat);

    gdip_enum_int_define(cPixelFormat, table, "Indexed", PixelFormatIndexed);
    gdip_enum_int_define(cPixelFormat, table, "GDI", PixelFormatGDI);
    gdip_enum_int_define(cPixelFormat, table, "Gdi", PixelFormatGDI);
    gdip_enum_int_define(cPixelFormat, table, "Alpha", PixelFormatAlpha);
    gdip_enum_int_define(cPixelFormat, table, "PAlpha", PixelFormatPAlpha);
    gdip_enum_int_define(cPixelFormat, table, "Extended", PixelFormatExtended);
    gdip_enum_int_define(cPixelFormat, table, "Canonical", PixelFormatCanonical);
    gdip_enum_int_define(cPixelFormat, table, "DontCare", PixelFormatDontCare);
    gdip_enum_int_define(cPixelFormat, table, "Undefined", PixelFormatUndefined);
    gdip_enum_int_define(cPixelFormat, table, "Format1bppIndexed", PixelFormat1bppIndexed);
    gdip_enum_int_define(cPixelFormat, table, "Format4bppIndexed", PixelFormat4bppIndexed);
    gdip_enum_int_define(cPixelFormat, table, "Format8bppIndexed", PixelFormat8bppIndexed);
    gdip_enum_int_define(cPixelFormat, table, "Format16bppGrayScale", PixelFormat16bppGrayScale);
    gdip_enum_int_define(cPixelFormat, table, "Format16bppRGB555", PixelFormat16bppRGB555);
    gdip_enum_int_define(cPixelFormat, table, "Format16bppRGB565", PixelFormat16bppRGB565);
    gdip_enum_int_define(cPixelFormat, table, "Format16bppARGB1555", PixelFormat16bppARGB1555);
    gdip_enum_int_define(cPixelFormat, table, "Format24bppRGB", PixelFormat24bppRGB);
    gdip_enum_int_define(cPixelFormat, table, "Format32bppRGB", PixelFormat32bppRGB);
    gdip_enum_int_define(cPixelFormat, table, "Format32bppARGB", PixelFormat32bppARGB);
    gdip_enum_int_define(cPixelFormat, table, "Format32bppPARGB", PixelFormat32bppPARGB);
    gdip_enum_int_define(cPixelFormat, table, "Format48bppRGB", PixelFormat48bppRGB);
    gdip_enum_int_define(cPixelFormat, table, "Format64bppARGB", PixelFormat64bppARGB);
    gdip_enum_int_define(cPixelFormat, table, "Format64bppPARGB", PixelFormat64bppPARGB);
    gdip_enum_int_define(cPixelFormat, table, "Format32bppCMYK", PixelFormat32bppCMYK);
    gdip_enum_int_define(cPixelFormat, table, "Max", PixelFormatMax);

    rb_undef_alloc_func(cPixelFormat);
}

static void
Init_EncoderParameterValueType()
{
    cEncoderParameterValueType = rb_define_class_under(mGdiplus, "EncoderParameterValueType", cEnumInt);
    st_table *table = gdip_enum_int_get_table(cEncoderParameterValueType);

    gdip_enum_int_define(cEncoderParameterValueType, table, "ValueTypeByte", EncoderParameterValueTypeByte);
    gdip_enum_int_define(cEncoderParameterValueType, table, "ValueTypeASCII", EncoderParameterValueTypeASCII);
    gdip_enum_int_define(cEncoderParameterValueType, table, "ValueTypeShort", EncoderParameterValueTypeShort);
    gdip_enum_int_define(cEncoderParameterValueType, table, "ValueTypeLong", EncoderParameterValueTypeLong);
    gdip_enum_int_define(cEncoderParameterValueType, table, "ValueTypeRational", EncoderParameterValueTypeRational);
    gdip_enum_int_define(cEncoderParameterValueType, table, "ValueTypeLongRange", EncoderParameterValueTypeLongRange);
    gdip_enum_int_define(cEncoderParameterValueType, table, "ValueTypeUndefined", EncoderParameterValueTypeUndefined);
    gdip_enum_int_define(cEncoderParameterValueType, table, "ValueTypeRationalRange", EncoderParameterValueTypeRationalRange);
    gdip_enum_int_define(cEncoderParameterValueType, table, "ValueTypePointer", EncoderParameterValueTypePointer);

    rb_undef_alloc_func(cEncoderParameterValueType);
}


 void
 Init_enum()
 {
    gdip_enum_int_init();

    cEnumInt = rb_define_class_under(mGdiplus, "EnumInt", rb_cObject);
    rb_undef_alloc_func(cEnumInt);
    rb_define_method(cEnumInt, "inspect", RUBY_METHOD_FUNC(gdip_enum_int_inspect), 0);
    rb_define_method(cEnumInt, "to_int", RUBY_METHOD_FUNC(gdip_enum_int_to_int), 0);
    rb_define_method(cEnumInt, "to_i", RUBY_METHOD_FUNC(gdip_enum_int_to_int), 0);

    Init_PixelFormat();
    Init_EncoderParameterValueType();


    rb_set_end_proc(gdip_enum_int_end, Qnil);
 }