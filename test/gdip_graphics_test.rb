# coding: utf-8
require 'test_helper'

class GdiplusGraphicsTest < Test::Unit::TestCase
  include Gdiplus
  
  def draw(name = nil)
    bmp = Bitmap.new(400, 400)
    bmp.draw { |g|
      g.Clear(:White)
      yield g
    }
  end
  
  def test_properties
    # Clip
    draw { |g|
      rect = RectangleF.new(100.0, 100.0, 100.0, 100.0)
      region = Region.new(rect)
      assert_nothing_raised { g.Clip = region }
      assert_equal(true, g.clip.Equals(region, g))
      
      # ClipBounds
      assert_equal(rect, g.ClipBounds)
    }
    
    # CompositingMode
    draw { |g|
      assert_instance_of(CompositingMode, g.CompositingMode)
      assert_nothing_raised { g.compositing_mode = CompositingMode.SourceCopy }
      assert_nothing_raised { g.CompositingMode = :SourceOver }
      assert_equal(CompositingMode.SourceOver, g.CompositingMode)
    }
    
    # CompositingQuality
    draw { |g|
      assert_instance_of(CompositingQuality, g.CompositingQuality)
      assert_nothing_raised { g.compositing_quality = CompositingQuality.HighQuality }
      assert_nothing_raised { g.CompositingQuality = :GammaCorrected }
      assert_equal(CompositingQuality.GammaCorrected, g.CompositingQuality)
    }
    
    # DpiX, DpiY
    draw { |g|
      assert_instance_of(Float, g.DpiX)
      assert_instance_of(Float, g.DpiY)
    }
    
    # InterpolationMode
    draw { |g|
      assert_instance_of(InterpolationMode, g.InterpolationMode)
      assert_nothing_raised { g. interpolation_mode = InterpolationMode.High }
      assert_nothing_raised { g.InterpolationMode = :Bicubic }
      assert_equal(InterpolationMode.Bicubic, g.InterpolationMode)
    }
    
    # IsClipEmpty, #IsVisibleClipEmpty
    draw { |g|
      assert_equal(false, g.IsClipEmpty)
      assert_equal(false, g.IsVisibleClipEmpty)
      region = Region.new
      region.MakeEmpty
      g.Clip = region
      assert_equal(true, g.IsClipEmpty)
      assert_equal(true, g.IsVisibleClipEmpty)
    }
    
    # PageScale
    draw { |g|
      assert_instance_of(Float, g.PageScale)
      assert_nothing_raised { g.PageScale = 2.0 }
      assert_equal(2.0, g.page_scale)
      assert_nothing_raised { g.PageScale = 1.0 }
    }
    
    # PageUnit
    draw { |g|
      assert_instance_of(GraphicsUnit, g.PageUnit)
      assert_nothing_raised { g. page_unit = GraphicsUnit.Point }
      assert_nothing_raised { g.PageUnit = :Pixel }
      assert_equal(GraphicsUnit.Pixel, g.PageUnit)
    }
    
    # PixelOffsetMode
    draw { |g|
      assert_instance_of(PixelOffsetMode, g.PixelOffsetMode)
      assert_nothing_raised { g. pixel_offset_mode = PixelOffsetMode.HighQuality }
      assert_nothing_raised { g.PixelOffsetMode = :HighSpeed }
      assert_equal(PixelOffsetMode.HighSpeed, g.PixelOffsetMode)
    }
    
    # RenderingOrigin
    draw { |g|
      point = Point.new(2, 2)
      assert_instance_of(Point, g.RenderingOrigin)
      assert_nothing_raised { g.RenderingOrigin = point }
      assert_equal(point, g.rendering_origin)
    }
    
    # SmoothingMode
    draw { |g|
      assert_instance_of(SmoothingMode, g.SmoothingMode)
      assert_nothing_raised { g.smoothing_mode = SmoothingMode.HighQuality }
      assert_nothing_raised { g.SmoothingMode = :AntiAlias }
      assert_equal(SmoothingMode.AntiAlias, g.SmoothingMode)
    }
    
    # TextContrast
    draw { |g|
      assert_kind_of(Integer, g.TextContrast)
      assert_nothing_raised { g.text_contrast = 6 }
      assert_equal(6, g.TextContrast)
    }
    
    # TextRenderingHint
    draw { |g|
      assert_instance_of(TextRenderingHint, g.TextRenderingHint)
      assert_nothing_raised { g.text_rendering_hint = TextRenderingHint.ClearTypeGridFit }
      assert_nothing_raised { g.TextRenderingHint = :AntiAlias }
      assert_equal(TextRenderingHint.AntiAlias, g.TextRenderingHint)
    }
    
    # Transform
    draw { |g|
      assert_instance_of(Matrix, g.Transform)
      matrix = Matrix.new
      matrix.Rotate(45.0)
      assert_nothing_raised { g.transform = matrix }
      assert_equal(matrix, g.transform)
      assert_nothing_raised { g.transform = Matrix.new }
    }
    
    # VisibleClipBounds
    draw { |g|
      rect = RectangleF.new(100.0, 100.0, 400.0, 400.0)
      region = Region.new(rect)
      g.PageUnit = :Pixel
      g.Clip = region
      assert_equal(RectangleF.new(100.0, 100.0, 300.0, 300.0), g.VisibleClipBounds)
      
      region = Region.new
      region.MakeEmpty
      g.Clip = region
      assert_equal(RectangleF.new(0.0, 0.0, 0.0, 0.0), g.VisibleClipBounds)
    }
  end
  
  def test_drawing_methods
    # DrawArc
    draw("DrawArcI1") { |g|
      assert_same(g, g.DrawArc(Pens.Black, Rectangle.new(0, 0, 300, 300), 30, 60))
    }
    draw("DrawArcI2") { |g|
      assert_same(g, g.DrawArc(Pens.Black, 0, 0, 300, 300, 30.0, 60.0))
    }
    draw("DrawArcF1") { |g|
      assert_same(g, g.DrawArc(Pens.Black, RectangleF.new(0.0, 0.0, 300.0, 300.0), 30.0, 60.0))
    }
    draw("DrawArcF2") { |g|
      assert_same(g, g.DrawArc(Pens.Black, 0.0, 0.0, 300.0, 300.0, 30, 60))
    }
    
    # DrawBezier
    draw("DrawBezierI1") { |g|
      assert_same(g, g.DrawBezier(Pens.Black, Point.new(50, 50), Point.new(150, 50), Point.new(150, 150), Point.new(50, 150)))
    }
    draw("DrawBezierI2") { |g|
      assert_same(g, g.DrawBezier(Pens.Black, 50, 50, 150, 50, 150, 150, 50, 150))
    }
    draw("DrawBezierF1") { |g|
      assert_same(g, g.DrawBezier(Pens.Black, PointF.new(50.0, 50.0), PointF.new(150.0, 50.0), PointF.new(150.0, 150.0), PointF.new(50.0, 150.0)))
    }
    draw("DrawBezierF2") { |g|
      assert_same(g, g.DrawBezier(Pens.Black, 50.0, 50.0, 150.0, 50.0, 150.0, 150.0, 50.0, 150.0))
    }
    
    # DrawBeziers
    points = []
    points << Point.new(1, 2)
    points << Point.new(0, 2)
    points << Point.new(0, 1)
    points << Point.new(1, 1)
    points << Point.new(1, 0)
    points << Point.new(2, 0)
    points << Point.new(2, 1)
    points << Point.new(3, 1)
    points << Point.new(3, 2)
    points << Point.new(2, 2)
    points << Point.new(2, 3)
    points << Point.new(1, 3)
    points << Point.new(1, 2)
    pointsI = points.map { |po| Point.new(50 + 100 * po.x, 50 + 100 * po.y) }
    pointsF = points.map { |po| PointF.new(50.0 + 100.0 * po.x, 50.0 + 100.0 * po.y) }
    draw("DrawBeziersI") { |g|
      assert_same(g, g.DrawBeziers(Pens.Black, pointsI))
    }
    draw("DrawBeziersF") { |g|
      assert_same(g, g.DrawBeziers(Pens.Black, pointsF))
    }
    
    # DrawClosedCurve
    draw("DrawClosedCurveI1") { |g|
      assert_same(g, g.DrawClosedCurve(Pens.Black, pointsI))
    }
    draw("DrawClosedCurveI2") { |g|
      assert_same(g, g.DrawClosedCurve(Pens.Black, pointsI, 0.7))
    }
    draw("DrawClosedCurveF1") { |g|
      assert_same(g, g.DrawClosedCurve(Pens.Black, pointsF))
    }
    draw("DrawClosedCurveF2") { |g|
      assert_same(g, g.DrawClosedCurve(Pens.Black, pointsF, 0.7))
    }
    
    # DrawCurve
    draw("DrawCurveI1") { |g|
      assert_same(g, g.DrawCurve(Pens.Black, pointsI))
    }
    draw("DrawCurveI2") { |g|
      assert_same(g, g.DrawCurve(Pens.Black, pointsI, 0.7))
    }
    draw("DrawCurveI3") { |g|
      assert_same(g, g.DrawCurve(Pens.Black, pointsI, 3, 8))
    }
    draw("DrawCurveI4") { |g|
      assert_same(g, g.DrawCurve(Pens.Black, pointsI, 3, 8, 0.3))
    }
    draw("DrawCurveF1") { |g|
      assert_same(g, g.DrawCurve(Pens.Black, pointsF))
    }
    draw("DrawCurveF2") { |g|
      assert_same(g, g.DrawCurve(Pens.Black, pointsF, 0.7))
    }
    draw("DrawCurveF3") { |g|
      assert_same(g, g.DrawCurve(Pens.Black, pointsF, 3, 8))
    }
    draw("DrawCurveF4") { |g|
      assert_same(g, g.DrawCurve(Pens.Black, pointsF, 3, 8, 0.3))
    }
    
    # DrawEllipse
    draw("DrawEllipseI1") { |g|
      assert_same(g, g.DrawEllipse(Pens.Black, Rectangle.new(30, 30, 300, 200)))
    }
    draw("DrawEllipseI2") { |g|
      assert_same(g, g.DrawEllipse(Pens.Black, 30, 30, 300, 200))
    }
    draw("DrawEllipseF1") { |g|
      assert_same(g, g.DrawEllipse(Pens.Black, RectangleF.new(30.0, 30.0, 300.0, 200.0)))
    }
    draw("DrawEllipseF2") { |g|
      assert_same(g, g.DrawEllipse(Pens.Black, 30.0, 30.0, 300.0, 200.0))
    }
    
    # DrawLine
    draw("DrawLineI1") { |g|
      assert_same(g, g.DrawLine(Pens.Black, Point.new(30, 30), Point.new(300, 200)))
    }
    draw("DrawLineI2") { |g|
      assert_same(g, g.DrawLine(Pens.Black, 30, 30, 300, 200))
    }
    draw("DrawLineF1") { |g|
      assert_same(g, g.DrawLine(Pens.Black, PointF.new(30.0, 30.0), PointF.new(300.0, 200.0)))
    }
    draw("DrawLineF2") { |g|
      assert_same(g, g.DrawLine(Pens.Black, 30.0, 30.0, 300.0, 200.0))
    }
    
    # DrawLines
    draw("DrawLinesI") { |g|
      assert_same(g, g.DrawLines(Pens.Black, pointsI))
    }
    draw("DrawLinesF") { |g|
      assert_same(g, g.DrawLines(Pens.Black, pointsF))
    }
    
    # DrawPath
    path = GraphicsPath.new
    path.AddEllipse(100, 100, 100, 100)
    draw("DrawPath") { |g|
      assert_same(g, g.DrawPath(Pens.Black, path))
    }
    
    # DrawPie
    draw("DrawPieI1") { |g|
      assert_same(g, g.DrawPie(Pens.Black, Rectangle.new(0, 0, 300, 300), 30, 60))
    }
    draw("DrawPieI2") { |g|
      assert_same(g, g.DrawPie(Pens.Black, 0, 0, 300, 300, 30.0, 60.0))
    }
    draw("DrawPieF1") { |g|
      assert_same(g, g.DrawPie(Pens.Black, RectangleF.new(0.0, 0.0, 300.0, 300.0), 30.0, 60.0))
    }
    draw("DrawPieF2") { |g|
      assert_same(g, g.DrawPie(Pens.Black, 0.0, 0.0, 300.0, 300.0, 30, 60))
    }
    
    # DrawPolygon
    draw("DrawPolygonI") { |g|
      assert_same(g, g.DrawPolygon(Pens.Black, pointsI))
    }
    draw("DrawPolygonF") { |g|
      assert_same(g, g.DrawPolygon(Pens.Black, pointsF))
    }
    
    # DrawRectangle
    draw("DrawRectangleI1") { |g|
      assert_same(g, g.DrawRectangle(Pens.Black, Rectangle.new(30, 30, 300, 200)))
    }
    draw("DrawRectangleI2") { |g|
      assert_same(g, g.DrawRectangle(Pens.Black, 30, 30, 300, 200))
    }
    draw("DrawRectangleF1") { |g|
      assert_same(g, g.DrawRectangle(Pens.Black, RectangleF.new(30.0, 30.0, 300.0, 200.0)))
    }
    draw("DrawRectangleF2") { |g|
      assert_same(g, g.DrawRectangle(Pens.Black, 30.0, 30.0, 300.0, 200.0))
    }
    
    # DrawRectangles
    rectsI = []
    rectsF = []
    10.times {
      rectsI << Rectangle.new(rand(360), rand(360), rand(200), rand(200))
      rectsF << RectangleF.new(rand(360).to_f, rand(360).to_f, rand(200).to_f, rand(200).to_f)
    }
    draw("DrawRectanglesI") { |g|
      assert_same(g, g.DrawRectangles(Pens.Black, rectsI))
    }
    draw("DrawRectanglesF") { |g|
      assert_same(g, g.DrawRectangles(Pens.Black, rectsF))
    }
    
    # DrawString
    unless InstalledFontCollection.broken?
      draw("DrawStringI") { |g|
        font = Font.new("Arial", 20)
        fmt = StringFormat.new(StringFormatFlags.NoWrap)
        assert_same(g, g.DrawString("TEST1", font, Brushes.Black, 20, 20))
        assert_same(g, g.DrawString("TEST2", font, Brushes.Black, 20, 40), fmt)
        assert_same(g, g.DrawString("TEST3", font, Brushes.Black, Point.new(20, 60)))
        assert_same(g, g.DrawString("TEST4", font, Brushes.Black, Point.new(20, 80)), fmt)
        assert_same(g, g.DrawString("TEST5", font, Brushes.Black, Rectangle.new(20, 100, 300, 20)))
        assert_same(g, g.DrawString("TEST6", font, Brushes.Black, Rectangle.new(20, 120, 300, 20)), fmt)
      }
      
      draw("DrawStringF") { |g|
        font = Font.new("Arial", 20)
        fmt = StringFormat.new(StringFormatFlags.NoWrap)
        assert_same(g, g.DrawString("TEST1", font, Brushes.Black, 20.0, 20.0))
        assert_same(g, g.DrawString("TEST2", font, Brushes.Black, 20.0, 40.0), fmt)
        assert_same(g, g.DrawString("TEST3", font, Brushes.Black, PointF.new(20.0, 60.0)))
        assert_same(g, g.DrawString("TEST4", font, Brushes.Black, PointF.new(20.0, 80.0)), fmt)
        assert_same(g, g.DrawString("TEST5", font, Brushes.Black, RectangleF.new(20.0, 100.0, 300.0, 20.0)))
        assert_same(g, g.DrawString("TEST6", font, Brushes.Black, RectangleF.new(20.0, 120.0, 300.0, 20.0)), fmt)
      }
    end
    
    # FillClosedCurve
    draw("FillClosedCurveI1") { |g|
      assert_same(g, g.FillClosedCurve(Brushes.Black, pointsI))
    }
    draw("FillClosedCurveI2") { |g|
      assert_same(g, g.FillClosedCurve(Brushes.Black, pointsI, FillMode.Alternate, 0.7))
    }
    draw("FillClosedCurveF1") { |g|
      assert_same(g, g.FillClosedCurve(Brushes.Black, pointsF))
    }
    draw("FillClosedCurveF2") { |g|
      assert_same(g, g.FillClosedCurve(Brushes.Black, pointsF, :Winding, 0.7))
    }
    
    # FillEllipse
    draw("FillEllipseI1") { |g|
      assert_same(g, g.FillEllipse(Brushes.Black, Rectangle.new(30, 30, 300, 200)))
    }
    draw("FillEllipseI2") { |g|
      assert_same(g, g.FillEllipse(Brushes.Black, 30, 30, 300, 200))
    }
    draw("FillEllipseF1") { |g|
      assert_same(g, g.FillEllipse(Brushes.Black, RectangleF.new(30.0, 30.0, 300.0, 200.0)))
    }
    draw("FillEllipseF2") { |g|
      assert_same(g, g.FillEllipse(Brushes.Black, 30.0, 30.0, 300.0, 200.0))
    }
    
    # FillPath
    path = GraphicsPath.new
    path.AddEllipse(100, 100, 100, 100)
    draw("FillPath") { |g|
      assert_same(g, g.FillPath(Brushes.Black, path))
    }
    
    # FillPie
    draw("FillPieI1") { |g|
      assert_same(g, g.FillPie(Brushes.Black, Rectangle.new(0, 0, 300, 300), 30, 60))
    }
    draw("FillPieI2") { |g|
      assert_same(g, g.FillPie(Brushes.Black, 0, 0, 300, 300, 30.0, 60.0))
    }
    draw("FillPieF1") { |g|
      assert_same(g, g.FillPie(Brushes.Black, RectangleF.new(0.0, 0.0, 300.0, 300.0), 30.0, 60.0))
    }
    draw("FillPieF2") { |g|
      assert_same(g, g.FillPie(Brushes.Black, 0.0, 0.0, 300.0, 300.0, 30, 60))
    }
    
    # FillPolygon
    draw("FillPolygonI1") { |g|
      assert_same(g, g.FillPolygon(Brushes.Black, pointsI))
    }
    draw("FillPolygonI2") { |g|
      assert_same(g, g.FillPolygon(Brushes.Black, pointsI, FillMode.Winding))
    }
    draw("FillPolygonF1") { |g|
      assert_same(g, g.FillPolygon(Brushes.Black, pointsF))
    }
    draw("FillPolygonF2") { |g|
      assert_same(g, g.FillPolygon(Brushes.Black, pointsF, :Winding))
    }
    
    # FillRectangle
    draw("FillRectangleI1") { |g|
      assert_same(g, g.FillRectangle(Brushes.Black, Rectangle.new(30, 30, 300, 200)))
    }
    draw("FillRectangleI2") { |g|
      assert_same(g, g.FillRectangle(Brushes.Black, 30, 30, 300, 200))
    }
    draw("FillRectangleF1") { |g|
      assert_same(g, g.FillRectangle(Brushes.Black, RectangleF.new(30.0, 30.0, 300.0, 200.0)))
    }
    draw("FillRectangleF2") { |g|
      assert_same(g, g.FillRectangle(Brushes.Black, 30.0, 30.0, 300.0, 200.0))
    }
    
    # FillRectangles
    rectsI = []
    rectsF = []
    10.times {
      rectsI << Rectangle.new(rand(360), rand(360), rand(200), rand(200))
      rectsF << RectangleF.new(rand(360).to_f, rand(360).to_f, rand(200).to_f, rand(200).to_f)
    }
    draw("FillRectanglesI") { |g|
      assert_same(g, g.FillRectangles(Brushes.Black, rectsI))
    }
    draw("FillRectanglesF") { |g|
      assert_same(g, g.FillRectangles(Brushes.Black, rectsF))
    }
    
    # FillRegion
    path = GraphicsPath.new
    path.AddEllipse(100, 100, 100, 100)
    region = Region.new(path)
    draw("FillRegion") { |g|
      assert_same(g, g.FillRegion(Brushes.Black, region))
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
