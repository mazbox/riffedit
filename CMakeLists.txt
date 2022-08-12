cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

project("riffedit")

add_executable(riffedit riffmod.cpp)
add_executable(riffinfo riffinfo.cpp)
