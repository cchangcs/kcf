file(REMOVE_RECURSE
  "libkcf.pdb"
  "libkcf.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/kcf.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
