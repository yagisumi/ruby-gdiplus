/*
 * gdip_font.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tFontFamily = _MAKE_DATA_TYPE(
    "FontFamily", 0, GDIP_OBJ_FREE(FontFamily *), NULL, NULL, &cFontFamily);

const rb_data_type_t tFontCollection = _MAKE_DATA_TYPE(
    "FontCollection", 0, GDIP_OBJ_FREE(FontCollection *), NULL, NULL, &cFontCollection);

const rb_data_type_t tInstalledFontCollection = _MAKE_DATA_TYPE(
    "InstalledFontCollection", 0, GDIP_OBJ_FREE(InstalledFontCollection *), NULL, &tFontCollection, &cInstalledFontCollection);

const rb_data_type_t tPrivateFontCollection = _MAKE_DATA_TYPE(
    "PrivateFontCollection", 0, GDIP_OBJ_FREE(PrivateFontCollection *), NULL, &tFontCollection, &cPrivateFontCollection);


const rb_data_type_t tFont = _MAKE_DATA_TYPE(
    "Font", 0, GDIP_OBJ_FREE(Font *), NULL, NULL, &cFont);



static VALUE
gdip_fontfamily_create(FontFamily *fontfamily)
{
    VALUE r = typeddata_alloc_null<&tFontFamily>(cFontFamily);
    _DATA_PTR(r) = gdip_obj_create(fontfamily);
    return r;
}


static VALUE vGenericSansSerif = Qnil;
static VALUE vGenericSerif = Qnil;
static VALUE vGenericMonospace = Qnil;

static VALUE
gdip_fontfamily_s_get_generic_sans_serif(VALUE self)
{
    if (RB_NIL_P(vGenericSansSerif)) {
        #if IFVC
            const FontFamily *family = FontFamily::GenericSansSerif();
            if (family == NULL) {
                return Qnil;
            }
            vGenericSansSerif = gdip_fontfamily_create(family->Clone());
        #else
            vGenericSansSerif = gdip_fontfamily_create(const_cast<FontFamily *>(FontFamily::GenericSansSerif()));
        #endif
        rb_gc_register_address(&vGenericSansSerif);
    }
    return vGenericSansSerif;
}

static VALUE
gdip_fontfamily_s_get_generic_serif(VALUE self)
{
    if (RB_NIL_P(vGenericSerif)) {
        #if IFVC
            const FontFamily *family = FontFamily::GenericSerif();
            if (family == NULL) {
                return Qnil;
            }
            vGenericSerif = gdip_fontfamily_create(family->Clone());
        #else
            vGenericSerif = gdip_fontfamily_create(const_cast<FontFamily *>(FontFamily::GenericSerif()));
        #endif
        rb_gc_register_address(&vGenericSerif);
    }
    return vGenericSerif;
}

static VALUE
gdip_fontfamily_s_get_generic_monospace(VALUE self)
{
    if (RB_NIL_P(vGenericMonospace)) {
        #if IFVC
            const FontFamily *family = FontFamily::GenericMonospace();
            if (family == NULL) {
                return Qnil;
            }
            vGenericMonospace = gdip_fontfamily_create(family->Clone());
        #else
            vGenericMonospace = gdip_fontfamily_create(const_cast<FontFamily *>(FontFamily::GenericMonospace()));
        #endif
        rb_gc_register_address(&vGenericMonospace);
    }
    return vGenericMonospace;
}

static VALUE
gdip_fontfamily_init(int argc, VALUE *argv, VALUE self)
{
    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }

    if (_DATA_PTR(self) != NULL) {
        _VERBOSE("This FontFamily object is already initialized.");
        return self;
    }
    
    if (argc == 1) {
        int gff;
        if (_RB_STRING_P(argv[0])) {
            VALUE wstr = util_utf16_str_new(argv[0]);
            _DATA_PTR(self) = gdip_obj_create(new FontFamily(RString_Ptr<WCHAR *>(wstr)));
            RB_GC_GUARD(wstr);
        }
        else if (gdip_arg_to_enumint(cGenericFontFamilies, argv[0], &gff)) {
            VALUE v_generic;
            switch (gff) {
                case 0: //GenericFontFamilySerif:
                    v_generic = gdip_fontfamily_s_get_generic_serif(cFontFamily);
                    break;
                case 1: //GenericFontFamilySansSerif:
                    v_generic = gdip_fontfamily_s_get_generic_sans_serif(cFontFamily);
                    break;
                case 2: //GenericFontFamilyMonospace:
                    v_generic = gdip_fontfamily_s_get_generic_monospace(cFontFamily);
                    break;
                default:
                    rb_raise(rb_eArgError, "The argument has a wrong value for GenericFontFamilies. (%d for 0..2)", gff);
                    break;
            }
            FontFamily *generic = Data_Ptr<FontFamily *>(v_generic);
            _DATA_PTR(self) = gdip_obj_create(generic->Clone());
        }
        else {
            rb_raise(rb_eTypeError, "The first argument should be String or GenericFontFamilies.");
        }
    }
    else if (argc == 2) {
        if (_RB_STRING_P(argv[0]) && _KIND_OF(argv[1], &tFontCollection)) {
            VALUE wstr = util_utf16_str_new(argv[0]);
            FontCollection *fontcol = Data_Ptr<FontCollection *>(argv[1]);
            _DATA_PTR(self) = gdip_obj_create(new FontFamily(RString_Ptr<WCHAR *>(wstr), fontcol));
            RB_GC_GUARD(wstr);
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be String and FontCollection.");
        }
    }

    return self;
}

static VALUE
gdip_fontfamily_get_name(VALUE self)
{
    FontFamily *fontfamily = Data_Ptr<FontFamily *>(self);
    Check_NULL(fontfamily, "The FontFamily object does not exist.");
    WCHAR wname[LF_FACESIZE];
    fontfamily->GetFamilyName(wname);
    return util_utf8_str_new_from_wstr(wname);
}


static VALUE
gdip_fontfamily_m_get_name(VALUE self, VALUE lang)
{
    FontFamily *fontfamily = Data_Ptr<FontFamily *>(self);
    Check_NULL(fontfamily, "The FontFamily object does not exist.");
    LANGID langid;
    gdip_arg_to_langid(lang, &langid, "The argument should be LangId or Integer.");
    WCHAR wname[LF_FACESIZE];
    Status status = fontfamily->GetFamilyName(wname, langid);
    Check_Status(status);
    return util_utf8_str_new_from_wstr(wname);
}


static VALUE
gdip_fontfamily_get_em_height(int argc, VALUE *argv, VALUE self)
{
    VALUE v_style = Qnil;
    rb_scan_args(argc, argv, "01", &v_style);

    FontFamily *fontfamily = Data_Ptr<FontFamily *>(self);
    Check_NULL(fontfamily, "This FontFamily object does not exist.");

    int style = FontStyleRegular;
    if (!RB_NIL_P(v_style)) {
        gdip_arg_to_enumint(cFontStyle, v_style, &style, "The argument should be FontStyle.", ArgOptionAcceptInt);
    }
    UINT16 h = fontfamily->GetEmHeight(style);
    return RB_UINT2NUM(h);
}

static VALUE
gdip_fontfamily_get_cell_ascent(int argc, VALUE *argv, VALUE self)
{
    VALUE v_style = Qnil;
    rb_scan_args(argc, argv, "01", &v_style);

    FontFamily *fontfamily = Data_Ptr<FontFamily *>(self);
    Check_NULL(fontfamily, "This FontFamily object does not exist.");

    int style = FontStyleRegular;
    if (!RB_NIL_P(v_style)) {
        gdip_arg_to_enumint(cFontStyle, v_style, &style, "The argument should be FontStyle.", ArgOptionAcceptInt);
    }
    UINT16 h = fontfamily->GetCellAscent(style);
    return RB_UINT2NUM(h);
}

static VALUE
gdip_fontfamily_get_cell_descent(int argc, VALUE *argv, VALUE self)
{
    VALUE v_style = Qnil;
    rb_scan_args(argc, argv, "01", &v_style);

    FontFamily *fontfamily = Data_Ptr<FontFamily *>(self);
    Check_NULL(fontfamily, "This FontFamily object does not exist.");

    int style = FontStyleRegular;
    if (!RB_NIL_P(v_style)) {
        gdip_arg_to_enumint(cFontStyle, v_style, &style, "The argument should be FontStyle.", ArgOptionAcceptInt);
    }
    UINT16 h = fontfamily->GetCellDescent(style);
    return RB_UINT2NUM(h);
}

static VALUE
gdip_fontfamily_get_line_spacing(int argc, VALUE *argv, VALUE self)
{
    VALUE v_style = Qnil;
    rb_scan_args(argc, argv, "01", &v_style);

    FontFamily *fontfamily = Data_Ptr<FontFamily *>(self);
    Check_NULL(fontfamily, "This FontFamily object does not exist.");

    int style = FontStyleRegular;
    if (!RB_NIL_P(v_style)) {
        gdip_arg_to_enumint(cFontStyle, v_style, &style, "The argument should be FontStyle.", ArgOptionAcceptInt);
    }
    UINT16 h = fontfamily->GetLineSpacing(style);
    return RB_UINT2NUM(h);
}

static VALUE
gdip_instfontcol_alloc(VALUE klass)
{
    InstalledFontCollection *ptr = gdip_obj_create(new InstalledFontCollection());
    dp("InstalledFontCollection alloc");
    VALUE r = _Data_Wrap_Struct(klass, &tInstalledFontCollection, ptr);
    return r;
}

static VALUE
gdip_privfontcol_alloc(VALUE klass)
{
    PrivateFontCollection *ptr = gdip_obj_create(new PrivateFontCollection());
    dp("PrivateFontCollection alloc");
    VALUE r = _Data_Wrap_Struct(klass, &tPrivateFontCollection, ptr);
    return r;
}

static VALUE
gdip_privfontcol_add_font_file(VALUE self, VALUE filename)
{
    PrivateFontCollection *privfontcol = Data_Ptr<PrivateFontCollection *>(self);
    Check_NULL(privfontcol, "This PrivateFontCollection does not exist.");

    VALUE wstr = util_utf16_str_new(filename);
    Status status = privfontcol->AddFontFile(RString_Ptr<WCHAR *>(wstr));
    RB_GC_GUARD(wstr);
    Check_Status(status);
    return self;
}

static VALUE
gdip_fontcol_get_families(VALUE self)
{
    FontCollection *fontcol = Data_Ptr<FontCollection *>(self);
    Check_NULL(fontcol, "The FontCollection object does not exist.");
    int count = fontcol->GetFamilyCount();
    
    FontFamily *families = new FontFamily[count];
    int num_found = 0;
    fontcol->GetFamilies(count, families, &num_found);
    VALUE r = rb_ary_new_capa(num_found);
    
    for (int i = 0; i < num_found; ++i) {
        FontFamily *fontfamily = families[i].Clone();
        if (fontfamily != NULL) {
            if (fontfamily->GetLastStatus() == Ok) {
                rb_ary_push(r, gdip_fontfamily_create(fontfamily));
            }
            else {
                delete fontfamily;
            }
        }
    }
    delete [] families;

    return r;
}

/**
 * @overload initialize(font, style)
 *   @param font [Font]
 *   @param style [FontStyle]
 * @overload initialize(family_or_name, size, unit=GraphicsUnit.Point)
 *   @param family_or_name [FontFamily or String]
 *   @param size [Float]
 *   @param unit [GraphicsUnit]
 * @overload initialize(family_or_name, size, style=FontStyle.Regular, unit=GraphicsUnit.Point)
 *   @param family_or_name [FontFamily or String]
 *   @param size [Float]
 *   @param style [FontStyle]
 *   @param unit [GraphicsUnit]
 */
