cmake_minimum_required(VERSION 3.10)

project(harmony-server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "-fsanitize=address")
endif()


add_subdirectory(harmony-http/)

add_executable(harmony_server 
  src/harmony_server.cc
  src/auth.h
  src/auth.cc
  src/routes.h
  src/routes.cc
  src/data/error.h
  src/data/login.h
  src/data/register.h
  src/data/user.h
)

target_link_libraries(harmony_server harmony_http)
