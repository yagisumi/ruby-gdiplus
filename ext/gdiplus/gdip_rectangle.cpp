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


VALUE
gdip_point_create(int x, int y)
{
    VALUE r = typeddata_alloc<Point, &tPoint>(cPoint);
    Point *point = Data_Ptr<Point *>(r);
    point->X = x;
    point->Y = y;
    return r;
}

/**
 *
 * @overload initialize()
 * @overload initialize(x, y)
 *   @param x [Integer]
 *   @param y [Integer]
 */
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

/**
 * @return [String]
 */
static VALUE
gdip_point_inspect(VALUE self)
{
    Point *point = Data_Ptr<Point *>(self);
    VALUE r = util_utf8_sprintf("#<%s X=%d, Y=%d>", __class__(self), point->X, point->Y);
    return r;
}

/**
 * Gets or sets the x-coordinate of this point.
 * @return [Integer]
 */
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

/**
 * Gets or sets the y-coordinate of this point.
 * @return [Integer]
 */
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


/**
 * Compares two points.
 * @param other [Point]
 * @return [Boolean]
 */
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


/**
 * Adds another point to this point.
 * @param other [Point]
 * @return [Point]
 */
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

/**
 * Subtracts another point from this point.
 * @param other [Point]
 * @return [Point]
 */
static VALUE
gdip_point_sub(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tPoint)) {
        VALUE r = typeddata_alloc<Point, &tPoint>(cPoint);
        Point *po_r = Data_Ptr<Point *>(r);
        Point *po_self = Data_Ptr<Point *>(self);
        Point *po_other = Data_Ptr<Point *>(other);
        po_r->X = po_self->X - po_other->X;
        po_r->Y = po_self->Y - po_other->Y;
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Point or Size.");
        return Qnil;
    }
}

VALUE
gdip_pointf_create(float x, float y)
{
    VALUE r = typeddata_alloc<PointF, &tPointF>(cPointF);
    PointF *point = Data_Ptr<PointF *>(r);
    point->X = x;
    point->Y = y;
    return r;
}

/**
 *
 * @overload initialize()
 * @overload initialize(x, y)
 *   @param x [Float]
 *   @param y [Float]
 */
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

/**
 * @return [String]
 */
static VALUE
gdip_pointf_inspect(VALUE self)
{
    PointF *point = Data_Ptr<PointF *>(self);
    VALUE r = util_utf8_sprintf("#<%s X=%f, Y=%f>", __class__(self), point->X, point->Y);
    return r;
}

/**
 * Gets or sets the x-coordinate of this point.
 * @return [Float]
 */
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

/**
 * Gets or sets the y-coordinate of this point.
 * @return [Float]
 */
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

/**
 * Compares two points.
 * @param other [PointF]
 * @return [Boolean]
 */
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

/**
 * Adds another point to this point.
 * @param other [PointF]
 * @return [PointF]
 */
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

/**
 * Subtracts another point from this point.
 * @param other [PointF]
 * @return [PointF]
 */
static VALUE
gdip_pointf_sub(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tPointF)) {
        VALUE r = typeddata_alloc<PointF, &tPointF>(cPointF);
        PointF *po_r = Data_Ptr<PointF *>(r);
        PointF *po_self = Data_Ptr<PointF *>(self);
        PointF *po_other = Data_Ptr<PointF *>(other);
        po_r->X = po_self->X - po_other->X;
        po_r->Y = po_self->Y - po_other->Y;
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Point or Size.");
        return Qnil;
    }
}

VALUE
gdip_size_create(int width, int height)
{
    VALUE r = typeddata_alloc<Size, &tSize>(cSize);
    Size *size = Data_Ptr<Size *>(r);
    size->Width = width;
    size->Height = height;
    return r;
}

/**
 *
 * @overload initialize()
 * @overload initialize(width, height)
 *   @param width [Integer]
 *   @param height [Integer]
 */
static VALUE
gdip_size_init(int argc, VALUE *argv, VALUE self)
{
    if (argc != 0 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0 or 2)", argc);
    }

    Size *size = Data_Ptr<Size *>(self);
    if (argc == 0) return self;
    else if (Integer_p(argv[0], argv[1])) {
        size->Width = RB_NUM2INT(argv[0]);
        size->Height = RB_NUM2INT(argv[1]);
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Integer.");
    }
    return self;
}

/**
 *
 * @return [String]
 * 
 */
static VALUE
gdip_size_inspect(VALUE self)
{
    Size *size = Data_Ptr<Size *>(self);
    VALUE r = util_utf8_sprintf("#<%s Width=%d, Height=%d>", __class__(self), size->Width, size->Height);
    return r;
}

/**
 * Gets or sets the width of this size.
 * @return [Integer]
 */
static VALUE
gdip_size_get_width(VALUE self)
{
    Size *size = Data_Ptr<Size *>(self);
    return RB_INT2NUM(size->Width);
}