static VALUE
gdip_font_init(int argc, VALUE *argv, VALUE self)
{
    if (argc < 2 || 4 < argc) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2..4)", argc);
    }

    Font *font = Data_Ptr<Font *>(self);
    if (font != NULL) {
        _VERBOSE("This font object is already initialized.");
        return self;
    }

    if (_KIND_OF(argv[0], &tFont)) {
        if (argc != 2) {
            rb_raise(rb_eArgError, "The number of arguments should be 2 if The first arugment is Font.");
        }
        Font *arg_font = Data_Ptr<Font *>(argv[0]);
        Check_NULL(arg_font, "The Font object of an argument does not exist.");
        FontFamily *family = new FontFamily();
        
        Status status = arg_font->GetFamily(family);
        if (status != Ok) {
            if (family != NULL) {
                delete family;
            }
            Check_Status(status);
        }
        VALUE v_family = gdip_fontfamily_create(family);

        int style = FontStyleRegular;
        if (!gdip_arg_to_enumint(cFontStyle, argv[1], &style)) {
            rb_raise(rb_eArgError, "The second argument should be FontStyle.");
        }

        _DATA_PTR(self) = gdip_obj_create(new Font(family, arg_font->GetSize(), style, arg_font->GetUnit()));
        RB_GC_GUARD(v_family);
    }
    else if (_RB_STRING_P(argv[0]) || _KIND_OF(argv[0], &tFontFamily)) {
        float size = 12.0f;
        gdip_arg_to_single(argv[1], &size, "The second argument should be Float or Integer.");
        Unit unit = UnitPixel;
        int style = FontStyleRegular;
        if (argc == 3) {
            if (!gdip_arg_to_enumint(cGraphicsUnit, argv[2], (int*)&unit) &&
                !gdip_arg_to_enumint(cFontStyle, argv[2], &style)) {
                rb_raise(rb_eTypeError, "The third argument should be GraphicsUnit or FontStyle.");
            }
        }
        else if (argc == 4) {
            gdip_arg_to_enumint(cFontStyle, argv[2], &style, "The third argument should be FontStyle.", ArgOptionAcceptInt);
            gdip_arg_to_enumint(cGraphicsUnit, argv[3], (int*)&unit, "The fourth argument should be GraphicsUnit.", ArgOptionAcceptInt);
        }
        if (_RB_STRING_P(argv[0])) {
            VALUE wstr = util_utf16_str_new(argv[0]);
            _DATA_PTR(self) = gdip_obj_create(new Font(RString_Ptr<WCHAR *>(wstr), size, style, unit));
            RB_GC_GUARD(wstr);
        }
        else {
            FontFamily *family = Data_Ptr<FontFamily *>(argv[0]);
            Check_NULL(family, "The FontFamily object does not exist.");
            _DATA_PTR(self) = gdip_obj_create(new Font(family, size, style, unit));
        }
    }
    else {
        rb_raise(rb_eTypeError, "The first argument should be Font, FontFamily or String.");
    }

    return self;
}

