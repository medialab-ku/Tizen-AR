#ifndef __NET_THREAD_H__
#define __NET_THREAD_H__

#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>
#include "Type.h"
#include "Net.h"

class NetThread
{
    private:
        std::thread _thread;
        bool _camInited;
        bool _planeInited;
        bool _run;
        Vec3 _camPos;
        Quat _camRot;
        cv::Mat _left, _right;
        Eigen::Vector4f _planeEq;
        Eigen::Vector3f _planePos;

    private:
        void __Procedure__();

    public:
        NetThread();
        ~NetThread();
        void UpdateCameraData(Vec3 camPos, Quat camRot, cv::Mat left, cv::Mat right);
        void UpdatePlaneData(Eigen::Vector4f eq, Eigen::Vector3f pos);
};

#endif