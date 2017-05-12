/*
 * gdip_codec.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"


const rb_data_type_t tGuid = _MAKE_DATA_TYPE(
    "Guid", 0, GDIP_DEFAULT_FREE(GUID), &typeddata_size<GUID>, NULL, &cGuid);

VALUE
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
    if (!_KIND_OF(other, tGuid)) return Qfalse;
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

class gdipImageCodecInfo : public ImageCodecInfo {
public:
    VALUE _clsid;
    VALUE _formatid;
    VALUE _codecname;
    VALUE _dllname;
    VALUE _filenameext;
    VALUE _formatdesc;
    VALUE _mimetype;
    VALUE _sigpat;
    VALUE _sigmask;
};

static void
gdip_icinfo_mark(gdipImageCodecInfo *icinfo)
{
    if (icinfo->_clsid) rb_gc_mark(icinfo->_clsid);
    if (icinfo->_formatid) rb_gc_mark(icinfo->_formatid);
    if (icinfo->_codecname) rb_gc_mark(icinfo->_codecname);
    if (icinfo->_dllname) rb_gc_mark(icinfo->_dllname);
    if (icinfo->_filenameext) rb_gc_mark(icinfo->_filenameext);
    if (icinfo->_formatdesc) rb_gc_mark(icinfo->_formatdesc);
    if (icinfo->_mimetype) rb_gc_mark(icinfo->_mimetype);
    if (icinfo->_sigpat) rb_gc_mark(icinfo->_sigpat);
    if (icinfo->_sigmask) rb_gc_mark(icinfo->_sigmask);
}

const rb_data_type_t tImageCodecInfo = _MAKE_DATA_TYPE(
    "ImageCodecInfo", RUBY_DATA_FUNC(gdip_icinfo_mark), GDIP_DEFAULT_FREE(gdipImageCodecInfo), &typeddata_size<ImageCodecInfo>, NULL, &cImageCodecInfo);

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
    VALUE v = typeddata_alloc<gdipImageCodecInfo, &tImageCodecInfo>(cImageCodecInfo);
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
    gdipImageCodecInfo *icinfo = Data_Ptr<gdipImageCodecInfo *>(self);
    if (icinfo->_clsid) return icinfo->_clsid;
    
    VALUE v = gdip_guid_create(&icinfo->Clsid);
    RB_OBJ_FREEZE(v);
    icinfo->_clsid = v;
    return v;
}

/*
 * Returns the Guid that identifies the codec's format. 
 * @return [Gdiplus::Guid]
 */
static VALUE
gdip_icinfo_formatid(VALUE self)
{
    gdipImageCodecInfo *icinfo = Data_Ptr<gdipImageCodecInfo *>(self);
    if (icinfo->_formatid) return icinfo->_formatid;
    
    VALUE v = gdip_guid_create(&icinfo->FormatID);
    RB_OBJ_FREEZE(v);
    icinfo->_formatid = v;
    return v;
}

/*
 * Returns the name of the codec. 
 * @return [String]
 */
static VALUE
gdip_icinfo_codecname(VALUE self)
{
    gdipImageCodecInfo *icinfo = Data_Ptr<gdipImageCodecInfo *>(self);
    if (icinfo->_codecname) return icinfo->_codecname;
    
    VALUE v = util_utf8_str_new_from_wstr(icinfo->CodecName);
    RB_OBJ_FREEZE(v);
    icinfo->_codecname = v;
    return v;
}

/*
 * Returns the Dll name of the codec. 
 * @return [String]
 */
static VALUE
gdip_icinfo_dllname(VALUE self)
{
    gdipImageCodecInfo *icinfo = Data_Ptr<gdipImageCodecInfo *>(self);
    if (icinfo->_dllname) return icinfo->_dllname;
    
    VALUE v = util_utf8_str_new_from_wstr(icinfo->DllName);
    RB_OBJ_FREEZE(v);
    icinfo->_dllname = v;
    return v;
}

/*
 * Returns the format description. 
 * @return [String]
 */
static VALUE
gdip_icinfo_formatdesc(VALUE self)
{
    gdipImageCodecInfo *icinfo = Data_Ptr<gdipImageCodecInfo *>(self);
    if (icinfo->_formatdesc) return icinfo->_formatdesc;
    
    VALUE v = util_utf8_str_new_from_wstr(icinfo->FormatDescription);
    RB_OBJ_FREEZE(v);
    icinfo->_formatdesc = v;
    return v;
}

/*
 * Returns the file name extensions. 
 * @return [String]
 */
static VALUE
gdip_icinfo_filenameext(VALUE self)
{
    gdipImageCodecInfo *icinfo = Data_Ptr<gdipImageCodecInfo *>(self);
    if (icinfo->_filenameext) return icinfo->_filenameext;
    
    VALUE v = util_utf8_str_new_from_wstr(icinfo->FilenameExtension);
    RB_OBJ_FREEZE(v);
    icinfo->_filenameext = v;
    return v;
}

