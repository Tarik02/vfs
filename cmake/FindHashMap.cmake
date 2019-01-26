#FindHashMap.cmake
# 
# Finds the hashmap library
#
# This will define the following variables
#
#    HashMap_FOUND
#    HashMap_INCLUDE_DIRS
#
# and the following imported targets
#
#     HashMap::HashMap
#

find_package(PkgConfig)
pkg_check_modules(PC_HashMap QUIET HashMap)

find_path(HashMap_INCLUDE_DIR
  NAMES hashmap.h
  PATHS ${PC_HashMap_INCLUDE_DIRS}
)

set(HashMap_VERSION ${PC_HashMap_VERSION})

mark_as_advanced(HashMap_FOUND HashMap_INCLUDE_DIR HashMap_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HashMap
  REQUIRED_VARS HashMap_INCLUDE_DIR
  VERSION_VAR HashMap_VERSION
)

if(HashMap_FOUND)
  set(HashMap_INCLUDE_DIRS ${HashMap_INCLUDE_DIR})
endif()

if(HashMap_FOUND AND NOT TARGET HashMap::HashMap)
  add_library(HashMap::HashMap INTERFACE IMPORTED)
  set_target_properties(HashMap::HashMap PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${HashMap_INCLUDE_DIR}"
  )
endif()
