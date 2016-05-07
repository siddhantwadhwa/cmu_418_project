#include <stdio.h>
#include <stdlib>
#include <opencv2/opencv.hpp>
#include <ctime>
#include "baseline_convolve.h"

std::vector<cv::Mat> extractFilterResponses(cv:Mat I, filter_bank filterBank)
{
    
    cv::Mat ch1, ch2, ch3;
    cv::Mat Im;
    if (I.dims != 3){
        channels[0] = I.clone();
        channels[1] = I.clone();
        channels[2] = I.clone();
        cv::merge(channels, Im);
    }
    else {
        Im = I.clone();
    }

    cv::Mat im;
    cvtColor(Im, im, COLOR_BGR2Lab);
    std::vector<cv::Mat> channels(3);
    cv::split(im, channels);
    ch1 = channels[0];
    ch2 = channels[1];
    ch3 = channels[2];

    std::vector<cv::Mat> filterResponses(3*filterBank.count);

    for (int i = 0; i < filterBank.count; i++){
        cv:Mat filter = filterBank.filters[i];
        for (int j = 0; j < 3; j++){
            filterResponses[i*3 + j] = baseline_convolve(channels[j], filter);
        }
    }

    return filterResponses;


}