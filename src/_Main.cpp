#include <chrono>
#include <dali/dali.h>
#include <dali-toolkit/dali-toolkit.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Net.h"
#include "ORB_SLAM2/System.h"
#include "Background.h"
#include "SLAM.h"
#include "SensorDevice.h"
#include "NetThread.h"
#include "DebugScene.h"

const unsigned int      TICK_RATE           = 30;           // per second
const float             Focal_X             = 609.275f;     // realsense
const float             Focal_Y             = 609.119f;     // realsense
const int               SCREEN_WIDTH        = 640;
const int               SCREEN_HEIGHT       = 480;
const float             CAMERA_NEAR         = 0.001f;
const float             CAMERA_FAR          = 2000.0f;
const float             CAMERA_ASPECT       = (SCREEN_WIDTH * Focal_Y) / (SCREEN_HEIGHT * Focal_X);
const float             CAMERA_FOV          = atanf(SCREEN_HEIGHT / (2.0f * Focal_Y)) * 2;

int ARGC;
char **ARGV;
SLAM *slam;
SensorDevice *sensor;

class UbuntuServer : public Dali::ConnectionTracker
{
    private:
        // Core
        Dali::Application _application;
        cv::Mat _rgb, _depth;
        NetThread _netThread;
        Scene *_scene;
        Vec3 _cameraPos;
        Quat _cameraRot;
        Vec3 _planeNormal;
        Vec3 _planePos;
        bool _updateSlam;

        // Dali
        Dali::Stage _stage;
        Dali::CameraActor _camera;
        Dali::Timer _timer;
        Background _bg;

        // Plane
        int _planeInliers = 0;
        bool _updatePlane = true;

        // Time 
        std::chrono::time_point<std::chrono::high_resolution_clock> _currentTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> _oldTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> _initTime;

        int error;

    public:
        UbuntuServer(Dali::Application &application)
            : _application(application),
              _updateSlam(true),
              error(0)
        {
            _application.InitSignal().Connect(this, &UbuntuServer::__Create__);
        }

        int GetError() { return error; }

    private:
        void __Create__(Dali::Application &application)
        {
            application.GetWindow().SetSize(Window::WindowSize(SCREEN_WIDTH, SCREEN_HEIGHT));
            _stage = Dali::Stage::GetCurrent();
            _camera = _stage.GetRenderTaskList().GetTask(0).GetCameraActor();
            _InitCamera();
            _InitSignals();
            _bg.Create(_stage);
            _scene = new DebugScene(_stage, _camera, slam);
        }

        bool __Update__()
        {
            static int _updateCount = 0;
            double deltaTime = 0.0;
            double elapsedTime = 0.0;
            // First frame that Dali Actors are not initialized yet
            if (_updateCount < 1)
            {
                ++_updateCount;
            }
            // Second frame right after Dali Actors are initialized
            else if (_updateCount < 2)
            {
                ++_updateCount;
                _initTime = std::chrono::high_resolution_clock::now();
                _oldTime = _initTime;
                _currentTime = _initTime;

                _scene->Start();
            }
            // Real update routine starts from now on
            else
            {
                _UpdateTime(deltaTime, elapsedTime);
                _UpdateSlam(elapsedTime);
                _UpdatePlane();
                _bg.UpdateMat(_rgb);
                _UpdateScene(deltaTime);
            }
            
            return true;
        }

        void __OnKeyEvent__( const KeyEvent& event )
        {
            if( event.state == KeyEvent::Down )
            {
                // cout << event.keyCode << endl;
                if ( IsKey( event, Dali::DALI_KEY_ESCAPE ) || IsKey( event, Dali::DALI_KEY_BACK ) )
                {
                    _application.Quit();
                }

                // q pressed
                if(event.keyCode == 24)
                {
                    std::string ip = "127.0.0.1";
                    if (ARGC > 1) ip = std::string(ARGV[1]);
                    Net::BeginServer(ip, 9999);
                }

                // p pressed
                if (event.keyCode == 33)
                {
                    _updatePlane = not _updatePlane;
                }

                // spacebar pressed
                if (event.keyCode == 65)
                {
                    _updateSlam = not _updateSlam;
                }

                std::cout << event.keyCode << std::endl;
            }

            _scene->KeyEvent(event);
        }

