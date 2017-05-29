/*
 * gdip_codec.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"


const rb_data_type_t tGuid = _MAKE_DATA_TYPE(
    "Guid", 0, GDIP_DEFAULT_FREE(GUID), &typeddata_size<GUID>, NULL, &cGuid);

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
    VALUE r = util_utf8_sprintf("#<%s {%s}>", rb_obj_classname(self), RSTRING_PTR(to_s));
    RB_GC_GUARD(to_s);
    return r;
}

/*
 * Compares GUID structures.
 * @return [Boolean]
 */
static VALUE
gdip_guid_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    if (!_KIND_OF(other, &tGuid)) return Qfalse;
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
    VALUE _encprms;
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
    if (icinfo->_encprms) rb_gc_mark(icinfo->_encprms);
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

    VALUE r = util_utf8_sprintf(
        "#<%s Clsid={%s} FormatID={%s} CodecName=\"%s\" DllName=\"%s\" FormatDescription=\"%s\" FilenameExtension=\"%s\" MimeType=\"%s\" Flags=0x%08lx, Version=%d>", 
        rb_obj_classname(self), RSTRING_PTR(clsid_str), RSTRING_PTR(formatid_str),
        RSTRING_PTR(codecname), RSTRING_PTR(dllname), RSTRING_PTR(formatdesc), RSTRING_PTR(ext), RSTRING_PTR(mime),
        icinfo->Flags, icinfo->Version);
    
    return r;
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

static VALUE gdip_encprms_create(EncoderParameters *encprms);
/**
 * Returns parameter specifications of this encoder.
 * @return [EncoderParameters]
 *
 */
static VALUE
gdip_icinfo_prmlist(VALUE self)
{
    gdipImageCodecInfo *icinfo = Data_Ptr<gdipImageCodecInfo *>(self);
    if (icinfo->_encprms) return icinfo->_encprms;
    VALUE r;
    Bitmap *bmp = new Bitmap(1, 1);
    UINT prmsize = bmp->GetEncoderParameterListSize(&icinfo->Clsid);
    if (prmsize == 0) {
        r = gdip_encprms_create(NULL);
    }
    else {
        EncoderParameters *prms = static_cast<EncoderParameters *>(ruby_xcalloc(1, prmsize));
        Status status = bmp->GetEncoderParameterList(&icinfo->Clsid, prmsize, prms);
        if (status == Ok) {
            r = gdip_encprms_create(prms);
        }
        else {
            r = gdip_encprms_create(NULL);
        }
        ruby_xfree(prms);
    }
    delete bmp;
    icinfo->_encprms = r;
    return r;
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

    return decoders;
}

/* EncoderParameter */

struct gdipEncoderParameter {
    GUID Guid; // Encoder
    ULONG Type; // EncoderParameterValueType
    VALUE values; // Array or String or nil
    ID _enc_id;
};

static void
gdip_encprm_mark(gdipEncoderParameter *ptr)
{
    rb_gc_mark(ptr->values);
}

const rb_data_type_t tEncoderParameter = _MAKE_DATA_TYPE(
    "EncoderParameter", RUBY_DATA_FUNC(gdip_encprm_mark), GDIP_DEFAULT_FREE(gdipEncoderParameter), &typeddata_size<gdipEncoderParameter>, NULL, &cEncoderParameter);

static VALUE
gdip_encprm_alloc(VALUE klass)
{
    dp("gdipEncoderParameter: alloc");
    gdipEncoderParameter *ptr = RB_ZALLOC(gdipEncoderParameter);
    ptr->values = Qnil;
    VALUE r = _Data_Wrap_Struct(klass, &tEncoderParameter, ptr);
    return r;
}

static VALUE
gdip_encprm_create(EncoderParameter *encprm)
{
    VALUE r = gdip_encprm_alloc(cEncoderParameter);
    gdipEncoderParameter *gpencprm = Data_Ptr<gdipEncoderParameter *>(r);
    gpencprm->Guid = encprm->Guid;
    gpencprm->Type = encprm->Type;
    gpencprm->values = Qnil;
    gpencprm->_enc_id = gdip_enum_get_id<GUID *>(cEncoder, &encprm->Guid);
    if (encprm->NumberOfValues > 0) {
        VALUE ary = rb_ary_new_capa(encprm->NumberOfValues);

        switch(encprm->Type) {
        case EncoderParameterValueTypeLong: {
            int *values = static_cast<int *>(encprm->Value); // ULONG in fact
            for (ULONG i = 0; i < encprm->NumberOfValues; ++i) {
                rb_ary_push(ary, RB_INT2NUM(values[i]));
            }
        } break;
        case EncoderParameterValueTypeLongRange: {
            int *values = static_cast<int *>(encprm->Value); // ULONG in fact
            for (ULONG i = 0; i < encprm->NumberOfValues; ++i) {
                VALUE range = rb_range_new(RB_INT2NUM(values[2 * i]), RB_INT2NUM(values[2 * i + 1]), 0);
                rb_ary_push(ary, range);
            }
        } break;
        case EncoderParameterValueTypeShort: {
            short *values = static_cast<short *>(encprm->Value); // unsigned short in fact
            for (ULONG i = 0; i < encprm->NumberOfValues; ++i) {
                rb_ary_push(ary, RB_INT2NUM(values[i]));
            }
        } break;
        case EncoderParameterValueTypeByte: {
            BYTE *values = static_cast<BYTE *>(encprm->Value);
            for (ULONG i = 0; i < encprm->NumberOfValues; ++i) {
                rb_ary_push(ary, RB_UINT2NUM(values[i]));
            }
        } break;
        case EncoderParameterValueTypeASCII: {
            VALUE str = rb_usascii_str_new(static_cast<char *>(encprm->Value), encprm->NumberOfValues - 1);
            RB_OBJ_FREEZE(str);
            gpencprm->values = str;
            return r;
        } break;
        case EncoderParameterValueTypeRational: {
            int *values = static_cast<int *>(encprm->Value);
            for (ULONG i = 0; i < encprm->NumberOfValues; ++i) {
                VALUE rat = rb_rational_new(RB_INT2NUM(values[2 * i]), RB_INT2NUM(values[2 * i + 1]));
                rb_ary_push(ary, rat);
            }
        } break;
        case EncoderParameterValueTypeRationalRange: {
            int *values = static_cast<int *>(encprm->Value);
            for (ULONG i = 0; i < encprm->NumberOfValues; ++i) {
                VALUE r1 = rb_rational_new(RB_INT2NUM(values[4 * i]), RB_INT2NUM(values[4 * i + 1]));
                VALUE r2 = rb_rational_new(RB_INT2NUM(values[4 * i + 2]), RB_INT2NUM(values[4 * i + 3]));
                VALUE range = rb_range_new(r1, r2, 0);
                rb_ary_push(ary, range);
            }
        } break;
        case EncoderParameterValueTypeUndefined: {
            BYTE *values = static_cast<BYTE *>(encprm->Value);
            for (ULONG i = 0; i < encprm->NumberOfValues; ++i) {
                rb_ary_push(ary, RB_UINT2NUM(values[i]));
            }
        } break;
        case EncoderParameterValueTypePointer:
            _WARNING("Not Implemented for EncoderParameterValueTypePointer");
            break;
        default:
            _WARNING("Not Implemented for EncoderParameterValueTypePointer");
            break;
        }

        RB_OBJ_FREEZE(ary);
        gpencprm->values = ary;
    }
    return r;
}


