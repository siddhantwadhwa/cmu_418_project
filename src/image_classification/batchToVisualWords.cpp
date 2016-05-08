#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include "create_filter_bank.h"
#include "get_dictionary.h"
#include "get_visual_words.h"


using namespace std;

int main(int argc, char** argv){

    if( argc != 4)
    {
        printf("usage: batchToVisualWords <imagenames_list_path> <classification_data_fir_path> <path_to_filterbank_yaml_file>\n");
        return -1;
    }

    std::vector<string> imagePaths;
    std::string line;

    ifstream infile;
    infile.open(argv[1]);
    int numLines = 0;
    while (!infile.eof()){
        getline(infile, line);
        if(line.length()==0)
            break;
        std::string tmp = std::string(argv[2]) + "/" + std::string(line);
        imagePaths.push_back(tmp);
        numLines += 1;
    }
    infile.close();

    string wordMapPath(argv[2]);

    cv::Mat dictionary = get_dictionary(imagePaths, 50, 100, argv[3]);
/* 
    filter_bank filterBank = create_filter_bank();

    
    for (int i = 0; i < numLines; i++){
        cv::Mat image = cv::imread(imagePaths[i]);
        cv::Mat wordMap = get_visual_words(image, dictionary, filterBank);
        string storePath = std::string(imagePaths[i]) + "_wm";
        cv::imwrite(storePath, wordMap);
    }
*/  

}
