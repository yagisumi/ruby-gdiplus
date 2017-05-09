# coding: utf-8
require 'test_helper'

class Guid2 < Gdiplus::Guid
end

class GdiplusCodecTest < Test::Unit::TestCase
  include Gdiplus
  
  def test_guid
    assert_raise(ArgumentError) { Guid.new }
    g1 = nil
    assert_nothing_raised() { g1 = Guid.new("b96b3ca9-0728-11d3-9d7b-0000f81ef32e") }
    assert_equal("<Gdiplus::Guid {b96b3ca9-0728-11d3-9d7b-0000f81ef32e}>", g1.inspect)
    assert_equal("b96b3ca9-0728-11d3-9d7b-0000f81ef32e", g1.to_s)
    assert_equal("{0xb96b3ca9, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}}", g1.c_struct)
    assert_raise(GdiplusError) { Guid.new("{b96b3ca9-0728-11d3-9d7b-0000f81ef32e}") } # contains braces => error
    assert_raise(TypeError) { Guid.new(1) } # requires a string
    
    g1b = Guid.new("b96b3ca9-0728-11d3-9d7b-0000f81ef32e")
    h = {}
    h[g1] = 1
    h[g1b] = 2 # override value
    assert_equal(true, g1 == g1b)
    assert_equal(true, g1b == g1)
    assert_equal(true, g1.eql?(g1b)) # same GUID value
    assert_equal(2, h[g1])
    
    g2 = Guid2.new("b96b3ca9-0728-11d3-9d7b-0000f81ef32e") # inheritance
    h[g2] = 3 # override value
    assert_equal(true, g1 == g2) # same GUID value
    assert_equal(true, g1.eql?(g2)) # same GUID value
    assert_equal(true, g2.eql?(g1))
    assert_match(/Guid2/, g2.inspect)
    assert_equal(3, h[g1])
    
    g3 = Guid.new("b96b3ca9-0728-11d3-9d7b-0000f81ef320")
    h[g3] = 4
    assert_equal(false, h[g1] == h[g3]) # different GUID value
  end
  
  require 'pp'
  def test_imagecodecinfo
    encoders = ImageCodecInfo.image_encoders
    decoders = ImageCodecInfo.image_decoders # 
    # remains of memory check
#    encoders = ImageCodecInfo.image_encoders
#    decoders = ImageCodecInfo.image_decoders
#    encoders = ImageCodecInfo.image_encoders
#    decoders = ImageCodecInfo.image_decoders
#    encoders = ImageCodecInfo.image_encoders
#    decoders = ImageCodecInfo.image_decoders
#    encoders = ImageCodecInfo.image_encoders
#    decoders = ImageCodecInfo.image_decoders
    bmp_encoder = encoders.find {|icinfo|
      icinfo.FormatDescription == "BMP"
    }
    #p bmp_encoder
    assert_not_nil(bmp_encoder)
    assert_equal("image/bmp", bmp_encoder.mime_type)
    assert_equal("image/bmp", bmp_encoder.MimeType)
    assert_equal(true, bmp_encoder.MimeType.frozen?)
    assert_equal("BMP", bmp_encoder.format_description)
    assert_equal("BMP", bmp_encoder.FormatDescription)
    assert_equal(true, bmp_encoder.FormatDescription.frozen?)
    assert_equal("*.BMP;*.DIB;*.RLE", bmp_encoder.filename_extension)
    assert_equal("*.BMP;*.DIB;*.RLE", bmp_encoder.FilenameExtension)
    assert_equal(true, bmp_encoder.FilenameExtension.frozen?)
    assert_instance_of(Array, bmp_encoder.SignaturePatterns)
    assert_instance_of(Array, bmp_encoder.signature_patterns)
    assert_equal(true, bmp_encoder.signature_patterns.frozen?)
    assert_equal("BM", bmp_encoder.SignaturePatterns.first)
    assert_equal(true, bmp_encoder.signature_patterns.first.frozen?)
    
    bmp_decoder = decoders.find {|icinfo|
      icinfo.FormatDescription == "BMP"
    }
    assert_not_nil(bmp_decoder)
    assert_equal("image/bmp", bmp_decoder.mime_type)
    assert_equal("image/bmp", bmp_decoder.MimeType)
    assert_equal(true, bmp_decoder.MimeType.frozen?)
    assert_equal("BMP", bmp_decoder.format_description)
    assert_equal("BMP", bmp_decoder.FormatDescription)
    assert_equal(true, bmp_decoder.FormatDescription.frozen?)
    assert_equal("*.BMP;*.DIB;*.RLE", bmp_decoder.filename_extension)
    assert_equal("*.BMP;*.DIB;*.RLE", bmp_decoder.FilenameExtension)
    assert_equal(true, bmp_decoder.FilenameExtension.frozen?)
    assert_instance_of(Array, bmp_decoder.SignaturePatterns)
    assert_instance_of(Array, bmp_decoder.signature_patterns)
    assert_equal(true, bmp_decoder.signature_patterns.frozen?)
    assert_equal("BM", bmp_decoder.SignaturePatterns.first)
    assert_equal(true, bmp_decoder.signature_patterns.first.frozen?)
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
