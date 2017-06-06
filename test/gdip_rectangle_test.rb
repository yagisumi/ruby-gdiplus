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
    assert_not_equal(0, po)
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
    assert_not_equal(0, po)
  end
  
  def test_Size
    assert_instance_of(Size, Size.new)
    assert_instance_of(Size, Size.new(100, -100))
    assert_raise(ArgumentError) { Size.new(1) }
    assert_raise(TypeError) { Size.new("1", "2") }
    size = Size.new
    size.Width = -100
    size.Height = -200
    assert_equal(-100, size.Width)
    assert_equal(-200, size.Height)
    assert_equal(Size.new(-100, -200), size)
    size2 = Size.new(100, 200)
    assert_equal(Size.new, size + size2)
    assert_not_equal(0.0, size)
  end
  
  def test_SizeF
    assert_instance_of(SizeF, SizeF.new)
    assert_instance_of(SizeF, SizeF.new(100.0, -100.0))
    assert_raise(ArgumentError) { SizeF.new(1.0) }
    assert_raise(TypeError) { SizeF.new("1", "2") }
    size = SizeF.new
    size.Width = -100.0
    size.Height = -200.0
    assert_equal(-100.0, size.Width)
    assert_equal(-200.0, size.Height)
    assert_equal(SizeF.new(-100.0, -200.0), size)
    size2 = SizeF.new(100.0, 200.0)
    assert_equal(SizeF.new, size + size2)
    assert_not_equal(0.0, size)
  end
  
  def test_Rectangle_properties
    assert_instance_of(Rectangle, Rectangle.new)
    assert_instance_of(Rectangle, Rectangle.new(100, 200, 300, 400))
    assert_raise(ArgumentError) { Rectangle.new(1) }
    assert_raise(ArgumentError) { Rectangle.new(1, 2) }
    assert_raise(ArgumentError) { Rectangle.new(1, 2, 3) }
    assert_raise(TypeError) { Rectangle.new("1", "2", "3", "4") }
    
    rect = Rectangle.new
    rect.X = 100
    rect.Y = 200
    rect.Width = 300
    rect.Height = 400
    assert_equal(100, rect.X)
    assert_equal(rect.Left, rect.X)
    assert_equal(rect.left, rect.x)
    assert_equal(200, rect.Y)
    assert_equal(rect.Top, rect.Y)
    assert_equal(300, rect.Width)
    assert_equal(400, rect.Right)
    assert_equal(400, rect.Height)
    assert_equal(600, rect.Bottom)
    assert_equal(Point.new(100, 200), rect.Location)
    assert_equal(Size.new(300, 400), rect.Size)
    rect.Location = Point.new(0, 0)
    rect.Size = Size.new(30, 50)
    assert_equal(Point.new(0, 0), rect.Location)
    assert_equal(Size.new(30, 50), rect.Size)
    assert_equal(Rectangle.new(0, 0, 30, 50), rect)
  end
  
  def test_Rectangle_methods
    assert_equal(Rectangle.new(100, 100, 100, 100), Rectangle.FromLTRB(100, 100, 200, 200))
    
    rect1 = Rectangle.new(0, 0, 100, 100)
    assert_equal(true, rect1.contains?(50, 50))
    assert_equal(true, rect1.contains?(Point.new(30, 30)))
    assert_equal(false, rect1.contains?(100, 100))
    assert_equal(false, rect1.contains?(-50, 50))
    assert_equal(Rectangle.new(-100, -100, 300, 300), rect1.inflate(100, 100))
    assert_equal(Rectangle.new(-100, -100, 300, 300), rect1.inflate(Size.new(100, 100)))
    rect1.Inflate(100, 100) # inflate!
    assert_equal(Rectangle.new(-100, -100, 300, 300), rect1)
    
    rect2 = Rectangle.new(0, 0, 100, 100)
    assert_equal(true, rect2.IntersectsWith(Rectangle.new(-10, -10, 20, 20)))
    assert_equal(false, rect2.IntersectsWith(Rectangle.new(100, 100, 20, 20)))
    
    assert_equal(Rectangle.new(0, 0, 10, 10), rect2.intersect(Rectangle.new(-10, -10, 20, 20)))
    rect2.Intersect(Rectangle.new(-10, -10, 20, 20)) # intersect!
    assert_equal(Rectangle.new(0, 0, 10, 10), rect2)
    assert_equal(Rectangle.new(0, 0, 200, 200), rect2.union(Rectangle.new(100, 100, 100, 100)))
  end
  
  def test_RectangleF_properties
    assert_instance_of(RectangleF, RectangleF.new)
    assert_instance_of(RectangleF, RectangleF.new(100.0, 200.0, 300.0, 400.0))
    assert_raise(ArgumentError) { RectangleF.new(1.0) }
    assert_raise(ArgumentError) { RectangleF.new(1.0, 2.0) }
    assert_raise(ArgumentError) { RectangleF.new(1.0, 2.0, 3.0) }
    assert_raise(TypeError) { RectangleF.new("1.0", "2.0", "3.0", "4.0") }
    
    rect = RectangleF.new
    rect.X = 100.0
    rect.Y = 200.0
    rect.Width = 300.0
    rect.Height = 400.0
    assert_equal(100.0, rect.X)
    assert_equal(rect.Left, rect.X)
    assert_equal(rect.left, rect.x)
    assert_equal(200.0, rect.Y)
    assert_equal(rect.Top, rect.Y)
    assert_equal(300.0, rect.Width)
    assert_equal(400.0, rect.Right)
    assert_equal(400.0, rect.Height)
    assert_equal(600.0, rect.Bottom)
    assert_equal(PointF.new(100.0, 200.0), rect.Location)
    assert_equal(SizeF.new(300.0, 400.0), rect.Size)
    rect.Location = PointF.new(0.0, 0.0)
    rect.Size = SizeF.new(30.0, 50.0)
    assert_equal(PointF.new(0.0, 0.0), rect.Location)
    assert_equal(SizeF.new(30.0, 50.0), rect.Size)
    assert_equal(RectangleF.new(0.0, 0.0, 30.0, 50.0), rect)
  end
  
  def test_RectangleF_methods
    assert_equal(RectangleF.new(100.0, 100.0, 100.0, 100.0), RectangleF.FromLTRB(100.0, 100.0, 200.0, 200.0))
    rect1 = RectangleF.new(0.0, 0.0, 100.0, 100.0)
    assert_equal(true, rect1.contains?(50.0, 50.0))
    assert_equal(true, rect1.contains?(PointF.new(30.0, 30.0)))
    assert_equal(false, rect1.contains?(100.0, 100.0))
    assert_equal(false, rect1.contains?(-50.0, 50.0))
    assert_equal(RectangleF.new(-100.0, -100.0, 300.0, 300.0), rect1.inflate(100.0, 100.0))
    assert_equal(RectangleF.new(-100.0, -100.0, 300.0, 300.0), rect1.inflate(SizeF.new(100.0, 100.0)))
    rect1.Inflate(100.0, 100.0) # inflate!
    assert_equal(RectangleF.new(-100.0, -100.0, 300.0, 300.0), rect1)
    
    rect2 = RectangleF.new(0.0, 0.0, 100.0, 100.0)
    assert_equal(true, rect2.IntersectsWith(RectangleF.new(-10.0, -10.0, 20.0, 20.0)))
    assert_equal(false, rect2.IntersectsWith(RectangleF.new(100.0, 100.0, 20.0, 20.0)))
    
    assert_equal(RectangleF.new(0.0, 0.0, 10.0, 10.0), rect2.intersect(RectangleF.new(-10.0, -10.0, 20.0, 20.0)))
    rect2.Intersect(RectangleF.new(-10.0, -10.0, 20.0, 20.0)) # intersect!
    assert_equal(RectangleF.new(0.0, 0.0, 10.0, 10.0), rect2)
    assert_equal(RectangleF.new(0.0, 0.0, 200.0, 200.0), rect2.union(RectangleF.new(100.0, 100.0, 100.0, 100.0)))
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
