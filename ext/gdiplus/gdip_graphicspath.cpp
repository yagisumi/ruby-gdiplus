/*
 * gdip_graphicspath.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tGraphicsPath = _MAKE_DATA_TYPE(
    "GraphicsPath", 0, GDIP_OBJ_FREE(GraphicsPath *), NULL, NULL, &cGraphicsPath);

static VALUE cPathData;
static const rb_data_type_t tPathData = _MAKE_DATA_TYPE(
    "PathData", 0, GDIP_OBJ_FREE(PathData *), NULL, NULL, &cPathData);

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
        _VERBOSE("object already initialized");
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

        rb_raise(rb_eNotImpError, "!!!!!!!!!!!!!!!!!!!!!!!");
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

void
Init_graphicspath()
{
    cPathData = rb_define_class_under(mGdiplus, "PathData", cGpObject);
    rb_undef_alloc_func(cPathData);


    cGraphicsPath = rb_define_class_under(mGdiplus, "GraphicsPath", cGpObject);
    rb_define_alloc_func(cGraphicsPath, &typeddata_alloc_null<&tGraphicsPath>);
    rb_define_method(cGraphicsPath, "initialize", RUBY_METHOD_FUNC(gdip_gpath_init), -1);
    ATTR_RW(cGraphicsPath, FillMode, fill_mode, gpath);
    ATTR_R(cGraphicsPath, PointCount, point_count, gpath);

}