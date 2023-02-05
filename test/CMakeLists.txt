cmake_minimum_required(VERSION 3.16)

add_subdirectory(googletest EXCLUDE_FROM_ALL)
# ensure gtest include directory is added before any of the
# system-wide include directories that might contain gtest
include_directories(BEFORE ${gtest_SOURCE_DIR}/include)

include(GoogleTest)

enable_testing()

add_executable(test_lukas_kanal src/test_lukas_kanal.cpp)
target_link_libraries(test_lukas_kanal ${OpenCV_LIBS} gtest gtest_main lukas_kanal)