cmake_minimum_required(VERSION 3.20)
project(oganyan_lab3)

set(CMAKE_CXX_STANDARD 17)

add_executable(oganyan_lab3 main.cpp)

set(FIND_LIBRARY_USE_LIB64_PATHS TRUE)
find_package(CUDAToolkit 8.0 REQUIRED)

message("${CUDA_CUT_INCLUDE_DIR}")
message("${CUDAToolkit_LIBRARY_DIR}")
message("${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}")
message("${CUDA_TOOLKIT_ROOT_DIR}")
target_link_libraries(oganyan_lab3 PUBLIC CUDA::OpenCL)

find_package(OpenMP REQUIRED)
target_link_libraries(oganyan_lab3 PUBLIC OpenMP::OpenMP_CXX)


file(GLOB_RECURSE TARGET_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE TARGET_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