static VALUE
gdip_size_set_width(VALUE self, VALUE num)
{
    Size *size = Data_Ptr<Size *>(self);
    if (Integer_p(num)) {
        size->Width = RB_NUM2INT(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Integer.");
    }
    return self;
}

/**
 * Gets or sets the height of this size.
 * @return [Integer]
 */
static VALUE
gdip_size_get_height(VALUE self)
{
    Size *size = Data_Ptr<Size *>(self);
    return RB_INT2NUM(size->Height);
}

static VALUE
gdip_size_set_height(VALUE self, VALUE num)
{
    Size *size = Data_Ptr<Size *>(self);
    if (Integer_p(num)) {
        size->Height = RB_NUM2INT(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Integer.");
    }
    return self;
}


/**
 * Compare two sizes.
 * @param other [Size]
 * @return [Boolean]
 */
static VALUE
gdip_size_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    else if (!_KIND_OF(other, &tSize)) return Qfalse;
    else {
        Size *size1 = Data_Ptr<Size *>(self);
        Size *size2 = Data_Ptr<Size *>(other);
        return (size1->Width == size2->Width && size1->Height == size2->Height) ? Qtrue : Qfalse;
    }
}

/**
 * Adds another size to this size.
 * @param other [Size]
 * @return [Size]
 */
static VALUE
gdip_size_add(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tSize)) {
        VALUE r = typeddata_alloc<Size, &tSize>(cSize);
        Size *size_r = Data_Ptr<Size *>(r);
        Size *size_self = Data_Ptr<Size *>(self);
        Size *size_other = Data_Ptr<Size *>(other);
        size_r->Width = size_self->Width + size_other->Width;
        size_r->Height = size_self->Height + size_other->Height;
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Size.");
        return Qnil;
    }
}

/**
 * Subtracts another size from this size.
 * @param other [Size]
 * @return [Size]
 */
static VALUE
gdip_size_sub(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tSize)) {
        VALUE r = typeddata_alloc<Size, &tSize>(cSize);
        Size *size_r = Data_Ptr<Size *>(r);
        Size *size_self = Data_Ptr<Size *>(self);
        Size *size_other = Data_Ptr<Size *>(other);
        size_r->Width = size_self->Width - size_other->Width;
        size_r->Height = size_self->Height - size_other->Height;
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Size.");
        return Qnil;
    }
}


VALUE
gdip_sizef_create(float width, float height)
{
    VALUE r = typeddata_alloc<SizeF, &tSizeF>(cSizeF);
    SizeF *size = Data_Ptr<SizeF *>(r);
    size->Width = width;
    size->Height = height;
    return r;
}

/**
 *
 * @overload initialize()
 * @overload initialize(width, height)
 *   @param width [Float]
 *   @param height [Float]
 */
static VALUE
gdip_sizef_init(int argc, VALUE *argv, VALUE self)
{
    if (argc != 0 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0 or 2)", argc);
    }

    SizeF *size = Data_Ptr<SizeF *>(self);
    if (argc == 0) {
        size->Width = 0.0f;
        size->Height = 0.0f;
    }
    else if (Float_p(argv[0], argv[1])) {
        size->Width = NUM2SINGLE(argv[0]);
        size->Height = NUM2SINGLE(argv[1]);
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Float.");
    }
    return self;
}


/**
 *
 * @return [String]
 * 
 */
static VALUE
gdip_sizef_inspect(VALUE self)
{
    SizeF *size = Data_Ptr<SizeF *>(self);
    VALUE r = util_utf8_sprintf("#<%s Width=%f, Height=%f>", __class__(self), size->Width, size->Height);
    return r;
}

/**
 * Gets or sets the width of this size.
 * @return [Float]
 */
static VALUE
gdip_sizef_get_width(VALUE self)
{
    SizeF *size = Data_Ptr<SizeF *>(self);
    return SINGLE2NUM(size->Width);
}

static VALUE
gdip_sizef_set_width(VALUE self, VALUE num)
{
    SizeF *size = Data_Ptr<SizeF *>(self);
    if (Float_p(num)) {
        size->Width = NUM2SINGLE(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Float.");
    }
    return self;
}

/**
 * Gets or sets the height of this size.
 * @return [Float]
 */
static VALUE
gdip_sizef_get_height(VALUE self)
{
    SizeF *size = Data_Ptr<SizeF *>(self);
    return RB_INT2NUM(size->Height);
}

static VALUE
gdip_sizef_set_height(VALUE self, VALUE num)
{
    SizeF *size = Data_Ptr<SizeF *>(self);
    if (Float_p(num)) {
        size->Height = NUM2SINGLE(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Float.");
    }
    return self;
}

/**
 * Compare two sizes.
 * @param other [SizeF]
 * @return [Boolean]
 */
static VALUE
gdip_sizef_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    else if (!_KIND_OF(other, &tSizeF)) return Qfalse;
    else {
        SizeF *size1 = Data_Ptr<SizeF *>(self);
        SizeF *size2 = Data_Ptr<SizeF *>(other);
        return (fabsf(size1->Width - size2->Width) < FLT_EPSILON && fabsf(size1->Height - size2->Height) < FLT_EPSILON) ? Qtrue : Qfalse;
    }
}

/**
 * Adds another size to this size.
 * @param other [SizeF]
 * @return [SizeF]
 */
static VALUE
gdip_sizef_add(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tSizeF)) {
        VALUE r = typeddata_alloc<SizeF, &tSizeF>(cSizeF);
        SizeF *size_r = Data_Ptr<SizeF *>(r);
        SizeF *size_self = Data_Ptr<SizeF *>(self);
        SizeF *size_other = Data_Ptr<SizeF *>(other);
        size_r->Width = size_self->Width + size_other->Width;
        size_r->Height = size_self->Height + size_other->Height;
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Size.");
        return Qnil;
    }
}

/**
 * Subtracts another size from this size.
 * @param other [SizeF]
 * @return [SizeF]
 */
static VALUE
gdip_sizef_sub(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tSizeF)) {
        VALUE r = typeddata_alloc<SizeF, &tSizeF>(cSizeF);
        SizeF *size_r = Data_Ptr<SizeF *>(r);
        SizeF *size_self = Data_Ptr<SizeF *>(self);
        SizeF *size_other = Data_Ptr<SizeF *>(other);
        size_r->Width = size_self->Width - size_other->Width;
        size_r->Height = size_self->Height - size_other->Height;
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Size.");
        return Qnil;
    }
}

