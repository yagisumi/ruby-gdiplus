# coding: utf-8
require 'test_helper'

class GdiplusRegionTest < Test::Unit::TestCase
  include Gdiplus
  
  def test_constructor
    assert_instance_of(Region, Region.new)
    assert_instance_of(Region, Region.new(GraphicsPath.new))
    assert_instance_of(Region, Region.new(RectangleF.new(0.0, 0.0, 100.0, 100.0)))
    assert_instance_of(Region, Region.new(Rectangle.new(0, 0, 100, 100)))
    data = Region.new(Rectangle.new(0, 0, 100, 100)).GetData
    assert_instance_of(Region, Region.new(data))
  end
  
  def region_circle
    path = GraphicsPath.new
    path.AddEllipse(0, 0, 200, 200)
    return Region.new(path)
  end
  
  def region_square
    path = GraphicsPath.new
    path.AddRectangle(100, 100, 200, 200)
    return Region.new(path)
  end
  
  def test_methods
    # Clone Equals
    region = Region.new(Rectangle.new(0, 0, 100, 100))
    region2 = region.Clone
    assert_instance_of(Region, region2)
    assert_not_same(region, region2)
    bmp = Bitmap.new(300, 300)
    bmp.draw { |g|
      p g.gdiplus_id
      assert_equal(true, region.Equals(region2, g))
    }
    
    # Complement(Region)
    r1 = region_circle
    r2 = region_square
    assert_same(r1, r1.Complement(r2))
    
    # Complement(GraphicsPath)
    r1 = region_circle
    path = GraphicsPath.new
    path.AddRectangle(100, 100, 200, 200)
    assert_same(r1, r1.Complement(path))
    
    # Complement(Rectangle)
    r1 = region_circle
    assert_same(r1, r1.Complement(Rectangle.new(100, 100, 200, 200)))
    
    # Complement(RectangleF)
    r1 = region_circle
    assert_same(r1, r1.Complement(RectangleF.new(100.0, 100.0, 200.0, 200.0)))
    
    # Exclude(Region)
    r1 = region_circle
    r2 = region_square
    assert_same(r1, r1.Exclude(r2))
    
    # Exclude(GraphicsPath)
    r1 = region_circle
    path = GraphicsPath.new
    path.AddRectangle(100, 100, 200, 200)
    assert_same(r1, r1.Exclude(path))
    
    # Exclude(Rectangle)
    r1 = region_circle
    assert_same(r1, r1.Exclude(Rectangle.new(100, 100, 200, 200)))
    
    # Exclude(RectangleF)
    r1 = region_circle
    assert_same(r1, r1.Exclude(RectangleF.new(100.0, 100.0, 200.0, 200.0)))
    
    # Intersect(Region)
    r1 = region_circle
    r2 = region_square
    assert_same(r1, r1.Intersect(r2))
    
    # Intersect(GraphicsPath)
    r1 = region_circle
    path = GraphicsPath.new
    path.AddRectangle(100, 100, 200, 200)
    assert_same(r1, r1.Intersect(path))
    
    # Intersect(Rectangle)
    r1 = region_circle
    assert_same(r1, r1.Intersect(Rectangle.new(100, 100, 200, 200)))
    
    # Intersect(RectangleF)
    r1 = region_circle
    assert_same(r1, r1.Intersect(RectangleF.new(100.0, 100.0, 200.0, 200.0)))
    
    # Union(Region)
    r1 = region_circle
    r2 = region_square
    assert_same(r1, r1.Union(r2))
    
    # Union(GraphicsPath)
    r1 = region_circle
    path = GraphicsPath.new
    path.AddRectangle(100, 100, 200, 200)
    assert_same(r1, r1.Union(path))
    
    # Union(Rectangle)
    r1 = region_circle
    assert_same(r1, r1.Union(Rectangle.new(100, 100, 200, 200)))
    
    # Union(RectangleF)
    r1 = region_circle
    assert_same(r1, r1.Union(RectangleF.new(100.0, 100.0, 200.0, 200.0)))
    
    # Xor(Region)
    r1 = region_circle
    r2 = region_square
    assert_same(r1, r1.Xor(r2))
    
    # Xor(GraphicsPath)
    r1 = region_circle
    path = GraphicsPath.new
    path.AddRectangle(100, 100, 200, 200)
    assert_same(r1, r1.Xor(path))
    
    # Xor(Rectangle)
    r1 = region_circle
    assert_same(r1, r1.Xor(Rectangle.new(100, 100, 200, 200)))
    
    # Xor(RectangleF)
    r1 = region_circle
    assert_same(r1, r1.Xor(RectangleF.new(100.0, 100.0, 200.0, 200.0)))
    
    # GetBounds
    r1 = region_circle
    bmp.draw {|g|
      assert_equal(RectangleF.new(0.0, 0.0, 200.0, 200.0), r1.GetBounds(g))
    }
    
    # GetRegionScans
    region = Region.new(Rectangle.new(0, 0, 30, 30))
    ary = region.GetRegionScans(Matrix.new)
    assert_instance_of(Array, ary)
    assert_instance_of(RectangleF, ary.first)
    
    # IsEmpty, IsInfinite, MakeEmpty, MakeInfinite
    bmp.draw { |g|
      r1 = region_circle
      assert_equal(false, r1.IsEmpty(g))
      assert_equal(false, r1.IsInfinite(g))
      r1.MakeEmpty
      assert_equal(true, r1.IsEmpty(g))
      r1.MakeInfinite
      assert_equal(true, r1.IsInfinite(g))
    }
    
    # Transform, Translate
    bmp.draw { |g|
      r1 = region_square
      r1.Translate(-200, -200)
      assert_equal(RectangleF.new(-100.0, -100.0, 200.0, 200.0), r1.GetBounds(g))
      matrix = Matrix.new
      matrix.Rotate(45)
      r1.Transform(matrix)
      n = 100 * Math.sqrt(2.0)
      assert((r1.GetBounds(g).X + n).abs < 0.001)
    }
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
