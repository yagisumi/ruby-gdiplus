/*
 * gdip_codec.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
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
 *   Returns a GUID string. 
 *   @return [String] Returns GUID string.<br>(e.g. +b96b3ca9-0728-11d3-9d7b-0000f81ef32e+)
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
 *   @return [String]
 */
static VALUE
gdip_guid_inspect(VALUE self)
{
    VALUE to_s = gdip_guid_to_s(self);
    return util_utf8_sprintf("<%s {%s}>", rb_obj_classname(self), RSTRING_PTR(to_s));
}

/*
 * Compares GUID structures.
 * @return [Boolean]
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
 *   Returns a hash code.
 *   @return [Integer]   
 */
static VALUE
gdip_guid_hash(VALUE self)
{
    GUID *guid = Data_Ptr<GUID *>(self);
    st_index_t hash = rb_memhash(guid, sizeof(GUID));
    return RB_INT2FIX(hash);
}

/* ImageCodecInfo*/

const rb_data_type_t tImageCodecInfo = _MAKE_DATA_TYPE(
    "ImageCodecInfo", 0, RUBY_DEFAULT_FREE, &typeddata_size<ImageCodecInfo>, NULL, &cImageCodecInfo);

static VALUE gdip_icinfo_codecname(VALUE);
static VALUE gdip_icinfo_dllname(VALUE);
static VALUE gdip_icinfo_formatdesc(VALUE);
static VALUE gdip_icinfo_filenameext(VALUE);
static VALUE gdip_icinfo_mimetype(VALUE);
static VALUE gdip_icinfo_sigpat(VALUE);
static VALUE gdip_icinfo_sigmask(VALUE);

static VALUE
gdip_icinfo_create(ImageCodecInfo *icinfo)
{
    VALUE v = typeddata_alloc<ImageCodecInfo, &tImageCodecInfo>(cImageCodecInfo);
    Data_Ptr_Set_Data(v, icinfo);
    // I don't know how long the memories of string members survive, so I will copy them.
    gdip_icinfo_codecname(v);
    gdip_icinfo_dllname(v);
    gdip_icinfo_formatdesc(v);
    gdip_icinfo_filenameext(v);
    gdip_icinfo_mimetype(v);
    gdip_icinfo_sigpat(v);
    gdip_icinfo_sigmask(v);

    return v;
}

/*
 * Returns the Guid that identifies a specific codec.
 * @return [Gdiplus::Guid]
 */
static VALUE
gdip_icinfo_clsid(VALUE self)
{
    VALUE v = rb_iv_get(self, "clsid");
    if (!RB_NIL_P(v))
        return v;
    
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    v = gdip_guid_create(&icinfo->Clsid);
    RB_OBJ_FREEZE(v);
    rb_iv_set(self, "clsid", v);
    return v;
}

/*
 * Returns the Guid that identifies the codec's format. 
 * @return [Gdiplus::Guid]
 */
static VALUE
gdip_icinfo_formatid(VALUE self)
{
    VALUE v = rb_iv_get(self, "format_id");
    if (!RB_NIL_P(v))
        return v;
    
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    v = gdip_guid_create(&icinfo->FormatID);
    RB_OBJ_FREEZE(v);
    rb_iv_set(self, "format_id", v);
    return v;
}

/*
 * Returns the name of the codec. 
 * @return [String]
 */
static VALUE
gdip_icinfo_codecname(VALUE self)
{
    VALUE v = rb_iv_get(self, "codec_name");
    if (!RB_NIL_P(v))
        return v;
    
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    v = util_utf8_str_new_from_wstr(icinfo->CodecName);
    RB_OBJ_FREEZE(v);
    rb_iv_set(self, "codec_name", v);
    return v;
}

/*
 * Returns the Dll name of the codec. 
 * @return [String]
 */
