# coding: utf-8
require 'test_helper'

class GdiplusEnumIntTest < Test::Unit::TestCase
  include Gdiplus

  def test_PixelFormat
    assert_equal(0, PixelFormat::Undefined.to_i)
    assert_match(/Undefined/, PixelFormat::Undefined.inspect)
    assert_equal(0, PixelFormat::DontCare.to_i)
    assert_match(/Undefined/, PixelFormat::DontCare.inspect) #

    assert_equal(0x00010000, PixelFormat::Indexed.to_i)
    assert_match(/Indexed/, PixelFormat::Indexed.inspect)
    assert_equal(0x00020000, PixelFormat::GDI.to_i)
    assert_match(/GDI/, PixelFormat::GDI.inspect)
    assert_equal(0x00040000, PixelFormat::Alpha.to_i)
    assert_match(/Alpha/, PixelFormat::Alpha.inspect)
    assert_equal(0x00080000, PixelFormat::PAlpha.to_i)
    assert_match(/PAlpha/, PixelFormat::PAlpha.inspect)
    assert_equal(0x00100000, PixelFormat::Extended.to_i)
    assert_match(/Extended/, PixelFormat::Extended.inspect)
    assert_equal(0x00200000, PixelFormat::Canonical.to_i)
    assert_match(/Canonical/, PixelFormat::Canonical.inspect)
    assert_equal(16, PixelFormat::Max.to_i)
    assert_match(/Max/, PixelFormat::Max.inspect)

    _PixelFormat1bppIndexed = (1 | ( 1 << 8) | PixelFormat::Indexed | PixelFormat::GDI)
    _PixelFormat4bppIndexed = (2 | ( 4 << 8) | PixelFormat::Indexed | PixelFormat::GDI)
    _PixelFormat8bppIndexed = (3 | ( 8 << 8) | PixelFormat::Indexed | PixelFormat::GDI)
    _PixelFormat16bppGrayScale = (4 | (16 << 8) | PixelFormat::Extended)
    _PixelFormat16bppRGB555 = (5 | (16 << 8) | PixelFormat::GDI)
    _PixelFormat16bppRGB565 = (6 | (16 << 8) | PixelFormat::GDI)
    _PixelFormat16bppARGB1555 = (7 | (16 << 8) | PixelFormat::Alpha | PixelFormat::GDI)
    _PixelFormat24bppRGB = (8 | (24 << 8) | PixelFormat::GDI)
    _PixelFormat32bppRGB = (9 | (32 << 8) | PixelFormat::GDI)
    _PixelFormat32bppARGB = (10 | (32 << 8) | PixelFormat::Alpha | PixelFormat::GDI | PixelFormat::Canonical)
    _PixelFormat32bppPARGB = (11 | (32 << 8) | PixelFormat::Alpha | PixelFormat::PAlpha | PixelFormat::GDI)
    _PixelFormat48bppRGB = (12 | (48 << 8) | PixelFormat::Extended)
    _PixelFormat64bppARGB = (13 | (64 << 8) | PixelFormat::Alpha  | PixelFormat::Canonical | PixelFormat::Extended)
    _PixelFormat64bppPARGB = (14 | (64 << 8) | PixelFormat::Alpha  | PixelFormat::PAlpha | PixelFormat::Extended)
    _PixelFormat32bppCMYK = (15 | (32 << 8))
    _PixelFormatMax = 16

    assert_equal(_PixelFormat1bppIndexed, PixelFormat::Format1bppIndexed.to_i)
    assert_match(/Format1bppIndexed/, PixelFormat::Format1bppIndexed.inspect)
    assert_equal(_PixelFormat4bppIndexed, PixelFormat::Format4bppIndexed.to_i)
    assert_match(/Format4bppIndexed/, PixelFormat::Format4bppIndexed.inspect)
    assert_equal(_PixelFormat4bppIndexed, PixelFormat::Format4bppIndexed.to_i)
    assert_match(/Format4bppIndexed/, PixelFormat::Format4bppIndexed.inspect)
    assert_equal(_PixelFormat8bppIndexed, PixelFormat::Format8bppIndexed.to_i)
    assert_match(/Format8bppIndexed/, PixelFormat::Format8bppIndexed.inspect)
    assert_equal(_PixelFormat16bppGrayScale, PixelFormat::Format16bppGrayScale.to_i)
    assert_match(/Format16bppGrayScale/, PixelFormat::Format16bppGrayScale.inspect)
    assert_equal(_PixelFormat16bppRGB555, PixelFormat::Format16bppRGB555.to_i)
    assert_match(/Format16bppRGB555/, PixelFormat::Format16bppRGB555.inspect)
    assert_equal(_PixelFormat16bppRGB565, PixelFormat::Format16bppRGB565.to_i)
    assert_match(/Format16bppRGB565/, PixelFormat::Format16bppRGB565.inspect)
    assert_equal(_PixelFormat16bppARGB1555, PixelFormat::Format16bppARGB1555.to_i)
    assert_match(/Format16bppARGB1555/, PixelFormat::Format16bppARGB1555.inspect)
    assert_equal(_PixelFormat24bppRGB, PixelFormat::Format24bppRGB.to_i)
    assert_match(/Format24bppRGB/, PixelFormat::Format24bppRGB.inspect)
    assert_equal(_PixelFormat32bppRGB, PixelFormat::Format32bppRGB.to_i)
    assert_match(/Format32bppRGB/, PixelFormat::Format32bppRGB.inspect)
    assert_equal(_PixelFormat32bppARGB, PixelFormat::Format32bppARGB.to_i)
    assert_match(/Format32bppARGB/, PixelFormat::Format32bppARGB.inspect)
    assert_equal(_PixelFormat32bppPARGB, PixelFormat::Format32bppPARGB.to_i)
    assert_match(/Format32bppPARGB/, PixelFormat::Format32bppPARGB.inspect)
    assert_equal(_PixelFormat48bppRGB, PixelFormat::Format48bppRGB.to_i)
    assert_match(/Format48bppRGB/, PixelFormat::Format48bppRGB.inspect)
    assert_equal(_PixelFormat64bppARGB, PixelFormat::Format64bppARGB.to_i)
    assert_match(/Format64bppARGB/, PixelFormat::Format64bppARGB.inspect)
    assert_equal(_PixelFormat64bppPARGB, PixelFormat::Format64bppPARGB.to_i)
    assert_match(/Format64bppPARGB/, PixelFormat::Format64bppPARGB.inspect)
    assert_equal(_PixelFormat32bppCMYK, PixelFormat::Format32bppCMYK.to_i)
    assert_match(/Format32bppCMYK/, PixelFormat::Format32bppCMYK.inspect)
  end

  def test_enumint
    assert_equal(PixelFormat::Format32bppARGB.object_id, PixelFormat.Format32bppARGB.object_id)
    #<Gdiplus::PixelFormat.Format32bppARGB: 0x0026200a>
    assert_match(/Format32bppARGB/, PixelFormat::Format32bppARGB.inspect)
    assert_match(/#{PixelFormat::Format32bppARGB.to_i.to_s(16)}/, PixelFormat::Format32bppARGB.inspect)
    assert(PixelFormat::Undefined == 0)
    assert_equal(1, 1 + PixelFormat::Undefined)
    assert_equal(0x30000, PixelFormat.Indexed | PixelFormat.GDI)
    assert_raise(TypeError) { "str" + PixelFormat::Undefined }
  end

  def test_EncoderValue
    assert_equal(0, EncoderValue.ColorTypeCMYK.to_i)
    assert_match(/ColorTypeCMYK/, EncoderValue.ColorTypeCMYK.inspect)
    assert_equal(1, EncoderValue.ColorTypeYCCK.to_i)
    assert_match(/ColorTypeYCCK/, EncoderValue.ColorTypeYCCK.inspect)
    assert_equal(2, EncoderValue.CompressionLZW.to_i)
    assert_match(/CompressionLZW/, EncoderValue.CompressionLZW.inspect)
    assert_equal(3, EncoderValue.CompressionCCITT3.to_i)
    assert_match(/CompressionCCITT3/, EncoderValue.CompressionCCITT3.inspect)
    assert_equal(4, EncoderValue.CompressionCCITT4.to_i)
    assert_match(/CompressionCCITT4/, EncoderValue.CompressionCCITT4.inspect)
    assert_equal(5, EncoderValue.CompressionRle.to_i)
    assert_match(/CompressionRle/, EncoderValue.CompressionRle.inspect)
    assert_equal(6, EncoderValue.CompressionNone.to_i)
    assert_match(/CompressionNone/, EncoderValue.CompressionNone.inspect)
    assert_equal(7, EncoderValue.ScanMethodInterlaced.to_i)
    assert_match(/ScanMethodInterlaced/, EncoderValue.ScanMethodInterlaced.inspect)
    assert_equal(8, EncoderValue.ScanMethodNonInterlaced.to_i)
    assert_match(/ScanMethodNonInterlaced/, EncoderValue.ScanMethodNonInterlaced.inspect)
    assert_equal(9, EncoderValue.VersionGif87.to_i)
    assert_match(/VersionGif87/, EncoderValue.VersionGif87.inspect)
    assert_equal(10, EncoderValue.VersionGif89.to_i)
    assert_match(/VersionGif89/, EncoderValue.VersionGif89.inspect)
    assert_equal(11, EncoderValue.RenderProgressive.to_i)
    assert_match(/RenderProgressive/, EncoderValue.RenderProgressive.inspect)
    assert_equal(12, EncoderValue.RenderNonProgressive.to_i)
    assert_match(/RenderNonProgressive/, EncoderValue.RenderNonProgressive.inspect)
    assert_equal(13, EncoderValue.TransformRotate90.to_i)
    assert_match(/TransformRotate90/, EncoderValue.TransformRotate90.inspect)
    assert_equal(14, EncoderValue.TransformRotate180.to_i)
    assert_match(/TransformRotate180/, EncoderValue.TransformRotate180.inspect)
    assert_equal(15, EncoderValue.TransformRotate270.to_i)
    assert_match(/TransformRotate270/, EncoderValue.TransformRotate270.inspect)
    assert_equal(16, EncoderValue.TransformFlipHorizontal.to_i)
    assert_match(/TransformFlipHorizontal/, EncoderValue.TransformFlipHorizontal.inspect)
    assert_equal(17, EncoderValue.TransformFlipVertical.to_i)
    assert_match(/TransformFlipVertical/, EncoderValue.TransformFlipVertical.inspect)
    assert_equal(18, EncoderValue.MultiFrame.to_i)
    assert_match(/MultiFrame/, EncoderValue.MultiFrame.inspect)
    assert_equal(19, EncoderValue.LastFrame.to_i)
    assert_match(/LastFrame/, EncoderValue.LastFrame.inspect)
    assert_equal(20, EncoderValue.Flush.to_i)
    assert_match(/Flush/, EncoderValue.Flush.inspect)
    assert_equal(21, EncoderValue.FrameDimensionTime.to_i)
    assert_match(/FrameDimensionTime/, EncoderValue.FrameDimensionTime.inspect)
    assert_equal(22, EncoderValue.FrameDimensionResolution.to_i)
    assert_match(/FrameDimensionResolution/, EncoderValue.FrameDimensionResolution.inspect)
    assert_equal(23, EncoderValue.FrameDimensionPage.to_i)
    assert_match(/FrameDimensionPage/, EncoderValue.FrameDimensionPage.inspect)
    assert_equal(24, EncoderValue.ColorTypeGray.to_i)
    assert_match(/ColorTypeGray/, EncoderValue.ColorTypeGray.inspect)
    assert_equal(25, EncoderValue.ColorTypeRGB.to_i)
    assert_match(/ColorTypeRGB/, EncoderValue.ColorTypeRGB.inspect)
  end

  def test_EncoderParameter
    assert_equal(1, EncoderParameterValueType.ValueTypeByte.to_i)
    assert_match(/ValueTypeByte/, EncoderParameterValueType.ValueTypeByte.inspect)
    assert_equal(2, EncoderParameterValueType.ValueTypeAscii.to_i)
    assert_match(/ValueTypeAscii/, EncoderParameterValueType.ValueTypeAscii.inspect)
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

  def test_Encoder
    assert_kind_of(Guid, Encoder.Quality)
    assert_equal("1d5be4b5-fa4a-452d-9cdd-5db35105e7eb", Encoder.Quality.to_s)
    assert_match(/Compression/, Encoder.Compression.inspect)
    assert_match(/ColorDepth/, Encoder.ColorDepth.inspect)
    assert_match(/ScanMethod/, Encoder.ScanMethod.inspect)
    assert_match(/Version/, Encoder.Version.inspect)
    assert_match(/RenderMethod/, Encoder.RenderMethod.inspect)
    assert_match(/Quality/, Encoder.Quality.inspect)
    assert_match(/Transformation/, Encoder.Transformation.inspect)
    assert_match(/LuminanceTable/, Encoder.LuminanceTable.inspect)
    assert_match(/ChrominanceTable/, Encoder.ChrominanceTable.inspect)
    assert_match(/SaveFlag/, Encoder.SaveFlag.inspect)
  end

  def test_BrushType
    assert_equal(0, BrushType.SolidColor.to_i)
    assert_match(/SolidColor/, BrushType.SolidColor.inspect)
    assert_equal(1, BrushType.HatchFill.to_i)
    assert_match(/HatchFill/, BrushType.HatchFill.inspect)
    assert_equal(2, BrushType.TextureFill.to_i)
    assert_match(/TextureFill/, BrushType.TextureFill.inspect)
    assert_equal(3, BrushType.PathGradient.to_i)
    assert_match(/PathGradient/, BrushType.PathGradient.inspect)
    assert_equal(4, BrushType.LinearGradient.to_i)
    assert_match(/LinearGradient/, BrushType.LinearGradient.inspect)
  end

  def test_CustomLineCapType
    assert_equal(0, CustomLineCapType.Default.to_i)
    assert_match(/Default/, CustomLineCapType.Default.inspect)
    assert_equal(1, CustomLineCapType.AdjustableArrow.to_i)
    assert_match(/AdjustableArrow/, CustomLineCapType.AdjustableArrow.inspect)
  end

  def test_DashCap
    assert_equal(0, DashCap.Flat.to_i)
    assert_match(/Flat/, DashCap.Flat.inspect)
    assert_equal(2, DashCap.Round.to_i)
    assert_match(/Round/, DashCap.Round.inspect)
    assert_equal(3, DashCap.Triangle.to_i)
    assert_match(/Triangle/, DashCap.Triangle.inspect)
  end

  def test_DashStyle
    assert_equal(0, DashStyle.Solid.to_i)
    assert_match(/Solid/, DashStyle.Solid.inspect)
    assert_equal(1, DashStyle.Dash.to_i)
    assert_match(/Dash/, DashStyle.Dash.inspect)
    assert_equal(2, DashStyle.Dot.to_i)
    assert_match(/Dot/, DashStyle.Dot.inspect)
    assert_equal(3, DashStyle.DashDot.to_i)
    assert_match(/DashDot/, DashStyle.DashDot.inspect)
    assert_equal(4, DashStyle.DashDotDot.to_i)
    assert_match(/DashDotDot/, DashStyle.DashDotDot.inspect)
    assert_equal(5, DashStyle.Custom.to_i)
    assert_match(/Custom/, DashStyle.Custom.inspect)
  end

  def test_LineCap
    assert_equal(0, LineCap.Flat.to_i)
    assert_match(/Flat/, LineCap.Flat.inspect)
    assert_equal(1, LineCap.Square.to_i)
    assert_match(/Square/, LineCap.Square.inspect)
    assert_equal(2, LineCap.Round.to_i)
    assert_match(/Round/, LineCap.Round.inspect)
    assert_equal(3, LineCap.Triangle.to_i)
    assert_match(/Triangle/, LineCap.Triangle.inspect)
    assert_equal(16, LineCap.NoAnchor.to_i)
    assert_match(/NoAnchor/, LineCap.NoAnchor.inspect)
    assert_equal(17, LineCap.SquareAnchor.to_i)
    assert_match(/SquareAnchor/, LineCap.SquareAnchor.inspect)
    assert_equal(18, LineCap.RoundAnchor.to_i)
    assert_match(/RoundAnchor/, LineCap.RoundAnchor.inspect)
    assert_equal(19, LineCap.DiamondAnchor.to_i)
    assert_match(/DiamondAnchor/, LineCap.DiamondAnchor.inspect)
    assert_equal(20, LineCap.ArrowAnchor.to_i)
    assert_match(/ArrowAnchor/, LineCap.ArrowAnchor.inspect)
    assert_equal(255, LineCap.Custom.to_i)
    assert_match(/Custom/, LineCap.Custom.inspect)
    assert_equal(240, LineCap.AnchorMask.to_i)
    assert_match(/AnchorMask/, LineCap.AnchorMask.inspect)
  end

  def test_LineJoin
    assert_equal(0, LineJoin.Miter.to_i)
    assert_match(/Miter/, LineJoin.Miter.inspect)
    assert_equal(1, LineJoin.Bevel.to_i)
    assert_match(/Bevel/, LineJoin.Bevel.inspect)
    assert_equal(2, LineJoin.Round.to_i)
    assert_match(/Round/, LineJoin.Round.inspect)
    assert_equal(3, LineJoin.MiterClipped.to_i)
    assert_match(/MiterClipped/, LineJoin.MiterClipped.inspect)
  end

  def test_MatrixOrder
    assert_equal(0, MatrixOrder.Prepend.to_i)
    assert_match(/Prepend/, MatrixOrder.Prepend.inspect)
    assert_equal(1, MatrixOrder.Append.to_i)
    assert_match(/Append/, MatrixOrder.Append.inspect)
  end

  def test_PenAlignment
    assert_equal(0, PenAlignment.Center.to_i)
    assert_match(/Center/, PenAlignment.Center.inspect)
    assert_equal(1, PenAlignment.Inset.to_i)
    assert_match(/Inset/, PenAlignment.Inset.inspect)
  end

  def test_PenType
    assert_equal(0, PenType.SolidColor.to_i)
    assert_match(/SolidColor/, PenType.SolidColor.inspect)
    assert_equal(1, PenType.HatchFill.to_i)
    assert_match(/HatchFill/, PenType.HatchFill.inspect)
    assert_equal(2, PenType.TextureFill.to_i)
    assert_match(/TextureFill/, PenType.TextureFill.inspect)
    assert_equal(3, PenType.PathGradient.to_i)
    assert_match(/PathGradient/, PenType.PathGradient.inspect)
    assert_equal(4, PenType.LinearGradient.to_i)
    assert_match(/LinearGradient/, PenType.LinearGradient.inspect)
    assert_equal(-1, PenType.Unknown.to_i)
    assert_match(/Unknown/, PenType.Unknown.inspect)
  end

  def test_FontStyle
    assert_equal(0, FontStyle.Regular.to_i)
    assert_match(/Regular/, FontStyle.Regular.inspect)
    assert_equal(1, FontStyle.Bold.to_i)
    assert_match(/Bold/, FontStyle.Bold.inspect)
    assert_equal(2, FontStyle.Italic.to_i)
    assert_match(/Italic/, FontStyle.Italic.inspect)
    #assert_equal(3, FontStyle.BoldItalic.to_i)
    #assert_match(/BoldItalic/, FontStyle.BoldItalic.inspect)
    assert_equal(4, FontStyle.Underline.to_i)
    assert_match(/Underline/, FontStyle.Underline.inspect)
    assert_equal(8, FontStyle.Strikeout.to_i)
    assert_match(/Strikeout/, FontStyle.Strikeout.inspect)

    # EnumFlags
    assert_equal(FontStyle.new(3), FontStyle.Bold | FontStyle.Italic)
    assert_equal(FontStyle.new(3), FontStyle.new(:Bold, :Italic))
    assert_equal(FontStyle.new(3), FontStyle.new | :Bold | :Italic)
    assert_equal(FontStyle.new(3), FontStyle.Bold.Italic)
  end

  def test_GenericFontFamilies
    assert_equal(0, GenericFontFamilies.Serif.to_i)
    assert_match(/Serif/, GenericFontFamilies.Serif.inspect)
    assert_equal(1, GenericFontFamilies.SansSerif.to_i)
    assert_match(/SansSerif/, GenericFontFamilies.SansSerif.inspect)
    assert_equal(2, GenericFontFamilies.Monospace.to_i)
    assert_match(/Monospace/, GenericFontFamilies.Monospace.inspect)
  end

  def test_PixelOffsetMode
    assert_equal(-1, PixelOffsetMode.Invalid.to_i)
    assert_match(/Invalid/, PixelOffsetMode.Invalid.inspect)
    assert_equal(0, PixelOffsetMode.Default.to_i)
    assert_match(/Default/, PixelOffsetMode.Default.inspect)
    assert_equal(1, PixelOffsetMode.HighSpeed.to_i)
    assert_match(/HighSpeed/, PixelOffsetMode.HighSpeed.inspect)
    assert_equal(2, PixelOffsetMode.HighQuality.to_i)
    assert_match(/HighQuality/, PixelOffsetMode.HighQuality.inspect)
    assert_equal(3, PixelOffsetMode.None.to_i)
    assert_match(/None/, PixelOffsetMode.None.inspect)
    assert_equal(4, PixelOffsetMode.Half.to_i)
    assert_match(/Half/, PixelOffsetMode.Half.inspect)
  end

  def test_SmoothingMode
    assert_equal(-1, SmoothingMode.Invalid.to_i)
    assert_match(/Invalid/, SmoothingMode.Invalid.inspect)
    assert_equal(0, SmoothingMode.Default.to_i)
    assert_match(/Default/, SmoothingMode.Default.inspect)
    assert_equal(1, SmoothingMode.HighSpeed.to_i)
    assert_match(/HighSpeed/, SmoothingMode.HighSpeed.inspect)
    assert_equal(2, SmoothingMode.HighQuality.to_i)
    assert_match(/HighQuality/, SmoothingMode.HighQuality.inspect)
    assert_equal(3, SmoothingMode.None.to_i)
    assert_match(/None/, SmoothingMode.None.inspect)
    assert_equal(4, SmoothingMode.AntiAlias.to_i)
    assert_match(/AntiAlias/, SmoothingMode.AntiAlias.inspect)
    assert_equal(4, SmoothingMode.AntiAlias8x4.to_i)
    assert_match(/AntiAlias/, SmoothingMode.AntiAlias8x4.inspect) #
    assert_equal(5, SmoothingMode.AntiAlias8x8.to_i)
    assert_match(/AntiAlias8x8/, SmoothingMode.AntiAlias8x8.inspect)
  end

  def test_TextRenderingHint
    assert_equal(0, TextRenderingHint.SystemDefault.to_i)
    assert_match(/SystemDefault/, TextRenderingHint.SystemDefault.inspect)
    assert_equal(1, TextRenderingHint.SingleBitPerPixelGridFit.to_i)
    assert_match(/SingleBitPerPixelGridFit/, TextRenderingHint.SingleBitPerPixelGridFit.inspect)
    assert_equal(2, TextRenderingHint.SingleBitPerPixel.to_i)
    assert_match(/SingleBitPerPixel/, TextRenderingHint.SingleBitPerPixel.inspect)
    assert_equal(3, TextRenderingHint.AntiAliasGridFit.to_i)
    assert_match(/AntiAliasGridFit/, TextRenderingHint.AntiAliasGridFit.inspect)
    assert_equal(4, TextRenderingHint.AntiAlias.to_i)
    assert_match(/AntiAlias/, TextRenderingHint.AntiAlias.inspect)
    assert_equal(5, TextRenderingHint.ClearTypeGridFit.to_i)
    assert_match(/ClearTypeGridFit/, TextRenderingHint.ClearTypeGridFit.inspect)
  end

  def test_CompositingMode
    assert_equal(0, CompositingMode.SourceOver.to_i)
    assert_match(/SourceOver/, CompositingMode.SourceOver.inspect)
    assert_equal(1, CompositingMode.SourceCopy.to_i)
    assert_match(/SourceCopy/, CompositingMode.SourceCopy.inspect)
  end

  def test_CompositingQuality
    assert_equal(-1, CompositingQuality.Invalid.to_i)
    assert_match(/Invalid/, CompositingQuality.Invalid.inspect)
    assert_equal(0, CompositingQuality.Default.to_i)
    assert_match(/Default/, CompositingQuality.Default.inspect)
    assert_equal(1, CompositingQuality.HighSpeed.to_i)
    assert_match(/HighSpeed/, CompositingQuality.HighSpeed.inspect)
    assert_equal(2, CompositingQuality.HighQuality.to_i)
    assert_match(/HighQuality/, CompositingQuality.HighQuality.inspect)
    assert_equal(3, CompositingQuality.GammaCorrected.to_i)
    assert_match(/GammaCorrected/, CompositingQuality.GammaCorrected.inspect)
    assert_equal(4, CompositingQuality.AssumeLinear.to_i)
    assert_match(/AssumeLinear/, CompositingQuality.AssumeLinear.inspect)
  end

  def test_FillMode
    assert_equal(0, FillMode.Alternate.to_i)
    assert_match(/Alternate/, FillMode.Alternate.inspect)
    assert_equal(1, FillMode.Winding.to_i)
    assert_match(/Winding/, FillMode.Winding.inspect)
  end

  def test_GraphicsUnit
    assert_equal(0, GraphicsUnit.World.to_i)
    assert_match(/World/, GraphicsUnit.World.inspect)
    assert_equal(1, GraphicsUnit.Display.to_i)
    assert_match(/Display/, GraphicsUnit.Display.inspect)
    assert_equal(2, GraphicsUnit.Pixel.to_i)
    assert_match(/Pixel/, GraphicsUnit.Pixel.inspect)
    assert_equal(3, GraphicsUnit.Point.to_i)
    assert_match(/Point/, GraphicsUnit.Point.inspect)
    assert_equal(4, GraphicsUnit.Inch.to_i)
    assert_match(/Inch/, GraphicsUnit.Inch.inspect)
    assert_equal(5, GraphicsUnit.Document.to_i)
    assert_match(/Document/, GraphicsUnit.Document.inspect)
    assert_equal(6, GraphicsUnit.Millimeter.to_i)
    assert_match(/Millimeter/, GraphicsUnit.Millimeter.inspect)
  end

  def test_InterpolationMode
    assert_equal(-1, InterpolationMode.Invalid.to_i)
    assert_match(/Invalid/, InterpolationMode.Invalid.inspect)
    assert_equal(0, InterpolationMode.Default.to_i)
    assert_match(/Default/, InterpolationMode.Default.inspect)
    assert_equal(1, InterpolationMode.Low.to_i)
    assert_match(/Low/, InterpolationMode.Low.inspect)
    assert_equal(2, InterpolationMode.High.to_i)
    assert_match(/High/, InterpolationMode.High.inspect)
    assert_equal(3, InterpolationMode.Bilinear.to_i)
    assert_match(/Bilinear/, InterpolationMode.Bilinear.inspect)
    assert_equal(4, InterpolationMode.Bicubic.to_i)
    assert_match(/Bicubic/, InterpolationMode.Bicubic.inspect)
    assert_equal(5, InterpolationMode.NearestNeighbor.to_i)
    assert_match(/NearestNeighbor/, InterpolationMode.NearestNeighbor.inspect)
    assert_equal(6, InterpolationMode.HighQualityBilinear.to_i)
    assert_match(/HighQualityBilinear/, InterpolationMode.HighQualityBilinear.inspect)
    assert_equal(7, InterpolationMode.HighQualityBicubic.to_i)
    assert_match(/HighQualityBicubic/, InterpolationMode.HighQualityBicubic.inspect)
  end

  def test_DriverStringOptions
    assert_equal(0x00000001, DriverStringOptions.CmapLookup.to_i)
    assert_match(/CmapLookup/, DriverStringOptions.CmapLookup.inspect)
    assert_equal(0x00000002, DriverStringOptions.Vertical.to_i)
    assert_match(/Vertical/, DriverStringOptions.Vertical.inspect)
    assert_equal(0x00000004, DriverStringOptions.RealizedAdvance.to_i)
    assert_match(/RealizedAdvance/, DriverStringOptions.RealizedAdvance.inspect)
    assert_equal(0x00000008, DriverStringOptions.LimitSubpixel.to_i)
    assert_match(/LimitSubpixel/, DriverStringOptions.LimitSubpixel.inspect)
  end

  def test_FlushIntention
    assert_equal(0, FlushIntention.Flush.to_i)
    assert_match(/Flush/, FlushIntention.Flush.inspect)
    assert_equal(1, FlushIntention.Sync.to_i)
    assert_match(/Sync/, FlushIntention.Sync.inspect)
  end

  def test_HotkeyPrefix
    assert_equal(0, HotkeyPrefix.None.to_i)
    assert_match(/None/, HotkeyPrefix.None.inspect)
    assert_equal(1, HotkeyPrefix.Show.to_i)
    assert_match(/Show/, HotkeyPrefix.Show.inspect)
    assert_equal(2, HotkeyPrefix.Hide.to_i)
    assert_match(/Hide/, HotkeyPrefix.Hide.inspect)
  end

  def test_StringAlignment
    assert_equal(0, StringAlignment.Near.to_i)
    assert_match(/Near/, StringAlignment.Near.inspect)
    assert_equal(1, StringAlignment.Center.to_i)
    assert_match(/Center/, StringAlignment.Center.inspect)
    assert_equal(2, StringAlignment.Far.to_i)
    assert_match(/Far/, StringAlignment.Far.inspect)
  end

  def test_StringDigitSubstitute
    assert_equal(0, StringDigitSubstitute.User.to_i)
    assert_match(/User/, StringDigitSubstitute.User.inspect)
    assert_equal(1, StringDigitSubstitute.None.to_i)
    assert_match(/None/, StringDigitSubstitute.None.inspect)
    assert_equal(2, StringDigitSubstitute.National.to_i)
    assert_match(/National/, StringDigitSubstitute.National.inspect)
    assert_equal(3, StringDigitSubstitute.Traditional.to_i)
    assert_match(/Traditional/, StringDigitSubstitute.Traditional.inspect)
  end

  def test_StringFormatFlags
    assert_equal(0x00000001, StringFormatFlags.DirectionRightToLeft.to_i)
    assert_match(/DirectionRightToLeft/, StringFormatFlags.DirectionRightToLeft.inspect)
    assert_equal(0x00000002, StringFormatFlags.DirectionVertical.to_i)
    assert_match(/DirectionVertical/, StringFormatFlags.DirectionVertical.inspect)
    assert_equal(0x00000004, StringFormatFlags.NoFitBlackBox.to_i)
    assert_match(/NoFitBlackBox/, StringFormatFlags.NoFitBlackBox.inspect)
    assert_equal(0x00000020, StringFormatFlags.DisplayFormatControl.to_i)
    assert_match(/DisplayFormatControl/, StringFormatFlags.DisplayFormatControl.inspect)
    assert_equal(0x00000400, StringFormatFlags.NoFontFallback.to_i)
    assert_match(/NoFontFallback/, StringFormatFlags.NoFontFallback.inspect)
    assert_equal(0x00000800, StringFormatFlags.MeasureTrailingSpaces.to_i)
    assert_match(/MeasureTrailingSpaces/, StringFormatFlags.MeasureTrailingSpaces.inspect)
    assert_equal(0x00001000, StringFormatFlags.NoWrap.to_i)
    assert_match(/NoWrap/, StringFormatFlags.NoWrap.inspect)
    assert_equal(0x00002000, StringFormatFlags.LineLimit.to_i)
    assert_match(/LineLimit/, StringFormatFlags.LineLimit.inspect)
    assert_equal(0x00004000, StringFormatFlags.NoClip.to_i)
    assert_match(/NoClip/, StringFormatFlags.NoClip.inspect)
  end

  def test_StringTrimming
    assert_equal(0, StringTrimming.None.to_i)
    assert_match(/None/, StringTrimming.None.inspect)
    assert_equal(1, StringTrimming.Character.to_i)
    assert_match(/Character/, StringTrimming.Character.inspect)
    assert_equal(2, StringTrimming.Word.to_i)
    assert_match(/Word/, StringTrimming.Word.inspect)
    assert_equal(3, StringTrimming.EllipsisCharacter.to_i)
    assert_match(/EllipsisCharacter/, StringTrimming.EllipsisCharacter.inspect)
    assert_equal(4, StringTrimming.EllipsisWord.to_i)
    assert_match(/EllipsisWord/, StringTrimming.EllipsisWord.inspect)
    assert_equal(5, StringTrimming.EllipsisPath.to_i)
    assert_match(/EllipsisPath/, StringTrimming.EllipsisPath.inspect)
  end

  def test_PathPointType
    assert_equal(0x00000000, PathPointType.Start.to_i)
    assert_match(/Start/, PathPointType.Start.inspect)
    assert_equal(true, PathPointType.Start.start?)
    assert_equal(false, PathPointType.Start.line?)
    assert_equal(false, PathPointType.Start.bezier?)
    assert_equal(0x00000001, PathPointType.Line.to_i)
    assert_match(/Line/, PathPointType.Line.inspect)
    assert_equal(false, PathPointType.Line.start?)
    assert_equal(true, PathPointType.Line.line?)
    assert_equal(false, PathPointType.Line.bezier?)
    assert_equal(0x00000003, PathPointType.Bezier.to_i)
    assert_match(/Bezier/, PathPointType.Bezier.inspect)
    assert_equal(false, PathPointType.Bezier.start?)
    assert_equal(false, PathPointType.Bezier.line?)
    assert_equal(true, PathPointType.Bezier.bezier?)
    assert_equal(0x00000007, PathPointType.PathTypeMask.to_i)
    assert_match(/PathTypeMask/, PathPointType.PathTypeMask.inspect)
    assert_equal(0x00000010, PathPointType.DashMode.to_i)
    assert_match(/DashMode/, PathPointType.DashMode.inspect)
    assert_equal(true, PathPointType.DashMode.dash_mode?)
    assert_equal(false, PathPointType.DashMode.path_marker?)
    assert_equal(false, PathPointType.DashMode.close_subpath?)
    assert_equal(0x00000020, PathPointType.PathMarker.to_i)
    assert_match(/PathMarker/, PathPointType.PathMarker.inspect)
    assert_equal(false, PathPointType.PathMarker.dash_mode?)
    assert_equal(true, PathPointType.PathMarker.path_marker?)
    assert_equal(false, PathPointType.PathMarker.close_subpath?)
    assert_equal(0x00000080, PathPointType.CloseSubpath.to_i)
    assert_match(/CloseSubpath/, PathPointType.CloseSubpath.inspect)
    assert_equal(false, PathPointType.CloseSubpath.dash_mode?)
    assert_equal(false, PathPointType.CloseSubpath.path_marker?)
    assert_equal(true, PathPointType.CloseSubpath.close_subpath?)
  end

  def test_CombineMode
    assert_equal(0, CombineMode.Replace.to_i)
    assert_match(/Replace/, CombineMode.Replace.inspect)
    assert_equal(1, CombineMode.Intersect.to_i)
    assert_match(/Intersect/, CombineMode.Intersect.inspect)
    assert_equal(2, CombineMode.Union.to_i)
    assert_match(/Union/, CombineMode.Union.inspect)
    assert_equal(3, CombineMode.Xor.to_i)
    assert_match(/Xor/, CombineMode.Xor.inspect)
    assert_equal(4, CombineMode.Exclude.to_i)
    assert_match(/Exclude/, CombineMode.Exclude.inspect)
    assert_equal(5, CombineMode.Complement.to_i)
    assert_match(/Complement/, CombineMode.Complement.inspect)
  end

  def test_CoordinateSpace
    assert_equal(0, CoordinateSpace.World.to_i)
    assert_match(/World/, CoordinateSpace.World.inspect)
    assert_equal(1, CoordinateSpace.Page.to_i)
    assert_match(/Page/, CoordinateSpace.Page.inspect)
    assert_equal(2, CoordinateSpace.Device.to_i)
    assert_match(/Device/, CoordinateSpace.Device.inspect)
  end

  def test_HatchStyle
    assert_equal(0, HatchStyle.Horizontal.to_i)
    assert_match(/Horizontal/, HatchStyle.Horizontal.inspect)
    assert_equal(1, HatchStyle.Vertical.to_i)
    assert_match(/Vertical/, HatchStyle.Vertical.inspect)
    assert_equal(2, HatchStyle.ForwardDiagonal.to_i)
    assert_match(/ForwardDiagonal/, HatchStyle.ForwardDiagonal.inspect)
    assert_equal(3, HatchStyle.BackwardDiagonal.to_i)
    assert_match(/BackwardDiagonal/, HatchStyle.BackwardDiagonal.inspect)
    assert_equal(4, HatchStyle.Cross.to_i)
    assert_match(/Cross/, HatchStyle.Cross.inspect)
    assert_equal(5, HatchStyle.DiagonalCross.to_i)
    assert_match(/DiagonalCross/, HatchStyle.DiagonalCross.inspect)
    assert_equal(6, HatchStyle.Percent05.to_i)
    assert_match(/Percent05/, HatchStyle.Percent05.inspect)
    assert_equal(7, HatchStyle.Percent10.to_i)
    assert_match(/Percent10/, HatchStyle.Percent10.inspect)
    assert_equal(8, HatchStyle.Percent20.to_i)
    assert_match(/Percent20/, HatchStyle.Percent20.inspect)
    assert_equal(9, HatchStyle.Percent25.to_i)
    assert_match(/Percent25/, HatchStyle.Percent25.inspect)
    assert_equal(10, HatchStyle.Percent30.to_i)
    assert_match(/Percent30/, HatchStyle.Percent30.inspect)
    assert_equal(11, HatchStyle.Percent40.to_i)
    assert_match(/Percent40/, HatchStyle.Percent40.inspect)
    assert_equal(12, HatchStyle.Percent50.to_i)
    assert_match(/Percent50/, HatchStyle.Percent50.inspect)
    assert_equal(13, HatchStyle.Percent60.to_i)
    assert_match(/Percent60/, HatchStyle.Percent60.inspect)
    assert_equal(14, HatchStyle.Percent70.to_i)
    assert_match(/Percent70/, HatchStyle.Percent70.inspect)
    assert_equal(15, HatchStyle.Percent75.to_i)
    assert_match(/Percent75/, HatchStyle.Percent75.inspect)
    assert_equal(16, HatchStyle.Percent80.to_i)
    assert_match(/Percent80/, HatchStyle.Percent80.inspect)
    assert_equal(17, HatchStyle.Percent90.to_i)
    assert_match(/Percent90/, HatchStyle.Percent90.inspect)
    assert_equal(18, HatchStyle.LightDownwardDiagonal.to_i)
    assert_match(/LightDownwardDiagonal/, HatchStyle.LightDownwardDiagonal.inspect)
    assert_equal(19, HatchStyle.LightUpwardDiagonal.to_i)
    assert_match(/LightUpwardDiagonal/, HatchStyle.LightUpwardDiagonal.inspect)
    assert_equal(20, HatchStyle.DarkDownwardDiagonal.to_i)
    assert_match(/DarkDownwardDiagonal/, HatchStyle.DarkDownwardDiagonal.inspect)
    assert_equal(21, HatchStyle.DarkUpwardDiagonal.to_i)
    assert_match(/DarkUpwardDiagonal/, HatchStyle.DarkUpwardDiagonal.inspect)
    assert_equal(22, HatchStyle.WideDownwardDiagonal.to_i)
    assert_match(/WideDownwardDiagonal/, HatchStyle.WideDownwardDiagonal.inspect)
    assert_equal(23, HatchStyle.WideUpwardDiagonal.to_i)
    assert_match(/WideUpwardDiagonal/, HatchStyle.WideUpwardDiagonal.inspect)
    assert_equal(24, HatchStyle.LightVertical.to_i)
    assert_match(/LightVertical/, HatchStyle.LightVertical.inspect)
    assert_equal(25, HatchStyle.LightHorizontal.to_i)
    assert_match(/LightHorizontal/, HatchStyle.LightHorizontal.inspect)
    assert_equal(26, HatchStyle.NarrowVertical.to_i)
    assert_match(/NarrowVertical/, HatchStyle.NarrowVertical.inspect)
    assert_equal(27, HatchStyle.NarrowHorizontal.to_i)
    assert_match(/NarrowHorizontal/, HatchStyle.NarrowHorizontal.inspect)
    assert_equal(28, HatchStyle.DarkVertical.to_i)
    assert_match(/DarkVertical/, HatchStyle.DarkVertical.inspect)
    assert_equal(29, HatchStyle.DarkHorizontal.to_i)
    assert_match(/DarkHorizontal/, HatchStyle.DarkHorizontal.inspect)
    assert_equal(30, HatchStyle.DashedDownwardDiagonal.to_i)
    assert_match(/DashedDownwardDiagonal/, HatchStyle.DashedDownwardDiagonal.inspect)
    assert_equal(31, HatchStyle.DashedUpwardDiagonal.to_i)
    assert_match(/DashedUpwardDiagonal/, HatchStyle.DashedUpwardDiagonal.inspect)
    assert_equal(32, HatchStyle.DashedHorizontal.to_i)
    assert_match(/DashedHorizontal/, HatchStyle.DashedHorizontal.inspect)
    assert_equal(33, HatchStyle.DashedVertical.to_i)
    assert_match(/DashedVertical/, HatchStyle.DashedVertical.inspect)
    assert_equal(34, HatchStyle.SmallConfetti.to_i)
    assert_match(/SmallConfetti/, HatchStyle.SmallConfetti.inspect)
    assert_equal(35, HatchStyle.LargeConfetti.to_i)
    assert_match(/LargeConfetti/, HatchStyle.LargeConfetti.inspect)
    assert_equal(36, HatchStyle.ZigZag.to_i)
    assert_match(/ZigZag/, HatchStyle.ZigZag.inspect)
    assert_equal(37, HatchStyle.Wave.to_i)
    assert_match(/Wave/, HatchStyle.Wave.inspect)
    assert_equal(38, HatchStyle.DiagonalBrick.to_i)
    assert_match(/DiagonalBrick/, HatchStyle.DiagonalBrick.inspect)
    assert_equal(39, HatchStyle.HorizontalBrick.to_i)
    assert_match(/HorizontalBrick/, HatchStyle.HorizontalBrick.inspect)
    assert_equal(40, HatchStyle.Weave.to_i)
    assert_match(/Weave/, HatchStyle.Weave.inspect)
    assert_equal(41, HatchStyle.Plaid.to_i)
    assert_match(/Plaid/, HatchStyle.Plaid.inspect)
    assert_equal(42, HatchStyle.Divot.to_i)
    assert_match(/Divot/, HatchStyle.Divot.inspect)
    assert_equal(43, HatchStyle.DottedGrid.to_i)
    assert_match(/DottedGrid/, HatchStyle.DottedGrid.inspect)
    assert_equal(44, HatchStyle.DottedDiamond.to_i)
    assert_match(/DottedDiamond/, HatchStyle.DottedDiamond.inspect)
    assert_equal(45, HatchStyle.Shingle.to_i)
    assert_match(/Shingle/, HatchStyle.Shingle.inspect)
    assert_equal(46, HatchStyle.Trellis.to_i)
    assert_match(/Trellis/, HatchStyle.Trellis.inspect)
    assert_equal(47, HatchStyle.Sphere.to_i)
    assert_match(/Sphere/, HatchStyle.Sphere.inspect)
    assert_equal(48, HatchStyle.SmallGrid.to_i)
    assert_match(/SmallGrid/, HatchStyle.SmallGrid.inspect)
    assert_equal(49, HatchStyle.SmallCheckerBoard.to_i)
    assert_match(/SmallCheckerBoard/, HatchStyle.SmallCheckerBoard.inspect)
    assert_equal(50, HatchStyle.LargeCheckerBoard.to_i)
    assert_match(/LargeCheckerBoard/, HatchStyle.LargeCheckerBoard.inspect)
    assert_equal(51, HatchStyle.OutlinedDiamond.to_i)
    assert_match(/OutlinedDiamond/, HatchStyle.OutlinedDiamond.inspect)
    assert_equal(52, HatchStyle.SolidDiamond.to_i)
    assert_match(/SolidDiamond/, HatchStyle.SolidDiamond.inspect)
    #assert_equal(53, HatchStyle.Total.to_i)
    #assert_match(/Total/, HatchStyle.Total.inspect)
    assert_equal(4, HatchStyle.LargeGrid.to_i) # alias
    assert_equal(0, HatchStyle.Min.to_i) # alias
    assert_equal(52, HatchStyle.Max.to_i) # alias
  end

  def test_ImageType
    assert_equal(0, ImageType.Unknown.to_i)
    assert_match(/Unknown/, ImageType.Unknown.inspect)
    assert_equal(1, ImageType.Bitmap.to_i)
    assert_match(/Bitmap/, ImageType.Bitmap.inspect)
    assert_equal(2, ImageType.Metafile.to_i)
    assert_match(/Metafile/, ImageType.Metafile.inspect)
  end

  def test_LinearGradientMode
    assert_equal(0, LinearGradientMode.Horizontal.to_i)
    assert_match(/Horizontal/, LinearGradientMode.Horizontal.inspect)
    assert_equal(1, LinearGradientMode.Vertical.to_i)
    assert_match(/Vertical/, LinearGradientMode.Vertical.inspect)
    assert_equal(2, LinearGradientMode.ForwardDiagonal.to_i)
    assert_match(/ForwardDiagonal/, LinearGradientMode.ForwardDiagonal.inspect)
    assert_equal(3, LinearGradientMode.BackwardDiagonal.to_i)
    assert_match(/BackwardDiagonal/, LinearGradientMode.BackwardDiagonal.inspect)
  end

  def test_WarpMode
    assert_equal(0, WarpMode.Perspective.to_i)
    assert_match(/Perspective/, WarpMode.Perspective.inspect)
    assert_equal(1, WarpMode.Bilinear.to_i)
    assert_match(/Bilinear/, WarpMode.Bilinear.inspect)
  end

  def test_WrapMode
    assert_equal(0, WrapMode.Tile.to_i)
    assert_match(/Tile/, WrapMode.Tile.inspect)
    assert_equal(1, WrapMode.TileFlipX.to_i)
    assert_match(/TileFlipX/, WrapMode.TileFlipX.inspect)
    assert_equal(2, WrapMode.TileFlipY.to_i)
    assert_match(/TileFlipY/, WrapMode.TileFlipY.inspect)
    assert_equal(3, WrapMode.TileFlipXY.to_i)
    assert_match(/TileFlipXY/, WrapMode.TileFlipXY.inspect)
    assert_equal(4, WrapMode.Clamp.to_i)
    assert_match(/Clamp/, WrapMode.Clamp.inspect)
  end

  def test_ColorAdjustType
    assert_equal(0, ColorAdjustType.Default.to_i)
    assert_match(/Default/, ColorAdjustType.Default.inspect)
    assert_equal(1, ColorAdjustType.Bitmap.to_i)
    assert_match(/Bitmap/, ColorAdjustType.Bitmap.inspect)
    assert_equal(2, ColorAdjustType.Brush.to_i)
    assert_match(/Brush/, ColorAdjustType.Brush.inspect)
    assert_equal(3, ColorAdjustType.Pen.to_i)
    assert_match(/Pen/, ColorAdjustType.Pen.inspect)
    assert_equal(4, ColorAdjustType.Text.to_i)
    assert_match(/Text/, ColorAdjustType.Text.inspect)
    assert_equal(5, ColorAdjustType.Count.to_i)
    assert_match(/Count/, ColorAdjustType.Count.inspect)
    assert_equal(6, ColorAdjustType.Any.to_i)
    assert_match(/Any/, ColorAdjustType.Any.inspect)
  end

  def test_ColorChannelFlag
    assert_equal(0, ColorChannelFlag.ColorChannelC.to_i)
    assert_match(/ColorChannelC/, ColorChannelFlag.ColorChannelC.inspect)
    assert_equal(1, ColorChannelFlag.ColorChannelM.to_i)
    assert_match(/ColorChannelM/, ColorChannelFlag.ColorChannelM.inspect)
    assert_equal(2, ColorChannelFlag.ColorChannelY.to_i)
    assert_match(/ColorChannelY/, ColorChannelFlag.ColorChannelY.inspect)
    assert_equal(3, ColorChannelFlag.ColorChannelK.to_i)
    assert_match(/ColorChannelK/, ColorChannelFlag.ColorChannelK.inspect)
    assert_equal(4, ColorChannelFlag.ColorChannelLast.to_i)
    assert_match(/ColorChannelLast/, ColorChannelFlag.ColorChannelLast.inspect)

    assert_equal(0, ColorChannelFlag.C.to_i)
    assert_equal(1, ColorChannelFlag.M.to_i)
    assert_equal(2, ColorChannelFlag.Y.to_i)
    assert_equal(3, ColorChannelFlag.K.to_i)
    assert_equal(4, ColorChannelFlag.Last.to_i)
  end

  def test_ColorMatrixFlag
    assert_equal(0, ColorMatrixFlag.Default.to_i)
    assert_match(/Default/, ColorMatrixFlag.Default.inspect)
    assert_equal(1, ColorMatrixFlag.SkipGrays.to_i)
    assert_match(/SkipGrays/, ColorMatrixFlag.SkipGrays.inspect)
    assert_equal(2, ColorMatrixFlag.AltGray.to_i)
    assert_match(/AltGray/, ColorMatrixFlag.AltGray.inspect)
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
