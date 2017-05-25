/*
 * gdip_pen_brush.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"



const rb_data_type_t tPen = _MAKE_DATA_TYPE(
    "Pen", 0, GDIP_OBJ_FREE(Pen *), NULL, NULL, &cPen);

/**
 *
 * @overload initialize(color, width=1.0)
 *   @param color [Color or Integer]
 *   @param width [Integer or Float]
 * @overload initialize(brush, width=1.0)
 *   @param brush [Brush]
 *   @param width [Integer or Float]
 *
 */
static VALUE
gdip_pen_init(int argc, VALUE *argv, VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    if (pen != NULL) {
        _VERBOSE("Gdiplus::Pen already initialized");
        return self;
    }

    float width = 1.0f;
    VALUE color_or_brush, width_v;
    rb_scan_args(argc, argv, "11", &color_or_brush, &width_v);
    if (!RB_NIL_P(width_v)) {
        if (_RB_FLOAT_P(width_v)) {
            width = static_cast<float>(NUM2DBL(width_v));
        }
        else if (_RB_INTEGER_P(width_v)) {
            width = 1.0f * RB_NUM2INT(width_v);
        }
        else {
            rb_raise(rb_eTypeError, "The 2nd argument must be Integer or Float");
        }
    }

    Color color;
    if (gdip_value2color(color_or_brush, &color, false, false)) {
        _DATA_PTR(self) = gdip_obj_create<Pen *>(new Pen(color, width));
    }
    else if (_KIND_OF(color_or_brush, &tBrush)) {
        Brush *brush = Data_Ptr<Brush *>(color_or_brush);
        _DATA_PTR(self) = gdip_obj_create<Pen *>(new Pen(brush, width));
    }
    else {
        rb_raise(rb_eTypeError, "The first argument must be Color, Integer or Brush");
    }
    return self;
}

/**
 *
 *   @return [Float]
 */
static VALUE
gdip_pen_get_width(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    float w = pen->GetWidth();
    Check_LastStatus(pen);
    return SINGLE2NUM(w);
}

/**
 *
 *   @param width [Float or Integer]
 */
static VALUE
gdip_pen_set_width(VALUE self, VALUE width)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    float w = 0.0f;
    gdip_value2single(width, &w);
    pen->SetWidth(w);
    Check_LastStatus(pen);
    return self;
}

/**
 *
 *   @return [Float]
 */
static VALUE
gdip_pen_get_dash_offset(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    float oft = pen->GetDashOffset();
    Check_LastStatus(pen);
    return SINGLE2NUM(oft);
}

/**
 *
 *   @param offset [Float or Integer]
 */
static VALUE
gdip_pen_set_dash_offset(VALUE self, VALUE offset)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    float oft = 0.0f;
    gdip_value2single(offset, &oft);
    pen->SetDashOffset(oft);
    Check_LastStatus(pen);
    return self;
}

/**
 *
 *   @return [Float]
 */
static VALUE
gdip_pen_get_miter_limit(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    float lmt = pen->GetMiterLimit();
    Check_LastStatus(pen);
    return SINGLE2NUM(lmt);
}

/**
 *
 *   @param offset [Float or Integer]
 */
static VALUE
gdip_pen_set_miter_limit(VALUE self, VALUE limit)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    float lmt = 0.0f;
    gdip_value2single(limit, &lmt);
    pen->SetMiterLimit(lmt);
    Check_LastStatus(pen);
    return self;
}

/**
 *
 * @return [Color]
 * 
 */
static VALUE
gdip_pen_get_color(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    Color color;
    Check_Status(pen->GetColor(&color));
    return gdip_color_create(color);
 }
/**
 *
 * @param color [Color]
 * 
 */
static VALUE
gdip_pen_set_color(VALUE self, VALUE color)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    Color clr;
    gdip_value2color(color, &clr);
    Check_Status(pen->SetColor(clr));
    return self;
 }

/**
 *
 *   @return [PenType]
 */
static VALUE
gdip_pen_get_pen_type(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    return gdip_enumint_create(cPenType, pen->GetPenType());
}

/**
 *
 * @return [Array<Float>]
 * 
 */
static VALUE
gdip_pen_get_compound_array(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int count = pen->GetCompoundArrayCount();
    if (count <= 0) {
        return rb_ary_new();
    }
    VALUE r = rb_ary_new_capa(count);
    float *array = static_cast<float *>(ruby_xmalloc(count * sizeof(float)));
    Status status = pen->GetCompoundArray(array, count);
    if (status != Ok) {
        ruby_xfree(array);
        Check_Status(status);
    }
    else {
        for (int i = 0; i < count; ++i) {
            rb_ary_push(r, SINGLE2NUM(array[i]));
        }
        ruby_xfree(array);
    }
    return r;
}

