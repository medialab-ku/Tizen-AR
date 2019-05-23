#include "Scene.h"

Scene::Scene(Dali::Stage &stage, Dali::CameraActor &camera, Dali::Layer &uiLayer, FrameActor *plane)
    : _actorList(),
      _stage(stage),
      _uiLayer(uiLayer),
      _plane(plane)
{
    AddActor(plane);
    _camera = new CameraFrameActor(stage, camera);
    AddActor(_camera);
}

void
Scene::Update(double deltaTime)
{
    OnUpdate(deltaTime);
    for(auto itr = _actorList.begin(); itr != _actorList.end(); ++itr)
    {
        (*itr)->OnUpdate(deltaTime);
    }
}

void
Scene::Dispose()
{
    _actorList.clear();
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
Scene::AddUI(Dali::Actor &ui)
{
    _uiLayer.Add(ui);
}

void
Scene::OnStart()
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
Scene::OnUpdate(double deltaTime)
{
    for(auto itr = _actorList.begin(); itr != _actorList.end(); ++itr)
    {
        (*itr)->OnUpdate(deltaTime);
    }
}

void
Scene::OnKeyEvent(const Dali::KeyEvent &event)
{

}

void
Scene::OnTouch(Dali::Actor actor, const Dali::TouchData &touch)
{

}