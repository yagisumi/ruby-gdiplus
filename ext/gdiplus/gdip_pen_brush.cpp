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
 *   @param color [Gdiplus::Color or Integer]
 *   @param width [Integer or Float]
 * @overload initialize(brush, width=1.0)
 *   @param brush [Gdiplus::Brush]
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
    if (gdip_arg_to_color(color_or_brush, &color, ArgOptionAcceptInt)) {
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
 * Gets or sets the width of this pen.
 * @return [Float]
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
 * @param width [Float or Integer]
 */
static VALUE
gdip_pen_set_width(VALUE self, VALUE width)
{
    Check_Frozen(self);
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    float w = 0.0f;
    gdip_arg_to_single(width, &w, "The argument should be Float or Integer.");
    pen->SetWidth(w);
    Check_LastStatus(pen);
    return self;
}

/**
 * Gets or sets a dash pattern offset.
 * @return [Float]
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
    Check_Frozen(self);
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    float oft = 0.0f;
    gdip_arg_to_single(offset, &oft, "The argument should be Float or Integer.");
    pen->SetDashOffset(oft);
    Check_LastStatus(pen);
    return self;
}

/**
 * Gets or Sets a miter limit.
 * @return [Float]
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
    Check_Frozen(self);
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    float lmt = 0.0f;
    gdip_arg_to_single(limit, &lmt, "The argument should be Float or Integer.");
    pen->SetMiterLimit(lmt);
    Check_LastStatus(pen);
    return self;
}

/**
 * Gets or sets the color of this pen.
 * @return [Color]
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
    Check_Frozen(self);
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    Color clr;
    gdip_arg_to_color(color, &clr);
    Check_Status(pen->SetColor(clr));
    return self;
 }

/**
 * Gets the type of this pen.
 * @return [PenType]
 */
static VALUE
gdip_pen_get_pen_type(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    return gdip_enumint_create(cPenType, pen->GetPenType());
}

/**
 * Gets or sets a compound array.
 * @return [Array<Float>]
 * @note Do not use this with a Inset alignment. 
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
    Check_Frozen(self);
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
 * Gets or sets a dash pattern.
 * @return [Array<Float>]
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
 */
static VALUE
gdip_pen_set_dash_pattern(VALUE self, VALUE ary)
{
    Check_Frozen(self);
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
            _VERBOSE("The argument should be Array of Float");
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
 * Gets or sets the alignment of this pen.
 * @return [PenAlignment]
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
 * @param align [PenAlignment]
 */
static VALUE
gdip_pen_set_alignment(VALUE self, VALUE align)
{
    Check_Frozen(self);
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    int enum_align = 0;
    gdip_arg_to_enumint(cPenAlignment, align, &enum_align, ArgOptionAcceptInt, "The argument should be PenAlignment.");
    PenAlignment pa = static_cast<PenAlignment>(enum_align);
    if (pa == PenAlignmentInset && pen->GetCompoundArrayCount() > 0) {
        _WARNING("Do not use PenAlignment.Inset with CompoundArray");
    }
    else {
        Status status = pen->SetAlignment(pa);
        Check_Status(status);
    }
    return self;
}

/**
 * Gets or sets a DashCap.
 * @return [DashCap]
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
 */
static VALUE
gdip_pen_set_dash_cap(VALUE self, VALUE cap)
{
    Check_Frozen(self);
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int enum_cap = 0;
    gdip_arg_to_enumint(cDashCap, cap, &enum_cap, ArgOptionAcceptInt, "The argument should be DashCap.");
    pen->SetDashCap(static_cast<DashCap>(enum_cap));
    Check_LastStatus(pen);
    return self;
}

/**
 * Gets or sets a DashStyel.
 * @return [DashStyle]
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
    Check_Frozen(self);
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int enum_style = 0;
    gdip_arg_to_enumint(cDashStyle, style, &enum_style, ArgOptionAcceptInt, "The argument should be Float or Integer.");
    pen->SetDashStyle(static_cast<DashStyle>(enum_style));
    Check_LastStatus(pen);
    return self;
}

/**
 * Gets or sets a LineCap.
 * @return [LineCap]
 */
static VALUE
gdip_pen_get_end_cap(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");
    return gdip_enumint_create(cLineCap, pen->GetEndCap());
}

/**
 * Gets or sets a LineCap.
 * @param cap [LineCap]
 */
static VALUE
gdip_pen_set_end_cap(VALUE self, VALUE cap)
{
    Check_Frozen(self);
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int enum_cap = 0;
    gdip_arg_to_enumint(cLineCap, cap, &enum_cap, ArgOptionAcceptInt, "The argument should be LineCap.");
    pen->SetEndCap(static_cast<LineCap>(enum_cap));
    Check_LastStatus(pen);
    return self;
}

/**
 * Gets or sets a LineJoin.
 * @return [LineJoin]
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
    Check_Frozen(self);
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int enum_join = 0;
    gdip_arg_to_enumint(cLineJoin, join, &enum_join, ArgOptionAcceptInt, "The argument should be LineJoin.");
    pen->SetLineJoin(static_cast<LineJoin>(enum_join));
    Check_LastStatus(pen);
    return self;
}

/**
 * Gets or sets a LineCap.
 * @return [LineCap]
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
    Check_Frozen(self);
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int enum_cap = 0;
    gdip_arg_to_enumint(cLineCap, cap, &enum_cap, ArgOptionAcceptInt, "The argument should be LineCap.");
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
 * Gets the tyep of this brush.
 * @return [BrushType]
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
 * @param color [Color, Integer]
 * @example
 *   include Gdiplus
 *   color = Color.new(255, 127, 63)
 *   brush = SolidBrush.new(color)
 *   brush = SolidBrush.new(0xFF006699)
 *   brush = SolidBrush.new(Color.Red)
 *   brush = SolidBrush.new(:Green)
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
    gdip_arg_to_color(color, &clr);
    _DATA_PTR(self) = gdip_obj_create<SolidBrush *>(new SolidBrush(clr));
    return self;
}

/**
 * Gets or sets the color of this brush.
 * @return [Color]
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
    Check_Frozen(self);
    SolidBrush *brush = Data_Ptr<SolidBrush *>(self);
    Check_NULL(brush, "This object does not exist");
    Color clr;
    gdip_arg_to_color(color, &clr);
    Check_Status(brush->SetColor(clr));
    return self;
}

/**
 * Document-class: Gdiplus::Pen
 * Pen.
 * @example
 *   include Gdiplus
 *   bitmap = Bitmap.new(300, 300)
 *   bitmap.draw {|g|
 *     pen = Pen.new(0x99ff3333, 10)
 *     pen.StartCap = :ArrowAnchor
 *     pen.EndCap = :ArrowAnchor
 *     g.DrawLine(pen, 30, 30, 270, 30)
 *     pen.color = :Green
 *     pen.LineJoin = LineJoin.Round
 *     pen.width = 3
 *     g.DrawRectangle(pen, 100, 100, 100, 100)
 *   }
 */
/**
 * Document-class: Gdiplus::SolidBrush
 * SolidBrush
 * @example
 *   include Gdiplus
 *   bitmap = Bitmap.new(300, 300)
 *   bitmap.draw {|g|
 *     brush = SolidBrush.new(Color.Blue)
 *     g.FillRectangle(brush, 100, 100, 100, 100)
 *   }
 */
void
Init_pen_brush()
{
    cPen = rb_define_class_under(mGdiplus, "Pen", cGpObject);
    rb_define_alloc_func(cPen, &typeddata_alloc_null<&tPen>);
    rb_define_method(cPen, "initialize", RUBY_METHOD_FUNC(gdip_pen_init), -1);
    ATTR_RW(cPen, Width, width, pen);
    ATTR_RW(cPen, Color, color, pen);
    ATTR_R(cPen, PenType, pen_type, pen);
    ATTR_RW(cPen, CompoundArray, compound_array, pen);
    ATTR_RW(cPen, DashPattern, dash_pattern, pen);
    ATTR_RW(cPen, DashOffset, dash_offset, pen);
    ATTR_RW(cPen, MiterLimit, miter_limit, pen);
    ATTR_RW(cPen, Alignment, alignment, pen);
    ATTR_RW(cPen, DashCap, dash_cap, pen);
    ATTR_RW(cPen, DashStyle, dash_style, pen);
    ATTR_RW(cPen, EndCap, end_cap, pen);
    ATTR_RW(cPen, LineJoin, line_join, pen);
    ATTR_RW(cPen, StartCap, start_cap, pen);

    cBrush = rb_define_class_under(mGdiplus, "Brush", cGpObject);
    rb_undef_alloc_func(cBrush);
    ATTR_R(cBrush, BrushType, brush_type, brush);

    cSolidBrush = rb_define_class_under(mGdiplus, "SolidBrush", cBrush);
    rb_define_alloc_func(cSolidBrush, &typeddata_alloc_null<&tBrush>);
    rb_define_method(cSolidBrush, "initialize", RUBY_METHOD_FUNC(gdip_solidbrush_init), 1);
    ATTR_RW(cSolidBrush, Color, color, solidbrush);
}

// ToDo: Pen#Transform
