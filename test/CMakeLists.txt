project(ubench-tests)
cmake_minimum_required(VERSION 3.12)
include(CTest)

# set(CMAKE_C_STANDARD 90) set(CMAKE_CXX_STANDARD 11)

file(
  GLOB TEST_SRCS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS *.c *.cpp)

foreach(TEST_SRC IN LISTS TEST_SRCS)
  get_filename_component(TEST_BIN ${TEST_SRC} NAME_WLE)
  if(TEST_BIN MATCHES ".*\.mod$")
    continue()
  endif()
  add_executable(test-${TEST_BIN} ${TEST_SRC})
  set_property(TARGET test-${TEST_BIN} PROPERTY C_STANDARD 90)
  set_property(TARGET test-${TEST_BIN} PROPERTY C_STANDARD_REQUIRED ON)
  set_property(TARGET test-${TEST_BIN} PROPERTY CXX_STANDARD 11)
  add_test(NAME test-${TEST_BIN} COMMAND test-${TEST_BIN})
endforeach()
