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
  
  def bmp2
    bmp = Bitmap.new(512, 512)
    bmp.draw { |g|
      g.Clear(:Green)
    }
    bmp
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
    
    # DrawImage
    # DrawImagePoint
    draw("DrawImagePointI1") { |g|
      assert_same(g, g.DrawImagePoint(bmp2, 100, 100))
    }
    draw("DrawImagePointI2") { |g|
      assert_same(g, g.DrawImagePoint(bmp2, Point.new(100, 100)))
    }
    draw("DrawImagePointF1") { |g|
      assert_same(g, g.DrawImagePoint(bmp2, 100.0, 100.0))
    }
    draw("DrawImagePointF2") { |g|
      assert_same(g, g.DrawImagePoint(bmp2, PointF.new(100.0, 100.0)))
    }
    
    draw("DrawImage_DrawImagePointI1") { |g|
      assert_same(g, g.DrawImage(bmp2, 100, 100))
    }
    draw("DrawImage_DrawImagePointI2") { |g|
      assert_same(g, g.DrawImage(bmp2, Point.new(100, 100)))
    }
    draw("DrawImage_DrawImagePointF1") { |g|
      assert_same(g, g.DrawImage(bmp2, 100.0, 100.0))
    }
    draw("DrawImage_DrawImagePointF2") { |g|
      assert_same(g, g.DrawImage(bmp2, PointF.new(100.0, 100.0)))
    }
    
    # DrawImageRect
    draw("DrawImageRectI1") { |g|
      assert_same(g, g.DrawImageRect(bmp2, 100, 100, 200, 200))
    }
    draw("DrawImageRectI2") { |g|
      assert_same(g, g.DrawImageRect(bmp2, Rectangle.new(100, 100, 200, 200)))
    }
    draw("DrawImageRectF1") { |g|
      assert_same(g, g.DrawImageRect(bmp2, 100.0, 100.0, 200.0, 200.0))
    }
    draw("DrawImageRectF2") { |g|
      assert_same(g, g.DrawImageRect(bmp2, RectangleF.new(100.0, 100.0, 200.0, 200.0)))
    }
    
    draw("DrawImage_DrawImageRectI1") { |g|
      assert_same(g, g.DrawImage(bmp2, 100, 100, 200, 200))
    }
    draw("DrawImage_DrawImageRectI2") { |g|
      assert_same(g, g.DrawImage(bmp2, Rectangle.new(100, 100, 200, 200)))
    }
    draw("DrawImage_DrawImageRectF1") { |g|
      assert_same(g, g.DrawImage(bmp2, 100.0, 100.0, 200.0, 200.0))
    }
    draw("DrawImage_DrawImageRectF2") { |g|
      assert_same(g, g.DrawImage(bmp2, RectangleF.new(100.0, 100.0, 200.0, 200.0)))
    }
    
    # DrawImagePoints
    points3I = [Point.new(130, 100), Point.new(330, 100), Point.new(100, 300)]
    draw("DrawImagePointsI") { |g|
      assert_same(g, g.DrawImagePoints(bmp2, points3I))
    }
    points3F = [PointF.new(130.0, 100.0), PointF.new(330.0, 100.0), PointF.new(100.0, 300.0)]
    draw("DrawImagePointsF") { |g|
      assert_same(g, g.DrawImagePoints(bmp2, points3F))
    }
    
    draw("DrawImage_DrawImagePointsI") { |g|
      assert_same(g, g.DrawImage(bmp2, points3I))
    }
    draw("DrawImage_DrawImagePointsF") { |g|
      assert_same(g, g.DrawImage(bmp2, points3F))
    }
    
    # DrawImagePointRect
    rect_numsI = [181, 181, 208, 208]
    rectI = Rectangle.new(*rect_numsI)
    rect_numsF = [181.0, 181.0, 208.0, 208.0]
    rectF = RectangleF.new(*rect_numsF)
    
    draw("DrawImagePointRectI1") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, Point.new(100, 100), rectI))
    }
    draw("DrawImagePointRectI2") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, Point.new(100, 100), rectI, GraphicsUnit.Pixel))
    }
    draw("DrawImagePointRectI3") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, Point.new(100, 100), *rect_numsI))
    }
    draw("DrawImagePointRectI4") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, Point.new(100, 100), *rect_numsI, GraphicsUnit.Pixel))
    }
    draw("DrawImagePointRectI5") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, 100, 100, rectI))
    }
    draw("DrawImagePointRectI6") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, 100, 100, rectI, GraphicsUnit.Pixel))
    }
    draw("DrawImagePointRectI7") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, 100, 100, *rect_numsI))
    }
    draw("DrawImagePointRectI8") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, 100, 100, *rect_numsI, GraphicsUnit.Pixel))
    }
    
    draw("DrawImage_DrawImagePointRectI1") { |g|
      assert_same(g, g.DrawImage(bmp2, Point.new(100, 100), rectI))
    }
    draw("DrawImage_DrawImagePointRectI2") { |g|
      assert_same(g, g.DrawImage(bmp2, Point.new(100, 100), rectI, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImagePointRectI3") { |g|
      assert_same(g, g.DrawImage(bmp2, Point.new(100, 100), *rect_numsI))
    }
    draw("DrawImage_DrawImagePointRectI4") { |g|
      assert_same(g, g.DrawImage(bmp2, Point.new(100, 100), *rect_numsI, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImagePointRectI5") { |g|
      assert_same(g, g.DrawImage(bmp2, 100, 100, rectI))
    }
    draw("DrawImage_DrawImagePointRectI6") { |g|
      assert_same(g, g.DrawImage(bmp2, 100, 100, rectI, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImagePointRectI7") { |g|
      assert_same(g, g.DrawImage(bmp2, 100, 100, *rect_numsI))
    }
    draw("DrawImage_DrawImagePointRectI8") { |g|
      assert_same(g, g.DrawImage(bmp2, 100, 100, *rect_numsI, GraphicsUnit.Pixel))
    }
    
    draw("DrawImagePointRectF1") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, PointF.new(100.0, 100.0), rectF))
    }
    draw("DrawImagePointRectF2") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, PointF.new(100.0, 100.0), rectF, GraphicsUnit.Pixel))
    }
    draw("DrawImagePointRectF3") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, PointF.new(100.0, 100.0), *rect_numsF))
    }
    draw("DrawImagePointRectF4") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, PointF.new(100.0, 100.0), *rect_numsF, GraphicsUnit.Pixel))
    }
    draw("DrawImagePointRectF5") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, 100.0, 100.0, rectF))
    }
    draw("DrawImagePointRectF6") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, 100.0, 100.0, rectF, GraphicsUnit.Pixel))
    }
    draw("DrawImagePointRectF7") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, 100.0, 100.0, *rect_numsF))
    }
    draw("DrawImagePointRectF8") { |g|
      assert_same(g, g.DrawImagePointRect(bmp2, 100.0, 100.0, *rect_numsF, GraphicsUnit.Pixel))
    }
    
    draw("DrawImage_DrawImagePointRectF1") { |g|
      assert_same(g, g.DrawImage(bmp2, PointF.new(100.0, 100.0), rectF))
    }
    draw("DrawImage_DrawImagePointRectF2") { |g|
      assert_same(g, g.DrawImage(bmp2, PointF.new(100.0, 100.0), rectF, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImagePointRectF3") { |g|
      assert_same(g, g.DrawImage(bmp2, PointF.new(100.0, 100.0), *rect_numsF))
    }
    draw("DrawImage_DrawImagePointRectF4") { |g|
      assert_same(g, g.DrawImage(bmp2, PointF.new(100.0, 100.0), *rect_numsF, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImagePointRectF5") { |g|
      assert_same(g, g.DrawImage(bmp2, 100.0, 100.0, rectF))
    }
    draw("DrawImage_DrawImagePointRectF6") { |g|
      assert_same(g, g.DrawImage(bmp2, 100.0, 100.0, rectF, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImagePointRectF7") { |g|
      assert_same(g, g.DrawImage(bmp2, 100.0, 100.0, *rect_numsF))
    }
    draw("DrawImage_DrawImagePointRectF8") { |g|
      assert_same(g, g.DrawImage(bmp2, 100.0, 100.0, *rect_numsF, GraphicsUnit.Pixel))
    }
    
    # DrawImageRectRect
    dest_rect_numsI = [100, 100, 200, 200]
    dest_rectI = Rectangle.new(*dest_rect_numsI)
    dest_rect_numsF = [100.0, 100.0, 200.0, 200.0]
    dest_rectF = RectangleF.new(*dest_rect_numsF)
    
    
    draw("DrawImageRectRectI1A") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectI, rectI))
    }
    draw("DrawImageRectRectI1B") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectI, rectI, GraphicsUnit.Pixel))
    }
    draw("DrawImageRectRectI1C") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectI, rectI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImageRectRectI2A") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectI, *rect_numsI))
    }
    draw("DrawImageRectRectI2B") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectI, *rect_numsI, GraphicsUnit.Pixel))
    }
    draw("DrawImageRectRectI2C") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectI, *rect_numsI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImageRectRectI3A") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsI, rectI))
    }
    draw("DrawImageRectRectI3B") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsI, rectI, GraphicsUnit.Pixel))
    }
    draw("DrawImageRectRectI3C") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsI, rectI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImageRectRectI4A") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsI, *rect_numsI))
    }
    draw("DrawImageRectRectI4B") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsI, *rect_numsI, GraphicsUnit.Pixel))
    }
    draw("DrawImageRectRectI4C") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsI, *rect_numsI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    
    draw("DrawImageRectRectF1A") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectF, rectF))
    }
    draw("DrawImageRectRectF1B") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectF, rectF, GraphicsUnit.Pixel))
    }
    draw("DrawImageRectRectF1C") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectF, rectF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImageRectRectF2A") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectF, *rect_numsF))
    }
    draw("DrawImageRectRectF2B") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectF, *rect_numsF, GraphicsUnit.Pixel))
    }
    draw("DrawImageRectRectF2C") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, dest_rectF, *rect_numsF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImageRectRectF3A") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsF, rectF))
    }
    draw("DrawImageRectRectF3B") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsF, rectF, GraphicsUnit.Pixel))
    }
    draw("DrawImageRectRectF3C") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsF, rectF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImageRectRectF4A") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsF, *rect_numsF))
    }
    draw("DrawImageRectRectF4B") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsF, *rect_numsF, GraphicsUnit.Pixel))
    }
    draw("DrawImageRectRectF4C") { |g|
      assert_same(g, g.DrawImageRectRect(bmp2, *dest_rect_numsF, *rect_numsF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    
    draw("DrawImage_DrawImageRectRectI1A") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectI, rectI))
    }
    draw("DrawImage_DrawImageRectRectI1B") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectI, rectI, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImageRectRectI1C") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectI, rectI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImage_DrawImageRectRectI2A") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectI, *rect_numsI))
    }
    draw("DrawImage_DrawImageRectRectI2B") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectI, *rect_numsI, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImageRectRectI2C") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectI, *rect_numsI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImage_DrawImageRectRectI3A") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsI, rectI))
    }
    draw("DrawImage_DrawImageRectRectI3B") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsI, rectI, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImageRectRectI3C") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsI, rectI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImage_DrawImageRectRectI4A") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsI, *rect_numsI))
    }
    draw("DrawImage_DrawImageRectRectI4B") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsI, *rect_numsI, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImageRectRectI4C") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsI, *rect_numsI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    
    draw("DrawImage_DrawImageRectRectF1A") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectF, rectF))
    }
    draw("DrawImage_DrawImageRectRectF1B") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectF, rectF, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImageRectRectF1C") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectF, rectF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImage_DrawImageRectRectF2A") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectF, *rect_numsF))
    }
    draw("DrawImage_DrawImageRectRectF2B") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectF, *rect_numsF, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImageRectRectF2C") { |g|
      assert_same(g, g.DrawImage(bmp2, dest_rectF, *rect_numsF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImage_DrawImageRectRectF3A") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsF, rectF))
    }
    draw("DrawImage_DrawImageRectRectF3B") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsF, rectF, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImageRectRectF3C") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsF, rectF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    draw("DrawImage_DrawImageRectRectF4A") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsF, *rect_numsF))
    }
    draw("DrawImage_DrawImageRectRectF4B") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsF, *rect_numsF, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImageRectRectF4C") { |g|
      assert_same(g, g.DrawImage(bmp2, *dest_rect_numsF, *rect_numsF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    
    # DrawImagePointsRect
    draw("DrawImagePointsRectI1A") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3I, rectI))
    }
    draw("DrawImagePointsRectI1B") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3I, rectI, GraphicsUnit.Pixel))
    }
    draw("DrawImagePointsRectI1C") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3I, rectI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    
    draw("DrawImagePointsRectI2A") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3I, *rect_numsI))
    }
    draw("DrawImagePointsRectI2B") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3I, *rect_numsI, GraphicsUnit.Pixel))
    }
    draw("DrawImagePointsRectI2C") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3I, *rect_numsI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    
    draw("DrawImagePointsRectF1A") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3F, rectF))
    }
    draw("DrawImagePointsRectF1B") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3F, rectF, GraphicsUnit.Pixel))
    }
    draw("DrawImagePointsRectF1C") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3F, rectF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    
    draw("DrawImagePointsRectF2A") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3F, *rect_numsF))
    }
    draw("DrawImagePointsRectF2B") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3F, *rect_numsF, GraphicsUnit.Pixel))
    }
    draw("DrawImagePointsRectF2C") { |g|
      assert_same(g, g.DrawImagePointsRect(bmp2, points3F, *rect_numsF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    
    draw("DrawImage_DrawImagePointsRectI1A") { |g|
      assert_same(g, g.DrawImage(bmp2, points3I, rectI))
    }
    draw("DrawImage_DrawImagePointsRectI1B") { |g|
      assert_same(g, g.DrawImage(bmp2, points3I, rectI, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImagePointsRectI1C") { |g|
      assert_same(g, g.DrawImage(bmp2, points3I, rectI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    
    draw("DrawImage_DrawImagePointsRectI2A") { |g|
      assert_same(g, g.DrawImage(bmp2, points3I, *rect_numsI))
    }
    draw("DrawImage_DrawImagePointsRectI2B") { |g|
      assert_same(g, g.DrawImage(bmp2, points3I, *rect_numsI, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImagePointsRectI2C") { |g|
      assert_same(g, g.DrawImage(bmp2, points3I, *rect_numsI, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    
    draw("DrawImage_DrawImagePointsRectF1A") { |g|
      assert_same(g, g.DrawImage(bmp2, points3F, rectF))
    }
    draw("DrawImage_DrawImagePointsRectF1B") { |g|
      assert_same(g, g.DrawImage(bmp2, points3F, rectF, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImagePointsRectF1C") { |g|
      assert_same(g, g.DrawImage(bmp2, points3F, rectF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
    
    draw("DrawImage_DrawImagePointsRectF2A") { |g|
      assert_same(g, g.DrawImage(bmp2, points3F, *rect_numsF))
    }
    draw("DrawImage_DrawImagePointsRectF2B") { |g|
      assert_same(g, g.DrawImage(bmp2, points3F, *rect_numsF, GraphicsUnit.Pixel))
    }
    draw("DrawImage_DrawImagePointsRectF2C") { |g|
      assert_same(g, g.DrawImage(bmp2, points3F, *rect_numsF, GraphicsUnit.Pixel, ImageAttributes.new))
    }
  end
  
  def test_clip_methods
    # SetClip
    draw("SetClip_rect1") { |g|
      assert_same(g, g.SetClip(Rectangle.new(100, 100, 200, 200)))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    # SetClip
    draw("SetClip_rect2") { |g|
      assert_same(g, g.SetClip(Rectangle.new(100, 100, 200, 200), CombineMode.Xor))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    draw("SetClip_rect3") { |g|
      assert_same(g, g.SetClip(RectangleF.new(100.0, 100.0, 200.0, 200.0)))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    # SetClip
    draw("SetClip_rect4") { |g|
      assert_same(g, g.SetClip(RectangleF.new(100.0, 100.0, 200.0, 200.0), :Exclude))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    
    clip_path = GraphicsPath.new
    clip_path.AddEllipse(100, 100, 200, 200)
    
    draw("SetClip_path1") { |g|
      assert_same(g, g.SetClip(clip_path))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    draw("SetClip_path2") { |g|
      assert_same(g, g.SetClip(clip_path, CombineMode.Xor))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    
    clip_region = Region.new(clip_path)
    
    draw("SetClip_region1") { |g|
      assert_same(g, g.SetClip(clip_region))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    draw("SetClip_region2") { |g|
      assert_same(g, g.SetClip(clip_region, CombineMode.Xor))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    
    draw("SetClip_graphics1") { |g|
      draw { |g2|
        g2.SetClip(clip_path)
        assert_same(g, g.SetClip(g2))
        g.DrawImagePoint(bmp2, 0, 0)
      }
    }
    draw("SetClip_graphics2") { |g|
      draw { |g2|
        g2.SetClip(clip_path)
        assert_same(g, g.SetClip(g2, CombineMode.Xor))
        g.DrawImagePoint(bmp2, 0, 0)
      }
    }
    
    # ExcludeClip
    draw("ExcludeClip_rect1") { |g|
      g.SetClip(Rectangle.new(0, 0, 200, 400))
      assert_same(g, g.ExcludeClip(Rectangle.new(100, 100, 200, 200)))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    draw("ExcludeClip_rect2") { |g|
      g.SetClip(Rectangle.new(0, 0, 200, 400))
      assert_same(g, g.ExcludeClip(RectangleF.new(100.0, 100.0, 200.0, 200.0)))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    
    draw("ExcludeClip_region") { |g|
      g.SetClip(Rectangle.new(0, 0, 200, 400))
      assert_same(g, g.ExcludeClip(clip_region))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    
    # IntersectClip
    draw("IntersectClip_rect1") { |g|
      g.SetClip(Rectangle.new(0, 0, 200, 400))
      assert_same(g, g.IntersectClip(Rectangle.new(100, 100, 200, 200)))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    draw("IntersectClip_rect2") { |g|
      g.SetClip(Rectangle.new(0, 0, 200, 400))
      assert_same(g, g.IntersectClip(RectangleF.new(100.0, 100.0, 200.0, 200.0)))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    
    draw("IntersectClip_region") { |g|
      g.SetClip(Rectangle.new(0, 0, 200, 400))
      assert_same(g, g.IntersectClip(clip_region))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    
    # ResetClip
    draw("ResetClip") { |g|
      g.SetClip(Rectangle.new(100, 100, 200, 200))
      assert_same(g, g.ResetClip)
      g.DrawImagePoint(bmp2, 0, 0)
    }
    
    # TranslateClip
    draw("TranslateClip1") { |g|
      g.SetClip(clip_region)
      assert_same(g, g.TranslateClip(100, 100))
      g.DrawImagePoint(bmp2, 0, 0)
    }
    draw("TranslateClip2") { |g|
      g.SetClip(clip_region)
      assert_same(g, g.TranslateClip(100.0, 100.0))
      g.DrawImagePoint(bmp2, 0, 0)
    }
  end
  
  def test_transform_methods
    # TranslateTransform
    draw("TranslateTransformI1") { |g|
      assert_same(g, g.TranslateTransform(100, 100))
      g.DrawImage(bmp2, 0, 0)
    }
    draw("TranslateTransformI2") { |g|
      assert_same(g, g.TranslateTransform(100, 100, MatrixOrder.Append))
      g.DrawImage(bmp2, 0, 0)
    }
    draw("TranslateTransformF1") { |g|
      assert_same(g, g.TranslateTransform(100.0, 100.0))
      g.DrawImage(bmp2, 0.0, 0.0)
    }
    draw("TranslateTransformF2") { |g|
      assert_same(g, g.TranslateTransform(100.0, 100.0, :Append))
      g.DrawImage(bmp2, 0.0, 0.0)
    }
    
    # ScaleTransform
    draw("ScaleTransform1") { |g|
      assert_same(g, g.ScaleTransform(2, 0.5))
      g.DrawImage(bmp2, 0, 0)
    }
    draw("ScaleTransform2") { |g|
      assert_same(g, g.ScaleTransform(0.5, 2.0, MatrixOrder.Append))
      g.DrawImage(bmp2, 0, 0)
    }
    
    # RotateTransform
    draw("RotateTransform1") { |g|
      assert_same(g, g.RotateTransform(45))
      g.DrawImage(bmp2, 0, 0)
    }
    draw("RotateTransform2") { |g|
      assert_same(g, g.RotateTransform(45.0, MatrixOrder.Append))
      g.DrawImage(bmp2, 0, 0)
    }
    
    # MultiplyTransform
    matrix = Matrix.new
    matrix.Rotate(45)
    draw("MultiplyTransform1") { |g|
      assert_same(g, g.MultiplyTransform(matrix))
      g.DrawImage(bmp2, 0, 0)
    }
    draw("MultiplyTransform2") { |g|
      assert_same(g, g.MultiplyTransform(matrix, MatrixOrder.Append))
      g.DrawImage(bmp2, 0, 0)
      assert_equal(matrix, g.Transform)
    }
    
    # ResetTransform
    draw("ResetTransform") { |g|
      g.TranslateTransform(100, 100)
      assert_same(g, g.ResetTransform)
      g.DrawImage(bmp2, 0, 0)
    }
    
    # TransformPoints
    points = [Point.new(10, 10), Point.new(20, 20), Point.new(30, 30)]
    points2 = [Point.new(110, 110), Point.new(120, 120), Point.new(130, 130)]
    draw { |g|
      g.TranslateTransform(100, 100)
      assert_equal(points2, g.TransformPoints(CoordinateSpace.Page, :World, points))
    }
    pointsF = [PointF.new(10.0, 10.0), PointF.new(20.0, 20.0), PointF.new(30.0, 30.0)]
    pointsF2 = [PointF.new(110.0, 110.0), PointF.new(120.0, 120.0), PointF.new(130.0, 130.0)]
    draw { |g|
      g.TranslateTransform(100, 100)
      assert_equal(pointsF2, g.TransformPoints(:Page, :World, pointsF))
    }
    
  end
  
  def test_other_methods
    #IsVisible
    draw {|g|
      assert_equal(true, g.IsVisible(100, 100))
      assert_equal(true, g.IsVisible(100.0, 100.0))
      assert_equal(false, g.IsVisible(-100, -100))
      assert_equal(false, g.IsVisible(-100.0, -100.0))
      
      assert_equal(true, g.IsVisible(100, 100, 100, 100))
      assert_equal(true, g.IsVisible(100, 100, 1000, 1000))
      assert_equal(true, g.IsVisible(100.0, 100.0, 100.0, 100.0))
      assert_equal(false, g.IsVisible(-100, -100, 50, 50))
      assert_equal(false, g.IsVisible(-100.0, -100.0, -50.0, -50.0))
      
      g.TranslateTransform(100, 100)
      assert_equal(true, g.IsVisible(-50, -50))
      assert_equal(false, g.IsVisible(350, 350))
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