static inline int
gdip_encprm_size(gdipEncoderParameter *encprm)
{
    int num = 0;
    switch(TYPE(encprm->values)) {
        case RUBY_T_ARRAY:
            num = RARRAY_LEN(encprm->values);
            break;
        case RUBY_T_STRING:
            num = RSTRING_LEN(encprm->values) + 1;
            break;
    }
    return num;
}

static VALUE
gdip_encprm_inspect(VALUE self)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    int num = gdip_encprm_size(encprm);
    ID id_type = gdip_enum_get_id<int>(cEncoderParameterValueType, encprm->Type);
    VALUE v_inspect = rb_funcall(encprm->values, rb_intern("inspect"), 0);
    VALUE r = util_utf8_sprintf("#<%s Guid=%s NumberOfValues=%d Type=%s Value=%s>", 
        __class__(self), rb_id2name(encprm->_enc_id), num, rb_id2name(id_type), RSTRING_PTR(v_inspect));
    RB_GC_GUARD(v_inspect);
    return r;
}


static VALUE
gdip_encprm_get_int_ary(VALUE value)
{
    VALUE ary = rb_ary_new();

    if (Integer_p(value)) {
        rb_ary_push(ary, value);
    }
    else if (_KIND_OF(value, &tEnumInt)) {
        DWORD x = Data_Ptr_As<DWORD>(value);
        rb_ary_push(ary, RB_UINT2NUM(x));
    }
    else if (_RB_ARRAY_P(value)) {
        for (int i = 0; i < RARRAY_LEN(value); ++i) {
            VALUE v = rb_ary_entry(value, i);
            if (Integer_p(v)) {
                rb_ary_push(ary, v);
            }
            else if (_KIND_OF(v, &tEnumInt)) {
                DWORD x = Data_Ptr_As<DWORD>(v);
                rb_ary_push(ary, RB_UINT2NUM(x));
            }
        }
    }
    else {
        rb_raise(rb_eTypeError, "this ValueType requires Integer or Array<Integer>");
    }
    RB_OBJ_FREEZE(ary);
    return ary;
}


