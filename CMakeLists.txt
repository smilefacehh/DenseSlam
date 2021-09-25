cmake_minimum_required( VERSION 2.8 )
project( dense-slam )

set(CMAKE_CXX_COMPILER "g++")
set( CMAKE_BUILD_TYPE Release  )
# SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-std=c++11 -g")

file(GLOB ALL_SOURCES
    "src/*.cpp"
)
file(GLOB ALL_INCLUDES
    "include/*.h"
)
set(ALL_SRCS ${ALL_SOURCES} ${ALL_INCLUDES})
include_directories(include)

# PCL
# FIND_PACKAGE( PCL REQUIRED COMPONENTS common io )
# ADD_DEFINITIONS( ${PCL_DEFINITIONS} )
# INCLUDE_DIRECTORIES( ${PCL_INCLUDE_DIRS}  )
# LINK_LIBRARIES( ${PCL_LIBRARY_DIRS} )

# octomap
find_package( octomap REQUIRED )
include_directories( ${OCTOMAP_INCLUDE_DIRS} )

# opencv
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

# eigen
find_package(Eigen3 REQUIRED)
include_directories( ${Eigen3_INCLUDE_DIRS} )

add_executable( slam ${ALL_SRCS} )
target_link_libraries( slam
    ${OCTOMAP_LIBRARIES}
    ${OpenCV_LIBS})