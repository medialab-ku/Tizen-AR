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
        switch (id)
        {
            case Net::ID_CAM:
            {
                //_SendCamData_raw();
                _SendCamData_resize();
                //_SendCamData_cvjpeg();
                //_SendCamData_cvpng();
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

void NetThread::_SendCamData_raw()
{
    // Prepare send buffer large enough
    static char *sendBuff = new char[3000000];

    size_t imsize = _left.total() * _left.elemSize();
    
    Net::Mat param_left(_left.data, imsize);
    Net::Vec3 param_pos(_camPos.x, _camPos.y, _camPos.z);
    Net::Vec4 param_rot(_camRot.x, _camRot.y, _camRot.z, _camRot.w);
    size_t encoded = Net::EncodeCameraData(sendBuff, param_left, param_pos, param_rot);
    size_t sent = Net::Send(Net::ID_CAM, sendBuff, encoded);
}

void NetThread::_SendCamData_resize()
{
    // Prepare send buffer large enough
    static char *sendBuff = new char[3000000];

    cv::resize(_left, _left, cv::Size(320, 240), 0, 0, CV_INTER_NN);
    size_t imsize = _left.total() * _left.elemSize();
    
    Net::Mat param_left(_left.data, imsize);
    Net::Vec3 param_pos(_camPos.x, _camPos.y, _camPos.z);
    Net::Vec4 param_rot(_camRot.x, _camRot.y, _camRot.z, _camRot.w);
    size_t encoded = Net::EncodeCameraData(sendBuff, param_left, param_pos, param_rot);
    size_t sent = Net::Send(Net::ID_CAM, sendBuff, encoded);
}

void NetThread::_SendCamData_cvjpeg()
{
    // Prepare send buffer large enough
    static char *sendBuff = new char[3000000];

    std::vector<unsigned char> encodeImg;
    std::vector<int> encodeParams;
    encodeParams.push_back(CV_IMWRITE_JPEG_QUALITY);
    encodeParams.push_back(50);
    cv::imencode(".jpg", _left.clone(), encodeImg, encodeParams);

    size_t imsize = encodeImg.size() * sizeof(char);
    std::cout << "image size : " << imsize << std::endl;

    Net::Mat param_left(&encodeImg[0], imsize);
    Net::Vec3 param_pos(_camPos.x, _camPos.y, _camPos.z);
    Net::Vec4 param_rot(_camRot.x, _camRot.y, _camRot.z, _camRot.w);
    size_t encoded = Net::EncodeCameraData(sendBuff, param_left, param_pos, param_rot);
    std::cout << "encoded : " << encoded << std::endl;
    size_t sent = Net::Send(Net::ID_CAM, sendBuff, encoded);
    encodeImg.clear();
    encodeParams.clear();
}

void NetThread::_SendCamData_cvpng()
{
    // Prepare send buffer large enough
    static char *sendBuff = new char[3000000];

    std::vector<unsigned char> encodeImg;
    std::vector<int> encodeParams;
    encodeParams.push_back(CV_IMWRITE_PNG_COMPRESSION);
    encodeParams.push_back(3);
    cv::imencode(".png", _left.clone(), encodeImg, encodeParams);

    size_t imsize = encodeImg.size() * sizeof(char);
    std::cout << "image size : " << imsize << std::endl;

    Net::Mat param_left(&encodeImg[0], imsize);
    Net::Vec3 param_pos(_camPos.x, _camPos.y, _camPos.z);
    Net::Vec4 param_rot(_camRot.x, _camRot.y, _camRot.z, _camRot.w);
    size_t encoded = Net::EncodeCameraData(sendBuff, param_left, param_pos, param_rot);
    std::cout << "encoded : " << encoded << std::endl;
    size_t sent = Net::Send(Net::ID_CAM, sendBuff, encoded);
    encodeImg.clear();
    encodeParams.clear();
}