VALUE
gdip_rect_create(int x, int y, int width, int height)
{
    VALUE r = typeddata_alloc<Rect, &tRectangle>(cRectangle);
    Rect *rect = Data_Ptr<Rect *>(r);
    rect->X = x;
    rect->Y = y;
    rect->Width = width;
    rect->Height = height;
    return r;
}

VALUE
gdip_rect_create(Rect *rect)
{
    VALUE r = typeddata_alloc<Rect, &tRectangle>(cRectangle);
    Rect *rect_r = Data_Ptr<Rect *>(r);
    if (rect) {
        rect_r->X = rect->X;
        rect_r->Y = rect->Y;
        rect_r->Width = rect->Width;
        rect_r->Height = rect->Height;
    }
    return r;
}

/**
 *
 * @overload initialize()
 * @overload initialize(x, y, width, height)
 *   @param x [Integer]
 *   @param y [Integer]
 *   @param width [Integer]
 *   @param height [Integer]
 */
static VALUE
gdip_rect_init(int argc, VALUE *argv, VALUE self)
{
    if (argc != 0 && argc != 4) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0 or 4)", argc);
    }
    if (argc == 0) return self;
    else if (Integer_p(argv[0], argv[1], argv[2], argv[3])) {
        Rect *rect = Data_Ptr<Rect *>(self);
        rect->X = RB_NUM2INT(argv[0]);
        rect->Y = RB_NUM2INT(argv[1]);
        rect->Width = RB_NUM2INT(argv[2]);
        rect->Height = RB_NUM2INT(argv[3]);
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Integer.");
    }
    return self;
}

/**
 * Creates a new rectangle.
 * FromLTRB(left, top, right, bottom)
 * @param left [Integer]
 * @param top [Integer]
 * @param right [Integer]
 * @param bottom [Integer]
 * @return [Gdiplus::Rectangle]
 */
static VALUE
gdip_rect_s_from_ltrb(VALUE self, VALUE left, VALUE top, VALUE right, VALUE bottom)
{
    if (!Integer_p(left, top, right, bottom)) {
        rb_raise(rb_eTypeError, "The arguments should be Integer.");
    }
    return gdip_rect_create(RB_NUM2INT(left), RB_NUM2INT(top), RB_NUM2INT(right) - RB_NUM2INT(left), RB_NUM2INT(bottom) - RB_NUM2INT(top));
}

/**
 * @return [String]
 */
static VALUE
gdip_rect_inspect(VALUE self)
{
    Rect *rect = Data_Ptr<Rect *>(self);
    VALUE r = util_utf8_sprintf("#<%s X=%d, Y=%d, Width=%d, Height=%d>", __class__(self), rect->X, rect->Y, rect->Width, rect->Height);
    return r;
}

/**
 * Gets or sets the x coordinate of the upper-left corner of this rectangle.
 * @return [Integer]
 */
static VALUE
gdip_rect_get_x(VALUE self)
{
    Rect *rect = Data_Ptr<Rect *>(self);
    return RB_INT2NUM(rect->X);
}

static VALUE
gdip_rect_set_x(VALUE self, VALUE num)
{
    if (Integer_p(num)) {
        Rect *rect = Data_Ptr<Rect *>(self);
        rect->X = RB_NUM2INT(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Integer.");
    }
    return self;
}

/**
 * Gets or sets the y coordinate of the upper-left corner of this rectangle.
 * @return [Integer]
 */
static VALUE
gdip_rect_get_y(VALUE self)
{
    Rect *rect = Data_Ptr<Rect *>(self);
    return RB_INT2NUM(rect->Y);
}

static VALUE
gdip_rect_set_y(VALUE self, VALUE num)
{
    if (Integer_p(num)) {
        Rect *rect = Data_Ptr<Rect *>(self);
        rect->Y = RB_NUM2INT(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Integer.");
    }
    return self;
}

/**
 * Gets or sets the width of this rectangle.
 * @return [Integer]
 */
static VALUE
gdip_rect_get_width(VALUE self)
{
    Rect *rect = Data_Ptr<Rect *>(self);
    return RB_INT2NUM(rect->Width);
}

static VALUE
gdip_rect_set_width(VALUE self, VALUE num)
{
    if (Integer_p(num)) {
        Rect *rect = Data_Ptr<Rect *>(self);
        rect->Width = RB_NUM2INT(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Integer.");
    }
    return self;
}

/**
 * Gets or sets the height of this rectangle.
 * @return [Integer]
 */
static VALUE
gdip_rect_get_height(VALUE self)
{
    Rect *rect = Data_Ptr<Rect *>(self);
    return RB_INT2NUM(rect->Height);
}


static VALUE
gdip_rect_set_height(VALUE self, VALUE num)
{
    if (Integer_p(num)) {
        Rect *rect = Data_Ptr<Rect *>(self);
        rect->Height = RB_NUM2INT(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Integer.");
    }
    return self;
}

/**
 * Gets the x coordinate of the right edge of this rectangle.
 * @return [Integer]
 */
static VALUE
gdip_rect_get_right(VALUE self)
{
    Rect *rect = Data_Ptr<Rect *>(self);
    return RB_INT2NUM(rect->X + rect->Width);
}

/**
 * Gets the y coordinate of the bottom edge of this rectangle.
 * @return [Integer]
 */
static VALUE
gdip_rect_get_bottom(VALUE self)
{
    Rect *rect = Data_Ptr<Rect *>(self);
    return RB_INT2NUM(rect->Y + rect->Height);
}

/**
 * Gets or sets the point of the upper left corner of this rectangle.
 * @return [Gdiplus::Point]
 */
static VALUE
gdip_rect_get_location(VALUE self)
{
    Rect *rect = Data_Ptr<Rect *>(self);
    return gdip_point_create(rect->X, rect->Y);
}

static VALUE
gdip_rect_set_location(VALUE self, VALUE loc)
{
    if (_KIND_OF(loc, &tPoint)) {
        Rect *rect = Data_Ptr<Rect *>(self);
        Point *point = Data_Ptr<Point *>(loc);
        rect->X = point->X;
        rect->Y = point->Y;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Point.");
    }
    return self;
}

/**
 * Gets or sets the size of this rectangle.
 * @return [Gdiplus::Size]
 */
static VALUE
gdip_rect_get_size(VALUE self)
{
    Rect *rect = Data_Ptr<Rect *>(self);
    return gdip_size_create(rect->Width, rect->Height);
}

static VALUE
gdip_rect_set_size(VALUE self, VALUE size)
{
    if (_KIND_OF(size, &tSize)) {
        Rect *rect = Data_Ptr<Rect *>(self);
        Size *sz = Data_Ptr<Size *>(size);
        rect->Width = sz->Width;
        rect->Height = sz->Height;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Size.");
    }
    return self;
}

/**
 * Compares this and other rectangles.
 * @return [Boolean]
 */
static VALUE
gdip_rect_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    else if (!_KIND_OF(other, &tRectangle)) return Qfalse;
    else {
        Rect *rect_self = Data_Ptr<Rect *>(self);
        Rect *rect_other = Data_Ptr<Rect *>(other);
        return (rect_self->X == rect_other->X && rect_self->Y == rect_other->Y &&
            rect_self->Width == rect_other->Width && rect_self->Height == rect_other->Height) ? Qtrue : Qfalse;
    }
}

/**
 * Whether a point or rectangle is within this rectangle.
 * @return [Boolean]
 * @overload Contains(obj)
 *   @param obj [Gdiplus::Point, Gdiplus::Rectangle]
 * @overload Contains(x, y)
 *   @param x [Integer]
 *   @param y [Integer]
 */
static VALUE
gdip_rect_contains(int argc, VALUE *argv, VALUE self)
{
    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }
    Rect *rect = Data_Ptr<Rect *>(self);
    if (argc == 1) {
        if (_KIND_OF(argv[0], &tPoint)) {
            Point *po = Data_Ptr<Point *>(argv[0]);
            return rect->Contains(*po) ? Qtrue : Qfalse;
        }
        else if (_KIND_OF(argv[0], &tRectangle)) {
            Rect *rect_other = Data_Ptr<Rect *>(argv[0]);
            return rect->Contains(*rect_other) ? Qtrue : Qfalse;
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Point or Rectangle.");
        }
    }
    if (argc == 2) {
        if (Integer_p(argv[0], argv[1])) {
            return rect->Contains(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1])) ? Qtrue : Qfalse;
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer.");
        }
    }
    return Qfalse;
}

