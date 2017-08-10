# coding: utf-8
require 'test_helper'

class GdiplusGraphicsPathTest < Test::Unit::TestCase
  include Gdiplus
  include Math

  POINTS1 = []
  POINTS1 << PointF.new(100.0, 100.0)
  POINTS1 << PointF.new(200.0, 100.0)
  POINTS1 << PointF.new(200.0, 200.0)
  POINTS1 << PointF.new(100.0, 200.0)

  TYPES1 = []
  TYPES1 << PathPointType.Start
  TYPES1 << PathPointType.Line
  TYPES1 << PathPointType.Line
  TYPES1 << PathPointType.Line.CloseSubpath.PathMarker

  def test_constructor
    assert_instance_of(GraphicsPath, GraphicsPath.new)
    assert_instance_of(GraphicsPath, GraphicsPath.new(FillMode.Alternate))
    assert_instance_of(GraphicsPath, GraphicsPath.new(:Winding))

    assert_instance_of(GraphicsPath, GraphicsPath.new(POINTS1, TYPES1))
    assert_instance_of(GraphicsPath, GraphicsPath.new(POINTS1, TYPES1, FillMode.Winding))
  end

  def test_properties
    # FillMode
    assert_equal(FillMode.Alternate, GraphicsPath.new.FillMode)
    assert_equal(FillMode.Winding, GraphicsPath.new(FillMode.Winding).fill_mode)

    # PathData
    pd = GraphicsPath.new.PathData
    assert_instance_of(PathData, pd)
    assert_equal([], pd.Points)
    assert_equal([], pd.Types)

    pd = GraphicsPath.new(POINTS1, TYPES1).path_data
    assert_instance_of(PathData, pd)
    assert_equal(POINTS1, pd.points)
    assert_equal(TYPES1, pd.types)

    # PathPoints
    assert_equal([], GraphicsPath.new.PathPoints)
    assert_equal(POINTS1, GraphicsPath.new(POINTS1, TYPES1).path_points)

    # PathTypes
    assert_equal([], GraphicsPath.new.PathTypes)
    assert_equal(TYPES1, GraphicsPath.new(POINTS1, TYPES1).path_types)

    # PointCount
    assert_equal(0, GraphicsPath.new.PointCount)
    assert_equal(POINTS1.size, GraphicsPath.new(POINTS1, TYPES1).point_count)
  end

  def test_add_methods
    # AddArc
    gp = GraphicsPath.new
    assert_same(gp, gp.AddArc(Rectangle.new(0, 0, 100, 100), -30, 30))
    assert_same(gp, gp.add_arc(0.0, 0.0, 100.0, 100.0, -30.0, 30.0))

    # AddBezier
    gp = GraphicsPath.new
    assert_same(gp, gp.AddBezier(*POINTS1))
    assert_same(gp, gp.add_bezier(0, 0, 100, 0, 100, 100, 0, 100))

    # AddBeziers
    gp = GraphicsPath.new
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
    points.each {|po| po.x = 50 + 100 * po.x; po.y = 50 + 100 * po.y }

    assert_same(gp, gp.AddBeziers(points))

    # invalid number of points
    points << Point.new(300, 300)
    assert_raise(ArgumentError) { gp.AddBeziers(points) }

    # AddClosedCurve
    gp = GraphicsPath.new
    count = 17
    d_theta = 2 * Math::PI / count
    r = 180
    points = []
    count.times {|i|
      points << PointF.new(200 + r * cos(d_theta * i), 200 + r * sin(d_theta * i))
    }
    gp2 = gp.AddClosedCurve(points)
    assert_same(gp, gp2)
    assert_equal(true, gp2.PathTypes[-1].HasFlag(PathPointType.CloseSubpath))

    # AddCurve
    gp = GraphicsPath.new
    gp2 = gp.AddCurve(points)
    assert_same(gp, gp2)
    assert_equal(false, gp2.PathTypes[-1].HasFlag(PathPointType.CloseSubpath))

    # AddEllipse
    gp = GraphicsPath.new
    assert_same(gp, gp.AddEllipse(Rectangle.new(0, 0, 100, 100)))
    assert_same(gp, gp.AddEllipse(0.0, 0.0, 100.0, 100.0))

    # AddLine
    gp = GraphicsPath.new
    assert_same(gp, gp.AddLine(Point.new(0, 0), Point.new(100, 100)))
    assert_same(gp, gp.add_line(0.0, 0.0, 100.0, 100.0))

    # AddLines
    gp = GraphicsPath.new
    assert_same(gp, gp.AddLines(POINTS1))
    assert_same(gp, gp.add_lines(POINTS1))

    # AddPath
    gp2 = GraphicsPath.new
    gp2.add_lines(POINTS1)
    assert_equal(gp2, gp2.AddPath(gp))
    assert_equal(gp2, gp2.add_path(gp, true))

    # AddPie
    gp = GraphicsPath.new
    assert_same(gp, gp.AddPie(Rectangle.new(0, 0, 100, 100), -30, 30))
    assert_same(gp, gp.add_pie(0.0, 0.0, 100.0, 100.0, -30.0, 30.0))

    # AddPolygon
    gp = GraphicsPath.new
    points = []
    30.times {||
      points << Point.new(rand(400), rand(400))
    }
    assert_equal(gp, gp.AddPolygon(points))
    assert_equal(gp, gp.add_polygon(points))

    # AddRectangle
    gp = GraphicsPath.new
    assert_same(gp, gp.AddRectangle(Rectangle.new(0, 0, 100, 100)))
    assert_same(gp, gp.add_rectangle(0.0, 0.0, 100.0, 100.0))

    # AddRectangles
    gp = GraphicsPath.new
    rects = []
    10.times {
      rects << Rectangle.new(rand(360), rand(360), rand(200), rand(200))
    }
    assert_same(gp, gp.AddRectangles(rects))
    assert_same(gp, gp.add_rectangles(rects))

    # AddString
    gp = GraphicsPath.new
    unless InstalledFontCollection.broken?
      gp.AddString("test", FontFamily.GenericSansSerif, 0, 30, Point.new(30, 30), StringFormat.new)
    end
  end

  def test_other_methods
    # Reset
    gp = GraphicsPath.new
    points = [PointF.new(0.0, 0.0), PointF.new(100.0, 0.0), PointF.new(100.0, 100.0)]
    gp.AddLines(points)
    assert_equal(points, gp.PathPoints)
    gp.Reset
    assert_equal([], gp.PathPoints)

    # Reverse
    gp = GraphicsPath.new
    points = [PointF.new(0.0, 0.0), PointF.new(100.0, 0.0), PointF.new(100.0, 100.0)]
    gp.AddLines(points)
    assert_equal(points, gp.PathPoints)
    gp.Reverse
    assert_equal(points.reverse, gp.PathPoints)

    # CloseFigure
    gp = GraphicsPath.new
    points = [PointF.new(0.0, 0.0), PointF.new(100.0, 0.0), PointF.new(100.0, 100.0)]
    gp.AddLines(points)
    assert_equal(false, gp.PathTypes[-1].close_subpath?)
    gp.CloseFigure
    assert_equal(true, gp.PathTypes[-1].close_subpath?)

    # CloseAllFigures, StartFigure
    gp = GraphicsPath.new
    points = [PointF.new(0.0, 0.0), PointF.new(100.0, 0.0), PointF.new(100.0, 100.0)]
    points2 = [PointF.new(0.0, 0.0), PointF.new(-100.0, 0.0), PointF.new(-100.0, -100.0)]
    gp.AddLines(points)
    gp.StartFigure
    gp.AddLines(points2)
    assert_equal(false, gp.PathTypes[-1].close_subpath?)
    assert_equal(false, gp.PathTypes[2].close_subpath?)
    gp.CloseAllFigures
    assert_equal(true, gp.PathTypes[-1].close_subpath?)
    assert_equal(true, gp.PathTypes[2].close_subpath?)

    # SetMarker
    gp = GraphicsPath.new
    points = [PointF.new(0.0, 0.0), PointF.new(100.0, 0.0), PointF.new(100.0, 100.0)]
    gp.AddLines(points)
    assert_equal(false, gp.PathTypes[-1].path_marker?)
    gp.SetMarker
    assert_equal(true, gp.PathTypes[-1].path_marker?)

    # ClearMarkers
    gp = GraphicsPath.new
    points = [PointF.new(0.0, 0.0), PointF.new(100.0, 0.0), PointF.new(100.0, 100.0)]
    points2 = [PointF.new(0.0, 0.0), PointF.new(-100.0, 0.0), PointF.new(-100.0, -100.0)]
    gp.AddLines(points)
    gp.SetMarker
    gp.StartFigure
    gp.AddLines(points2)
    gp.SetMarker
    assert_equal(true, gp.PathTypes[-1].path_marker?)
    assert_equal(true, gp.PathTypes[2].path_marker?)
    gp.ClearMarkers
    assert_equal(false, gp.PathTypes[-1].path_marker?)
    assert_equal(false, gp.PathTypes[2].path_marker?)

    # GetBounds, GetLastPoint
    gp = GraphicsPath.new
    points = [PointF.new(0.0, 0.0), PointF.new(100.0, 0.0), PointF.new(100.0, 100.0)]
    gp.AddLines(points)
    assert_equal(points[-1], gp.GetLastPoint)
    assert_equal(RectangleF.new(0.0, 0.0, 100.0, 100.0), gp.GetBounds)

    # IsOutlineVisible, IsVisible
    gp = GraphicsPath.new
    pen = Pen.new(:Green, 10.0)
    gp.AddRectangle(RectangleF.new(0.0, 0.0, 100.0, 100.0))
    assert_equal(true, gp.IsOutlineVisible(PointF.new(50.0, 0.0), pen))
    assert_equal(true, gp.IsOutlineVisible(50.0, 0.0, pen))
    assert_equal(false, gp.IsOutlineVisible(Point.new(50, 50), pen))
    assert_equal(false, gp.IsOutlineVisible(50, 50, pen))
    assert_equal(false, gp.IsVisible(PointF.new(-50.0, 0.0)))
    assert_equal(false, gp.IsVisible(-50.0, 0.0))
    assert_equal(true, gp.IsVisible(Point.new(50, 50)))
    assert_equal(true, gp.IsVisible(50.0, 0.0))

    # Widen
    gp = GraphicsPath.new
    pen = Pen.new(:Green, 10.0)
    gp.AddRectangle(RectangleF.new(0.0, 0.0, 100.0, 100.0))
    assert_equal(false, gp.IsVisible(50.0, -5.0))
    gp.Widen(pen)
    assert_equal(true, gp.IsVisible(50.0, -5.0))

    # Transform
    gp = GraphicsPath.new
    gp.AddRectangle(RectangleF.new(-100.0, -100.0, 200.0, 200.0))
    matrix = Matrix.new.Rotate(45.0)
    gp.Transform(matrix)
    assert_equal(0, gp.PathPoints.first.X.to_i)

    # Warp
    gp = GraphicsPath.new
    gp.AddEllipse(100, 100, 200, 200)
    points = [PointF.new(0.0, 100.0), PointF.new(380.0, 0.0), PointF.new(100.0, 300.0)]
    rect = RectangleF.new(100.0, 100.0, 200.0, 200.0)
    assert_same(gp, gp.Warp(points, rect))
    points << PointF.new(380.0, 380.0)
    assert_same(gp, gp.Warp(points, rect))
    assert_same(gp, gp.Warp(points, rect))
    matrix = Matrix.new.Rotate(45.0)
    assert_same(gp, gp.Warp(points, rect, matrix))
    assert_same(gp, gp.Warp(points, rect, matrix, WarpMode.Perspective))
    assert_same(gp, gp.Warp(points, rect, matrix, WarpMode.Perspective, 3.0))
    assert_same(gp, gp.Warp(points, rect, nil, WarpMode.Perspective))
    assert_same(gp, gp.Warp(points, rect, nil, nil, 3.0))

    # Flatten
    gp = GraphicsPath.new
    gp.AddEllipse(100, 100, 200, 200)
    assert_same(gp, gp.Flatten())

    gp = GraphicsPath.new
    gp.AddEllipse(100, 100, 200, 200)
    matrix = Matrix.new.Rotate(45.0)
    assert_same(gp, gp.Flatten(matrix))

    gp = GraphicsPath.new
    gp.AddEllipse(100, 100, 200, 200)
    matrix = Matrix.new.Rotate(45.0)
    assert_same(gp, gp.Flatten(matrix, 3.0))

    gp = GraphicsPath.new
    gp.AddEllipse(100, 100, 200, 200)
    matrix = Matrix.new.Rotate(45.0)
    assert_same(gp, gp.Flatten(nil, 3.0))
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
