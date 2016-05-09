#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "create_filter_bank.h"
#include "extractFilterresponses.h"

cv::Mat cuda_get_visual_words(cv::Mat img, cv::Mat dict, filter_bank fb);
