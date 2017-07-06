# coding: utf-8
require 'test_helper'

class GdiplusImageAttributesTest < Test::Unit::TestCase
  include Gdiplus
  
  def test_colormatrix
    matrix = [
      0.0, 0.1, 0.2, 0.3, 0.0,
      0.4, 0.5, 0.6, 0.7, 0.0,
      0.8, 0.9, 1.0, 0.1, 0.0,
      0.2, 0.3, 0.4, 0.5, 0.0,
      0.6, 0.7, 0.8, 0.9, 1.0,
    ]
    assert_instance_of(ColorMatrix, ColorMatrix.new)
    assert_instance_of(ColorMatrix, ColorMatrix.new(matrix))
    assert_instance_of(ColorMatrix, ColorMatrix.new(*matrix))
    
    cmatrix = ColorMatrix.new()
    assert_equal(1.0, cmatrix[0, 0])
    assert_equal(1.0, cmatrix[1, 1])
    assert_equal(1.0, cmatrix[2, 2])
    assert_equal(1.0, cmatrix[3, 3])
    assert_equal(1.0, cmatrix[4, 4])
    assert_equal(0.0, cmatrix[1, 0])
    assert_equal(0.0, cmatrix[0, 1])
    
    cmatrix = ColorMatrix.new(matrix)
    5.times { |i|
      5.times { |j|
        assert_equal(matrix[5 * i + j], cmatrix[i, j])
        cmatrix[i, j] = 2.0
        assert_equal(2.0, cmatrix[i, j])
      }
    }
    
    cmatrix = ColorMatrix.new(*matrix)
    5.times { |i|
      5.times { |j|
        assert_equal(matrix[5 * i + j], cmatrix[i, j])
        cmatrix[i, j] = 2.0
        assert_equal(2.0, cmatrix[i, j])
      }
    }
    
    assert_raise(TypeError) { ColorMatrix.new(0) }
    assert_raise(ArgumentError) { ColorMatrix.new(0, 1) }
  end
  
  def test_image_attributes
    # constructor
    assert_instance_of(ImageAttributes, ImageAttributes.new)
    
    # Clone
    attrs = ImageAttributes.new
    clone = attrs.Clone
    assert_not_same(attrs, clone)
    assert_not_equal(attrs.gdiplus_id, clone.gdiplus_id)
    
    # SetColorMatrix, ClearColorMatrix
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetColorMatrix(ColorMatrix.new))
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetColorMatrix(ColorMatrix.new, ColorMatrixFlag.SkipGrays))
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.ClearColorMatrix)
    assert_same(attrs, attrs.SetColorMatrix(ColorMatrix.new, ColorMatrixFlag.SkipGrays, ColorAdjustType.Bitmap))
    assert_same(attrs, attrs.ClearColorMatrix, ColorAdjustType.Bitmap)
    # * ColorMatrixFlag.AltGrays => InvalidParameter
    
    # SetColorMatrices
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetColorMatrices(ColorMatrix.new, ColorMatrix.new))
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetColorMatrices(ColorMatrix.new, ColorMatrix.new, ColorMatrixFlag.SkipGrays))
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetColorMatrices(ColorMatrix.new, ColorMatrix.new, ColorMatrixFlag.SkipGrays, ColorAdjustType.Bitmap))
    
    
    # SetThreshold, ClearThreshold
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetThreshold(0.5))
    assert_same(attrs, attrs.ClearThreshold())
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetThreshold(0.5, ColorAdjustType.Bitmap))
    assert_same(attrs, attrs.ClearThreshold(ColorAdjustType.Bitmap))
    
    # SetGamma, ClearGamma
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetGamma(2.2))
    assert_same(attrs, attrs.ClearGamma())
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetGamma(2.2, ColorAdjustType.Bitmap))
    assert_same(attrs, attrs.ClearGamma(ColorAdjustType.Bitmap))
    
    # SetNoOp, ClearNoOp
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetNoOp())
    assert_same(attrs, attrs.ClearNoOp())
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetNoOp(ColorAdjustType.Bitmap))
    assert_same(attrs, attrs.ClearNoOp(ColorAdjustType.Bitmap))
    
    # SetColorKey, ClearColorKey
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetColorKey(0xffee0000, 0xffff0000))
    assert_same(attrs, attrs.ClearColorKey)
    assert_same(attrs, attrs.SetColorKey(Color.Red, Color.White))
    assert_same(attrs, attrs.ClearColorKey)
    assert_same(attrs, attrs.SetColorKey(:Red, :White))
    assert_same(attrs, attrs.ClearColorKey)
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetColorKey(Color.Red, Color.White, ColorAdjustType.Bitmap))
    assert_same(attrs, attrs.ClearColorKey(ColorAdjustType.Bitmap))
    
    # SetOutputChannel, ClearOutputChannel
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetOutputChannel(ColorChannelFlag.ColorChannelM))
    assert_same(attrs, attrs.ClearOutputChannel())
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetOutputChannel(ColorChannelFlag.ColorChannelM, ColorAdjustType.Bitmap))
    assert_same(attrs, attrs.ClearOutputChannel(ColorAdjustType.Bitmap))
    
    # SetOutputChannelColorProfile, ClearOutputChannelColorProfile
    attrs = ImageAttributes.new
    #assert_same(attrs, attrs.SetOutputChannelColorProfile("sRGB Color Space Profile.icm"))
    assert_same(attrs, attrs.ClearOutputChannelColorProfile())
    attrs = ImageAttributes.new
    #assert_same(attrs, attrs.SetOutputChannelColorProfile("sRGB Color Space Profile.icm", ColorAdjustType.Bitmap))
    assert_same(attrs, attrs.ClearOutputChannelColorProfile(ColorAdjustType.Bitmap))
    
    # SetRemapTable, ClearRemapTable
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetRemapTable({Color.Red => Color.Blue}))
    assert_same(attrs, attrs.ClearRemapTable())
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetRemapTable({Color.Red => Color.Blue}, ColorAdjustType.Bitmap))
    assert_same(attrs, attrs.ClearRemapTable(ColorAdjustType.Bitmap))
    attrs = ImageAttributes.new
    assert_raise(TypeError) { attrs.SetRemapTable({:a => :b}) }
    
    # SetBrushRemapTable
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetBrushRemapTable({Color.Red => Color.Blue}))
    assert_same(attrs, attrs.ClearBrushRemapTable())
    
    # SetWrapMode
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetWrapMode(WrapMode.TileFlipX))
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetWrapMode(WrapMode.TileFlipX, Color.Green))
    attrs = ImageAttributes.new
    assert_same(attrs, attrs.SetWrapMode(WrapMode.TileFlipX, Color.Green, true))
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
