cmake_minimum_required(VERSION 2.8)

project(ysSocket)

set(SOURCES
	src/ysNodeV4.cpp
	src/ysServer.cpp
	src/ysClient.cpp
	include/ysNodeV4.h
	include/ysClient.h
	include/ysServer.h)

include_directories(include)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

if (UNIX)
	target_link_libraries(ysSocket pthread)
endif ()