# Specify minimum CMake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Project name
PROJECT( GocadSGridReader )

IF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
        IF( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )
                MESSAGE(STATUS "Set GCC release flags")
                SET(CMAKE_CXX_FLAGS "-O3 -DNDEBUG")
	ELSE()
                MESSAGE(STATUS "Set GCC debug flags")
                SET(CMAKE_CXX_FLAGS "-g")
        ENDIF()
        # add code quality options
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wall -Wextra -fno-nonansi-builtins")
	# enable C++11
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        ADD_DEFINITIONS(
                -DGCC
        )
ENDIF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

ADD_SUBDIRECTORY(GocadSGridReader)

# Set build directories
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )

