cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (Parallel_CV)

# version number
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# Link CUDA libraries
find_package(CUDA QUIET REQUIRED)

# Link OpenCV libraries
find_package( OpenCV REQUIRED )

# Linking PCL library
#find_package(PCL 1.3 REQUIRED COMPONENTS common io)
#include_directories(${PCL_INCLUDE_DIRS})
#link_directories(${PCL_LIBRARY_DIRS})
#add_definitions(${PCL_DEFINITIONS})

# Specify binary name and source file to build it from
cuda_add_executable(
    sample_cuda
    src/sample_cuda.cu)

# Specify executable file to be created for the sample
# OpenCV source code and link opencv libraries to it
add_executable( sample_opencv src/sample_opencv.cpp )
target_link_libraries( sample_opencv ${OpenCV_LIBS} )
add_executable( baseline_convolve src/primitives/baseline_convolve.cpp )
target_link_libraries( baseline_convolve ${OpenCV_LIBS} )
#set_property(TARGET baseline_convolve PROPERTY CXX_STANDARD 11)

# Specify executable file to be created for the sample
# PCL source code and link PCL libraries to it
#add_executable(sample_pcl src/pcd_write.cpp)
#target_link_libraries(sample_pcl ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES})

