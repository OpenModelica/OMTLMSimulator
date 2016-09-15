file(REMOVE_RECURSE
  "../install/doc/html"
  "../install/doc/latex"
  "CMakeFiles/doxygen"
)

# Per-language clean rules from dependency scanning.
foreach(lang)
  include(CMakeFiles/doxygen.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
