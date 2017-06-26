/*
 * gdip_matrix.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tMatrix = _MAKE_DATA_TYPE(
    "Matrix", 0, GDIP_OBJ_FREE(Matrix *), NULL, NULL, &cMatrix);

static VALUE
gdip_matrix_create(Matrix *matrix)
{
    VALUE v_matrix = typeddata_alloc_null<&tMatrix>(cMatrix);
    _DATA_PTR(v_matrix) = gdip_obj_create(matrix);
    return v_matrix;
}

/**
 * @overload initialize()
 * @overload initialize(rect, points)
 *   @param rect [Rectangle]
 *   @param points [Array<Point>] the upper-left, upper-right, and lower-left corners of the rectangle
 * @overload initialize(rect, points)
 *   @param rect [RectangleF]
 *   @param points [Array<PointF>] the upper-left, upper-right, and lower-left corners of the rectangle
 * @overload initialize(m11, m12, m21, m22, dx, dy)
 *   @param m11 [Float]
 *   @param m12 [Float]
 *   @param m21 [Float]
 *   @param m22 [Float]
 *   @param dx [Float]
 *   @param dy [Float]
 */
static VALUE
gdip_matrix_init(int argc, VALUE *argv, VALUE self)
{
    if (_DATA_PTR(self) != NULL) {
        _VERBOSE("This Matrix object is aloready initialized.");
        return self;
    }

    if (argc != 0 && argc != 2 && argc != 6) {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0, 2 or 6)", argc);
    }

    if (argc == 0) {
        _DATA_PTR(self) = gdip_obj_create(new Matrix());
    }
    else if (argc == 2) {
        if (_KIND_OF(argv[0], &tRectangle)) {
            int count = 0;
            Point *points = alloc_array_of<Point, &tPoint>(argv[1], count);
            if (count >= 3) {
                Rect *rect = Data_Ptr<Rect *>(argv[0]);
                Matrix *matrix = new Matrix(*rect, points);
                ruby_xfree(points);
                _DATA_PTR(self) = gdip_obj_create(matrix);
            }
            else {
                ruby_xfree(points);
                rb_raise(rb_eArgError, "The second argument should be Array with three Point.");
            }
        }
        else if (_KIND_OF(argv[0], &tRectangleF)) {
            int count = 0;
            PointF *points = alloc_array_of<PointF, &tPointF>(argv[1], count);
            if (count >= 3) {
                RectF *rect = Data_Ptr<RectF *>(argv[0]);
                Matrix *matrix = new Matrix(*rect, points);
                ruby_xfree(points);
                _DATA_PTR(self) = gdip_obj_create(matrix);
            }
            else {
                ruby_xfree(points);
                rb_raise(rb_eArgError, "The second argument should be Array with three Point.");
            }
        }
        else {
            rb_raise(rb_eTypeError, "The first argument should be Rectangle or RectangleF.");
        }
    }
    else if (argc == 6) {
        if (Float_p(6, argv)) {
            _DATA_PTR(self) = gdip_obj_create(new Matrix(
                    NUM2SINGLE(argv[0]), NUM2SINGLE(argv[1]), 
                    NUM2SINGLE(argv[2]), NUM2SINGLE(argv[3]), 
                    NUM2SINGLE(argv[4]), NUM2SINGLE(argv[5]) ));
        }
        else {
            rb_raise(rb_eTypeError, "The arguments should be Float.");
        }
    }

    return self;
}

static VALUE
gdip_matrix_clone(VALUE self)
{
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    return gdip_matrix_create(matrix->Clone());
}

/**
 * Gets the elements of this Matrix.
 * @return [Array<Float>]
 */
static VALUE
gdip_matrix_get_elements(VALUE self)
{
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    float elems[6];
    Status status = matrix->GetElements(elems);
    Check_Status(status);

    VALUE r = rb_ary_new_capa(6);
    for (int i = 0; i < 6; ++i) {
        rb_ary_push(r, SINGLE2NUM(elems[i]));
    }

    return r;
}

/**
 * Gets whether this matrix is identity matrix.
 * @return [Boolean]
 */
static VALUE
gdip_matrix_get_is_identity(VALUE self)
{
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    return matrix->IsIdentity() ? Qtrue : Qfalse;
}

/**
 * Gets whether this matrix is invertible.
 * @return [Boolean]
 */
static VALUE
gdip_matrix_get_is_invertible(VALUE self)
{
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    return matrix->IsInvertible() ? Qtrue : Qfalse;
}

/**
 * Gets the dx value.
 * @return [Float]
 */
static VALUE
gdip_matrix_get_offset_x(VALUE self)
{
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    return SINGLE2NUM(matrix->OffsetX());
}

/**
 * Gets the dy value.
 * @return [Float]
 */
