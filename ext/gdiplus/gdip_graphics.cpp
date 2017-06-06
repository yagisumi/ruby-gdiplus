/*
 * gdip_graphics.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tGraphics = _MAKE_DATA_TYPE(
    "Graphics", 0, GDIP_OBJ_FREE(Graphics *), NULL, NULL, &cGraphics);

VALUE
gdip_graphics_create(Graphics *g)
{
    g = gdip_obj_create(g);
    VALUE r = typeddata_alloc_null<&tGraphics>(cGraphics);
    _DATA_PTR(r) = g;
    return r;
}

/**
 * Gets the rectangle of clipping region.
 * @return [RectangleF]
 * 
 */
static VALUE
gdip_graphics_get_clip_bounds(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    RectF rect;
    Status status = g->GetClipBounds(&rect);
    Check_Status(status);
    return gdip_rectf_create(&rect);
}

/**
 * Gets or sets the compositing mode of this graphics.
 * @return [Gdiplus::CompositingMode]
 * 
 */
static VALUE
gdip_graphics_get_compositing_mode(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return gdip_enumint_create(cCompositingMode, g->GetCompositingMode());
}

static VALUE
gdip_graphics_set_compositing_mode(VALUE self, VALUE v)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    int enumint = gdip_arg_to_enumint(cCompositingMode, v);
    Status status = g->SetCompositingMode(static_cast<CompositingMode>(enumint));
    Check_Status(status);
    return self;
}

/**
 * Gets or sets the rendering quality.
 * @return [Gdiplus::CompositingQuality]
 * 
 */
static VALUE
gdip_graphics_get_compositing_quality(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return gdip_enumint_create(cCompositingQuality, g->GetCompositingQuality());
}

static VALUE
gdip_graphics_set_compositing_quality(VALUE self, VALUE v)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    int enumint = gdip_arg_to_enumint(cCompositingQuality, v);
    Status status = g->SetCompositingQuality(static_cast<CompositingQuality>(enumint));
    Check_Status(status);
    return self;
}

/**
 * Gets the horizontal resolution of this Graphics.
 * @return [Float]
 * 
 */
static VALUE
gdip_graphics_get_dpi_x(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return SINGLE2NUM(g->GetDpiX());
}

/**
 * Gets the vertical resolution of this Graphics.
 * @return [Float]
 * 
 */
static VALUE
gdip_graphics_get_dpi_y(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return SINGLE2NUM(g->GetDpiY());
}

/**
 * Gets or sets the interpolation mode.
 * @return [Gdiplus::InterpolationMode]
 * 
 */
static VALUE
gdip_graphics_get_interpolation_mode(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return gdip_enumint_create(cCompositingQuality, g->GetInterpolationMode());
}

static VALUE
gdip_graphics_set_interpolation_mode(VALUE self, VALUE v)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    int enumint = gdip_arg_to_enumint(cInterpolationMode, v);
    Status status = g->SetInterpolationMode(static_cast<InterpolationMode>(enumint));
    Check_Status(status);
    return self;
}

/**
 * Whether the clipping region of this Graphics is empty.
 * @return [Boolean]
 * 
 */
static VALUE
gdip_graphics_get_is_clip_empty(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return (g->IsClipEmpty()) ? Qtrue : Qfalse;
}

/**
 * Whether the visible clipping region of this Graphics is empty.
 * @return [Boolean]
 * 
 */
static VALUE
gdip_graphics_get_is_visible_clip_empty(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return (g->IsVisibleClipEmpty()) ? Qtrue : Qfalse;
}

/**
 * Gets or sets the page scale.
 * @return [Float]
 * 
 */
static VALUE
gdip_graphics_get_page_scale(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return SINGLE2NUM(g->GetPageScale());
}

static VALUE
gdip_graphics_set_page_scale(VALUE self, VALUE v)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    float scale;
    gdip_arg_to_single(v, &scale);
    Status status = g->SetPageScale(scale);
    Check_Status(status);
    return self;
}

