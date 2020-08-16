cmake_minimum_required(VERSION 3.16)
project(RStest_c)

set(CMAKE_CXX_STANDARD 14)
set(OpenCV_DIR /home/ljx/soft/cv/opencv-lhd-opencv-3.4.9/build)
find_package( OpenCV REQUIRED )
FIND_PACKAGE(realsense2 REQUIRED)
include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${realsense2_INCLUDE_DIRS}
)

add_executable(RStest_c main.cpp)
target_link_libraries(RStest_c ${OpenCV_LIBS} ${realsense2_LIBRARY})