static VALUE
gdip_matrix_get_offset_y(VALUE self)
{
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    return SINGLE2NUM(matrix->OffsetY());
}

/**
 * Compares with another matrix.
 * @return [Boolean]
 */
static VALUE
gdip_matrix_equal(VALUE self, VALUE other)
{
    if (self == other) return true;
    else if (_KIND_OF(other, &tMatrix)) {
        Matrix *matrix_self = Data_Ptr<Matrix *>(self);
        Check_NULL(matrix_self, "This Matrix object does not exist.");

        Matrix *matrix_other = Data_Ptr<Matrix *>(other);
        Check_NULL(matrix_other, "The argument Matrix object does not exist.");

        return matrix_self->Equals(matrix_other) ? Qtrue : Qfalse;
    }
    else return false;
}

/**
 * Multiplies two matrices.
 * @return [self]
 */
static VALUE
gdip_matrix_multiply(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    Matrix *matrix_self = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix_self, "This Matrix object does not exist.");

    VALUE v_other;
    VALUE v_order;
    rb_scan_args(argc, argv, "11", &v_other, &v_order);

    MatrixOrder order = MatrixOrderPrepend;
    if (!RB_NIL_P(v_order)) {
        gdip_arg_to_enumint(cMatrixOrder, v_order, &order, "The second argument should be MatrixOrder.");
    }

    if (_KIND_OF(v_other, &tMatrix)) {
        Matrix *matrix_other = Data_Ptr<Matrix *>(v_other);
        Check_NULL(matrix_other, "The argument Matrix object does not exist.");

        Status status = matrix_self->Multiply(matrix_other, order);
        Check_Status(status);
    }
    else {
        rb_raise(rb_eTypeError, "The first argument should be Matrix.");
    }

    return self;
}

/**
 * Inverts this Matrix.
 * @return [self]
 */
static VALUE
gdip_matrix_invert(VALUE self)
{
    Check_Frozen(self);
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    Check_Status(matrix->Invert());
    return self;
}

/**
 * Resets this matrix to identity matrix.
 * @return [self]
 */
static VALUE
gdip_matrix_reset(VALUE self)
{
    Check_Frozen(self);
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    Check_Status(matrix->Reset());
    return self;
}

/**
 * Rotates by the specified angle.
 * @overload Rotate(angle, order=MatrixOrder.Prepend)
 *   @param angle [Float] The rotation angle, in degrees.
 *   @param order [MatrixOrder]
 *   @return [self]
 */
static VALUE
gdip_matrix_rotate(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    VALUE v_angle;
    VALUE v_order;
    rb_scan_args(argc, argv, "11", &v_angle, &v_order);

    float angle = 0.0f;
    gdip_arg_to_single(v_angle, &angle, "The argument should be Float or Integer.");

    MatrixOrder order = MatrixOrderPrepend;
    if (!RB_NIL_P(v_order)) {
        gdip_arg_to_enumint(cMatrixOrder, v_order, &order, "The second argument should be MatrixOrder.");
    }

    Status status = matrix->Rotate(angle, order);
    Check_Status(status);

    return self;
}

/**
 * Rotates by the specified angle around the given point.
 * @overload RotateAt(angle, point, order=MatrixOrder.Prepend)
 *   @param angle [Float] The rotation angle, in degrees.
 *   @param point [PointF or Point]
 *   @param order [MatrixOrder]
 *   @return [self]
 */
static VALUE
gdip_matrix_rotate_at(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    VALUE v_angle;
    VALUE v_point;
    VALUE v_order;
    rb_scan_args(argc, argv, "21", &v_angle, &v_point, &v_order);

    float angle = 0.0f;
    gdip_arg_to_single(v_angle, &angle, "The argument should be Float or Integer.");

    MatrixOrder order = MatrixOrderPrepend;
    if (!RB_NIL_P(v_order)) {
        gdip_arg_to_enumint(cMatrixOrder, v_order, &order, "The second argument should be MatrixOrder.");
    }

    Status status = Ok;
    if (_KIND_OF(v_point, &tPointF)) {
        PointF *point = Data_Ptr<PointF *>(v_point);
        status = matrix->RotateAt(angle, *point, order);
    }
    else if (_KIND_OF(v_point, &tPoint)) {
        Point *point_i = Data_Ptr<Point *>(v_point);
        PointF point;
        point.X = static_cast<float>(point_i->X);
        point.Y = static_cast<float>(point_i->Y);
        status = matrix->RotateAt(angle, point, order);
    }
    else {
        rb_raise(rb_eTypeError, "The second argument should be PointF or Point.");
    }
    Check_Status(status);

    return self;
}

/**
 * Scales to the specified scale.
 * @overload Scale(scale_x, scale_y, order=MatrixOrder.Prepend)
 *   @param scale_x [Float]
 *   @param scale_y [Float]
 *   @param order [MatrixOrder]
 *   @return [self]
 */
