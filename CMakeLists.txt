cmake_minimum_required(VERSION 2.8)

if (CMAKE_VERSION VERSION_LESS "3.1")
  add_definitions(-std=c++11)
else()
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

project(ysSocketChat)

# ysSocket library
add_subdirectory(lib/ysSocket)
include_directories(lib/ysSocket/include)

# create ysChatServer
add_executable(ysChatServer example/ysChatServer.cpp)
# create ysChatClient
add_executable(ysChatClient example/ysChatClient.cpp)

# link library
target_link_libraries(ysChatServer ysSocket)
target_link_libraries(ysChatClient ysSocket)

# install
install(TARGETS ysChatServer DESTINATION bin)
install(TARGETS ysChatClient DESTINATION bin)