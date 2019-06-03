#ifndef __SLAM_H__
#define __SLAM_H__

#include <string>
#include <vector>
#include <dali-toolkit/dali-toolkit.h>
#include "ORB_SLAM2/System.h"
#include "ORB_SLAM2/Converter.h"
#include "Type.h"
#include "SensorDevice.h"

class SLAM
{
    private:
        ORB_SLAM2::System *_orb;
        SensorDevice &_sensor;

    public:
        SLAM(SensorDevice &sensor);
        ~SLAM();
        void Update(cv::Mat rgb, cv::Mat depth, double elapsedTime, Dali::CameraActor &camera);
        void GetPlane(Eigen::Vector4f &eq, Eigen::Vector3f &pos, int &inlierCount);
};

#endif