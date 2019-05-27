#include "SensorDevice.h"

void TUM::_LoadImages(const std::string &strAssociationFilename, const std::string &strSeqFilename)
{
    std::ifstream fAssociation;
    fAssociation.open(strAssociationFilename.c_str());
    while(!fAssociation.eof())
    {
        std::string s;
        getline(fAssociation,s);
        if(!s.empty())
        {
            std::stringstream ss;
            ss << s;
            double t;
            std::string sRGB, sD;
            ss >> t;
            timestamps.push_back(t);
            ss >> sRGB;
            ss >> t;
            ss >> sD;

            cv::Mat rgb, d, rgb_2;
            rgb = cv::imread("../res/SLAM/" + strSeqFilename + "/" + sRGB, CV_LOAD_IMAGE_UNCHANGED);
            cv::cvtColor(rgb, rgb_2, CV_BGR2RGB);
		    d =   cv::imread("../res/SLAM/" + strSeqFilename + "/" + sD,   CV_LOAD_IMAGE_UNCHANGED);
            imRGB.push_back(rgb_2);
            imD.push_back(d);    
        }
    }
    maxSeq = imRGB.size() - 1;
}

void TUM::Init()
{
    seq = 0;
    maxSeq = 0;
    _LoadImages("../res/SLAM/fr1_xyz.txt", "rgbd_dataset_freiburg1_xyz");
}

bool TUM::IsExists()
{
    DIR *dir = opendir("../res/SLAM");
    return dir != nullptr;
}

void TUM::GetImage(cv::Mat& left, cv::Mat& right)
{
    if(seq > maxSeq) seq = 0;

    left = imRGB[seq];
    right = imD[seq];
    seq++;
}