/**
 * Enlarges this rectangle.
 * @return [self]
 * @see #inflate
 * @overload Inflate(size)
 *   @param size [Gdiplus::Size]
 * @overload Inflate(width, height)
 *   @param width [Integer]
 *   @param height [Integer]
 */
static VALUE
gdip_rect_inflate_bang(int argc, VALUE *argv, VALUE self)
{
    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }
    Rect *rect = Data_Ptr<Rect *>(self);
    if (argc == 1) {
        if (_KIND_OF(argv[0], &tPoint)) {
            Point *point = Data_Ptr<Point *>(argv[0]);
            rect->Inflate(point->X, point->Y);
        }
        else if (_KIND_OF(argv[0], &tSize)) {
            Size *size = Data_Ptr<Size *>(argv[0]);
            rect->Inflate(size->Width, size->Height);
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Point or Size.");
        }
    }
    else if (argc == 2) {
        if (Integer_p(argv[0], argv[1])) {
            rect->Inflate(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer.");
        }
    }
    return self;
}


/**
 * Returns a rectangle enlarged from this rectangle.
 * @return [Gdiplus::Rectangle]
 * @overload inflate(size)
 *   @param size [Gdiplus::Size]
 * @overload inflate(width, height)
 *   @param width [Integer]
 *   @param height [Integer]
 */
static VALUE
gdip_rect_inflate(int argc, VALUE *argv, VALUE self)
{
    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }
    Rect *rect = Data_Ptr<Rect *>(self);
    if (argc == 1) {
        if (_KIND_OF(argv[0], &tPoint)) {
            VALUE r = gdip_rect_create(rect);
            Rect *rect_r = Data_Ptr<Rect *>(r);
            Point *point = Data_Ptr<Point *>(argv[0]);
            rect_r->Inflate(point->X, point->Y);
            return r;
        }
        else if (_KIND_OF(argv[0], &tSize)) {
            VALUE r = gdip_rect_create(rect);
            Rect *rect_r = Data_Ptr<Rect *>(r);
            Size *size = Data_Ptr<Size *>(argv[0]);
            rect_r->Inflate(size->Width, size->Height);
            return r;
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Point or Size.");
        }
    }
    else if (argc == 2) {
        if (Integer_p(argv[0], argv[1])) {
            VALUE r = gdip_rect_create(rect);
            Rect *rect_r = Data_Ptr<Rect *>(r);
            rect_r->Inflate(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]));
            return r;
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer.");
        }
    }
    return Qnil;
}

/**
 * Whether another rectangle intersects this rectangle.
 * @param other [Gdiplus::Rectangle]
 * @return [Boolean]
 */
static VALUE
gdip_rect_intersects_with(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tRectangle)) {
        Rect *rect_self = Data_Ptr<Rect *>(self);
        Rect *rect_other = Data_Ptr<Rect *>(other);
        return rect_self->IntersectsWith(*rect_other) ? Qtrue : Qfalse;
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Rectangle.");
        return Qfalse;
    }
}

/**
 * Replaces this rectangle with the intersection of this and another rectangles.
 * @param other [Gdiplus::Rectangle]
 * @return [self]
 */
