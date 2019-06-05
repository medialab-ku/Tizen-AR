#include "CameraFrameActor.h"

CameraFrameActor::CameraFrameActor(Dali::Stage &stage, Dali::CameraActor &actor)
    : FrameActor(stage, actor),
      _cameraActor(actor)
{
    
}