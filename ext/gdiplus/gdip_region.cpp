/*
 * gdip_region.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tRegion = _MAKE_DATA_TYPE(
    "Region", 0, GDIP_OBJ_FREE(Region *), NULL, NULL, &cRegion);

struct RegionData {
    BYTE *data;
    unsigned int count;
};

static void
gdip_regiondata_free(RegionData *ptr)
{
    dp("RegionData: RData free");
    ruby_xfree(ptr->data);
    ruby_xfree(ptr);
}

static VALUE cRegionData;
static const rb_data_type_t tRegionData = _MAKE_DATA_TYPE(
    "RegionData", 0, RUBY_DATA_FUNC(gdip_regiondata_free), NULL, NULL, &cRegionData);

static VALUE
gdip_regiondata_create(BYTE *data, unsigned int count)
{
    dp("RegionData: RData alloc");
    RegionData *ptr = static_cast<RegionData *>(RB_ZALLOC(RegionData));
    ptr->data = data;
    ptr->count = count;
    VALUE r = _Data_Wrap_Struct(cRegionData, &tRegionData, ptr);
    return r;
}

static VALUE
gdip_regiondata_size(VALUE self)
{
    RegionData *rdata = Data_Ptr<RegionData *>(self);
    return RB_UINT2NUM(rdata->count);
}

/**
 * @overload initialize()
 * @overload initialize(path)
 *   @param path [GraphicsPath]
 * @overload initialize(rect)
 *   @param path [Rectangle or RectangleF]
 * @overload initialize(data)
 *   @param data [RegionData]
 */
static VALUE
gdip_region_init(int argc, VALUE *argv, VALUE self)
{
    if (_DATA_PTR(self) != NULL) {
        _VERBOSE("This Region object is already initialized.");
        return self;
    }

    if (argc > 1) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    if (argc == 0) {
        _DATA_PTR(self) = gdip_obj_create(new Region());
    }
    else if (argc == 1) {
        if (_KIND_OF(argv[0], &tGraphicsPath)) {
            GraphicsPath *gp = Data_Ptr<GraphicsPath *>(argv[0]);
            Check_NULL(gp, "The GraphicsPath object of argument does not exist.");
            _DATA_PTR(self) = gdip_obj_create(new Region(gp));
        }
        else if (_KIND_OF(argv[0], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[0]);
            _DATA_PTR(self) = gdip_obj_create(new Region(*rect));
        }
        else if (_KIND_OF(argv[0], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[0]);
            _DATA_PTR(self) = gdip_obj_create(new Region(*rect));
        }
        else if (_KIND_OF(argv[0], &tRegionData)) {
            RegionData *rdata = Data_Ptr<RegionData *>(argv[0]);
            _DATA_PTR(self) = gdip_obj_create(new Region(rdata->data, rdata->count));
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be GraphicsPath, Rectangle, RectangleF or RegionData.");
        }
    }
    return self;
}

/**
 * Duplicates this Region object.
 * @return [Region]
 */
static VALUE
gdip_region_clone(VALUE self)
{
    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");

    VALUE r = typeddata_alloc_null<&tRegion>(cRegion);
    _DATA_PTR(r) = gdip_obj_create(region->Clone());

    return r;
}

/**
 * Empties this Region object.
 * @return [self]
 */
static VALUE
gdip_region_make_empty(VALUE self)
{
    Check_Frozen(self);
    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");

    Status status = region->MakeEmpty();
    Check_Status(status);

    return self;
}

/**
 * 
 * @return [self]
 */
static VALUE
gdip_region_make_infinite(VALUE self)
{
    Check_Frozen(self);
    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");

    Status status = region->MakeInfinite();
    Check_Status(status);

    return self;
}

/**
 * Gets a RegionData object.
 * @return [RegionData]
 */
static VALUE
gdip_region_get_data(VALUE self)
{
    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");

    unsigned int count = region->GetDataSize();
    BYTE *data = static_cast<BYTE *>(RB_ZALLOC_N(BYTE, count));
    Status status = region->GetData(data, count, NULL);
    VALUE r = gdip_regiondata_create(data, count);
    Check_Status(status);

    return r;
}

