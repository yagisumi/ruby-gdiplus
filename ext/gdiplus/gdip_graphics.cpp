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
    return gdip_enumint_create(cInterpolationMode, g->GetInterpolationMode());
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
 * Gets or sets the clipping Region of this Graphics.
 * @return [Region]
 * @see #SetClip
 */
static VALUE
gdip_graphics_get_clip(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");

    VALUE r = typeddata_alloc_null<&tRegion>(cRegion);
    Region *region = new Region();
    _DATA_PTR(r) = gdip_obj_create(region);

    Status status = g->GetClip(region);
    Check_Status(status);

    return r;
}

static VALUE
gdip_graphics_set_clip(VALUE self, VALUE v_region)
{
    if (!_KIND_OF(v_region, &tRegion)) {
        rb_raise(rb_eTypeError, "The argument should be Region.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Region *region = Data_Ptr<Region *>(v_region);
    Check_NULL(region, "The Region object of argument does not exist.");

    Status status = g->SetClip(region);
    Check_Status(status);

    return self;
}

/**
 * Gets or sets the matrix of this Graphics.
 * @return [Matrix]
 */
static VALUE
gdip_graphics_get_transform(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");

    VALUE r = typeddata_alloc_null<&tMatrix>(cMatrix);
    Matrix *matrix = new Matrix();
    _DATA_PTR(r) = gdip_obj_create(matrix);

    Status status = g->GetTransform(matrix);
    Check_Status(status);

    return r;
}

static VALUE
gdip_graphics_set_transform(VALUE self, VALUE v_matrix)
{
    if (!_KIND_OF(v_matrix, &tMatrix)) {
        rb_raise(rb_eTypeError, "The argument should be Matrix.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    Matrix *matrix = Data_Ptr<Matrix *>(v_matrix);
    Check_NULL(matrix, "The Matrix object of argument does not exist.");

    Status status = g->SetTransform(matrix);
    Check_Status(status);

    return self;
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
    gdip_arg_to_color(color_v, &color, "The first argument should be Color.");
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
 *     g.DrawArc(pen, 20.0, 220.0, 160.0, 160.0, 90.0, 180.0)
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
    Check_NULL(brush, "The Brush object of argument does not exist.");
    GraphicsPath *path = Data_Ptr<GraphicsPath *>(v_path);
    Check_NULL(path, "This GraphicsPath object does not exist.");
    
    Status status = g->FillPath(brush, path);
    Check_Status(status);

    return self;
}

/**
 * @overload FillRegion(brush, region)
 *   @param brush [Brush]
 *   @param region [Region]
 *   @return [self]
 */
static VALUE
gdip_graphics_fill_region(VALUE self, VALUE v_brush, VALUE v_region)
{
    if (!_KIND_OF(v_brush, &tBrush)) {
        rb_raise(rb_eTypeError, "The first argument should be Brush.");
    }
    if (!_KIND_OF(v_region, &tRegion)) {
        rb_raise(rb_eTypeError, "The secont argument should be Region.");
    }
    
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    Brush *brush = Data_Ptr<Brush *>(v_brush);
    Check_NULL(brush, "The Brush object of argument does not exist.");
    Region *region = Data_Ptr<Region *>(v_region);
    Check_NULL(region, "The Region object of argument does not exist.");
    
    Status status = g->FillRegion(brush, region);
    Check_Status(status);

    return self;
}

/**
 * @overload DrawImagePoint(image, point)
 *   @param image [Image]
 *   @param point [Point or PointF] The drawing destination.
 * @overload DrawImagePoint(image, x, y)
 *   @param image [Image]
 *   @param x [Integer or Float] The drawing destination.
 *   @param y [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_graphics_draw_image_point(int argc, VALUE *argv, VALUE self)
{
    if (argc != 2 && argc != 3) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2..3)", argc);
    }

    if (!_KIND_OF(argv[0], &tImage)) {
        rb_raise(rb_eTypeError, "The first argument should be a subclass of Image.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    Image *image = Data_Ptr<Image *>(argv[0]);
    Check_NULL(image, "The Image object of the argument does not exist.");

    Status status = Ok;
    if (argc == 2) {
        if (_KIND_OF(argv[1], &tPoint)) {
            Point *point = Data_Ptr<Point *>(argv[1]);
            status = g->DrawImage(image, *point);
        }
        else if (_KIND_OF(argv[1], &tPointF)) {
            PointF *point = Data_Ptr<PointF *>(argv[1]);
            status = g->DrawImage(image, *point);
        }
        else {
            rb_raise(rb_eTypeError, "The second argument should be Point or PointF.");
        }
    }
    else if (argc == 3) {
        if (Integer_p(argv[1], argv[2])) {
            status = g->DrawImage(image, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]));
        }
        else if (Float_p(argv[1], argv[2])) {
            status = g->DrawImage(image, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]));
        }
        else {
            rb_raise(rb_eTypeError, "Invalid types of arguments representing coordinates.");
        }
    }
    Check_Status(status);

    return self;
}

/**
 * @overload DrawImageRect(image, rect)
 *   @param image [Image]
 *   @param rect [Rectangle or RectangleF] The drawing destination.
 * @overload DrawImageRect(image, x, y, w, h)
 *   @param image [Image]
 *   @param x [Integer or Float] The drawing destination.
 *   @param y [Integer or Float]
 *   @param w [Integer or Float]
 *   @param h [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_graphics_draw_image_rect(int argc, VALUE *argv, VALUE self)
{
    if (argc != 2 && argc != 5) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2 or 5)", argc);
    }

    if (!_KIND_OF(argv[0], &tImage)) {
        rb_raise(rb_eTypeError, "The first argument should be a subclass of Image.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    Image *image = Data_Ptr<Image *>(argv[0]);
    Check_NULL(image, "The Image object of the argument does not exist.");

    Status status = Ok;
    if (argc == 2) {
        if (_KIND_OF(argv[1], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[1]);
            status = g->DrawImage(image, *rect);
        }
        else if (_KIND_OF(argv[1], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[1]);
            status = g->DrawImage(image, *rect);
        }
        else {
            rb_raise(rb_eTypeError, "The second argument should be Rectangle or RectangleF.");
        }
    }
    else if (argc == 5) {
        if (Integer_p(4, &argv[1])) {
            status = g->DrawImage(image, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]));
        }
        else if (Float_p(4, &argv[1])) {
            status = g->DrawImage(image, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]));
        }
        else {
            rb_raise(rb_eTypeError, "Invalid types of arguments representing coordinates.");
        }
    }
    Check_Status(status);

    return self;
}

/**
 * @overload DrawImagePoints(image, points)
 *   @param image [Image]
 *   @param points [Array<Point or PointF>] The coordinates of the upper-left corner, upper-right corner, and lower-left corner of the parallelogram of the drawing destination.
 *
 */
static VALUE
gdip_graphics_draw_image_points(VALUE self, VALUE v_image, VALUE v_points)
{
    if (!_KIND_OF(v_image, &tImage)) {
        rb_raise(rb_eTypeError, "The first argument should be a subclass of Image.");
    }

    if (!_RB_ARRAY_P(v_points) || RARRAY_LEN(v_points) <= 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    Image *image = Data_Ptr<Image *>(v_image);
    Check_NULL(image, "The Image object of the argument does not exist.");

    VALUE first = rb_ary_entry(v_points, 0);

    Status status = Ok;
    if (_KIND_OF(first, &tPoint)) {
        int count = 0;
        Point *points = alloc_array_of<Point, &tPoint>(v_points, count);
        if (count != 3) {
            ruby_xfree(points);
            rb_raise(rb_eArgError, "The number of points should be 3.");
        }
        status = g->DrawImage(image, points, count);
        ruby_xfree(points);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count = 0;
        PointF *points = alloc_array_of<PointF, &tPointF>(v_points, count);
        if (count != 3) {
            ruby_xfree(points);
            rb_raise(rb_eArgError, "The number of points should be 3.");
        }
        status = g->DrawImage(image, points, count);
        ruby_xfree(points);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    Check_Status(status);

    return self;
}

/**
 * @overload DrawImagePointRect(image, dest_point, src_rect, src_unit=GraphicsUnit.Pixel)
 *   @param image [Image]
 *   @param dest_point [Point or PointF]
 *   @param src_rect [Rectangle or RectangleF]
 *   @param src_unit [GraphicsUnit]
 * @overload DrawimagePointRect(image, dx, dy, src_rect, src_unit=GraphicsUnit.Pixel)
 *   @param image [Image]
 *   @param dx [Integer or Float]
 *   @param dy [Integer or Float]
 *   @param src_rect [Rectangle or RectangleF]
 *   @param src_unit [GraphicsUnit]
 * @overload DrawImagePointRect(image, dest_point, sx, sy, sw, sh, src_unit=GraphicsUnit.Pixel)
 *   @param image [Image]
 *   @param dest_point [Point or PointF]
 *   @param sx [Integer or Float]
 *   @param sy [Integer or Float]
 *   @param sw [Integer or Float]
 *   @param sh [Integer or Float]
 *   @param src_unit [GraphicsUnit]
 * @overload DrawImagePointRect(image, dx, dy, sx, sy, sw, sh, src_unit=GraphicsUnit.Pixel)
 *   @param image [Image]
 *   @param dx [Integer or Float]
 *   @param dy [Integer or Float]
 *   @param sx [Integer or Float]
 *   @param sy [Integer or Float]
 *   @param sw [Integer or Float]
 *   @param sh [Integer or Float]
 *   @param src_unit [GraphicsUnit]
 * @return [self]
 */
static VALUE
gdip_graphics_draw_image_point_rect(int argc, VALUE *argv, VALUE self)
{
    if (argc < 3 || 8 < argc) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 3..8)", argc);
    }

    if (!_KIND_OF(argv[0], &tImage)) {
        rb_raise(rb_eTypeError, "The first argument should be a subclass of Image.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    Image *image = Data_Ptr<Image *>(argv[0]);
    Check_NULL(image, "The Image object of the argument does not exist.");
    Unit unit = UnitPixel;

    Status status = Ok;
    if (argc == 3) {
        if (_KIND_OF(argv[1], &tPoint) && _KIND_OF(argv[2], &tRectangle)) {
            Point *point = Data_Ptr<Point *>(argv[1]);
            Rect *rect = Data_Ptr<Rect *>(argv[2]);
            status = g->DrawImage(image, point->X, point->Y, rect->X, rect->Y, rect->Width, rect->Height, unit);
        }
        else if (_KIND_OF(argv[1], &tPointF) && _KIND_OF(argv[2], &tRectangleF)) {
            PointF *point = Data_Ptr<PointF *>(argv[1]);
            RectF *rect = Data_Ptr<RectF *>(argv[2]);
            status = g->DrawImage(image, point->X, point->Y, rect->X, rect->Y, rect->Width, rect->Height, unit);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc == 4) {
        if (Integer_p(argv[1], argv[2]) && _KIND_OF(argv[3], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[3]);
            status = g->DrawImage(image, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), rect->X, rect->Y, rect->Width, rect->Height, unit);
        }
        else if (Float_p(argv[1], argv[2]) && _KIND_OF(argv[3], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[3]);
            status = g->DrawImage(image, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), rect->X, rect->Y, rect->Width, rect->Height, unit);
        }
        else {
            gdip_arg_to_enumint(cGraphicsUnit, argv[3], &unit, "The last argument should be GraphicsUnit.");
            if (_KIND_OF(argv[1], &tPoint) && _KIND_OF(argv[2], &tRectangle)) {
                Point *point = Data_Ptr<Point *>(argv[1]);
                Rect *rect = Data_Ptr<Rect *>(argv[2]);
                status = g->DrawImage(image, point->X, point->Y, rect->X, rect->Y, rect->Width, rect->Height, unit);
            }
            else if (_KIND_OF(argv[1], &tPointF) && _KIND_OF(argv[2], &tRectangleF)) {
                PointF *point = Data_Ptr<PointF *>(argv[1]);
                RectF *rect = Data_Ptr<RectF *>(argv[2]);
                status = g->DrawImage(image, point->X, point->Y, rect->X, rect->Y, rect->Width, rect->Height, unit);
            }
            else {
                rb_raise(rb_eTypeError, "wrong types of arguments");
            }

        }
    }
    else if (argc == 5) {
        gdip_arg_to_enumint(cGraphicsUnit, argv[4], &unit, "The last argument should be GraphicsUnit.");
        if (Integer_p(argv[1], argv[2]) && _KIND_OF(argv[3], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[3]);
            status = g->DrawImage(image, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), rect->X, rect->Y, rect->Width, rect->Height, unit);
        }
        else if (Float_p(argv[1], argv[2]) && _KIND_OF(argv[3], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[3]);
            status = g->DrawImage(image, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), rect->X, rect->Y, rect->Width, rect->Height, unit);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc == 6) {
        if (_KIND_OF(argv[1], &tPoint) && Integer_p(4, &argv[2])) {
            Point *point = Data_Ptr<Point *>(argv[1]);
            status = g->DrawImage(image, point->X, point->Y, RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]), RB_NUM2INT(argv[5]), unit);
        }
        else if (_KIND_OF(argv[1], &tPointF) && Float_p(4, &argv[2])) {
            PointF *point = Data_Ptr<PointF *>(argv[1]);
            status = g->DrawImage(image, point->X, point->Y, NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]), NUM2SINGLE(argv[5]), unit);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc == 7) {
        if (Integer_p(6, &argv[1])) {
            status = g->DrawImage(image, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]), RB_NUM2INT(argv[5]), RB_NUM2INT(argv[6]), unit);
        }
        else if (Float_p(6, &argv[1])) {
            status = g->DrawImage(image, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]), NUM2SINGLE(argv[5]), NUM2SINGLE(argv[6]), unit);
        }
        else {
            gdip_arg_to_enumint(cGraphicsUnit, argv[6], &unit, "The last argument should be GraphicsUnit.");
            if (_KIND_OF(argv[1], &tPoint) && Integer_p(4, &argv[2])) {
                Point *point = Data_Ptr<Point *>(argv[1]);
                status = g->DrawImage(image, point->X, point->Y, RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]), RB_NUM2INT(argv[5]), unit);
            }
            else if (_KIND_OF(argv[1], &tPointF) && Float_p(4, &argv[2])) {
                PointF *point = Data_Ptr<PointF *>(argv[1]);
                status = g->DrawImage(image, point->X, point->Y, NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]), NUM2SINGLE(argv[5]), unit);
            }
            else {
                rb_raise(rb_eTypeError, "wrong types of arguments");
            }
        }
    }
    else if (argc == 8) {
        gdip_arg_to_enumint(cGraphicsUnit, argv[7], &unit, "The last argument should be GraphicsUnit.");
        if (Integer_p(6, &argv[1])) {
            status = g->DrawImage(image, RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]), RB_NUM2INT(argv[5]), RB_NUM2INT(argv[6]), unit);
        }
        else if (Float_p(6, &argv[1])) {
            status = g->DrawImage(image, NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]), NUM2SINGLE(argv[5]), NUM2SINGLE(argv[6]), unit);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    Check_Status(status);

    return self;
}