static VALUE
gdip_rect_intersect_bang(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tRectangle)) {
        Rect *rect_self = Data_Ptr<Rect *>(self);
        Rect *rect_other = Data_Ptr<Rect *>(other);
        rect_self->Intersect(*rect_other);
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Rectangle.");
    }
    return self;
}

/**
 * Returns a rectangle that is the intersection of this and another rectangles.
 * @param other [Gdiplus::Rectangle]
 * @return [Gdiplus::Rectangle]
 */
static VALUE
gdip_rect_intersect(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tRectangle)) {
        Rect *rect_self = Data_Ptr<Rect *>(self);
        VALUE r = gdip_rect_create(rect_self);
        Rect *rect_r = Data_Ptr<Rect *>(r);
        Rect *rect_other = Data_Ptr<Rect *>(other);
        rect_r->Intersect(*rect_other);
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Rectangle.");
        return Qnil;
    }
}

/**
 * Returns a rectangle that contains the union of this and another rectangles.
 * @param other [Gdiplus::Rectangle]
 * @return [Gdiplus::Rectangle]
 */
static VALUE
gdip_rect_union(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tRectangle)) {
        Rect *rect_self = Data_Ptr<Rect *>(self);
        VALUE r = gdip_rect_create();
        Rect *rect_r = Data_Ptr<Rect *>(r);
        Rect *rect_other = Data_Ptr<Rect *>(other);
        Rect::Union(*rect_r, *rect_self, *rect_other);
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Rectangle.");
        return Qnil;
    }
}


VALUE
gdip_rectf_create(float x, float y, float width, float height)
{
    VALUE r = typeddata_alloc<RectF, &tRectangleF>(cRectangleF);
    RectF *rect = Data_Ptr<RectF *>(r);
    rect->X = x;
    rect->Y = y;
    rect->Width = width;
    rect->Height = height;
    return r;
}

VALUE
gdip_rectf_create(RectF *rect)
{
    VALUE r = typeddata_alloc<RectF, &tRectangleF>(cRectangleF);
    RectF *rect_r = Data_Ptr<RectF *>(r);
    if (rect) {
        rect_r->X = rect->X;
        rect_r->Y = rect->Y;
        rect_r->Width = rect->Width;
        rect_r->Height = rect->Height;
    }
    return r;
}

/**
 *
 * @overload initialize()
 * @overload initialize(x, y, width, height)
 *   @param x [Float]
 *   @param y [Float]
 *   @param width [Float]
 *   @param height [Float]
 */
static VALUE
gdip_rectf_init(int argc, VALUE *argv, VALUE self)
{
    if (argc != 0 && argc != 4) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0 or 4)", argc);
    }
    if (argc == 0) return self;
    else if (Float_p(argv[0], argv[1], argv[2], argv[3])) {
        RectF *rect = Data_Ptr<RectF *>(self);
        rect->X = NUM2SINGLE(argv[0]);
        rect->Y = NUM2SINGLE(argv[1]);
        rect->Width = NUM2SINGLE(argv[2]);
        rect->Height = NUM2SINGLE(argv[3]);
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Float.");
    }
    return self;
}

/**
 * Creates a new rectangle.
 * FromLTRB(left, top, right, bottom)
 * @param left [Float]
 * @param top [Float]
 * @param right [Float]
 * @param bottom [Float]
 * @return [Gdiplus::RectangleF]
 */
static VALUE
gdip_rectf_s_from_ltrb(VALUE self, VALUE left, VALUE top, VALUE right, VALUE bottom)
{
    if (!Float_p(left, top, right, bottom)) {
        rb_raise(rb_eTypeError, "The arguments should be Float.");
    }
    return gdip_rectf_create(NUM2SINGLE(left), NUM2SINGLE(top), NUM2SINGLE(right) - NUM2SINGLE(left), NUM2SINGLE(bottom) - NUM2SINGLE(top));
}

/**
 * @return [String]
 */
static VALUE
gdip_rectf_inspect(VALUE self)
{
    RectF *rect = Data_Ptr<RectF *>(self);
    VALUE r = util_utf8_sprintf("#<%s X=%f, Y=%f, Width=%f, Height=%f>", __class__(self), rect->X, rect->Y, rect->Width, rect->Height);
    return r;
}

/**
 * Gets or sets the x coordinate of the upper-left corner of this rectangle.
 * @return [Float]
 */
static VALUE
gdip_rectf_get_x(VALUE self)
{
    RectF *rect = Data_Ptr<RectF *>(self);
    return SINGLE2NUM(rect->X);
}

