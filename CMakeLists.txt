cmake_minimum_required (VERSION 2.8)
project (Parallel_CV)

# version number
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# Link CUDA libraries
find_package(CUDA QUIET REQUIRED)

# Link OpenCV libraries
find_package( OpenCV REQUIRED )

# Specify binary name and source file to build it from
cuda_add_executable(
    sample_cuda
    src/sample_cuda.cu)

# Specify executable file to be created for the sample
# OpenCV source code and link opencv libraries to it
add_executable( sample_opencv src/sample_opencv.cpp )
target_link_libraries( sample_opencv ${OpenCV_LIBS} )

