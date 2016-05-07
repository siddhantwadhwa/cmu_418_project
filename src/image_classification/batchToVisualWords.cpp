#include <stdio.h>
#include <stdlib>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include "create_filter_bank.h"

using namespace std;

int main(){

    if( argc != 3)
    {
        printf("usage: batchToVisualWords <imagenames_path> <wordmap_path>\n");
        return -1;
    }

    vector<string> imagePaths;
    string line;

    ifstream infile;
    infile.open(argv[1]);
    int numLines = 0;
    while (!infile.eof){
        getline(infile, line);
        imagePaths.push_back(line);
        numLines += 1;
    }
    infile.close();

    string wordMapPath(argv[2]);

    cv::Mat dictionary = get_dictionary(imagePaths, 50, 100);
    filter_bank filterBank = create_filter_bank();

    for (int i = 0; i < numLines; i++){
        cv::Mat image = cv::imread(imagePaths[i]);
        cv::Mat wordMap = get_visual_words(image, dictionary, filterBank);
        string storePath = wordMapPath + "wm" + imagePaths[i];
        cv::imwrite(storePath, wordMap);
    }


}