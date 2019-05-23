#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <opencv2/opencv.hpp>

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

    private:
        static void Dispose();

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

#endif