file(REMOVE_RECURSE
  "../install/doc/html"
  "../install/doc/latex"
)

# Per-language clean rules from dependency scanning.
foreach(lang)
  include(CMakeFiles/doc.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