/**
 * Gets or sets the page unit.
 * @return [GraphicsUnit]
 * 
 */
static VALUE
gdip_graphics_get_page_unit(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return gdip_enumint_create(cGraphicsUnit, g->GetPageUnit());
}

static VALUE
gdip_graphics_set_page_unit(VALUE self, VALUE v)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    int enumint = gdip_arg_to_enumint(cGraphicsUnit, v);
    Status status = g->SetPageUnit(static_cast<Unit>(enumint));
    Check_Status(status);
    return self;
}

/**
 * Gets or set the PixelOffsetMode.
 * @return [Gdiplus::PixelOffsetMode]
 * 
 */
static VALUE
gdip_graphics_get_pixel_offset_mode(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return gdip_enumint_create(cPixelOffsetMode, g->GetPixelOffsetMode());
}

static VALUE
gdip_graphics_set_pixel_offset_mode(VALUE self, VALUE v)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    int enumint = gdip_arg_to_enumint(cPixelOffsetMode, v);
    Status status = g->SetPixelOffsetMode(static_cast<PixelOffsetMode>(enumint));
    Check_Status(status);
    return self;
}

/**
 * Gets or sets the rendering origin of this graphics for hatch brushes.
 * @return [Point]
 * 
 */
static VALUE
gdip_graphics_get_rendering_origin(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    int x, y;
    Status status = g->GetRenderingOrigin(&x, &y);
    Check_Status(status);
    return gdip_point_create(x, y);
}

static VALUE
gdip_graphics_set_rendering_origin(VALUE self, VALUE v)
{
    if (_KIND_OF(v, &tPoint)) {
        Graphics *g = Data_Ptr<Graphics *>(self);
        Check_NULL(g, "The graphics object does not exist.");
        Point *point = Data_Ptr<Point *>(v);
        Status status = g->SetRenderingOrigin(point->X, point->Y);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Point.");
    }
    return self;
}

/**
 * Gets or sets the soothing mode of this graphics.
 * @return [Gdiplus::SmoothingMode]
 * 
 */
static VALUE
gdip_graphics_get_smoothing_mode(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return gdip_enumint_create(cSmoothingMode, g->GetSmoothingMode());
}

static VALUE
gdip_graphics_set_smoothing_mode(VALUE self, VALUE mode)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    int enum_mode = gdip_arg_to_enumint(cSmoothingMode, mode);
    g->SetSmoothingMode(static_cast<SmoothingMode>(enum_mode));
    return self;
}

/**
 * Gets or sets the text contrast.
 * @return [Integer] This value should be between 0 and 12.
 * 
 */
static VALUE
gdip_graphics_get_text_contrast(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return RB_INT2NUM(g->GetTextContrast());
}

static VALUE
gdip_graphics_set_text_contrast(VALUE self, VALUE v)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    if (Integer_p(v)) {
        int x = RB_NUM2INT(v);
        if (x < 0) {
            x = 0;
            _VERBOSE("The argument should be between 0 and 12.");
        }
        else if (12 < x) {
            x = 12;
            _VERBOSE("The argument should be between 0 and 12.");
        }
        Status status = g->SetTextContrast(x);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Integer.");
    }

    return self;
}

/**
 * Gets or sets the text rendering mode for this graphics.
 * @return [Gdiplus::TextRenderingHint]
 * 
 */
static VALUE
gdip_graphics_get_text_rendering_hint(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return gdip_enumint_create(cTextRenderingHint, g->GetTextRenderingHint());
}

static VALUE
gdip_graphics_set_text_rendering_hint(VALUE self, VALUE v)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    int enumint = gdip_arg_to_enumint(cTextRenderingHint, v);
    if (enumint == TextRenderingHintClearTypeGridFit && g->GetCompositingMode() == CompositingModeSourceCopy) {
        _WARNING("Do not set ClearTypeGridFit when CompositingMode is SourceCopy.");
    }
    else {
        Status status = g->SetTextRenderingHint(static_cast<TextRenderingHint>(enumint));
        Check_Status(status);
    }
    return self;
}