static VALUE
gdip_icinfo_dllname(VALUE self)
{
    VALUE v = rb_iv_get(self, "dll_name");
    if (!RB_NIL_P(v))
        return v;
    
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    v = util_utf8_str_new_from_wstr(icinfo->DllName);
    RB_OBJ_FREEZE(v);
    rb_iv_set(self, "dll_name", v);
    return v;
}

/*
 * Returns the format description. 
 * @return [String]
 */
static VALUE
gdip_icinfo_formatdesc(VALUE self)
{
    VALUE v = rb_iv_get(self, "format_desc");
    if (!RB_NIL_P(v))
        return v;
    
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    v = util_utf8_str_new_from_wstr(icinfo->FormatDescription);
    RB_OBJ_FREEZE(v);
    rb_iv_set(self, "format_desc", v);
    return v;
}

/*
 * Returns the file name extensions. 
 * @return [String]
 */
static VALUE
gdip_icinfo_filenameext(VALUE self)
{
    VALUE v = rb_iv_get(self, "filename_ext");
    if (!RB_NIL_P(v))
        return v;
    
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    v = util_utf8_str_new_from_wstr(icinfo->FilenameExtension);
    RB_OBJ_FREEZE(v);
    rb_iv_set(self, "filename_ext", v);
    return v;
}

/*
 * Returns the MIME type. 
 * @return [String]
 */
static VALUE
gdip_icinfo_mimetype(VALUE self)
{
    VALUE v = rb_iv_get(self, "mime_type");
    if (!RB_NIL_P(v))
        return v;
    
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    v = util_utf8_str_new_from_wstr(icinfo->MimeType);
    RB_OBJ_FREEZE(v);
    rb_iv_set(self, "mime_type", v);
    return v;
}

/*
 * Returns flags. 
 * @return [Integer]
 */
static VALUE
gdip_icinfo_flags(VALUE self)
{
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    return UINT2NUM(icinfo->Flags);
}

/*
 * Returns the version number. 
 * @return [Integer]
 */
static VALUE
gdip_icinfo_version(VALUE self)
{
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    return UINT2NUM(icinfo->Version);
}

/*
 * @return [String]
 */
static VALUE
gdip_icinfo_inspect(VALUE self)
{
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    VALUE clsid = gdip_icinfo_clsid(self);
    VALUE clsid_str = gdip_guid_to_s(clsid);
    VALUE formatid = gdip_icinfo_formatid(self);
    VALUE formatid_str = gdip_guid_to_s(formatid);
    VALUE dllname = gdip_icinfo_dllname(self);
    VALUE codecname = gdip_icinfo_codecname(self);
    VALUE formatdesc = gdip_icinfo_formatdesc(self);
    VALUE ext = gdip_icinfo_filenameext(self);
    VALUE mime = gdip_icinfo_mimetype(self);

    return util_utf8_sprintf(
        "<%s Clsid={%s} FormatID={%s} CodecName=\"%s\" DllName=\"%s\" FormatDescription=\"%s\" FilenameExtension=\"%s\" MimeType=\"%s\" Flags=0x%08lx, Version=%d>", 
        rb_obj_classname(self), RSTRING_PTR(clsid_str), RSTRING_PTR(formatid_str),
        RSTRING_PTR(codecname), RSTRING_PTR(dllname), RSTRING_PTR(formatdesc), RSTRING_PTR(ext), RSTRING_PTR(mime),
        icinfo->Flags, icinfo->Version);
}

/*
 * Returns Signiture Patterns.
 * @return [Array<String>] (e.g. <code>["GIF89a", "GIF87a"]</code>)
 */
static VALUE
gdip_icinfo_sigpat(VALUE self)
{
    VALUE v = rb_iv_get(self, "sigpat");
    if (!RB_NIL_P(v))
        return v;
    
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    v = rb_ary_new();
    if (icinfo->SigPattern != NULL) {
        for (unsigned int i = 0; i < icinfo->SigCount; i++) {
            VALUE s = rb_str_new(reinterpret_cast<char *>(&(icinfo->SigPattern[icinfo->SigSize * i])), icinfo->SigSize);
            RB_OBJ_FREEZE(s);
            rb_ary_push(v, s);
        }
        
    }
    RB_OBJ_FREEZE(v);
    rb_iv_set(self, "sigpat", v);
    return v;
}

