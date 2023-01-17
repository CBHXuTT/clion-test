cmake_minimum_required(VERSION 3.23)
project(clion)

set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    set(CMAKE_CXX_FLAGS "/EHsc")
else ()
    set(CMAKE_CXX_FLAGS "-fexceptions")
endif ()


include_directories(${CMAKE_SOURCE_DIR})

add_definitions(-DMODULE_NAME="native")
add_definitions(-DASIO_STANDALONE)

add_executable(clion main.cpp easylog.cpp boost/sml.hpp boost/mp.hpp)
