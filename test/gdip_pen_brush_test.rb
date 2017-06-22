# coding: utf-8
require 'test_helper'

class GdiplusPenBrushTest < Test::Unit::TestCase
  include Gdiplus
  
  def test_pen
    pen = Pen.new(Color.Red)
    assert_instance_of(Pen, pen)
    assert_equal(Color.Red, pen.Color)
    assert_equal(1.0, pen.Width)
    assert_equal(0.0, pen.DashOffset)
    assert_equal(10.0, pen.MiterLimit)
    assert_equal(PenType.SolidColor, pen.PenType)
    assert_equal(DashStyle.Solid, pen.DashStyle)
    assert_equal(PenAlignment.Center, pen.Alignment)
    assert_equal(DashCap.Flat, pen.DashCap)
    assert_equal(LineCap.Flat, pen.EndCap)
    assert_equal(LineCap.Flat, pen.StartCap)
    assert_equal(LineJoin.Miter, pen.LineJoin)
    assert_equal([], pen.CompoundArray)
    assert_equal([], pen.DashPattern)
    
    pen.width = 3.0
    assert_equal(3.0, pen.Width)
    
    pen.Color = :Green
    assert_equal(Color.Green, pen.Color)
    pen.Alignment = PenAlignment.Inset
    assert_equal(PenAlignment.Inset, pen.Alignment)
    pen.Alignment = PenAlignment.Center
    assert_equal(PenAlignment.Center, pen.Alignment)
    
    pen.CompoundArray = [0.0, 0.2, 0.7, 1.0]
    assert_equal([0.0, 0.2, 0.7, 1.0], pen.CompoundArray)
    pen.CompoundArray = [0.0, 1.0]
    assert_equal([0.0, 1.0], pen.CompoundArray)
    
    _assert_stderr(/CompoundArray/, false) { pen.Alignment = PenAlignment.Inset }
    
    pen.DashPattern = [3.0, 1.0]
    assert_equal([3.0, 1.0], pen.DashPattern)
    assert_equal(DashStyle.Custom, pen.DashStyle)
    
    pen.DashPattern = [2.0, 6.0, 3.0]
    assert_equal([2.0, 6.0, 3.0], pen.DashPattern)
    
    pen.DashStyle = DashStyle.Dash
    assert_equal(DashStyle.Dash, pen.DashStyle)
    
    pen.DashCap = DashCap.Round
    assert_equal(DashCap.Round, pen.DashCap)
    
    pen.EndCap = LineCap.ArrowAnchor
    assert_equal(LineCap.ArrowAnchor, pen.EndCap)
    
    pen.StartCap = :Round
    assert_equal(LineCap.Round, pen.StartCap)
    
    pen.LineJoin = :Bevel
    assert_equal(LineJoin.Bevel, pen.LineJoin)
    
    pen = Pen.new(Color.Black)
    pen.Alignment = PenAlignment.Inset
    _assert_stderr(/Inset/, false) { pen.CompoundArray = [0.0, 0.2, 0.7, 1.0] }
    
    pen = Pen.new(Color.Black)
  end

  def test_brush
    
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
