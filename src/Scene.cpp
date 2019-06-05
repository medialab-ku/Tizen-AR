#include "Scene.h"

PlaneActor::PlaneActor(Dali::Stage &stage, Model &model)
    : GraphicsActor(stage, model)
{
    SetName("Plane");
}

void
PlaneActor::OnSpaceUpdated(FrameActor *plane, Vec3 basisX, Vec3 basisY, Vec3 basisZ, Vec3 origin)
{
    GraphicsActor::OnSpaceUpdated(plane, basisX, basisY, basisZ, origin);
    auto pos = origin;
    auto rot = Quat(Dali::Quaternion(basisX.ToDali(), basisY.ToDali(), basisZ.ToDali()));
    SetPosition(pos);
    SetRotation(rot);
}

Scene::Scene(Dali::Stage &stage, Dali::CameraActor &camera)
    : _actorList(),
      _stage(stage),
      _origin(), _basisX(), _basisY(), _basisZ()
{
    // camera actor
    _camera = new CameraFrameActor(stage, camera);

    // plane actor
    Dali::Shader planeShader;
    if (not Assets::GetShader("vertexColor.glsl", "fragmentColor.glsl", planeShader))
        std::cerr << "Failed to load plane shader." << std::endl;
    planeShader.RegisterProperty("uAlpha", 0.3f);
    PrimitivePlane model("wood.png", planeShader);
    _plane = new PlaneActor(_stage, model);
    
    AddActor(_camera);
    AddActor(_plane);
}

void
Scene::OnStart()
{
    Start();
}

void
Scene::OnUpdate(double deltaTime, Vec3 planeNormal, Vec3 planeOrigin, Vec3 cameraPos, Quat cameraRot)
{
    _UpdatePlane(planeNormal, planeOrigin);
    _UpdateCamera(cameraPos, cameraRot);
    Update(deltaTime);
    for(auto itr = _actorList.begin(); itr != _actorList.end(); ++itr)
    {
        (*itr)->OnUpdate(deltaTime);
    }
}

void 
Scene::OnKeyEvent(const Dali::KeyEvent &event)
{
    KeyEvent(event);
}
        
void 
Scene::OnTouch(Dali::Actor actor, const Dali::TouchData &touch)
{
    Touch(actor, touch);
}

void 
Scene::_UpdatePlane(Vec3 planeNormal, Vec3 planeOrigin)
{
    // origin
    _origin = planeOrigin;

    // basis
    auto n = planeNormal.ToDali();
    auto z = Dali::Vector3::XAXIS.Cross(n);
    auto x = n.Cross(z);
    _basisX = x;
    _basisY = n;
    _basisZ = z;

    for(auto itr = _actorList.begin(); itr != _actorList.end(); ++itr)
    {
        (*itr)->OnSpaceUpdated(_plane, _basisX, _basisY, _basisZ, _origin);
    }
}

void
Scene::_UpdateCamera(Vec3 cameraPos, Quat cameraRot)
{
    _camera->SetPosition(cameraPos);
    _camera->SetRotation(cameraRot);
}

void 
Scene::AddActor(FrameActor *actor)
{
    bool found = (std::find(_actorList.begin(), _actorList.end(), actor) != _actorList.end());
    if (!found)
    {
        _actorList.push_back(actor);
        actor->OnSpaceUpdated(_plane, _basisX, _basisY, _basisZ, _origin);

        if (actor != _plane && actor != _camera)
        {
            _plane->AddChild(actor);
        }
    }
}

void 
Scene::RemoveActor(FrameActor *actor)
{
    bool found = (std::find(_actorList.begin(), _actorList.end(), actor) != _actorList.end());
    if (found)
    {
        _actorList.remove(actor);
        actor->OnSpaceUpdated(_plane, Vec3::right, Vec3::up, Vec3::forward, Vec3::zero);

        if (actor != _plane && actor != _camera)
        {
            _plane->RemoveChild(actor);
        }
        
        delete actor;
    }
}

void
Scene::Start()
{
    for(auto itr = _actorList.begin(); itr != _actorList.end(); ++itr)
    {
        (*itr)->OnStart();
    }
}

void
Scene::Update(double deltaTime)
{

}

void
Scene::KeyEvent(const Dali::KeyEvent &event)
{

}

void
Scene::Touch(Dali::Actor actor, const Dali::TouchData &touch)
{

}