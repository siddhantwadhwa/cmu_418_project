#include <stdio.h>
#include <opencv2/opencv.hpp>

#define UPDIV(x,y) ((x+y-1)/y)
//#define NUM_SIZES 5
#define NUM_SIZES 1

struct filter_bank {
    int count;
    std::vector<cv::Mat> filters;
};

cv::Mat get_gaussian_kernel(int size, float sigma)
{
    cv::Mat kernel = cv::Mat::zeros(size, size, CV_32F);
    float r, s = 2.0 * sigma * sigma;
 
    // sum is for normalization
    float sum = 0.0;

    int half = size/2;
    printf("half = %d", half); 
    // generate 5x5 kernel
    for (int x = -half; x <= half; x++)
    {
        for(int y = -half; y <= half; y++)
        {   
            std::cout<<x+half<<","<<y+half<<std::endl;  
            r = sqrt(x*x + y*y);
            kernel.at<float>(x+half,y+half) = (exp(-(r*r)/s))/((float)M_PI * s);
            sum += kernel.at<float>(x+half,y+half);
        }
    }
 
    // normalize the Kernel
    for(int i = 0; i < size; ++i)
        for(int j = 0; j < size; ++j)
            kernel.at<float>(i,j) /= sum;

    return kernel;
 
}

cv::Mat get_LoG_kernel(int size, float sigma)
{
    cv::Mat kernel = cv::Mat::zeros(size, size, CV_32F);
 
    // sum is for normalization
    float sum = 0.0;

    int half = size/2;
    // generate kernel
    for (int x = -half; x <= half; x++)
    {
        for(int y = -half; y <= half; y++)
        {   
            kernel.at<float>(x+half,y+half) = (1.0 /(M_PI*pow(sigma,4))) * (1 - (x*x+y*y)/(sigma*sigma))* (pow(2.718281828, - (x*x + y*y) / 2*sigma*sigma));
            sum += kernel.at<float>(x+half,y+half);
        }
    }
 
    // normalize the Kernel
    //for(int i = 0; i < size; ++i)
    //    for(int j = 0; j < size; ++j)
    //        kernel.at<float>(i,j) /= sum;

    return kernel;
 
}


filter_bank create_filter_bank()
{
    // Declare filter_bank object
    filter_bank fb;
    
    // Create array containing sizes for kernels
    //int sizes[] = { 1, 2, 4, 8, (int)(std::sqrt(2)*8) };
    int sizes[] = { 11 };
    

    // Append gaussian kernel in all sizes
    for (int i=0; i < NUM_SIZES; i++) {
        fb.count += 1;
        fb.filters.push_back(get_gaussian_kernel(sizes[i], 2*((int)(2.5*sizes[i])+1)+1 ));
    }
    
    // Append Laplacian of Gaussian (LoG) kernel in all sizes
    for (int i=0; i < NUM_SIZES; i++) {
        fb.count += 1;
        fb.filters.push_back(get_LoG_kernel(sizes[i], 2*((int)(2.5*sizes[i])+1)+1 ));
    }

    // Append derivative kernel in all sizes
    for (int i=0; i < NUM_SIZES; i++) {
        fb.count += 2;
        cv::Mat G_x, G_y;
        cv::Mat G = get_gaussian_kernel(sizes[i], 2*((int)(2.5*sizes[i])+1)+1 );
        cv::Sobel(G, G_x, CV_32F, 1, 0);
        cv::Sobel(G, G_y, CV_32F, 0, 1);
        fb.filters.push_back(G_x);
        fb.filters.push_back(G_y);
    }
    
    return fb;

}

int main()
{ 
    filter_bank a = create_filter_bank();
    int i=0;
    for(auto const& value: a.filters)
    {
        i+=1;
        std::cout<<"Kernel "<<i<<std::endl<<value<<std::endl;
    }
             
    

    return 0;
}
