cmake_minimum_required(VERSION 2.6)
project(tizen-server)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations -luuid")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -luuid")

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    add_definitions(-DCOMPILEDWITHC11)
    message(STATUS "Using flag -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    add_definitions(-DCOMPILEDWITHC0X)
    message(STATUS "Using flag -std=c++0x.")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

##################################################
# Project
##################################################

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/simple-net
    ${PROJECT_SOURCE_DIR}/Thirdparty/include
    ${PROJECT_SOURCE_DIR}/scenes
)

FILE(GLOB PROJECT_SRC "src/*.cpp")
ADD_EXECUTABLE(${PROJECT_NAME} ${PROJECT_SRC})

##################################################
# OpenCV & Eigen3
##################################################
find_package(OpenCV 3.0 QUIET)
if(NOT OpenCV_FOUND)
   find_package(OpenCV 2.4.3 QUIET)
   if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
endif()

find_package(Eigen3 3.1.0 REQUIRED)

INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
${EIGEN3_LIBS}
)

##################################################
# ORB-SLAM2
##################################################

target_link_libraries(${PROJECT_NAME}
${PROJECT_SOURCE_DIR}/Thirdparty/lib/libDBoW2.so
${PROJECT_SOURCE_DIR}/Thirdparty/lib/libg2o.so
${PROJECT_SOURCE_DIR}/Thirdparty/lib/libORB_SLAM2.so
)

##################################################
# DALi
##################################################

SET(DALi_PKG_LIST dali-core dali-adaptor dali-toolkit)
INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(DALi REQUIRED ${DALi_PKG_LIST})
INCLUDE_DIRECTORIES(${DALi_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${DALi_LDFLAGS})

FOREACH(flag ${DALi_CFLAGS})
	SET(REQUIRED_CFLAGS "${REQUIRED_CFLAGS} ${flag}")
ENDFOREACH(flag)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${REQUIRED_CFLAGS} -fPIE")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")

##################################################
# Bullet
##################################################

find_package(Bullet REQUIRED)
INCLUDE_DIRECTORIES(${BULLET_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${BULLET_LIBRARIES})

##################################################
# Intel RealSense2 SDK
##################################################
find_package(realsense2 REQUIRED)
INCLUDE_DIRECTORIES(${realsesne2_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${realsense2_LIBRARY})