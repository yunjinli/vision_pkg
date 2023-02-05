cmake_minimum_required(VERSION 3.16)

project(vision_pkg)

set(OPENCV_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/build-opencv")

SET(CXX_MARCH native)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OPENCV_INCLUDE_DIR_HINTS ${OPENCV_ROOT})
find_package(OpenCV REQUIRED CONFIG HINTS ${OPENCV_ROOT} NO_DEFAULT_PATH)

set(OPENCV_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/opencv/include")
include_directories(${OPENCV_INCLUDE_DIR})
include_directories(include)

message(STATUS "Found OpenCV ${OpenCV_VERSION} headers in: ${OPENCV_INCLUDE_DIR}")
message(STATUS "Found Opencv headers in: ${OPENCV_INCLUDE_DIR}")

add_executable(DisplayImage src/DisplayImage.cpp)
target_link_libraries(DisplayImage ${OpenCV_LIBS})

add_executable(optical_flow src/optical_flow.cpp)
target_link_libraries(optical_flow ${OpenCV_LIBS})

add_executable(dense_optical_flow src/dense_optical_flow.cpp)
target_link_libraries(dense_optical_flow ${OpenCV_LIBS})

add_executable(display_webcam src/display_webcam.cpp)
target_link_libraries(display_webcam ${OpenCV_LIBS})

add_library(lukas_kanal src/lukas_kanal.cpp)
target_link_libraries(lukas_kanal ${OpenCV_LIBS})
add_subdirectory(test)