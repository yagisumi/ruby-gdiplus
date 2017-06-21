/*
 * gdip_graphics.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"


const rb_data_type_t tStringFormat = _MAKE_DATA_TYPE(
    "StringFormat", 0, GDIP_OBJ_FREE(StringFormat *), NULL, NULL, &cStringFormat);


/**
 * @overload initialize()
 * @overload initialize(format)
 *   @param format [StringFormat]
 * @overload initialize(options, language=0)
 *   @param options [StringFormatFlags]
 *   @param language [LangId] Specifies the language of the text.
 */
static VALUE
gdip_strfmt_init(int argc, VALUE *argv, VALUE self)
{
    StringFormat *strfmt = Data_Ptr<StringFormat *>(self);
    if (strfmt != NULL) {
        _VERBOSE("Object already initialized.");
        return self;
    }

    if (argc > 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..2)", argc);
    }

    int flags = 0;
    LANGID language = LANG_NEUTRAL;

    if (argc == 1) {
        if (_KIND_OF(argv[0], &tStringFormat)) {
            StringFormat *format = Data_Ptr<StringFormat *>(argv[0]);
            Check_NULL(format, "The StringFormat of the argument does not exist.");
            _DATA_PTR(self) = gdip_obj_create(new StringFormat(format));
            return self;
        }
        else {
            gdip_arg_to_enumint(cStringFormatFlags, argv[0], &flags, ArgOptionAcceptInt, "The first argument should be StringFormatFlags or StringFormat.");
        }
    }
    else if (argc == 2) {
        if (!gdip_arg_to_enumint(cStringFormatFlags, argv[0], &flags) || !gdip_arg_to_langid(argv[1], &language)) {
            rb_raise(rb_eTypeError, "Argument types do not match.");
        }
    }
    _DATA_PTR(self) = gdip_obj_create(new StringFormat(flags, language));
    return self;
}

static VALUE vGenericDefault = Qnil;
static VALUE vGenericTypographic = Qnil;

static VALUE
gdip_strfmt_s_get_generic_default(VALUE self)
{
    if (RB_NIL_P(vGenericDefault)) {
        vGenericDefault = typeddata_alloc_null<&tStringFormat>(cStringFormat);
        _DATA_PTR(vGenericDefault) = gdip_obj_create(const_cast<StringFormat *>(StringFormat::GenericDefault()));
        RB_OBJ_FREEZE(vGenericDefault);
        rb_gc_register_address(&vGenericDefault);
    }
    return vGenericDefault;
}

static VALUE
gdip_strfmt_s_get_generic_typographic(VALUE self)
{
    if (RB_NIL_P(vGenericTypographic)) {
        vGenericTypographic = typeddata_alloc_null<&tStringFormat>(cStringFormat);
        _DATA_PTR(vGenericTypographic) = gdip_obj_create(const_cast<StringFormat *>(StringFormat::GenericDefault()));
        RB_OBJ_FREEZE(vGenericTypographic);
        rb_gc_register_address(&vGenericTypographic);
    }
    return vGenericTypographic;
}

static VALUE
gdip_strfmt_get_alignment(VALUE self)
{
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    return gdip_enumint_create(cStringAlignment, format->GetAlignment());
}

static VALUE
gdip_strfmt_set_alignment(VALUE self, VALUE arg)
{
    Check_Frozen(self);
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    int enumint = 0;
    gdip_arg_to_enumint(cStringAlignment, arg, &enumint, ArgOptionAcceptInt, "The argument should be StringAlignment.");
    Status status = format->SetAlignment(static_cast<StringAlignment>(enumint));
    Check_Status(status);
    return self;
}

static VALUE
gdip_strfmt_get_digit_substitution_language(VALUE self)
{
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    return gdip_langid_create(format->GetDigitSubstitutionLanguage());
}

static VALUE
gdip_strfmt_get_digit_substitution_method(VALUE self)
{
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    return gdip_enumint_create(cStringDigitSubstitute, format->GetDigitSubstitutionMethod());
}

static VALUE
gdip_strfmt_get_format_flags(VALUE self)
{
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    return gdip_enumint_create(cStringFormatFlags, format->GetFormatFlags());
}

static VALUE
gdip_strfmt_set_format_flags(VALUE self, VALUE arg)
{
    Check_Frozen(self);
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    int enumint = 0;
    gdip_arg_to_enumint(cStringFormatFlags, arg, &enumint, ArgOptionAcceptInt, "The argument should be StringFormatFlags.");
    Status status = format->SetFormatFlags(static_cast<StringFormatFlags>(enumint));
    Check_Status(status);
    return self;
}

static VALUE
gdip_strfmt_get_hotkey_prefix(VALUE self)
{
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    return gdip_enumint_create(cHotkeyPrefix, format->GetHotkeyPrefix());
}

static VALUE
gdip_strfmt_set_hotkey_prefix(VALUE self, VALUE arg)
{
    Check_Frozen(self);
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    int enumint = 0;
    gdip_arg_to_enumint(cHotkeyPrefix, arg, &enumint, ArgOptionAcceptInt, "The argument should be HotkeyPrefix.");
    Status status = format->SetHotkeyPrefix(static_cast<HotkeyPrefix>(enumint));
    Check_Status(status);
    return self;
}

static VALUE
gdip_strfmt_get_line_alignment(VALUE self)
{
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    return gdip_enumint_create(cStringAlignment, format->GetLineAlignment());
}