/**
 * Updates this Region object to the intersection with another.
 * @param other [GraphicsPath, Rectangle, RectangleF or Region]
 * @return [self]
 */
static VALUE
gdip_region_intersect(VALUE self, VALUE other)
{
    Check_Frozen(self);
    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");
    
    Status status = Ok;
    if (_KIND_OF(other, &tGraphicsPath)) {
        GraphicsPath *gp = Data_Ptr<GraphicsPath *>(other);
        Check_NULL(gp, "The GraphicsPath object of argument does not exist.");
        status = region->Intersect(gp);
    }
    else if (_KIND_OF(other, &tRectangle)) {
        Rect *rect = Data_Ptr<Rect *>(other);
        status = region->Intersect(*rect);
    }
    else if (_KIND_OF(other, &tRectangleF)) {
        RectF *rect = Data_Ptr<RectF *>(other);
        status = region->Intersect(*rect);
    }
    else if (_KIND_OF(other, &tRegion)) {
        Region *other_region = Data_Ptr<Region *>(other);
        Check_NULL(other_region, "The Region object of argument does not exist.");
        status = region->Intersect(other_region);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be GraphicsPath, Rectangle, RectangleF or Region.");
    }
    Check_Status(status);

    return self;
}

/**
 * Updates this Region object to the union with others.
 * @param other [GraphicsPath, Rectangle, RectangleF, Region]
 * @return [self]
 */
static VALUE
gdip_region_union(VALUE self, VALUE other)
{
    Check_Frozen(self);
    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");
    
    Status status = Ok;
    if (_KIND_OF(other, &tGraphicsPath)) {
        GraphicsPath *gp = Data_Ptr<GraphicsPath *>(other);
        Check_NULL(gp, "The GraphicsPath object of argument does not exist.");
        status = region->Union(gp);
    }
    else if (_KIND_OF(other, &tRectangle)) {
        Rect *rect = Data_Ptr<Rect *>(other);
        status = region->Union(*rect);
    }
    else if (_KIND_OF(other, &tRectangleF)) {
        RectF *rect = Data_Ptr<RectF *>(other);
        status = region->Union(*rect);
    }
    else if (_KIND_OF(other, &tRegion)) {
        Region *other_region = Data_Ptr<Region *>(other);
        Check_NULL(other_region, "The Region object of argument does not exist.");
        status = region->Union(other_region);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be GraphicsPath, Rectangle, RectangleF or Region.");
    }
    Check_Status(status);

    return self;
}

 /**
 * XOR this Region with another and Updates itself.
 * @param other [GraphicsPath, Rectangle, RectangleF, Region]
 * @return [self]
 */
static VALUE
gdip_region_xor(VALUE self, VALUE other)
{
    Check_Frozen(self);
    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");
    
    Status status = Ok;
    if (_KIND_OF(other, &tGraphicsPath)) {
        GraphicsPath *gp = Data_Ptr<GraphicsPath *>(other);
        Check_NULL(gp, "The GraphicsPath object of argument does not exist.");
        status = region->Xor(gp);
    }
    else if (_KIND_OF(other, &tRectangle)) {
        Rect *rect = Data_Ptr<Rect *>(other);
        status = region->Xor(*rect);
    }
    else if (_KIND_OF(other, &tRectangleF)) {
        RectF *rect = Data_Ptr<RectF *>(other);
        status = region->Xor(*rect);
    }
    else if (_KIND_OF(other, &tRegion)) {
        Region *other_region = Data_Ptr<Region *>(other);
        Check_NULL(other_region, "The Region object of argument does not exist.");
        status = region->Xor(other_region);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be GraphicsPath, Rectangle, RectangleF or Region.");
    }
    Check_Status(status);

    return self;
}

/**
 * Updates this Region object to the portion not intersecting with another.
 * @param other [GraphicsPath, Rectangle, RectangleF, Region]
 * @return [self]
 */
