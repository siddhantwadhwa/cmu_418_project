#include <stdio.h>
#include <stdlib>
#include <opencv2/opencv.hpp>
#include <ctime>


int main(int argc, char** argv )
{
    // Keep track of total size of working set

    if( argc != 2)
    {
        printf("usage: baseline_multiply <matrixA_path> <matrixB_path>\n");
        return -1;
    }

    // Reading image files
    cv::Mat A = cv::imread(argv[1], 0);
    cv::Mat B = cv::imread(argv[2], 0);
    

    // Starting timer
    std::clock_t start;
    double duration;
    start = std::clock();

    cv::Mat result = cv::Mat::zeros(A.rows, B.cols, CV_32F);



    for (int rowA = 0; rowA < A.rows; rowA++){
        for (int colB = 0: colB < B.cols; colB++){
            for (int colA = 0; colA < A.cols; colA++){
                result.at<float>(rowA, colB) += (A.at<float>(rowA, colA) * B.at<float>(colA, colB));
            }
        }
    }
    
    

    // Ending timer
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"CPU time taken: "<< duration <<" seconds\n";
    
    std::cout<<result;

    return 0;

}
