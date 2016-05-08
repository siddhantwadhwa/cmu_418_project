#include "cuda_naive_convolve.h"


// Cuda scheduling parameters
#define BLOCK_SIDE 16
#define THREADS_PER_BLOCK (BLOCK_SIDE*BLOCK_SIDE)

// Helper macros
#define UPDIV(x,y) ((x+y-1)/y)

struct msg {
    float* padded_image;
    float* result_image;
    int padded_image_rows;
    int padded_image_cols;
    float* kernel;
};



msg cuda_device_setup(cv::Mat padded_image, cv::Mat kernel) {
    
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

    // Allocating memory to store the input image, kernel and output result
    // on the cuda device
    float* device_padded_image;
    float* device_kernel;
    float* device_result;
    int kernel_radius = kernel.rows/2;
    int kernel_size = kernel.rows*kernel.cols*sizeof(float);
    int result_rows = (padded_image.rows - 2*kernel_radius);
    int result_cols = (padded_image.cols - 2*kernel_radius);
    int result_size = result_rows * result_cols * sizeof(float);
    int padded_image_size = padded_image.rows * padded_image.cols * sizeof(float);
    cudaMalloc( (void **)&device_padded_image, padded_image_size );
    cudaMalloc( (void **)&device_kernel, kernel_size);
    cudaMalloc( (void **)&device_result, result_size);

    cv::Mat result = cv::Mat::zeros(result_rows, result_cols, CV_32F);

    // Transferring image and kernel data to cuda device global memory
    cudaMemcpy( device_padded_image, padded_image.data,
                                padded_image_size, cudaMemcpyHostToDevice );
    cudaMemcpy( device_kernel, kernel.data, kernel_size, cudaMemcpyHostToDevice );
    cudaMemcpy( device_result, result.data, result_size, cudaMemcpyHostToDevice );

    msg device_ptrs;
    device_ptrs.padded_image = device_padded_image;
    device_ptrs.kernel = device_kernel;
    device_ptrs.padded_image_rows = padded_image.rows;
    device_ptrs.padded_image_cols = padded_image.cols;
    device_ptrs.result_image = device_result;

    return device_ptrs;
    
}

__global__ void kernel_convolve( int image_rows, int image_cols, int kernel_rows, int kernel_cols,
                                 int padded_img_rows, int padded_img_cols, int kernel_radius, 
                                 float kernel_sum,
                                 float* padded_image, float* kernel_arr, float* device_result )
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    //printf("(%d, %d)\n", row, col);   
    
    if(row<image_rows && col<image_cols)
    {
       int kernel_start_row = row - kernel_radius;
       int kernel_start_col = col - kernel_radius;
       
       for(int k_row=0; k_row<(kernel_rows); k_row++)
       {
           for(int k_col=0; k_col<(kernel_cols); k_col++)
           {
               //printf("result(%d,%d) += ",row,col);
               //printf("kernel(%d,%d) * ",k_row,k_col);
               //printf("padded_image(%d,%d)\n",kernel_radius+kernel_start_row+k_row,kernel_radius+kernel_start_col+k_col);
               device_result[(row*image_cols+col)] += 
               kernel_arr[((k_row)*kernel_cols+k_col)] *
               padded_image[((kernel_radius+kernel_start_row+k_row)*padded_img_cols + 
                                    kernel_radius+kernel_start_col+k_col)] / kernel_sum;
           }
       } 
    }
}


cv::Mat cuda_convolve(cv::Mat cpu_image, cv::Mat cpu_kernel) 
{   // TODO : pass by ref
    
    // Establish cudaThread grid
    dim3 block_dim(BLOCK_SIDE, BLOCK_SIDE);
    dim3 block_grid_dim(UPDIV(cpu_image.cols, block_dim.x),
                        UPDIV(cpu_image.rows, block_dim.y));
    //dim3 block_dim(1,2);
    //dim3 block_grid_dim(1,1);
    

    int kernel_radius = cpu_kernel.rows/2;
    
    // Pad image
    cv::Mat padded_image;
    cv::copyMakeBorder( cpu_image, padded_image, kernel_radius, kernel_radius, 
                        kernel_radius, kernel_radius, cv::BORDER_REPLICATE);

    //std::cout<<"padded = \n"<<padded_image;
    // Prepare the cuda device for kernel launch
    msg device_ptrs = cuda_device_setup(padded_image, cpu_kernel);
    float kernel_sum;
    for(int i=0; i<cpu_kernel.rows; i++)
    {
        for(int j=0; j<cpu_kernel.cols; j++)
        {
            kernel_sum += cpu_kernel.at<float>(i,j);
        }
    }

    // Start timer
    std::clock_t start;
    double duration;
    start = std::clock();

    // Launch Cuda convolve kernel
    kernel_convolve<<<block_grid_dim, block_dim>>>(cpu_image.rows, cpu_image.cols,
                                                   cpu_kernel.rows, cpu_kernel.cols,
                                                   device_ptrs.padded_image_rows,
                                                   device_ptrs.padded_image_cols,
                                                   kernel_radius, kernel_sum,
                                                   device_ptrs.padded_image,
                                                   device_ptrs.kernel,
                                                   device_ptrs.result_image
                                                   );
    
    // Wait for computation to complete across all cuda threads
    cudaThreadSynchronize();

    // Stopping timer
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Time taken: "<< duration <<" seconds\n";

    // Copy result array back from CUDA memory
    int result_size = cpu_image.rows * cpu_image.cols * sizeof(float);
    float* result_arr = (float*)malloc(result_size);
    cudaMemcpy(result_arr, device_ptrs.result_image, result_size, cudaMemcpyDeviceToHost);
    
    // Convert float array to cv::Mat
    cv::Mat result = cv::Mat(cpu_image.rows, cpu_image.cols, CV_32F, result_arr);
    cv::Mat op_image;

    // Cast to CV_8U for display
    result.convertTo(op_image,CV_8U);
    
    return op_image;
}

int main(int argc, char** argv )
{
    if( argc != 2)
    {
        printf("usage: baseline_convolve <image_path> <kernel_path>\n");
        return -1;
    }

    // Reading image file
    cv::Mat gray_image = cv::imread(argv[1], 0 );
    cv::Mat image;
    gray_image.convertTo(image, CV_32F);
    //cv::Mat image = (cv::Mat_<float>(5,4) << 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20);
    
    // Reading kernel file
    cv::Mat kernel = cv::Mat::ones(31, 31, CV_32F);;

    // Convolve
    cv::Mat result = cuda_convolve(image, kernel);
    //std::cout<<"Result = \n"<<result;
    
    // Displaying results
    cv::namedWindow( "Source Image", cv::WINDOW_AUTOSIZE );
    cv::namedWindow( "Convolved Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Source Image", gray_image);
    cv::imshow("Convolved Image", result);

    cv::waitKey(0);

    return 0;

}





