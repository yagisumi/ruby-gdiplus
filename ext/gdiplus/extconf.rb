require "mkmf"

have_library('gdiplus')
have_library('Rpcrt4')

if $mingw
  $libs = "-static-libgcc -static-libstdc++ " + $libs
end

#$CPPFLAGS += " -DNDEBUG "

if $warnflags
  $warnflags = $warnflags.sub(/-Wdeclaration-after-statement/, '') # is valid for C/ObjC but not for C++
  $warnflags = $warnflags.sub(/-Wimplicit-function-declaration/, '') # is valid for C/ObjC but not for C++
end

create_makefile("gdiplus/gdiplus")
