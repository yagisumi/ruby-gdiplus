$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'gdiplus'

#require 'minitest/autorun'
require 'test/unit'
require 'stringio'

module Test::Unit::Assertions
  VERBOSE_ = []
  def verbose(v)
    _v = $VERBOSE
    $VERBOSE = v
    begin
      yield
    ensure
      $VERBOSE = _v
    end
  end
  
  def capture_output
    sio_out = StringIO.new
    sio_err = StringIO.new
    _stdout = $stdout
    _stderr = $stderr
    $stdout = sio_out
    $stderr = sio_err
    begin
      yield
      [sio_out.string, sio_err.string]
    ensure
      $stdout = _stdout
      $stderr = _stderr
    end
  end
  
  def assert_output(stdout=nil, stderr=nil, &block)
    out, err = capture_output(&block)
    
    x = true
    case stdout
    when Regexp
      x = assert_match(stdout, out, "stdout")
    when String
      x = assert_equal(stdout, out, "stdout")
    end
    
    y = true
    case stderr
    when Regexp
      y = assert_match(stderr, err, "stderr")
    when String
      y = assert_equal(stderr, err, "stderr")
    end
    
    x && y
  end
  
  def assert_silent
    assert_output("", "") { yield }
  end
  
  def assert_stderr(stderr, &block)
    err = capture_output(&block).last
    
    y = true
    case stderr
    when Regexp
      y = assert_match(stderr, err, "stderr")
    when String
      y = assert_equal(stderr, err, "stderr")
    end
    
    y
  end
  
  def assert_stderr_silent
    assert_stderr("") { yield }
  end
  
  def assert_stdout(stdout, &block)
    out = capture_output(&block).first
    
    x = true
    case stdout
    when Regexp
      x = assert_match(stdout, out, "stdout")
    when String
      x = assert_equal(stdout, out, "stdout")
    end
    
    x
  end
  
  def assert_stdout_silent
    assert_stdout("") { yield }
  end
  
end