static VALUE
gdip_matrix_scale(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    VALUE v_scale_x, v_scale_y, v_order;
    rb_scan_args(argc, argv, "21", &v_scale_x, &v_scale_y, &v_order);

    float scale_x = 0.0f;
    float scale_y = 0.0f;
    gdip_arg_to_single(v_scale_x, &scale_x, "The first argument should be Float or Integer.");
    gdip_arg_to_single(v_scale_y, &scale_y, "The second argument should be Float or Integer.");

    MatrixOrder order = MatrixOrderPrepend;
    if (!RB_NIL_P(v_order)) {
        gdip_arg_to_enumint(cMatrixOrder, v_order, &order, "The second argument should be MatrixOrder.");
    }

    Status status = matrix->Scale(scale_x, scale_y, order);
    Check_Status(status);

    return self;
}

/**
 * Shears.
 * @overload Shear(shear_x, shear_y, order=MatrixOrder.Prepend)
 *   @param shear_x [Float]
 *   @param shear_y [Float]
 *   @param order [MatrixOrder]
 *   @return [self]
 */
static VALUE
gdip_matrix_shear(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    VALUE v_shear_x, v_shear_y, v_order;
    rb_scan_args(argc, argv, "21", &v_shear_x, &v_shear_y, &v_order);

    float shear_x = 0.0f;
    float shear_y = 0.0f;
    gdip_arg_to_single(v_shear_x, &shear_x, "The first argument should be Float or Integer.");
    gdip_arg_to_single(v_shear_y, &shear_y, "The second argument should be Float or Integer.");

    MatrixOrder order = MatrixOrderPrepend;
    if (!RB_NIL_P(v_order)) {
        gdip_arg_to_enumint(cMatrixOrder, v_order, &order, "The second argument should be MatrixOrder.");
    }

    Status status = matrix->Shear(shear_x, shear_y, order);
    Check_Status(status);

    return self;
}

/**
 * Translates.
 * @overload Translate(offset_x, offset_y, order=MatrixOrder.Prepend)
 *   @param offset_x [Float]
 *   @param offset_y [Float]
 *   @param order [MatrixOrder]
 *   @return [self]
 */
static VALUE
gdip_matrix_translate(int argc, VALUE *argv, VALUE self)
{
    Check_Frozen(self);
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");

    VALUE v_offset_x, v_offset_y, v_order;
    rb_scan_args(argc, argv, "21", &v_offset_x, &v_offset_y, &v_order);

    float offset_x = 0.0f;
    float offset_y = 0.0f;
    gdip_arg_to_single(v_offset_x, &offset_x, "The first argument should be Float or Integer.");
    gdip_arg_to_single(v_offset_y, &offset_y, "The second argument should be Float or Integer.");

    MatrixOrder order = MatrixOrderPrepend;
    if (!RB_NIL_P(v_order)) {
        gdip_arg_to_enumint(cMatrixOrder, v_order, &order, "The second argument should be MatrixOrder.");
    }

    Status status = matrix->Translate(offset_x, offset_y, order);
    Check_Status(status);

    return self;
}

/**
 * @overload TransformPoints(points)
 *   Returns the result of applying this matrix to the specified points.
 *   @param points[Array<Point or PointF>]
 *   @return [Array<Point or PointF>]
 */
static VALUE
gdip_matrix_transform_points(VALUE self, VALUE v_points)
{
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");
    
    if (!_RB_ARRAY_P(v_points) || RARRAY_LEN(v_points) < 1) {
        rb_raise(rb_eTypeError, "The arguments should be Array of Point or PointF.");
    }

    VALUE r = Qnil;
    VALUE first = rb_ary_entry(v_points, 0);
    Status status = Ok;
    if (_KIND_OF(first, &tPoint)) {
        int count = 0;
        Point *points = alloc_array_of<Point, &tPoint>(v_points, count);
        status = matrix->TransformPoints(points, count);
        r = rb_ary_new_capa(count);
        for (int i = 0; i < count; ++i) {
            rb_ary_push(r, gdip_point_create(points[i].X, points[i].Y));
        }
        ruby_xfree(points);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count = 0;
        PointF *points = alloc_array_of<PointF, &tPointF>(v_points, count);
        status = matrix->TransformPoints(points, count);
        r = rb_ary_new_capa(count);
        for (int i = 0; i < count; ++i) {
            rb_ary_push(r, gdip_pointf_create(points[i].X, points[i].Y));
        }
        ruby_xfree(points);
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Array of Point or PointF.");
    }
    Check_Status(status);

    return r;
}

/**
 * @overload TransformVectors(points)
 *   Returns the result of applying this matrix excluding translation to the specified points, ignoring translation (dx, dy).
 *   @param points[Array<Point or PointF>]
 *   @return [Array<Point or PointF>]
 */
