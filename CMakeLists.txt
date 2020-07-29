cmake_minimum_required(VERSION 3.16)

project(parser CXX)

# Include all CMake scripts
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/Cache.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Sanitizers.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/Conan.cmake)

#
# Interface libraries
#
add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)

# Set options
target_compile_features(project_options INTERFACE cxx_std_20)
enable_sanitizers(project_options)
target_compile_options(project_options INTERFACE "-fconcepts")

# Set relevant warnings
set_project_warnings(project_warnings)
target_compile_options(project_warnings INTERFACE "-Wno-psabi")

#
# Configure and run conan
#
set(CONAN_EXTRA_REQUIRES)
set(CONAN_EXTRA_OPTIONS)

run_conan()

#
# Compile targets
#
add_subdirectory(src)

if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
