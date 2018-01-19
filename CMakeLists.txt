cmake_minimum_required( VERSION 2.8 )
project( OCLBootstrapTopLevel )

find_package( OpenCL REQUIRED )

add_subdirectory( src )

