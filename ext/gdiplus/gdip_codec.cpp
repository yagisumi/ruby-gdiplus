/*
 * gdip_codec.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * This software is released under the MIT License.
 */
#include "ruby_gdiplus.h"



const rb_data_type_t tGuid = _MAKE_DATA_TYPE(
    "Guid", 0, RUBY_DEFAULT_FREE, &typeddata_size<GUID>, NULL, &cGuid);

static VALUE
gdip_guid_create(GUID *guid)
{
    VALUE v = typeddata_alloc<GUID, &tGuid>(cGuid);
    Data_Ptr_Set_Data(v, guid);
    return v;
}

/*
 * @overload initialize(str)
 *   @param str [String] GUID string. Exclude braces. <br>(e.g. +\"b96b3ca9-0728-11d3-9d7b-0000f81ef32e\"+)
 *
 */
static VALUE
gdip_guid_init(VALUE self, VALUE str)
{
    Check_Type(str, RUBY_T_STRING);
    GUID *guid = Data_Ptr<GUID *>(self);
    if (RPC_S_OK != UuidFromStringA(reinterpret_cast<unsigned char *>(RSTRING_PTR(str)), guid)) {
        rb_raise(eGdiplus, "Guid parse error: '%s'", RSTRING_PTR(str));
    }
    return Qnil;
}

/*
 * @overload to_s()
 *   Returns GUID string. 
 *   @return [String] Returns GUID string.<br>
 *     (e.g. +b96b3ca9-0728-11d3-9d7b-0000f81ef32e+)
 */
static VALUE
gdip_guid_to_s(VALUE self)
{
    GUID *guid = Data_Ptr<GUID *>(self);
    VALUE str = Qnil;
    unsigned char *cstr; // RPC_CSTR
    if (RPC_S_OK == ::UuidToStringA(guid, &cstr)) {
        str = rb_utf8_str_new_cstr(reinterpret_cast<char *>(cstr));
        RpcStringFree(&cstr);
    }
    else {
        str = rb_str_new_cstr("");
    }
    return str;
}

/*
 * @overload c_struct()
 *   Returns C struct initializer of GUID.
 *   @return [String] C struct initializer of GUID.<br>
 *     (e.g. <code>{0xb96b3ca9, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}}</code>)
*/
static VALUE
gdip_guid_cstruct(VALUE self)
{
    GUID *guid = Data_Ptr<GUID *>(self);
    return util_utf8_sprintf(
        "{0x%08lx, 0x%04x, 0x%04x, {0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x}}", 
        guid->Data1, guid->Data2, guid->Data3, 
        guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3], 
        guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]);
}

/*
 * @overload inspect
 *   @return [String] c struct initializer of GUID.
 */
static VALUE
gdip_guid_inspect(VALUE self)
{
    VALUE to_s = gdip_guid_to_s(self);
    return util_utf8_sprintf("<%s {%s}>", rb_obj_classname(self), RSTRING_PTR(to_s));
}

/*
 * Compares GUID structure.
 * @!method ==(other)
 *   @return [Boolean]
 * Compares GUID structure.
 * @!method eql?(other)
 *   @return [Boolean]
 */
static VALUE
gdip_guid_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    if (!rb_typeddata_is_kind_of(other, &tGuid)) return Qfalse;
    if (memcmp(_DATA_PTR(self), _DATA_PTR(other), sizeof(GUID)) == 0) return Qtrue;
    else return Qfalse;
}

/*
 * @overload hash
 *   Returns hash code.
 *   @return [Integer]   
 */
static VALUE
gdip_guid_hash(VALUE self)
{
    GUID *guid = Data_Ptr<GUID *>(self);
    st_index_t hash = rb_memhash(guid, sizeof(GUID));
    return RB_INT2FIX(hash);
}

void
Init_codec()
{
    // GUID
    cGuid = rb_define_class_under(mGdiplus, "Guid", rb_cObject);
    rb_define_alloc_func(cGuid, &typeddata_alloc<GUID, &tGuid>);
    rb_define_method(cGuid, "initialize", RUBY_METHOD_FUNC(gdip_guid_init), 1);
    rb_define_method(cGuid, "to_s", RUBY_METHOD_FUNC(gdip_guid_to_s), 0);
    rb_define_method(cGuid, "inspect", RUBY_METHOD_FUNC(gdip_guid_inspect), 0);
    rb_define_method(cGuid, "c_struct", RUBY_METHOD_FUNC(gdip_guid_cstruct), 0);
    rb_define_method(cGuid, "==", RUBY_METHOD_FUNC(gdip_guid_equal), 1);
    rb_define_method(cGuid, "eql?", RUBY_METHOD_FUNC(gdip_guid_equal), 1);
    rb_define_method(cGuid, "hash", RUBY_METHOD_FUNC(gdip_guid_hash), 0);
    
}