static VALUE
gdip_font_get_bold(VALUE self)
{
    Font *font = Data_Ptr<Font *>(self);
    Check_NULL(font, "This Font object does not exist.");
    int style = font->GetStyle();
    return style & FontStyleBold ? Qtrue : Qfalse;
}

static VALUE
gdip_font_get_italic(VALUE self)
{
    Font *font = Data_Ptr<Font *>(self);
    Check_NULL(font, "This Font object does not exist.");
    int style = font->GetStyle();
    return style & FontStyleItalic ? Qtrue : Qfalse;
}

static VALUE
gdip_font_get_strikeout(VALUE self)
{
    Font *font = Data_Ptr<Font *>(self);
    Check_NULL(font, "This Font object does not exist.");
    int style = font->GetStyle();
    return style & FontStyleStrikeout ? Qtrue : Qfalse;
}

static VALUE
gdip_font_get_underline(VALUE self)
{
    Font *font = Data_Ptr<Font *>(self);
    Check_NULL(font, "This Font object does not exist.");
    int style = font->GetStyle();
    return style & FontStyleUnderline ? Qtrue : Qfalse;
}

static VALUE
gdip_font_get_style(VALUE self)
{
    Font *font = Data_Ptr<Font *>(self);
    Check_NULL(font, "This Font object does not exist.");
    return gdip_enumint_create(cFontStyle, font->GetStyle());
}

