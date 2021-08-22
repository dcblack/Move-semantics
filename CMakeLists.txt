# CMakeLists.txt -- Top-level CMake script

cmake_minimum_required ( VERSION 3.11 )
project ( useful CXX )

set( CMAKE_BUILD_TYPE debug )
include( cmake/BuildTypes.cmake )
include( cmake/strict.cmake )

set( TARGET_NAME useful )

set ( CMAKE_CXX_STANDARD 17 CACHE STRING
     "C++ standard to build all targets." )

set ( CMAKE_CXX_STANDARD_REQUIRED 17 CACHE BOOL
     "The with CMAKE_CXX_STANDARD selected C++ standard is a requirement." )

add_executable( noisy1 usage.cpp )
target_compile_definitions( noisy1 PUBLIC USE_IOSTREAM NOISY1_SELFTEST )

add_executable( noisy2 usage.cpp )
target_compile_definitions( noisy2 PUBLIC USE_IOSTREAM NOISY2_SELFTEST )

add_executable( uniqueid usage.cpp )
target_compile_definitions( uniqueid PUBLIC USE_IOSTREAM UNIQUEID_SELFTEST )

# vim:nospell
