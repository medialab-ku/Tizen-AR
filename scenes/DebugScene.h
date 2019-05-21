#ifndef __DEBUG_SCENE__
#define __DEBUG_SCENE__

#include <iostream>
#include "Scene.h"
#include "PhysicsActor.h"
#include "Shader.h"
#include "PrimitiveModels.h"
#include "System.h"
#include "Particle.h"
#include "Assets.h"
#include "obj-loader.h"
#include "Model.h"

using namespace std;

class AppleModel : public Model
{
    public:
        AppleModel(std::string textureName, Dali::Shader &shader)
            : Model(shader)
        {
            AddTexture(0, textureName);
            ObjLoader obj;
            if (!Assets::GetObj("Apple.obj", obj))
            {
                std::cout << "fuck! cant read obj" << std::endl;
                return;
            }

            SetMass(0.1);
            
            Geometry geometry = obj.CreateGeometry(7, true);
		    SetGeometry(geometry);
        }

        btCollisionShape* CreateCollisionShape() override
        {
            btCollisionShape *shape = new btSphereShape(btScalar(0.4));
            return shape;
        }
};

class DebugScene : public Scene
{
    public:
        DebugScene(Dali::Stage &stage, Dali::CameraActor &camera, Dali::Layer &uiLayer, 
            btDiscreteDynamicsWorld *dynamicsWorld, FrameActor *plane, ORB_SLAM2::System *slam)
            : Scene(stage, camera, uiLayer, dynamicsWorld, plane),
              SLAM(slam),
              mLightDir(wVector3(0.2, 1, -0.3))
        {

        }

        void Init() override
        {
            InitUI();
            CreateDebug_MapPoints();
            // Debug_CreateContents();
            // CreateWater();
            // CreateFloor();
            // CreateBlocks();
            // CreateTorches();
            // CreateParticle();
            CreateApple(wVector3(1,1,1));
        }

        void OnUpdate(double deltaTime) override
        {
            // Scene::OnUpdate(deltaTime);
            // UpdateWater(deltaTime);
            // UpdateFloor();
            // UpdateBlocks();
            // UpdateTorches();
            if (SLAM->GetTrackingState() == 2) UpdateDebug_MapPoints();
        }

        void OnKeyEvent(const Dali::KeyEvent &event) override
        {
            if( event.state == KeyEvent::Down )
            {
                
            }
        }

        void OnTouch(Dali::Actor actor, const Dali::TouchData &touch) override
        {
            if (touch.GetPointCount() > 0)
            {
                Dali::Vector2 touchPos = touch.GetScreenPosition(0);
                std::cout << "Touch!!!!!!!!!!!!!!" <<  touchPos << std::endl;
                if (touch.GetState(0) == PointState::DOWN)
                {
                    ScreenToWorldResult stw = mCamera->ScreenToWorld(touchPos);
                    auto apple = CreateApple(stw.worldPos);
                    apple->ApplyForce(stw.direction * 20.0);
                }
            }
        }

    private:
        void InitUI()
        {
            mLabel = Dali::Toolkit::TextLabel::New();
            cout << "label init pos is : " << mLabel.GetCurrentWorldPosition() << endl; 
            cout << "label init rot is : " << mLabel.GetCurrentWorldOrientation() << endl;
            mLabel.SetParentOrigin( Dali::ParentOrigin::TOP_LEFT );
            mLabel.SetAnchorPoint( Dali::AnchorPoint::TOP_LEFT );
            // Is it necessary?
            mLabel.SetSize(Dali::Vector2( mStage.GetSize().y * (640.0f/480.0f), mStage.GetSize().y ) / 5.0f);
            // The size of font in points
            mLabel.SetProperty( Dali::Toolkit::TextLabel::Property::POINT_SIZE, 30.0f );
            // After title text has been entered, it can be retrieved from the TEXT property
            mLabel.SetProperty( Dali::Toolkit::TextLabel::Property::TEXT, "title" );
            // The text can be aligned horizontally to the beginning, end, or center of the available area
            mLabel.SetProperty( Dali::Toolkit::TextLabel::Property::HORIZONTAL_ALIGNMENT, "CENTER" );
            // The text can be aligned vertically to the beginning, end, or center of the available area
            mLabel.SetProperty( Dali::Toolkit::TextLabel::Property::VERTICAL_ALIGNMENT, "CENTER" );
            AddUI(mLabel);
            mLabel.SetPosition(Dali::Vector3(0, 0, 0));
        }

        void CreateDebug_MapPoints()
        {
            Dali::Shader cubeShader = LoadShaders("vertexPhong.glsl", "fragmentPhong.glsl");
            cubeShader.RegisterProperty("uLightPos", Dali::Vector3(1, -1, 1));
            cubeShader.RegisterProperty("uViewPos", Dali::Vector3(0, 0, 0));
            cubeShader.RegisterProperty("uLightColor", Dali::Vector3(1, 1, 1));

            PrimitiveTexturedCube cubeModel("wood.png", cubeShader);
            
            for(int i = 0; i < 2000; i++)
            {
                GraphicsActor* point = new GraphicsActor(mStage, cubeModel);
                point->SetPosition(0, 0, 3);
                point->SetSize(0.002, 0.002, 0.002);
                //AddActor(point);
                mMapPointActors.push_back(point);
            }
        }