        bool __OnTouch__(Dali::Actor actor, const Dali::TouchData &touch)
        {
            _scene->Touch(actor, touch);
            return true;
        }

        void _InitCamera()
        {
            // Camera default transform
            // Initial rotation is (0, 180, 0)
            _camera = _stage.GetRenderTaskList().GetTask(0).GetCameraActor();
            _stage.GetRenderTaskList().GetTask(0).SetCullMode( false );
            _camera.SetNearClippingPlane(CAMERA_NEAR);
            _camera.SetFarClippingPlane(CAMERA_FAR);
            cout << "camera fov, asepct : " << _camera.GetFieldOfView() << ", " << _camera.GetAspectRatio() << endl;
            _camera.SetAspectRatio(CAMERA_ASPECT);
            _camera.SetFieldOfView(CAMERA_FOV);
            _camera.SetAnchorPoint(AnchorPoint::CENTER);
            _camera.SetParentOrigin(ParentOrigin::CENTER);
        }

        void _InitSignals()
        {
            _stage.KeyEventSignal().Connect( this, &UbuntuServer::__OnKeyEvent__ );
            _timer = Dali::Timer::New(1000 / TICK_RATE);
            _timer.TickSignal().Connect(this, &UbuntuServer::__Update__);
            _timer.Start();
        }

        void _UpdateTime(double &deltaTime, double &elapsedTime)
        {
            // Get times measured in seconds
            _currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsedFromPrevFrame = _currentTime - _oldTime; 
            std::chrono::duration<double> elapsedFromStart = _currentTime - _initTime;
            deltaTime = elapsedFromPrevFrame.count();
            elapsedTime = elapsedFromStart.count();
            _oldTime = _currentTime;
        }

        void _UpdateSlam(double elapsedTime)
        {
            if (_updateSlam)
            {
                sensor->GetImage(_rgb, _depth);
                slam->Update(_rgb, _depth, elapsedTime, _cameraPos, _cameraRot);
                _netThread.UpdateCameraData(_camera.GetCurrentPosition(),
                                            _camera.GetCurrentOrientation(),
                                            _rgb, _depth);
            }
        }

        void _UpdatePlane()
        {
            Eigen::Vector4f eq;
            Eigen::Vector3f pos;
            int inlierCount;
            if (_updatePlane)
            {
                slam->GetPlane(eq, pos, inlierCount);
                if(inlierCount >= _planeInliers)
                {
                    _planeInliers = inlierCount;
                    _netThread.UpdatePlaneData(eq, pos);

                    _planeNormal = Vec3(Eigen::Vector3f(eq(0), eq(1), eq(2)));
                    _planeNormal.Normalize();
                    if (_planeNormal.y > 0) _planeNormal = -_planeNormal;
                    _planePos = Vec3(pos);
                }
            }
        }

        void _UpdateScene(double deltaTime)
        {
            _scene->Update(deltaTime, _planeNormal, _planePos, _cameraPos, _cameraRot);
        }
};

// main function
int DALI_EXPORT_API main(int argc, char **argv)
{
    ARGC = argc;
    ARGV = argv;
    Assets::Init();
    sensor = SensorDevice::Get(SensorDevice::Type::Realsense);
    if (sensor == nullptr) return -1;
    slam = new SLAM(*sensor);

    Dali::Application application = Dali::Application::New(&argc, &argv);
    UbuntuServer server(application);
    int error = server.GetError();

    if (not error) application.MainLoop();

    delete slam;
    delete sensor;
    return 0;
}