static VALUE
gdip_region_exclude(VALUE self, VALUE other)
{
    Check_Frozen(self);
    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");
    
    Status status = Ok;
    if (_KIND_OF(other, &tGraphicsPath)) {
        GraphicsPath *gp = Data_Ptr<GraphicsPath *>(other);
        Check_NULL(gp, "The GraphicsPath object of argument does not exist.");
        status = region->Exclude(gp);
    }
    else if (_KIND_OF(other, &tRectangle)) {
        Rect *rect = Data_Ptr<Rect *>(other);
        status = region->Exclude(*rect);
    }
    else if (_KIND_OF(other, &tRectangleF)) {
        RectF *rect = Data_Ptr<RectF *>(other);
        status = region->Exclude(*rect);
    }
    else if (_KIND_OF(other, &tRegion)) {
        Region *other_region = Data_Ptr<Region *>(other);
        Check_NULL(other_region, "The Region object of argument does not exist.");
        status = region->Exclude(other_region);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be GraphicsPath, Rectangle, RectangleF or Region.");
    }
    Check_Status(status);

    return self;
}

/**
 * Updates this Region object to +other.Exclude(self)+.
 * @param other [GraphicsPath, Rectangle, RectangleF, Region]
 * @return [self]
 */
static VALUE
gdip_region_complement(VALUE self, VALUE other)
{
    Check_Frozen(self);
    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");
    
    Status status = Ok;
    if (_KIND_OF(other, &tGraphicsPath)) {
        GraphicsPath *gp = Data_Ptr<GraphicsPath *>(other);
        Check_NULL(gp, "The GraphicsPath object of argument does not exist.");
        status = region->Complement(gp);
    }
    else if (_KIND_OF(other, &tRectangle)) {
        Rect *rect = Data_Ptr<Rect *>(other);
        status = region->Complement(*rect);
    }
    else if (_KIND_OF(other, &tRectangleF)) {
        RectF *rect = Data_Ptr<RectF *>(other);
        status = region->Complement(*rect);
    }
    else if (_KIND_OF(other, &tRegion)) {
        Region *other_region = Data_Ptr<Region *>(other);
        Check_NULL(other_region, "The Region object of argument does not exist.");
        status = region->Complement(other_region);
    }
    else {
        rb_raise(rb_eTypeError, "The argument should be GraphicsPath, Rectangle, RectangleF or Region.");
    }
    Check_Status(status);

    return self;
}

/**
 * Translate this Region by the specified amount.
 * @param dx [Integer or Float]
 * @param dy [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_region_translate(VALUE self, VALUE dx, VALUE dy)
{
    Check_Frozen(self);
    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");

    Status status = Ok;
    if (Integer_p(dx, dy)) {
        status = region->Translate(RB_NUM2INT(dx), RB_NUM2INT(dy));
    }
    else if (Float_p(dx, dy)) {
        status = region->Translate(NUM2SINGLE(dx), NUM2SINGLE(dy));
    }
    else {
        rb_raise(rb_eTypeError, "wrong types of arguments");
    }
    Check_Status(status);

    return self;
}

/**
 * @overload Transform(matrix)
 *   Transform this Region by the specified matrix.
 *   @param matrix [Matrix]
 *   @return [self]
 */
static VALUE
gdip_region_transform(VALUE self, VALUE v_matrix)
{
    Check_Frozen(self);

    if (!_KIND_OF(v_matrix, &tMatrix)) {
        rb_raise(rb_eTypeError, "The argument should be Matrix.");
    }

    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");
    Matrix *matrix = Data_Ptr<Matrix *>(v_matrix);
    Check_NULL(matrix, "The Matrix object of argument does not exist.");

    Status status = region->Transform(matrix);
    Check_Status(status);

    return self;
}

/**
 * Gets a rectangle circumscribing this Region.
 * @param graphics [Graphics]
 * @return [RectangleF]
 */
static VALUE
gdip_region_get_bounds(VALUE self, VALUE graphics)
{
    if (!_KIND_OF(graphics, &tGraphics)) {
        rb_raise(rb_eTypeError, "The argument should be Graphics.");
    }

    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");
    Graphics *g = Data_Ptr<Graphics *>(graphics);
    Check_NULL(g, "The Graphics object of argument does not exist.");

    RectF rect;
    Status status = region->GetBounds(&rect, g);
    Check_Status(status);

    return gdip_rectf_create(&rect);
}

