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
gdip_pen_init(VALUE self)
{
    Pen *pen = Data_Ptr<Pen *>(self);
    if (bmp != NULL) {
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
        Color color = Data_Ptr_As<Color>(color_or_brush);
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
    Check_NULL(pen, "this object does not exist");

    float w = pen->GetWidth();
    Check_Status(pen);
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
    Check_NULL(pen, "this object does not exist");

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
    Check_Status(pen);
    return self;
}

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
        Color c = Data_Ptr_As<Color>(color);
        _DATA_PTR(self) = gdip_obj_create<SolidBrush *>(new SolidBrush(c));
    }
    else if (_RB_INTEGER_P(color)) {
        ARGB argb = RB_NUM2UINT(color);
        Color c = _ARGB2COLOR(argb);
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
    rb_define_method(cPen, "width", RUBY_METHOD_FUNC(gdip_pen_get_width), 0);
    rb_define_method(cPen, "width=", RUBY_METHOD_FUNC(gdip_pen_set_width), 1);

    cBrush = rb_define_class_under(mGdiplus, "Brush", rb_cObject);
    rb_undef_alloc_func(cBrush);

    VALUE cSolidBrush = rb_define_class_under(mGdiplus, "SolidBrush", rb_cBrush);
    rb_define_alloc_func(cSolidBrush, TYPEDDATA_ALLOC_NULL(Brush, &tBrush));
    rb_define_method(cSolidBrush, "initialize", RUBY_METHOD_FUNC(gdip_solidbrush_init), 1);
}