# coding: utf-8
require 'test_helper'

class GdiplusImageTest < Test::Unit::TestCase
  include Gdiplus

  IMGFMT_SIGPAT_MAP = {
    ImageFormat.Bmp => ["BM"],
    ImageFormat.Jpeg => ["\xFF\xD8"],
    ImageFormat.Gif => ["GIF89a", "GIF87a"],
    ImageFormat.Tiff => ["II", "MM"],
    ImageFormat.Png => ["\x89PNG\r\n\x1A\n"],
  }

  EXT_SIGPAT_MAP = {
    ".bmp" => ["BM"],
    ".dib" => ["BM"],
    ".rle" => ["BM"],
    ".jpg" => ["\xFF\xD8"],
    ".jpeg" => ["\xFF\xD8"],
    ".jpe" => ["\xFF\xD8"],
    ".jfif" => ["\xFF\xD8"],
    ".gif" => ["GIF89a", "GIF87a"],
    ".tif" => ["II", "MM"],
    ".tiff" => ["II", "MM"],
    ".png" => ["\x89PNG\r\n\x1A\n"],
  }

  if "string".respond_to?(:encoding)
    EXT_SIGPAT_MAP.each {|ext, patterns|
      patterns.each {|pat|
        pat.force_encoding(Encoding::ASCII_8BIT)
      }
    }
    IMGFMT_SIGPAT_MAP.each {|ext, patterns|
      patterns.each {|pat|
        pat.force_encoding(Encoding::ASCII_8BIT)
      }
    }
  end

  def test_image_save1
    EXT_SIGPAT_MAP.each {|ext, patterns|
      savename = "test_image_save#{ext}"
      bmp = Bitmap.new(1, 1)
      bmp.save(savename)
      begin
        assert(FileTest.exist?(savename))
        head = File.open(savename, "rb") {|f| f.read(patterns.first.size) }
        assert(patterns.any?{|pat| head == pat })
      ensure
        File.delete(savename)
      end
    }
  end

  def test_image_save2_imgfmt
    IMGFMT_SIGPAT_MAP.each {|imgfmt, patterns|
      savename = "test_image_save.img"
      bmp = Bitmap.new(1, 1)
      bmp.save(savename, imgfmt)
      begin
        assert(FileTest.exist?(savename))
        head = File.open(savename, "rb") {|f| f.read(patterns.first.size) }
        assert(patterns.any?{|pat| head == pat })
      ensure
        File.delete(savename)
      end
    }
  end

  def test_image_save2_encprms
    bmp = Bitmap.new(1, 1)
    savename = "test_image_save.png"
    encprms = EncoderParameters.new
    bmp.save(savename, encprms)
    begin
      assert(FileTest.exist?(savename))
    ensure
      File.delete(savename)
    end

    bmp = Bitmap.new(1, 1)
    savename = "test_image_save.jpg"
    encprms = EncoderParameters.new
    encprms.add(EncoderParameterQuality.new(93))
    bmp.save(savename, encprms)
    begin
      assert(FileTest.exist?(savename))
    ensure
      File.delete(savename)
    end
  end

  def test_image_properties
    bmp = Bitmap.new(40, 30)
    assert_equal(40, bmp.Width)
    assert_equal(30, bmp.Height)
    assert(bmp.HorizontalResolution > 0)
    assert(bmp.VerticalResolution > 0)
    assert_equal(PixelFormat.Format32bppARGB, bmp.PixelFormat)
    assert_equal(ImageFormat.MemoryBmp, bmp.RawFormat)
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
