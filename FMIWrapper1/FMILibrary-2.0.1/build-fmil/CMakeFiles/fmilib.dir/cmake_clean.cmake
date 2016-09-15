file(REMOVE_RECURSE
  "../install/doc/html"
  "../install/doc/latex"
  "fmilib_dummy.c"
  "libfmiimport.a.objlist"
  "libjmutils.a.objlist"
  "libfmixml.a.objlist"
  "libfmizip.a.objlist"
  "libfmicapi.a.objlist"
  "ExpatEx/libexpat.a.objlist"
  "Config.cmake/Minizip/libminizip.a.objlist"
  "zlib/libz.a.objlist"
  "ThirdParty/c99_snprintf/libc99snprintf.a.objlist"
  "CMakeFiles/fmilib.dir/fmilib_dummy.c.o"
  "libfmilib.pdb"
  "libfmilib.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/fmilib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
