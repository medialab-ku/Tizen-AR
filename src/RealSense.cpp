#include "RealSense.h"

RealSense::RealSense()
{
    rs2::log_to_console(RS2_LOG_SEVERITY_ERROR);
    
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_RGB8, 60);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 60);
    rs2::pipeline_profile prof = pipe.start(cfg);
    get_field_of_view(prof.get_stream(RS2_STREAM_COLOR));
}

RealSense::~RealSense()
{
    pipe.stop();
}

void RealSense::GetImage(cv::Mat& left, cv::Mat& depth)
{
    rs2::frameset data = pipe.wait_for_frames();
    rs2::frame c = data.get_color_frame();
    rs2::frame d = data.get_depth_frame();
    left = frame_to_mat(c);
    depth = depth_frame_to_meters(pipe, d);
}