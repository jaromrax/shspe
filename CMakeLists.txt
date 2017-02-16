#
#
#
cmake_minimum_required (VERSION 2.6)

project (shspe)
add_executable(shspe shspe.C)
include_directories("$ENV{ROOTSYS}/include")
#
#
#
#SET(CMAKE_C_COMPILER $ENV{ROOTSYS}/bin/root)
#set(CMAKE_CXX_COMPILER $ENV{ROOTSYS}/bin/root)
set(CMAKE_C_COMPILER "./repeatall")
SET(CMAKE_CXX_COMPILER "./repeatall")
