#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <ctime>


cv::Mat baseline_convolve(cv::Mat gray_image, cv::Mat kernel)
{
    // Coverting to matrix of floats
    cv::Mat image;
    gray_image.convertTo(image,CV_32F);  
    
    // Assuming square matrix where numRows is odd
    int padding = (int)(kernel.rows/2);
    
    // Pad image with replicated border
    cv::Mat padded_image;
    cv::copyMakeBorder( image, padded_image, padding, padding, padding, padding, cv::BORDER_REPLICATE);
    
    // Initializing output cv Mat
    cv::Mat convolved = cv::Mat::zeros(image.rows, image.cols, CV_32F); 

    // calculate sum of kernel values
    float kernel_sum;
    for(int i=0; i<kernel.rows; i++)
    {
        for(int j=0; j<kernel.cols; j++)
        {
            kernel_sum += kernel.at<float>(i,j);
        }
    }
    
    // Loop limits
    int start_row = padding;
    int end_row = padded_image.rows-padding;
    int start_col = padding;
    int end_col = padded_image.cols-padding;

    float* kernel_data = (float *)kernel.data;
    float* convolved_data = (float *)convolved.data;
    float* padded_image_data = (float *)padded_image.data;
    // Naive looping over all pixels in the image
    for(int row=start_row; row<end_row; row++)
    {
        for(int col=start_col; col<end_col; col++)
        {
            int kernel_start_row = row - padding;
            int kernel_start_col = col - padding;
            for(int k_row=0; k_row<(kernel.rows); k_row++)
            {
                for(int k_col=0; k_col<(kernel.cols); k_col++)
                {
                    // Commented out code is useful for debugging
                    //std::cout<<"convolved["<<row-start_row<<"]["<<col-start_col<<"] += "<<
                    //    "kernel["<<k_row<<"]["<<k_col<<"] * padded_image["<<kernel_start_row+k_row<<
                    //    "]["<<kernel_start_col+k_col<<"]\n";
                    convolved.at<float>(row-start_row,col-start_col) += kernel.at<float>(k_row,k_col) * 
                        padded_image.at<float>(kernel_start_row+k_row,kernel_start_col+k_col) / kernel_sum;
                    //convolved.data[(row-start_row)*convolved.cols+col-start_col] += (
                    //                kernel.data[k_row*kernel.cols + k_col] * 
                    //                padded.image_data[(kernel_start_row+k_row) * padded_image.cols + 
                    //                kernel_start_col+k_col] / kernel_sum);
                }
            }
        }
    }
   
    return convolved;
} 


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
    
    // Reading kernel file
    cv::Mat kernel = cv::Mat::ones(31, 31, CV_32F);;

    // Starting timer
    std::clock_t start;
    double duration;
    start = std::clock();

    // Coverting to matrix of floats
    cv::Mat image;
    gray_image.convertTo(image,CV_32F);  
    
    // Assuming square matrix where numRows is odd
    int padding = (int)(kernel.rows/2);
    
    // Pad image with replicated border
    cv::Mat padded_image;
    cv::copyMakeBorder( image, padded_image, padding, padding, padding, padding, cv::BORDER_REPLICATE);
    
    // Initializing output cv Mat
    cv::Mat convolved = cv::Mat::zeros(image.rows, image.cols, CV_32F); 

    // calculate sum of kernel values
    float kernel_sum;
    for(int i=0; i<kernel.rows; i++)
    {
        for(int j=0; j<kernel.cols; j++)
        {
            kernel_sum += kernel.at<float>(i,j);
        }
    }
    
    // Loop limits
    int start_row = padding;
    int end_row = padded_image.rows-padding;
    int start_col = padding;
    int end_col = padded_image.cols-padding;

    float* kernel_data = (float *)kernel.data;
    float* convolved_data = (float *)convolved.data;
    float* padded_image_data = (float *)padded_image.data;
    // Naive looping over all pixels in the image
    for(int row=start_row; row<end_row; row++)
    {
        for(int col=start_col; col<end_col; col++)
        {
            int kernel_start_row = row - padding;
            int kernel_start_col = col - padding;
            for(int k_row=0; k_row<(kernel.rows); k_row++)
            {
                for(int k_col=0; k_col<(kernel.cols); k_col++)
                {
                    // Commented out code is useful for debugging
                    //std::cout<<"convolved["<<row-start_row<<"]["<<col-start_col<<"] += "<<
                    //    "kernel["<<k_row<<"]["<<k_col<<"] * padded_image["<<kernel_start_row+k_row<<
                    //    "]["<<kernel_start_col+k_col<<"]\n";
                    convolved.at<float>(row-start_row,col-start_col) += kernel.at<float>(k_row,k_col) * 
                        padded_image.at<float>(kernel_start_row+k_row,kernel_start_col+k_col) / kernel_sum;
                    //convolved.data[(row-start_row)*convolved.cols+col-start_col] += (
                    //                kernel.data[k_row*kernel.cols + k_col] * 
                    //                padded.image_data[(kernel_start_row+k_row) * padded_image.cols + 
                    //                kernel_start_col+k_col] / kernel_sum);
                }
            }
        }
    }
   
    cv::Mat op_image;
    convolved.convertTo(op_image,CV_8U);

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
