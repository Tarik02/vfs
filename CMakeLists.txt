cmake_minimum_required(VERSION 3.5)
project(vfs VERSION 1.0.0 LANGUAGES C)

##############################################
# Declare dependencies
find_package(HashMap 1.0 REQUIRED)

##############################################
# Create target and set properties

add_library(vfs
  src/sources/memory.c
  src/sources/prefixed.c
  src/file.c
  src/source.c
  src/vfs.c
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(VFS::vfs ALIAS vfs)

#Set target properties
target_include_directories(vfs
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(vfs PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

target_link_libraries(vfs
  PRIVATE
    HashMap::HashMap
)

##############################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/VFS)

install(TARGETS vfs
  EXPORT vfs-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#This is required so that the exported target has the name VFS and not vfs
set_target_properties(vfs PROPERTIES EXPORT_NAME VFS)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT vfs-targets
  FILE
    VFSTargets.cmake
  NAMESPACE
    VFS::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/VFSConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/VFSConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/VFSConfig.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/VFSConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/VFSConfigVersion.cmake
  DESTINATION ${INSTALL_CONFIGDIR}
)

##############################################
## Exporting from the build tree
export(EXPORT vfs-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/VFSTargets.cmake NAMESPACE VFS::)

#Register package in user's package registry
export(PACKAGE VFS)
