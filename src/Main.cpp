#include <chrono>
#include "net.h"
#include "ORB_SLAM2/System.h"
#include "Realsense.h"
#include "Background.h"

const double TICK_RATE = 30;

ORB_SLAM2::System* _slam;
Realsense _cam;
cv::Mat _rgb, _depth;

void Update(double delta)
{
    _cam.GetImage(_rgb, _depth);
    bg::UpdateBackgroundMat(_rgb);
}

int main()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> curtime;
    std::chrono::time_point<std::chrono::high_resolution_clock> oldtime;
    double delta;
    bool run = true;

    oldtime = curtime = std::chrono::high_resolution_clock::now();
    while (run)
    {
        // Get elapsed time measured in seconds
        curtime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = curtime - oldtime;
        delta = elapsed.count();

        if (delta >= 1.0 / TICK_RATE)
        {
            oldtime = curtime;
            Update(delta);
        }
    }

    return 0;
}