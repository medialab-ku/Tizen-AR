#include <chrono>
#include "Net.h"
#include "ORB_SLAM2/System.h"
#include "Background.h"
#include "Slam.h"
#include "SensorDevice.h"

const unsigned int      TICK_RATE           = 30;           // per second
const float             Focal_X             = 609.275f;     // realsense
const float             Focal_Y             = 609.119f;     // realsense
const int               SCREEN_WIDTH        = 640;
const int               SCREEN_HEIGHT       = 480;
const float             CAMERA_NEAR         = 0.001f;
const float             CAMERA_FAR          = 2000.0f;
const float             CAMERA_ASPECT       = (SCREEN_WIDTH * Focal_Y) / (SCREEN_HEIGHT * Focal_X);
const float             CAMERA_FOV          = atanf(SCREEN_HEIGHT / (2.0f * Focal_Y)) * 2;

class UbuntuServer : public Dali::ConnectionTracker
{
    private:
        // Core
        Dali::Application _application;
        Slam _slam;
        Realsense _realsense;
        cv::Mat _rgb, _depth;
        SensorDevice *_sensor;

        // Dali
        Dali::Stage _stage;
        Dali::CameraActor _camera;
        Dali::Timer _timer;
        Dali::Toolkit::Control mUIControl;
        Dali::Layer mUILayer;
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
            : _application(application)
        {
            error = 0;

            _sensor = SensorDevice::Get(SensorDevice::Type::Realsense);
            if (_sensor == nullptr)
            {
                error = -1;
                return;
            }

            _application.InitSignal().Connect(this, &UbuntuServer::_Create);
        }

        int GetError() { return error; }

    private:
        void _Create(Dali::Application &application)
        {
            _stage = Dali::Stage::GetCurrent();
            _camera = _stage.GetRenderTaskList().GetTask(0).GetCameraActor();

            _stage.KeyEventSignal().Connect( this, &UbuntuServer::_OnKeyEvent );
            //_uiLayer.TouchSignal().Connect(this, &UbuntuServer::_OnTouch);
            _timer = Dali::Timer::New(1000 / TICK_RATE);
            _timer.TickSignal().Connect(this, &UbuntuServer::_Update);
            _timer.Start();

            _bg.Create(_stage);
        }

        bool _Update()
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
            }
            // Real update routine starts from now on
            else
            {
                // Get elapsed time measured in seconds
                _currentTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsedFromPrevFrame = _currentTime - _oldTime; 
                std::chrono::duration<double> elapsedFromStart = _currentTime - _initTime;
                deltaTime = elapsedFromPrevFrame.count();
                elapsedTime = elapsedFromStart.count();
                _oldTime = _currentTime;

                // Update SLAM
                _sensor->GetImage(_rgb, _depth);
                _slam.Update(_rgb, _depth, elapsedTime, _camera);
                
                // Plane detection
                Eigen::Vector4f planeEq;
                Eigen::Vector3f planePos;
                int inlierCount;
                if (_updatePlane)
                {
                    _slam.GetPlane(planeEq, planePos, inlierCount);
                    if(inlierCount >= _planeInliers)
                    {
                        // todo: send planeEq, planePos
                        _planeInliers = inlierCount;
                    }
                }
                
                _bg.UpdateMat(_rgb);
            }
            
            return true;
        }

        void _OnKeyEvent( const KeyEvent& event )
        {
            // if( event.state == KeyEvent::Down )
            // {
            //     // cout << event.keyCode << endl;
            //     if ( IsKey( event, Dali::DALI_KEY_ESCAPE ) || IsKey( event, Dali::DALI_KEY_BACK ) )
            //     {
            //         mApplication.Quit();
            //     }

            //     // q pressed
            //     if(event.keyCode == 24)
            //     {
            //         mUpdatePlane = !mUpdatePlane;
            //         cout << "plane detection mode changed to" << mUpdatePlane << endl;
            //     }
            // }
        }

        // bool _OnTouch(Dali::Actor actor, const Dali::TouchData &touch)
        // {
        //     return true;
        // }
};

// main function
int DALI_EXPORT_API main(int argc, char **argv)
{
    Assets::Init();
    Dali::Application application = Dali::Application::New(&argc, &argv);
    UbuntuServer server(application);
    int error = server.GetError();
    if (not error) application.MainLoop();
}