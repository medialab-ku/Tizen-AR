#ifndef __DEBUG_SCENE_H__
#define __DEBUG_SCENE_H__

#include "Scene.h"
#include "Type.h"
#include "ORB_SLAM2/System.h"
#include <vector>

class DebugScene : public Scene
{
    private:
        FrameActor *_parent;
        FrameActor *_child;

        ORB_SLAM2::System *_orb;
        std::vector<Dali::Vector3> mpsDali;
        Dali::Actor _debugger;

    private:
        Vec3 PointFromMP(ORB_SLAM2::MapPoint* mp)
        {
            cv::Mat pos = mp->GetWorldPos();
            Eigen::Vector3f point(pos.at<float>(0), pos.at<float>(1), pos.at<float>(2));
            Vec3 p(point);
            return p;
        }

        void CreateDebugger()
        {
            Dali::PropertyBuffer vertexBuffer = Dali::PropertyBuffer::New( Dali::Property::Map()
                                                                            .Add( "aPosition", Dali::Property::VECTOR3 ));
            //vertexBuffer.SetData( backgroundVertices, sizeof(backgroundVertices) / sizeof(Vertex) );

            Dali::Geometry geo = Dali::Geometry::New();
            geo.AddVertexBuffer( vertexBuffer );
            geo.SetType( Dali::Geometry::POINTS );

            Dali::Shader shader;
            if (not Assets::GetShader("vertexDebug.glsl", "fragmentDebug.glsl", shader))
            {
                std::cerr << "Debug::Create(): Failed to load shader" << std::endl;
                return;
            }

            auto renderer = Dali::Renderer::New(geo, shader);
            renderer.SetProperty( Dali::Renderer::Property::DEPTH_INDEX, 1.0f );

            _debugger = Dali::Actor::New();
            _debugger.SetName( "Background" );
            _debugger.SetAnchorPoint( Dali::AnchorPoint::CENTER );
            _debugger.SetParentOrigin( Dali::ParentOrigin::CENTER );
            _debugger.AddRenderer( renderer );
            _debugger.SetPosition( Dali::Vector3(0, 0, 0) );
            _debugger.SetOrientation( Dali::Quaternion::IDENTITY );
            _stage.Add(_debugger);
        }

        void UpdateDebugger()
        {
            if(_orb->GetTrackingState() != ORB_SLAM2::Tracking::eTrackingState::OK) return;
            std::vector<ORB_SLAM2::MapPoint*> mps = _orb->GetMapPoints();
            mpsDali.clear();
            for(auto mp : mps)
            {
                mpsDali.push_back( PointFromMP(mp).ToDali());
            }
            _debugger.GetRendererAt(0).GetGeometry().RemoveVertexBuffer(0);
            Dali::PropertyBuffer vertexBuffer = Dali::PropertyBuffer::New( Dali::Property::Map()
                                                                            .Add( "aPosition", Dali::Property::VECTOR3 ));
            vertexBuffer.SetData( &mpsDali[0], mpsDali.size());
            _debugger.GetRendererAt(0).GetGeometry().AddVertexBuffer( vertexBuffer );
        }
        
    public:
        DebugScene(Dali::Stage &stage, Dali::CameraActor &camera, SLAM* slam) : Scene(stage, camera)
        {
            _orb = slam->GetSystem();

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

            CreateDebugger();
        }

        void OnUpdate(double deltaTime) override
        {
            UpdateDebugger();
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