/**
 * Gets the VisibleClipBounds.
 * @return [RectangleF]
 * 
 */
static VALUE
gdip_graphics_get_visible_clip_bounds(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    RectF rect;
    Status status = g->GetVisibleClipBounds(&rect);
    Check_Status(status);
    return gdip_rectf_create(&rect);
}

//
// METHOD
//

/**
 * @example
 *   bmp.draw {|g|
 *     g.DrawRectangle(pen, rect)
 *     g.DrawRectangle(pen, 10, 10, 300, 200)
 *     g.DrawRectangle(pen, 100.0, 100.0, 60.0, 30.0)
 *   }
 * @return [self]
 * @overload DrawRectangle(pen, rectangle)
 *   @param pen [Pen]
 *   @param rectangle [Rectangle or RectangleF]
 * @overload DrawRectangle(pen, x, y, width, height)
 *   @param pen [Pen]
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 *   
 */
static VALUE
gdip_graphics_draw_rectangle(int argc, VALUE *argv, VALUE self)
{
    if (argc != 2 && argc != 5) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2 or 5)", argc);
    }
    if (!_KIND_OF(argv[0], &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Brush.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(argv[0]);
    if (argc == 2) {
        if (_KIND_OF(argv[1], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[1]);
            g->DrawRectangle(pen, *rect);
        }
        else if (_KIND_OF(argv[1], &tRectangle)) {
            RectF *rect = Data_Ptr<RectF *>(argv[1]);
            g->DrawRectangle(pen, *rect);
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Rectangle or RectangleF.");
        }

    }
    else if (argc == 5) {
        if (Integer_p(argv[1], argv[2], argv[3], argv[4])) {
            g->DrawRectangle(pen, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]));
        }
        else if (Float_p(argv[1], argv[2], argv[3], argv[4])) {
            g->DrawRectangle(pen, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer or Float.");
        }
    }
    
    return self;
}

/**
 * @example
 *   bmp.draw {|g|
 *     g.FillRectangle(brush, rect)
 *     g.FillRectangle(brush, 10, 10, 300, 200)
 *     g.FillRectangle(brush, 100.0, 100.0, 60.0, 30.0)
 *   }
 * @return [self]
 * @overload FillRectangle(brush, rectangle)
 *   @param brush [Brush]
 *   @param rectangle [Rectangle or RectangleF]
 * @overload FillRectangle(brush, x, y, width, height)
 *   @param brush [Brush]
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 *   
 */
static VALUE
gdip_graphics_fill_rectangle(int argc, VALUE *argv, VALUE self)
{
    if (argc != 2 && argc != 5) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2 or 5)", argc);
    }
    if (!_KIND_OF(argv[0], &tBrush)) {
        rb_raise(rb_eTypeError, "The first argument should be Brush.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Brush *brush = Data_Ptr<Brush *>(argv[0]);
    if (argc == 2) {
        if (_KIND_OF(argv[1], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[1]);
            g->FillRectangle(brush, *rect);
        }
        else if (_KIND_OF(argv[1], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[1]);
            g->FillRectangle(brush, *rect);
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Rectangle or RectangleF.");
        }

    }
    else if (argc == 5) {
        if (Integer_p(argv[1], argv[2], argv[3], argv[4])) {
            g->FillRectangle(brush, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]));
        }
        else if (Float_p(argv[1], argv[2], argv[3], argv[4])) {
            g->FillRectangle(brush, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer or Float.");
        }
    }
    
    return self;
}

