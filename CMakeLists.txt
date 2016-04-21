cmake_minimum_required (VERSION 2.8)
project (Parallel_CV)

# version number
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# Link CUDA libraries
find_package(CUDA QUIET REQUIRED)

# Specify binary name and source file to build it from
cuda_add_executable(
    cudaSample
    src/sample.cu)

