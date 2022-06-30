cmake_minimum_required(VERSION 3.20)
project(oganyan_lab1)

set(CMAKE_CXX_STANDARD 14)

add_executable(oganyan_lab1 main.cpp)

set(FIND_LIBRARY_USE_LIB64_PATHS TRUE)
find_package(CUDAToolkit 8.0 REQUIRED)

message("${CUDA_CUT_INCLUDE_DIR}")
message("${CUDAToolkit_LIBRARY_DIR}")
message("${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}")
message("${CUDA_TOOLKIT_ROOT_DIR}")
target_link_libraries(oganyan_lab1 PUBLIC CUDA::OpenCL)

