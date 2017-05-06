# coding: utf-8
require 'test_helper'

class Guid2 < Gdiplus::Guid
end

class GdiplusCodecTest < Test::Unit::TestCase
  include Gdiplus
  
  def test_guid
    # Guid.new(str)
    assert_raise(ArgumentError) { Guid.new }
    g1 = nil
    assert_nothing_raised() { g1 = Guid.new("b96b3ca9-0728-11d3-9d7b-0000f81ef32e") }
    assert_equal("<Gdiplus::Guid {b96b3ca9-0728-11d3-9d7b-0000f81ef32e}>", g1.inspect)
    assert_equal("b96b3ca9-0728-11d3-9d7b-0000f81ef32e", g1.to_s)
    assert_equal("{0xb96b3ca9, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}}", g1.c_struct)
    assert_raise(GdiplusError) { Guid.new("{b96b3ca9-0728-11d3-9d7b-0000f81ef32e}") }
    assert_raise(TypeError) { Guid.new(1) }
    
    g1b = Guid.new("b96b3ca9-0728-11d3-9d7b-0000f81ef32e")
    
    h = {}
    h[g1] = 1
    h[g1b] = 2
    assert_equal(true, g1 == g1b)
    assert_equal(true, g1.eql?(g1b), "same value")
    assert_equal(2, h[g1])
    
    g2 = Guid2.new("b96b3ca9-0728-11d3-9d7b-0000f81ef32e")
    h[g2] = 3
    assert_equal(true, g1 == g2)
    assert_equal(true, g1.eql?(g2), "different class but same value")
    assert_equal(3, h[g1])
    
    g3 = Guid.new("b96b3ca9-0728-11d3-9d7b-0000f81ef320")
    h[g3] = 4
    assert_equal(false, h[g1] == h[g3])
end
  
end
