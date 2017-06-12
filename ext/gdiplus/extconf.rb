
require "mkmf"

versions = RUBY_VERSION.split(/\./).map {|v| v.to_i }
RUBY_VERSION_NUM = versions[0] * 10000 + versions[1] * 100 + versions[2]

have_library('gdiplus')
have_library('Rpcrt4')

gdiplus_debug = true ### check before release ###

if gdiplus_debug
  $defs << "-DGDIPLUS_DEBUG=1"
end

if $mingw
  #$CXXFLAGS += " -std=c++11 "
  if gdiplus_debug # DEBUG
    
    if RUBY_VERSION_NUM >= 20400 # for 2.4
      # http://stackoverflow.com/questions/13768515/how-to-do-static-linking-of-libwinpthread-1-dll-in-mingw
      $libs += " -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive "
#    elsif RUBY_VERSION_NUM >= 20300 # for 2.3
#      $libs += " -static-libgcc -static-libstdc++ "
    elsif RUBY_VERSION_NUM >= 20000 # for 2.0-2.2
      $libs += " -static-libgcc -static-libstdc++ "
    elsif RUBY_VERSION_NUM < 10900 # for 1.8
      #have_library('stdc++')
      $libs += " -Wl,-Bstatic -lstdc++ -Wl,-Bdynamic "
    end
    
  else # RELEASE
    
    if RUBY_VERSION_NUM >= 20400 # for 2.4
      $libs += " -static-libgcc "
    elsif RUBY_VERSION_NUM >= 20300 # for 2.3
      #$libs += " -static-libstdc++ "
    elsif RUBY_VERSION_NUM >= 20000 # for 2.0-2.2
      $libs += " -static-libgcc -static-libstdc++ "
    end
    
  end # gdiplus_debug

  if $warnflags
    $warnflags = $warnflags.sub(/-Wdeclaration-after-statement/, '') # is valid for C/ObjC but not for C++
    $warnflags = $warnflags.sub(/-Wimplicit-function-declaration/, '') # is valid for C/ObjC but not for C++
  end
  
end # $mingw


create_makefile("gdiplus/gdiplus")
