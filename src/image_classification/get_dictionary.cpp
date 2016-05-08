#include "get_dictionary.h"
#include "extractFilterResponses.cpp"


//std::vector<cv::Point> has 2 int type elements x and y

std::vector<cv::Point> get_random_points(int alpha, int rows, int cols)
{  
    std::vector<cv::Point> vec;
    for(int i=0; i<alpha; i++)
    {
        vec.push_back(cv::Point(rand()%rows, rand()%cols));
    }
    return vec;    
}    
cv::Mat get_dictionary(std::vector<std::string> imgPaths, int alpha, int K, std::string fb_path)
{    

    // Create filter bank
    filter_bank fb = create_filter_bank(fb_path);
    std::vector< std::vector<float> > alpha_responses;
    int i=0;
    for(auto const& path: imgPaths)
    {
        std::cout<<"img "<<i++<<" : "<<path<<std::endl;

        // Open image
        cv::Mat img = cv::imread(path);

        //cv::namedWindow("tmp",cv::WINDOW_AUTOSIZE);
        //cv::imshow("tmp",img);
        //cv::waitKey(0);
        
        // Apply filterbank to the image
        std::vector<cv::Mat> filter_responses = extractFilterResponses(img, fb);
        
        // Get random points to sample
        std::vector<cv::Point> random_points = get_random_points(alpha, img.rows, img.cols);
        
        // Iterate over all random points and form a row vector of filter responses for each
        for (cv::Point const& pt: random_points)
        {
            std::vector<float> row(filter_responses.size());
            for (int i=0; i<filter_responses.size(); i++)
            {
                row[i] = (filter_responses[i].at<float>(pt.x,pt.y));
            }
            alpha_responses.push_back(row);
        }
    }
    // Form cv::Mat from vector of vectors
    cv::Mat alpha_resp_mat = cv::Mat(alpha_responses.size(), alpha_responses[0].size(), CV_32F);
    for (int row = 0; row <alpha_responses.size(); row++) {
        for (int col = 0; col < alpha_responses[row].size(); col++) {
            alpha_resp_mat.at<float>(row, col) = alpha_responses[row][col];
        }
    }
    //std::cout<<"Formed alpha_resp_mat :\n";
    //std::cout<<alpha_resp_mat<<std::endl;
    std::cout<<"Formed alpha_resp_mat\n";
    std::cout<<"Size of alpha_resp_mat = "<<alpha_resp_mat.rows<<","<<
        alpha_resp_mat.cols<<"\n";
    cv::Mat labels;
    cv::Mat centers;
    
    // TODO return the dict formed from this:
    cv::kmeans(alpha_resp_mat, K, labels,
                  cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0),
                  3, cv::KMEANS_PP_CENTERS, centers  );
    
    std::cout<<"Size of dict = "<<centers.rows<<", "<<centers.cols<<std::endl;
    std::cout<<"Dict : \n"<<centers;
    return centers;
}
