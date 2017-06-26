/*
 * gdip_graphicspath.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tGraphicsPath = _MAKE_DATA_TYPE(
    "GraphicsPath", 0, GDIP_OBJ_FREE(GraphicsPath *), NULL, NULL, &cGraphicsPath);


struct gdipPathData {
public:
    int count;
    VALUE v_points;
    VALUE v_types;
};

static void
gdip_pathdata_mark(gdipPathData *pathdata)
{
    if (pathdata != NULL) {
        rb_gc_mark(pathdata->v_points);
        rb_gc_mark(pathdata->v_types);
    }
}

static VALUE cPathData;
const rb_data_type_t tPathData = _MAKE_DATA_TYPE(
    "PathData", RUBY_DATA_FUNC(gdip_pathdata_mark), GDIP_DEFAULT_FREE(gdipPathData), NULL, NULL, &cPathData);

static VALUE
gdip_pathdata_create(PathData *pathdata)
{
    VALUE r = typeddata_alloc<gdipPathData, &tPathData>(cPathData);
    gdipPathData *p = Data_Ptr<gdipPathData *>(r);
    int count = pathdata->Count;
    p->count = count;
    if (count <= 0) {
        p->v_points = rb_ary_new();
        p->v_types = rb_ary_new();
        RB_OBJ_FREEZE(p->v_points);
        RB_OBJ_FREEZE(p->v_types);
    }
    else {
        VALUE points = rb_ary_new_capa(count);
        VALUE types = rb_ary_new_capa(count);
        for (int i = 0; i < count; ++i) {
            rb_ary_push(points, gdip_pointf_create(pathdata->Points[i].X, pathdata->Points[i].Y));
            rb_ary_push(types, gdip_enumint_create(cPathPointType, pathdata->Types[i]));
        }
        RB_OBJ_FREEZE(points);
        RB_OBJ_FREEZE(types);
        p->v_points = points;
        p->v_types = types;
    }
    return r;
}

static VALUE
gdip_pathdata_get_points(VALUE self)
{
    gdipPathData *pathdata = Data_Ptr<gdipPathData *>(self);
    Check_NULL(pathdata, "This PathData object does not exist.");

    return pathdata->v_points;
}

static VALUE
gdip_pathdata_get_types(VALUE self)
{
    gdipPathData *pathdata = Data_Ptr<gdipPathData *>(self);
    Check_NULL(pathdata, "This PathData object does not exist.");

    return pathdata->v_types;
}

static VALUE
gdip_pathdata_get_count(VALUE self)
{
    gdipPathData *pathdata = Data_Ptr<gdipPathData *>(self);
    Check_NULL(pathdata, "This PathData object does not exist.");

    return RB_INT2NUM(pathdata->count);
}

static BYTE *
alloc_array_of_pptype(VALUE ary, int& count)
{
    count = 0;
    for (long i = 0; i < RARRAY_LEN(ary); ++i) {
        VALUE elem = rb_ary_entry(ary, i);
        int enumint;
        if (gdip_arg_to_enumint(cPathPointType, elem, &enumint)) {
            count += 1;
        }
    }
    if (count == 0) return NULL;

    int idx = 0;
    BYTE *tary = static_cast<BYTE *>(ruby_xcalloc(count, sizeof(BYTE)));
    for (long i = 0; i < RARRAY_LEN(ary); ++i) {
        VALUE elem = rb_ary_entry(ary, i);
        int enumint;
        if (gdip_arg_to_enumint(cPathPointType, elem, &enumint)) {
            tary[idx] = static_cast<BYTE>(enumint);
            idx += 1;
        }
    }
    return tary;
}

/**
 * @overload initialize(fill_mode=FillMode.Alternate)
 *   @param fill_mode [FillMode]
 * @overload initialize(points, types, fill_mode=FillMode.Alternate)
 *   @param points [Array<Point or PointF>]
 *   @param types [PathPointType]
 *   @param fill_mode [FillMode]
 */