static VALUE
gdip_rectf_set_x(VALUE self, VALUE num)
{
    if (Float_p(num)) {
        RectF *rect = Data_Ptr<RectF *>(self);
        rect->X = NUM2SINGLE(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Float.");
    }
    return self;
}

/**
 * Gets or sets the y coordinate of the upper-left corner of this rectangle.
 * @return [Float]
 */
static VALUE
gdip_rectf_get_y(VALUE self)
{
    RectF *rect = Data_Ptr<RectF *>(self);
    return SINGLE2NUM(rect->Y);
}

static VALUE
gdip_rectf_set_y(VALUE self, VALUE num)
{
    if (Float_p(num)) {
        RectF *rect = Data_Ptr<RectF *>(self);
        rect->Y = NUM2SINGLE(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Float.");
    }
    return self;
}

/**
 * Gets or sets the width of this rectangle.
 * @return [Float]
 */
static VALUE
gdip_rectf_get_width(VALUE self)
{
    RectF *rect = Data_Ptr<RectF *>(self);
    return SINGLE2NUM(rect->Width);
}

static VALUE
gdip_rectf_set_width(VALUE self, VALUE num)
{
    if (Float_p(num)) {
        RectF *rect = Data_Ptr<RectF *>(self);
        rect->Width = NUM2SINGLE(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Float.");
    }
    return self;
}

/**
 * Gets or sets the height of this rectangle.
 * @return [Integer]
 */
static VALUE
gdip_rectf_get_height(VALUE self)
{
    RectF *rect = Data_Ptr<RectF *>(self);
    return SINGLE2NUM(rect->Height);
}


static VALUE
gdip_rectf_set_height(VALUE self, VALUE num)
{
    if (Float_p(num)) {
        RectF *rect = Data_Ptr<RectF *>(self);
        rect->Height = NUM2SINGLE(num);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Float.");
    }
    return self;
}

/**
 * Gets the x coordinate of the right edge of this rectangle.
 * @return [Float]
 */
static VALUE
gdip_rectf_get_right(VALUE self)
{
    RectF *rect = Data_Ptr<RectF *>(self);
    return SINGLE2NUM(rect->X + rect->Width);
}

/**
 * Gets the y coordinate of the bottom edge of this rectangle.
 * @return [Float]
 */
static VALUE
gdip_rectf_get_bottom(VALUE self)
{
    RectF *rect = Data_Ptr<RectF *>(self);
    return SINGLE2NUM(rect->Y + rect->Height);
}

/**
 * Gets or sets the point of the upper left corner of this rectangle.
 * @return [Gdiplus::PointF]
 */
static VALUE
gdip_rectf_get_location(VALUE self)
{
    RectF *rect = Data_Ptr<RectF *>(self);
    return gdip_pointf_create(rect->X, rect->Y);
}

static VALUE
gdip_rectf_set_location(VALUE self, VALUE loc)
{
    if (_KIND_OF(loc, &tPointF)) {
        RectF *rect = Data_Ptr<RectF *>(self);
        Point *point = Data_Ptr<Point *>(loc);
        rect->X = point->X;
        rect->Y = point->Y;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be PointF.");
    }
    return self;
}

/**
 * Gets or sets the size of this rectangle.
 * @return [Gdiplus::SizeF]
 */
static VALUE
gdip_rectf_get_size(VALUE self)
{
    RectF *rect = Data_Ptr<RectF *>(self);
    return gdip_sizef_create(rect->Width, rect->Height);
}

static VALUE
gdip_rectf_set_size(VALUE self, VALUE size)
{
    if (_KIND_OF(size, &tSizeF)) {
        RectF *rect = Data_Ptr<RectF *>(self);
        SizeF *sz = Data_Ptr<SizeF *>(size);
        rect->Width = sz->Width;
        rect->Height = sz->Height;
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be SizeF.");
    }
    return self;
}

/**
 * Compares this and other rectangles.
 * @return [Boolean]
 */
static VALUE
gdip_rectf_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    else if (!_KIND_OF(other, &tRectangleF)) return Qfalse;
    else {
        RectF *rect_self = Data_Ptr<RectF *>(self);
        RectF *rect_other = Data_Ptr<RectF *>(other);
        return (rect_self->X == rect_other->X && rect_self->Y == rect_other->Y &&
            rect_self->Width == rect_other->Width && rect_self->Height == rect_other->Height) ? Qtrue : Qfalse;
    }
}

/**
 * Whether a point or rectangle is within this rectangle.
 * @return [Boolean]
 * @overload Contains(obj)
 *   @param obj [Gdiplus::PointF, Gdiplus::RectangleF]
 * @overload Contains(x, y)
 *   @param x [Float]
 *   @param y [Float]
 */
static VALUE
gdip_rectf_contains(int argc, VALUE *argv, VALUE self)
{
    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }
    RectF *rect = Data_Ptr<RectF *>(self);
    if (argc == 1) {
        if (_KIND_OF(argv[0], &tPointF)) {
            PointF *po = Data_Ptr<PointF *>(argv[0]);
            return rect->Contains(*po) ? Qtrue : Qfalse;
        }
        else if (_KIND_OF(argv[0], &tRectangleF)) {
            RectF *rect_other = Data_Ptr<RectF *>(argv[0]);
            return rect->Contains(*rect_other) ? Qtrue : Qfalse;
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be PointF or RectFangleF.");
        }
    }
    if (argc == 2) {
        if (Float_p(argv[0], argv[1])) {
            return rect->Contains(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1])) ? Qtrue : Qfalse;
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Float.");
        }
    }
    return Qfalse;
}

/**
 * Enlarges this rectangle.
 * @return [self]
 * @see #inflate
 * @overload Inflate(size)
 *   @param size [Gdiplus::SizeF]
 * @overload Inflate(width, height)
 *   @param width [Float]
 *   @param height [Float]
 */
static VALUE
gdip_rectf_inflate_bang(int argc, VALUE *argv, VALUE self)
{
    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }
    RectF *rect = Data_Ptr<RectF *>(self);
    if (argc == 1) {
        if (_KIND_OF(argv[0], &tPointF)) {
            PointF *point = Data_Ptr<PointF *>(argv[0]);
            rect->Inflate(point->X, point->Y);
        }
        else if (_KIND_OF(argv[0], &tSizeF)) {
            SizeF *size = Data_Ptr<SizeF *>(argv[0]);
            rect->Inflate(size->Width, size->Height);
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be PointF or SizeF.");
        }
    }
    else if (argc == 2) {
        if (Float_p(argv[0], argv[1])) {
            rect->Inflate(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Float.");
        }
    }
    return self;
}

/**
 * Returns a rectangle enlarged from this rectangle.
 * @return [Gdiplus::RectangleF]
 * @overload inflate(size)
 *   @param size [Gdiplus::SizeF]
 * @overload inflate(width, height)
 *   @param width [Float]
 *   @param height [Float]
 */
