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
 * @return [CompositingMode]
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
    int enumint = 0;
    gdip_arg_to_enumint(cCompositingMode, v, &enumint, "The argument should be CompositingMode.", ArgOptionAcceptInt);
    Status status = g->SetCompositingMode(static_cast<CompositingMode>(enumint));
    Check_Status(status);
    return self;
}

/**
 * Gets or sets the rendering quality.
 * @return [CompositingQuality]
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
    int enumint = 0;
    gdip_arg_to_enumint(cCompositingQuality, v, &enumint, "The argument should be CompositingQuality.", ArgOptionAcceptInt);
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
 * @return [InterpolationMode]
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
    int enumint = 0;
    gdip_arg_to_enumint(cInterpolationMode, v, &enumint, "The argument should be InterpolationMode.", ArgOptionAcceptInt);
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
    gdip_arg_to_single(v, &scale, "The argument should be Float or Integer.");
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
    int enumint = 0;
    gdip_arg_to_enumint(cGraphicsUnit, v, &enumint, "The argument should be GraphicsUnit.", ArgOptionAcceptInt);
    Status status = g->SetPageUnit(static_cast<Unit>(enumint));
    Check_Status(status);
    return self;
}

/**
 * Gets or set the PixelOffsetMode.
 * @return [PixelOffsetMode]
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
    int enumint = 0;
    gdip_arg_to_enumint(cPixelOffsetMode, v, &enumint, "The argument should be PixelOffsetMode.", ArgOptionAcceptInt);
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
 * @return [SmoothingMode]
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
    int enumint = 0;
    gdip_arg_to_enumint(cSmoothingMode, mode, &enumint, "The argument should be SmoothingMode.", ArgOptionAcceptInt);
    g->SetSmoothingMode(static_cast<SmoothingMode>(enumint));
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
 * @return [TextRenderingHint]
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
    int enumint = 0;
    gdip_arg_to_enumint(cTextRenderingHint, v, &enumint, "The argument should be TextRenderingHint.", ArgOptionAcceptInt);
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


/**
 * @overload Clear(color)
 *   Fills the whole with the specified color.
 *   @param color [Color]
 *   @return [self]
 */
static VALUE
gdip_graphics_clear(VALUE self, VALUE color_v)
{
    Color color;
    gdip_arg_to_color(color_v, &color);
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    g->Clear(color);
    return self;
}


/**
 * @group Drawing Method Summary 
 * @example
 *   bmp.draw {|g|
 *     g.DrawRectangle(pen, rect)
 *     g.DrawRectangle(pen, 10, 10, 300, 200)
 *     g.DrawRectangle(pen, 100.0, 100.0, 60.0, 30.0)
 *   }
 * @overload DrawRectangle(pen, rectangle)
 *   @param pen [Pen]
 *   @param rectangle [Rectangle or RectangleF]
 * @overload DrawRectangle(pen, x, y, width, height)
 *   @param pen [Pen]
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 * @return [self]
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
    Check_NULL(pen, "The pen object does not exist.");

    if (argc == 2) {
        if (_KIND_OF(argv[1], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[1]);
            g->DrawRectangle(pen, *rect);
        }
        else if (_KIND_OF(argv[1], &tRectangleF)) {
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
 * @overload DrawRectangles(pen, rectangles)
 *   @param pen [Pen]
 *   @param rectangles [Array<Rectangle or RectangleF>]
 *   @return [self]
 */