static VALUE
gdip_gpath_init(int argc, VALUE *argv, VALUE self)
{
    if (argc > 3) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..3)", argc);
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    if (gp != NULL) {
        _VERBOSE("This GraphicsPath object is already initialized.");
        return self;
    }

    if (argc == 0 || argc == 1) {
        VALUE v_fill_mode = Qnil;
        rb_scan_args(argc, argv, "01", &v_fill_mode);

        int enumint = 0;
        if (!RB_NIL_P(v_fill_mode)) {
            gdip_arg_to_enumint(cFillMode, v_fill_mode, &enumint, "The argument should be FillMode.");
        }
        _DATA_PTR(self) = gdip_obj_create(new GraphicsPath(static_cast<FillMode>(enumint)));
    }
    else if (argc == 2 || argc == 3) {
        VALUE v_points;
        VALUE v_types;
        VALUE v_fill_mode = Qnil;
        rb_scan_args(argc, argv, "21", &v_points, &v_types, &v_fill_mode);

        int enumint = 0;
        if (!RB_NIL_P(v_fill_mode)) {
            gdip_arg_to_enumint(cFillMode, v_fill_mode, &enumint, "The argument should be FillMode.");
        }

        if (_RB_ARRAY_P(v_points) && _RB_ARRAY_P(v_types)) {
            if (RARRAY_LEN(v_points) <= 0) {
                rb_raise(rb_eTypeError, "The first arguments should be Array of Point or PointF.");
            }
            
            VALUE first = rb_ary_entry(v_points, 0);
            if (_KIND_OF(first, &tPoint)) {
                int cnt_points = 0;
                int cnt_types = 0;
                Point *points = alloc_array_of<Point, &tPoint>(v_points, cnt_points);
                BYTE *types = alloc_array_of_pptype(v_types, cnt_types);
                if (cnt_points == cnt_types) {
                    GraphicsPath *new_gp = new GraphicsPath(points, types, cnt_points, static_cast<FillMode>(enumint));
                    ruby_xfree(points);
                    ruby_xfree(types);
                    _DATA_PTR(self) = gdip_obj_create(new_gp);
                }
                else {
                    ruby_xfree(points);
                    ruby_xfree(types);
                    rb_raise(rb_eArgError, "The number of elements in two arrays is different. (Point: %d, PathPointType: %d)", cnt_points, cnt_types);
                }
            }
            else if (_KIND_OF(first, &tPointF)) {
                int cnt_points = 0;
                int cnt_types = 0;
                PointF *points = alloc_array_of<PointF, &tPointF>(v_points, cnt_points);
                BYTE *types = alloc_array_of_pptype(v_types, cnt_types);
                if (cnt_points == cnt_types) {
                    GraphicsPath *new_gp = new GraphicsPath(points, types, cnt_points, static_cast<FillMode>(enumint));
                    ruby_xfree(points);
                    ruby_xfree(types);
                    _DATA_PTR(self) = gdip_obj_create(new_gp);
                }
                else {
                    ruby_xfree(points);
                    ruby_xfree(types);
                    rb_raise(rb_eArgError, "The number of elements in two arrays is different. (Point: %d, PathPointType: %d)", cnt_points, cnt_types);
                }
            }
            else {
                rb_raise(rb_eTypeError, "The first arguments should be Array of Point or PointF.");
            }
        }
        else {
            rb_raise(rb_eTypeError, "The first and second arguments should be Array.");
        }
    }

    return self;
}

/**
 * Gets or sets a FillMode.
 * @return [FillMode]
 */
static VALUE
gdip_gpath_get_fill_mode(VALUE self)
{
    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");
    return gdip_enumint_create(cFillMode, gp->GetFillMode());
}

static VALUE
gdip_gpath_set_fill_mode(VALUE self, VALUE arg)
{
    Check_Frozen(self);
    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");

    int enumint = 0;
    gdip_arg_to_enumint(cFillMode, arg, &enumint, "The argument should be FillMode.");
    Status status = gp->SetFillMode(static_cast<FillMode>(enumint));
    Check_Status(status);

    return self;
}

static VALUE
gdip_gpath_get_point_count(VALUE self)
{
    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");
    return RB_INT2NUM(gp->GetPointCount());
}

static VALUE
gdip_gpath_get_path_types(VALUE self)
{
    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");

    int count = gp->GetPointCount();
    if (count < 1) {
        return rb_ary_new();
    }

    BYTE *types = static_cast<BYTE *>(RB_ZALLOC_N(BYTE, count));
    Status status = gp->GetPathTypes(types, count);
    VALUE r = rb_ary_new_capa(count);
    for (int i = 0; i < count; ++i) {
        rb_ary_push(r, gdip_enumint_create(cPathPointType, types[i]));
    }
    ruby_xfree(types);
    Check_Status(status);

    return r;
}

static VALUE
gdip_gpath_get_path_points(VALUE self)
{
    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");

    int count = gp->GetPointCount();
    if (count < 1) {
        return rb_ary_new();
    }

    PointF *points = static_cast<PointF *>(RB_ZALLOC_N(PointF, count));
    Status status = gp->GetPathPoints(points, count);
    VALUE r = rb_ary_new_capa(count);
    for (int i = 0; i < count; ++i) {
        rb_ary_push(r, gdip_pointf_create(points[i].X, points[i].Y));
    }
    ruby_xfree(points);
    Check_Status(status);

    return r;
}