/**
 *
 * @param ary [Array<Float>]
 * 
 */
static VALUE
gdip_pen_set_compound_array(VALUE self, VALUE ary)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    if (!_RB_ARRAY_P(ary)) {
        rb_raise(rb_eTypeError, "An argument must be Array of Float");
    }
    if (pen->GetAlignment() == PenAlignmentInset) {
        _WARNING("Don't set CompoundArray if the pen alignment is Inset");
        return self;
    }
    
    int count = 0;
    bool verbose = false;
    for (int i = 0; i < RARRAY_LEN(ary); ++i) {
        VALUE num = rb_ary_entry(ary, i);
        if (_RB_FLOAT_P(num)) {
            ++count;
        }
        else if (!verbose) {
            _VERBOSE("An argument must be Array of Float");
            verbose = true;
        }
    }
    dp("count: %d", count);
    if (count == 0) {
        rb_raise(rb_eTypeError, "An argument must be Array of Float");
        //Status status = pen->SetCompoundArray(NULL, 0);
        //Check_Status(status); // InvalidParameter
    }
    else {
        int index = 0;
        verbose = false;
        float *array = static_cast<float *>(ruby_xmalloc(count * sizeof(float)));
        for (int i = 0; i < RARRAY_LEN(ary); ++i) {
            VALUE num = rb_ary_entry(ary, i);
            if (_RB_FLOAT_P(num)) {
                float flt = NUM2SINGLE(num);
                array[index] = flt;
                ++index;
                if (!verbose && (flt < 0.0 || 1.0 < flt)) {
                    _VERBOSE("Tje value in CompoundArray must be 0.0-1.0");
                    verbose = true;
                }
            }
        }

        Status status = pen->SetCompoundArray(array, count);
        ruby_xfree(array);
        Check_Status(status);
    }

    return self;
}

/**
 *
 * @return [Array<Float>]
 * 
 */
static VALUE
gdip_pen_get_dash_pattern(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int count = pen->GetDashPatternCount();
    if (count <= 0) {
        return rb_ary_new();
    }
    VALUE r = rb_ary_new_capa(count);
    float *array = static_cast<float *>(ruby_xmalloc(count * sizeof(float)));
    Status status = pen->GetDashPattern(array, count);
    if (status != Ok) {
        ruby_xfree(array);
        Check_Status(status);
    }
    else {
        for (int i = 0; i < count; ++i) {
            rb_ary_push(r, SINGLE2NUM(array[i]));
        }
        ruby_xfree(array);
    }
    return r;
}

/**
 *
 * @param ary [Array<Float>]
 * 
 */
static VALUE
gdip_pen_set_dash_pattern(VALUE self, VALUE ary)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    if (!_RB_ARRAY_P(ary)) {
        rb_raise(rb_eTypeError, "An argument must be Array of Float");
    }

    int count = 0;
    bool verbose = false;
    for (int i = 0; i < RARRAY_LEN(ary); ++i) {
        VALUE num = rb_ary_entry(ary, i);
        if (_RB_FLOAT_P(num)) {
            ++count;
        }
        else if (!verbose) {
            _VERBOSE("An argument must be Array of Float");
            verbose = true;
        }
    }

    if (count == 0) {
        //rb_raise(rb_eTypeError, "An argument must be Array of Float");
        Status status = pen->SetDashPattern(NULL, 0);
        Check_Status(status);
    }
    else {
        int index = 0;
        float *array = static_cast<float *>(ruby_xmalloc(count * sizeof(float)));
        for (int i = 0; i < RARRAY_LEN(ary); ++i) {
            VALUE num = rb_ary_entry(ary, i);
            if (_RB_FLOAT_P(num)) {
                array[index] = NUM2SINGLE(num);
                ++index;
            }
        }

        Status status = pen->SetDashPattern(array, count);
        ruby_xfree(array);
        Check_Status(status);
    }
    
    return self;
}

/**
 *
 *   @return [PenAlignment]
 *
 */
static VALUE
gdip_pen_get_alignment(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    return gdip_enumint_create(cPenAlignment, pen->GetAlignment());
}

/**
 *
 *   @param align [PenAlignment]
 *
 */
