cmake_minimum_required(VERSION 3.14)

project(main)

file(GLOB_RECURSE SOURCES "source/*.cpp" "resources/*.cpp")
file(GLOB_RECURSE HEADERS "source/*.h" "resources/*.h")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})

