# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  )
# The set of files for implicit dependencies of each language:

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS
  "FMILIB_BUILDING_LIBRARY"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  )

# The include file search paths:
set(CMAKE_C_TARGET_INCLUDE_PATH
  "."
  "../src/Util/include"
  "../ThirdParty/FMI/default"
  "../src/XML/include"
  "../ThirdParty/FMI"
  "ExpatEx/install/include"
  "../src/CAPI/include"
  "../src/CAPI/src"
  "../src/ZIP/include"
  "../ThirdParty/Minizip/minizip"
  "../ThirdParty/Zlib/zlib-1.2.6"
  "zlib"
  "../src/Import"
  "../src/Import/include"
  "../Test"
  "../Test/FMI1"
  "../Test/FMI2"
  )
set(CMAKE_CXX_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
set(CMAKE_Fortran_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
set(CMAKE_ASM_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