static VALUE
gdip_pen_set_alignment(VALUE self, VALUE align)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    int enum_align = gdip_enumint_to_int(cPenAlignment, align);
    PenAlignment pa = static_cast<PenAlignment>(enum_align);
    if (pa == PenAlignmentInset && pen->GetCompoundArrayCount() > 0) {
        _WARNING("Do not use PenAlignment.Inset with CompoundArray");
    }
    else {
        pen->SetAlignment(pa);
        Check_LastStatus(pen);
    }
    return self;
}

/**
 *
 *   @return [DashCap]
 *
 */
static VALUE
gdip_pen_get_dash_cap(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    return gdip_enumint_create(cDashCap, pen->GetDashCap());
}

/**
 *
 *   @param cap [DashCap]
 *
 */
static VALUE
gdip_pen_set_dash_cap(VALUE self, VALUE cap)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int enum_cap = gdip_enumint_to_int(cDashCap, cap);
    pen->SetDashCap(static_cast<DashCap>(enum_cap));
    Check_LastStatus(pen);
    return self;
}

/**
 *
 *   @return [DashStyle]
 *
 */
static VALUE
gdip_pen_get_dash_style(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    return gdip_enumint_create(cDashStyle, pen->GetDashStyle());
}

/**
 *
 *   @param style [DashStyle]
 *
 */
static VALUE
gdip_pen_set_dash_style(VALUE self, VALUE style)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int enum_style = gdip_enumint_to_int(cDashStyle, style);
    pen->SetDashStyle(static_cast<DashStyle>(enum_style));
    Check_LastStatus(pen);
    return self;
}

/**
 *
 *   @return [LineCap]
 *
 */
static VALUE
gdip_pen_get_end_cap(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    return gdip_enumint_create(cLineCap, pen->GetEndCap());
}

/**
 *
 *   @param cap [LineCap]
 *
 */
static VALUE
gdip_pen_set_end_cap(VALUE self, VALUE cap)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int enum_cap = gdip_enumint_to_int(cLineCap, cap);
    pen->SetEndCap(static_cast<LineCap>(enum_cap));
    Check_LastStatus(pen);
    return self;
}

/**
 *
 *   @return [LineJoin]
 *
 */
static VALUE
gdip_pen_get_line_join(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    return gdip_enumint_create(cLineJoin, pen->GetLineJoin());
}

/**
 *
 *   @param join [LineJoin]
 *
 */
static VALUE
gdip_pen_set_line_join(VALUE self, VALUE join)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int enum_join = gdip_enumint_to_int(cLineJoin, join);
    pen->SetLineJoin(static_cast<LineJoin>(enum_join));
    Check_LastStatus(pen);
    return self;
}

/**
 *
 *   @return [LineCap]
 *
 */
static VALUE
gdip_pen_get_start_cap(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    return gdip_enumint_create(cLineCap, pen->GetStartCap());
}

/**
 *
 *   @param cap [LineCap]
 *
 */
static VALUE
gdip_pen_set_start_cap(VALUE self, VALUE cap)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int enum_cap = gdip_enumint_to_int(cLineCap, cap);
    pen->SetStartCap(static_cast<LineCap>(enum_cap));
    Check_LastStatus(pen);
    return self;
}

////
//  //
//////
//  ///
//   //
/////
const rb_data_type_t tBrush = _MAKE_DATA_TYPE(
    "Brush", 0, GDIP_OBJ_FREE(Brush *), NULL, NULL, &cBrush);

/**
 *
 *   @return [PenType]
 */
static VALUE
gdip_brush_get_brush_type(VALUE self)
{
    Brush *brush = Data_Ptr<Brush *>(self);
    Check_NULL(brush, "This object does not exist");
    return gdip_enumint_create(cBrushType, brush->GetType());
}

/**
 *
 * @overload initialize(color)
 *   @param color [Color or Integer]
 * 
 */
static VALUE
gdip_solidbrush_init(VALUE self, VALUE color)
{
    SolidBrush *brush = Data_Ptr<SolidBrush *>(self);
    if (brush != NULL) {
        _VERBOSE("Gdiplus::SolidBrush already initialized");
        return self;
    }
    Color clr;
    gdip_value2color(color, &clr);
    _DATA_PTR(self) = gdip_obj_create<SolidBrush *>(new SolidBrush(clr));
    return self;
}

/**
 *
 * @return [Color]
 * 
 */
 static VALUE