static VALUE
gdip_matrix_transform_vectors(VALUE self, VALUE v_points)
{
    Matrix *matrix = Data_Ptr<Matrix *>(self);
    Check_NULL(matrix, "This Matrix object does not exist.");
    
    if (!_RB_ARRAY_P(v_points) || RARRAY_LEN(v_points) < 1) {
        rb_raise(rb_eTypeError, "The arguments should be Array of Point or PointF.");
    }

    VALUE r = Qnil;
    VALUE first = rb_ary_entry(v_points, 0);
    Status status = Ok;
    if (_KIND_OF(first, &tPoint)) {
        int count = 0;
        Point *points = alloc_array_of<Point, &tPoint>(v_points, count);
        status = matrix->TransformVectors(points, count);
        r = rb_ary_new_capa(count);
        for (int i = 0; i < count; ++i) {
            rb_ary_push(r, gdip_point_create(points[i].X, points[i].Y));
        }
        ruby_xfree(points);
    }
    else if (_KIND_OF(first, &tPointF)) {
        int count = 0;
        PointF *points = alloc_array_of<PointF, &tPointF>(v_points, count);
        status = matrix->TransformVectors(points, count);
        r = rb_ary_new_capa(count);
        for (int i = 0; i < count; ++i) {
            rb_ary_push(r, gdip_pointf_create(points[i].X, points[i].Y));
        }
        ruby_xfree(points);
    }
    else {
        rb_raise(rb_eTypeError, "The arguments should be Array of Point or PointF.");
    }
    Check_Status(status);

    return r;
}


void
Init_matrix()
{
    cMatrix = rb_define_class_under(mGdiplus, "Matrix", cGpObject);
    rb_define_alloc_func(cMatrix, &typeddata_alloc_null<&tMatrix>);
    rb_define_method(cMatrix, "initialize", RUBY_METHOD_FUNC(gdip_matrix_init), -1);
    ATTR_R(cMatrix, Elements, elements, matrix);
    ATTR_R_Q(cMatrix, IsIdentity, is_identity, matrix);
    ATTR_R_Q(cMatrix, IsInvertible, is_invertible, matrix);
    ATTR_R(cMatrix, OffsetX, offset_x, matrix);
    ATTR_R(cMatrix, OffsetY, offset_y, matrix);

    rb_define_method(cMatrix, "==", RUBY_METHOD_FUNC(gdip_matrix_equal), 1);
    rb_define_method(cMatrix, "Clone", RUBY_METHOD_FUNC(gdip_matrix_clone), 0);
    rb_define_alias(cMatrix, "clone", "Clone");
    rb_define_alias(cMatrix, "dup", "Clone");

    rb_define_method(cMatrix, "Multiply", RUBY_METHOD_FUNC(gdip_matrix_multiply), -1);
    rb_define_alias(cMatrix, "multiply", "Multiply");
    rb_define_method(cMatrix, "Invert", RUBY_METHOD_FUNC(gdip_matrix_invert), 0);
    rb_define_alias(cMatrix, "invert", "Invert");
    rb_define_method(cMatrix, "Reset", RUBY_METHOD_FUNC(gdip_matrix_reset), 0);
    rb_define_alias(cMatrix, "reset", "Reset");
    rb_define_method(cMatrix, "Rotate", RUBY_METHOD_FUNC(gdip_matrix_rotate), -1);
    rb_define_alias(cMatrix, "rotate", "Rotate");
    rb_define_method(cMatrix, "RotateAt", RUBY_METHOD_FUNC(gdip_matrix_rotate_at), -1);
    rb_define_alias(cMatrix, "rotate_at", "RotateAt");
    rb_define_method(cMatrix, "Scale", RUBY_METHOD_FUNC(gdip_matrix_scale), -1);
    rb_define_alias(cMatrix, "scale", "Scale");
    rb_define_method(cMatrix, "Shear", RUBY_METHOD_FUNC(gdip_matrix_shear), -1);
    rb_define_alias(cMatrix, "shear", "Shear");
    rb_define_method(cMatrix, "Translate", RUBY_METHOD_FUNC(gdip_matrix_translate), -1);
    rb_define_alias(cMatrix, "translate", "Translate");
    rb_define_method(cMatrix, "TransformPoints", RUBY_METHOD_FUNC(gdip_matrix_transform_points), 1);
    rb_define_alias(cMatrix, "transform_points", "TransformPoints");
    rb_define_method(cMatrix, "TransformVectors", RUBY_METHOD_FUNC(gdip_matrix_transform_vectors), 1);
    rb_define_alias(cMatrix, "transform_points", "TransformVectors");
    rb_define_alias(cMatrix, "VectorTransformPoints", "TransformVectors");
    rb_define_alias(cMatrix, "vector_transform_points", "TransformVectors");
}
