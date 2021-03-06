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
        Eigen::Vector4f _eq;
        Eigen::Vector3f _pos; 
        int _inlierCount;

    public:
        SLAM(SensorDevice &sensor);
        ~SLAM();
        void Update(cv::Mat rgb, cv::Mat depth, double elapsedTime, Vec3 &outCameraPos, Quat &outCameraRot);
        void GetPlane(Eigen::Vector4f &eq, Eigen::Vector3f &pos, int &inlierCount);
        ORB_SLAM2::System* GetSystem() { return _orb; }
};

#endif