/*
 * Returns Signiture Masks.
 * @return [Array<String>] (e.g. <code>["\xFF\xFF\xFF\xFF\xFF\xFF", "\xFF\xFF\xFF\xFF\xFF\xFF"]</code>)
 */
static VALUE
gdip_icinfo_sigmask(VALUE self)
{
    VALUE v = rb_iv_get(self, "sigmask");
    if (!RB_NIL_P(v))
        return v;
    
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    VALUE ary = rb_ary_new();
    if (icinfo->SigMask != NULL) {
        for (unsigned int i = 0; i < icinfo->SigCount; i++) {
            VALUE s = rb_str_new(reinterpret_cast<char *>(&(icinfo->SigMask[icinfo->SigSize * i])), icinfo->SigSize);
            RB_OBJ_FREEZE(s);
            rb_ary_push(ary, s);
        }
        
    }
    RB_OBJ_FREEZE(v);
    rb_iv_set(self, "sigmask", v);
    return v;
}

/*
 * Returns the image encoders.
 * @return [Array<Gdiplus::ImageCodecInfo>]
 */
static VALUE
gdip_icinfo_s_image_encoders(VALUE self)
{
    UINT num = 0;
    UINT size = 0;
    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0) { return rb_ary_new(); }

    pImageCodecInfo = static_cast<ImageCodecInfo*>(ruby_xmalloc(size));
    GetImageEncoders(num, size, pImageCodecInfo);

    VALUE encoders = rb_ary_new_capa(num);
    for (UINT i=0; i < num; i++) {
        VALUE icinfo = gdip_icinfo_create(&(pImageCodecInfo[i]));
        rb_ary_push(encoders, icinfo);
        RB_GC_GUARD(icinfo);
    }
    ruby_xfree(pImageCodecInfo);

    return encoders;
}

/*
 * Returns the image decoders.
 * @return [Array<Gdiplus::ImageCodecInfo>]
 */
static VALUE
gdip_icinfo_s_image_decoders(VALUE self)
{
    UINT num = 0;
    UINT size = 0;
    ImageCodecInfo *pImageCodecInfo = NULL;

    GetImageDecodersSize(&num, &size);
    if (size == 0) { return rb_ary_new(); }

    pImageCodecInfo = static_cast<ImageCodecInfo*>(ruby_xmalloc(size));
    GetImageDecoders(num, size, pImageCodecInfo);

    VALUE encoders = rb_ary_new_capa(num);
    for (UINT i=0; i < num; i++) {
        rb_ary_push(encoders, gdip_icinfo_create(&pImageCodecInfo[i]));
    }
    ruby_xfree(pImageCodecInfo);

    return encoders;
}



/*
Document-class: Gdiplus::Guid
This class wraps a GUID structure.
@example
  guid = Gdiplus::Guid.new("557cf400-1a04-11d3-9a73-0000f81ef32e")
*/

