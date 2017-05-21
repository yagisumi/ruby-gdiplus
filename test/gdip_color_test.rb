# coding: utf-8
require 'test_helper'

class GdiplusColorTest < Test::Unit::TestCase
  include Gdiplus
  
  def test_color_args
    assert_instance_of(Color, Color.new())
    assert_instance_of(Color, Color.new(0xff000000))
    assert_instance_of(Color, Color.new(128, Color.new(0xf0123456)))
    assert_instance_of(Color, Color.new(255, 0, 0))
    assert_instance_of(Color, Color.new(200, 255, 0, 0))
    assert_raise(ArgumentError) { Color.new(1, 2, 3, 4, 5) }
    assert_raise(TypeError) { Color.new("1") }
    assert_raise(TypeError) { Color.new(128, 255) }
    assert_raise(TypeError) { Color.new("1", Color.new) }
    assert_raise(TypeError) { Color.new(1, 2, "3") }
    assert_raise(TypeError) { Color.new(1, 2, 3, "4") }
  end
  
  def test_color_int
    assert_equal(0xff000000, Color.new.to_i)
    assert_equal(0xff112233, Color.new(0xff112233).to_i)
    assert_equal(0xff112233, Color.new(0x11, 0x22, 0x33).to_i)
    assert_equal(0xff112233, Color.new(0xff, 0x11, 0x22, 0x33).to_i)
    assert_equal(0x88112233, Color.new(0x88, Color.new(0xff112233)).to_i)
  end
  
  def test_color_equal
    assert_equal(true, Color.new == Color.new)
    assert_equal(true, Color.new(255, 255, 255) == Color.new(255, 255, 255, 255))
    assert_equal(true, Color.new(0xffffffff) == 0xffffffff)
    assert_equal(false, Color.new == 1.1)
    assert_equal(false, Color.new == "1")
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
