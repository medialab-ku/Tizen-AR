#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include <dali/dali.h>
#include <dali-toolkit/dali-toolkit.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>

void CreateBackgroundImg(std::string path);
void UpdateBackgroundImg(std::string path);
void CreateBackgroundMat(cv::Mat img);
void UpdateBackgroundMat(cv::Mat img);

#endif