gdip_solidbrush_get_color(VALUE self)
{
    SolidBrush *brush = Data_Ptr<SolidBrush *>(self);
    Check_NULL(brush, "This object does not exist");
    Color color;
    Check_Status(brush->GetColor(&color));
    return gdip_color_create(color);
}

/**
 *
 * @param color [Color]
 * 
 */
 static VALUE
gdip_solidbrush_set_color(VALUE self, VALUE color)
{
    SolidBrush *brush = Data_Ptr<SolidBrush *>(self);
    Check_NULL(brush, "This object does not exist");
    Color clr;
    gdip_value2color(color, &clr);
    Check_Status(brush->SetColor(clr));
    return self;
}


void
Init_pen_brush()
{
    cPen = rb_define_class_under(mGdiplus, "Pen", rb_cObject);
    rb_define_alloc_func(cPen, &typeddata_alloc_null<&tPen>);
    rb_define_method(cPen, "initialize", RUBY_METHOD_FUNC(gdip_pen_init), -1);
    rb_define_method(cPen, "gdiplus_id", RUBY_METHOD_FUNC(gdip_obj_id), 0);
    rb_define_method(cPen, "width", RUBY_METHOD_FUNC(gdip_pen_get_width), 0);
    rb_define_method(cPen, "width=", RUBY_METHOD_FUNC(gdip_pen_set_width), 1);
    rb_define_method(cPen, "Width", RUBY_METHOD_FUNC(gdip_pen_get_width), 0);
    rb_define_method(cPen, "Width=", RUBY_METHOD_FUNC(gdip_pen_set_width), 1);
    rb_define_method(cPen, "color", RUBY_METHOD_FUNC(gdip_pen_get_color), 0);
    rb_define_method(cPen, "color=", RUBY_METHOD_FUNC(gdip_pen_set_color), 1);
    rb_define_method(cPen, "Color", RUBY_METHOD_FUNC(gdip_pen_get_color), 0);
    rb_define_method(cPen, "Color=", RUBY_METHOD_FUNC(gdip_pen_set_color), 1);
    rb_define_method(cPen, "PenType", RUBY_METHOD_FUNC(gdip_pen_get_pen_type), 0);
    rb_define_method(cPen, "pen_type", RUBY_METHOD_FUNC(gdip_pen_get_pen_type), 0);
    rb_define_method(cPen, "compound_array", RUBY_METHOD_FUNC(gdip_pen_get_compound_array), 0);
    rb_define_method(cPen, "compound_array=", RUBY_METHOD_FUNC(gdip_pen_set_compound_array), 1);
    rb_define_method(cPen, "CompoundArray", RUBY_METHOD_FUNC(gdip_pen_get_compound_array), 0);
    rb_define_method(cPen, "CompoundArray=", RUBY_METHOD_FUNC(gdip_pen_set_compound_array), 1);
    rb_define_method(cPen, "dash_pattern", RUBY_METHOD_FUNC(gdip_pen_get_dash_pattern), 0);
    rb_define_method(cPen, "dash_pattern=", RUBY_METHOD_FUNC(gdip_pen_set_dash_pattern), 1);
    rb_define_method(cPen, "DashPattern", RUBY_METHOD_FUNC(gdip_pen_get_dash_pattern), 0);
    rb_define_method(cPen, "DashPattern=", RUBY_METHOD_FUNC(gdip_pen_set_dash_pattern), 1);
    rb_define_method(cPen, "dash_offset", RUBY_METHOD_FUNC(gdip_pen_get_dash_offset), 0);
    rb_define_method(cPen, "dash_offset=", RUBY_METHOD_FUNC(gdip_pen_set_dash_offset), 1);
    rb_define_method(cPen, "DashOffset", RUBY_METHOD_FUNC(gdip_pen_get_dash_offset), 0);
    rb_define_method(cPen, "DashOffset=", RUBY_METHOD_FUNC(gdip_pen_set_dash_offset), 1);
    rb_define_method(cPen, "miter_limit", RUBY_METHOD_FUNC(gdip_pen_get_miter_limit), 0);
    rb_define_method(cPen, "miter_limit=", RUBY_METHOD_FUNC(gdip_pen_set_miter_limit), 1);
    rb_define_method(cPen, "MiterLimit", RUBY_METHOD_FUNC(gdip_pen_get_miter_limit), 0);
    rb_define_method(cPen, "MiterLimit=", RUBY_METHOD_FUNC(gdip_pen_set_miter_limit), 1);

    rb_define_method(cPen, "Alignment", RUBY_METHOD_FUNC(gdip_pen_get_alignment), 0);
    rb_define_method(cPen, "alignment", RUBY_METHOD_FUNC(gdip_pen_get_alignment), 0);
    rb_define_method(cPen, "Alignment=", RUBY_METHOD_FUNC(gdip_pen_set_alignment), 1);
    rb_define_method(cPen, "alignment=", RUBY_METHOD_FUNC(gdip_pen_set_alignment), 1);
    rb_define_method(cPen, "DashCap", RUBY_METHOD_FUNC(gdip_pen_get_dash_cap), 0);
    rb_define_method(cPen, "dash_cap", RUBY_METHOD_FUNC(gdip_pen_get_dash_cap), 0);
    rb_define_method(cPen, "DashCap=", RUBY_METHOD_FUNC(gdip_pen_set_dash_cap), 1);
    rb_define_method(cPen, "dash_cap=", RUBY_METHOD_FUNC(gdip_pen_set_dash_cap), 1);
    rb_define_method(cPen, "DashStyle", RUBY_METHOD_FUNC(gdip_pen_get_dash_style), 0);
    rb_define_method(cPen, "dash_style", RUBY_METHOD_FUNC(gdip_pen_get_dash_style), 0);
    rb_define_method(cPen, "DashStyle=", RUBY_METHOD_FUNC(gdip_pen_set_dash_style), 1);
    rb_define_method(cPen, "dash_style=", RUBY_METHOD_FUNC(gdip_pen_set_dash_style), 1);
    rb_define_method(cPen, "EndCap", RUBY_METHOD_FUNC(gdip_pen_get_end_cap), 0);
    rb_define_method(cPen, "end_cap", RUBY_METHOD_FUNC(gdip_pen_get_end_cap), 0);
    rb_define_method(cPen, "EndCap=", RUBY_METHOD_FUNC(gdip_pen_set_end_cap), 1);
    rb_define_method(cPen, "end_cap=", RUBY_METHOD_FUNC(gdip_pen_set_end_cap), 1);
    rb_define_method(cPen, "LineJoin", RUBY_METHOD_FUNC(gdip_pen_get_line_join), 0);
    rb_define_method(cPen, "line_join", RUBY_METHOD_FUNC(gdip_pen_get_line_join), 0);
    rb_define_method(cPen, "LineJoin=", RUBY_METHOD_FUNC(gdip_pen_set_line_join), 1);
    rb_define_method(cPen, "line_join=", RUBY_METHOD_FUNC(gdip_pen_set_line_join), 1);
    rb_define_method(cPen, "StartCap", RUBY_METHOD_FUNC(gdip_pen_get_start_cap), 0);
    rb_define_method(cPen, "start_cap", RUBY_METHOD_FUNC(gdip_pen_get_start_cap), 0);
    rb_define_method(cPen, "StartCap=", RUBY_METHOD_FUNC(gdip_pen_set_start_cap), 1);
    rb_define_method(cPen, "start_cap=", RUBY_METHOD_FUNC(gdip_pen_set_start_cap), 1);

    cBrush = rb_define_class_under(mGdiplus, "Brush", rb_cObject);
    rb_undef_alloc_func(cBrush);
    rb_define_method(cBrush, "BrushType", RUBY_METHOD_FUNC(gdip_brush_get_brush_type), 0);
    rb_define_method(cBrush, "brush_type", RUBY_METHOD_FUNC(gdip_brush_get_brush_type), 0);
    

    cSolidBrush = rb_define_class_under(mGdiplus, "SolidBrush", cBrush);
    rb_define_alloc_func(cSolidBrush, &typeddata_alloc_null<&tBrush>);
    rb_define_method(cSolidBrush, "initialize", RUBY_METHOD_FUNC(gdip_solidbrush_init), 1);
    rb_define_method(cSolidBrush, "gdiplus_id", RUBY_METHOD_FUNC(gdip_obj_id), 0);
    rb_define_method(cSolidBrush, "color", RUBY_METHOD_FUNC(gdip_solidbrush_get_color), 0);
    rb_define_method(cSolidBrush, "color=", RUBY_METHOD_FUNC(gdip_solidbrush_set_color), 1);
    rb_define_method(cSolidBrush, "Color", RUBY_METHOD_FUNC(gdip_solidbrush_get_color), 0);
    rb_define_method(cSolidBrush, "Color=", RUBY_METHOD_FUNC(gdip_solidbrush_set_color), 1);
}

// ToDo: Pen#Transform
