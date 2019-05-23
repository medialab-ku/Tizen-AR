#ifndef __TUM_H__
#define __TUM_H__

#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "SensorDevice.h"

class Tum : public SensorDevice
{
    public:
        void GetImage(cv::Mat& left, cv::Mat& right) override;

    protected:
        void Init() override;
        bool IsExists() override;

    private:
        void _LoadImages(const std::string &strAssociationFilename, const std::string &strSeqFilename);

    private:
        int seq;
        int maxSeq;
        
        std::vector<cv::Mat> imRGB;
        std::vector<cv::Mat> imD;
        std::vector<double> timestamps;
};

#endif