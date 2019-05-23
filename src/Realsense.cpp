#include "Realsense.h"

Realsense::~Realsense()
{
    _pipe.stop();
}

cv::Mat Realsense::_frame_to_mat(const rs2::frame& f)
{
    using namespace cv;
    using namespace rs2;

    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    //std::cout << "width : " << w << ", height : " << h << std::endl;

    if (f.get_profile().format() == RS2_FORMAT_RGB8)
    {
        return Mat(cv::Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_BGR8)
    {
        auto r = Mat(cv::Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
        cvtColor(r, r, COLOR_BGR2RGB);
        return r;
    }
    else if (f.get_profile().format() == RS2_FORMAT_Z16)
    {
        return Mat(cv::Size(w, h), CV_16UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_Y8)
    {
        return Mat(cv::Size(w, h), CV_8UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }

    throw std::runtime_error("Frame format is not supported yet!");
}

cv::Mat Realsense::_depth_frame_to_meters(const rs2::pipeline& pipe, const rs2::depth_frame& f)
{
    using namespace cv;
    using namespace rs2;

    Mat dm = _frame_to_mat(f);
    dm.convertTo(dm, CV_64F);
    auto depth_scale = pipe.get_active_profile()
        .get_device()
        .first<depth_sensor>()
        .get_depth_scale();
    //std::cout << "scale: " << depth_scale << std::endl;
    dm = dm * depth_scale;
    return dm;
}

void Realsense::_get_field_of_view(const rs2::stream_profile& stream)
{
    // A sensor's stream (rs2::stream_profile) is in general a stream of data with no specific type.
    // For video streams (streams of images), the sensor that produces the data has a lens and thus has properties such
    //  as a focal point, distortion, and principal point.
    // To get these intrinsics parameters, we need to take a stream and first check if it is a video stream
    if (auto video_stream = stream.as<rs2::video_stream_profile>())
    {
        try
        {
            //If the stream is indeed a video stream, we can now simply call get_intrinsics()
            rs2_intrinsics intrinsics = video_stream.get_intrinsics();

            auto principal_point = std::make_pair(intrinsics.ppx, intrinsics.ppy);
            auto focal_length = std::make_pair(intrinsics.fx, intrinsics.fy);
            rs2_distortion model = intrinsics.model;

            std::cout << "Principal Point         : " << principal_point.first << ", " << principal_point.second << std::endl;
            std::cout << "Focal Length            : " << focal_length.first << ", " << focal_length.second << std::endl;
            std::cout << "Distortion Model        : " << model << std::endl;
            std::cout << "Distortion Coefficients : [" << intrinsics.coeffs[0] << "," << intrinsics.coeffs[1] << "," <<
                intrinsics.coeffs[2] << "," << intrinsics.coeffs[3] << "," << intrinsics.coeffs[4] << "]" << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to get intrinsics for the given stream. " << e.what() << std::endl;
        }
    }
    else if (auto motion_stream = stream.as<rs2::motion_stream_profile>())
    {
        try
        {
            //If the stream is indeed a motion stream, we can now simply call get_motion_intrinsics()
            rs2_motion_device_intrinsic intrinsics = motion_stream.get_motion_intrinsics();

            std::cout << " Scale X      cross axis      cross axis  Bias X \n";
            std::cout << " cross axis    Scale Y        cross axis  Bias Y  \n";
            std::cout << " cross axis    cross axis     Scale Z     Bias Z  \n";
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    std::cout << intrinsics.data[i][j] << "    ";
                }
                std::cout << "\n";
            }

            std::cout << "Variance of noise for X, Y, Z axis \n";
            for (int i = 0; i < 3; i++)
                std::cout << intrinsics.noise_variances[i] << " ";
            std::cout << "\n";

            std::cout << "Variance of bias for X, Y, Z axis \n";
            for (int i = 0; i < 3; i++)
                std::cout << intrinsics.bias_variances[i] << " ";
            std::cout << "\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to get intrinsics for the given stream. " << e.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "Given stream profile has no intrinsics data" << std::endl;
    }
}

void Realsense::Init()
{
    rs2::log_to_console(RS2_LOG_SEVERITY_ERROR);
    
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_RGB8, 60);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 60);
    rs2::pipeline_profile prof = _pipe.start(cfg);
    _get_field_of_view(prof.get_stream(RS2_STREAM_COLOR));
}

bool Realsense::IsExists()
{
    rs2::context ctx;
    rs2::device_list devices = ctx.query_devices();
    return devices.size() != 0;
}

void Realsense::GetImage(cv::Mat& left, cv::Mat& right)
{
    rs2::frameset data = _pipe.wait_for_frames();
    rs2::frame c = data.get_color_frame();
    rs2::frame d = data.get_depth_frame();
    left = _frame_to_mat(c);
    right = _depth_frame_to_meters(_pipe, d);
}