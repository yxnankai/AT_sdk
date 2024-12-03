#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "AT::CxBaseLib" for configuration "Release"
set_property(TARGET AT::CxBaseLib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(AT::CxBaseLib PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/CxBaseLib_3_0.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/CxBaseLib_3_0.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS AT::CxBaseLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_AT::CxBaseLib "${_IMPORT_PREFIX}/lib/CxBaseLib_3_0.lib" "${_IMPORT_PREFIX}/bin/CxBaseLib_3_0.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
