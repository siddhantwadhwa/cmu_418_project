#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>


int main(int argc, char** argv){

    if( argc != 2)
    {
        printf("usage: evaluateRecognitionSystem_NN <train_features_path> <train_labels_path> <imagenames_list_path> <classification_data_fir_path>\n");
        return -1;
    }


    cv::FileStorage trainF(argv[1], cv::FileStorage::READ);

    cv::Mat trainFeatures;
    trainF["trainFeatures"] >> trainFeatures;
    trainF.release();

    //assumes labels are stored as a .yml file
    cv::FileStorage trainLF(argv[2], cv::FileStorage::READ);
    cv::Mat trainLabels;
    trainLF["trainLabels"] >> trainLabels;
    trainLF.release();

    std::vector<string> imagePaths;
    std::string line;

    ifstream infile;
    infile.open(argv[3]);
    int numLines = 0;
    while (!infile.eof()){
        getline(infile, line);
        if(line.length()==0)
            break;
        std::string tmp = std::string(argv[4]) + "/" + std::string(line);
        imagePaths.push_back(tmp);
        numLines += 1;
    }
    infile.close();

    int correct = 0;
    cv::Mat confusionMatrix = cv::Mat::zeros(8, 8, CV_32S);

    for (int i = 0; i < numLines; i++){
        //assumes wordMaps are stored as .yml files
        std::string wMapPath = imagePaths[i] + "_wm";
        cv::FileStorage wMapFile(wMapPath, cv::FileStorage::READ);
        cv::Mat wMap;
        wMapFile["map"] >> wMap;
        wMapFile.release();
        std::vector<int> h = getImageFeatures(wMap, K);
        /* assumes getImageDistance takes in a histogram (vector of ints) and 
         * trainFeatures (cv::Mat of ints)
         */
        std::vector<float> distances = getImageDistance(h, trainFeatures);
        std::vector<float>::iterator best = std::min_element(std::begin(distances), std::end(distances));
        int predictedLabel = std::distance(std::begin(distances), best) + 1;
        int actualLabel = trainLabels.at<int>(i, 0);
        if (predictedLabel == actualLabel){
            correct += 1;
        }
        confusionMatrix.at<int>(actualLabel-1, predictedLabel-1) += 1;
    }

    float accuracy = (float)correct/numLines * 100.0;


}