static VALUE
gdip_graphics_draw_rectangles(VALUE self, VALUE v_pen, VALUE ary)
{
    if (!_KIND_OF(v_pen, &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }
    if (!_RB_ARRAY_P(ary) && RARRAY_LEN(ary) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Rectangle or RectangleF.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(v_pen);
    Check_NULL(pen, "The pen object does not exist.");

    VALUE first = rb_ary_entry(ary, 0);
    if (_KIND_OF(first, &tRectangle)) {
        int count;
        Rect *rects = alloc_array_of<Rect, &tRectangle>(ary, count);
        Status status = g->DrawRectangles(pen, rects, count);
        ruby_xfree(rects);
        Check_Status(status);
    }
    else if (_KIND_OF(first, &tRectangleF)) {
        int count;
        RectF *rects = alloc_array_of<RectF, &tRectangleF>(ary, count);
        Status status = g->DrawRectangles(pen, rects, count);
        ruby_xfree(rects);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Rectangle or RectangleF.");
    }

    return self;
}

/**
 * @overload FillRectangles(pen, rectangles)
 *   @param pen [Pen]
 *   @param rectangles [Array<Rectangle or RectangleF>]
 *   @return [self]
 */
static VALUE
gdip_graphics_fill_rectangles(VALUE self, VALUE v_brush, VALUE ary)
{
    if (!_KIND_OF(v_brush, &tBrush)) {
        rb_raise(rb_eTypeError, "The first argument should be Brush.");
    }
    if (!_RB_ARRAY_P(ary) && RARRAY_LEN(ary) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Rectangle or RectangleF.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Brush *brush = Data_Ptr<Brush *>(v_brush);
    Check_NULL(brush, "This Brush object does not exist.");

    VALUE first = rb_ary_entry(ary, 0);
    if (_KIND_OF(first, &tRectangle)) {
        int count;
        Rect *rects = alloc_array_of<Rect, &tRectangle>(ary, count);
        Status status = g->FillRectangles(brush, rects, count);
        ruby_xfree(rects);
        Check_Status(status);
    }
    else if (_KIND_OF(first, &tRectangleF)) {
        int count;
        RectF *rects = alloc_array_of<RectF, &tRectangleF>(ary, count);
        Status status = g->FillRectangles(brush, rects, count);
        ruby_xfree(rects);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Rectangle or RectangleF.");
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
 * @overload FillRectangle(brush, rectangle)
 *   @param brush [Brush]
 *   @param rectangle [Rectangle or RectangleF]
 * @overload FillRectangle(brush, x, y, width, height)
 *   @param brush [Brush]
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 * @return [self]
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
    Check_NULL(brush, "The brush object does not exist.");

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
 * @return [self]
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
    Check_NULL(pen, "The pen object does not exist.");

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
 * @overload DrawLines(pen, points)
 *   @param pen [Pen]
 *   @param points [Array<Point or PointF>]
 *   @return [self]
 * @example
 *   points = [Point.new(0, 10), Point.new(30, 30), Point.new(100, 30)]
 *   bmp.draw {|g|
 *     g.DrawLines(pen, points)
 *   }
 */
static VALUE
gdip_graphics_draw_lines(VALUE self, VALUE v_pen, VALUE ary)
{
    if (!_KIND_OF(v_pen, &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }
    if (!_RB_ARRAY_P(ary) && RARRAY_LEN(ary) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(v_pen);
    Check_NULL(pen, "The pen object does not exist.");

    VALUE first = rb_ary_entry(ary, 0);
    if (_KIND_OF(first, &tPoint)) {
        int count;
        Point *points = alloc_array_of<Point, &tPoint>(ary, count);
        Status status = g->DrawLines(pen, points, count);
        ruby_xfree(points);
        Check_Status(status);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count;
        PointF *points = alloc_array_of<PointF, &tPointF>(ary, count);
        Status status = g->DrawLines(pen, points, count);
        ruby_xfree(points);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
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
 * @overload DrawCurve(pen, points, tension=0.5)
 *   @param pen [Pen]
 *   @param points [Array<Point or PointF>]
 *   @param tension [Float] 0.0-1.0
 * @overload DrawCurve(pen, points, offset, num, tension=0.5)
 *   @param pen [Pen]
 *   @param points [Array<Point or PointF>]
 *   @param offset [Integer] start index of points
 *   @param num [Integer] number of segments
 *   @param tension [Float] 0.0-1.0
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
    if (!_RB_ARRAY_P(argv[1]) || RARRAY_LEN(argv[1]) == 0) {
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
    Check_NULL(pen, "The pen object does not exist.");

    VALUE first = rb_ary_entry(argv[1], 0);
    int offset = 0;
    int num = 0;
    if (argc > 3) {
        offset = RB_NUM2INT(argv[2]);
        num = RB_NUM2INT(argv[3]);
    }
    float tension = 0.5f;
    if (argc == 3) {
        tension = clamp(NUM2SINGLE(argv[2]), 0.0f, 1.0f);
    }
    if (argc == 5) {
        tension = clamp(NUM2SINGLE(argv[4]), 0.0f, 1.0f);
    }
    if (_KIND_OF(first, &tPoint)) {
        int count;
        Point *points = alloc_array_of<Point, &tPoint>(argv[1], count);
        Status status;
        if (argc > 3) {
            status = g->DrawCurve(pen, points, count, offset, num, tension);
        }
        else {
            status = g->DrawCurve(pen, points, count, tension);
        }
        ruby_xfree(points);
        Check_Status(status);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count;
        PointF *points = alloc_array_of<PointF, &tPointF>(argv[1], count);
        Status status;
        if (argc > 3) {
            status = g->DrawCurve(pen, points, count, offset, num, tension);
        }
        else {
            status = g->DrawCurve(pen, points, count, tension);
        }
        ruby_xfree(points);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
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
    Check_NULL(pen, "The pen object does not exist.");

    if (argc == 2) {
        if (_KIND_OF(argv[1], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[1]);
            g->DrawEllipse(pen, *rect);
        }
        else if (_KIND_OF(argv[1], &tRectangleF)) {
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
 *   @param brush [Brush]
 *   @param rectangle [Rectangle or RectangleF]
 * @overload FillEllipse(brush, x, y, width, height)
 *   @param brush [Brush]
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
    Check_NULL(g, "The brush object does not exist.");

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


/**
 * @overload DrawClosedCurve(pen, points, tension=0.5)
 *   @param pen [Pen]
 *   @param points [Array<Point or PointF>]
 *   @param tension [Float] 0.0-1.0
 *   @return [self]
 * @example
 *   bmp.draw {|g|
 *     g.DrawClosedCurve(pen, points)
 *   }
 */
static VALUE
gdip_graphics_draw_closed_curve(int argc, VALUE *argv, VALUE self)
{
    VALUE v_pen;
    VALUE v_ary;
    VALUE v_tension = Qnil;
    rb_scan_args(argc, argv, "21", &v_pen, &v_ary, &v_tension);

    if (!_KIND_OF(v_pen, &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(v_pen);
    Check_NULL(g, "The pen object does not exist.");
    float tension = 0.5f;

    if (!RB_NIL_P(v_tension)) {
        if (Float_p(v_tension)) {
            tension = clamp(NUM2SINGLE(v_tension), 0.0f, 1.0f);
        }
        else {
            rb_raise(rb_eTypeError, "The third argument should be Float.");
        }
    }

    if (_RB_ARRAY_P(v_ary) && RARRAY_LEN(v_ary) > 0) {
        VALUE first = rb_ary_entry(v_ary, 0);
        if (_KIND_OF(first, &tPoint)) {
            int count;
            Point *points = alloc_array_of<Point, &tPoint>(v_ary, count);
            Status status = g->DrawClosedCurve(pen, points, count, tension);
            ruby_xfree(points);
            Check_Status(status);
        }
        else if (_KIND_OF(first, &tPointF)) {
            int count;
            PointF *points = alloc_array_of<PointF, &tPointF>(v_ary, count);
            Status status = g->DrawClosedCurve(pen, points, count, tension);
            ruby_xfree(points);
            Check_Status(status);
        }
        else {
            rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
        }
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    return self;
}

/**
 * @example
 *   bmp.draw {|g|
 *     g.FillClosedCurve(brush, points)
 *   }
 * @overload FillClosedCurve(brush, points)
 *   @param brush [Brush]
 *   @param points [Array<Point or PointF>]
 * @overload FillClosedCurve(pen, points, fillmode, tension=0.5)
 *   @param brush [Brush]
 *   @param points [Array<Point or PointF>]
 *   @param fillmode [FillMode]
 *   @param tension [Float]
 * @return [self]
 */
static VALUE
gdip_graphics_fill_closed_curve(int argc, VALUE *argv, VALUE self)
{
    if (argc < 2 || 4 < argc) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2..4)", argc);
    }
    if (_KIND_OF(argv[1], &tBrush)) {
        rb_raise(rb_eTypeError, "The second argument should be Brush.");
    }
    if (_RB_ARRAY_P(argv[1]) && RARRAY_LEN(argv[1]) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    if (argc == 4 && !Float_p(argv[3])) {
        rb_raise(rb_eTypeError, "The fourth argument should be Float.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Brush *brush = Data_Ptr<Brush *>(argv[0]);
    Check_NULL(brush, "The brush object does not exist.");
    float tension = 0.5f;

    if (argc == 4) {
        tension = NUM2SINGLE(argv[3]);
    }
    FillMode fillmode = FillModeAlternate;
    if (argc > 2) {
        gdip_arg_to_enumint(cFillMode, argv[2], (int*)&fillmode, "The third argument should be FillMode.");
    }
    
    VALUE first = rb_ary_entry(argv[1], 0);
    if (_KIND_OF(first, &tPoint)) {
        int count;
        Point *points = alloc_array_of<Point, &tPoint>(argv[1], count);
        Status status;
        if (argc > 2) {
            status = g->FillClosedCurve(brush, points, count, fillmode, tension);
        }
        else {
            status = g->FillClosedCurve(brush, points, count);
        }
        ruby_xfree(points);
        Check_Status(status);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count;
        PointF *points = alloc_array_of<PointF, &tPointF>(argv[1], count);
        Status status;
        if (argc > 2) {
            status = g->FillClosedCurve(brush, points, count, fillmode, tension);
        }
        else {
            status = g->FillClosedCurve(brush, points, count);
        }
        ruby_xfree(points);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    
    return self;
}

/**
 * @example
 *   bmp.draw {|g|
 *     g.DrawArc(pen, rect, -30, 60)
 *     g.DrawArc(pen, 20, 20, 160, 160, 90.0, 180.0)
 *     g.DrawArc(pen1, 20.0, 220.0, 160.0, 160.0, 90.0, 180.0)
 *   }
 * @overload DrawArc(pen, rect, start_angle, sweep_angle)
 *   @param pen [Pen]
 *   @param rect [Rectangle or RectangleF]
 *   @param start_angle [Integer or Float]
 *   @param sweep_angle [Integer or Float]
 * @overload DrawArc(pen, x, y, width, height, start_angle, sweep_angle)
 *   @param pen [Pen]
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 *   @param start_angle [Integer or Float]
 *   @param sweep_angle [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_graphics_draw_arc(int argc, VALUE *argv, VALUE self)
{
    if (argc != 4 && argc != 7) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 4 or 7)", argc);
    }
    if (!_KIND_OF(argv[0], &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(argv[0]);
    Check_NULL(pen, "The pen object does not exist.");

    float start_angle;
    float sweep_angle;
    if (argc == 4 && !gdip_arg_to_single(argv[2], &start_angle)) {
        rb_raise(rb_eTypeError, "The argument of the start angle should be Integer or Float.");
    }
    if (argc == 4 && !gdip_arg_to_single(argv[3], &sweep_angle)) {
        rb_raise(rb_eTypeError, "The argument of the sweep angle should be Integer or Float.");
    }
    if (argc == 7 && !gdip_arg_to_single(argv[5], &start_angle)) {
        rb_raise(rb_eTypeError, "The argument of the start angle should be Integer or Float.");
    }
    if (argc == 7 && !gdip_arg_to_single(argv[6], &sweep_angle)) {
        rb_raise(rb_eTypeError, "The argument of the sweep angle should be Integer or Float.");
    }

    Status status = Ok;
    if (argc == 4) {
        if (_KIND_OF(argv[1], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[1]);
            status = g->DrawArc(pen, *rect, start_angle, sweep_angle);
        }
        else if (_KIND_OF(argv[1], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[1]);
            status = g->DrawArc(pen, *rect, start_angle, sweep_angle);
        }
        else {
            rb_raise(rb_eTypeError, "The second argument should be Rectangle or RectangleF");
        }
    }
    else if (argc == 7) {
        if (Integer_p(argv[1], argv[2], argv[3], argv[4])) {
            status = g->DrawArc(pen, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]), start_angle, sweep_angle);
        }
        else if (Float_p(argv[1], argv[2], argv[3], argv[4])) {
            status = g->DrawArc(pen, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]), start_angle, sweep_angle);
        }
        else {
            rb_raise(rb_eTypeError, "The arguments representing the position and size of the rectangle should be Integer or Float.");
        }
    }
    Check_Status(status);

    return self;
}


/**
 * @example
 *   bmp.draw {|g|
 *     points = []
 *     points << Point.new(30, 30)
 *     points << Point.new(130, 30)
 *     points << Point.new(80, 130)
 *     points << Point.new(180, 130)
 *     g.DrawBezier(pen, *points)
 *   }
 * @overload DrawBezier(pen, point1, point2, point3, point4)
 *   @param pen [Pen]
 *   @param point1 [Point or PointF]
 *   @param point2 [Point or PointF]
 *   @param point3 [Point or PointF]
 *   @param point4 [Point or PointF]
 * @overload DrawBezier(pen, x1, y1, x2, y2, x3, y3, x4, y4)
 *   @param pen [Pen]
 *   @param x1 [Integer or Float]
 *   @param y1 [Integer or Float]
 *   @param x2 [Integer or Float]
 *   @param y2 [Integer or Float]
 *   @param x3 [Integer or Float]
 *   @param y3 [Integer or Float]
 *   @param x4 [Integer or Float]
 *   @param y4 [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_graphics_draw_bezier(int argc, VALUE *argv, VALUE self)
{
    if (argc != 5 && argc != 9) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 5 or 9)", argc);
    }

    if (!_KIND_OF(argv[0], &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(argv[0]);
    Check_NULL(pen, "The pen object does not exist.");

    Status status = Ok;
    if (argc == 5) {
        if (Typeddata_p(4, &argv[1], &tPoint)) {
            Point *po1 = Data_Ptr<Point *>(argv[1]);
            Point *po2 = Data_Ptr<Point *>(argv[2]);
            Point *po3 = Data_Ptr<Point *>(argv[3]);
            Point *po4 = Data_Ptr<Point *>(argv[4]);
            status = g->DrawBezier(pen, *po1, *po2, *po3, *po4);
        }
        else if (Typeddata_p(4, &argv[1], &tPointF)) {
            PointF *po1 = Data_Ptr<PointF *>(argv[1]);
            PointF *po2 = Data_Ptr<PointF *>(argv[2]);
            PointF *po3 = Data_Ptr<PointF *>(argv[3]);
            PointF *po4 = Data_Ptr<PointF *>(argv[4]);
            status = g->DrawBezier(pen, *po1, *po2, *po3, *po4);
        }
        else {
            rb_raise(rb_eTypeError, "The argument representing points should be Point or PointF.");
        }
    }
    else if (argc == 9) {
        if (Integer_p(8, &argv[1])) {
            status = g->DrawBezier(pen, 
                RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]), 
                RB_NUM2INT(argv[5]), RB_NUM2INT(argv[6]), RB_NUM2INT(argv[7]), RB_NUM2INT(argv[8]));
        }
        else if (Float_p(8, &argv[1])) {
            status = g->DrawBezier(pen, 
                NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]), 
                NUM2SINGLE(argv[5]), NUM2SINGLE(argv[6]), NUM2SINGLE(argv[7]), NUM2SINGLE(argv[8]));
        }
        else {
            rb_raise(rb_eTypeError, "The argument representing the coordinates of points should be Integer or Float.");
        }
    }
    Check_Status(status);

    return self;
}

/**
 * @overload DrawBeziers(pen, points)
 *   @param pen [Pen]
 *   @param points [Array<Point or PointF>] The number of points should be 1 + 3n (n >= 1: 4, 7, 10, 13, ...).
 *   @return [self]
 * @example
 *   bmp.draw {|g|
 *     points = []
 *     points << Point.new(1, 2)
 *     points << Point.new(0, 2)
 *     points << Point.new(0, 1)
 *     points << Point.new(1, 1)
 *     points << Point.new(1, 0)
 *     points << Point.new(2, 0)
 *     points << Point.new(2, 1)
 *     points << Point.new(3, 1)
 *     points << Point.new(3, 2)
 *     points << Point.new(2, 2)
 *     points << Point.new(2, 3)
 *     points << Point.new(1, 3)
 *     points << Point.new(1, 2)
 *     points.each {|po| po.x = 50 + 100 * po.x; po.y = 50 + 100 * po.y }
 *     g.DrawBeziers(pen, points)
 *   }
 */
static VALUE
gdip_graphics_draw_beziers(VALUE self, VALUE v_pen, VALUE ary)
{
    if (!_KIND_OF(v_pen, &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }
    if (!_RB_ARRAY_P(ary) || RARRAY_LEN(ary) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(v_pen);
    Check_NULL(pen, "The pen object does not exist.");
    VALUE first = rb_ary_entry(ary, 0);

    if (_KIND_OF(first, &tPoint)) {
        int count;
        Point *points = alloc_array_of<Point, &tPoint>(ary, count);
        if ((count - 1) % 3 == 0) {
            Status status = g->DrawBeziers(pen, points, count);
            ruby_xfree(points);
            Check_Status(status);
        }
        else {
            ruby_xfree(points);
            rb_raise(rb_eArgError, "wrong number of elements of the array (%d for 4, 7, 10, 13, ...)", count);
        }
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count;
        PointF *points = alloc_array_of<PointF, &tPointF>(ary, count);
        if ((count - 1) % 3 == 0) {
            Status status = g->DrawBeziers(pen, points, count);
            ruby_xfree(points);
            Check_Status(status);
        }
        else {
            ruby_xfree(points);
            rb_raise(rb_eArgError, "wrong number of elements of the array (%d for 4, 7, 10, 13, ...)", count);
        }
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }

    return self;
}

/**
 * @example
 *   bmp.draw {|g|
 *     g.DrawPie(pen, rect, -30, 60)
 *     g.DrawPie(pen, 20, 20, 160, 160, 90.0, 180.0)
 *     g.DrawPie(pen1, 20.0, 220.0, 160.0, 160.0, 90.0, 180.0)
 *   }
 * @overload DrawPie(pen, rect, start_angle, sweep_angle)
 *   @param pen [Pen]
 *   @param rect [Rectangle or RectangleF]
 *   @param start_angle [Integer or Float]
 *   @param sweep_angle [Integer or Float]
 * @overload DrawPie(pen, x, y, width, height, start_angle, sweep_angle)
 *   @param pen [Pen]
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 *   @param start_angle [Integer or Float]
 *   @param sweep_angle [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_graphics_draw_pie(int argc, VALUE *argv, VALUE self)
{
    if (argc != 4 && argc != 7) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 4 or 7)", argc);
    }
    if (!_KIND_OF(argv[0], &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(argv[0]);
    Check_NULL(pen, "The pen object does not exist.");

    float start_angle;
    float sweep_angle;
    if (argc == 4 && !gdip_arg_to_single(argv[2], &start_angle)) {
        rb_raise(rb_eTypeError, "The argument of the start angle should be Integer or Float.");
    }
    if (argc == 4 && !gdip_arg_to_single(argv[3], &sweep_angle)) {
        rb_raise(rb_eTypeError, "The argument of the sweep angle should be Integer or Float.");
    }
    if (argc == 7 && !gdip_arg_to_single(argv[5], &start_angle)) {
        rb_raise(rb_eTypeError, "The argument of the start angle should be Integer or Float.");
    }
    if (argc == 7 && !gdip_arg_to_single(argv[6], &sweep_angle)) {
        rb_raise(rb_eTypeError, "The argument of the sweep angle should be Integer or Float.");
    }

    Status status = Ok;
    if (argc == 4) {
        if (_KIND_OF(argv[1], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[1]);
            status = g->DrawPie(pen, *rect, start_angle, sweep_angle);
        }
        else if (_KIND_OF(argv[1], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[1]);
            status = g->DrawPie(pen, *rect, start_angle, sweep_angle);
        }
        else {
            rb_raise(rb_eTypeError, "The second argument should be Rectangle or RectangleF");
        }
    }
    else if (argc == 7) {
        if (Integer_p(argv[1], argv[2], argv[3], argv[4])) {
            status = g->DrawPie(pen, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]), start_angle, sweep_angle);
        }
        else if (Float_p(argv[1], argv[2], argv[3], argv[4])) {
            status = g->DrawPie(pen, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]), start_angle, sweep_angle);
        }
        else {
            rb_raise(rb_eTypeError, "The arguments representing the position and size of the rectangle should be Integer or Float.");
        }
    }
    Check_Status(status);

    return self;
}

/**
 * @example
 *   bmp.draw {|g|
 *     g.FillPie(pen, rect, -30, 60)
 *     g.FillPie(pen, 20, 20, 160, 160, 90.0, 180.0)
 *     g.FillPie(pen1, 20.0, 220.0, 160.0, 160.0, 90.0, 180.0)
 *   }
 * @overload FillPie(brush, rect, start_angle, sweep_angle)
 *   @param brush [Brush]
 *   @param rect [Rectangle or RectangleF]
 *   @param start_angle [Integer or Float]
 *   @param sweep_angle [Integer or Float]
 * @overload FillPie(pen, x, y, width, height, start_angle, sweep_angle)
 *   @param brush [Brush]
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 *   @param start_angle [Integer or Float]
 *   @param sweep_angle [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_graphics_fill_pie(int argc, VALUE *argv, VALUE self)
{
    if (argc != 4 && argc != 7) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 4 or 7)", argc);
    }
    if (!_KIND_OF(argv[0], &tBrush)) {
        rb_raise(rb_eTypeError, "The first argument should be Brush.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Brush *brush = Data_Ptr<Brush *>(argv[0]);
    Check_NULL(brush, "The brush object does not exist.");

    float start_angle;
    float sweep_angle;
    if (argc == 4 && !gdip_arg_to_single(argv[2], &start_angle)) {
        rb_raise(rb_eTypeError, "The argument of the start angle should be Integer or Float.");
    }
    if (argc == 4 && !gdip_arg_to_single(argv[3], &sweep_angle)) {
        rb_raise(rb_eTypeError, "The argument of the sweep angle should be Integer or Float.");
    }
    if (argc == 7 && !gdip_arg_to_single(argv[5], &start_angle)) {
        rb_raise(rb_eTypeError, "The argument of the start angle should be Integer or Float.");
    }
    if (argc == 7 && !gdip_arg_to_single(argv[6], &sweep_angle)) {
        rb_raise(rb_eTypeError, "The argument of the sweep angle should be Integer or Float.");
    }

    Status status = Ok;
    if (argc == 4) {
        if (_KIND_OF(argv[1], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[1]);
            status = g->FillPie(brush, *rect, start_angle, sweep_angle);
        }
        else if (_KIND_OF(argv[1], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[1]);
            status = g->FillPie(brush, *rect, start_angle, sweep_angle);
        }
        else {
            rb_raise(rb_eTypeError, "The second argument should be Rectangle or RectangleF");
        }
    }
    else if (argc == 7) {
        if (Integer_p(argv[1], argv[2], argv[3], argv[4])) {
            status = g->FillPie(brush, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]), start_angle, sweep_angle);
        }
        else if (Float_p(argv[1], argv[2], argv[3], argv[4])) {
            status = g->FillPie(brush, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]), start_angle, sweep_angle);
        }
        else {
            rb_raise(rb_eTypeError, "The arguments representing the position and size of the rectangle should be Integer or Float.");
        }
    }
    Check_Status(status);

    return self;
}

/**
 * @overload DrawPolygon(pen, points)
 *   @param pen [Pen]
 *   @param points [Array<Point or PointF>]
 *   @return [self]
 * @example
 *   bmp.draw {|g|
 *     points = []
 *     30.times {
 *       points << Point.new(rand(400), rand(400))
 *     }
 *     g.DrawPolygon(pen, points)
 *   }
 */
static VALUE
gdip_graphics_draw_polygon(VALUE self, VALUE v_pen, VALUE ary)
{
    if (!_KIND_OF(v_pen, &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }
    if (!_RB_ARRAY_P(ary) && RARRAY_LEN(ary) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(v_pen);
    Check_NULL(pen, "The pen object does not exist.");

    VALUE first = rb_ary_entry(ary, 0);
    if (_KIND_OF(first, &tPoint)) {
        int count;
        Point *points = alloc_array_of<Point, &tPoint>(ary, count);
        Status status = g->DrawPolygon(pen, points, count);
        ruby_xfree(points);
        Check_Status(status);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count;
        PointF *points = alloc_array_of<PointF, &tPointF>(ary, count);
        Status status = g->DrawPolygon(pen, points, count);
        ruby_xfree(points);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    
    return self;
}

/**
 * @overload FillPolygon(brush, points, fillmode = FillMode.Alternate)
 *   @param brush [Brush]
 *   @param points [Array<Point or PointF>]
 *   @param fillmode [FillMode]
 *   @return [self]
 * @example
 *   bmp.draw {|g|
 *     points = []
 *     30.times {
 *       points << Point.new(rand(400), rand(400))
 *     }
 *     g.FillPolygon(brush, points)
 *   }
 */
static VALUE
gdip_graphics_fill_polygon(int argc, VALUE *argv, VALUE self)
{
    VALUE v_brush;
    VALUE v_points;
    VALUE v_fillmode = Qnil;
    rb_scan_args(argc, argv, "21", &v_brush, &v_points, &v_fillmode);
    if (!_KIND_OF(v_brush, &tBrush)) {
        rb_raise(rb_eTypeError, "The first argument should be Brush.");
    }
    if (!_RB_ARRAY_P(v_points) || RARRAY_LEN(v_points) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    FillMode fillmode = FillModeAlternate;
    if (!RB_NIL_P(v_fillmode)) {
        gdip_arg_to_enumint(cFillMode, v_fillmode, (int*)&fillmode, "The argument should be FillMode.", ArgOptionAcceptInt);
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Brush *brush = Data_Ptr<Brush *>(v_brush);
    Check_NULL(brush, "The brush object does not exist.");
    VALUE first = rb_ary_entry(v_points, 0);

    Status status = Ok;
    if (_KIND_OF(first, &tPoint)) {
        int count;
        Point *points = alloc_array_of<Point, &tPoint>(v_points, count);
        if (!RB_NIL_P(v_fillmode)) {
            status = g->FillPolygon(brush, points, count, fillmode);
        }
        else {
            status = g->FillPolygon(brush, points, count);
        }
        ruby_xfree(points);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count;
        PointF *points = alloc_array_of<PointF, &tPointF>(v_points, count);
        if (!RB_NIL_P(v_fillmode)) {
            status = g->FillPolygon(brush, points, count, fillmode);
        }
        else {
            status = g->FillPolygon(brush, points, count);
        }
        ruby_xfree(points);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    Check_Status(status);

    return self;
}

static Status
gdip_graphics_draw_string_at_point(Graphics *g, VALUE wstr, Font *font, Brush *brush, PointF& point, VALUE v_format)
{
    Status status = Ok;
    if (_KIND_OF(v_format, &tStringFormat)) {
        StringFormat *format = Data_Ptr<StringFormat *>(v_format);
        Check_NULL(format, "The StringFormat object does not exist.");
        status = g->DrawString(RString_Ptr<WCHAR *>(wstr), -1, font, point, format, brush);
    }
    else if (RB_NIL_P(v_format)) {
        status = g->DrawString(RString_Ptr<WCHAR *>(wstr), -1, font, point, brush);
    }
    else {
        rb_raise(rb_eTypeError, "The last argument should be StringFormat.");
    }
    return status;
}

static Status
gdip_graphics_draw_string_at_rect(Graphics *g, VALUE wstr, Font *font, Brush *brush, RectF& rect, VALUE v_format)
{
    Status status = Ok;
    if (_KIND_OF(v_format, &tStringFormat)) {
        StringFormat *format = Data_Ptr<StringFormat *>(v_format);
        Check_NULL(format, "The StringFormat object does not exist.");
        status = g->DrawString(RString_Ptr<WCHAR *>(wstr), -1, font, rect, format, brush);
    }
    else if (RB_NIL_P(v_format)) {
        status = g->DrawString(RString_Ptr<WCHAR *>(wstr), -1, font, rect, NULL, brush);
    }
    else {
        rb_raise(rb_eTypeError, "The last argument should be StringFormat.");
    }
    return status;
}

/**
 * @return [self]
 * @overload DrawString(str, font, brush, x, y, format=nil)
 *   @param str [String]
 *   @param font [Font]
 *   @param brush [Brush]
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param format [StringFormat]
 * @overload DrawString(str, font, brush, point, format=nil)
 *   @param str [String]
 *   @param font [Font]
 *   @param brush [Brush]
 *   @param point [Point or PointF]
 *   @param format [StringFormat]
 * @overload DrawString(str, font, brush, rect, format=nil)
 *   @param str [String]
 *   @param font [Font]
 *   @param brush [Brush]
 *   @param rect [Rectangle or RectangleF]
 *   @param format [StringFormat]
 */
static VALUE
gdip_graphics_draw_string(int argc, VALUE *argv, VALUE self)
{
    if (argc < 4 || 6 < argc) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 4..6)", argc);
    }
    if (!_RB_STRING_P(argv[0])) {
        rb_raise(rb_eTypeError, "The first argument should be String.");
    }
    if (!_KIND_OF(argv[1], &tFont)) {
        rb_raise(rb_eTypeError, "The second argument should be Font.");
    }
    if (!_KIND_OF(argv[2], &tBrush)) {
        rb_raise(rb_eTypeError, "The third argument should be Brush.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    VALUE wstr = util_utf16_str_new(argv[0]);
    Font *font = Data_Ptr<Font *>(argv[1]);
    Brush *brush = Data_Ptr<Brush *>(argv[2]);
    Check_NULL(font, "The Font object does not exist.");
    Check_NULL(font, "The Brush object does not exist.");

    Status status = Ok;
    if (argc >= 5 && Integer_p(argv[3]) && Integer_p(argv[4])) {
        PointF point(static_cast<float>(RB_NUM2INT(argv[3])), static_cast<float>(RB_NUM2INT(argv[4])));
        status = gdip_graphics_draw_string_at_point(g, wstr, font, brush, point, argc == 6 ? argv[5] : Qnil);
    }
    else if (argc >= 5 && Float_p(argv[3]) && Float_p(argv[4])) {
        PointF point(NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]));
        status = gdip_graphics_draw_string_at_point(g, wstr, font, brush, point, argc == 6 ? argv[5] : Qnil);
    }
    else if (_KIND_OF(argv[3], &tPointF)) {
        PointF *point = Data_Ptr<PointF *>(argv[3]);
        status = gdip_graphics_draw_string_at_point(g, wstr, font, brush, *point, argc == 5 ? argv[4] : Qnil);
    }
    else if (_KIND_OF(argv[3], &tPoint)) {
        Point *point_arg = Data_Ptr<Point *>(argv[3]);
        PointF point(static_cast<float>(point_arg->X), static_cast<float>(point_arg->Y));
        status = gdip_graphics_draw_string_at_point(g, wstr, font, brush, point, argc == 5 ? argv[4] : Qnil);
    }
    else if (_KIND_OF(argv[3], &tRectangleF)) {
        RectF *rect = Data_Ptr<RectF *>(argv[3]);
        status = gdip_graphics_draw_string_at_rect(g, wstr, font, brush, *rect, argc == 5 ? argv[4] : Qnil);
    }
    else if (_KIND_OF(argv[3], &tRectangle)) {
        Rect *rect_arg = Data_Ptr<Rect *>(argv[3]);
        RectF rect;
        rect.X = static_cast<float>(rect_arg->X);
        rect.Y = static_cast<float>(rect_arg->Y);
        rect.Width = static_cast<float>(rect_arg->Width);
        rect.Height = static_cast<float>(rect_arg->Height);
        status = gdip_graphics_draw_string_at_rect(g, wstr, font, brush, rect, argc == 5 ? argv[4] : Qnil);
    }
    else {
        rb_raise(rb_eTypeError, "Argument types do not match.");
    }
    Check_Status(status);

    return self;
}

/**
 * @overload DrawPath(pen, path)
 *   @param pen [Pen]
 *   @param path [GraphicsPath]
 *   @return [self]
 */
static VALUE
gdip_graphics_draw_path(VALUE self, VALUE v_pen, VALUE v_path)
{
    if (!_KIND_OF(v_pen, &tPen)) {
        rb_raise(rb_eTypeError, "The first argument should be Pen.");
    }
    if (!_KIND_OF(v_path, &tGraphicsPath)) {
        rb_raise(rb_eTypeError, "The secont argument should be GraphicsPath.");
    }
    
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    Pen *pen = Data_Ptr<Pen *>(v_pen);
    Check_NULL(pen, "This Pen object does not exist.");
    GraphicsPath *path = Data_Ptr<GraphicsPath *>(v_path);
    Check_NULL(path, "This GraphicsPath object does not exist.");
    
    Status status = g->DrawPath(pen, path);
    Check_Status(status);

    return self;
}

/**
 * @overload FillPath(brush, path)
 *   @param brush [Brush]
 *   @param path [GraphicsPath]
 *   @return [self]
 */
static VALUE
gdip_graphics_fill_path(VALUE self, VALUE v_brush, VALUE v_path)
{
    if (!_KIND_OF(v_brush, &tBrush)) {
        rb_raise(rb_eTypeError, "The first argument should be Brush.");
    }
    if (!_KIND_OF(v_path, &tGraphicsPath)) {
        rb_raise(rb_eTypeError, "The secont argument should be GraphicsPath.");
    }
    
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    Brush *brush = Data_Ptr<Brush *>(v_brush);
    Check_NULL(brush, "This Pen object does not exist.");
    GraphicsPath *path = Data_Ptr<GraphicsPath *>(v_path);
    Check_NULL(path, "This GraphicsPath object does not exist.");
    
    Status status = g->FillPath(brush, path);
    Check_Status(status);

    return self;
}

void
Init_graphics()
{
    cGraphics = rb_define_class_under(mGdiplus, "Graphics", cGpObject);
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

    rb_define_method(cGraphics, "Clear", RUBY_METHOD_FUNC(gdip_graphics_clear), 1);
    rb_define_alias(cGraphics, "clear", "Clear");
    
    rb_define_method(cGraphics, "DrawRectangle", RUBY_METHOD_FUNC(gdip_graphics_draw_rectangle), -1);
    rb_define_alias(cGraphics, "draw_rectangle", "DrawRectangle");
    rb_define_method(cGraphics, "DrawRectangles", RUBY_METHOD_FUNC(gdip_graphics_draw_rectangles), 2);
    rb_define_alias(cGraphics, "draw_rectangles", "DrawRectangles");
    rb_define_method(cGraphics, "FillRectangle", RUBY_METHOD_FUNC(gdip_graphics_fill_rectangle), -1);
    rb_define_alias(cGraphics, "fill_rectangle", "FillRectangle");
    rb_define_method(cGraphics, "FillRectangles", RUBY_METHOD_FUNC(gdip_graphics_fill_rectangles), 2);
    rb_define_alias(cGraphics, "fill_rectangles", "FillRectangles");
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
    rb_define_method(cGraphics, "DrawClosedCurve", RUBY_METHOD_FUNC(gdip_graphics_draw_closed_curve), -1);
    rb_define_alias(cGraphics, "draw_closed_curve", "DrawClosedCurve");
    rb_define_method(cGraphics, "FillClosedCurve", RUBY_METHOD_FUNC(gdip_graphics_fill_closed_curve), -1);
    rb_define_alias(cGraphics, "fill_closed_curve", "FillClosedCurve");
    rb_define_method(cGraphics, "DrawArc", RUBY_METHOD_FUNC(gdip_graphics_draw_arc), -1);
    rb_define_alias(cGraphics, "draw_arc", "DrawArc");
    rb_define_method(cGraphics, "DrawBezier", RUBY_METHOD_FUNC(gdip_graphics_draw_bezier), -1);
    rb_define_alias(cGraphics, "draw_bezier", "DrawBezier");
    rb_define_method(cGraphics, "DrawBeziers", RUBY_METHOD_FUNC(gdip_graphics_draw_beziers), 2);
    rb_define_alias(cGraphics, "draw_beziers", "DrawBeziers");
    rb_define_method(cGraphics, "DrawPie", RUBY_METHOD_FUNC(gdip_graphics_draw_pie), -1);
    rb_define_alias(cGraphics, "draw_pie", "DrawPie");
    rb_define_method(cGraphics, "FillPie", RUBY_METHOD_FUNC(gdip_graphics_fill_pie), -1);
    rb_define_alias(cGraphics, "fill_pie", "FillPie");
    rb_define_method(cGraphics, "DrawPolygon", RUBY_METHOD_FUNC(gdip_graphics_draw_polygon), 2);
    rb_define_alias(cGraphics, "draw_polygon", "DrawPolygon");
    rb_define_method(cGraphics, "FillPolygon", RUBY_METHOD_FUNC(gdip_graphics_fill_polygon), -1);
    rb_define_alias(cGraphics, "fill_polygon", "FillPolygon");
    rb_define_method(cGraphics, "DrawString", RUBY_METHOD_FUNC(gdip_graphics_draw_string), -1);
    rb_define_alias(cGraphics, "draw_string", "DrawString");
    rb_define_method(cGraphics, "DrawPath", RUBY_METHOD_FUNC(gdip_graphics_draw_path), 2);
    rb_define_alias(cGraphics, "draw_path", "DrawPath");
    rb_define_method(cGraphics, "FillPath", RUBY_METHOD_FUNC(gdip_graphics_fill_path), 2);
    rb_define_alias(cGraphics, "fill_path", "FillPath");
    
}