require "mkmf"

have_library('gdiplus')
have_library('Rpcrt4')

if $mingw
  $libs = "-static-libgcc -static-libstdc++ " + $libs
end

#$CPPFLAGS += " -DNDEBUG "

create_makefile("gdiplus/gdiplus")