/**
 * @example
 *   bmp.draw {|g|
 *     g.DrawLine(pen, point1, point2)
 *     g.DrawLine(pen, 10, 10, 50, 60)
 *     g.DrawLine(pen, 30.0, 100.0, 60.0, 100.0)
 *   }
 * @return [self]
 * @overload DrawLine(pen, point1, point2)
 *   @param pen [Pen]
 *   @param point1 [Point or PointF]
 *   @param point2 [Point or PointF]
 * @overload DrawLine(pen, x1, y1, x2, y2)
 *   @param pen [Pen]
 *   @param x1 [Integer or Float]
 *   @param y1 [Integer or Float]
 *   @param x2 [Integer or Float]
 *   @param y2 [Integer or Float]
 */
static VALUE
gdip_graphics_draw_line(int argc, VALUE *argv, VALUE self)
{
    if (argc != 3 && argc != 5) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 3 or 5)", argc);
    }
    if (!_KIND_OF(argv[0], &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(argv[0]);
    if (argc == 3) {
        if (_KIND_OF(argv[1], &tPoint) && _KIND_OF(argv[2], &tPoint)) {
            Point *po1 = Data_Ptr<Point *>(argv[1]);
            Point *po2 = Data_Ptr<Point *>(argv[2]);
            g->DrawLine(pen, po1->X, po1->Y, po2->X, po2->Y);
        }
        else if (_KIND_OF(argv[1], &tPointF) && _KIND_OF(argv[2], &tPointF)) {
            PointF *po1 = Data_Ptr<PointF *>(argv[1]);
            PointF *po2 = Data_Ptr<PointF *>(argv[2]);
            g->DrawLine(pen, po1->X, po1->Y, po2->X, po2->Y);
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Point or PointF.");
        }
    }
    else if (argc == 5) {
        if (Integer_p(argv[1], argv[2], argv[3], argv[4])) {
            g->DrawLine(pen, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]));
        }
        else if (Float_p(argv[1], argv[2], argv[3], argv[4])) {
            g->DrawLine(pen, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer or Float.");
        }
    }

    return self;
}

/**
 * @example
 *   points = [Point.new(0, 10), Point.new(30, 30), Point.new(100, 30)]
 *   bmp.draw {|g|
 *     g.DrawLines(pen, points)
 *   }
 * @overload DrawLines(pen, points)
 *   @param pen [Pen]
 *   @param points [Array<Point or PointF>]
 * @return [self]
 */
