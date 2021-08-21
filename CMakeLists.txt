# CMakeLists.txt -- Top-level CMake script

cmake_minimum_required ( VERSION 3.11 )
project ( useful CXX )

set( CMAKE_BUILD_TYPE release )
include( $ENV{APPS}/cmake/BuildTypes.cmake )
include( $ENV{APPS}/cmake/strict.cmake )
#find_package( yaml-cpp CONFIG REQUIRED )
#find_package( Boost 1.65 REQUIRED filesystem )

set( TARGET_NAME useful )

set ( CMAKE_CXX_STANDARD 17 CACHE STRING
     "C++ standard to build all targets." )

set ( CMAKE_CXX_STANDARD_REQUIRED 17 CACHE BOOL
     "The with CMAKE_CXX_STANDARD selected C++ standard is a requirement." )

add_executable( noisy1 )
target_compile_definitions( noisy1 PUBLIC NOISY1_SELFTEST )
target_sources(             noisy1 PUBLIC usage.cpp )

add_executable( noisy2 )
target_compile_definitions( noisy2 PUBLIC NOISY2_SELFTEST )
target_sources(             noisy2 PUBLIC usage.cpp )

add_executable( uniqueid )
target_compile_definitions( uniqueid PUBLIC UNIQUEID_SELFTEST )
target_sources(             uniqueid PUBLIC usage.cpp )
