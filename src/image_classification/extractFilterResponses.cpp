#include "extractFilterResponses.h"
#include "../primitives/cuda_naive_convolve.h"

std::vector<cv::Mat> extractFilterResponses(cv::Mat I, filter_bank filterBank, int version)
{
    //std::cout<<"Entered extractFilterRespnonses\n";

    cv::Mat Im = I.clone();
    //std::vector<cv::Mat> channels(3);

    /*
    if (I.dims != 3){
        std::cout<<"num dims = "<<I.dims<<std::endl;
        std::cout<<"grayscale image\n";
        channels[0] = I.clone();
        channels[1] = I.clone();
        channels[2] = I.clone();
        cv::merge(channels, Im);
    }
    else {
    */
        //std::cout<<"num dims = "<<I.dims<<std::endl;
        //std::cout<<"color image\n";
        //Im = I.clone();
    //}

    cv::Mat im;
    cv::cvtColor(Im, im, cv::COLOR_BGR2Lab);
    //std::cout<<"After conversion\n";

    //cv::Mat im;
    //im1.convertTo(im, CV_32F);
        
    std::vector<cv::Mat> channels2(3);
    //std::cout<<"Converted color space\n";
    cv::split(im, channels2);
    //std::cout<<"After splitting\n";

    /*
    for(int i=0; i<3; i++)
    {
        std::cout<<"channels2[i] : \n"<<channels2[i]<<std::endl;
        cv::namedWindow("tmp", cv::WINDOW_AUTOSIZE);
        cv::imshow("tmp", channels[i]);
        cv::waitKey(0);
    }
    */
    
    std::vector<cv::Mat> filterResponses(3*filterBank.count);

    for (int i = 0; i < filterBank.count; i++){
        cv::Mat filter = filterBank.filters[i];
        //std::cout<<"Read in the filter\n";
        for (int j = 0; j < 3; j++){
            //std::cout<<"(i,j) = "<<i<<","<<j<<std::endl;
            //filterResponses[i*3 + j] = baseline_convolve(channels2[j], filter);
            cv::Mat im;
            switch(version){
                case 0:
                    //std::cout<<"Right case\n";     
                    cv::filter2D(channels2[j], im, CV_32F, filter);
                    //std::cout<<"After opencv filter\n";
                    break;
                case 5:
                    im = cuda_convolve(channels2[j], filter);
                    break;
                default:
                    cv::filter2D(channels2[j], im, CV_32F, filter);
                    break;
            }
            
            //std::cout<<"After inner loop\n";

            filterResponses[3*i+j] = im.clone();
            //std::cout<<"After setting mat in vector\n";
            
            
        }
    }
    
    //std::cout<<"Exiting extractFilterRespnonses\n";
    return filterResponses;


}
