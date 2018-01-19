cmake_minimum_required( VERSION 2.8 )
project( OCLBootstrap )

set( CMAKE_CXX_FLAGS "-std=c++1y ${CMAKE_CXX_FLAGS}" )

add_definitions( -DCL_HPP_MINIMUM_OPENCL_VERSION=110 )
add_definitions( -DCL_HPP_TARGET_OPENCL_VERSION=110 )

set( OCLBootstrap_SOURCES OCLBootstrap.cpp )
set( OCLBootstrap_HEADERS OCLBootstrap.h )

add_executable( OCLBootstrap
  ${OCLBootstrap_SOURCES}
  ${OCLBootstrap_HEADERS}
)

target_link_libraries( OCLBootstrap ${OpenCL_LIBRARIES} )