static VALUE
gdip_region_is_empty(VALUE self, VALUE graphics)
{
    if (!_KIND_OF(graphics, &tGraphics)) {
        rb_raise(rb_eTypeError, "The argument should be Graphics.");
    }

    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");
    Graphics *g = Data_Ptr<Graphics *>(graphics);
    Check_NULL(g, "The Graphics object of argument does not exist.");

    return region->IsEmpty(g) ? Qtrue : Qfalse;
}

static VALUE
gdip_region_is_infinite(VALUE self, VALUE graphics)
{
    if (!_KIND_OF(graphics, &tGraphics)) {
        rb_raise(rb_eTypeError, "The argument should be Graphics.");
    }

    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");
    Graphics *g = Data_Ptr<Graphics *>(graphics);
    Check_NULL(g, "The Graphics object of argument does not exist.");

    return region->IsInfinite(g) ? Qtrue : Qfalse;
}

/**
 * Whether the specified point or rectangle is contained within this Region.
 * @overload IsVisible(x, y, graphics=nil)
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param graphics [Graphics]
 * @overload IsVisible(point, graphics=nil)
 *   @param point [Point or PointF]
 *   @param graphics [Graphics]
 * @overload IsVisible(x, y, width, height, graphics=nil)
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 *   @param graphics [Graphics]
 * @overload IsVisible(rect, graphics=nil)
 *   @param rect [Rectangle or RectangleF]
 *   @param graphics [Graphics]
 * @return [Boolean]
 */
