#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <ctime>
#include "../primitives/baseline_convolve.h"

std::vector<cv::Mat> extractFilterResponses(cv::Mat I, filter_bank filterBank);

