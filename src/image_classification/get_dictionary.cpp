#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <stdlib.h>


//std::vector<cv::Point> has 2 int type elements x and y

std::vector<cv::Point> get_random_points(int alpha, int rows, int cols)
{  
    std::vector<cv::Point> vec;
    for(int i=0; i<alpha; i++)
    {
        vec.push_back(cv::Point(rand()%cols, rand()%rows));
    }
    return vec;    
}    

cv::Mat get_dictionary(std::vector<std::string> imgPaths, int alpha, int K)
    
// TODO return the dict formed from this:{

    // Create filter bank
    filter_bank fb = create_filter_bank();
    std::vector<std::vector<CV_32F>> alpha_responses;
    std::vector<CV_32F> row;

    for(auto const& path: imgPaths)
    {
        // Open image
        cv::Mat img = imread(path);
        
        // Apply filterbank to the image
        std::vector<cv::Mat> filter_responses = extract_filter_responses(img, fb);

        // Get random points to sample
        std::vector<cv::Point> random_points = get_random_points(alpha, img.rows, img.cols);
        
        // Iterate over all random points and form a row vector of filter responses for each
        for (cv::Point const& pt: random_points)
        {
            row.clear();
            for (int i=0; i<fiter_responses.size(); i++)
            {
                row.push_back(filter_responses[i].at<float>(pt.x,pt.y));
            }
            alpha_responses.push_back(row);
        }
    }

    // Form cv::Mat from vector of vectors
    cv::Mat alpha_resp_mat = cv::CreateMat(alpha_responses.size(), alpha_responses[0].size(), CV_32F);
    for (size_t i = 0; i <alpha_responses.size(); i++) {
        for (size_t j = 0; j < alpha_responses[i].size(); j++) {
            alpha_resp_mat.at<double>(i, j) = alpha_responses[i][j];
        }
    }
    
    cv::Mat labels;
    cv::Mat centers(K, alpha_resp_mat.cols , CV_32F);
    
    // TODO return the dict formed from this:
    double kmeans(alpha_resp_mat, K, labels,
                  cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0),
                  5, cv::KMEANS_PP_CENTERS, centers  );

    return centers;
}