/**
 * @overload DrawImageRectRect(image, dest_rect, src_rect, src_unit=GraphicsUnit.Pixel, attributes=nil)
 *   @param image [Image]
 *   @param dest_rect [Rectangle or RectangleF]
 *   @param src_rect [Rectangle or RectangleF]
 *   @param src_unit [GraphicsUnit]
 *   @param attributes [ImageAttributes]
 * @overload DrawImageRectRect(image, dx, dy, dw, dh, src_rect, src_unit=GraphicsUnit.Pixel, attributes=nil)
 *   @param image [Image]
 *   @param dx [Integer or Float]
 *   @param dy [Integer or Float]
 *   @param dw [Integer or Float]
 *   @param dh [Integer or Float]
 *   @param src_rect [Rectangle or RectangleF]
 *   @param src_unit [GraphicsUnit]
 *   @param attributes [ImageAttributes]
 * @overload DrawImageRectRect(image, dest_rect, sx, sy, sw, sh, src_unit=GraphicsUnit.Pixel, attributes=nil)
 *   @param image [Image]
 *   @param dest_rect [Rectangle or RectangleF]
 *   @param sx [Integer or Float]
 *   @param sy [Integer or Float]
 *   @param sw [Integer or Float]
 *   @param sh [Integer or Float]
 *   @param src_unit [GraphicsUnit]
 *   @param attributes [ImageAttributes]
 * @overload DrawImageRectRect(image, dx, dy, dw, dh, sx, sy, sw, sh, src_unit=GraphicsUnit.Pixel, attributes=nil)
 *   @param image [Image]
 *   @param dx [Integer or Float]
 *   @param dy [Integer or Float]
 *   @param dw [Integer or Float]
 *   @param dh [Integer or Float]
 *   @param sx [Integer or Float]
 *   @param sy [Integer or Float]
 *   @param sw [Integer or Float]
 *   @param sh [Integer or Float]
 *   @param src_unit [GraphicsUnit]
 *   @param attributes [ImageAttributes]
 * @return [self]
 */
