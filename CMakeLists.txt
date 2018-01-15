cmake_minimum_required( VERSION 2.8 )
project( OCLBootstrap )

find_package( OpenCL REQUIRED )

set( OCLBootstrap_SOURCES
  OCLBootstrap.cpp
)

set( OCLBootstrap_HEADERS
  OCLBootstrap.h
)

add_executable( OCLBootstrap
  ${OCLBootstrap_SOURCES}
  ${OCLBootstrap_HEADERS}
)

target_link_libraries( OCLBootstrap ${OpenCL_LIBRARIES} )




