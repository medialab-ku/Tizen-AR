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
    _plane = new GraphicsActor(_stage, model);
    _plane->SetName("Plane");
    _plane->SetSize(1, 0.05, 1);
    AddActor(_plane);

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
Scene::OnUpdate(double deltaTime, Vec3 planeNormal, Vec3 planeOrigin)
{
    _UpdatePlane(planeNormal, planeOrigin);
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

void Scene::_UpdatePlane(Vec3 planeNormal, Vec3 planeOrigin)
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
    Quat planeRotation(Dali::Quaternion(x, n, z));

    _plane->SetPosition(_origin);
    _plane->SetRotation(planeRotation);
}

void 
Scene::AddActor(FrameActor *actor)
{
    bool found = (std::find(_actorList.begin(), _actorList.end(), actor) != _actorList.end());
    if (!found)
    {
        _actorList.push_back(actor);
    }
}

void 
Scene::RemoveActor(FrameActor *actor)
{
    bool found = (std::find(_actorList.begin(), _actorList.end(), actor) != _actorList.end());
    if (found)
    {
        _actorList.remove(actor);
        delete actor;
    }
}

void
Scene::Start()
{
    // Put all actors on the plane
    for(auto itr = _actorList.begin(); itr != _actorList.end(); ++itr)
    {
        std::cout << (*itr)->GetPosition().ToDali() << std::endl;
        if ((*itr) == _plane) continue;

        auto pos = (*itr)->GetPosition();
        auto rot = (*itr)->GetRotation();
        (*itr)->SetPosition(pos + _plane->GetPosition());
        (*itr)->RotateBy(_plane->GetRotation());
        std::cout << (*itr)->GetName() << ": " << pos.ToDali() << std::endl;
    }

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