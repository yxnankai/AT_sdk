#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "AT::CxCamLib" for configuration "Release"
set_property(TARGET AT::CxCamLib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(AT::CxCamLib PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/CxCamLib_3_1.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "AT::CxBaseLib;GenApi;GCBase"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/CxCamLib_3_1.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS AT::CxCamLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_AT::CxCamLib "${_IMPORT_PREFIX}/lib/CxCamLib_3_1.lib" "${_IMPORT_PREFIX}/bin/CxCamLib_3_1.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