static VALUE
gdip_encprm_init_type_value(VALUE self, int type, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Type = type;
    switch(type) {
        case EncoderParameterValueTypeByte:
            encprm->values = gdip_encprm_get_int_ary(value);
            break;
        case EncoderParameterValueTypeShort:
            encprm->values = gdip_encprm_get_int_ary(value);
            break;
        case EncoderParameterValueTypeLong:
            encprm->values = gdip_encprm_get_int_ary(value);
            break;
        case EncoderParameterValueTypeUndefined:
            encprm->values = gdip_encprm_get_int_ary(value);
            break;
        case EncoderParameterValueTypeASCII: { // EncoderParameterValueTypeASCII
            if (RB_TYPE_P(value, RUBY_T_STRING)) {
                VALUE str = rb_str_dup(value);
                RB_OBJ_FREEZE(str);
                encprm->values = str;
            }
            else {
                rb_raise(rb_eTypeError, "ValueTypeASCII requires string");
            }
        } break;
        case EncoderParameterValueTypeRational: { // EncoderParameterValueTypeRational
            VALUE ary = rb_ary_new();

            if (_RB_RATIONAL_P(value)) {
                rb_ary_push(ary, value);
            }
            else if (_RB_ARRAY_P(value)) {
                for (int i = 0; i < RARRAY_LEN(value); ++i) {
                    VALUE v = rb_ary_entry(value, i);
                    if (_RB_RATIONAL_P(v)) {
                        rb_ary_push(ary, v);
                    }
                }
                if (RARRAY_LEN(ary) == 0) {
                    rb_raise(rb_eTypeError, "ValueTypeRational requires Rational or Array<Rational>");
                }
            }
            else {
                rb_raise(rb_eTypeError, "ValueTypeRational requires Rational or Array<Rational>");
            }
            RB_OBJ_FREEZE(ary);
            encprm->values = ary;
        } break;
        case EncoderParameterValueTypeLongRange: { // EncoderParameterValueTypeLongRange
            VALUE ary = rb_ary_new();

            if (rb_obj_is_kind_of(value, rb_cRange)) {
                VALUE r_beg = rb_funcall(value, rb_intern("begin"), 0);
                VALUE r_end = rb_funcall(value, rb_intern("end"), 0);
                if (Integer_p(r_beg, r_end)) {
                    rb_ary_push(ary, value);
                }
                else {
                    rb_raise(rb_eTypeError, "ValueTypeLongRange requires Range(Integer) or Array<Range(Integer)>");
                }
            }
            else if (_RB_ARRAY_P(value)) {
                for (int i = 0; i < RARRAY_LEN(value); ++i) {
                    VALUE v = rb_ary_entry(value, i);
                    if (rb_obj_is_kind_of(v, rb_cRange)) {
                        VALUE r_beg = rb_funcall(v, rb_intern("begin"), 0);
                        VALUE r_end = rb_funcall(v, rb_intern("end"), 0);
                        if (Integer_p(r_beg, r_end)) {
                            rb_ary_push(ary, v);
                        }
                        rb_ary_push(ary, v);
                    }
                }
                if (RARRAY_LEN(ary) == 0) {
                    rb_raise(rb_eTypeError, "ValueTypeLongRange requires Range(Integer) or Array<Range(Integer)>");
                }
            }
            else {
                rb_raise(rb_eTypeError, "ValueTypeLongRange requires Range(Integer) or Array<Range(Integer)>");
            }
            RB_OBJ_FREEZE(ary);
            encprm->values = ary;
        } break;
        case EncoderParameterValueTypeRationalRange: { // EncoderParameterValueTypeRationalRange
            VALUE ary = rb_ary_new();

            if (rb_obj_is_kind_of(value, rb_cRange)) {
                VALUE r_beg = rb_funcall(value, rb_intern("begin"), 0);
                VALUE r_end = rb_funcall(value, rb_intern("end"), 0);
                if (_RB_RATIONAL_P(r_beg) && _RB_RATIONAL_P(r_end)) {
                    rb_ary_push(ary, value);
                }
                else {
                    rb_raise(rb_eTypeError, "ValueTypeLongRange requires Range(Integer) or Array<Range(Integer)>");
                }
            }
            else if (_RB_ARRAY_P(value)) {
                for (int i = 0; i < RARRAY_LEN(value); ++i) {
                    VALUE v = rb_ary_entry(value, i);
                    if (rb_obj_is_kind_of(v, rb_cRange)) {
                        VALUE r_beg = rb_funcall(v, rb_intern("begin"), 0);
                        VALUE r_end = rb_funcall(v, rb_intern("end"), 0);
                        if (_RB_RATIONAL_P(r_beg) && _RB_RATIONAL_P(r_end)) {
                            rb_ary_push(ary, v);
                        }
                        rb_ary_push(ary, v);
                    }
                }
                if (RARRAY_LEN(ary) == 0) {
                    rb_raise(rb_eTypeError, "ValueTypeLongRange requires Range(Integer) or Array<Range(Integer)>");
                }
            }
            else {
                rb_raise(rb_eTypeError, "ValueTypeLongRange requires Range(Integer) or Array<Range(Integer)>");
            }
            RB_OBJ_FREEZE(ary);
            encprm->values = ary;
        } break;
        case EncoderParameterValueTypePointer: {
            rb_raise(rb_eNotImpError, "Not implemented type: ValueTypePointer");
        } break;
        default:
            rb_raise(rb_eNotImpError, "Not implemented type: %d", type);
    }
    return self;
}


static VALUE
gdip_encprm_quality_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = EncoderQuality;
    encprm->_enc_id = ID_Quality;
    encprm->Type = EncoderParameterValueTypeLong;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeLong, value);

    if (_RB_ARRAY_P(encprm->values) && RARRAY_LEN(encprm->values) == 1) {
        int q = RB_NUM2INT(rb_ary_entry(encprm->values, 0));
        if (q < 0 || 100 < q) {
            _VERBOSE("Gdiplus::EncoderParameter Quality must be Integer from 0 to 100");
        }
    }
    else {
        _VERBOSE("Gdiplus::EncoderParameter Quality must be Integer from 0 to 100");
    }
    return self;
}

static VALUE
gdip_encprm_transformation_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = EncoderTransformation;
    encprm->_enc_id = ID_Transformation;
    encprm->Type = EncoderParameterValueTypeLong;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeLong, value);

    if (_RB_ARRAY_P(encprm->values) && RARRAY_LEN(encprm->values) == 1) {
        int q = RB_NUM2INT(rb_ary_entry(encprm->values, 0));
        if (q < 13 || 17 < q) {
            _VERBOSE("Gdiplus::EncoderParameter Transformation must be EncoderValue.Transform***");
        }
    }
    else {
        _VERBOSE("Gdiplus::EncoderParameter Transformation must be EncoderValue.Transform***");
    }
    return self;
}

static VALUE
gdip_encprm_compression_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = EncoderCompression;
    encprm->_enc_id = ID_Compression;
    encprm->Type = EncoderParameterValueTypeLong;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeLong, value);

    if (_RB_ARRAY_P(encprm->values) && RARRAY_LEN(encprm->values) == 1) {
        int q = RB_NUM2INT(rb_ary_entry(encprm->values, 0));
        if (q < 2 || 6 < q) {
            _VERBOSE("Gdiplus::EncoderParameter Compression must be EncoderValue.Compression***");
        }
    }
    else {
        _VERBOSE("Gdiplus::EncoderParameter Compression must be EncoderValue.Compression***");
    }
    return self;
}

