# coding: utf-8
require 'test_helper'

class GdiplusFontTest < Test::Unit::TestCase
  include Gdiplus
  
  def test_FontFamily
    return if InstalledFontCollection.broken?
    
    assert_instance_of(FontFamily, FontFamily.GenericSansSerif)
    assert_instance_of(FontFamily, FontFamily.GenericSerif)
    assert_instance_of(FontFamily, FontFamily.GenericMonospace)
    
    assert_instance_of(String, FontFamily.GenericSansSerif.name)
    assert_instance_of(String, FontFamily.GenericSansSerif.Name)
    
    assert_instance_of(FontFamily, FontFamily.new(GenericFontFamilies.SansSerif))
    assert_instance_of(FontFamily, FontFamily.new(GenericFontFamilies.Serif))
    assert_instance_of(FontFamily, FontFamily.new(GenericFontFamilies.Monospace))
    assert_instance_of(FontFamily, FontFamily.new(:SansSerif))
    assert_instance_of(FontFamily, FontFamily.new(:Serif))
    assert_instance_of(FontFamily, FontFamily.new(:Monospace))
    assert_instance_of(FontFamily, FontFamily.new(FontFamily.GenericSansSerif.name))
    assert_instance_of(FontFamily, FontFamily.new(FontFamily.GenericSerif.Name))
    assert_instance_of(FontFamily, FontFamily.new(FontFamily.GenericMonospace.name))
    assert_instance_of(FontFamily, FontFamily.new(FontFamily.GenericSansSerif.name, InstalledFontCollection.new))
    assert_raise(GdiplusError) { FontFamily.new("jkjagsljslkdjfljaslgrfjds") }
    
    fontfamily = FontFamily.new("Times New Roman")
    assert_kind_of(Integer, fontfamily.GetEmHeight())
    assert_kind_of(Integer, fontfamily.GetCellAscent())
    assert_kind_of(Integer, fontfamily.GetCellDescent())
    assert_kind_of(Integer, fontfamily.GetLineSpacing())
    assert_kind_of(Integer, fontfamily.get_em_height())
    assert_kind_of(Integer, fontfamily.get_cell_ascent())
    assert_kind_of(Integer, fontfamily.get_cell_descent())
    assert_kind_of(Integer, fontfamily.get_line_spacing())
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
