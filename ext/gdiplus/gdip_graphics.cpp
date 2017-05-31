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

static VALUE
gdip_graphics_get_dpi_x(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return SINGLE2NUM(g->GetDpiX());
}

static VALUE
gdip_graphics_get_dpi_y(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return SINGLE2NUM(g->GetDpiY());
}

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

static VALUE
gdip_graphics_get_is_clip_empty(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return (g->IsClipEmpty()) ? Qtrue : Qfalse;
}

static VALUE
gdip_graphics_get_is_visible_clip_empty(VALUE self)
{
    Graphics *g = Data_Ptr<Graphics *>(self);
    Check_NULL(g, "The graphics object does not exist.");
    return (g->IsVisibleClipEmpty()) ? Qtrue : Qfalse;
}

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

    rb_define_method(cGraphics, "ClipBounds", RUBY_METHOD_FUNC(gdip_graphics_get_clip_bounds), 0);
    rb_define_method(cGraphics, "clip_bounds", RUBY_METHOD_FUNC(gdip_graphics_get_clip_bounds), 0);
    rb_define_method(cGraphics, "CompositingMode", RUBY_METHOD_FUNC(gdip_graphics_get_compositing_mode), 0);
    rb_define_method(cGraphics, "compositing_mode", RUBY_METHOD_FUNC(gdip_graphics_get_compositing_mode), 0);
    rb_define_method(cGraphics, "CompositingMode=", RUBY_METHOD_FUNC(gdip_graphics_set_compositing_mode), 1);
    rb_define_method(cGraphics, "compositing_mode=", RUBY_METHOD_FUNC(gdip_graphics_set_compositing_mode), 1);
    rb_define_method(cGraphics, "CompositingQuality", RUBY_METHOD_FUNC(gdip_graphics_get_compositing_quality), 0);
    rb_define_method(cGraphics, "compositing_quality", RUBY_METHOD_FUNC(gdip_graphics_get_compositing_quality), 0);
    rb_define_method(cGraphics, "CompositingQuality=", RUBY_METHOD_FUNC(gdip_graphics_set_compositing_quality), 1);
    rb_define_method(cGraphics, "compositing_quality=", RUBY_METHOD_FUNC(gdip_graphics_set_compositing_quality), 1);
    rb_define_method(cGraphics, "DpiX", RUBY_METHOD_FUNC(gdip_graphics_get_dpi_x), 0);
    rb_define_method(cGraphics, "dpi_x", RUBY_METHOD_FUNC(gdip_graphics_get_dpi_x), 0);
    rb_define_method(cGraphics, "DpiY", RUBY_METHOD_FUNC(gdip_graphics_get_dpi_y), 0);
    rb_define_method(cGraphics, "dpi_y", RUBY_METHOD_FUNC(gdip_graphics_get_dpi_y), 0);
    rb_define_method(cGraphics, "InterpolationMode", RUBY_METHOD_FUNC(gdip_graphics_get_interpolation_mode), 0);
    rb_define_method(cGraphics, "interpolation_mode", RUBY_METHOD_FUNC(gdip_graphics_get_interpolation_mode), 0);
    rb_define_method(cGraphics, "InterpolationMode=", RUBY_METHOD_FUNC(gdip_graphics_set_interpolation_mode), 1);
    rb_define_method(cGraphics, "interpolation_mode=", RUBY_METHOD_FUNC(gdip_graphics_set_interpolation_mode), 1);
    rb_define_method(cGraphics, "IsClipEmpty", RUBY_METHOD_FUNC(gdip_graphics_get_is_clip_empty), 0);
    rb_define_method(cGraphics, "is_clip_empty?", RUBY_METHOD_FUNC(gdip_graphics_get_is_clip_empty), 0);
    rb_define_method(cGraphics, "IsVisibleClipEmpty", RUBY_METHOD_FUNC(gdip_graphics_get_is_visible_clip_empty), 0);
    rb_define_method(cGraphics, "is_visible_clip_empty?", RUBY_METHOD_FUNC(gdip_graphics_get_is_visible_clip_empty), 0);
    rb_define_method(cGraphics, "PageScale", RUBY_METHOD_FUNC(gdip_graphics_get_page_scale), 0);
    rb_define_method(cGraphics, "page_scale", RUBY_METHOD_FUNC(gdip_graphics_get_page_scale), 0);
    rb_define_method(cGraphics, "PageScale=", RUBY_METHOD_FUNC(gdip_graphics_set_page_scale), 1);
    rb_define_method(cGraphics, "page_scale=", RUBY_METHOD_FUNC(gdip_graphics_set_page_scale), 1);
    rb_define_method(cGraphics, "PageUnit", RUBY_METHOD_FUNC(gdip_graphics_get_page_unit), 0);
    rb_define_method(cGraphics, "page_unit", RUBY_METHOD_FUNC(gdip_graphics_get_page_unit), 0);
    rb_define_method(cGraphics, "PageUnit=", RUBY_METHOD_FUNC(gdip_graphics_set_page_unit), 1);
    rb_define_method(cGraphics, "page_unit=", RUBY_METHOD_FUNC(gdip_graphics_set_page_unit), 1);
    rb_define_method(cGraphics, "PixelOffsetMode", RUBY_METHOD_FUNC(gdip_graphics_get_pixel_offset_mode), 0);
    rb_define_method(cGraphics, "pixelOffset_mode", RUBY_METHOD_FUNC(gdip_graphics_get_pixel_offset_mode), 0);
    rb_define_method(cGraphics, "PixelOffsetMode=", RUBY_METHOD_FUNC(gdip_graphics_set_pixel_offset_mode), 1);
    rb_define_method(cGraphics, "pixelOffset_mode=", RUBY_METHOD_FUNC(gdip_graphics_set_pixel_offset_mode), 1);
    rb_define_method(cGraphics, "RenderingOrigin", RUBY_METHOD_FUNC(gdip_graphics_get_rendering_origin), 0);
    rb_define_method(cGraphics, "rendering_origin", RUBY_METHOD_FUNC(gdip_graphics_get_rendering_origin), 0);
    rb_define_method(cGraphics, "RenderingOrigin=", RUBY_METHOD_FUNC(gdip_graphics_set_rendering_origin), 1);
    rb_define_method(cGraphics, "rendering_origin=", RUBY_METHOD_FUNC(gdip_graphics_set_rendering_origin), 1);
    rb_define_method(cGraphics, "SmoothingMode", RUBY_METHOD_FUNC(gdip_graphics_get_smoothing_mode), 0);
    rb_define_method(cGraphics, "smoothing_mode", RUBY_METHOD_FUNC(gdip_graphics_get_smoothing_mode), 0);
    rb_define_method(cGraphics, "SmoothingMode=", RUBY_METHOD_FUNC(gdip_graphics_set_smoothing_mode), 1);
    rb_define_method(cGraphics, "smoothing_mode=", RUBY_METHOD_FUNC(gdip_graphics_set_smoothing_mode), 1);
    rb_define_method(cGraphics, "TextContrast", RUBY_METHOD_FUNC(gdip_graphics_get_text_contrast), 0);
    rb_define_method(cGraphics, "text_contrast", RUBY_METHOD_FUNC(gdip_graphics_get_text_contrast), 0);
    rb_define_method(cGraphics, "TextContrast=", RUBY_METHOD_FUNC(gdip_graphics_set_text_contrast), 1);
    rb_define_method(cGraphics, "text_contrast=", RUBY_METHOD_FUNC(gdip_graphics_set_text_contrast), 1);
    rb_define_method(cGraphics, "TextRenderingHint", RUBY_METHOD_FUNC(gdip_graphics_get_text_rendering_hint), 0);
    rb_define_method(cGraphics, "text_rendering_hint", RUBY_METHOD_FUNC(gdip_graphics_get_text_rendering_hint), 0);
    rb_define_method(cGraphics, "TextRenderingHint=", RUBY_METHOD_FUNC(gdip_graphics_set_text_rendering_hint), 1);
    rb_define_method(cGraphics, "text_rendering_hint=", RUBY_METHOD_FUNC(gdip_graphics_set_text_rendering_hint), 1);
    rb_define_method(cGraphics, "VisibleClipBounds", RUBY_METHOD_FUNC(gdip_graphics_get_visible_clip_bounds), 0);
    rb_define_method(cGraphics, "visible_clip_bounds", RUBY_METHOD_FUNC(gdip_graphics_get_visible_clip_bounds), 0);
    

    rb_define_method(cGraphics, "DrawRectangle", RUBY_METHOD_FUNC(gdip_graphics_draw_rectangle), -1);
    rb_define_method(cGraphics, "draw_rectangle", RUBY_METHOD_FUNC(gdip_graphics_draw_rectangle), -1);
    rb_define_method(cGraphics, "FillRectangle", RUBY_METHOD_FUNC(gdip_graphics_fill_rectangle), -1);
    rb_define_method(cGraphics, "fill_rectangle", RUBY_METHOD_FUNC(gdip_graphics_fill_rectangle), -1);
    rb_define_method(cGraphics, "DrawLine", RUBY_METHOD_FUNC(gdip_graphics_draw_line), -1);
    rb_define_method(cGraphics, "draw_line", RUBY_METHOD_FUNC(gdip_graphics_draw_line), -1);
    rb_define_method(cGraphics, "DrawEllipse", RUBY_METHOD_FUNC(gdip_graphics_draw_ellipse), -1);
    rb_define_method(cGraphics, "draw_ellipse", RUBY_METHOD_FUNC(gdip_graphics_draw_ellipse), -1);
    rb_define_method(cGraphics, "FillEllipse", RUBY_METHOD_FUNC(gdip_graphics_fill_ellipse), -1);
    rb_define_method(cGraphics, "fill_ellipse", RUBY_METHOD_FUNC(gdip_graphics_fill_ellipse), -1);
}