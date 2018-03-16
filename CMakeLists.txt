# NEW COMPILATION.  CMAKE
#    cmake .;  cmake --build .
#
cmake_minimum_required (VERSION 2.6)


# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git tag
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GITTAG
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
execute_process(
  COMMAND cat /etc/issue.net
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE ISSUE
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
execute_process(
  COMMAND root-config --version
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE ROOTVER
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
execute_process(
  COMMAND date +%Y/%m/%d %H:%M:%S
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE COMPDATE
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )



project (shspe)
add_executable(shspe shspe.C)
include_directories("$ENV{ROOTSYS}/include")
set(cf_example_VERSION 0.1)
configure_file(sh_ver.h.in  sh_ver.h)
#
#
#
#SET(CMAKE_C_COMPILER $ENV{ROOTSYS}/bin/root)
#set(CMAKE_CXX_COMPILER $ENV{ROOTSYS}/bin/root)
set(CMAKE_C_COMPILER "./repeatall")
SET(CMAKE_CXX_COMPILER "./repeatall")
#configure_file(ver.h.in ${PROJECT_BINARY_DIR}/ver.h)
