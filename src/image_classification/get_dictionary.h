#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "create_filter_bank.h"

std::vector<cv::Point> get_random_points(int alpha, int rows, int cols);
cv::Mat get_dictionary(std::vector<std::string> imgPaths, int alpha, int K, std::string fb_path);
