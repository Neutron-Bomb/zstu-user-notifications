cmake_minimum_required(VERSION 3.25)
project(ZstuUserNotifications C)

set(CMAKE_C_STANDARD 11)

include_directories(ZstuUserNotifications/include)

find_package(CJSON REQUIRED)
include_directories(${CJSON_INCLUDE_DIR})
link_libraries(${CJSON_LIBRARIES})

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
link_libraries(${CURL_LIBRARIES})

#find_package(Curses REQUIRED)
#include_directories(${CURSES_INCLUDE_DIR})
#link_libraries(${CURSES_LIBRARIES})

file(GLOB SOURCES "ZstuUserNotifications/src/*.c")
add_executable(ZstuUserNotifications ${SOURCES})
