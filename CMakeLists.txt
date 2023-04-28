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

add_executable(ZstuUserNotifications
        ZstuUserNotifications/include/receiver.h
        ZstuUserNotifications/src/main.c
        ZstuUserNotifications/src/receiver.c)
