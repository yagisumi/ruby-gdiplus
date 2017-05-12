# coding: utf-8
require 'test_helper'

class GdiplusEnumIntTest < Test::Unit::TestCase
  include Gdiplus
  
  def test_PixelFormat
    assert_equal(0, PixelFormat::Undefined.to_i)
    assert_equal(1, 1 + PixelFormat::Undefined)
    assert_equal(0x30000, PixelFormat.Indexed | PixelFormat.GDI)
    assert_raise(TypeError) { "str" + PixelFormat::Undefined }
    assert_equal(PixelFormat::Format32bppARGB, PixelFormat.Format32bppARGB)
    # <Gdiplus::PixelFormat.Format32bppARGB: 0x0026200a>
    assert_match(/Format32bppARGB/, PixelFormat::Format32bppARGB.inspect)
    assert_match(/#{PixelFormat::Format32bppARGB.to_i.to_s(16)}/, PixelFormat::Format32bppARGB.inspect)
  end
  
  def test_Encoder
    assert_kind_of(Guid, Encoder.Quality)
    assert_equal("1d5be4b5-fa4a-452d-9cdd-5db35105e7eb", Encoder.Quality.to_s)
  end
  
  def test_ValueType
    assert_equal(0, ValueType.ColorTypeCMYK.to_i)
    assert_match(/ColorTypeCMYK/, ValueType.ColorTypeCMYK.inspect)
    assert_equal(1, ValueType.ColorTypeYCCK.to_i)
    assert_match(/ColorTypeYCCK/, ValueType.ColorTypeYCCK.inspect)
    assert_equal(2, ValueType.CompressionLZW.to_i)
    assert_match(/CompressionLZW/, ValueType.CompressionLZW.inspect)
    assert_equal(3, ValueType.CompressionCCITT3.to_i)
    assert_match(/CompressionCCITT3/, ValueType.CompressionCCITT3.inspect)
    assert_equal(4, ValueType.CompressionCCITT4.to_i)
    assert_match(/CompressionCCITT4/, ValueType.CompressionCCITT4.inspect)
    assert_equal(5, ValueType.CompressionRle.to_i)
    assert_match(/CompressionRle/, ValueType.CompressionRle.inspect)
    assert_equal(6, ValueType.CompressionNone.to_i)
    assert_match(/CompressionNone/, ValueType.CompressionNone.inspect)
    assert_equal(7, ValueType.ScanMethodInterlaced.to_i)
    assert_match(/ScanMethodInterlaced/, ValueType.ScanMethodInterlaced.inspect)
    assert_equal(8, ValueType.ScanMethodNonInterlaced.to_i)
    assert_match(/ScanMethodNonInterlaced/, ValueType.ScanMethodNonInterlaced.inspect)
    assert_equal(9, ValueType.VersionGif87.to_i)
    assert_match(/VersionGif87/, ValueType.VersionGif87.inspect)
    assert_equal(10, ValueType.VersionGif89.to_i)
    assert_match(/VersionGif89/, ValueType.VersionGif89.inspect)
    assert_equal(11, ValueType.RenderProgressive.to_i)
    assert_match(/RenderProgressive/, ValueType.RenderProgressive.inspect)
    assert_equal(12, ValueType.RenderNonProgressive.to_i)
    assert_match(/RenderNonProgressive/, ValueType.RenderNonProgressive.inspect)
    assert_equal(13, ValueType.TransformRotate90.to_i)
    assert_match(/TransformRotate90/, ValueType.TransformRotate90.inspect)
    assert_equal(14, ValueType.TransformRotate180.to_i)
    assert_match(/TransformRotate180/, ValueType.TransformRotate180.inspect)
    assert_equal(15, ValueType.TransformRotate270.to_i)
    assert_match(/TransformRotate270/, ValueType.TransformRotate270.inspect)
    assert_equal(16, ValueType.TransformFlipHorizontal.to_i)
    assert_match(/TransformFlipHorizontal/, ValueType.TransformFlipHorizontal.inspect)
    assert_equal(17, ValueType.TransformFlipVertical.to_i)
    assert_match(/TransformFlipVertical/, ValueType.TransformFlipVertical.inspect)
    assert_equal(18, ValueType.MultiFrame.to_i)
    assert_match(/MultiFrame/, ValueType.MultiFrame.inspect)
    assert_equal(19, ValueType.LastFrame.to_i)
    assert_match(/LastFrame/, ValueType.LastFrame.inspect)
    assert_equal(20, ValueType.Flush.to_i)
    assert_match(/Flush/, ValueType.Flush.inspect)
    assert_equal(21, ValueType.FrameDimensionTime.to_i)
    assert_match(/FrameDimensionTime/, ValueType.FrameDimensionTime.inspect)
    assert_equal(22, ValueType.FrameDimensionResolution.to_i)
    assert_match(/FrameDimensionResolution/, ValueType.FrameDimensionResolution.inspect)
    assert_equal(23, ValueType.FrameDimensionPage.to_i)
    assert_match(/FrameDimensionPage/, ValueType.FrameDimensionPage.inspect)
    assert_equal(24, ValueType.ColorTypeGray.to_i)
    assert_match(/ColorTypeGray/, ValueType.ColorTypeGray.inspect)
    assert_equal(25, ValueType.ColorTypeRGB.to_i)
    assert_match(/ColorTypeRGB/, ValueType.ColorTypeRGB.inspect)
  end
  
  def test_EncoderParameter
    assert_equal(1, EncoderParameterValueType.ValueTypeByte.to_i)
    assert_match(/ValueTypeByte/, EncoderParameterValueType.ValueTypeByte.inspect)
    assert_equal(2, EncoderParameterValueType.ValueTypeASCII.to_i)
    assert_match(/ValueTypeASCII/, EncoderParameterValueType.ValueTypeASCII.inspect)
    assert_equal(3, EncoderParameterValueType.ValueTypeShort.to_i)
    assert_match(/ValueTypeShort/, EncoderParameterValueType.ValueTypeShort.inspect)
    assert_equal(4, EncoderParameterValueType.ValueTypeLong.to_i)
    assert_match(/ValueTypeLong/, EncoderParameterValueType.ValueTypeLong.inspect)
    assert_equal(5, EncoderParameterValueType.ValueTypeRational.to_i)
    assert_match(/ValueTypeRational/, EncoderParameterValueType.ValueTypeRational.inspect)
    assert_equal(6, EncoderParameterValueType.ValueTypeLongRange.to_i)
    assert_match(/ValueTypeLongRange/, EncoderParameterValueType.ValueTypeLongRange.inspect)
    assert_equal(7, EncoderParameterValueType.ValueTypeUndefined.to_i)
    assert_match(/ValueTypeUndefined/, EncoderParameterValueType.ValueTypeUndefined.inspect)
    assert_equal(8, EncoderParameterValueType.ValueTypeRationalRange.to_i)
    assert_match(/ValueTypeRationalRange/, EncoderParameterValueType.ValueTypeRationalRange.inspect)
    assert_equal(9, EncoderParameterValueType.ValueTypePointer.to_i)
    assert_match(/ValueTypePointer/, EncoderParameterValueType.ValueTypePointer.inspect)
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