/*
 * Returns the MIME type. 
 * @return [String]
 */
static VALUE
gdip_icinfo_mimetype(VALUE self)
{
    gdipImageCodecInfo *icinfo = Data_Ptr<gdipImageCodecInfo *>(self);
    if (icinfo->_mimetype) return icinfo->_mimetype;
    
    VALUE v = util_utf8_str_new_from_wstr(icinfo->MimeType);
    RB_OBJ_FREEZE(v);
    icinfo->_mimetype = v;
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
    return RB_UINT2NUM(icinfo->Flags);
}

/*
 * Returns the version number. 
 * @return [Integer]
 */
static VALUE
gdip_icinfo_version(VALUE self)
{
    ImageCodecInfo *icinfo = Data_Ptr<ImageCodecInfo *>(self);
    return RB_UINT2NUM(icinfo->Version);
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
    gdipImageCodecInfo *icinfo = Data_Ptr<gdipImageCodecInfo *>(self);
    if (icinfo->_sigpat) return icinfo->_sigpat;
    
    VALUE v = rb_ary_new();
    if (icinfo->SigPattern != NULL) {
        for (unsigned int i = 0; i < icinfo->SigCount; i++) {
            VALUE s = rb_str_new(reinterpret_cast<char *>(&(icinfo->SigPattern[icinfo->SigSize * i])), icinfo->SigSize);
            RB_OBJ_FREEZE(s);
            rb_ary_push(v, s);
        }
    }
    RB_OBJ_FREEZE(v);
    icinfo->_sigpat = v;
    return v;
}

/*
 * Returns Signiture Masks.
 * @return [Array<String>] (e.g. <code>["\xFF\xFF\xFF\xFF\xFF\xFF", "\xFF\xFF\xFF\xFF\xFF\xFF"]</code>)
 */
static VALUE
gdip_icinfo_sigmask(VALUE self)
{
    gdipImageCodecInfo *icinfo = Data_Ptr<gdipImageCodecInfo *>(self);
    if (icinfo->_sigmask) return icinfo->_sigmask;
    
    VALUE v = rb_ary_new();
    if (icinfo->SigMask != NULL) {
        for (unsigned int i = 0; i < icinfo->SigCount; i++) {
            VALUE s = rb_str_new(reinterpret_cast<char *>(&(icinfo->SigMask[icinfo->SigSize * i])), icinfo->SigSize);
            RB_OBJ_FREEZE(s);
            rb_ary_push(v, s);
        }
    }
    RB_OBJ_FREEZE(v);
    icinfo->_sigmask = v;
    return v;
}

static VALUE encoders = Qnil;
static VALUE decoders = Qnil;

/*
 * Returns the image encoders.
 * @return [Array<Gdiplus::ImageCodecInfo>]
 */
static VALUE
gdip_icinfo_s_image_encoders(VALUE self)
{
    if (!RB_NIL_P(encoders)) return encoders;

    UINT num = 0;
    UINT size = 0;
    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0) { return rb_ary_new(); }

    pImageCodecInfo = static_cast<ImageCodecInfo*>(ruby_xmalloc(size));
    GetImageEncoders(num, size, pImageCodecInfo);

    VALUE ary = rb_ary_new_capa(num);
    for (UINT i=0; i < num; i++) {
        VALUE icinfo = gdip_icinfo_create(&(pImageCodecInfo[i]));
        rb_ary_push(ary, icinfo);
    }
    ruby_xfree(pImageCodecInfo);
    encoders = ary;
    rb_gc_register_address(&encoders);

    return encoders;
}

/*
 * Returns the image decoders.
 * @return [Array<Gdiplus::ImageCodecInfo>]
 */
static VALUE
gdip_icinfo_s_image_decoders(VALUE self)
{
    if (!RB_NIL_P(decoders)) return decoders;

    UINT num = 0;
    UINT size = 0;
    ImageCodecInfo *pImageCodecInfo = NULL;

    GetImageDecodersSize(&num, &size);
    if (size == 0) { return rb_ary_new(); }

    pImageCodecInfo = static_cast<ImageCodecInfo*>(ruby_xmalloc(size));
    GetImageDecoders(num, size, pImageCodecInfo);

    VALUE ary = rb_ary_new_capa(num);
    for (UINT i=0; i < num; i++) {
        VALUE icinfo = gdip_icinfo_create(&(pImageCodecInfo[i]));
        rb_ary_push(ary, icinfo);
    }
    ruby_xfree(pImageCodecInfo);
    decoders = ary;
    rb_gc_register_address(&decoders);

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
    

    cImageCodecInfo = rb_define_class_under(mGdiplus, "ImageCodecInfo", rb_cObject);
    //rb_define_alloc_func(cImageCodecInfo, &typeddata_alloc<gdipImageCodecInfo, &tImageCodecInfo>);
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