        Dali::Vector3 PointFromMP(ORB_SLAM2::MapPoint* mp)
        {
            cv::Mat pos = mp->GetWorldPos();
            Dali::Vector3 point = Dali::Vector3(-pos.at<float>(0), pos.at<float>(1), pos.at<float>(2));
            return point;
        }

        void UpdateDebug_MapPoints()
        {
            std::vector<ORB_SLAM2::MapPoint*> mp = SLAM->GetMapPoints();
            for(int i = 0; i < 2000; i++)
            {
                if(i == mp.size()) break;
                Dali::Vector3 point = PointFromMP(mp[i]);
                mMapPointActors[i]->SetPosition(wVector3(point));
            }
        }

        void Debug_CreateContents()
        {
            Dali::Shader cubeShader = LoadShaders("vertexPhong.glsl", "fragmentPhong.glsl");
            cubeShader.RegisterProperty("uLightPos", Dali::Vector3(1, -1, 11));
            cubeShader.RegisterProperty("uViewPos", Dali::Vector3(0, 0, 0));
            cubeShader.RegisterProperty("uLightColor", Dali::Vector3(1, 1, 1));

            // Primitive models are defined in PrimitiveModels.h
            // Model need to receive its shader from the outside
            PrimitiveTexturedCube cubeModel("wood.png", cubeShader);

            // PhysicsActor behaves as dynamic rigidbody
            // and it inherits FrameActor which is basic concept of object
            auto cube1 = new PhysicsActor(mStage, cubeModel, mDynamicsWorld);
            cube1->SetName("Cube 1");
            cube1->SetPosition(-2, 0, 0);
            // Pass FrameActor so that it can be controlled and receive events
            AddActor(cube1);

            cubeModel.SetMass(1);
            auto cube2 = new PhysicsActor(mStage, cubeModel, mDynamicsWorld);
            cube2->SetName("Cube 2");
            cube2->SetPosition(-1, 3, 1);
            AddActor(cube2);

            // You can also remove FrameActor by using RemoveActor() method
            // And destructor of FrameActor will be called when removed
        }

        void CreateWater()
        {
            mWaterShader = LoadShaders("water_vert.glsl", "water_frag.glsl");
            mWaterShader.RegisterProperty("uLightDir", mLightDir.ToDali());
            mWaterShader.RegisterProperty("uViewPos", Dali::Vector3(0, 0, 0));
            mWaterShader.RegisterProperty("uLightColor", Dali::Vector3(1, 1, 1));
            mWaterShader.RegisterProperty("uAlpha", 0.8f);

            PrimitivePlane floorModel("water.png", mWaterShader);

            auto water = new GraphicsActor(mStage, floorModel);
            water->SetName("Water");
            water->SetPosition(0, 0.3, 0);
            water->SetSize( 1, 0.05, 1 );
            AddActor(water);
        }

        void UpdateWater(double deltaTime)
        {
            static float passedTime = 0.0f;
            mWaterShader.RegisterProperty("uTime", passedTime);
            mWaterShader.RegisterProperty("uViewPos", mCamera->GetPosition().ToDali());
            passedTime += deltaTime;
        }

        void CreateFloor()
        {
            mFloorShader = LoadShaders("vertexDiffuse.glsl", "fragmentDiffuse.glsl");
            mFloorShader.RegisterProperty("uLightDir", mLightDir.ToDali());
            mFloorShader.RegisterProperty("uViewPos", Dali::Vector3(0, 0, 0));
            Dali::Vector3 floorColor =  Dali::Vector3(239.0, 199.0, 166.0) / 255.0f;
            mFloorShader.RegisterProperty("uObjectColor", floorColor);
            
            PrimitiveCube floorModel( "wood.png", mFloorShader ); // not using texture. only for consturctor
            floorModel.SetMass(0);

            auto floorActor = new PhysicsActor(mStage, floorModel, mDynamicsWorld);
            floorActor->SetName("Floor");
            floorActor->SetPosition(0, 0.2, 0);
            floorActor->SetSize(0.3, 0.1, 0.3);
            AddActor(floorActor);
        }

        void UpdateFloor()
        {
            mFloorShader.RegisterProperty("uViewPos", mCamera->GetPosition().ToDali());
        }