static VALUE
gdip_strfmt_set_line_alignment(VALUE self, VALUE arg)
{
    Check_Frozen(self);
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    int enumint = 0;
    gdip_arg_to_enumint(cStringAlignment, arg, &enumint, ArgOptionAcceptInt, "The argument should be StringAlignment.");
    Status status = format->SetLineAlignment(static_cast<StringAlignment>(enumint));
    Check_Status(status);
    return self;
}

static VALUE
gdip_strfmt_get_trimming(VALUE self)
{
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    return gdip_enumint_create(cStringTrimming, format->GetTrimming());
}

static VALUE
gdip_strfmt_set_trimming(VALUE self, VALUE arg)
{
    Check_Frozen(self);
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    int enumint = 0;
    gdip_arg_to_enumint(cStringTrimming, arg, &enumint, ArgOptionAcceptInt, "The argument should be StringTrimming.");
    Status status = format->SetTrimming(static_cast<StringTrimming>(enumint));
    Check_Status(status);
    return self;
}

static VALUE
gdip_strfmt_get_tab_stops(VALUE self)
{
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    float offset = 0.0f;
    VALUE r = Qnil;
    int count = format->GetTabStopCount();
    if (count == 0) {
        r = rb_ary_new();
    }
    else {
        float *stops = static_cast<float *>(ruby_xmalloc(count * sizeof(float)));
        Status status = format->GetTabStops(count, &offset, stops);
        if (status == Ok) {
            r = rb_ary_new_capa(count);
            for (int i = 0; i < count; ++i) {
                rb_ary_push(r, SINGLE2NUM(stops[i]));
            }
        }
        ruby_xfree(stops);
        Check_Status(status);
    }
    return r;
}
static VALUE
gdip_strfmt_get_first_tab_offset(VALUE self)
{
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");
    float offset = 0.0f;
    float stops;
    Status status = format->GetTabStops(0, &offset, &stops);
    Check_Status(status);
    return SINGLE2NUM(offset);
}

static VALUE
gdip_strfmt_set_tab_stops(VALUE self, VALUE first_offset, VALUE tab_stops)
{
    Check_Frozen(self);
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");

    float first = 0.0f;
    gdip_arg_to_single(first_offset, &first, "The first argument should be Float.");
    if (_RB_ARRAY_P(tab_stops)) {
        int count = 0;
        float *ary = alloc_array_of_single(tab_stops, count);
        Status status = Ok;
        if (ary == NULL) {
            float tmp = 0.0f;
            status = format->SetTabStops(first, 0, &tmp);
        }
        else {
            status = format->SetTabStops(first, count, ary);
            ruby_xfree(ary);
        }
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Float.");
    }
    
    return self;
}

static VALUE
gdip_strfmt_set_digit_substitution(VALUE self, VALUE language, VALUE substitute)
{
    Check_Frozen(self);
    StringFormat *format = Data_Ptr<StringFormat *>(self);
    Check_NULL(format, "The StringFormat object does not exist.");

    LANGID langid = 0;
    gdip_arg_to_langid(language, &langid, "The first argument should be LangId.");
    int enumint = 0;
    gdip_arg_to_enumint(cStringDigitSubstitute, substitute, &enumint, ArgOptionAcceptInt, "The second argument should be StringDigitSubstitute.");
    Status status = format->SetDigitSubstitution(langid, static_cast<StringDigitSubstitute>(enumint));
    Check_Status(status);

    return self;
}

void
Init_stringformat()
{
    cStringFormat = rb_define_class_under(mGdiplus, "StringFormat", cGpObject);
    rb_define_alloc_func(cStringFormat, &typeddata_alloc_null<&tStringFormat>);
    rb_define_method(cStringFormat, "initialize", RUBY_METHOD_FUNC(gdip_strfmt_init), -1);

    CLASS_ATTR_R(cStringFormat, GenericDefault, generic_default, strfmt);
    CLASS_ATTR_R(cStringFormat, GenericTypographic, generic_typographic, strfmt);

    ATTR_RW(cStringFormat, Alignment, alignment, strfmt);
    ATTR_R(cStringFormat, DigitSubstitutionLanguage, digit_substitution_language, strfmt);
    ATTR_R(cStringFormat, DigitSubstitutionMethod, digit_substitution_method, strfmt);
    ATTR_RW(cStringFormat, FormatFlags, format_flags, strfmt);
    ATTR_RW(cStringFormat, HotkeyPrefix, hotkey_prefix, strfmt);
    ATTR_RW(cStringFormat, LineAlignment, line_alignment, strfmt);
    ATTR_RW(cStringFormat, Trimming, trimming, strfmt);

    rb_define_method(cStringFormat, "GetTabStops", RUBY_METHOD_FUNC(gdip_strfmt_get_tab_stops), 0);
    rb_define_alias(cStringFormat, "get_tab_stops", "GetTabStops");
    rb_define_method(cStringFormat, "GetFirstTabOffset", RUBY_METHOD_FUNC(gdip_strfmt_get_first_tab_offset), 0);
    rb_define_alias(cStringFormat, "get_first_tab_offset", "GetFirstTabOffset");
    rb_define_method(cStringFormat, "SetTabStops", RUBY_METHOD_FUNC(gdip_strfmt_set_tab_stops), 2);
    rb_define_alias(cStringFormat, "set_tab_stops", "SetTabStops");
    rb_define_method(cStringFormat, "SetDigitSubstitution", RUBY_METHOD_FUNC(gdip_strfmt_set_digit_substitution), 2);
    rb_define_alias(cStringFormat, "set_digit_substitution", "SetDigitSubstitution");
    
}