static VALUE
gdip_gpath_get_path_data(VALUE self)
{
    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");

    PathData *pathdata = new PathData();
    Status status = Ok;
    if (gp->GetPointCount()) {
        status = gp->GetPathData(pathdata);
    }
    VALUE v_pathdata = gdip_pathdata_create(pathdata);
    delete pathdata;
    Check_Status(status);

    return v_pathdata;
}

/**
 * @overload AddArc(rect, start_angle, sweep_angle)
 *   @param rect [Rectangle or RectangleF]
 *   @param start_angle [Integer or Float]
 *   @param sweep_angle [Integer or Float]
 * @overload AddArc(x, y, width, height, start_angle, sweep_angle)
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 *   @param start_angle [Integer or Float]
 *   @param sweep_angle [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_gpath_add_arc(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    if (argc != 3 && argc != 6) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 3 or 6)", argc);
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");

    float start_angle;
    float sweep_angle;
    if (argc == 3 && !gdip_arg_to_single(argv[1], &start_angle)) {
        rb_raise(rb_eTypeError, "The argument of the start angle should be Integer or Float.");
    }
    if (argc == 3 && !gdip_arg_to_single(argv[2], &sweep_angle)) {
        rb_raise(rb_eTypeError, "The argument of the sweep angle should be Integer or Float.");
    }
    if (argc == 6 && !gdip_arg_to_single(argv[4], &start_angle)) {
        rb_raise(rb_eTypeError, "The argument of the start angle should be Integer or Float.");
    }
    if (argc == 6 && !gdip_arg_to_single(argv[5], &sweep_angle)) {
        rb_raise(rb_eTypeError, "The argument of the sweep angle should be Integer or Float.");
    }

    Status status = Ok;
    if (argc == 3) {
        if (_KIND_OF(argv[0], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[0]);
            status = gp->AddArc(*rect, start_angle, sweep_angle);
        }
        else if (_KIND_OF(argv[0], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[0]);
            status = gp->AddArc(*rect, start_angle, sweep_angle);
        }
        else {
            rb_raise(rb_eTypeError, "The second argument should be Rectangle or RectangleF");
        }
    }
    else if (argc == 6) {
        if (Integer_p(argv[0], argv[1], argv[2], argv[3])) {
            status = gp->AddArc(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), start_angle, sweep_angle);
        }
        else if (Float_p(argv[0], argv[1], argv[2], argv[3])) {
            status = gp->AddArc(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), start_angle, sweep_angle);
        }
        else {
            rb_raise(rb_eTypeError, "The arguments representing the position and size of the rectangle should be Integer or Float.");
        }
    }
    Check_Status(status);

    return self;
}

/**
 * @overload AddBezier(point1, point2, point3, point4)
 *   @param point1 [Point or PointF]
 *   @param point2 [Point or PointF]
 *   @param point3 [Point or PointF]
 *   @param point4 [Point or PointF]
 * @overload AddBezier(x1, y1, x2, y2, x3, y3, x4, y4)
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
gdip_gpath_add_bezier(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    if (argc != 4 && argc != 8) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 4 or 8)", argc);
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "The GraphicsPath object does not exist.");

    Status status = Ok;
    if (argc == 4) {
        if (Typeddata_p(3, &argv[0], &tPoint)) {
            Point *po1 = Data_Ptr<Point *>(argv[0]);
            Point *po2 = Data_Ptr<Point *>(argv[1]);
            Point *po3 = Data_Ptr<Point *>(argv[2]);
            Point *po4 = Data_Ptr<Point *>(argv[3]);
            status = gp->AddBezier(*po1, *po2, *po3, *po4);
        }
        else if (Typeddata_p(3, &argv[0], &tPointF)) {
            PointF *po1 = Data_Ptr<PointF *>(argv[0]);
            PointF *po2 = Data_Ptr<PointF *>(argv[1]);
            PointF *po3 = Data_Ptr<PointF *>(argv[2]);
            PointF *po4 = Data_Ptr<PointF *>(argv[3]);
            status = gp->AddBezier(*po1, *po2, *po3, *po4);
        }
        else {
            rb_raise(rb_eTypeError, "The argument representing points should be Point or PointF.");
        }
    }
    else if (argc == 8) {
        if (Integer_p(8, argv)) {
            status = gp->AddBezier(
                RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), 
                RB_NUM2INT(argv[4]), RB_NUM2INT(argv[5]), RB_NUM2INT(argv[6]), RB_NUM2INT(argv[7]));
        }
        else if (Float_p(8, argv)) {
            status = gp->AddBezier(
                NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), 
                NUM2SINGLE(argv[4]), NUM2SINGLE(argv[5]), NUM2SINGLE(argv[6]), NUM2SINGLE(argv[7]));
        }
        else {
            rb_raise(rb_eTypeError, "The argument representing the coordinates of points should be Integer or Float.");
        }
    }
    Check_Status(status);

    return self;
}

/**
 * @overload AddBeziers(points)
 *   @param points [Array<Point or PointF>] The number of points should be 1 + 3n (n >= 1: 4, 7, 10, 13, ...).
 *   @return [self]
 */
