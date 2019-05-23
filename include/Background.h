#ifndef __BG_H__
#define __BG_H__

#include <dali/dali.h>
#include <dali-toolkit/dali-toolkit.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "Assets.h"

/**
 * @author leejm
 * 
 * @breif
 * Static class for managing background
 */
class Background
{
    private:
        Dali::Actor _bgActor;
        Dali::TextureSet _bgTextureSet;

    private:
        Dali::Geometry _CreateGeometry();
        Dali::PixelData _CVMat2Pixel(cv::Mat& img);

    public:
        void Create(Dali::Stage &stage);

        /**
         * @breif
         * Update background by image file (for debug)
         */
        void UpdateImg(std::string path);

        /**
         * @breif
         * Update background by cv::Mat data which is passed from input device (realsense camera)
         */
        void UpdateMat(cv::Mat img);
};

#endif