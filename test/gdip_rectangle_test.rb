# coding: utf-8
require 'test_helper'

class GdiplusRectangleTest < Test::Unit::TestCase
  include Gdiplus
  
  def test_Point
    assert_instance_of(Point, Point.new)
    assert_instance_of(Point, Point.new(100, -100))
    assert_raise(ArgumentError) { Point.new(1) }
    assert_raise(TypeError) { Point.new("1", "2") }
    po = Point.new
    po.X = -100
    po.Y = -200
    assert_equal(-100, po.X)
    assert_equal(-200, po.Y)
    assert_equal(Point.new(-100, -200), po)
    po2 = Point.new(100, 200)
    assert_equal(Point.new, po + po2)
  end
  
  def test_PointF
    assert_instance_of(PointF, PointF.new)
    assert_instance_of(PointF, PointF.new(100.0, -100.0))
    assert_raise(ArgumentError) { PointF.new(1) }
    assert_raise(TypeError) { PointF.new("1", "2") }
    assert_raise(TypeError) { PointF.new(1, 2) }
    po = PointF.new
    po.X = -100.0
    po.Y = -200.0
    assert_equal(-100.0, po.X)
    assert_equal(-200.0, po.Y)
    assert_equal(PointF.new(-100.0, -200.0), po)
    po2 = PointF.new(100.0, 200.0)
    assert_equal(PointF.new, po + po2)
  end
end

__END__
#assert_equal(expected, actual, message=nil)
#assert_raise(expected_exception_klass, message="") { ... }
#assert_not_equal(expected, actual, message="")
#assert_instance_of(klass, object, message="")
#assert_nil(object, message="")
#assert_not_nil(object, message="")
#assert_kind_of(klass, object, message="")
#assert_respond_to(object, method, message="")
#assert_match(regexp, string, message="")
#assert_no_match(regexp, string, message="")
#assert_same(expected, actual, message="")
#assert_not_same(expected, actual, message="")
#assert_operator(object1, operator, object2, message="")
#assert_nothing_raised(klass1, klass2, ..., message = "") { ... } # klass1, klass2, ... => fail / others => error
#assert_block(message="assert_block failed.") { ... } # (block -> true) => pass
#assert_throws(expected_symbol, message="") { ... }
#assert_nothing_thrown(message="") { ... }
