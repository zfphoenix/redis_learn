cmake_minimum_required(VERSION 2.6)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(hiredis REQUIRED)

SET(CMAKE_USE_RELATIVE_PATH true)

SET(SRC_PATH ./src/)

SET(SRC_LIST
${SRC_PATH}redis.cc	
		)

ADD_EXECUTABLE(redd ${SRC_LIST})

TARGET_LINK_LIBRARIES(redd "hiredis")
