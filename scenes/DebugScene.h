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
            _parent->SetPosition(-0.2, 0.05, 0);
            _parent->SetSize(0.02, 0.02, 0.02);
            AddActor(_parent);

            _child = new GraphicsActor(_stage, model);
            _child->SetPosition(0.2, 0.05, 0);
            _child->SetSize(0.02, 0.02, 0.02);
            AddActor(_child);
        }

        void KeyEvent(const Dali::KeyEvent &event)
        {
            static bool _state = true;

            if( event.state == KeyEvent::Down )
            {
                // s pressed
                if (event.keyCode == 39)
                {

                    if(_state)
                    {
                        _parent->GetActor().Add(_child->GetActor());
                        std::cout << _child->GetRealRotation().ToDali() << std::endl;
                    }
                    else
                    {
                        _parent->GetActor().Remove(_child->GetActor());
                        _stage.Add(_child->GetActor());
                        std::cout << _child->GetRealRotation().ToDali() << std::endl;
                    }

                    _state = not _state;
                }

                // enter pressed
                if(event.keyCode == 36)
                {
                    std::cout << _parent->GetPosition().ToDali() << std::endl;
                    std::cout << _parent->GetRealPosition().ToDali() << std::endl;
                }
            }
        }
};

#endif