# coding: utf-8
require 'test_helper'

class GdiplusEnumIntTest < Test::Unit::TestCase
  include Gdiplus
  
  def test_pixel_format
    assert_equal(PixelFormat::Format32bppARGB, PixelFormat.Format32bppARGB)
    # <Gdiplus::PixelFormat.Format32bppARGB: 0x0026200a>
    assert_match(/Format32bppARGB/, PixelFormat::Format32bppARGB.inspect)
    assert_match(/#{PixelFormat::Format32bppARGB.to_i.to_s(16)}/, PixelFormat::Format32bppARGB.inspect)
  end
  
  def test_encoder
    p Encoder.Quality
  end
  
  
end

__END__
assert_equal(expected, actual, message=nil)
assert_raise(expected_exception_klass, message="") { ... }
assert_not_equal(expected, actual, message="")
assert_instance_of(klass, object, message="")
assert_nil(object, message="")
assert_not_nil(object, message="")
assert_kind_of(klass, object, message="")
assert_respond_to(object, method, message="")
assert_match(regexp, string, message="")
assert_no_match(regexp, string, message="")
assert_same(expected, actual, message="")
assert_not_same(expected, actual, message="")
assert_operator(object1, operator, object2, message="")
assert_nothing_raised(klass1, klass2, ..., message = "") { ... } # klass1, klass2, ... => fail / others => error
assert_block(message="assert_block failed.") { ... } # (block -> true) => pass
assert_throws(expected_symbol, message="") { ... }
assert_nothing_thrown(message="") { ... }