static VALUE
gdip_gpath_add_beziers(VALUE self, VALUE ary)
{
    Check_Frozen(self);
    if (!_RB_ARRAY_P(ary) || RARRAY_LEN(ary) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "The GraphicsPath object does not exist.");
    VALUE first = rb_ary_entry(ary, 0);

    if (_KIND_OF(first, &tPoint)) {
        int count;
        Point *points = alloc_array_of<Point, &tPoint>(ary, count);
        if ((count - 1) % 3 == 0) {
            Status status = gp->AddBeziers(points, count);
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
            Status status = gp->AddBeziers(points, count);
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
 * @overload AddClosedCurve(points, tension=0.5)
 *   @param points [Array<Point or PointF>]
 *   @param tension [Float] 0.0-1.0
 *   @return [self]
 */
static VALUE
gdip_gpath_add_closed_curve(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    VALUE v_ary;
    VALUE v_tension = Qnil;
    rb_scan_args(argc, argv, "11", &v_ary, &v_tension);

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "The GraphicsPath object does not exist.");
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
            Status status = gp->AddClosedCurve(points, count, tension);
            ruby_xfree(points);
            Check_Status(status);
        }
        else if (_KIND_OF(first, &tPointF)) {
            int count;
            PointF *points = alloc_array_of<PointF, &tPointF>(v_ary, count);
            Status status = gp->AddClosedCurve(points, count, tension);
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
 * @overload AddCurve(points, tension=0.5)
 *   @param points [Array<Point or PointF>]
 *   @param tension [Float] 0.0-1.0
 * @overload AddCurve(points, offset, num, tension=0.5)
 *   @param points [Array<Point or PointF>]
 *   @param offset [Integer] start index of points
 *   @param num [Integer] number of segments
 *   @param tension [Float] 0.0-1.0
 * @return [self]
 */
static VALUE
gdip_gpath_add_curve(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    if (argc < 1 || 4 < argc) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..4)", argc);
    }
    if (!_RB_ARRAY_P(argv[0]) || RARRAY_LEN(argv[0]) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    if (argc == 2 && !Float_p(argv[1])) {
        rb_raise(rb_eTypeError, "The last argument should be Float.");
    }
    else if (argc == 3 && !Integer_p(argv[1], argv[2])) {
        rb_raise(rb_eTypeError, "The arguments should be Integer.");
    }
    else if (argc == 4) {
        if (!Float_p(argv[3])) {
            rb_raise(rb_eTypeError, "The last argument should be Float.");
        }
        else if (!Integer_p(argv[1], argv[2])) {
            rb_raise(rb_eTypeError, "The arguments should be Integer.");
        }
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "The GraphicsPath object does not exist.");

    VALUE first = rb_ary_entry(argv[0], 0);
    int offset = 0;
    int num = 0;
    if (argc > 2) {
        offset = RB_NUM2INT(argv[1]);
        num = RB_NUM2INT(argv[2]);
    }

    float tension = 0.5f;
    if (argc == 2) {
        tension = clamp(NUM2SINGLE(argv[1]), 0.0f, 1.0f);
    }

    if (argc == 4) {
        tension = clamp(NUM2SINGLE(argv[3]), 0.0f, 1.0f);
    }

    if (_KIND_OF(first, &tPoint)) {
        int count;
        Point *points = alloc_array_of<Point, &tPoint>(argv[0], count);
        Status status;
        if (argc > 2) {
            status = gp->AddCurve(points, count, offset, num, tension);
        }
        else {
            status = gp->AddCurve(points, count, tension);
        }
        ruby_xfree(points);
        Check_Status(status);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count;
        PointF *points = alloc_array_of<PointF, &tPointF>(argv[0], count);
        Status status;
        if (argc > 2) {
            status = gp->AddCurve(points, count, offset, num, tension);
        }
        else {
            status = gp->AddCurve(points, count, tension);
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
 * @overload AddEllipse(rectangle)
 *   @param rectangle [Rectangle or RectangleF]
 * @overload AddEllipse(x, y, width, height)
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_gpath_add_ellipse(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    if (argc != 1 && argc != 4) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1 or 4)", argc);
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "The GraphicsPath object does not exist.");

    if (argc == 1) {
        if (_KIND_OF(argv[0], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[0]);
            gp->AddEllipse(*rect);
        }
        else if (_KIND_OF(argv[0], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[0]);
            gp->AddEllipse(*rect);
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Rectangle or RectangleF.");
        }

    }
    else if (argc == 4) {
        if (Integer_p(argv[0], argv[1], argv[2], argv[3])) {
            gp->AddEllipse(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]));
        }
        else if (Float_p(argv[0], argv[1], argv[2], argv[3])) {
            gp->AddEllipse(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer or Float.");
        }
    }
    
    return self;
}

/**
 * @overload AddLine(point1, point2)
 *   @param point1 [Point or PointF]
 *   @param point2 [Point or PointF]
 * @overload AddLine(x1, y1, x2, y2)
 *   @param x1 [Integer or Float]
 *   @param y1 [Integer or Float]
 *   @param x2 [Integer or Float]
 *   @param y2 [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_gpath_add_line(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    if (argc != 2 && argc != 4) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 2 or 4)", argc);
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "The GraphicsPath object does not exist.");

    if (argc == 2) {
        if (_KIND_OF(argv[0], &tPoint) && _KIND_OF(argv[1], &tPoint)) {
            Point *po1 = Data_Ptr<Point *>(argv[0]);
            Point *po2 = Data_Ptr<Point *>(argv[1]);
            gp->AddLine(po1->X, po1->Y, po2->X, po2->Y);
        }
        else if (_KIND_OF(argv[0], &tPointF) && _KIND_OF(argv[1], &tPointF)) {
            PointF *po1 = Data_Ptr<PointF *>(argv[0]);
            PointF *po2 = Data_Ptr<PointF *>(argv[1]);
            gp->AddLine(po1->X, po1->Y, po2->X, po2->Y);
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Point or PointF.");
        }
    }
    else if (argc == 4) {
        if (Integer_p(argv[0], argv[1], argv[2], argv[3])) {
            gp->AddLine(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]));
        }
        else if (Float_p(argv[0], argv[1], argv[2], argv[3])) {
            gp->AddLine(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer or Float.");
        }
    }

    return self;
}

/**
 * @overload AddLines(points)
 *   @param points [Array<Point or PointF>]
 *   @return [self]
 */
static VALUE
gdip_gpath_add_lines(VALUE self, VALUE ary)
{
    Check_Frozen(self);
    if (!_RB_ARRAY_P(ary) && RARRAY_LEN(ary) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "The GraphicsPath object does not exist.");

    VALUE first = rb_ary_entry(ary, 0);
    if (_KIND_OF(first, &tPoint)) {
        int count;
        Point *points = alloc_array_of<Point, &tPoint>(ary, count);
        Status status = gp->AddLines(points, count);
        ruby_xfree(points);
        Check_Status(status);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count;
        PointF *points = alloc_array_of<PointF, &tPointF>(ary, count);
        Status status = gp->AddLines(points, count);
        ruby_xfree(points);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    
    return self;
}

/**
 * @overload AddPie(rect, start_angle, sweep_angle)
 *   @param rect [Rectangle or RectangleF]
 *   @param start_angle [Integer or Float]
 *   @param sweep_angle [Integer or Float]
 * @overload AddPie(x, y, width, height, start_angle, sweep_angle)
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 *   @param start_angle [Integer or Float]
 *   @param sweep_angle [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_gpath_add_pie(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    if (argc != 3 && argc != 6) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 3 or 6)", argc);
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "The GraphicsPath object does not exist.");

    float start_angle;
    float sweep_angle;
    if (argc == 3 && !gdip_arg_to_single(argv[1], &start_angle)) {
        rb_raise(rb_eTypeError, "The argument of the start angle should be Integer or Float.");
    }
    if (argc == 3 && !gdip_arg_to_single(argv[2], &sweep_angle)) {
        rb_raise(rb_eTypeError, "The argument of the sweep angle should be Integer or Float.");
    }
    if (argc == 6 && !gdip_arg_to_single(argv[4], &start_angle)) {
        rb_raise(rb_eTypeError, "The argument of the start angle should be Integer or Float.");
    }
    if (argc == 6 && !gdip_arg_to_single(argv[5], &sweep_angle)) {
        rb_raise(rb_eTypeError, "The argument of the sweep angle should be Integer or Float.");
    }

    Status status = Ok;
    if (argc == 3) {
        if (_KIND_OF(argv[0], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[0]);
            status = gp->AddPie(*rect, start_angle, sweep_angle);
        }
        else if (_KIND_OF(argv[0], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[0]);
            status = gp->AddPie(*rect, start_angle, sweep_angle);
        }
        else {
            rb_raise(rb_eTypeError, "The second argument should be Rectangle or RectangleF");
        }
    }
    else if (argc == 6) {
        if (Integer_p(argv[0], argv[1], argv[2], argv[3])) {
            status = gp->AddPie(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]), start_angle, sweep_angle);
        }
        else if (Float_p(argv[0], argv[1], argv[2], argv[3])) {
            status = gp->AddPie(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), start_angle, sweep_angle);
        }
        else {
            rb_raise(rb_eTypeError, "The arguments representing the position and size of the rectangle should be Integer or Float.");
        }
    }
    Check_Status(status);

    return self;
}

/**
 * @overload AddPolygon(points)
 *   @param points [Array<Point or PointF>]
 *   @return [self]
 */
static VALUE
gdip_gpath_add_polygon(VALUE self, VALUE ary)
{
    Check_Frozen(self);
    if (!_RB_ARRAY_P(ary) && RARRAY_LEN(ary) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "The GraphicsPath object does not exist.");

    VALUE first = rb_ary_entry(ary, 0);
    if (_KIND_OF(first, &tPoint)) {
        int count;
        Point *points = alloc_array_of<Point, &tPoint>(ary, count);
        Status status = gp->AddPolygon(points, count);
        ruby_xfree(points);
        Check_Status(status);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count;
        PointF *points = alloc_array_of<PointF, &tPointF>(ary, count);
        Status status = gp->AddPolygon(points, count);
        ruby_xfree(points);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Point or PointF.");
    }
    
    return self;
}

/**
 * @overload AddRectangle(rectangle)
 *   @param rectangle [Rectangle or RectangleF]
 * @overload AddRectangle(x, y, width, height)
 *   @param x [Integer or Float]
 *   @param y [Integer or Float]
 *   @param width [Integer or Float]
 *   @param height [Integer or Float]
 * @return [self]
 */
static VALUE
gdip_gpath_add_rectangle(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    if (argc != 1 && argc != 4) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1 or 4)", argc);
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "The GraphicsPath object does not exist.");

    if (argc == 1) {
        if (_KIND_OF(argv[0], &tRectangle)) {
            Rect *rect = Data_Ptr<Rect *>(argv[0]);
            gp->AddRectangle(*rect);
        }
        else if (_KIND_OF(argv[0], &tRectangleF)) {
            RectF *rect = Data_Ptr<RectF *>(argv[0]);
            gp->AddRectangle(*rect);
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Rectangle or RectangleF.");
        }

    }
    else if (argc == 4) {
        if (Integer_p(argv[0], argv[1], argv[2], argv[3])) {
            Rect rect(RB_NUM2INT(argv[0]), RB_NUM2INT(argv[1]), RB_NUM2INT(argv[2]), RB_NUM2INT(argv[3]));
            gp->AddRectangle(rect);
        }
        else if (Float_p(argv[0], argv[1], argv[2], argv[3])) {
            RectF rect(NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]), NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]));
            gp->AddRectangle(rect);
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Integer or Float.");
        }
    }
    
    return self;
}

