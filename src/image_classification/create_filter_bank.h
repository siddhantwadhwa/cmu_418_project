#include <stdio.h>
#include <opencv2/opencv.hpp>

#ifndef FILTER_BANK_STRUCT
#define FILTER_BANK_STRUCT
struct filter_bank {
    int count;
    std::vector<cv::Mat> filters;
};

#endif

filter_bank create_filter_bank(std::string fb_path);
