#pragma once

// pangolin
#include <pangolin/display/display.h>

// opencv
#include <opencv2/opencv.hpp>

class Viewer
{
public:
    Viewer();
    ~Viewer();

    void Process(const cv::Mat& rgb, const cv::Mat& depth); 
};