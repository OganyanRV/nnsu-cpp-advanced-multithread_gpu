cmake_minimum_required( VERSION 3.13 )

message( STATUS "Programming Course Reports" )

include(${CMAKE_SOURCE_DIR}/cmake/Configure.cmake)

############################### LATEX ###############################
option(USE_LATEX OFF)
if( USE_LATEX )
    include( 3rdparty/UseLATEX/UseLATEX.cmake )
endif( USE_LATEX )

add_subdirectory(modules)