static VALUE
gdip_encprm_colordepth_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = EncoderColorDepth;
    encprm->_enc_id = ID_ColorDepth;
    encprm->Type = EncoderParameterValueTypeLong;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeLong, value);

    if (_RB_ARRAY_P(encprm->values) && RARRAY_LEN(encprm->values) == 1) {
        int q = RB_NUM2INT(rb_ary_entry(encprm->values, 0));
        if (q != 1 && q != 4 && q != 8 && q != 24 && q != 32) {
            _VERBOSE("Gdiplus::EncoderParameter ColorDepth must be Integer 1, 4, 8, 24, 32");
        }
    }
    else {
        _VERBOSE("Gdiplus::EncoderParameter ColorDepth must be Integer 1, 4, 8, 24, 32");
    }
    return self;
}

static VALUE
gdip_encprm_saveflag_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = EncoderSaveFlag;
    encprm->_enc_id = ID_SaveFlag;
    encprm->Type = EncoderParameterValueTypeLong;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeLong, value);

    if (_RB_ARRAY_P(encprm->values) && RARRAY_LEN(encprm->values) == 1) {
        int q = RB_NUM2INT(rb_ary_entry(encprm->values, 0));
        if (q < EncoderValueMultiFrame || EncoderValueFrameDimensionPage < q) {
            _VERBOSE("Gdiplus::EncoderParameter SaveFlag must be EncoderValue.***Frame ,EncoderValue.Flush or EncoderValue.FrameDimension***");
        }
    }
    else {
        _VERBOSE("Gdiplus::EncoderParameter SaveFlag must be EncoderValue.***Frame ,EncoderValue.Flush or EncoderValue.FrameDimension***");
    }
    return self;
}

static VALUE
gdip_encprm_saveascmyk_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = _EncoderSaveAsCMYK;
    encprm->_enc_id = ID_SaveAsCMYK;
    encprm->Type = EncoderParameterValueTypeLong;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeLong, value);

    if (_RB_ARRAY_P(encprm->values) && RARRAY_LEN(encprm->values) == 1) {
        int q = RB_NUM2INT(rb_ary_entry(encprm->values, 0));
        if (q != 0 && q != 1 && q != 24 && q != 25) {
            _VERBOSE("Gdiplus::EncoderParameter ColorDepth must be EncoderValue.ColorType***");
        }
    }
    else {
        _VERBOSE("Gdiplus::EncoderParameter ColorDepth must be EncoderValue.ColorType***");
    }
    return self;
}

static VALUE
gdip_encprm_chrominance_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = EncoderChrominanceTable;
    encprm->_enc_id = ID_ChrominanceTable;
    encprm->Type = EncoderParameterValueTypeShort;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeShort, value);

    if (!_RB_ARRAY_P(encprm->values) || RARRAY_LEN(encprm->values) != 64) {
        _VERBOSE("Gdiplus::EncoderParameter ChrominanceTable must be an array of 64 entries of unsigned short");
    }
    return self;
}

static VALUE
gdip_encprm_luminance_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = EncoderLuminanceTable;
    encprm->_enc_id = ID_LuminanceTable;
    encprm->Type = EncoderParameterValueTypeShort;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeShort, value);

    if (!_RB_ARRAY_P(encprm->values) || RARRAY_LEN(encprm->values) != 64) {
        _VERBOSE("Gdiplus::EncoderParameter LuminanceTable must be an array of 64 entries of unsigned short");
    }
    return self;
}

static VALUE
gdip_encprm_version_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = EncoderVersion;
    encprm->_enc_id = ID_Version;
    encprm->Type = EncoderParameterValueTypeLong;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeLong, value);
    
    if (_RB_ARRAY_P(encprm->values) && RARRAY_LEN(encprm->values) == 1) {
        int q = RB_NUM2INT(rb_ary_entry(encprm->values, 0));
        if (q < 9 || 10 < q) {
            _VERBOSE("Gdiplus::EncoderParameter Version must be EncoderValue.Version***");
        }
    }
    else {
        _VERBOSE("Gdiplus::EncoderParameter Version must be EncoderValue.Version***");
    }
    
    return self;
}

static VALUE
gdip_encprm_scanmethod_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = EncoderScanMethod;
    encprm->_enc_id = ID_ScanMethod;
    encprm->Type = EncoderParameterValueTypeLong;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeLong, value);
    if (_RB_ARRAY_P(encprm->values) && RARRAY_LEN(encprm->values) == 1) {
        int q = RB_NUM2INT(rb_ary_entry(encprm->values, 0));
        if (q < EncoderValueScanMethodInterlaced || EncoderValueScanMethodNonInterlaced < q) {
            _VERBOSE("Gdiplus::EncoderParameter ScanMethod must be EncoderValue.ScanMethod***");
        }
    }
    else {
        _VERBOSE("Gdiplus::EncoderParameter ScanMethod must be EncoderValue.ScanMethod***");
    }
    
    return self;
}

static VALUE
gdip_encprm_rendermethod_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = EncoderRenderMethod;
    encprm->_enc_id = ID_RenderMethod;
    encprm->Type = EncoderParameterValueTypeLong;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeLong, value);
    if (_RB_ARRAY_P(encprm->values) && RARRAY_LEN(encprm->values) == 1) {
        int q = RB_NUM2INT(rb_ary_entry(encprm->values, 0));
        if (q < EncoderValueRenderProgressive || EncoderValueRenderNonProgressive < q) {
            _VERBOSE("Gdiplus::EncoderParameter RenderMethod must be EncoderValue.Render***");
        }
    }
    else {
        _VERBOSE("Gdiplus::EncoderParameter RenderMethod must be EncoderValue.Render***");
    }

    return self;
}

static VALUE
gdip_encprm_colorspace_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = _EncoderColorSpace;
    encprm->_enc_id = ID_ColorSpace;
    encprm->Type = EncoderParameterValueTypeLong;
    encprm->values = Qnil;

    gdip_encprm_init_type_value(self, EncoderParameterValueTypeLong, value);
    
    return self;
}