static VALUE
gdip_font_get_unit(VALUE self)
{
    Font *font = Data_Ptr<Font *>(self);
    Check_NULL(font, "This Font object does not exist.");
    return gdip_enumint_create(cGraphicsUnit, font->GetUnit());
}

static VALUE
gdip_font_get_size(VALUE self)
{
    Font *font = Data_Ptr<Font *>(self);
    Check_NULL(font, "This Font object does not exist.");
    return SINGLE2NUM(font->GetSize());
}

static VALUE
gdip_font_get_font_family(VALUE self)
{
    Font *font = Data_Ptr<Font *>(self);
    Check_NULL(font, "This Font object does not exist.");
    FontFamily *family = new FontFamily();
    Status status = font->GetFamily(family);
    if (status != Ok) {
        delete family;
        Check_Status(status);
    }
    return gdip_fontfamily_create(family);
}

static VALUE
gdip_font_get_name(VALUE self)
{
    Font *font = Data_Ptr<Font *>(self);
    Check_NULL(font, "This Font object does not exist.");
    FontFamily *family = new FontFamily();
    Status status = font->GetFamily(family);
    if (status != Ok) {
        delete family;
        Check_Status(status);
    }

    WCHAR wstr[LF_FACESIZE];
    status = family->GetFamilyName(wstr);
    delete family;
    Check_Status(status);
    return util_utf8_str_new_from_wstr(wstr);
}

