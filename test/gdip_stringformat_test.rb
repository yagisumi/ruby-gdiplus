# coding: utf-8
require 'test_helper'

class GdiplusStringFormatTest < Test::Unit::TestCase
  include Gdiplus
  
  def test_create
    assert_instance_of(StringFormat, StringFormat.new)
    assert_instance_of(StringFormat, StringFormat.new(StringFormat.new))
    assert_instance_of(StringFormat, StringFormat.new(StringFormatFlags.NoWrap))
    assert_instance_of(StringFormat, StringFormat.new(StringFormatFlags.NoWrap, LangId.en_US))
  end
  
  def test_properties
    assert_equal(StringFormat.GenericDefault.object_id, StringFormat.GenericDefault.object_id)
    assert_equal(StringFormat.GenericTypographic.object_id, StringFormat.GenericTypographic.object_id)
    
    sf = StringFormat.new(StringFormatFlags.NoWrap.NoClip)
    sf.Alignment = StringAlignment.Center
    assert_equal(StringAlignment.Center, sf.Alignment)
    sf.Alignment = :Far
    assert_equal(StringAlignment.Far, sf.Alignment)
    
    sf.SetDigitSubstitution(LangId.en_US, StringDigitSubstitute::National)
    assert_equal(LangId.en_US, sf.DigitSubstitutionLanguage)
    assert_equal(StringDigitSubstitute.National, sf.DigitSubstitutionMethod)
    
    assert_equal(StringFormatFlags.new(:NoWrap, :NoClip), sf.FormatFlags)
    sf.FormatFlags = :DirectionVertical
    assert_equal(StringFormatFlags.DirectionVertical, sf.FormatFlags)
    
    sf.HotkeyPrefix = HotkeyPrefix.Show
    assert_equal(HotkeyPrefix.Show, sf.HotkeyPrefix)
    
    sf.Trimming = StringTrimming.Word
    assert_equal(StringTrimming.Word, sf.Trimming)
  end
  
  def test_methods
    offset = 99.0
    tab_stops = [100.0, 200.0, 300.0]
    
    sf = StringFormat.new
    assert_same(sf, sf.SetTabStops(offset, tab_stops))
    assert_equal(offset, sf.GetFirstTabOffset)
    assert_equal(tab_stops, sf.GetTabStops)
    
    assert_same(sf, sf.SetMeasurableCharacterRanges([0..3, 10..13]))
    assert_raise(TypeError) { sf.SetMeasurableCharacterRanges([]) }
    _assert_stderr(/Range/, true) { sf.SetMeasurableCharacterRanges([1]) }
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
