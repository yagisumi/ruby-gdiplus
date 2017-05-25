$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'gdiplus'

#GC.stress = true

#require 'minitest/autorun'
require 'test/unit'
require 'stringio'

module Test::Unit::Assertions
  def _verbose(v)
    _v = $VERBOSE
    $VERBOSE = v
    begin
      yield
    ensure
      $VERBOSE = _v
    end
  end
  
  def _capture_output(verbose=nil, &block)
    _v = $VERBOSE
    unless verbose.nil?
      $VERBOSE = verbose
    end
    
    sio_out = StringIO.new
    sio_err = StringIO.new
    _stdout = $stdout
    _stderr = $stderr
    $stdout = sio_out
    $stderr = sio_err
    begin
      block.call
      [sio_out.string, sio_err.string]
    ensure
      $stdout = _stdout
      $stderr = _stderr
      $VERBOSE = _v
    end
  end
  
  def _assert_output(stdout=nil, stderr=nil, verbose=nil, &block)
    out, err = _capture_output(verbose, &block)
    
    x = true
    case stdout
    when Regexp
      x = assert_match(stdout, out, "stdout")
    when String
      x = assert_equal(stdout, out, "stdout")
    when nil
      x = assert_equal("", out, "stdout")
    else
      assert(false, "The first parameter should be a Regexp, String or nil.")
    end
    
    y = true
    case stderr
    when Regexp
      y = assert_match(stderr, err, "stderr")
    when String
      y = assert_equal(stderr, err, "stderr")
    when nil
      y = assert_equal("", err, "stdout")
    else
      assert(false, "The second parameter should be a Regexp or String.")
    end
    
    x && y
  end
  
  def _assert_silent(verbose=nil)
    _assert_output("", "", verbose) { yield }
  end
  
  def _assert_stderr(stderr, verbose=nil, &block)
    err = _capture_output(verbose, &block).last
    
    y = true
    case stderr
    when Regexp
      y = assert_match(stderr, err, "stderr")
    when String
      y = assert_equal(stderr, err, "stderr")
    when nil
      y = assert_equal("", err, "stdout")
    else
      assert(false, "The first parameter should be a Regexp, String or nil.")
    end
    
    y
  end
  
  def _assert_stderr_silent(verbose=nil)
    _assert_stderr("", verbose) { yield }
  end
  
  def _assert_stdout(stdout, verbose=nil, &block)
    out = _capture_output(verbose, &block).first
    
    x = true
    case stdout
    when Regexp
      x = assert_match(stdout, out, "stdout")
    when String
      x = assert_equal(stdout, out, "stdout")
    when nil
      x = assert_equal("", out, "stdout")
    else
      assert(false, "The first parameter should be a Regexp, String or nil.")
    end
    
    x
  end
  
  def _assert_stdout_silent(verbose=nil)
    _assert_stdout("", verbose) { yield }
  end
  
end