static float
gdip_font_get_height_i(Font *font)
{
    HDC hdc = GetDC(NULL);
    if (hdc == NULL) {
        rb_raise(rb_eRuntimeError, "Failed to GetDC.");
    }
    Graphics *g = Graphics::FromHDC(hdc);
    float h = font->GetHeight(g);
    delete g;
    ReleaseDC(NULL, hdc);
    return h;
}

static VALUE
gdip_font_get_height(VALUE self)
{
    Font *font = Data_Ptr<Font *>(self);
    Check_NULL(font, "This Font object does not exist.");
    return RB_INT2NUM(static_cast<int>(ceilf(gdip_font_get_height_i(font))));
}

static VALUE
gdip_font_m_get_height(int argc, VALUE *argv, VALUE self)
{
    if (argc != 0 && argc != 1) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    Font *font = Data_Ptr<Font *>(self);
    Check_NULL(font, "This Font object does not exist.");

    if (argc == 0) {
        return SINGLE2NUM(gdip_font_get_height_i(font));
    }
    else if (argc == 1) {
        float dpi;
        if (_KIND_OF(argv[0], &tGraphics)) {
            Graphics *g = Data_Ptr<Graphics *>(argv[0]);
            Check_NULL(g, "The Graphics object does not exist.");

            return SINGLE2NUM(font->GetHeight(g));
        }
        else if (gdip_arg_to_single(argv[0], &dpi)) {
            return SINGLE2NUM(font->GetHeight(dpi));
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Graphics or Float.");
        }
    }
    return Qnil;
}

static bool
test_font()
{
    InstalledFontCollection *fontcol = new InstalledFontCollection();
    if (fontcol == NULL) {
        return false;
    }
    else {
        int count = fontcol->GetFamilyCount();
        delete fontcol;
        return count == 0 ? false : true;
    }
}

static VALUE
gdip_instfontcol_s_broken(VALUE self)
{
    return test_font() ? Qfalse : Qtrue;
}


