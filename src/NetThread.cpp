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
        std::cout << id << std::endl;
        switch (id)
        {
            case Net::ID_CAM:
            {
                size_t size;
                char *buf;
                int head = 0;

                size_t imsize = _left.total() * _left.elemSize();
                size_t posesize = 7 * sizeof(float);
                size = imsize + posesize;
                buf = new char[size];

                // encode image
                std::memcpy(buf, _left.data, imsize);

                // encode camera pose
                std::memcpy(buf + imsize + head, (char*)(&(_camPos.x)), sizeof(float)); head += sizeof(float);
                std::memcpy(buf + imsize + head, (char*)(&(_camPos.y)), sizeof(float)); head += sizeof(float);
                std::memcpy(buf + imsize + head, (char*)(&(_camPos.z)), sizeof(float)); head += sizeof(float);
                std::memcpy(buf + imsize + head, (char*)(&(_camRot.x)), sizeof(float)); head += sizeof(float);
                std::memcpy(buf + imsize + head, (char*)(&(_camRot.y)), sizeof(float)); head += sizeof(float);
                std::memcpy(buf + imsize + head, (char*)(&(_camRot.z)), sizeof(float)); head += sizeof(float);
                std::memcpy(buf + imsize + head, (char*)(&(_camRot.w)), sizeof(float));

                size_t sent = Net::Send(Net::ID_CAM, buf, size);
                delete[] buf;
                std::cout << "Send Camera Data: " << sent << " bytes" << std::endl;
            }
            break;

            case Net::ID_PLANE:
            {

            }
            break;
        }
    }
}