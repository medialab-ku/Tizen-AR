#ifndef __TUM_H__
#define __TUM_H__

#include <opencv2/opencv.hpp>

class TUM
{
public:
    TUM();
    void LoadImages(const std::string &strAssociationFilename, const std::string &strSeqFilename);
    void GetImage(cv::Mat& left, cv::Mat& depth);

private:
    int seq;
    int maxSeq;
    
    std::vector<cv::Mat> imRGB;
    std::vector<cv::Mat> imD;
    std::vector<double> timestamps;
};

#endif