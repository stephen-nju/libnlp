cmake_minimum_required(VERSION 3.20)

# Set the project name to your project name, my project isn't very descriptive
project(nlp VERSION 0.0.1)
# set(CMAKE_C_STANDARD 11) set(CMAKE_C_STANDARD_REQUIRED ON)

# message(STATUS "<<< ${CMAKE_C_STANDARD}")
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE
         "Enable -ftime-trace to generate time tracing .json files on clang"
         OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(project_options INTERFACE -ftime-trace)
  endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# Add linker configuration
include(cmake/Linker.cmake)
configure_linker(project_options)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)
# option(ENABLE_DOXYGEN "Build document" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)
option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if(ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which
  # is a good idea if any #define's change
  #
  # consider breaking this out per project as necessary
  target_precompile_headers(project_options INTERFACE <vector> <string> <map>)
endif()

add_subdirectory(3rdparty)
include_directories(3rdparty/utf8proc)

if(WIN32 AND MSVC)
  if(CMAKE_CL_64)
    set(PLATFORM x64)
  else()
    set(PLATFORM x86)
  endif()
  add_definitions(-DHAVE_STRUCT_TIMESPEC)
  link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/winpthread/lib/${PLATFORM})
  message(STATUS "<<<< windows link directories ${PLATFORM}")
endif()

# add src include path

set(INCLUDE_DIRECTORIES
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    # ${PROJECT_BINARY_DIR}
     PRIVATE 
     ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/utf8proc)
add_subdirectory(src)

option(ENABLE_UNITY "Enable Unity builds of projects" OFF)
if(ENABLE_UNITY)
  # Add for any project you want to apply unity builds for
  set_target_properties(intro PROPERTIES UNITY_BUILD ON)
endif()

# add_executable(demo main.c) target_link_libraries(demo ${PROJECT_NAME})
# target_include_directories(demo ${INCLUDE_DIRECTORIES})

if(ENABLE_TESTING)
  enable_testing()
  message(
    STATUS
      "Building Tests. Be sure to check out test/constexpr_tests for constexpr testing"
  )
  add_subdirectory(test)
endif()
