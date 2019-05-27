#include "SensorDevice.h"

SensorDevice* SensorDevice::_tum;
SensorDevice* SensorDevice::_rs;

SensorDevice* SensorDevice::Get(SensorDevice::Type prefered)
{
    static bool init = false;
    if (not init)
    {
        _tum = new TUM();
        _rs = new Realsense();
        init = true;
    }
    
    switch (prefered)
    {
        case SensorDevice::Type::Realsense:
            if (_rs->IsExists())
            {
                _rs->Init();
                return _rs;
            }
            break;
    }

    std::cerr << "The prefered device not exists. TUM dataset will be used instead." << std::endl;
    if (_tum->IsExists())
    {
        _tum->Init();
        return _tum;
    }
    else
    {
        std::cerr << "Tum dataset not found!! System will crash because there is no camera input." << std::endl;
        return nullptr;
    }
}