static VALUE
gdip_encprm_imageitems_init(VALUE self, VALUE value)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    encprm->Guid = _EncoderImageItems;
    encprm->_enc_id = ID_ImageItems;
    encprm->Type = EncoderParameterValueTypePointer;

    // check value
    VALUE ary = rb_ary_new();
    rb_ary_push(ary, value);
    encprm->values = ary;
    // NOTIMPLEMENTED
    
    return self;
}

static VALUE
gdip_encprm_guid(VALUE self)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    VALUE r = gdip_enum_get<GUID *>(cEncoder, &encprm->Guid);
    return r;
}

static VALUE
gdip_encprm_type(VALUE self)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    VALUE r = gdip_enum_get<int>(cEncoderParameterValueType, encprm->Type);
    return r;
}

static VALUE
gdip_encprm_num(VALUE self)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    int num = gdip_encprm_size(encprm);
    return RB_INT2NUM(num);
}

static VALUE
gdip_encprm_value(VALUE self)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    return encprm->values;
}

static VALUE
gdip_encprm_init(int argc, VALUE* argv, VALUE self)
{
    gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(self);
    VALUE encoder = Qnil;
    VALUE value = Qnil;
    VALUE type = Qnil;
    rb_scan_args(argc, argv, "21", &encoder, &value, &type);

    ID enc_id = ID_UNKNOWN;
    if (_KIND_OF(encoder, &tGuid)) {
        GUID *guid = Data_Ptr<GUID *>(encoder);
        encprm->Guid = *guid;
        enc_id = gdip_enum_get_id<GUID *>(cEncoder, guid);
        encprm->_enc_id = enc_id;
    }
    else {
        rb_raise(rb_eTypeError, "first argument must be Gdiplus::Encoder (example: Encoder.Quality)");
    }

    if (!RB_NIL_P(type)) {
        int v_type = 0;
        if (rb_obj_is_kind_of(type, cEncoderParameterValueType)) {
            v_type = Data_Ptr_As<int>(type);
        }
        else {
            v_type = RB_NUM2INT(rb_to_int(type));
        }
        gdip_encprm_init_type_value(self, v_type, value);
    }
    else {
        if (enc_id == ID_Quality) {
            gdip_encprm_quality_init(self, value);
        }
        else if (enc_id == ID_SaveFlag) {
            gdip_encprm_saveflag_init(self, value);
        }
        else if (enc_id == ID_Transformation) {
            gdip_encprm_transformation_init(self, value);
        }
        else if (enc_id == ID_LuminanceTable) {
            gdip_encprm_luminance_init(self, value);
        }
        else if (enc_id == ID_ChrominanceTable) {
            gdip_encprm_chrominance_init(self, value);
        }
        else if (enc_id == ID_ImageItems) {
            gdip_encprm_imageitems_init(self, value);
        }
        else if (enc_id == ID_Compression) {
            gdip_encprm_compression_init(self, value);
        }
        else if (enc_id == ID_ColorDepth) {
            gdip_encprm_colordepth_init(self, value);
        }
        else if (enc_id == ID_SaveAsCMYK) {
            gdip_encprm_saveascmyk_init(self, value);
        }
        else if (enc_id == ID_ScanMethod) {
            gdip_encprm_scanmethod_init(self, value);
        }
        else if (enc_id == ID_RenderMethod) {
            gdip_encprm_rendermethod_init(self, value);
        }
        else if (enc_id == ID_Version) {
            gdip_encprm_version_init(self, value);
        }
        else if (enc_id == ID_ColorSpace) {
            gdip_encprm_colorspace_init(self, value);
        }
        else {
            rb_raise(rb_eArgError, "unknown encoder parameter");
        }
    }
    

    return self;
}

/* EncoderParameters */

struct gdipEncoderParameters {
    VALUE params;
    EncoderParameters *data;
};

static void
gdip_encprms_free_data(EncoderParameters *ptr)
{
    if (ptr == NULL) return;
    for (unsigned int i = 0; i < ptr->Count; ++i) {
        if (ptr->Parameter[i].Value) {
            dp("EncoderParameter.Value free (Type: %d, Num: %d)", ptr->Parameter[i].Type, ptr->Parameter[i].NumberOfValues);
            ruby_xfree(ptr->Parameter[i].Value);
        }
    }
    ruby_xfree(ptr);
    dp("EncoderParameters: free");
}

static void
gdip_encprms_free(gdipEncoderParameters *ptr)
{
    dp("gdipEncoderParameters: free");
    if (ptr->data) {
        gdip_encprms_free_data(ptr->data);
        ptr->data = NULL;
    }
    ruby_xfree(ptr);
}

static void
gdip_encprms_mark(gdipEncoderParameters *ptr)
{
    rb_gc_mark(ptr->params);
}

const rb_data_type_t tEncoderParameters = _MAKE_DATA_TYPE(
    "EncoderParameters", RUBY_DATA_FUNC(gdip_encprms_mark), RUBY_DATA_FUNC(gdip_encprms_free), &typeddata_size<gdipEncoderParameters>, NULL, &cEncoderParameters);

static VALUE
gdip_encprms_create(EncoderParameters *encprms)
{
    VALUE r = typeddata_alloc<gdipEncoderParameters, &tEncoderParameters>(cEncoderParameters);
    gdipEncoderParameters *gpencprms = Data_Ptr<gdipEncoderParameters *>(r);
    gpencprms->params = Qnil;
    if (encprms != NULL && encprms->Count > 0) {
        VALUE ary = rb_ary_new_capa(encprms->Count);
        for (UINT i = 0; i < encprms->Count; ++i) {
            VALUE e = gdip_encprm_create(&(encprms->Parameter[i]));
            rb_ary_push(ary, e);
        }
        //RB_OBJ_FREEZE(ary);
        gpencprms->params = ary;
    }
    return r;
}