/**
 * @overload AddRectangles(rectangles)
 *   @param rectangles [Array<Rectangle or RectangleF>]
 *   @return [self]
 */
static VALUE
gdip_gpath_add_rectangles(VALUE self, VALUE ary)
{
    Check_Frozen(self);
    if (!_RB_ARRAY_P(ary) && RARRAY_LEN(ary) == 0) {
        rb_raise(rb_eTypeError, "The second argument should be Array of Rectangle or RectangleF.");
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "The GraphicsPath object does not exist.");

    VALUE first = rb_ary_entry(ary, 0);
    if (_KIND_OF(first, &tRectangle)) {
        int count;
        Rect *rects = alloc_array_of<Rect, &tRectangle>(ary, count);
        Status status = gp->AddRectangles(rects, count);
        ruby_xfree(rects);
        Check_Status(status);
    }
    else if (_KIND_OF(first, &tRectangleF)) {
        int count;
        RectF *rects = alloc_array_of<RectF, &tRectangleF>(ary, count);
        Status status = gp->AddRectangles(rects, count);
        ruby_xfree(rects);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be Array of Rectangle or RectangleF.");
    }

    return self;
}

/**
 * @overload AddString(str, family, style, size, origin, format)
 *   @param str [String]
 *   @param family [FontFamily]
 *   @param style [FontStyle]
 *   @param size [Float or Integer]
 *   @param origin [Point or PointF]
 *   @param format [StringFormat]
 * @overload AddString(str, family, style, size, rect, format)
 *   @param str [String]
 *   @param family [FontFamily]
 *   @param style [FontStyle]
 *   @param size [Float or Integer]
 *   @param rect [Rectangle or RectangleF]
 *   @param format [StringFormat]
 * @return [self]
 */
