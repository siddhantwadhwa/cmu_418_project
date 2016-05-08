#include "get_visual_words.h"

// Returns row index of the visual word with the least euclidean distance
int find_closest_word(cv::Mat dict, cv::Mat pixel_response)
{
    std::cout<<"Entered find_closest_word\n";
    cv::Mat distances = cv::Mat::zeros(dict.rows,1,CV_32F);
    
    // Compute distances to each row of the dict
    for(int row=0; row<dict.rows; row++)
    {
        float distance = 0;
        for(int col=0; col<dict.cols; col++)
        {
            float tmp = (pixel_response.at<float>(0,col)*dict.at<float>(row,col));
            distance += sqrt(tmp*tmp);
        }
        distances.at<float>(row,0) = distance;
    }

    // Find index of row with minimum distance
    float min_idx = 0;
    float min_dist = distances.at<float>(0,0);
    for(int x=0; x<dict.rows; x++)
    {
        if(distances.at<float>(x,0)<min_dist)
        {
            min_idx = x;
            min_dist = distances.at<float>(x,0);
        }
        
    }
    std::cout<<"Exited find_closest_word\n";

    return min_idx;
}

cv::Mat get_visual_words(cv::Mat img, cv::Mat dict, filter_bank fb)
{
    std::cout<<"Entered get_visual_words\n";
    // Get filter responses
    std::vector<cv::Mat> filter_responses = extractFilterResponses(img, fb);

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

    // Build wordmap by associating each pixel to the index of the 
    // closest word in the dictionary (cost : euclidean distance)
    int num_pixels = img.rows*img.cols;
    cv::Mat wordmap = cv::Mat::zeros(img.rows, img.cols, CV_8S);
    for(int row=0; row<img.rows; row++)
    {
        for(int col=0; col<img.cols; col++)
        {
            cv::Mat pixel_response = row_wise_filter_response.row(row*img.cols+col);
            wordmap.at<int>(row,col) = find_closest_word(dict, pixel_response);
        }
    }

    std::cout<<"Exiting get_visual_words\n";
    return wordmap;
}
