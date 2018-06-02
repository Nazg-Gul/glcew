project(glcew)

cmake_minimum_required (VERSION 2.6)
if (NOT CMAKE_VERSION VERSION_LESS 2.8.4)
  cmake_policy (SET CMP0017 NEW)
endif()

include(CMakeParseArguments)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
message(STATUS "Project source dir = ${PROJECT_SOURCE_DIR}")
message(STATUS "Project build dir = ${CMAKE_BINARY_DIR}")

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

include_directories(include)

add_library(glcew
  source/glcew.c

  include/glcew.h
)

add_executable(testglcew glcewTest/glcewTest.c include/glcew.h)
target_link_libraries(testglcew glcew ${CMAKE_DL_LIBS})