/**
 * 
 * @overload initialize()
 * 
 */
static VALUE
gdip_encprms_init(VALUE self)
{
    gdipEncoderParameters *gpencprms = Data_Ptr<gdipEncoderParameters *>(self);
    gpencprms->params = Qnil;
    return self;
}

/**
 *
 * @return [String]
 * 
 */
static VALUE
gdip_encprms_inspect(VALUE self)
{
    gdipEncoderParameters *gpencprms = Data_Ptr<gdipEncoderParameters *>(self);
    int count = 0;
    if (_RB_ARRAY_P(gpencprms->params)) {
        count = RARRAY_LEN(gpencprms->params);
    }
    VALUE v_inspect = rb_funcall(gpencprms->params, rb_intern("inspect"), 0);
    VALUE r = util_utf8_sprintf("#<%s Count=%d Parameter=%s>", 
        __class__(self), count, RSTRING_PTR(v_inspect));
    RB_GC_GUARD(v_inspect);
    return r;
}

/**
 * Returns an array of EncoderParameter. 
 * @return [Array<EncoderParameter>]
 * 
 */
static VALUE
gdip_encprms_param(VALUE self)
{
    gdipEncoderParameters *gpencprms = Data_Ptr<gdipEncoderParameters *>(self);
    VALUE r;
    if (_RB_ARRAY_P(gpencprms->params)) {
        r = rb_ary_dup(gpencprms->params);
    }
    else {
        r = rb_ary_new();
    }
    RB_OBJ_FREEZE(r);

    return r;
}

/**
 *
 * @overload add(encprm)
 *   add EncoderParameter.
 *   @param encprm [EncoderParameter]
 * 
 */
 static VALUE
 gdip_encprms_add(VALUE self, VALUE v)
 {
     gdipEncoderParameters *gpencprms = Data_Ptr<gdipEncoderParameters *>(self);
     if (!_KIND_OF(v, &tEncoderParameter)) {
         rb_raise(rb_eTypeError, "argument must be EncoderParameter");
     }
     VALUE ary = gpencprms->params;
     if (!_RB_ARRAY_P(ary)) {
         ary = rb_ary_new();
         gpencprms->params = ary;
     }
     rb_ary_push(ary, v);
     return self;
 }

/**
 *
 * 
 * 
 */
EncoderParameters *
gdip_encprms_build_struct(VALUE v)
{
    if (!_KIND_OF(v, &tEncoderParameters)) {
        rb_raise(rb_eTypeError, "invalid type");
    }

    gdipEncoderParameters *encprms = Data_Ptr<gdipEncoderParameters *>(v);
    if (encprms->data) {
        gdip_encprms_free_data(encprms->data);
        encprms->data = NULL;
    }

    if (!_RB_ARRAY_P(encprms->params)) {
        return NULL;
    }

    VALUE params = encprms->params;
    if (RARRAY_LEN(params) == 0) {
        return NULL;
    }
    EncoderParameters *data = static_cast<EncoderParameters *>(ruby_xcalloc(1, sizeof(UINT) + sizeof(EncoderParameter) * RARRAY_LEN(params)));
    encprms->data = data;
    dp("EncoderParameters: malloc");

    data->Count = RARRAY_LEN(params);
    for (int i = 0; i < RARRAY_LEN(params); ++i) {
        VALUE param = rb_ary_entry(params, i);
        gdipEncoderParameter *encprm = Data_Ptr<gdipEncoderParameter *>(param);

        int size = gdip_encprm_size(encprm);
        data->Parameter[i].Guid = encprm->Guid;
        data->Parameter[i].NumberOfValues = size;
        data->Parameter[i].Type = encprm->Type;

        if (size == 0) continue;

        VALUE values = encprm->values;
        if (encprm->Type == EncoderParameterValueTypeByte || encprm->Type == EncoderParameterValueTypeUndefined) {
            BYTE *param_v = static_cast<BYTE *>(RB_ZALLOC_N(BYTE, size));
            dp("EncoderParameter.Value malloc (Type: %d, Num: %d)", encprm->Type, size);
            data->Parameter[i].Value = param_v;
            for (int j = 0; j < size; ++j) {
                VALUE e = rb_ary_entry(values, j);
                param_v[j] = static_cast<BYTE>(RB_NUM2UINT(e));
            }
        }
        else if (encprm->Type == EncoderParameterValueTypeShort) {
            WORD *param_v = static_cast<WORD *>(RB_ZALLOC_N(WORD, size));
            dp("EncoderParameter.Value malloc (Type: %d, Num: %d)", encprm->Type, size);
            data->Parameter[i].Value = param_v;
            for (int j = 0; j < size; ++j) {
                VALUE e = rb_ary_entry(values, j);
                param_v[j] = static_cast<WORD>(RB_NUM2UINT(e));
            }
        }
        else if (encprm->Type == EncoderParameterValueTypeLong) {
            DWORD *param_v = static_cast<DWORD *>(RB_ZALLOC_N(DWORD, size));
            dp("EncoderParameter.Value malloc (Type: %d, Num: %d)", encprm->Type, size);
            data->Parameter[i].Value = param_v;
            for (int j = 0; j < size; ++j) {
                VALUE e = rb_ary_entry(values, j);
                param_v[j] = static_cast<DWORD>(RB_NUM2UINT(e));
            }
        }
        else if (encprm->Type == EncoderParameterValueTypeASCII) {
            char *param_v = static_cast<char *>(RB_ZALLOC_N(char, size));
            dp("EncoderParameter.Value malloc (Type: %d, Num: %d)", encprm->Type, size);
            data->Parameter[i].Value = param_v;
            memcpy(param_v, RSTRING_PTR(values), size - 1); // RSTRING_LEN
        }
        else if (encprm->Type == EncoderParameterValueTypeRational) {
            DWORD *param_v = static_cast<DWORD *>(RB_ZALLOC_N(DWORD, size * 2));
            dp("EncoderParameter.Value malloc (Type: %d, Num: %d)", encprm->Type, size);
            data->Parameter[i].Value = param_v;
            for (int j = 0; j < size; ++j) {
                VALUE e = rb_ary_entry(values, j);
                VALUE num = rb_rational_num(e);
                VALUE den = rb_rational_den(e);
                param_v[j * 2] = RB_NUM2UINT(num);
                param_v[j * 2 + 1] = RB_NUM2UINT(den);
            }
        }
        else if (encprm->Type == EncoderParameterValueTypeLongRange) {
            DWORD *param_v = static_cast<DWORD *>(RB_ZALLOC_N(DWORD, size * 2));
            dp("EncoderParameter.Value malloc (Type: %d, Num: %d)", encprm->Type, size);
            data->Parameter[i].Value = param_v;
            for (int j = 0; j < size; ++j) {
                VALUE e = rb_ary_entry(values, j);
                VALUE beg = rb_funcall(e, rb_intern("begin"), 0);
                VALUE end = rb_funcall(e, rb_intern("end"), 0);
                param_v[j * 2] = RB_NUM2UINT(beg);
                param_v[j * 2 + 1] = RB_NUM2UINT(end);
            }
        }
        else if (encprm->Type == EncoderParameterValueTypeRationalRange) {
            DWORD *param_v = static_cast<DWORD *>(RB_ZALLOC_N(DWORD, size * 4));
            dp("EncoderParameter.Value malloc (Type: %d, Num: %d)", encprm->Type, size);
            data->Parameter[i].Value = param_v;
            for (int j = 0; j < size; ++j) {
                VALUE e = rb_ary_entry(values, j);
                VALUE beg = rb_funcall(e, rb_intern("begin"), 0);
                VALUE end = rb_funcall(e, rb_intern("end"), 0);
                VALUE beg_num = rb_rational_num(beg);
                VALUE beg_den = rb_rational_den(beg);
                VALUE end_num = rb_rational_num(end);
                VALUE end_den = rb_rational_den(end);
                param_v[j * 4] = RB_NUM2UINT(beg_num);
                param_v[j * 4 + 1] = RB_NUM2UINT(beg_den);
                param_v[j * 4 + 2] = RB_NUM2UINT(end_num);
                param_v[j * 4 + 3] = RB_NUM2UINT(end_den);
            }
        }
        else if (encprm->Type == EncoderParameterValueTypePointer) {
            _WARNING("Not Implemented for EncoderParameterValueTypePointer");
        }
        else {
            _WARNING("Invalid Type");
        }
    }
    return data;
}

