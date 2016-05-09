#include "cuda_get_visual_words.h"

// Cuda scheduling parameters
#define BLOCK_SIDE 16
#define THREADS_PER_BLOCK (BLOCK_SIDE*BLOCK_SIDE)

// Helper macros
#define UPDIV(x,y) ((x+y-1)/y)



__global__ 
void cuda_wordmap_kernel(int img_rows, int img_cols, int row_wise_fr_cols, int dict_rows, int dict_cols, float* row_wise_fr, float* dict, int* wordmap)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row>=img_rows || col>=img_cols)
        return;

    // Intializing min_idx and min_dist
    int min_idx = 0;
    float min_dist = 0;
    for(int j=0; j<row_wise_fr_cols; j++)
    {
        //float tmp = row_wise_fr[row*row_wise_fr_cols+col][j] - dict[0][j];
        float tmp = row_wise_fr[(row*row_wise_fr_cols+col)*row_wise_fr_cols + j] - dict[0 + j];
        min_dist = sqrt(tmp*tmp);
    }

    for(int i=1; i<dict_rows; i++)
    {
        for(int j=0; j<row_wise_fr_cols; j++)
        {
            //float tmp = row_wise_fr[row*row_wise_fr_cols+col][j] - dict[i][j];
            float tmp = row_wise_fr[(row*row_wise_fr_cols+col)*row_wise_fr_cols + j] - dict[i*dict_cols + j];
            float dist = sqrt(tmp*tmp);
            if(dist<min_dist)
            {
                min_dist = dist;
                min_idx = i;
            }
        }
    }

    wordmap[row*img_cols + col] = min_idx;
    return;    
}

cv::Mat cuda_get_visual_words(cv::Mat img, cv::Mat dict, filter_bank fb)
{
    // Get filter responses
    std::vector<cv::Mat> filter_responses = extractFilterResponses(img, fb,0);
    
    // Reshape filter_responses, so that each pixel gets a row of 3n dimensions
    cv::Mat row_wise_filter_response = cv::Mat::zeros(img.rows*img.cols, filter_responses.size(), CV_32F);
    for(int row=0; row<img.rows; row++)
    {
        for(int col=0; col<img.cols; col++)
        {
            for(int i=0; i<filter_responses.size(); i++)
            {
                row_wise_filter_response.at<float>(row*img.cols+col,i) = 
                filter_responses[i].at<float>(row,col);
        
            }
        }
    }
    
    int deviceCount = 0;
    cudaError_t err = cudaGetDeviceCount(&deviceCount);

    printf("---------------------------------------------------------\n");
    printf("Initializing CUDA for CudaRenderer\n");
    printf("Found %d CUDA devices\n", deviceCount);

    // Fetch cuda device specifications and display
    cudaDeviceProp deviceProps;
    cudaGetDeviceProperties(&deviceProps, 0);
    std::string name = deviceProps.name;
    printf("Device : %s\n", deviceProps.name);
    printf("   SMMs:        %d\n", deviceProps.multiProcessorCount);
    printf("   Global mem: %.0f MB\n", static_cast<float>(deviceProps.totalGlobalMem) / (1024 * 1024));
    printf("   CUDA Cap:   %d.%d\n", deviceProps.major, deviceProps.minor);
    printf("---------------------------------------------------------\n");

    // Declare pointers to device data
    float* device_row_wise_filter_responses;
    float* device_dict;
    int* device_wordmap;

    // Compute sizes of data to be transferred to device
    int row_wise_filter_responses_size = row_wise_filter_response.rows * row_wise_filter_response.cols * sizeof(float);
    int dict_size = dict.rows*dict.cols*sizeof(float);
    int wordmap_size = img.rows*img.cols*sizeof(int);
    
    // Allocate device memory
    cudaMalloc( (void **)&device_row_wise_filter_responses, row_wise_filter_responses_size );
    cudaMalloc( (void **)&device_dict, dict_size );
    cudaMalloc( (void **)&device_wordmap, wordmap_size );

    // Copy over data to device
    cudaMemcpy( device_row_wise_filter_responses, row_wise_filter_response.data, row_wise_filter_responses_size, cudaMemcpyHostToDevice );
    cudaMemcpy( device_dict, dict.data, dict_size, cudaMemcpyHostToDevice );

    // Establish cudaThread grid
    dim3 block_dim(BLOCK_SIDE, BLOCK_SIDE);
    dim3 block_grid_dim(UPDIV(img.cols, block_dim.x),
                        UPDIV(img.rows, block_dim.y));

    // Launch cuda kernel
    cuda_wordmap_kernel<<<block_grid_dim, block_dim>>>(img.rows, img.cols, row_wise_filter_response.cols, dict.rows, dict.cols,
                                                       device_row_wise_filter_responses, device_dict, device_wordmap);
    cudaThreadSynchronize();

    int* wordmap_arr = (int*)malloc(wordmap_size);
    cudaMemcpy(wordmap_arr, device_wordmap, dict_size, cudaMemcpyDeviceToHost);
    cv::Mat wordmap = cv::Mat(img.rows, img.cols, CV_32S, wordmap_arr);

    return wordmap;
}

