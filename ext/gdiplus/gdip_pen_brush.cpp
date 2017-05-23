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

    if (_KIND_OF(color_or_brush, &tColor)) {
        ARGB argb = Data_Ptr_As<ARGB>(color_or_brush);
        Color color = _ARGB2COLOR(&argb);
        _DATA_PTR(self) = gdip_obj_create<Pen *>(new Pen(color, width));
    }
    else if (_RB_INTEGER_P(color_or_brush)) {
        ARGB argb = RB_NUM2UINT(color_or_brush);
        Color color = _ARGB2COLOR(&argb);
        _DATA_PTR(self) = gdip_obj_create<Pen *>(new Pen(color, width));
    }
    else if (_KIND_OF(color_or_brush, &tBrush)) {
        Brush *brush = Data_Ptr<Brush *>(color_or_brush);
        _DATA_PTR(self) = gdip_obj_create<Pen *>(new Pen(brush, width));
    }
    else {
        rb_raise(rb_eTypeError, "The 1st argument must be Color, Integer or Brush");
    }
    return self;
}

/**
 *
 * @overload width()
 *   @return [Float]
 */
static VALUE
gdip_pen_get_width(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    float w = pen->GetWidth();
    Check_LastStatus(pen);
    return DBL2NUM(static_cast<double>(w));
}

/**
 *
 * @overload width=(width)
 *   @param width [Float or Integer]
 */
static VALUE
gdip_pen_set_width(VALUE self, VALUE width)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    if (_RB_FLOAT_P(width)) {
        double w = NUM2DBL(width);
        pen->SetWidth(static_cast<float>(w));
    }
    else if (_RB_INTEGER_P(width)) {
        float w = 1.0f * RB_NUM2INT(width);
        pen->SetWidth(w);
    }
    else {
        rb_raise(rb_eTypeError, "The argument must be Float or Integer");
    }
    Check_LastStatus(pen);
    return self;
}

/**
 *
 * @overload PenType()
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
 * @overload Alignment()
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
 * @overload Alignment=(align)
 *   @param align [PenAlignment]
 *
 */
static VALUE
gdip_pen_set_alignment(VALUE self, VALUE align)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    Check_NULL(pen, "This object does not exist");

    int enum_align = gdip_enumint_to_int(cPenAlignment, align);
    pen->SetAlignment(static_cast<PenAlignment>(enum_align));
    Check_LastStatus(pen);
    return self;
}

/**
 *
 * @overload DashCap()
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
 * @overload DashCap=(cap)
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
 * @overload DashStyle()
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
 * @overload DashStyle=(style)
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
 * @overload EndCap()
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
 * @overload EndCap=(cap)
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
 * @overload LineJoin()
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
 * @overload LineJoin=(join)
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
 * @overload StartCap()
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
 * @overload StartCap=(cap)
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

    if (_KIND_OF(color, &tColor)) {
        ARGB argb = Data_Ptr_As<ARGB>(color);
        Color c = _ARGB2COLOR(&argb);
        _DATA_PTR(self) = gdip_obj_create<SolidBrush *>(new SolidBrush(c));
    }
    else if (_RB_INTEGER_P(color)) {
        ARGB argb = RB_NUM2UINT(color);
        Color c = _ARGB2COLOR(&argb);
        _DATA_PTR(self) = gdip_obj_create<SolidBrush *>(new SolidBrush(c));
    }
    else {
        rb_raise(rb_eTypeError, "The argument must be Color or Integer");
    }
    return self;
}

void
Init_pen_brush()
{
    cPen = rb_define_class_under(mGdiplus, "Pen", rb_cObject);
    rb_define_alloc_func(cPen, TYPEDDATA_ALLOC_NULL(Pen, &tPen));
    rb_define_method(cPen, "initialize", RUBY_METHOD_FUNC(gdip_pen_init), -1);
    rb_define_method(cPen, "gdiplus_id", RUBY_METHOD_FUNC(gdip_obj_id), 0);
    rb_define_method(cPen, "width", RUBY_METHOD_FUNC(gdip_pen_get_width), 0);
    rb_define_method(cPen, "width=", RUBY_METHOD_FUNC(gdip_pen_set_width), 1);
    rb_define_method(cPen, "Width", RUBY_METHOD_FUNC(gdip_pen_get_width), 0);
    rb_define_method(cPen, "Width=", RUBY_METHOD_FUNC(gdip_pen_set_width), 1);
    rb_define_method(cPen, "PenType", RUBY_METHOD_FUNC(gdip_pen_get_pen_type), 0);
    rb_define_method(cPen, "pen_type", RUBY_METHOD_FUNC(gdip_pen_get_pen_type), 0);
    
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

    cSolidBrush = rb_define_class_under(mGdiplus, "SolidBrush", cBrush);
    rb_define_alloc_func(cSolidBrush, TYPEDDATA_ALLOC_NULL(SolidBrush, &tBrush));
    rb_define_method(cSolidBrush, "initialize", RUBY_METHOD_FUNC(gdip_solidbrush_init), 1);
    rb_define_method(cSolidBrush, "gdiplus_id", RUBY_METHOD_FUNC(gdip_obj_id), 0);
}