#if GDIPLUS_DEBUG
static VALUE
gdip_encprms_build_and_create_for_debug(VALUE self)
{
    EncoderParameters *params = gdip_encprms_build_struct(self);
    return gdip_encprms_create(params);
}
#endif

/**
 * Document-class: Gdiplus::Guid
 * This class wraps a GUID structure.
 * @example
 *   guid = Gdiplus::Guid.new("557cf400-1a04-11d3-9a73-0000f81ef32e")
 */

/*
 * Document-class: Gdiplus::ImageCodecInfo
 * This class wraps a ImageCodecInfo structure.
 * @example
 *   Gdiplus::ImageCodecInfo.GetImageEncoders.each {|icinfo|
 *     p [icinfo.FormatDescription, icinfo.MimeType]
 *   }
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
    
    rb_define_method(cImageCodecInfo, "GetEncoderParameterList", RUBY_METHOD_FUNC(gdip_icinfo_prmlist), 0);
    rb_define_method(cImageCodecInfo, "parameter_list", RUBY_METHOD_FUNC(gdip_icinfo_prmlist), 0);
    rb_define_method(cImageCodecInfo, "inspect", RUBY_METHOD_FUNC(gdip_icinfo_inspect), 0);

    rb_define_singleton_method(cImageCodecInfo, "image_encoders", RUBY_METHOD_FUNC(gdip_icinfo_s_image_encoders), 0);
    rb_define_singleton_method(cImageCodecInfo, "GetImageEncoders", RUBY_METHOD_FUNC(gdip_icinfo_s_image_encoders), 0);
    rb_define_singleton_method(cImageCodecInfo, "image_decoders", RUBY_METHOD_FUNC(gdip_icinfo_s_image_decoders), 0);
    rb_define_singleton_method(cImageCodecInfo, "GetImageDecoders", RUBY_METHOD_FUNC(gdip_icinfo_s_image_decoders), 0);

    cEncoderParameter = rb_define_class_under(mGdiplus, "EncoderParameter", rb_cObject);
    rb_define_alloc_func(cEncoderParameter, gdip_encprm_alloc);
    rb_define_method(cEncoderParameter, "initialize", RUBY_METHOD_FUNC(gdip_encprm_init), -1);
    rb_define_method(cEncoderParameter, "inspect", RUBY_METHOD_FUNC(gdip_encprm_inspect), 0);
    rb_define_method(cEncoderParameter, "Encoder", RUBY_METHOD_FUNC(gdip_encprm_guid), 0);
    rb_define_method(cEncoderParameter, "encoder", RUBY_METHOD_FUNC(gdip_encprm_guid), 0);
    rb_define_method(cEncoderParameter, "ValueType", RUBY_METHOD_FUNC(gdip_encprm_type), 0);
    rb_define_method(cEncoderParameter, "Type", RUBY_METHOD_FUNC(gdip_encprm_type), 0);
    rb_define_method(cEncoderParameter, "value_type", RUBY_METHOD_FUNC(gdip_encprm_type), 0);
    rb_define_method(cEncoderParameter, "type", RUBY_METHOD_FUNC(gdip_encprm_type), 0);
    rb_define_method(cEncoderParameter, "NumberOfValues", RUBY_METHOD_FUNC(gdip_encprm_num), 0);
    rb_define_method(cEncoderParameter, "number_of_values", RUBY_METHOD_FUNC(gdip_encprm_num), 0);
    rb_define_method(cEncoderParameter, "Value", RUBY_METHOD_FUNC(gdip_encprm_value), 0);
    rb_define_method(cEncoderParameter, "value", RUBY_METHOD_FUNC(gdip_encprm_value), 0);
    rb_define_method(cEncoderParameter, "values", RUBY_METHOD_FUNC(gdip_encprm_value), 0);

    VALUE cEncoderParameterQuality = rb_define_class_under(mGdiplus, "EncoderParameterQuality", cEncoderParameter);
    rb_define_method(cEncoderParameterQuality, "initialize", RUBY_METHOD_FUNC(gdip_encprm_quality_init), 1);

    VALUE cEncoderParameterCompression = rb_define_class_under(mGdiplus, "EncoderParameterCompression", cEncoderParameter);
    rb_define_method(cEncoderParameterCompression, "initialize", RUBY_METHOD_FUNC(gdip_encprm_compression_init), 1);

    VALUE cEncoderParameterColorDepth = rb_define_class_under(mGdiplus, "EncoderParameterColorDepth", cEncoderParameter);
    rb_define_method(cEncoderParameterColorDepth, "initialize", RUBY_METHOD_FUNC(gdip_encprm_colordepth_init), 1);

    VALUE cEncoderParameterTransformation = rb_define_class_under(mGdiplus, "EncoderParameterTransformation", cEncoderParameter);
    rb_define_method(cEncoderParameterTransformation, "initialize", RUBY_METHOD_FUNC(gdip_encprm_transformation_init), 1);

    VALUE cEncoderParameterSaveFlag = rb_define_class_under(mGdiplus, "EncoderParameterSaveFlag", cEncoderParameter);
    rb_define_method(cEncoderParameterSaveFlag, "initialize", RUBY_METHOD_FUNC(gdip_encprm_saveflag_init), 1);

    VALUE cEncoderParameterChrominanceTable = rb_define_class_under(mGdiplus, "EncoderParameterChrominanceTable", cEncoderParameter);
    rb_define_method(cEncoderParameterChrominanceTable, "initialize", RUBY_METHOD_FUNC(gdip_encprm_chrominance_init), 1);

    VALUE cEncoderParameterLuminanceTable = rb_define_class_under(mGdiplus, "EncoderParameterLuminanceTable", cEncoderParameter);
    rb_define_method(cEncoderParameterLuminanceTable, "initialize", RUBY_METHOD_FUNC(gdip_encprm_luminance_init), 1);

    VALUE cEncoderParameterVersion = rb_define_class_under(mGdiplus, "EncoderParameterVersion", cEncoderParameter);
    rb_define_method(cEncoderParameterVersion, "initialize", RUBY_METHOD_FUNC(gdip_encprm_version_init), 1);

    VALUE cEncoderParameterRenderMethod = rb_define_class_under(mGdiplus, "EncoderParameterRenderMethod", cEncoderParameter);
    rb_define_method(cEncoderParameterRenderMethod, "initialize", RUBY_METHOD_FUNC(gdip_encprm_rendermethod_init), 1);

    VALUE cEncoderParameterScanMethod = rb_define_class_under(mGdiplus, "EncoderParameterScanMethod", cEncoderParameter);
    rb_define_method(cEncoderParameterScanMethod, "initialize", RUBY_METHOD_FUNC(gdip_encprm_scanmethod_init), 1);


    VALUE cEncoderParameterColorSpace = rb_define_class_under(mGdiplus, "EncoderParameterColorSpace", cEncoderParameter);
    rb_define_method(cEncoderParameterColorSpace, "initialize", RUBY_METHOD_FUNC(gdip_encprm_colorspace_init), 1);

    VALUE cEncoderParameterSaveAsCMYK = rb_define_class_under(mGdiplus, "EncoderParameterSaveAsCMYK", cEncoderParameter);
    rb_define_method(cEncoderParameterSaveAsCMYK, "initialize", RUBY_METHOD_FUNC(gdip_encprm_saveascmyk_init), 1);

    VALUE cEncoderParameterImageItems = rb_define_class_under(mGdiplus, "EncoderParameterImageItems", cEncoderParameter);
    rb_define_method(cEncoderParameterImageItems, "initialize", RUBY_METHOD_FUNC(gdip_encprm_imageitems_init), 1);

    cEncoderParameters = rb_define_class_under(mGdiplus, "EncoderParameters", rb_cObject);
    rb_define_alloc_func(cEncoderParameters, &typeddata_alloc<gdipEncoderParameters, &tEncoderParameters>);
    rb_define_method(cEncoderParameters, "initialize", RUBY_METHOD_FUNC(gdip_encprms_init), 0);
    rb_define_method(cEncoderParameters, "inspect", RUBY_METHOD_FUNC(gdip_encprms_inspect), 0);
    rb_define_method(cEncoderParameters, "Param", RUBY_METHOD_FUNC(gdip_encprms_param), 0);
    rb_define_method(cEncoderParameters, "param", RUBY_METHOD_FUNC(gdip_encprms_param), 0);
    rb_define_method(cEncoderParameters, "params", RUBY_METHOD_FUNC(gdip_encprms_param), 0);
    rb_define_method(cEncoderParameters, "add", RUBY_METHOD_FUNC(gdip_encprms_add), 1);
    #if GDIPLUS_DEBUG
    rb_define_method(cEncoderParameters, "build_and_create_for_debug", RUBY_METHOD_FUNC(gdip_encprms_build_and_create_for_debug), 0);
    #endif
}