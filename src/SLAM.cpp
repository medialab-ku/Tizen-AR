#include "SLAM.h"

SLAM::SLAM(SensorDevice &sensor) : _sensor(sensor)
{
    const std::string orb = "../res/SLAM/ORB.bin";
    const std::string settings = sensor.GetConfigPath();
    _orb = new ORB_SLAM2::System(orb, settings, ORB_SLAM2::System::RGBD);
}

SLAM::~SLAM()
{
    _orb->SaveTrajectoryTUM("CameraTrajectory.txt");
    _orb->SaveMapPoints("MapPoints.txt");
    _orb->SaveDetectedPlane("Plane.txt");
    _orb->Shutdown();
    delete _orb;
}

void SLAM::Update(cv::Mat rgb, cv::Mat depth, double elapsedTime, Vec3 &outCameraPos, Quat &outCameraRot)
{
    try
    {
        cv::Mat tcw = _orb->TrackRGBD(rgb, depth, elapsedTime);
        cv::Mat rwc = tcw.rowRange(0,3).colRange(0,3).t();
        cv::Mat twc = -rwc*tcw.rowRange(0,3).col(3);
        vector<float> q = ORB_SLAM2::Converter::toQuaternion(rwc);
        Vec3 pos( Eigen::Vector3f(twc.at<float>(0), twc.at<float>(1), twc.at<float>(2)) );
        Quat rot( Eigen::Vector4f(q[0], q[1], q[2], q[3]) );
        rot = rot.Inverse(); // ORB-SLAM2 returns C->W. So we need inverse of that quaternion.

        outCameraPos = pos;
        outCameraRot = rot;
    }
    catch(const std::exception& e)
    {
        cout << "tracking failed!" << endl;
    }
}

void SLAM::GetPlane(Eigen::Vector4f &eq, Eigen::Vector3f &pos, int &inlierCount)
{
    _orb->GetPlaneDetector()->GetPlane(eq, pos, inlierCount);
}