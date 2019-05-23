#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <fstream>
#include <librealsense2/rs.hpp>
#include <exception>
#include <iostream>

/**
 * @author leejm
 * 
 * @breif
 * It provides scalability of camera devices.
 * If no deives connected, it feeds to us debug dataset.
 */
class SensorDevice
{
    public:
        enum class Type
        {
            Realsense
        };

    private:
        static SensorDevice *_tum;
        static SensorDevice *_rs;

    protected:
        virtual void Init() = 0;
        virtual bool IsExists() = 0;

    public:
        /**
         * @breif
         * Find available device and retrun its instance.
         * If prefered device is not exists, it selects another device available.
         * If no device is connected, it returns debug dataset.
         */
        static SensorDevice* Get(SensorDevice::Type prefered);

        /**
         * @breif
         * Get image from device.
         * 
         * @param left
         * rgb & rgbd: rgb channel image.
         * stereo: left camera image.
         * 
         * @param right
         * rgb: null
         * rgbd: depth channel image.
         * stereo: right camera image.
         */
        virtual void GetImage(cv::Mat &left, cv::Mat &right) = 0;
};

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