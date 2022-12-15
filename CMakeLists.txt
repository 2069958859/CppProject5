cmake_minimum_required(VERSION 3.16)

project(pro5)

add_definitions(-mavx2 -DWITH_AVX2)

add_definitions(-fopenmp -lgomp)

set(CMAKE_CXX_STANDARD 11)

add_executable(mat source.cpp operation.cpp )#加入可执行文件，空格分开   
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message("OpenMP found.")
   target_link_libraries(mat  OpenMP::OpenMP_CXX)
endif()
