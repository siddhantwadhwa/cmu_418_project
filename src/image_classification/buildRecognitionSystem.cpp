#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include "getImageFeatures.h"

int main(int argc, char** argv){

    if( argc != 2)
    {
        printf("usage: buildRecognitionSystem <dict_path> <imagenames_list_path> <classification_data_fir_path>\n");
        return -1;
    }

    cv::FileStorage fsDemo(argv[1], cv::FileStorage::READ);

    cv::Mat dict;
    fsDemo["dict"] >> dict;
    fsDemo.release();

    std::vector<string> imagePaths;
    std::string line;

    ifstream infile;
    infile.open(argv[2]);
    int numLines = 0;
    while (!infile.eof()){
        getline(infile, line);
        if(line.length()==0)
            break;
        std::string tmp = std::string(argv[3]) + "/" + std::string(line);
        imagePaths.push_back(tmp);
        numLines += 1;
    }
    infile.close();

    int K = 100;

    std::vector<std::vector<int>> trainFeatures(numLines);

    for (int i = 0; i < numLines; i++){
        std::string wMapPath = imagePaths[i] + "_wm";
        cv::FileStorage wMapFile(wMapPath, cv::FileStorage::READ);
        cv::Mat wMap;
        wMapFile["map"] >> wMap;
        wMapFile.release();
        trainFeatures[i] = getImageFeatures(wMap, K);
    }

    cv::Mat tFeatures = cv::Mat::zeros(numLines, K, CV_32S);
    for (int row = 0; row < numLines; row++){
        std::vector<int> rowVec = trainFeatures[row];
        for (int col = 0; col < K; col++){
            tFeatures.at<int>(row, col) = rowVec[col];
        }
    }

    cv::FileStorage fs("trainFeatures.yml", cv::FileStorage::WRITE);
    fs << "trainFeatures" << tFeatures;
    fs.release();



}