void
Init_font()
{
    cFontFamily = rb_define_class_under(mGdiplus, "FontFamily", cGpObject);
    rb_define_alloc_func(cFontFamily, &typeddata_alloc_null<&tFontFamily>);
    rb_define_method(cFontFamily, "initialize", RUBY_METHOD_FUNC(gdip_fontfamily_init), -1);
    CLASS_ATTR_R(cFontFamily, GenericSansSerif, generic_sans_serif, fontfamily);
    CLASS_ATTR_R(cFontFamily, GenericSerif, generic_serif, fontfamily);
    CLASS_ATTR_R(cFontFamily, GenericMonospace, generic_monospace, fontfamily);
    ATTR_R(cFontFamily, Name, name, fontfamily);
    rb_define_method(cFontFamily, "GetName", RUBY_METHOD_FUNC(gdip_fontfamily_m_get_name), 1);
    rb_define_alias(cFontFamily, "get_name", "GetName");
    rb_define_method(cFontFamily, "GetEmHeight", RUBY_METHOD_FUNC(gdip_fontfamily_get_em_height), -1);
    rb_define_alias(cFontFamily, "get_em_height", "GetEmHeight");
    rb_define_method(cFontFamily, "GetCellAscent", RUBY_METHOD_FUNC(gdip_fontfamily_get_cell_ascent), -1);
    rb_define_alias(cFontFamily, "get_cell_ascent", "GetCellAscent");
    rb_define_method(cFontFamily, "GetCellDescent", RUBY_METHOD_FUNC(gdip_fontfamily_get_cell_descent), -1);
    rb_define_alias(cFontFamily, "get_cell_descent", "GetCellDescent");
    rb_define_method(cFontFamily, "GetLineSpacing", RUBY_METHOD_FUNC(gdip_fontfamily_get_line_spacing), -1);
    rb_define_alias(cFontFamily, "get_line_spacing", "GetLineSpacing");

    cFontCollection = rb_define_class_under(mGdiplus, "FontCollection", cGpObject);
    rb_undef_alloc_func(cFontCollection);
    ATTR_R(cFontCollection, Families, families, fontcol);

    cInstalledFontCollection = rb_define_class_under(mGdiplus, "InstalledFontCollection", cFontCollection);
    rb_define_singleton_method(cInstalledFontCollection, "broken?", RUBY_METHOD_FUNC(gdip_instfontcol_s_broken), 0);
    rb_define_alloc_func(cInstalledFontCollection, gdip_instfontcol_alloc);
    cPrivateFontCollection = rb_define_class_under(mGdiplus, "PrivateFontCollection", cFontCollection);
    rb_define_alloc_func(cPrivateFontCollection, gdip_privfontcol_alloc);
    rb_define_method(cPrivateFontCollection, "AddFontFile", RUBY_METHOD_FUNC(gdip_privfontcol_add_font_file), 1);

    cFont = rb_define_class_under(mGdiplus, "Font", cGpObject);
    rb_define_alloc_func(cFont, &typeddata_alloc_null<&tFont>);
    rb_define_method(cFont, "initialize", RUBY_METHOD_FUNC(gdip_font_init), -1);
    ATTR_R_Q(cFont, Bold, bold, font);
    ATTR_R_Q(cFont, Italic, italic, font);
    ATTR_R_Q(cFont, Strikeout, strikeout, font);
    ATTR_R_Q(cFont, Underline, underline, font);
    ATTR_R(cFont, Style, style, font);
    ATTR_R(cFont, Unit, unit, font);
    ATTR_R(cFont, Size, size, font);
    ATTR_R(cFont, FontFamily, font_family, font);
    ATTR_R(cFont, Name, name, font);
    ATTR_R(cFont, Height, height, font);

    rb_define_method(cFont, "GetHeight", RUBY_METHOD_FUNC(gdip_font_m_get_height), -1);
    rb_define_alias(cFont, "get_height", "GetHeight");

    if (test_font() == false) {
        _WARNING(
            "\nIt seems that Font of gdiplus extension does not work properly on the combination of this ruby and windows.\n"
            "It will fail to load installed fonts.\n"
            "Use PrivateFontCollection#AddFontFile to get FontFamily.\n"
            "And someone please fix this problem.");
    }
}