/*
Document-class: Gdiplus::ImageCodecInfo
This class wraps a ImageCodecInfo structure.
@example
  Gdiplus::ImageCodecInfo.GetImageEncoders.each {|icinfo|
    p [icinfo.FormatDescription, icinfo.MimeType]
  }
*/
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
    
    // ImageCodecInfo
    cImageCodecInfo = rb_define_class_under(mGdiplus, "ImageCodecInfo", rb_cObject);
    //rb_define_alloc_func(cImageCodecInfo, &typeddata_alloc<ImageCodecInfo, &tImageCodecInfo>);
    rb_undef_alloc_func(cImageCodecInfo);
    rb_define_method(cImageCodecInfo, "clsid", RUBY_METHOD_FUNC(gdip_icinfo_clsid), 0);
    rb_define_method(cImageCodecInfo, "Clsid", RUBY_METHOD_FUNC(gdip_icinfo_clsid), 0);
    rb_define_method(cImageCodecInfo, "format_id", RUBY_METHOD_FUNC(gdip_icinfo_formatid), 0);
    rb_define_method(cImageCodecInfo, "FormatID", RUBY_METHOD_FUNC(gdip_icinfo_formatid), 0);
    rb_define_method(cImageCodecInfo, "codec_name", RUBY_METHOD_FUNC(gdip_icinfo_codecname), 0);
    rb_define_method(cImageCodecInfo, "CodecName", RUBY_METHOD_FUNC(gdip_icinfo_codecname), 0);
    rb_define_method(cImageCodecInfo, "dll_name", RUBY_METHOD_FUNC(gdip_icinfo_dllname), 0);
    rb_define_method(cImageCodecInfo, "DllName", RUBY_METHOD_FUNC(gdip_icinfo_dllname), 0);
    rb_define_method(cImageCodecInfo, "format_description", RUBY_METHOD_FUNC(gdip_icinfo_formatdesc), 0);
    rb_define_method(cImageCodecInfo, "FormatDescription", RUBY_METHOD_FUNC(gdip_icinfo_formatdesc), 0);
    rb_define_method(cImageCodecInfo, "filename_extension", RUBY_METHOD_FUNC(gdip_icinfo_filenameext), 0);
    rb_define_method(cImageCodecInfo, "FilenameExtension", RUBY_METHOD_FUNC(gdip_icinfo_filenameext), 0);
    rb_define_method(cImageCodecInfo, "mime_type", RUBY_METHOD_FUNC(gdip_icinfo_mimetype), 0);
    rb_define_method(cImageCodecInfo, "MimeType", RUBY_METHOD_FUNC(gdip_icinfo_mimetype), 0);
    rb_define_method(cImageCodecInfo, "version", RUBY_METHOD_FUNC(gdip_icinfo_version), 0);
    rb_define_method(cImageCodecInfo, "Version", RUBY_METHOD_FUNC(gdip_icinfo_version), 0);
    rb_define_method(cImageCodecInfo, "flags", RUBY_METHOD_FUNC(gdip_icinfo_flags), 0);
    rb_define_method(cImageCodecInfo, "Flags", RUBY_METHOD_FUNC(gdip_icinfo_flags), 0);
    rb_define_method(cImageCodecInfo, "signature_patterns", RUBY_METHOD_FUNC(gdip_icinfo_sigpat), 0);
    rb_define_method(cImageCodecInfo, "SignaturePatterns", RUBY_METHOD_FUNC(gdip_icinfo_sigpat), 0);
    rb_define_method(cImageCodecInfo, "signature_masks", RUBY_METHOD_FUNC(gdip_icinfo_sigmask), 0);
    rb_define_method(cImageCodecInfo, "SignatureMasks", RUBY_METHOD_FUNC(gdip_icinfo_sigmask), 0);
    rb_define_method(cImageCodecInfo, "inspect", RUBY_METHOD_FUNC(gdip_icinfo_inspect), 0);

    rb_define_singleton_method(cImageCodecInfo, "image_encoders", RUBY_METHOD_FUNC(gdip_icinfo_s_image_encoders), 0);
    rb_define_singleton_method(cImageCodecInfo, "GetImageEncoders", RUBY_METHOD_FUNC(gdip_icinfo_s_image_encoders), 0);
    rb_define_singleton_method(cImageCodecInfo, "image_decoders", RUBY_METHOD_FUNC(gdip_icinfo_s_image_decoders), 0);
    rb_define_singleton_method(cImageCodecInfo, "GetImageDecoders", RUBY_METHOD_FUNC(gdip_icinfo_s_image_decoders), 0);
}