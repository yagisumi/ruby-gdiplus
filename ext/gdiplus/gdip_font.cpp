/*
 * gdip_font.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */

const rb_data_type_t tFontFamily = _MAKE_DATA_TYPE(
    "FontFamily", 0, GDIP_OBJ_FREE(FontFamily *), NULL, NULL, &cFontFamily);

const rb_data_type_t tFontCollection = _MAKE_DATA_TYPE(
    "FontCollection", 0, GDIP_OBJ_FREE(FontCollection *), NULL, NULL, &cFontCollection);

const rb_data_type_t tFont = _MAKE_DATA_TYPE(
    "Font", 0, GDIP_OBJ_FREE(Font *), NULL, NULL, &cFont);

static VALUE
gdip_fontfamily_create(FontFamily *fontfamily)
{
    VALUE r = typeddata_alloc_null<&tFontFamily>(cFontFamily);
    _DATA_PTR(r) = gdip_obj_create(fontfamily);
    return r;
}

static VALUE
gdip_fontfamily_get_name(VALUE self)
{
    FontFamily *fontfamily = Data_Ptr<FontFamily *>(self);
    Check_NULL(fontfamily);
    WCHAR wname[LF_FACESIZE];
    fontfamily->GetFamilyName(wname);
    return util_utf8_str_new_from_wstr(wname);
}

static VALUE
gdip_instfontcol_alloc(VALUE klass)
{
    InstalledFontCollection *ptr = gdip_obj_create(new InstalledFontCollection());
    dp("FontCollection alloc (InstalledFontCollection)");
    r = _Data_Wrap_Struct(klass, &tFontCollection, ptr);
    return r;
}

static VALUE
gdip_fontcol_get_families(VALUE self)
{
    FontCollection *fontcol = Data_Ptr<FontCollection *>(self);
    int count = fontcol->GetFamilyCount();

    FontFamily *families = new FontFamily[count];
    int num_found = 0;
    fontcol->GetFamilies(count, families, num_found);
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

void
Init_font()
{
    cFontFamily = rb_define_class_under(mGdiplus, "FontFamily", cGpObject);
    rb_define_alloc_func(cFontFamily, &typeddata_alloc_null<&tFontFamily>);
    ATTR_R(cFontFamily, Name, name, fontfamily);

    cFontCollection = rb_define_class_under(mGdiplus, "FontCollection", cGpObject);
    rb_undef_alloc_func(cFontCollection);
    ATTR_R(cFontCollection, Families, families, fontcol);


    VALUE cInstalledFontCollection = rb_define_class_under(mGdiplus, "InstalledFontCollection", cFontCollection);
    rb_define_alloc_func(cInstalledFontCollection, &typeddata_alloc_null<&tFontCollection>);


}