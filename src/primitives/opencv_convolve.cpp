#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <ctime>

int main(int argc, char** argv )
{
    // Keep track of total size of working set
    // int working_set; TODO: not yet implemented

    if( argc != 2)
    {
        printf("usage: baseline_convolve <image_path> <kernel_path>\n");
        return -1;
    }

    // Reading image file
    cv::Mat gray_image = cv::imread(argv[1], 0 );
    
    // Starting timer
    std::clock_t start;
    double duration;
    start = std::clock();

    cv::Mat op_image;
    boxFilter(gray_image, op_image, -1, Size(10,10), Point anchor=Point(-1,-1), bool normalize=true, int borderType=cv::BORDER_REPLICATE )

    // Ending timer
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"CPU time taken: "<< duration <<" seconds\n";
    
    // Displaying results
    cv::namedWindow( "Source Image", cv::WINDOW_AUTOSIZE );
    cv::namedWindow( "Convolved Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Source Image", gray_image);
    cv::imshow("Convolved Image", op_image);

    cv::waitKey(0);

    return 0;

}
