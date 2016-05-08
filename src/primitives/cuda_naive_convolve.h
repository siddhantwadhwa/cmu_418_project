#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <ctime>

cv::Mat cuda_convolve(cv::Mat cpu_image, cv::Mat cpu_kernel);