static VALUE
gdip_region_is_visible(int argc, VALUE *argv, VALUE self)
{
    if (argc < 1 || 5 < argc) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..5)", argc);
    }

    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");

    BOOL b = false;
    if (Integer_p(argv[0])) {
        if (argc == 2 || argc == 3) {
            if (!Integer_p(argv[1])) {
                rb_raise(rb_eTypeError, "wrong types of arguments");
            }

            Graphics *g = NULL;
            if (argc == 3) {
                if (_KIND_OF(argv[2], &tGraphics)) {
                    g = Data_Ptr<Graphics *>(argv[2]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            b = region->IsVisible(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]), g);
        }
        else if (argc == 4 || argc == 5) {
            if (!Integer_p(4, argv)) {
                rb_raise(rb_eTypeError, "wrong types of arguments");
            }

            Graphics *g = NULL;
            if (argc == 5) {
                if (_KIND_OF(argv[4], &tGraphics)) {
                    g = Data_Ptr<Graphics *>(argv[4]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            b = region->IsVisible(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), g);
        }
        else {
            rb_raise(rb_eArgError, "unexpected arguments");
        }
    }
    else if (Float_p(argv[0])) {
        if (argc == 2 || argc == 3) {
            if (!Float_p(argv[1])) {
                rb_raise(rb_eTypeError, "wrong types of arguments");
            }

            Graphics *g = NULL;
            if (argc == 3) {
                if (_KIND_OF(argv[2], &tGraphics)) {
                    g = Data_Ptr<Graphics *>(argv[2]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            b = region->IsVisible(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]), g);
        }
        else if (argc == 4 || argc == 5) {
            if (!Float_p(4, argv)) {
                rb_raise(rb_eTypeError, "wrong types of arguments");
            }

            Graphics *g = NULL;
            if (argc == 5) {
                if (_KIND_OF(argv[4], &tGraphics)) {
                    g = Data_Ptr<Graphics *>(argv[4]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            b = region->IsVisible(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), g);
        }
        else {
            rb_raise(rb_eArgError, "unexpected arguments");
        }
    }
    else if (_KIND_OF(argv[0], &tPoint)) {
        if (argc == 1 || argc == 2) {
            Point *point = Data_Ptr<Point *>(argv[0]);
            Graphics *g = NULL;
            if (argc == 2) {
                if (_KIND_OF(argv[1], &tGraphics)) {
                    g = Data_Ptr<Graphics *>(argv[1]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }

            b = region->IsVisible(*point, g);
        }
        else {
            rb_raise(rb_eArgError, "unexpected arguments");
        }
    }
    else if (_KIND_OF(argv[0], &tPointF)) {
        if (argc == 1 || argc == 2) {
            PointF *point = Data_Ptr<PointF *>(argv[0]);
            Graphics *g = NULL;
            if (argc == 2) {
                if (_KIND_OF(argv[1], &tGraphics)) {
                    g = Data_Ptr<Graphics *>(argv[1]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }
            
            b = region->IsVisible(*point, g);
        }
        else {
            rb_raise(rb_eArgError, "unexpected arguments");
        }
    }
    else if (_KIND_OF(argv[0], &tRectangle)) {
        if (argc == 1 || argc == 2) {
            Rect *rect = Data_Ptr<Rect *>(argv[0]);
            Graphics *g = NULL;
            if (argc == 2) {
                if (_KIND_OF(argv[1], &tGraphics)) {
                    g = Data_Ptr<Graphics *>(argv[1]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }
            
            b = region->IsVisible(*rect, g);
        }
        else {
            rb_raise(rb_eArgError, "unexpected arguments");
        }
    }
    else if (_KIND_OF(argv[0], &tRectangleF)) {
        if (argc == 1 || argc == 2) {
            RectF *rect = Data_Ptr<RectF *>(argv[0]);
            Graphics *g = NULL;
            if (argc == 2) {
                if (_KIND_OF(argv[1], &tGraphics)) {
                    g = Data_Ptr<Graphics *>(argv[1]);
                }
                else {
                    rb_raise(rb_eTypeError, "wrong types of arguments");
                }
            }
            
            b = region->IsVisible(*rect, g);
        }
        else {
            rb_raise(rb_eArgError, "unexpected arguments");
        }
    }
    else {
        rb_raise(rb_eTypeError, "wrong types of arguments");
    }
    Check_LastStatus(region);

    return b ? Qtrue : Qfalse;
}

/**
 * Whether the specified Region is identical to this Region.
 * @return [Boolean]
 */
static VALUE
gdip_region_equals(VALUE self, VALUE v_other, VALUE graphics)
{
    if (!_KIND_OF(v_other, &tRegion)) {
        rb_raise(rb_eTypeError, "The first argument should be Region.");
    }

    if (!_KIND_OF(graphics, &tGraphics)) {
        rb_raise(rb_eTypeError, "The second argument should be Graphics");
    }

    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");

    Region *other = Data_Ptr<Region *>(v_other);
    Check_NULL(other, "The Region object of argument does not exist.");

    Graphics *g = Data_Ptr<Graphics *>(graphics);
    Check_NULL(g, "The Graphics object of argument does not exist.");

    BOOL b = region->Equals(other, g);
    Check_LastStatus(region);

    return b ? Qtrue : Qfalse;
}

/**
 * @overload GetRegionScans(matrix)
 *   @param matrix [Matrix]
 *   @return [Array<RectangleF>]
 */
static VALUE
gdip_region_get_region_scans(VALUE self, VALUE v_matrix)
{
    if (!_KIND_OF(v_matrix, &tMatrix)) {
        rb_raise(rb_eTypeError, "The first argument should be Matrix.");
    }

    Region *region = Data_Ptr<Region *>(self);
    Check_NULL(region, "This Region object does not exist.");

    Matrix *matrix = Data_Ptr<Matrix *>(v_matrix);
    Check_NULL(matrix, "The Matrix object of arugment does not exist.");

    int count = region->GetRegionScansCount(matrix);
    RectF *rects = static_cast<RectF *>(RB_ZALLOC_N(RectF, count));
    Status status = region->GetRegionScans(matrix, rects, &count);
    VALUE r = rb_ary_new_capa(count);
    for (int i = 0; i < count; ++i) {
        rb_ary_push(r, gdip_rectf_create(&rects[i]));
    }
    ruby_xfree(rects);
    Check_Status(status);

    return r;
}

void Init_region()
{
    cRegionData = rb_define_class_under(mGdiplus, "RegionData", rb_cObject);
    rb_undef_alloc_func(cRegionData);
    rb_define_method(cRegionData, "size", RUBY_METHOD_FUNC(gdip_regiondata_size), 0);

    cRegion = rb_define_class_under(mGdiplus, "Region", cGpObject);
    rb_define_alloc_func(cRegion, &typeddata_alloc_null<&tRegion>);
    rb_define_method(cRegion, "initialize", RUBY_METHOD_FUNC(gdip_region_init), -1);
    rb_define_method(cRegion, "Clone", RUBY_METHOD_FUNC(gdip_region_clone), 0);
    rb_define_alias(cRegion, "clone", "Clone");
    rb_define_alias(cRegion, "dup", "Clone");
    rb_define_method(cRegion, "MakeEmpty", RUBY_METHOD_FUNC(gdip_region_make_empty), 0);
    rb_define_alias(cRegion, "make_empty", "MakeEmpty");
    rb_define_method(cRegion, "MakeInfinite", RUBY_METHOD_FUNC(gdip_region_make_infinite), 0);
    rb_define_alias(cRegion, "make_infinite", "MakeInfinite");
    rb_define_method(cRegion, "GetData", RUBY_METHOD_FUNC(gdip_region_get_data), 0);
    rb_define_alias(cRegion, "get_data", "GetData");
    rb_define_method(cRegion, "Intersect", RUBY_METHOD_FUNC(gdip_region_intersect), 1);
    rb_define_alias(cRegion, "intersect", "Intersect");
    rb_define_method(cRegion, "Union", RUBY_METHOD_FUNC(gdip_region_union), 1);
    rb_define_alias(cRegion, "union", "Union");
    rb_define_method(cRegion, "Xor", RUBY_METHOD_FUNC(gdip_region_xor), 1);
    rb_define_alias(cRegion, "xor", "Xor");
    rb_define_method(cRegion, "Exclude", RUBY_METHOD_FUNC(gdip_region_exclude), 1);
    rb_define_alias(cRegion, "exclude", "Exclude");
    rb_define_method(cRegion, "Complement", RUBY_METHOD_FUNC(gdip_region_complement), 1);
    rb_define_alias(cRegion, "complement", "Complement");

    rb_define_method(cRegion, "Translate", RUBY_METHOD_FUNC(gdip_region_translate), 2);
    rb_define_alias(cRegion, "translate", "Translate");
    rb_define_method(cRegion, "Transform", RUBY_METHOD_FUNC(gdip_region_transform), 1);
    rb_define_alias(cRegion, "transform", "Transform");

    rb_define_method(cRegion, "GetBounds", RUBY_METHOD_FUNC(gdip_region_get_bounds), 1);
    rb_define_alias(cRegion, "get_bounds", "GetBounds");
    rb_define_method(cRegion, "IsEmpty", RUBY_METHOD_FUNC(gdip_region_is_empty), 1);
    rb_define_alias(cRegion, "IsEmpty?", "IsEmpty");
    rb_define_alias(cRegion, "is_empty?", "IsEmpty");
    rb_define_method(cRegion, "IsInfinite", RUBY_METHOD_FUNC(gdip_region_is_infinite), 1);
    rb_define_alias(cRegion, "IsInfinite?", "IsInfinite");
    rb_define_alias(cRegion, "is_infinite?", "IsInfinite");
    rb_define_method(cRegion, "IsVisible", RUBY_METHOD_FUNC(gdip_region_is_visible), -1);
    rb_define_alias(cRegion, "IsVisible?", "IsVisible");
    rb_define_alias(cRegion, "is_visible?", "IsVisible");
    rb_define_method(cRegion, "Equals", RUBY_METHOD_FUNC(gdip_region_equals), 2);
    rb_define_alias(cRegion, "Equals?", "Equals");
    rb_define_alias(cRegion, "equals?", "Equals");

    rb_define_method(cRegion, "GetRegionScans", RUBY_METHOD_FUNC(gdip_region_get_region_scans), 1);
    rb_define_alias(cRegion, "get_region_scans", "GetRegionScans");
}