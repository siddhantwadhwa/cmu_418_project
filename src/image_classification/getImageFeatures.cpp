#include <stdio.h>
#include <stdlib>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

vector<int> getImageFeatures(cv::Mat wordMap, int dictionarySize){

    int rows = wordMap.rows;
    int cols = wordMap.cols;

    int numPixels = rows*cols;

    vector<int> histogram(dictionarySize);

    for (int i = 0; i < dictionarySize; i++){
        histogram[i] = 0;
    }

    for (int row = 0; row < rows; row++){
        for (int col = 0; col < cols; col++){
            int index = wordMap.at<int>(row, col);
            histogram[index] += 1;
        }
    }

    for (int i = 0; i < dictionarySize; i++){
        histogram[i] = histogram[i]/numPixels;
    }

    return histogram;

}