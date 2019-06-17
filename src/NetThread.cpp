#include "NetThread.h"

NetThread::NetThread()
    : _camInited(false),
      _planeInited(false),
      _run(true),
      _thread(&NetThread::__Procedure__, this)
{
    
}

NetThread::~NetThread()
{
    _run = false;
    _thread.join();
}

void NetThread::UpdateCameraData(Vec3 camPos, Quat camRot, cv::Mat left, cv::Mat right)
{
    _camPos = camPos;
    _camRot = camRot;
    _left = left;
    _right = right;
    _camInited = true;
}

void NetThread::UpdatePlaneData(Eigen::Vector4f eq, Eigen::Vector3f pos)
{
    _planeEq = eq;
    _planePos = pos;
    _planeInited = true;
}

void NetThread::__Procedure__()
{
    while (_run)
    {
        //std::cout << "";
        std::this_thread::sleep_for(std::chrono::duration<double>(0));
        if (not _camInited || not _planeInited) continue;
        
        // wait until the client requests data 
        if (not Net::IsConnected()) continue;
        if (not Net::Receive()) continue;

        char id = Net::GetId();
        std::cout << "Packet Id: " << (int)id << std::endl;
        switch (id)
        {
            case Net::ID_CAM:
            {
                size_t imsize = _left.total() * _left.elemSize();
                size_t posesize = 7 * sizeof(float);
                size_t size = imsize + posesize;
                char *buf = new char[size];
                
                Net::Mat param_left(_left.data, imsize);
                Net::Vec3 param_pos(_camPos.x, _camPos.y, _camPos.z);
                Net::Vec4 param_rot(_camRot.x, _camRot.y, _camRot.z, _camRot.w);
                size_t encoded = Net::EncodeCameraData(buf, param_left, param_pos, param_rot);
                size_t sent = Net::Send(Net::ID_CAM, buf, encoded);
                delete[] buf;
            }
            break;

            case Net::ID_PLANE:
            {
                size_t size = 7 * sizeof(float);
                char *buf = new char[size];

                Net::Vec4 param_eq(_planeEq(0), _planeEq(1), _planeEq(2), _planeEq(3));
                Net::Vec3 param_pos(_planePos(0), _planePos(1), _planePos(2));
                size_t encoded = Net::EncodePlaneData(buf, param_eq, param_pos);
                size_t sent = Net::Send(Net::ID_PLANE, buf, encoded);
                delete[] buf;
            }
            break;
        }
    }
}