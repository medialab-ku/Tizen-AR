#ifndef __DEBUG_SCENE_H__
#define __DEBUG_SCENE_H__

#include "Scene.h"

class DebugScene : public Scene
{
    private:
        FrameActor *_parent;
        FrameActor *_child;
        
    public:
        DebugScene(Dali::Stage &stage, Dali::CameraActor &camera) : Scene(stage, camera)
        {
            Dali::Shader shader;
            if (not Assets::GetShader("vertexColor.glsl", "fragmentColor.glsl", shader))
                std::cerr << "Failed to load shader." << std::endl;
            shader.RegisterProperty("uAlpha", 1.0f);
            PrimitiveCube model("wood.png", shader);
            _parent = new GraphicsActor(_stage, model);
            _parent->SetPosition(-0.1, 0.02, 0);
            _parent->SetSize(0.02, 0.02, 0.02);
            AddActor(_parent);

            _child = new GraphicsActor(_stage, model);
            _child->SetPosition(0.1, 0.02, 0);
            _child->SetSize(0.02, 0.02, 0.02);
            AddActor(_child);
        }

        void OnKeyEvent(const Dali::KeyEvent &event)
        {
            static bool _state = true;

            if( event.state == KeyEvent::Down )
            {
                // s pressed
                if (event.keyCode == 39)
                {
                    if(_state)
                    {
                        _parent->AddChild(_child);
                    }
                    else
                    {
                        _parent->RemoveChild(_child);
                    }

                    _state = not _state;
                }

                // a pressed
                if (event.keyCode == 38)
                {
                    _parent->RotateBy(Quat(Dali::Quaternion(Radian(0), Radian(Degree(90)), Radian(0))));
                }

                // d pressed
                if (event.keyCode == 40)
                {
                    _child->RotateBy(Quat(Dali::Quaternion(Radian(0), Radian(Degree(90)), Radian(0))));
                }

                // enter pressed
                if(event.keyCode == 36)
                {
                    std::cout << _parent->GetPosition().ToDali() << std::endl;
                    std::cout << _plane->GetPosition().ToDali() << std::endl;
                }
            }
        }
};

#endif