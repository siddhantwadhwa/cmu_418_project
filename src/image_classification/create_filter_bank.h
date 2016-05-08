#include <stdio.h>
#include <opencv2/opencv.hpp>

struct filter_bank {
    int count;
    std::vector<cv::Mat> filters;
};
