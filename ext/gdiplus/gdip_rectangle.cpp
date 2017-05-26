/*
 * gdip_rectangle.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tPoint = _MAKE_DATA_TYPE(
    "Point", 0, GDIP_DEFAULT_FREE(Point), &typeddata_size<Point>, NULL, &cPoint);

const rb_data_type_t tPointF = _MAKE_DATA_TYPE(
    "PointF", 0, GDIP_DEFAULT_FREE(PointF), &typeddata_size<PointF>, NULL, &cPointF);

const rb_data_type_t tSize = _MAKE_DATA_TYPE(
    "Size", 0, GDIP_DEFAULT_FREE(Size), &typeddata_size<Size>, NULL, &cSize);

const rb_data_type_t tSizeF = _MAKE_DATA_TYPE(
    "SizeF", 0, GDIP_DEFAULT_FREE(SizeF), &typeddata_size<SizeF>, NULL, &cSizeF);

const rb_data_type_t tRectangle = _MAKE_DATA_TYPE(
    "Rectangle", 0, GDIP_DEFAULT_FREE(Rect), &typeddata_size<Rect>, NULL, &cRectangle);

const rb_data_type_t tRectangleF = _MAKE_DATA_TYPE(
    "RectangleF", 0, GDIP_DEFAULT_FREE(RectF), &typeddata_size<RectF>, NULL, &cRectangleF);



static VALUE
gdip_point_init(int argc, VALUE *argv, VALUE self)
{
    if (argc != 0 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0 or 2)", argc);
    }
    Point *point = Data_Ptr<Point *>(self);
    if (argc == 0) return self;
    else if (Integer_p(argv[0], argv[1])) {
        point->X = RB_NUM2INT(argv[0]);
        point->Y = RB_NUM2INT(argv[1]);
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Integer.");
    }
    return self;
}

static VALUE
gdip_point_inspect(VALUE self)
{
    Point *point = Data_Ptr<Point *>(self);
    VALUE r = util_utf8_sprintf("#<%s X=%d, Y=%d>", __class__(self), point->X, point->Y);
    return r;
}

static VALUE
gdip_point_get_x(VALUE self)
{
    Point *point = Data_Ptr<Point *>(self);
    return RB_INT2NUM(point->X);
}

static VALUE
gdip_point_set_x(VALUE self, VALUE num)
{
    Point *point = Data_Ptr<Point *>(self);
    if (_RB_INTEGER_P(num)) {
        point->X = RB_NUM2INT(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Integer.");
    }
    return self;
}

static VALUE
gdip_point_get_y(VALUE self)
{
    Point *point = Data_Ptr<Point *>(self);
    return RB_INT2NUM(point->Y);
}

static VALUE
gdip_point_set_y(VALUE self, VALUE num)
{
    Point *point = Data_Ptr<Point *>(self);
    if (_RB_INTEGER_P(num)) {
        point->Y = RB_NUM2INT(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Integer.");
    }
    return self;
}

static VALUE
gdip_point_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    else if (!_KIND_OF(other, &tPoint)) return Qfalse;
    else {
        Point *po1 = Data_Ptr<Point *>(self);
        Point *po2 = Data_Ptr<Point *>(other);
        return (po1->X == po2->X && po1->Y == po2->Y) ? Qtrue : Qfalse;
    }
}

static VALUE
gdip_point_add(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tPoint)) {
        VALUE r = typeddata_alloc<Point, &tPoint>(cPoint);
        Point *po_r = Data_Ptr<Point *>(r);
        Point *po_self = Data_Ptr<Point *>(self);
        Point *po_other = Data_Ptr<Point *>(other);
        po_r->X = po_self->X + po_other->X;
        po_r->Y = po_self->Y + po_other->Y;
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Point or Size.");
        return Qnil;
    }
}


static VALUE
gdip_pointf_init(int argc, VALUE *argv, VALUE self)
{
    if (argc != 0 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0 or 2)", argc);
    }
    PointF *point = Data_Ptr<PointF *>(self);
    if (argc == 0) {
        point->X = 0.0f;
        point->Y = 0.0f;
    }
    else if (Float_p(argv[0], argv[1])) {
        point->X = NUM2SINGLE(argv[0]);
        point->Y = NUM2SINGLE(argv[1]);
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Float.");
    }
    return self;
}

static VALUE
gdip_pointf_inspect(VALUE self)
{
    PointF *point = Data_Ptr<PointF *>(self);
    VALUE r = util_utf8_sprintf("#<%s X=%f, Y=%f>", __class__(self), point->X, point->Y);
    return r;
}

static VALUE
gdip_pointf_get_x(VALUE self)
{
    PointF *point = Data_Ptr<PointF *>(self);
    return SINGLE2NUM(point->X);
}

static VALUE
gdip_pointf_set_x(VALUE self, VALUE num)
{
    PointF *point = Data_Ptr<PointF *>(self);
    if (Float_p(num)) {
        point->X = NUM2SINGLE(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Float.");
    }
    return self;
}

static VALUE
gdip_pointf_get_y(VALUE self)
{
    PointF *point = Data_Ptr<PointF *>(self);
    return SINGLE2NUM(point->Y);
}

static VALUE
gdip_pointf_set_y(VALUE self, VALUE num)
{
    PointF *point = Data_Ptr<PointF *>(self);
    if (Float_p(num)) {
        point->Y = NUM2SINGLE(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Float.");
    }
    return self;
}

static VALUE
gdip_pointf_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    else if (!_KIND_OF(other, &tPointF)) return Qfalse;
    else {
        PointF *po1 = Data_Ptr<PointF *>(self);
        PointF *po2 = Data_Ptr<PointF *>(other);
        return (fabsf(po1->X - po2->X) < FLT_EPSILON && fabsf(po1->Y - po2->Y) < FLT_EPSILON) ? Qtrue : Qfalse;
    }
}

static VALUE
gdip_pointf_add(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tPointF)) {
        VALUE r = typeddata_alloc<PointF, &tPointF>(cPointF);
        PointF *po_r = Data_Ptr<PointF *>(r);
        PointF *po_self = Data_Ptr<PointF *>(self);
        PointF *po_other = Data_Ptr<PointF *>(other);
        po_r->X = po_self->X + po_other->X;
        po_r->Y = po_self->Y + po_other->Y;
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Point or Size.");
        return Qnil;
    }
}

void
Init_rectangle()
{
    cPoint = rb_define_class_under(mGdiplus, "Point", rb_cObject);
    rb_define_alloc_func(cPoint, &typeddata_alloc<Point, &tPoint>);

    cPointF = rb_define_class_under(mGdiplus, "PointF", rb_cObject);
    rb_define_alloc_func(cPointF, &typeddata_alloc<PointF, &tPointF>);

    cSize = rb_define_class_under(mGdiplus, "Size", rb_cObject);
    rb_define_alloc_func(cSize, &typeddata_alloc<Size, &tSize>);

    cSizeF = rb_define_class_under(mGdiplus, "SizeF", rb_cObject);
    rb_define_alloc_func(cSizeF, &typeddata_alloc<SizeF, &tSizeF>);

    cRectangle = rb_define_class_under(mGdiplus, "Rectangle", rb_cObject);
    rb_define_alloc_func(cRectangle, &typeddata_alloc<Rect, &tRectangle>);

    cRectangleF = rb_define_class_under(mGdiplus, "RectangleF", rb_cObject);
    rb_define_alloc_func(cRectangleF, &typeddata_alloc<RectF, &tRectangleF>);

    rb_define_method(cPoint, "initialize", RUBY_METHOD_FUNC(gdip_point_init), -1);
    rb_define_method(cPoint, "inspect", RUBY_METHOD_FUNC(gdip_point_inspect), 0);
    rb_define_method(cPoint, "x", RUBY_METHOD_FUNC(gdip_point_get_x), 0);
    rb_define_method(cPoint, "X", RUBY_METHOD_FUNC(gdip_point_get_x), 0);
    rb_define_method(cPoint, "x=", RUBY_METHOD_FUNC(gdip_point_set_x), 1);
    rb_define_method(cPoint, "X=", RUBY_METHOD_FUNC(gdip_point_set_x), 1);
    rb_define_method(cPoint, "y", RUBY_METHOD_FUNC(gdip_point_get_y), 0);
    rb_define_method(cPoint, "Y", RUBY_METHOD_FUNC(gdip_point_get_y), 0);
    rb_define_method(cPoint, "y=", RUBY_METHOD_FUNC(gdip_point_set_y), 1);
    rb_define_method(cPoint, "Y=", RUBY_METHOD_FUNC(gdip_point_set_y), 1);
    rb_define_method(cPoint, "==", RUBY_METHOD_FUNC(gdip_point_equal), 1);
    rb_define_method(cPoint, "+", RUBY_METHOD_FUNC(gdip_point_add), 1);

    rb_define_method(cPointF, "initialize", RUBY_METHOD_FUNC(gdip_pointf_init), -1);
    rb_define_method(cPointF, "inspect", RUBY_METHOD_FUNC(gdip_pointf_inspect), 0);
    rb_define_method(cPointF, "x", RUBY_METHOD_FUNC(gdip_pointf_get_x), 0);
    rb_define_method(cPointF, "X", RUBY_METHOD_FUNC(gdip_pointf_get_x), 0);
    rb_define_method(cPointF, "x=", RUBY_METHOD_FUNC(gdip_pointf_set_x), 1);
    rb_define_method(cPointF, "X=", RUBY_METHOD_FUNC(gdip_pointf_set_x), 1);
    rb_define_method(cPointF, "y", RUBY_METHOD_FUNC(gdip_pointf_get_y), 0);
    rb_define_method(cPointF, "Y", RUBY_METHOD_FUNC(gdip_pointf_get_y), 0);
    rb_define_method(cPointF, "y=", RUBY_METHOD_FUNC(gdip_pointf_set_y), 1);
    rb_define_method(cPointF, "Y=", RUBY_METHOD_FUNC(gdip_pointf_set_y), 1);
    rb_define_method(cPointF, "==", RUBY_METHOD_FUNC(gdip_pointf_equal), 1);
    rb_define_method(cPointF, "+", RUBY_METHOD_FUNC(gdip_pointf_add), 1);
}