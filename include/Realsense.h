#ifndef __REALSENSE_H__
#define __REALSENSE_H__

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <exception>
#include <iostream>
#include "SensorDevice.h"

class Realsense : public SensorDevice
{
    public:
        ~Realsense();
        void GetImage(cv::Mat &left, cv::Mat &right) override;

    protected:
        void Init() override;
        bool IsExists() override;

    private:
        rs2::pipeline _pipe;

        /** 
         * Convert rs2::frame to cv::Mat
         */ 
        cv::Mat _frame_to_mat(const rs2::frame& f);

        /**
         * Converts depth frame to a matrix of doubles with distances in meters
         */
        cv::Mat _depth_frame_to_meters(const rs2::pipeline& pipe, const rs2::depth_frame& f);

        void _get_field_of_view(const rs2::stream_profile& stream);
};

#endif