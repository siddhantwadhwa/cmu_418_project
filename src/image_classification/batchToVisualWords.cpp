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
#include "cuda_get_visual_words.h"


using namespace std;

int main(int argc, char** argv){

    if( argc != 4 && argc != 5 )
    {
        printf("usage: batchToVisualWords <imagenames_list_path> <classification_data_fir_path> <path_to_filterbank_yaml_file> <path_to_read_dict_from>\n");
        return -1;
    }
    std::string dict_path = "";
    
    if(argc==5)
        dict_path = argv[4];


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

    cv::Mat dictionary;
    if(dict_path=="")
    {
        dictionary = get_dictionary(imagePaths, 50, 100, argv[3]);
        cv::FileStorage fs("dict.yml", cv::FileStorage::WRITE );
        fs << "dict" << dictionary;
        fs.release();
        std::cout<<"Written dict to file\n";
    }   
    else
    {
        cv::FileStorage fs(dict_path , cv::FileStorage::READ);
        std::cout<<"Reading dict from file\n";
        fs["dict"] >> dictionary;
        std::cout<<"Done with reading the dictionary from file, with size ("<<dictionary.rows<<","<<dictionary.cols<<") and type "<<dictionary.type()<<"\n";
        fs.release();
    }

    filter_bank filterBank = create_filter_bank(argv[3]);
    
    for (int i = 0; i < numLines; i++){
        std::string wm_name = imagePaths[i].substr(0,imagePaths[i].length()-4);
        wm_name = wm_name + "_wm.yml";

        std::cout<<"Writing wordmap : "<<i<<" "<<wm_name<<std::endl; 
        
        cv::Mat img = cv::imread(imagePaths[i]);
        cv::Mat image;
        img.convertTo(image, CV_32F);
        cv::Mat wordMap = cuda_get_visual_words(image, dictionary, filterBank);
        
        cv::FileStorage fs(wm_name, cv::FileStorage::WRITE );
        fs << "wmap" << wordMap;
        fs.release();
        //std::cout<<"end of loop\n"; 

    }

}
