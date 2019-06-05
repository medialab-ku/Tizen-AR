#include "Scene.h"

Scene::Scene(Dali::Stage &stage, Dali::CameraActor &camera)
    : _actorList(),
      _stage(stage),
      _origin(), _basisX(), _basisY(), _basisZ()
{
    // plane actor
    Dali::Shader planeShader;
    if (not Assets::GetShader("vertexColor.glsl", "fragmentColor.glsl", planeShader))
        std::cerr << "Failed to load plane shader." << std::endl;
    planeShader.RegisterProperty("uAlpha", 0.3f);
    PrimitiveCube model("wood.png", planeShader);
    // _plane = new GraphicsActor(_stage, model);
    // _plane->SetName("Plane");
    // _plane->SetSize(1, 0.05, 1);
    // AddActor(_plane);

    // camera actor
    _camera = new CameraFrameActor(stage, camera);
    AddActor(_camera);
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
    auto z = Dali::Vector3::NEGATIVE_XAXIS.Cross(n);
    auto x = n.Cross(z);
    _basisX = x;
    _basisY = n;
    _basisZ = z;

    for(auto itr = _actorList.begin(); itr != _actorList.end(); ++itr)
    {
        (*itr)->OnSpaceUpdated(_basisX, _basisY, _basisZ, _origin);
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
        actor->OnSpaceUpdated(_basisX, _basisY, _basisZ, _origin);
        _actorList.push_back(actor);
    }
}

void 
Scene::RemoveActor(FrameActor *actor)
{
    bool found = (std::find(_actorList.begin(), _actorList.end(), actor) != _actorList.end());
    if (found)
    {
        actor->OnSpaceUpdated(Vec3::right, Vec3::up, Vec3::forward, Vec3::zero);
        _actorList.remove(actor);
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