static VALUE
gdip_graphics_draw_image_rect_rect(int argc, VALUE *argv, VALUE self)
{
    if (argc < 3 || 11 < argc) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 3..11)", argc);
    }

    if (!_KIND_OF(argv[0], &tImage)) {
        rb_raise(rb_eTypeError, "The first argument should be a subclass of Image.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    Image *image = Data_Ptr<Image *>(argv[0]);
    Check_NULL(image, "The Image object of the argument does not exist.");
    Unit unit = UnitPixel;
    ImageAttributes *attributes = NULL;

    Status status = Ok;
    if (argc <= 8 && _KIND_OF(argv[1], &tRectangle)) {
        Rect *dest_rect = Data_Ptr<Rect *>(argv[1]);

        if (argc <= 5 && _KIND_OF(argv[2], &tRectangle)) {
            Rect *src_rect = Data_Ptr<Rect *>(argv[2]);
            if (argc >= 4) {
                gdip_arg_to_enumint(cGraphicsUnit, argv[3], &unit, "The fourth argument should be GraphicsUnit.");
            }

            if (argc == 5) {
                if (_KIND_OF(argv[4], &tImageAttributes)) {
                    attributes = Data_Ptr<ImageAttributes *>(argv[4]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            status = g->DrawImage(image, *dest_rect, src_rect->X, src_rect->Y, src_rect->Width, src_rect->Height, unit, attributes);
        }
        else if (argc >= 6 && Integer_p(4, &argv[2])) {
            if (argc >= 7) {
                gdip_arg_to_enumint(cGraphicsUnit, argv[6], &unit, "The seventh argument should be GraphicsUnit.");
            }

            if (argc == 8) {
                if (_KIND_OF(argv[7], &tImageAttributes)) {
                    attributes = Data_Ptr<ImageAttributes *>(argv[7]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            status = g->DrawImage(image, *dest_rect, RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]), RB_NUM2INT(argv[5]), unit, attributes);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc <= 8 && _KIND_OF(argv[1], &tRectangleF)) {
        RectF *dest_rect = Data_Ptr<RectF *>(argv[1]);

        if (argc <= 5 && _KIND_OF(argv[2], &tRectangleF)) {
            RectF *src_rect = Data_Ptr<RectF *>(argv[2]);
            if (argc >= 4) {
                gdip_arg_to_enumint(cGraphicsUnit, argv[3], &unit, "The fourth argument should be GraphicsUnit.");
            }

            if (argc == 5) {
                if (_KIND_OF(argv[4], &tImageAttributes)) {
                    attributes = Data_Ptr<ImageAttributes *>(argv[4]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            status = g->DrawImage(image, *dest_rect, src_rect->X, src_rect->Y, src_rect->Width, src_rect->Height, unit, attributes);
        }
        else if (argc >= 6 && Float_p(4, &argv[2])) {
            if (argc >= 7) {
                gdip_arg_to_enumint(cGraphicsUnit, argv[6], &unit, "The seventh argument should be GraphicsUnit.");
            }

            if (argc == 8) {
                if (_KIND_OF(argv[7], &tImageAttributes)) {
                    attributes = Data_Ptr<ImageAttributes *>(argv[7]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            status = g->DrawImage(image, *dest_rect, NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]), NUM2SINGLE(argv[5]), unit, attributes);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc >= 6 && Integer_p(4, &argv[1])) {
        Rect dest_rect;
        dest_rect.X = RB_NUM2INT(argv[1]);
        dest_rect.Y = RB_NUM2INT(argv[2]);
        dest_rect.Width = RB_NUM2INT(argv[3]);
        dest_rect.Height = RB_NUM2INT(argv[4]);

        if (argc <= 8 && _KIND_OF(argv[5], &tRectangle)) {
            Rect *src_rect = Data_Ptr<Rect *>(argv[5]);

            if (argc >= 7) {
                gdip_arg_to_enumint(cGraphicsUnit, argv[6], &unit, "The seventh argument should be GraphicsUnit.");
            }

            if (argc == 8) {
                if (_KIND_OF(argv[7], &tImageAttributes)) {
                    attributes = Data_Ptr<ImageAttributes *>(argv[7]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            status = g->DrawImage(image, dest_rect, src_rect->X, src_rect->Y, src_rect->Width, src_rect->Height, unit, attributes);
        }
        else if (argc >= 9 && Integer_p(4, &argv[5])) {
            if (argc >= 10) {
                gdip_arg_to_enumint(cGraphicsUnit, argv[9], &unit, "The seventh argument should be GraphicsUnit.");
            }
            else if (argc == 11) {
                if (_KIND_OF(argv[10], &tImageAttributes)) {
                    attributes = Data_Ptr<ImageAttributes *>(argv[10]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            status = g->DrawImage(image, dest_rect, RB_NUM2INT(argv[5]), RB_NUM2INT(argv[6]), RB_NUM2INT(argv[7]), RB_NUM2INT(argv[8]), unit, attributes);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc >= 6 && Float_p(4, &argv[1])) {
        RectF dest_rect;
        dest_rect.X = NUM2SINGLE(argv[1]);
        dest_rect.Y = NUM2SINGLE(argv[2]);
        dest_rect.Width = NUM2SINGLE(argv[3]);
        dest_rect.Height = NUM2SINGLE(argv[4]);

        if (argc <= 8 && _KIND_OF(argv[5], &tRectangleF)) {
            RectF *src_rect = Data_Ptr<RectF *>(argv[5]);

            if (argc >= 7) {
                gdip_arg_to_enumint(cGraphicsUnit, argv[6], &unit, "The seventh argument should be GraphicsUnit.");
            }

            if (argc == 8) {
                if (_KIND_OF(argv[7], &tImageAttributes)) {
                    attributes = Data_Ptr<ImageAttributes *>(argv[7]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            status = g->DrawImage(image, dest_rect, src_rect->X, src_rect->Y, src_rect->Width, src_rect->Height, unit, attributes);
        }
        else if (argc >= 9 && Float_p(4, &argv[5])) {
            if (argc >= 10) {
                gdip_arg_to_enumint(cGraphicsUnit, argv[9], &unit, "The seventh argument should be GraphicsUnit.");
            }
            else if (argc == 11) {
                if (_KIND_OF(argv[10], &tImageAttributes)) {
                    attributes = Data_Ptr<ImageAttributes *>(argv[10]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            status = g->DrawImage(image, dest_rect, NUM2SINGLE(argv[5]), NUM2SINGLE(argv[6]), NUM2SINGLE(argv[7]), NUM2SINGLE(argv[8]), unit, attributes);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else {
        rb_raise(rb_eTypeError, "wrong types of arguments");
    }
    Check_Status(status);

    return self;
}

/**
 * @overload DrawImagePointsRect(image, dest_points, src_rect, src_unit=GraphicsUnit.Pixel, attributes=nil)
 *   @param dest_points [Array<Point or PointF>]
 *   @param src_rect [Rectangle or RectangleF]
 *   @param src_unit [GraphicsUnit]
 *   @param attributes [ImageAttribute]
 * @overload DrawImagePointsRect(image, dest_points, sx, sy, sw, sh, src_unit=GraphicsUnit.Pixel, attributes=nil)
 *   @param dest_points [Array<Point or PointF>]
 *   @param sx [Integer or Float]
 *   @param sy [Integer or Float]
 *   @param sw [Integer or Float]
 *   @param sh [Integer or Float]
 *   @param src_unit [GraphicsUnit]
 *   @param attributes [ImageAttribute]
 * @return [self]
 */
static VALUE
gdip_graphics_draw_image_points_rect(int argc, VALUE *argv, VALUE self)
{
    if (argc < 3 || 8 < argc) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 3..11)", argc);
    }

    if (!_KIND_OF(argv[0], &tImage)) {
        rb_raise(rb_eTypeError, "The first argument should be a subclass of Image.");
    }

    if (!_RB_ARRAY_P(argv[1]) || RARRAY_LEN(argv[1]) <= 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    Image *image = Data_Ptr<Image *>(argv[0]);
    Check_NULL(image, "The Image object of the argument does not exist.");
    Unit unit = UnitPixel;
    ImageAttributes *attributes = NULL;

    VALUE first = rb_ary_entry(argv[1], 0);

    Status status = Ok;
    if (_KIND_OF(first, &tPoint)) {
        if (argc <= 5) {
            if (_KIND_OF(argv[2], &tRectangle)) {
                Rect *rect = Data_Ptr<Rect *>(argv[2]);

                if (argc >= 4) {
                    gdip_arg_to_enumint(cGraphicsUnit, argv[3], &unit, "The fourth argument should be GraphicsUnit.");
                }

                if (argc == 5) {
                    if (_KIND_OF(argv[4], &tImageAttributes)) {
                        attributes = Data_Ptr<ImageAttributes *>(argv[4]);
                    }
                    else {
                        rb_raise(rb_eTypeError, "wrong types of arguments");
                    }
                }

                int count = 0;
                Point *points = alloc_array_of<Point, &tPoint>(argv[1], count);
                if (count != 3) {
                    ruby_xfree(points);
                    rb_raise(rb_eArgError, "The number of points should be 3.");
                }

                status = g->DrawImage(image, points, count, rect->X, rect->Y, rect->Width, rect->Height, unit, attributes);
                ruby_xfree(points);
            }
            else {
                rb_raise(rb_eTypeError, "wrong types of arguments.");
            }
        }
        else if (argc >= 6) {
            if (Integer_p(4, &argv[2])) {
                if (argc >= 7) {
                    gdip_arg_to_enumint(cGraphicsUnit, argv[6], &unit, "The seventh argument should be GraphicsUnit.");
                }

                if (argc == 8) {
                    if (_KIND_OF(argv[7], &tImageAttributes)) {
                        attributes = Data_Ptr<ImageAttributes *>(argv[7]);
                    }
                    else {
                        rb_raise(rb_eTypeError, "wrong types of arguments");
                    }
                }

                int count = 0;
                Point *points = alloc_array_of<Point, &tPoint>(argv[1], count);
                if (count != 3) {
                    ruby_xfree(points);
                    rb_raise(rb_eArgError, "The number of points should be 3.");
                }

                status = g->DrawImage(image, points, count, RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), RB_NUM2INT(argv[4]), RB_NUM2INT(argv[5]), unit, attributes);
                ruby_xfree(points);
            }
            else {
                rb_raise(rb_eTypeError, "wrong types of arguments.");
            }
        }
    }
    else if (_KIND_OF(first, &tPointF)) {
        if (argc <= 5) {
            if (_KIND_OF(argv[2], &tRectangleF)) {
                RectF *rect = Data_Ptr<RectF *>(argv[2]);

                if (argc >= 4) {
                    gdip_arg_to_enumint(cGraphicsUnit, argv[3], &unit, "The fourth argument should be GraphicsUnit.");
                }

                if (argc == 5) {
                    if (_KIND_OF(argv[4], &tImageAttributes)) {
                        attributes = Data_Ptr<ImageAttributes *>(argv[4]);
                    }
                    else {
                        rb_raise(rb_eTypeError, "wrong types of arguments");
                    }
                }

                int count = 0;
                PointF *points = alloc_array_of<PointF, &tPointF>(argv[1], count);
                if (count != 3) {
                    ruby_xfree(points);
                    rb_raise(rb_eArgError, "The number of points should be 3.");
                }

                status = g->DrawImage(image, points, count, rect->X, rect->Y, rect->Width, rect->Height, unit, attributes);
                ruby_xfree(points);
            }
            else {
                rb_raise(rb_eTypeError, "wrong types of arguments.");
            }
        }
        else if (argc >= 6) {
            if (Float_p(4, &argv[2])) {
                if (argc >= 7) {
                    gdip_arg_to_enumint(cGraphicsUnit, argv[6], &unit, "The seventh argument should be GraphicsUnit.");
                }

                if (argc == 8) {
                    if (_KIND_OF(argv[7], &tImageAttributes)) {
                        attributes = Data_Ptr<ImageAttributes *>(argv[7]);
                    }
                    else {
                        rb_raise(rb_eTypeError, "wrong types of arguments");
                    }
                }

                int count = 0;
                PointF *points = alloc_array_of<PointF, &tPointF>(argv[1], count);
                if (count != 3) {
                    ruby_xfree(points);
                    rb_raise(rb_eArgError, "The number of points should be 3.");
                }

                status = g->DrawImage(image, points, count, NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), NUM2SINGLE(argv[4]), NUM2SINGLE(argv[5]), unit, attributes);
                ruby_xfree(points);
            }
            else {
                rb_raise(rb_eTypeError, "wrong types of arguments.");
            }
        }
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    Check_Status(status);

    return self;
}


/**
 * See {#DrawImagePoint}, {#DrawImageRect}, {#DrawImagePoints}, {#DrawImagePointRect}, {#DrawImageRectRect}, {#DrawImagePointsRect}
 * ##Arguments list
 * |                     |       |        dest        |         src        |              |                |
 * |---------------------|-------|:------------------:|:------------------:|--------------|----------------|
 * | DrawImagePoint      | image |    point<br>x, y   |                    |              |                |
 * | DrawImageRect       | image | rect<br>x, y, w, h |                    |              |                |
 * | DrawImagePoints     | image |       points       |                    |              |                |
 * | DrawImagePointRect  | image |    point<br>x, y   | rect<br>x, y, w, h | GraphicsUnit |                |
 * | DrawImageRectRect   | image | rect<br>x, y, w, h | rect<br>x, y, w, h | GraphicsUnit | ImageAttribute |
 * | DrawImagePointsRect | image |       points       | rect<br>x, y, w, h | GraphicsUnit | ImageAttribute |
 * @figure Graphics_DrawImage.png
 * @return [self]
 * @!endgroup
 */
static VALUE
gdip_graphics_draw_image(int argc, VALUE *argv, VALUE self)
{
    if (argc < 2 || 11 < argc) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2..11)", argc);
    }

    if (!_KIND_OF(argv[0], &tImage)) {
        rb_raise(rb_eTypeError, "The first argument should be a subclass of Image.");
    }

    if (argc == 2) {
        if (_KIND_OF(argv[1], &tPoint) || _KIND_OF(argv[1], &tPointF)) {
            gdip_graphics_draw_image_point(argc, argv, self);
        }
        else if (_KIND_OF(argv[1], &tRectangle) || _KIND_OF(argv[1], &tRectangleF)) {
            gdip_graphics_draw_image_rect(argc, argv, self);
        }
        else if (_RB_ARRAY_P(argv[1])) {
            gdip_graphics_draw_image_points(self, argv[0], argv[1]);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc == 3) {
        if (Integer_p(argv[1]) || Float_p(argv[1])) {
            gdip_graphics_draw_image_point(argc, argv, self);
        }
        else if (_KIND_OF(argv[1], &tPoint) || _KIND_OF(argv[1], &tPointF)) {
            gdip_graphics_draw_image_point_rect(argc, argv, self);
        }
        else if (_KIND_OF(argv[1], &tRectangle) || _KIND_OF(argv[1], &tRectangleF)) {
            gdip_graphics_draw_image_rect_rect(argc, argv, self);
        }
        else if (_RB_ARRAY_P(argv[1])) {
            gdip_graphics_draw_image_points_rect(argc, argv, self);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc == 4) {
        if (Integer_p(argv[1]) || Float_p(argv[1])) {
            gdip_graphics_draw_image_point_rect(argc, argv, self);
        }
        else if (_KIND_OF(argv[1], &tPoint) || _KIND_OF(argv[1], &tPointF)) {
            gdip_graphics_draw_image_point_rect(argc, argv, self);
        }
        else if (_KIND_OF(argv[1], &tRectangle) || _KIND_OF(argv[1], &tRectangleF)) {
            gdip_graphics_draw_image_rect_rect(argc, argv, self);
        }
        else if (_RB_ARRAY_P(argv[1])) {
            gdip_graphics_draw_image_points_rect(argc, argv, self);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc == 5) {
        if (Integer_p(argv[1], argv[3]) || Float_p(argv[1], argv[3])) {
            gdip_graphics_draw_image_rect(argc, argv, self);
        }
        else if (Integer_p(argv[1]) || Float_p(argv[1])) {
            gdip_graphics_draw_image_point_rect(argc, argv, self);
        }
        else if (_KIND_OF(argv[1], &tRectangle) || _KIND_OF(argv[1], &tRectangleF)) {
            gdip_graphics_draw_image_rect_rect(argc, argv, self);
        }
        else if (_RB_ARRAY_P(argv[1])) {
            gdip_graphics_draw_image_points_rect(argc, argv, self);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc == 6) {
        if (Integer_p(argv[1]) || Float_p(argv[1])) {
            gdip_graphics_draw_image_rect_rect(argc, argv, self);
        }
        else if (_KIND_OF(argv[1], &tPoint) || _KIND_OF(argv[1], &tPointF)) {
            gdip_graphics_draw_image_point_rect(argc, argv, self);
        }
        else if (_KIND_OF(argv[1], &tRectangle) || _KIND_OF(argv[1], &tRectangleF)) {
            gdip_graphics_draw_image_rect_rect(argc, argv, self);
        }
        else if (_RB_ARRAY_P(argv[1])) {
            gdip_graphics_draw_image_points_rect(argc, argv, self);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc == 7) {
        if (Integer_p(argv[1], argv[5]) || Float_p(argv[1], argv[5])) {
            gdip_graphics_draw_image_point_rect(argc, argv, self);
        }
        else if (Integer_p(argv[1]) || Float_p(argv[1])) {
            gdip_graphics_draw_image_rect_rect(argc, argv, self);
        }
        else if (_KIND_OF(argv[1], &tPoint) || _KIND_OF(argv[1], &tPointF)) {
            gdip_graphics_draw_image_point_rect(argc, argv, self);
        }
        else if (_KIND_OF(argv[1], &tRectangle) || _KIND_OF(argv[1], &tRectangleF)) {
            gdip_graphics_draw_image_rect_rect(argc, argv, self);
        }
        else if (_RB_ARRAY_P(argv[1])) {
            gdip_graphics_draw_image_points_rect(argc, argv, self);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc == 8) {
        if (Integer_p(argv[1], argv[5]) || Float_p(argv[1], argv[5])) {
            gdip_graphics_draw_image_point_rect(argc, argv, self);
        }
        else if (Integer_p(argv[1]) || Float_p(argv[1])) {
            gdip_graphics_draw_image_rect_rect(argc, argv, self);
        }
        else if (_KIND_OF(argv[1], &tRectangle) || _KIND_OF(argv[1], &tRectangleF)) {
            gdip_graphics_draw_image_rect_rect(argc, argv, self);
        }
        else if (_RB_ARRAY_P(argv[1])) {
            gdip_graphics_draw_image_points_rect(argc, argv, self);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }
    else if (argc >= 9) {
        if (Integer_p(argv[1]) || Float_p(argv[1])) {
            gdip_graphics_draw_image_rect_rect(argc, argv, self);
        }
        else {
            rb_raise(rb_eTypeError, "wrong types of arguments");
        }
    }

    return self;
}

/**
 * Sets the clipping region.
 * @overload SetClip(graphics, mode=CombineMode.Replace)
 *   @param graphics [Graphics]
 *   @param mode [CombineMode]
 * @overload SetClip(path, mode=CombineMode.Replace)
 *   @param path [GraphicsPath]
 *   @param mode [CombineMode]
 * @overload SetClip(rect, mode=CombineMode.Replace)
 *   @param rect [Rectangle or RectangleF]
 *   @param mode [CombineMode]
 * @overload SetClip(region, mode=CombineMode.Replace)
 *   @param region [Region]
 *   @param mode [CombineMode]
 * @return [self]
 */
static VALUE
gdip_graphics_m_set_clip(int argc, VALUE *argv, VALUE self)
{
    VALUE area, v_mode;
    rb_scan_args(argc, argv, "11", &area, &v_mode);

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");

    CombineMode mode = CombineModeReplace;
    if (!RB_NIL_P(v_mode)) {
        gdip_arg_to_enumint(cCombineMode, v_mode, &mode, "The second argument should be CombineMode.");
    }

    Status status = Ok;
    if (_KIND_OF(area, &tGraphics)) {
        Graphics *graphics = Data_Ptr<Graphics *>(area);
        status = g->SetClip(graphics, mode);
    }
    else if (_KIND_OF(area, &tGraphicsPath)) {
        GraphicsPath *path = Data_Ptr<GraphicsPath *>(area);
        status = g->SetClip(path, mode);
    }
    else if (_KIND_OF(area, &tRectangle)) {
        Rect *rect = Data_Ptr<Rect *>(area);
        status = g->SetClip(*rect, mode);
    }
    else if (_KIND_OF(area, &tRectangleF)) {
        RectF *rect = Data_Ptr<RectF *>(area);
        status = g->SetClip(*rect, mode);
    }
    else if (_KIND_OF(area, &tRegion)) {
        Region *region = Data_Ptr<Region *>(area);
        status = g->SetClip(region, mode);
    }
    else {
        rb_raise(rb_eTypeError, "The first argument should be Graphics, GraphicsPath, Rectangle, RectangleF or Region.");
    }
    Check_Status(status);

    return self;
}

/**
 * Excludes the specified area from the clipping region of this Graphics.
 * @overload ExcludeClip(rect)
 *   @param rect [Rectangle or RectangleF]
 * @overload ExcludeClip(region)
 *   @param region [Region]
 * @return [self]
 */
static VALUE
gdip_graphics_exclude_clip(VALUE self, VALUE area)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");

    Status status = Ok;
    if (_KIND_OF(area, &tRectangle)) {
        Rect *rect = Data_Ptr<Rect *>(area);
        status = g->ExcludeClip(*rect);
    }
    else if (_KIND_OF(area, &tRectangleF)) {
        RectF *rect = Data_Ptr<RectF *>(area);
        status = g->ExcludeClip(*rect);
    }
    else if (_KIND_OF(area, &tRegion)) {
        Region *region = Data_Ptr<Region *>(area);
        status = g->ExcludeClip(region);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Rectangle, RectangleF or Region.");
    }
    Check_Status(status);

    return self;
}

/**
 * Updates the clipping region of this Graphics to the intersection.
 * @overload IntersectClip(rect)
 *   @param rect [Rectangle or RectangleF]
 * @overload IntersectClip(region)
 *   @param region [Region]
 * @return [self]
 */
static VALUE
gdip_graphics_intersect_clip(VALUE self, VALUE area)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");

    Status status = Ok;
    if (_KIND_OF(area, &tRectangle)) {
        Rect *rect = Data_Ptr<Rect *>(area);
        status = g->IntersectClip(*rect);
    }
    else if (_KIND_OF(area, &tRectangleF)) {
        RectF *rect = Data_Ptr<RectF *>(area);
        status = g->IntersectClip(*rect);
    }
    else if (_KIND_OF(area, &tRegion)) {
        Region *region = Data_Ptr<Region *>(area);
        status = g->IntersectClip(region);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be Rectangle, RectangleF or Region.");
    }
    Check_Status(status);

    return self;
}

/**
 * Reset the clipping region of this Graphics.
 * @return [self]
 */
static VALUE
gdip_graphics_reset_clip(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");

    Status status = g->ResetClip();
    Check_Status(status);

    return self;
}

/**
 * Translates the clipping region of this Graphics.
 * @param dx [Integer or Float]
 * @param dy [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_graphics_translate_clip(VALUE self, VALUE dx, VALUE dy)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");

    Status status = Ok;
    if (Integer_p(dx, dy)) {
        status = g->TranslateClip(RB_NUM2INT(dx), RB_NUM2INT(dy));
    }
    else if (Float_p(dx, dy)) {
        status = g->TranslateClip(NUM2SINGLE(dx), NUM2SINGLE(dy));
    }
    else {
        rb_raise(rb_eTypeError, "Every arguments should be Integer or Float.");
    }
    Check_Status(status);

    return self;
}

/**
 * Whether the specified Point or Rectangle is contained within the visible clip region.
 * @overload IsVisible(point)
 *   @param point [Point or PointF]
 * @overload IsVisible(x, y)
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 * @overload IsVisible(rect)
 *   @param rect [Rectangle or RectangleF]
 * @overload IsVisible(x, y, w, h)
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param w [Integer or Float]
 *   @param h [Integer or Float]
 * @return [Boolean]
 */
static VALUE
gdip_graphics_is_visible(int argc, VALUE *argv, VALUE self)
{
    if (argc != 1 && argc != 2 && argc != 4) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1, 2, 4)", argc);
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");

    BOOL r = false;
    if (argc == 1) {
        if (_KIND_OF(argv[0], &tPoint)) {
            Point *point = Data_Ptr<Point *>(argv[0]);
            r = g->IsVisible(*point);
        }
        else if (_KIND_OF(argv[0], &tPointF)) {
            PointF *point = Data_Ptr<PointF *>(argv[0]);
            r = g->IsVisible(*point);
        }
        else if (_KIND_OF(argv[0], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[0]);
            r = g->IsVisible(*rect);
        }
        else if (_KIND_OF(argv[0], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[0]);
            r = g->IsVisible(*rect);
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Point, PointF, Rectangle or RectangleF.");
        }
    }
    else if (argc == 2) {
        if (Integer_p(argv[0], argv[1])) {
            r = g->IsVisible(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]));
        }
        else if (Float_p(argv[0], argv[1])) {
            r = g->IsVisible(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer or Float.");
        }
    }
    else if (argc == 4) {
        if (Integer_p(4, argv)) {
            r = g->IsVisible(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]));
        }
        else if (Float_p(4, argv)) {
            r = g->IsVisible(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer or Float.");
        }
    }
    Check_LastStatus(g);

    return r ? Qtrue : Qfalse;
}

/**
 * Resets the transformation matrix.
 * @return [self]
 */
static VALUE
gdip_graphics_reset_transform(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");

    Status status = g->ResetTransform();
    Check_Status(status);

    return self;
}

/**
 * @overload MultiplyTransform(matrix, order=MatrixOrder.Prepend)
 *   Multiplies the transformation matrix.
 *   @param matrix [Matrix]
 *   @param order [MatrixOrder]
 *   @return [self]
 */

static VALUE
gdip_graphics_multiply_transform(int argc, VALUE *argv, VALUE self)
{
    VALUE v_matrix, v_order;
    rb_scan_args(argc, argv, "11", &v_matrix, &v_order);

    if (!_KIND_OF(v_matrix, &tMatrix)) {
        rb_raise(rb_eTypeError, "The first argument should be Matrix.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    Matrix *matrix = Data_Ptr<Matrix *>(v_matrix);
    MatrixOrder order = MatrixOrderPrepend;

    if (!RB_NIL_P(v_order)) {
        gdip_arg_to_enumint(cMatrixOrder, v_order, &order, "The second argument should be MatrixOrder.");
    }

    Status status = g->MultiplyTransform(matrix, order);
    Check_Status(status);
    
    return self;
}

/**
 * @overload TranslateTransform(dx, dy, order=MatrixOrder.Prepend)
 *   Translates.
 *   @param dx [Integer or Float]
 *   @param dy [Integer or Float]
 *   @param order [MatrixOrder]
 *   @return [self]
 */
static VALUE
gdip_graphics_translate_transform(int argc, VALUE *argv, VALUE self)
{
    VALUE v_dx, v_dy, v_order;
    rb_scan_args(argc, argv, "21", &v_dx, &v_dy, &v_order);

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    MatrixOrder order = MatrixOrderPrepend;

    if (!RB_NIL_P(v_order)) {
        gdip_arg_to_enumint(cMatrixOrder, v_order, &order, "The third argument should be MatrixOrder.");
    }

    float dx = 0.0f;
    float dy = 0.0f;
    gdip_arg_to_single(v_dx, &dx, "The first argument should be Integer or Float.");
    gdip_arg_to_single(v_dy, &dy, "The second argument should be Integer or Float.");
    Status status = g->TranslateTransform(dx, dy, order);
    Check_Status(status);
    
    return self;
}

/**
 * @overload ScaleTransform(sx, sy, order=MatrixOrder.Prepend)
 *   Scales.
 *   @param sx [Integer or Float]
 *   @param sy [Integer or Float]
 *   @param order [MatrixOrder]
 *   @return [self]
 */
static VALUE
gdip_graphics_scale_transform(int argc, VALUE *argv, VALUE self)
{
    VALUE v_sx, v_sy, v_order;
    rb_scan_args(argc, argv, "21", &v_sx, &v_sy, &v_order);

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    float sx = 0.0f;
    float sy = 0.0f;
    MatrixOrder order = MatrixOrderPrepend;

    gdip_arg_to_single(v_sx, &sx, "The first argument should be Float.");
    gdip_arg_to_single(v_sy, &sy, "The second argument should be Float.");

    if (!RB_NIL_P(v_order)) {
        gdip_arg_to_enumint(cMatrixOrder, v_order, &order, "The third argument should be MatrixOrder.");
    }
    
    Status status = g->ScaleTransform(sx, sy, order);
    Check_Status(status);
    
    return self;
}

/**
 * @overload RotateTransform(angle, order=MatrixOrder.Prepend)
 *   Scales.
 *   @param angle [Integer or Float]
 *   @param order [MatrixOrder]
 *   @return [self]
 */
static VALUE
gdip_graphics_rotate_transform(int argc, VALUE *argv, VALUE self)
{
    VALUE v_angle, v_order;
    rb_scan_args(argc, argv, "11", &v_angle, &v_order);

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");
    float angle = 0.0f;
    MatrixOrder order = MatrixOrderPrepend;

    gdip_arg_to_single(v_angle, &angle, "The first argument should be Integer or Float.");

    if (!RB_NIL_P(v_order)) {
        gdip_arg_to_enumint(cMatrixOrder, v_order, &order, "The second argument should be MatrixOrder.");
    }

    Status status = g->RotateTransform(angle, order);
    Check_Status(status);
    
    return self;
}

/**
 * @overload TransformPoints(dest, src, points)
 *   Transforms an array of points.
 *   @param dest [CoordinateSpace]
 *   @param src [CoordinateSpace]
 *   @param points [Array<Point or PointF>]
 *   @return [self]
 */
static VALUE
gdip_graphics_transform_points(VALUE self, VALUE v_dest, VALUE v_src, VALUE v_points)
{
    if (!_RB_ARRAY_P(v_points) || RARRAY_LEN(v_points) <= 0) {
        rb_raise(rb_eArgError, "The third argument should be Array of Point or PointF.");
    }

    CoordinateSpace dest, src;
    gdip_arg_to_enumint(cCoordinateSpace, v_dest, &dest, "The first arugment should be CoodinateSpace.");
    gdip_arg_to_enumint(cCoordinateSpace, v_src, &src, "The second arugment should be CoodinateSpace.");

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");

    VALUE first = rb_ary_entry(v_points, 0);
    Status status = Ok;
    VALUE r = Qnil;

    if (_KIND_OF(first, &tPoint)) {
        int count = 0;
        Point *points = alloc_array_of<Point, &tPoint>(v_points, count);
        VALUE tmp = _wrap_tmp_buffer(points);
        status = g->TransformPoints(dest, src, points, count);
        Check_Status(status);
        r = rb_ary_new_capa(count);
        for (int i = 0; i < count; ++i) {
            rb_ary_push(r, gdip_point_create(points[i].X, points[i].Y));
        }
        _rb_free_tmp_buffer(&tmp);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count = 0;
        PointF *points = alloc_array_of<PointF, &tPointF>(v_points, count);
        VALUE tmp = _wrap_tmp_buffer(points);
        status = g->TransformPoints(dest, src, points, count);
        Check_Status(status);
        r = rb_ary_new_capa(count);
        for (int i = 0; i < count; ++i) {
            rb_ary_push(r, gdip_pointf_create(points[i].X, points[i].Y));
        }
        _rb_free_tmp_buffer(&tmp);
    }
    else {
        rb_raise(rb_eArgError, "The third argument should be Array of Point or PointF.");
    }

    return r;
}

/**
 * Measures the size of the string to be drawn.
 * @overload MeasureString(str, font)
 *   @param str [String]
 *   @param font [Font]
 *   @return [SizeF]
 * @overload MeasureString(str, font, width, format=nil)
 *   @param str [String]
 *   @param font [Font]
 *   @param width [Integer or Float]
 *   @param format [StringFormat]
 *   @return [SizeF]
 * @overload MeasureString(str, font, origin, format=nil)
 *   @param str [String]
 *   @param font [Font]
 *   @param origin [PointF]
 *   @param format [StringFormat]
 *   @return [SizeF]
 * @overload MeasureString(str, font, area, format=nil, info=nil)
 *   @param str [String]
 *   @param font [Font]
 *   @param area [SizeF]
 *   @param format [StringFormat]
 *   @param info [Hash]
 *   @return [SizeF]
 * @example
 *   bmp = Bitmap.new(400, 400)
 *   bmp.draw { |g|
 *     font = Font.new("MS Gothic", 16)
 *     area = SizeF.new(80.0, 1000.0)
 *     info = {}
 *     size = g.MeasureString("The quick brown fox jumps over the lazy dog", font, area, nil, info)
 *     p size => #<Gdiplus::SizeF Width=79.507812, Height=82.000000>
 *     p info # => {"charactersFitted"=>43, "linesFilled"=>5}
 *   }
 */
static VALUE
gdip_graphics_measure_string(int argc, VALUE *argv, VALUE self)
{
    if (argc < 2 || 5 < argc) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2..5)", argc);
    }
    
    if (!_RB_STRING_P(argv[0])) {
        rb_raise(rb_eArgError, "The first argument should be String.");
    }

    if (!_KIND_OF(argv[1], &tFont)) {
        rb_raise(rb_eArgError, "The second argument should be Font.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");

    VALUE wstr = util_utf16_str_new(argv[0]);
    int length = RSTRING_LEN(wstr) / 2 - 1;
    if (length <= 0) {
        return gdip_sizef_create(0.0f, 0.0f);
    }

    Font *font = Data_Ptr<Font *>(argv[1]);
    Check_NULL(font, "The Font object does not exist.");

    SizeF size;
    Status status = Ok;

    if (argc == 2) {
        SizeF layout;
        status = g->MeasureString(RString_Ptr<WCHAR *>(wstr), length, font, layout, NULL, &size);
    }
    else if (argc == 3 || argc == 4) {
        StringFormat *format = NULL;
        if (argc == 4) {
            if (_KIND_OF(argv[3], &tStringFormat)) {
                format = Data_Ptr<StringFormat *>(argv[3]);
            }
            else {
                rb_raise(rb_eArgError, "The fourth argument should be StringFormat.");
            }
        }

        if (Integer_p(argv[2]) || Float_p(argv[2])) {
            float width = 0.0f;
            gdip_arg_to_single(argv[2], &width);
            SizeF layout(width, 1000000.0f);
            status = g->MeasureString(RString_Ptr<WCHAR *>(wstr), length, font, layout, format, &size);
        }
        else if (_KIND_OF(argv[2], &tSizeF)) {
            SizeF *p_layout = Data_Ptr<SizeF *>(argv[2]);
            status = g->MeasureString(RString_Ptr<WCHAR *>(wstr), length, font, *p_layout, format, &size);
        }
        else if (_KIND_OF(argv[2], &tPointF)) {
            PointF *origin = Data_Ptr<PointF *>(argv[2]);
            RectF box;
            status = g->MeasureString(RString_Ptr<WCHAR *>(wstr), length, font, *origin, format, &box);
            size.Width = box.Width;
            size.Height = box.Height;
        }
        else {
            rb_raise(rb_eArgError, "invalid type of argument 3");
        }
    }
    else if (argc == 5) {
        if (!_KIND_OF(argv[2], &tSizeF)) {
            rb_raise(rb_eArgError, "The third argument should be SizeF.");
        }
        
        if (!_KIND_OF(argv[3], &tStringFormat) && !RB_NIL_P(argv[3])) {
            rb_raise(rb_eArgError, "The fourth argument should be StringFormat.");
        }

        if (!_RB_HASH_P(argv[4]) && !RB_NIL_P(argv[4])) {
            rb_raise(rb_eArgError, "The fifth argument should be Hash.");
        }

        StringFormat *format = NULL;
        if (_KIND_OF(argv[3], &tStringFormat)) {
            format = Data_Ptr<StringFormat *>(argv[3]);
        }

        SizeF *p_layout = Data_Ptr<SizeF *>(argv[2]);
        int charactersFitted = 0;
        int linesFilled = 0;
        status = g->MeasureString(RString_Ptr<WCHAR *>(wstr), length, font, *p_layout, format, &size, &charactersFitted, &linesFilled);

        if (_RB_HASH_P(argv[4])) {
            rb_hash_aset(argv[4], rb_str_new_cstr("charactersFitted"), RB_INT2NUM(charactersFitted));
            rb_hash_aset(argv[4], rb_str_new_cstr("linesFilled"), RB_INT2NUM(linesFilled));
        }
    }
    RB_GC_GUARD(wstr);
    Check_Status(status);

    return gdip_sizef_create(size.Width, size.Height);
}

/**
 * @overload MeasureCharacterRanges(str, font, rect, format)
 *   @param str [String]
 *   @param font [Font]
 *   @param rect [RectangleF]
 *   @param format [StringFormat]
 *   @return [Array<Region>]
 * @example
 *   bmp.draw { |g|
 *     g.Clear(:White)
 *
 *     #      0123456789012345678901234567890123456789012
 *     str = "The quick brown fox jumps over the lazy dog"
 *     font = Font.new("MS Gothic", 16)
 *     format = StringFormat.new
 *     format.SetMeasurableCharacterRanges(
 *       [0..2, 4..8, 10..14, 16..18, 20..24, 26..29, 31..33, 35..38, 40..42])
 *     rect = RectangleF.new(20.0, 20.0, 200.0, 200.0)
 *     g.DrawString(str, font, Brushes.Black, rect, format)
 *     regions = g.MeasureCharacterRanges(str, font, rect, format)
 *     regions.each.with_index { |region, idx|
 *       bound = region.GetBounds(g)
 *       g.DrawRectangle(Pens.Red, bound)
 *     }
 *   }
 *
 */
static VALUE
gdip_graphics_measure_character_ranges(VALUE self, VALUE str, VALUE v_font, VALUE v_rect, VALUE v_format)
{
    if (!_RB_STRING_P(str)) {
        rb_raise(rb_eArgError, "The first argument should be String.");
    }

    if (!_KIND_OF(v_font, &tFont)) {
        rb_raise(rb_eArgError, "The second argument should be Font.");
    }

    if (!_KIND_OF(v_rect, &tRectangleF)) {
        rb_raise(rb_eArgError, "The third argument should be RectangleF.");
    }

    if (!_KIND_OF(v_format, &tStringFormat)) {
        rb_raise(rb_eArgError, "The fourth argument should be StringFormat.");
    }

    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "This Graphics object does not exist.");

    VALUE wstr = util_utf16_str_new(str);
    int length = RSTRING_LEN(wstr) / 2 - 1;

    Font *font = Data_Ptr<Font *>(v_font);
    Check_NULL(font, "The Font object does not exist.");
    RectF *rect = Data_Ptr<RectF *>(v_rect);
    StringFormat *format = Data_Ptr<StringFormat *>(v_format);
    Check_NULL(format, "The StringFormat object does not exist.");

    int count = format->GetMeasurableCharacterRangeCount();
    if (count <= 0) {
        return rb_ary_new();
    }

    Region *regions = new Region[count];
    VALUE tmp = wrap_tmp_ary(regions);
    Status status = g->MeasureCharacterRanges(RString_Ptr<WCHAR *>(wstr), length, font, *rect, format, count, regions);
    RB_GC_GUARD(wstr);
    if (status != Ok) {
        delete[] regions;
        Check_Status(status);
    }

    VALUE r = rb_ary_new_capa(count);
    for (int i = 0; i < count; ++i) {
        VALUE tmp = typeddata_alloc_null<&tRegion>(cRegion);
        DATA_PTR(tmp) = gdip_obj_create(regions[i].Clone());
        rb_ary_push(r, tmp);
    }

    // delete[] regions;
    delete_tmp_ary(&tmp);
    
    return r;
}

void
Init_graphics()
{
    cGraphics = rb_define_class_under(mGdiplus, "Graphics", cGpObject);
    rb_undef_alloc_func(cGraphics);

    ATTR_RW(cGraphics, Clip, clip, graphics);
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
    ATTR_RW(cGraphics, Transform, transform, graphics);
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
    rb_define_method(cGraphics, "FillRegion", RUBY_METHOD_FUNC(gdip_graphics_fill_region), 2);
    rb_define_alias(cGraphics, "fill_region", "FillRegion");

    rb_define_method(cGraphics, "DrawImagePoint", RUBY_METHOD_FUNC(gdip_graphics_draw_image_point), -1);
    rb_define_alias(cGraphics, "draw_image_point", "DrawImagePoint");
    rb_define_method(cGraphics, "DrawImageRect", RUBY_METHOD_FUNC(gdip_graphics_draw_image_rect), -1);
    rb_define_alias(cGraphics, "draw_image_rect", "DrawImageRect");
    rb_define_method(cGraphics, "DrawImagePoints", RUBY_METHOD_FUNC(gdip_graphics_draw_image_points), 2);
    rb_define_alias(cGraphics, "draw_image_points", "DrawImagePoints");
    rb_define_method(cGraphics, "DrawImagePointRect", RUBY_METHOD_FUNC(gdip_graphics_draw_image_point_rect), -1);
    rb_define_alias(cGraphics, "draw_image_point_rect", "DrawImagePointRect");
    rb_define_method(cGraphics, "DrawImageRectRect", RUBY_METHOD_FUNC(gdip_graphics_draw_image_rect_rect), -1);
    rb_define_alias(cGraphics, "draw_image_rect_rect", "DrawImageRectRect");    
    rb_define_method(cGraphics, "DrawImagePointsRect", RUBY_METHOD_FUNC(gdip_graphics_draw_image_points_rect), -1);
    rb_define_alias(cGraphics, "draw_image_points_rect", "DrawImagePointsRect");
    rb_define_method(cGraphics, "DrawImage", RUBY_METHOD_FUNC(gdip_graphics_draw_image), -1);
    rb_define_alias(cGraphics, "draw_image", "DrawImage");

    rb_define_method(cGraphics, "SetClip", RUBY_METHOD_FUNC(gdip_graphics_m_set_clip), -1);
    rb_define_alias(cGraphics, "set_clip", "SetClip");
    rb_define_method(cGraphics, "ExcludeClip", RUBY_METHOD_FUNC(gdip_graphics_exclude_clip), 1);
    rb_define_alias(cGraphics, "exclude_clip", "ExcludeClip");
    rb_define_method(cGraphics, "IntersectClip", RUBY_METHOD_FUNC(gdip_graphics_intersect_clip), 1);
    rb_define_alias(cGraphics, "intersect_clip", "IntersectClip");
    rb_define_method(cGraphics, "ResetClip", RUBY_METHOD_FUNC(gdip_graphics_reset_clip), 0);
    rb_define_alias(cGraphics, "reset_clip", "ResetClip");
    rb_define_method(cGraphics, "TranslateClip", RUBY_METHOD_FUNC(gdip_graphics_translate_clip), 2);
    rb_define_alias(cGraphics, "translate_clip", "TranslateClip");

    rb_define_method(cGraphics, "IsVisible", RUBY_METHOD_FUNC(gdip_graphics_is_visible), -1);
    rb_define_alias(cGraphics, "is_visible", "IsVisible");

    rb_define_method(cGraphics, "ResetTransform", RUBY_METHOD_FUNC(gdip_graphics_reset_transform), 0);
    rb_define_alias(cGraphics, "reset_transform", "ResetTransform");
    rb_define_method(cGraphics, "MultiplyTransform", RUBY_METHOD_FUNC(gdip_graphics_multiply_transform), -1);
    rb_define_alias(cGraphics, "multiply_transform", "MultiplyTransform");
    rb_define_method(cGraphics, "TranslateTransform", RUBY_METHOD_FUNC(gdip_graphics_translate_transform), -1);
    rb_define_alias(cGraphics, "translate_transform", "TranslateTransform");
    rb_define_method(cGraphics, "ScaleTransform", RUBY_METHOD_FUNC(gdip_graphics_scale_transform), -1);
    rb_define_alias(cGraphics, "scale_transform", "ScaleTransform");
    rb_define_method(cGraphics, "RotateTransform", RUBY_METHOD_FUNC(gdip_graphics_rotate_transform), -1);
    rb_define_alias(cGraphics, "rotate_transform", "RotateTransform");

    rb_define_method(cGraphics, "TransformPoints", RUBY_METHOD_FUNC(gdip_graphics_transform_points), 3);
    rb_define_alias(cGraphics, "transform_points", "TransformPoints");

    rb_define_method(cGraphics, "MeasureString", RUBY_METHOD_FUNC(gdip_graphics_measure_string), -1);
    rb_define_alias(cGraphics, "measure_string", "MeasureString");
    rb_define_method(cGraphics, "MeasureCharacterRanges", RUBY_METHOD_FUNC(gdip_graphics_measure_character_ranges), 4);
    rb_define_alias(cGraphics, "measure_character_ranges", "MeasureCharacterRanges");
}