static VALUE
gdip_rectf_inflate(int argc, VALUE *argv, VALUE self)
{
    if (argc != 1 && argc != 2) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    }
    RectF *rect = Data_Ptr<RectF *>(self);
    if (argc == 1) {
        if (_KIND_OF(argv[0], &tPointF)) {
            VALUE r = gdip_rectf_create(rect);
            RectF *rect_r = Data_Ptr<RectF *>(r);
            PointF *point = Data_Ptr<PointF *>(argv[0]);
            rect_r->Inflate(point->X, point->Y);
            return r;
        }
        else if (_KIND_OF(argv[0], &tSizeF)) {
            VALUE r = gdip_rectf_create(rect);
            RectF *rect_r = Data_Ptr<RectF *>(r);
            SizeF *size = Data_Ptr<SizeF *>(argv[0]);
            rect_r->Inflate(size->Width, size->Height);
            return r;
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be PointF or SizeF.");
        }
    }
    else if (argc == 2) {
        if (Float_p(argv[0], argv[1])) {
            VALUE r = gdip_rectf_create(rect);
            RectF *rect_r = Data_Ptr<RectF *>(r);
            rect_r->Inflate(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]));
            return r;
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Float.");
        }
    }
    return Qnil;
}

/**
 * Whether another rectangle intersects this rectangle.
 * @param other [Gdiplus::RectangleF]
 * @return [Boolean]
 */
static VALUE
gdip_rectf_intersects_with(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tRectangleF)) {
        RectF *rect_self = Data_Ptr<RectF *>(self);
        RectF *rect_other = Data_Ptr<RectF *>(other);
        return rect_self->IntersectsWith(*rect_other) ? Qtrue : Qfalse;
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be RectFangleF.");
        return Qfalse;
    }
}

/**
 * Replaces this rectangle with the intersection of this and another rectangles.
 * @param other [Gdiplus::RectangleF]
 * @return [self]
 */
static VALUE
gdip_rectf_intersect_bang(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tRectangleF)) {
        RectF *rect_self = Data_Ptr<RectF *>(self);
        RectF *rect_other = Data_Ptr<RectF *>(other);
        rect_self->Intersect(*rect_other);
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be RectFangleF.");
    }
    return self;
}

/**
 * Returns a rectangle that is the intersection of this and another rectangles.
 * @param other [Gdiplus::RectangleF]
 * @return [Gdiplus::RectangleF]
 */
static VALUE
gdip_rectf_intersect(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tRectangleF)) {
        RectF *rect_self = Data_Ptr<RectF *>(self);
        VALUE r = gdip_rectf_create(rect_self);
        RectF *rect_r = Data_Ptr<RectF *>(r);
        RectF *rect_other = Data_Ptr<RectF *>(other);
        rect_r->Intersect(*rect_other);
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be RectFangleF.");
        return Qnil;
    }
}

/**
 * Returns a rectangle that contains the union of this and another rectangles.
 * @param other [Gdiplus::RectangleF]
 * @return [Gdiplus::RectangleF]
 */