        void CreateBlocks()
        {
            mBlockShader = LoadShaders("vertexPhong.glsl", "fragmentPhong.glsl");
            mBlockShader.RegisterProperty("uLightPos", mLightDir.ToDali());
            mBlockShader.RegisterProperty("uViewPos", Dali::Vector3(0, 0, 0));
            mBlockShader.RegisterProperty("uLightColor", Dali::Vector3(1, 1, 1));

            PrimitiveTexturedCube cubeModel("wood.png", mBlockShader);
            cubeModel.SetMass(0.4);

            auto cube1 = new PhysicsActor(mStage, cubeModel, mDynamicsWorld);
            cube1->SetName("Cube 1");
            cube1->SetPosition(-0.08, 0.3, 0.2);
            cube1->SetSize(0.02, 0.1, 0.06);
            // Pass FrameActor so that it can be controlled and receive events
            AddActor(cube1);

            auto cube2 = new PhysicsActor(mStage, cubeModel, mDynamicsWorld);
            cube2->SetName("Cube 2");
            cube2->SetPosition(0.08, 0.3, 0.2);
            cube2->SetSize(0.02, 0.1, 0.06);
            AddActor(cube2);

            auto cube3 = new PhysicsActor(mStage, cubeModel, mDynamicsWorld);
            cube3->SetName("Cube 3");
            cube3->SetPosition(0, 0.37, 0.3);
            cube3->SetSize(0.12, 0.02, 0.06);
            AddActor(cube3);
        }

        void UpdateBlocks()
        {
            mBlockShader.RegisterProperty("uViewPos", mCamera->GetPosition().ToDali());
        }

        void CreateParticle()
        {
            Dali::Shader particleShader = LoadShaders("vertexParticle.glsl", "fragmentParticle.glsl");
            ParticleEffect* particleEffect = new ParticleEffect(mStage, particleShader, 10);
            // particleEffect->LoadTexture(0, "../res/images/001.jpg");
            particleEffect->LoadTexture(0, "FlameRoundParticleSheet.png");
            particleEffect->SetTile(10, 5);
            particleEffect->SetPosition(-0.2, 0.15, 0.2);
            particleEffect->SetSize(0.2, 0.2, 0.2);
            FireEmitter* fe = new FireEmitter();
            particleEffect->SetParticleEmitter(fe);
            AddActor(particleEffect);
        }

        void CreateTorches()
        {
            mTorchShader = LoadShaders("vertexPhong.glsl", "fragmentPhong.glsl");
            mTorchShader.RegisterProperty("uLightPos", mLightDir.ToDali());
            mTorchShader.RegisterProperty("uViewPos", Dali::Vector3(0, 0, 0));
            mTorchShader.RegisterProperty("uLightColor", Dali::Vector3(1, 1, 1));
            ObjLoader obj;
            if (!Assets::GetObj("Campfire.obj", obj))
            {
                std::cout << "fuck! cant read obj" << std::endl;
                return;
            }
            PrimitiveObj objModel("Default_Palette.png", mTorchShader, obj);

            auto campfire = new GraphicsActor(mStage, objModel);
            //campfire->GetActor().GetRendererAt(0).SetProperty(Dali::Renderer::Property::FACE_CULLING_MODE, Dali::FaceCullingMode::FRONT);
            campfire->SetPosition(-0.2, 0.2, 0.2);
            campfire->SetSize(0.1, 0.1, 0.1);
            AddActor(campfire);
        }

        PhysicsActor* CreateApple(wVector3 pos)
        {
            mAppleShader = LoadShaders("vertexPhong.glsl", "fragmentPhong.glsl");
            mAppleShader.RegisterProperty("uLightPos", mLightDir.ToDali());
            mAppleShader.RegisterProperty("uViewPos", Dali::Vector3(0, 0, 0));
            mAppleShader.RegisterProperty("uLightColor", Dali::Vector3(1, 1, 1));
            ObjLoader obj;
            if (!Assets::GetObj("Apple.obj", obj))
            {
                std::cout << "fuck! cant read obj" << std::endl;
                return nullptr;
            }

            AppleModel appleModel("Default_Palette.png", mAppleShader);

            auto apple = new PhysicsActor(mStage, appleModel, mDynamicsWorld);
            apple->SetPosition(pos);
            apple->SetSize(0.1, 0.1, 0.1);
            AddActor(apple);

            return apple;
        }

        void UpdateTorches()
        {
            mTorchShader.RegisterProperty("uViewPos", mCamera->GetPosition().ToDali());
        }

        void UpdateApple()
        {
            mAppleShader.RegisterProperty("uViewPos", mCamera->GetPosition().ToDali());
        }

    private:
        ORB_SLAM2::System *SLAM;
        Dali::Toolkit::TextLabel mLabel;
        wVector3 mLightDir; // Directional Light

        // Debug
        vector<GraphicsActor*> mMapPointActors;

        // Water
        Dali::Shader mWaterShader;

        // Floor
        Dali::Shader mFloorShader;

        // Blocks
        Dali::Shader mBlockShader;

        // Objs
        Dali::Shader mTorchShader;
        Dali::Shader mAppleShader;
};

#endif