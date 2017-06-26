# coding: utf-8
require 'test_helper'

class GdiplusMatrixTest < Test::Unit::TestCase
  include Gdiplus
  include Math
  
  E = [1.0, 0.0, 0.0, 1.0, 0.0, 0.0]
  
  def compare_single_array(a1, a2, epsilon=0.000001)
    a1.size.times { |i|
      assert((a1[i] - a2[i]).abs < epsilon)
    }
  end
  
  def test_constructor
    # initialize()
    assert_instance_of(Matrix, Matrix.new())
    
    # initialize(Rectangle, Array<Point>)
    points = [Point.new(20, 10), Point.new(30, 10), Point.new(20, 10)]
    rect = Rectangle.new(10, 10, 10, 10)
    assert_instance_of(Matrix, Matrix.new(rect, points))
    
    # initialize(RectangleF, Array<PointF>)
    points = [PointF.new(20.0, 10.0), PointF.new(30.0, 10.0), PointF.new(20.0, 10.0)]
    rect = RectangleF.new(10.0, 10.0, 10.0, 10.0)
    assert_instance_of(Matrix, Matrix.new(rect, points))
    
    # initialize(Float, Float, Float, Float, Float, Float)
    assert_instance_of(Matrix, Matrix.new(cos(PI / 4.0), sin(PI / 4.0), -sin(PI / 4.0), cos(PI / 4.0), 10.0, 10.0))
    
    # initialize(Rectangle, Array<Point>) array size > 3
    points = [Point.new(20, 10), Point.new(30, 10), Point.new(20, 10), Point.new(20, 30), Point.new(20, 30)]
    rect = Rectangle.new(10, 10, 10, 10)
    assert_instance_of(Matrix, Matrix.new(rect, points))
    
    # initialize(Rectangle, Array<Point>) array size < 3
    points = [Point.new(20, 10), Point.new(30, 10)]
    rect = Rectangle.new(10, 10, 10, 10)
    assert_raise(ArgumentError) { Matrix.new(rect, points) }
    
    # invalid number of arguments
    assert_raise(ArgumentError) { Matrix.new(1) }
    assert_raise(ArgumentError) { Matrix.new(1, 2, 3) }
    assert_raise(ArgumentError) { Matrix.new(1, 2, 3, 4, 5) }
    assert_raise(ArgumentError) { Matrix.new(1, 2, 3, 4, 5, 6, 7) }
  end
  
  def test_properties
    # Elements
    assert_equal(E, Matrix.new.Elements)
    
    elems = [cos(PI / 4.0), sin(PI / 4.0), -sin(PI / 4.0), cos(PI / 4.0), 10.0, 10.0]
    m_elems = Matrix.new(*elems).elements
    compare_single_array(elems, m_elems)
    
    # IsIdentity, IsInvertible
    assert_equal(true, Matrix.new.IsIdentity)
    assert_equal(true, Matrix.new.is_identity?)
    assert_equal(true, Matrix.new.IsInvertible)
    assert_equal(true, Matrix.new.is_invertible?)
    
    assert_equal(true, Matrix.new(*elems).IsInvertible)
    assert_equal(false, Matrix.new(*elems).IsIdentity)
    assert_equal(false, Matrix.new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0).IsInvertible)
    
    # OffsetX, OffsetY
    assert_equal(10.0, Matrix.new(1.0, 0.0, 0.0, 1.0, 10.0, 0.0).OffsetX)
    assert_equal(10.0, Matrix.new(1.0, 0.0, 0.0, 1.0, 10.0, 0.0).offset_x)
    assert_equal(10.0, Matrix.new(1.0, 0.0, 0.0, 1.0, 0.0, 10.0).OffsetY)
    assert_equal(10.0, Matrix.new(1.0, 0.0, 0.0, 1.0, 0.0, 10.0).offset_y)
  end
  
  def test_methods
    # Multiply, multiply
    elems1 = [cos(PI / 4.0), sin(PI / 4.0), -sin(PI / 4.0), cos(PI / 4.0), 0.0, 0.0]
    elems2 = [cos(-PI / 4.0), sin(-PI / 4.0), -sin(-PI / 4.0), cos(-PI / 4.0), 0.0, 0.0]
    
    assert_equal(Matrix.new, Matrix.new)
    m1 = Matrix.new(*elems1)
    m2 = Matrix.new(*elems2)
    
    assert_not_equal(m1, m2)
    
    assert_same(m1, m1.Multiply(m2))
    assert_equal(E, m1.Elements)
    
    m1 = Matrix.new(*elems1)
    m2 = Matrix.new(*elems2)
    assert_same(m1, m1.multiply(m2))
    assert_equal(E, m1.Elements)
    
    # Invert
    m1 = Matrix.new(*elems1)
    assert_same(m1, m1.Invert)
    compare_single_array(elems2, m1.elements)
    
    m1 = Matrix.new(*elems1)
    assert_same(m1, m1.invert)
    compare_single_array(elems2, m1.elements)
    
    assert_raise(GdiplusError) { Matrix.new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0).invert }
    
    # Reset
    m1 = Matrix.new(*elems1)
    assert_same(m1, m1.Reset)
    assert_equal(true, m1.IsIdentity)
    
    m1 = Matrix.new(*elems1)
    assert_same(m1, m1.reset)
    assert_equal(true, m1.is_identity?)
    
    # Rotate
    compare_single_array(elems1, Matrix.new.Rotate(45.0).elements)
    
    m1 = Matrix.new(*elems1)
    m1.rotate(-45.0)
    compare_single_array(E, m1.elements)
    
    # RotateAt
    assert_equal(Matrix.new.RotateAt(45.0, PointF.new(100.0, 100.0)), Matrix.new.rotate_at(45, Point.new(100, 100)))
    
    # Scale
    assert_equal([2.0, 0.0, 0.0, 3.0, 0.0, 0.0], Matrix.new.Scale(2, 3).Elements)
    assert_equal([2.0, 0.0, 0.0, 3.0, 0.0, 0.0], Matrix.new.scale(2.0, 3.0, MatrixOrder.Append).elements)
    
    # Shear
    assert_equal([1.0, 3.0, 2.0, 1.0, 0.0, 0.0], Matrix.new.Shear(2, 3).Elements)
    assert_equal([1.0, 3.0, 2.0, 1.0, 0.0, 0.0], Matrix.new.shear(2.0, 3.0, MatrixOrder.Append).elements)
    
    # Translate
    assert_equal([1.0, 0.0, 0.0, 1.0, 2.0, 3.0], Matrix.new.Translate(2, 3).Elements)
    assert_equal([1.0, 0.0, 0.0, 1.0, 2.0, 3.0], Matrix.new.translate(2.0, 3.0, MatrixOrder.Append).elements)
    
    # TransformPoints
    points = []
    points << PointF.new(100.0, 100.0)
    points << PointF.new(300.0, 100.0)
    points << PointF.new(300.0, 300.0)
    points << PointF.new(100.0, 300.0)
    matrix = Matrix.new.RotateAt(45.0, PointF.new(200.0, 200.0))
    points2 = matrix.TransformPoints(points)
    r = 100.0 * sqrt(2)
    compare_single_array(points2.map { |po| po.x }, [200.0, 200.0 + r, 200.0, 200.0 - r], 0.0001)
    
    # VectorTransformPoints (TransformVectors)
    points = []
    points << PointF.new(100.0, 100.0)
    points << PointF.new(300.0, 100.0)
    points << PointF.new(300.0, 300.0)
    points << PointF.new(100.0, 300.0)
    matrix = Matrix.new.RotateAt(45.0, PointF.new(200.0, 200.0))
    points2 = matrix.TransformVectors(points)
    compare_single_array([points2[2].x, points2[2].y], [0.0, 300.0 * sqrt(2)], 0.0001)
  end
  
  
end

__END__
#assert_equal(expected, actual, message=nil)
#assert_raise(expected_exception_klass, message="") { ... }
#assert_not_equal(expected, actual, message="")
#assert_instance_of(klass, object, message="")
#assert_kind_of(klass, object, message="")
#assert_nil(object, message="")
#assert_not_nil(object, message="")
#assert_respond_to(object, method, message="")
#assert_match(regexp, string, message="")
#assert_no_match(regexp, string, message="")
#_assert_output(stdout=nil, stderr=nil, verbose=nil) { ... }
#_assert_silent(verbose=nil) { ... }
#_assert_stderr(stderr, verbose=nil) { ... }
#_assert_stderr_silent(verbose=nil) { ... }
#_assert_stdout(stdout, verbose=nil) { ... }
#_assert_stdout_silent(verbose=nil) { ... }
#assert_same(expected, actual, message="")
#assert_not_same(expected, actual, message="")
#assert_operator(object1, operator, object2, message="")
#assert_nothing_raised(klass1, klass2, ..., message = "") { ... } # klass1, klass2, ... => fail / others => error
#assert_block(message="assert_block failed.") { ... } # (block -> true) => pass
#assert_throws(expected_symbol, message="") { ... }
#assert_nothing_thrown(message="") { ... }