static VALUE
gdip_gpath_add_string(VALUE self, VALUE str, VALUE v_family, VALUE v_style, VALUE v_size, VALUE point_or_rect, VALUE v_format)
{
    Check_Frozen(self);
    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");

    if (!_RB_STRING_P(str)) {
        rb_raise(rb_eTypeError, "The first argument should be String.");
    }
    if (!_KIND_OF(v_family, &tFontFamily)) {
        rb_raise(rb_eTypeError, "The second argument should be FontFamily.");
    }

    int style = 0;
    gdip_arg_to_enumint(cFontStyle, v_style, &style, "The third argument should be FontStyle.");
    float size = 12.0f;
    gdip_arg_to_single(v_size, &size, "The fourth argument should be Float or Integer.");
    if (!_KIND_OF(v_format, &tStringFormat)) {
        rb_raise(rb_eTypeError, "The sixth argument should be StringFormat.");
    }

    FontFamily *family = Data_Ptr<FontFamily *>(v_family);
    Check_NULL(family, "This FontFamily object does not exist.");
    StringFormat *format = Data_Ptr<StringFormat *>(v_format);
    Check_NULL(format, "This StringFormat object does not exist.");
    VALUE wstr = util_utf16_str_new(str);

    Status status = Ok;
    if (_KIND_OF(point_or_rect, &tPoint)) {
        Point *point = Data_Ptr<Point *>(point_or_rect);
        status = gp->AddString(RString_Ptr<WCHAR *>(wstr), -1, family, style, size, *point, format);
    }
    else if (_KIND_OF(point_or_rect, &tPointF)) {
        PointF *point = Data_Ptr<PointF *>(point_or_rect);
        status = gp->AddString(RString_Ptr<WCHAR *>(wstr), -1, family, style, size, *point, format);
    }
    else if (_KIND_OF(point_or_rect, &tRectangle)) {
        Rect *rect = Data_Ptr<Rect *>(point_or_rect);
        status = gp->AddString(RString_Ptr<WCHAR *>(wstr), -1, family, style, size, *rect, format);
    }
    else if (_KIND_OF(point_or_rect, &tRectangleF)) {
        RectF *rect = Data_Ptr<RectF *>(point_or_rect);
        status = gp->AddString(RString_Ptr<WCHAR *>(wstr), -1, family, style, size, *rect, format);
    }
    else {
        rb_raise(rb_eTypeError, "The fifth argument should be Point, PointF, Rectangle or RectangleF.");
    }
    Check_Status(status);
    
    return self;
}