static VALUE
gdip_rectf_union(VALUE self, VALUE other)
{
    if (_KIND_OF(other, &tRectangleF)) {
        RectF *rect_self = Data_Ptr<RectF *>(self);
        VALUE r = gdip_rectf_create();
        RectF *rect_r = Data_Ptr<RectF *>(r);
        RectF *rect_other = Data_Ptr<RectF *>(other);
        RectF::Union(*rect_r, *rect_self, *rect_other);
        return r;
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be RectangleF.");
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
    ATTR_RW(cPoint, X, x, point);
    ATTR_RW(cPoint, Y, y, point);
    rb_define_method(cPoint, "==", RUBY_METHOD_FUNC(gdip_point_equal), 1);
    rb_define_method(cPoint, "+", RUBY_METHOD_FUNC(gdip_point_add), 1);
    rb_define_method(cPoint, "-", RUBY_METHOD_FUNC(gdip_point_sub), 1);

    rb_define_method(cPointF, "initialize", RUBY_METHOD_FUNC(gdip_pointf_init), -1);
    rb_define_method(cPointF, "inspect", RUBY_METHOD_FUNC(gdip_pointf_inspect), 0);
    ATTR_RW(cPointF, X, x, pointf);
    ATTR_RW(cPointF, Y, y, pointf);
    rb_define_method(cPointF, "==", RUBY_METHOD_FUNC(gdip_pointf_equal), 1);
    rb_define_method(cPointF, "+", RUBY_METHOD_FUNC(gdip_pointf_add), 1);
    rb_define_method(cPointF, "-", RUBY_METHOD_FUNC(gdip_pointf_sub), 1);

    rb_define_method(cSize, "initialize", RUBY_METHOD_FUNC(gdip_size_init), -1);
    rb_define_method(cSize, "inspect", RUBY_METHOD_FUNC(gdip_size_inspect), 0);
    ATTR_RW(cSize, Width, width, size);
    ATTR_RW(cSize, Height, height, size);
    rb_define_method(cSize, "==", RUBY_METHOD_FUNC(gdip_size_equal), 1);
    rb_define_method(cSize, "+", RUBY_METHOD_FUNC(gdip_size_add), 1);
    rb_define_method(cSize, "-", RUBY_METHOD_FUNC(gdip_size_sub), 1);

    rb_define_method(cSizeF, "initialize", RUBY_METHOD_FUNC(gdip_sizef_init), -1);
    rb_define_method(cSizeF, "inspect", RUBY_METHOD_FUNC(gdip_sizef_inspect), 0);
    ATTR_RW(cSizeF, Width, width, sizef);
    ATTR_RW(cSizeF, Height, height, sizef);
    rb_define_method(cSizeF, "==", RUBY_METHOD_FUNC(gdip_sizef_equal), 1);
    rb_define_method(cSizeF, "+", RUBY_METHOD_FUNC(gdip_sizef_add), 1);
    rb_define_method(cSizeF, "-", RUBY_METHOD_FUNC(gdip_sizef_sub), 1);

    rb_define_singleton_method(cRectangle, "FromLTRB", RUBY_METHOD_FUNC(gdip_rect_s_from_ltrb), 4);
    rb_define_alias(rb_singleton_class(cRectangle), "from_ltrb", "FromLTRB");
    rb_define_method(cRectangle, "initialize", RUBY_METHOD_FUNC(gdip_rect_init), -1);
    rb_define_method(cRectangle, "inspect", RUBY_METHOD_FUNC(gdip_rect_inspect), 0);
    ATTR_RW(cRectangle, X, x, rect);
    /**
     * Gets the x coordinate the left edge of this rectangle.
     * @return [Float]
     */
    ATTR_R(cRectangle, Left, left, rect, x);
    ATTR_RW(cRectangle, Y, y, rect);
    /**
     * Gets the y coordinate the top edge of this rectangle.
     * @return [Float]
     */
    ATTR_R(cRectangle, Top, top, rect, y);
    ATTR_RW(cRectangle, Width, width, rect);
    ATTR_RW(cRectangle, Height, height, rect);
    ATTR_R(cRectangle, Right, right, rect);
    ATTR_R(cRectangle, Bottom, bottom, rect);
    ATTR_RW(cRectangle, Location, location, rect);
    ATTR_RW(cRectangle, Size, size, rect);
    rb_define_method(cRectangle, "==", RUBY_METHOD_FUNC(gdip_rect_equal), 1);
    rb_define_method(cRectangle, "Contains", RUBY_METHOD_FUNC(gdip_rect_contains), -1);
    rb_define_alias(cRectangle, "contains?", "Contains");
    rb_define_method(cRectangle, "Inflate", RUBY_METHOD_FUNC(gdip_rect_inflate_bang), -1);
    rb_define_alias(cRectangle, "inflate!", "Inflate");
    rb_define_method(cRectangle, "inflate", RUBY_METHOD_FUNC(gdip_rect_inflate), -1);
    rb_define_method(cRectangle, "IntersectsWith", RUBY_METHOD_FUNC(gdip_rect_intersects_with), 1);
    rb_define_alias(cRectangle, "intersects_with?", "IntersectsWith");
    rb_define_method(cRectangle, "Intersect", RUBY_METHOD_FUNC(gdip_rect_intersect_bang), 1);
    rb_define_alias(cRectangle, "intersect!", "Intersect");
    rb_define_method(cRectangle, "intersect", RUBY_METHOD_FUNC(gdip_rect_intersect), 1);
    rb_define_method(cRectangle, "Union", RUBY_METHOD_FUNC(gdip_rect_union), 1);
    rb_define_alias(cRectangle, "union", "Union");

    rb_define_singleton_method(cRectangleF, "FromLTRB", RUBY_METHOD_FUNC(gdip_rectf_s_from_ltrb), 4);
    rb_define_alias(rb_singleton_class(cRectangleF), "from_ltrb", "FromLTRB");
    rb_define_method(cRectangleF, "initialize", RUBY_METHOD_FUNC(gdip_rectf_init), -1);
    rb_define_method(cRectangleF, "inspect", RUBY_METHOD_FUNC(gdip_rectf_inspect), 0);
    ATTR_RW(cRectangleF, X, x, rectf);
    /**
     * Gets the x coordinate the left edge of this rectangle.
     * @return [Float]
     */
    ATTR_R(cRectangleF, Left, left, rectf, x);
    ATTR_RW(cRectangleF, Y, y, rectf);
    /**
     * Gets the y coordinate the top edge of this rectangle.
     * @return [Float]
     */
    ATTR_R(cRectangleF, Top, top, rectf, y);
    ATTR_RW(cRectangleF, Width, width, rectf);
    ATTR_RW(cRectangleF, Height, height, rectf);
    ATTR_R(cRectangleF, Right, right, rectf);
    ATTR_R(cRectangleF, Bottom, bottom, rectf);
    ATTR_RW(cRectangleF, Location, location, rectf);
    ATTR_RW(cRectangleF, Size, size, rectf);
    rb_define_method(cRectangleF, "==", RUBY_METHOD_FUNC(gdip_rectf_equal), 1);
    rb_define_method(cRectangleF, "Contains", RUBY_METHOD_FUNC(gdip_rectf_contains), -1);
    rb_define_alias(cRectangleF, "contains?", "Contains");
    rb_define_method(cRectangleF, "Inflate", RUBY_METHOD_FUNC(gdip_rectf_inflate_bang), -1);
    rb_define_alias(cRectangleF, "inflate!", "Inflate");
    rb_define_method(cRectangleF, "inflate", RUBY_METHOD_FUNC(gdip_rectf_inflate), -1);
    rb_define_method(cRectangleF, "IntersectsWith", RUBY_METHOD_FUNC(gdip_rectf_intersects_with), 1);
    rb_define_alias(cRectangleF, "intersects_with?", "IntersectsWith");
    rb_define_method(cRectangleF, "Intersect", RUBY_METHOD_FUNC(gdip_rectf_intersect_bang), 1);
    rb_define_alias(cRectangleF, "intersect!", "Intersect");
    rb_define_method(cRectangleF, "intersect", RUBY_METHOD_FUNC(gdip_rectf_intersect), 1);
    rb_define_method(cRectangleF, "Union", RUBY_METHOD_FUNC(gdip_rectf_union), 1);
    rb_define_alias(cRectangleF, "union", "Union");
}