static VALUE
gdip_graphics_draw_lines(VALUE self, VALUE pen_v, VALUE ary)
{
    if (!_KIND_OF(pen_v, &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }
    if (!_RB_ARRAY_P(ary)) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(pen_v);

    if (RARRAY_LEN(ary) == 0) {
        _VERBOSE("The array is empty.");
    }
    else {
        VALUE first = rb_ary_entry(ary, 0);
        if (_KIND_OF(first, &tPoint)) {
            int count = 0;
            for (long i = 0; i < RARRAY_LEN(ary); ++i) {
                VALUE elem = rb_ary_entry(ary, i);
                if (_KIND_OF(elem, &tPoint)) {
                    count += 1;
                }
            }
            int idx = 0;
            Point *points = static_cast<Point *>(ruby_xmalloc(count * sizeof(Point)));
            for (long i = 0; i < RARRAY_LEN(ary); ++i) {
                VALUE elem = rb_ary_entry(ary, i);
                if (_KIND_OF(elem, &tPoint)) {
                    Point *po = Data_Ptr<Point *>(elem);
                    points[idx] = *po;
                    idx += 1;
                }
            }
            Status status = g->DrawLines(pen, points, count);
            ruby_xfree(points);
            Check_Status(status);
        }
        else if (_KIND_OF(first, &tPointF)) {
            int count = 0;
            for (long i = 0; i < RARRAY_LEN(ary); ++i) {
                VALUE elem = rb_ary_entry(ary, i);
                if (_KIND_OF(elem, &tPointF)) {
                    count += 1;
                }
            }
            int idx = 0;
            PointF *points = static_cast<PointF *>(ruby_xmalloc(count * sizeof(PointF)));
            for (long i = 0; i < RARRAY_LEN(ary); ++i) {
                VALUE elem = rb_ary_entry(ary, i);
                if (_KIND_OF(elem, &tPointF)) {
                    PointF *po = Data_Ptr<PointF *>(elem);
                    points[idx] = *po;
                    idx += 1;
                }
            }
            Status status = g->DrawLines(pen, points, count);
            ruby_xfree(points);
            Check_Status(status);
        }
        else {
            rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
        }
    }
    return self;
}

/**
 * @example
 *   points = []
 *   points << Point.new(100, 100)
 *   points << Point.new(200, 100)
 *   points << Point.new(200, 200)
 *   points << Point.new(100, 200)
 *   bmp {|g|
 *     g.DrawCurve(pen, points);
 *   }
 * @overload DrawCurve(pen, points)
 *   @param pen [Pen]
 *   @param points [Array<Point or PointF>]
 * @overload DrawCurve(pen, points, tention)
 *   @param pen [Pen]
 *   @param points [Array<Point or PointF>]
 *   @param tention [Float] 0.0-1.0
 * @overload DrawCurve(pen, points, offset, num, tention)
 *   @param pen [Pen]
 *   @param points [Array<Point or PointF>]
 *   @param offset [Integer] start index of points
 *   @param num [Integer] number of segments
 *   @param tention [Float] 0.0-1.0
 * @return [self]
 */
static VALUE
gdip_graphics_draw_curve(int argc, VALUE *argv, VALUE self)
{
    if (argc < 2 || 5 < argc) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2..5)", argc);
    }
    if (!_KIND_OF(argv[0], &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }
    if (!_RB_ARRAY_P(argv[1])) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    if (argc == 3 && !Float_p(argv[2])) {
        rb_raise(rb_eTypeError, "The last argument should be Float.");
    }
    else if (argc == 4 && !Integer_p(argv[2], argv[3])) {
        rb_raise(rb_eTypeError, "The arguments should be Integer.");
    }
    else if (argc == 5) {
        if (!Float_p(argv[4])) {
            rb_raise(rb_eTypeError, "The last argument should be Float.");
        }
        else if (!Integer_p(argv[2], argv[3])) {
            rb_raise(rb_eTypeError, "The arguments should be Integer.");
        }
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(argv[0]);

    if (RARRAY_LEN(argv[1]) == 0) {
        _VERBOSE("The array is empty.");
    }
    else {
        VALUE first = rb_ary_entry(argv[1], 0);
        if (_KIND_OF(first, &tPoint)) {
            
            int count = 0;
            for (long i = 0; i < RARRAY_LEN(argv[1]); ++i) {
                VALUE elem = rb_ary_entry(argv[1], i);
                if (_KIND_OF(elem, &tPoint)) {
                    count += 1;
                }
            }
            int idx = 0;
            Point *points = static_cast<Point *>(ruby_xmalloc(count * sizeof(Point)));
            for (long i = 0; i < RARRAY_LEN(argv[1]); ++i) {
                VALUE elem = rb_ary_entry(argv[1], i);
                if (_KIND_OF(elem, &tPoint)) {
                    Point *po = Data_Ptr<Point *>(elem);
                    points[idx] = *po;
                    idx += 1;
                }
            }
            Status status;
            if (argc == 2) {
                status = g->DrawCurve(pen, points, count);
            }
            else if (argc == 3) {
                float tension = clamp(NUM2SINGLE(argv[2]), 0.0f, 1.0f);
                status = g->DrawCurve(pen, points, count, tension);
            }
            else if (argc == 4) {
                int offset = RB_NUM2INT(argv[2]);
                int num = RB_NUM2INT(argv[3]);
                status = g->DrawCurve(pen, points, count, offset, num, 0.5f);
            }
            else if (argc == 5) {
                int offset = RB_NUM2INT(argv[2]);
                int num = RB_NUM2INT(argv[3]);
                float tension = clamp(NUM2SINGLE(argv[4]), 0.0f, 1.0f);
                status = g->DrawCurve(pen, points, count, offset, num, tension);
            }
            ruby_xfree(points);
            Check_Status(status);
        }
        else if (_KIND_OF(first, &tPointF)) {
            int count = 0;
            for (long i = 0; i < RARRAY_LEN(argv[1]); ++i) {
                VALUE elem = rb_ary_entry(argv[1], i);
                if (_KIND_OF(elem, &tPointF)) {
                    count += 1;
                }
            }
            int idx = 0;
            PointF *points = static_cast<PointF *>(ruby_xmalloc(count * sizeof(PointF)));
            for (long i = 0; i < RARRAY_LEN(argv[1]); ++i) {
                VALUE elem = rb_ary_entry(argv[1], i);
                if (_KIND_OF(elem, &tPointF)) {
                    PointF *po = Data_Ptr<PointF *>(elem);
                    points[idx] = *po;
                    idx += 1;
                }
            }
            Status status;
            if (argc == 2) {
                status = g->DrawCurve(pen, points, count);
            }
            else if (argc == 3) {
                float tension = clamp(NUM2SINGLE(argv[2]), 0.0f, 1.0f);
                status = g->DrawCurve(pen, points, count, tension);
            }
            else if (argc == 4) {
                int offset = RB_NUM2INT(argv[2]);
                int num = RB_NUM2INT(argv[3]);
                status = g->DrawCurve(pen, points, count, offset, num, 0.5f);
            }
            else if (argc == 5) {
                int offset = RB_NUM2INT(argv[2]);
                int num = RB_NUM2INT(argv[3]);
                float tension = clamp(NUM2SINGLE(argv[4]), 0.0f, 1.0f);
                status = g->DrawCurve(pen, points, count, offset, num, tension);
            }
            ruby_xfree(points);
            Check_Status(status);
        }
        else {
            rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
        }
    }
    return self;
}

/**
 * @example
 *   bmp.draw {|g|
 *     g.DrawEllipse(pen, rect)
 *     g.DrawEllipse(pen, 30, 30, 100, 100)
 *     g.DrawEllipse(pen, 100.0, 100.0, 50.0, 50.0)
 *   }
 * @overload DrawEllipse(pen, rectangle)
 *   @param pen [Pen]
 *   @param rectangle [Rectangle or RectangleF]
 * @overload DrawEllipse(pen, x, y, width, height)
 *   @param pen [Pen]
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_graphics_draw_ellipse(int argc, VALUE *argv, VALUE self)
{
    if (argc != 2 && argc != 5) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2 or 5)", argc);
    }
    if (!_KIND_OF(argv[0], &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(argv[0]);
    if (argc == 2) {
        if (_KIND_OF(argv[1], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[1]);
            g->DrawEllipse(pen, *rect);
        }
        else if (_KIND_OF(argv[1], &tRectangle)) {
            RectF *rect = Data_Ptr<RectF *>(argv[1]);
            g->DrawEllipse(pen, *rect);
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Rectangle or RectangleF.");
        }

    }
    else if (argc == 5) {
        if (Integer_p(argv[1], argv[2], argv[3], argv[4])) {
            g->DrawEllipse(pen, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]));
        }
        else if (Float_p(argv[1], argv[2], argv[3], argv[4])) {
            g->DrawEllipse(pen, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer or Float.");
        }
    }
    
    return self;
}

/**
 * @example
 *   bmp.draw {|g|
 *     g.FillEllipse(brush, rect)
 *     g.FillEllipse(brush, 30, 30, 100, 100)
 *     g.FillEllipse(brush, 100.0, 100.0, 50.0, 50.0)
 *   }
 * @overload FillEllipse(brush, rectangle)
 *   @param pen [Brush]
 *   @param rectangle [Rectangle or RectangleF]
 * @overload FillEllipse(brush, x, y, width, height)
 *   @param pen [Brush]
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_graphics_fill_ellipse(int argc, VALUE *argv, VALUE self)
{
    if (argc != 2 && argc != 5) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2 or 5)", argc);
    }
    if (!_KIND_OF(argv[0], &tBrush)) {
        rb_raise(rb_eTypeError, "The first argument should be Brush.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
   Brush *brush = Data_Ptr<Brush *>(argv[0]);
    if (argc == 2) {
        if (_KIND_OF(argv[1], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[1]);
            g->FillEllipse(brush, *rect);
        }
        else if (_KIND_OF(argv[1], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[1]);
            g->FillEllipse(brush, *rect);
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Rectangle or RectangleF.");
        }

    }
    else if (argc == 5) {
        if (Integer_p(argv[1], argv[2], argv[3], argv[4])) {
            g->FillEllipse(brush, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]));
        }
        else if (Float_p(argv[1], argv[2], argv[3], argv[4])) {
            g->FillEllipse(brush, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer or Float.");
        }
    }
    
    return self;
}

void
Init_graphics()
{
    cGraphics = rb_define_class_under(mGdiplus, "Graphics", rb_cObject);
    rb_undef_alloc_func(cGraphics);

    ATTR_R(cGraphics, ClipBounds, clip_bounds, graphics);
    ATTR_RW(cGraphics, CompositingMode, compositing_mode, graphics);
    ATTR_RW(cGraphics, CompositingQuality, compositing_quality, graphics);
    ATTR_R(cGraphics, DpiX, dpi_x, graphics);
    ATTR_R(cGraphics, DpiY, dpi_y, graphics);
    ATTR_RW(cGraphics, InterpolationMode, interpolation_mode, graphics);
    ATTR_R_Q(cGraphics, IsClipEmpty, is_clip_empty, graphics);
    ATTR_R_Q(cGraphics, IsVisibleClipEmpty, is_visible_clip_empty, graphics);
    ATTR_RW(cGraphics, PageScale, page_scale, graphics);
    ATTR_RW(cGraphics, PageUnit, page_unit, graphics);
    ATTR_RW(cGraphics, PixelOffsetMode, pixel_offset_mode, graphics);
    ATTR_RW(cGraphics, RenderingOrigin, rendering_origin, graphics);
    ATTR_RW(cGraphics, SmoothingMode, smoothing_mode, graphics);
    ATTR_RW(cGraphics, TextContrast, text_contrast, graphics);
    ATTR_RW(cGraphics, TextRenderingHint, text_rendering_hint, graphics);
    ATTR_R(cGraphics, VisibleClipBounds, visible_clip_bounds, graphics);
    
    rb_define_method(cGraphics, "DrawRectangle", RUBY_METHOD_FUNC(gdip_graphics_draw_rectangle), -1);
    rb_define_alias(cGraphics, "draw_rectangle", "DrawRectangle");
    rb_define_method(cGraphics, "FillRectangle", RUBY_METHOD_FUNC(gdip_graphics_fill_rectangle), -1);
    rb_define_alias(cGraphics, "fill_rectangle", "FillRectangle");
    rb_define_method(cGraphics, "DrawLine", RUBY_METHOD_FUNC(gdip_graphics_draw_line), -1);
    rb_define_alias(cGraphics, "draw_line", "DrawLine");
    rb_define_method(cGraphics, "DrawLines", RUBY_METHOD_FUNC(gdip_graphics_draw_lines), 2);
    rb_define_alias(cGraphics, "draw_lines", "DrawLines");
    rb_define_method(cGraphics, "DrawEllipse", RUBY_METHOD_FUNC(gdip_graphics_draw_ellipse), -1);
    rb_define_alias(cGraphics, "draw_ellipse", "DrawEllipse");
    rb_define_method(cGraphics, "FillEllipse", RUBY_METHOD_FUNC(gdip_graphics_fill_ellipse), -1);
    rb_define_alias(cGraphics, "fill_ellipse", "FillEllipse");
    rb_define_method(cGraphics, "DrawCurve", RUBY_METHOD_FUNC(gdip_graphics_draw_curve), -1);
    rb_define_alias(cGraphics, "draw_curve", "DrawCurve");
}