/**
 * @overload AddPath(path, connect=false)
 *   @param path [GraphicsPath]
 *   @param connect [Boolean]
 *   @return [self]
 */
static VALUE
gdip_gpath_add_path(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    VALUE v_path;
    VALUE v_connect;
    rb_scan_args(argc, argv, "11", &v_path, &v_connect);
    if (!_KIND_OF(v_path, &tGraphicsPath)) {
        rb_raise(rb_eTypeError, "The first argument should be GraphicsPath.");
    }

    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");
    GraphicsPath *path = Data_Ptr<GraphicsPath *>(argv[0]);
    Check_NULL(path, "This GraphicsPath object does not exist.");

    Status status = gp->AddPath(path, RB_TEST(v_connect));
    Check_Status(status);

    return self;
}

/**
 * Clears all markers.
 * @return [self]
 */
static VALUE
gdip_gpath_clear_markers(VALUE self)
{
    Check_Frozen(self);
    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");
    
    Status status = gp->ClearMarkers();
    Check_Status(status);
    return self;
}

/**
 * Closes all figures.
 * @return [self]
 */
static VALUE
gdip_gpath_close_all_figures(VALUE self)
{
    Check_Frozen(self);
    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");
    
    Status status = gp->CloseAllFigures();
    Check_Status(status);
    return self;
}

/**
 * Closes the current figure.
 * @return [self]
 */
static VALUE
gdip_gpath_close_figure(VALUE self)
{
    Check_Frozen(self);
    GraphicsPath *gp = Data_Ptr<GraphicsPath *>(self);
    Check_NULL(gp, "This GraphicsPath object does not exist.");
    
    Status status = gp->CloseFigure();
    Check_Status(status);
    return self;
}

void
Init_graphicspath()
{
    cPathData = rb_define_class_under(mGdiplus, "PathData", cGpObject);
    rb_undef_alloc_func(cPathData);
    ATTR_R(cPathData, Points, points, pathdata);
    ATTR_R(cPathData, Types, types, pathdata);
    ATTR_R(cPathData, Count, count, pathdata);

    cGraphicsPath = rb_define_class_under(mGdiplus, "GraphicsPath", cGpObject);
    rb_define_alloc_func(cGraphicsPath, &typeddata_alloc_null<&tGraphicsPath>);
    rb_define_method(cGraphicsPath, "initialize", RUBY_METHOD_FUNC(gdip_gpath_init), -1);
    ATTR_RW(cGraphicsPath, FillMode, fill_mode, gpath);
    ATTR_R(cGraphicsPath, PointCount, point_count, gpath);
    ATTR_R(cGraphicsPath, PathTypes, path_types, gpath);
    ATTR_R(cGraphicsPath, PathPoints, path_points, gpath);
    ATTR_R(cGraphicsPath, PathData, path_data, gpath);
    
    rb_define_method(cGraphicsPath, "AddArc", RUBY_METHOD_FUNC(gdip_gpath_add_arc), -1);
    rb_define_alias(cGraphicsPath, "add_arc", "AddArc");
    rb_define_method(cGraphicsPath, "AddBezier", RUBY_METHOD_FUNC(gdip_gpath_add_bezier), -1);
    rb_define_alias(cGraphicsPath, "add_bezier", "AddBezier");
    rb_define_method(cGraphicsPath, "AddBeziers", RUBY_METHOD_FUNC(gdip_gpath_add_beziers), 1);
    rb_define_alias(cGraphicsPath, "add_beziers", "AddBeziers");
    rb_define_method(cGraphicsPath, "AddClosedCurve", RUBY_METHOD_FUNC(gdip_gpath_add_closed_curve), -1);
    rb_define_alias(cGraphicsPath, "add_closed_curve", "AddClosedCurve");
    rb_define_method(cGraphicsPath, "AddCurve", RUBY_METHOD_FUNC(gdip_gpath_add_curve), -1);
    rb_define_alias(cGraphicsPath, "add_curve", "AddCurve");
    rb_define_method(cGraphicsPath, "AddEllipse", RUBY_METHOD_FUNC(gdip_gpath_add_ellipse), -1);
    rb_define_alias(cGraphicsPath, "add_ellipse", "AddEllipse");
    rb_define_method(cGraphicsPath, "AddLine", RUBY_METHOD_FUNC(gdip_gpath_add_line), -1);
    rb_define_alias(cGraphicsPath, "add_line", "AddLine");
    rb_define_method(cGraphicsPath, "AddLines", RUBY_METHOD_FUNC(gdip_gpath_add_lines), 1);
    rb_define_alias(cGraphicsPath, "add_lines", "AddLines");
    rb_define_method(cGraphicsPath, "AddPie", RUBY_METHOD_FUNC(gdip_gpath_add_pie), -1);
    rb_define_alias(cGraphicsPath, "add_pie", "AddPie");
    rb_define_method(cGraphicsPath, "AddPolygon", RUBY_METHOD_FUNC(gdip_gpath_add_polygon), 1);
    rb_define_alias(cGraphicsPath, "add_polygon", "AddPolygon");
    rb_define_method(cGraphicsPath, "AddRectangle", RUBY_METHOD_FUNC(gdip_gpath_add_rectangle), -1);
    rb_define_alias(cGraphicsPath, "add_rectangle", "AddRectangle");
    rb_define_method(cGraphicsPath, "AddRectangles", RUBY_METHOD_FUNC(gdip_gpath_add_rectangles), 1);
    rb_define_alias(cGraphicsPath, "add_rectangles", "AddRectangles");
    rb_define_method(cGraphicsPath, "AddString", RUBY_METHOD_FUNC(gdip_gpath_add_string), 6);
    rb_define_alias(cGraphicsPath, "add_string", "AddString");
    rb_define_method(cGraphicsPath, "AddPath", RUBY_METHOD_FUNC(gdip_gpath_add_path), -1);
    rb_define_alias(cGraphicsPath, "add_path", "AddPath");

    rb_define_method(cGraphicsPath, "ClearMarkers", RUBY_METHOD_FUNC(gdip_gpath_clear_markers), 0);
    rb_define_alias(cGraphicsPath, "clear_markers", "ClearMarkers");
    rb_define_method(cGraphicsPath, "CloseAllFigures", RUBY_METHOD_FUNC(gdip_gpath_close_all_figures), 0);
    rb_define_alias(cGraphicsPath, "close_all_figures", "CloseAllFigures");
    rb_define_method(cGraphicsPath, "CloseFigure", RUBY_METHOD_FUNC(gdip_gpath_close_figure), 0);
    rb_define_alias(cGraphicsPath, "close_figure", "CloseFigure");
}