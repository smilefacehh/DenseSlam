#pragma once

#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Geometry>

class Frame
{
public:
    Frame();
    Frame(const int& frameId, const cv::Mat& rgb, const cv::Mat& depth, const Eigen::Isometry3f& pose);
    ~Frame();

    inline void Init(const int& frameId, const cv::Mat& rgb, const cv::Mat& depth, const Eigen::Isometry3f& pose)
    {
        mFrameId = frameId;
        mRgb = rgb;
        mDepth = depth;
        mPose = pose;
    }

    const int FrameId() { return mFrameId; }
    const cv::Mat& RgbImage() { return mRgb; }
    const cv::Mat& DepthImage() { return mDepth; }
    const Eigen::Isometry3f& Pose() { return mPose; }

    inline void SetFrameId(int frameId) { mFrameId = frameId; }
    inline void SetRgbImage(const cv::Mat& rgb) { mRgb = rgb; }
    inline void SetDepthImage(const cv::Mat& depth) { mDepth = depth; }
    inline void SetPose(const Eigen::Isometry3f& pose) { mPose = pose; }

private:
    int mFrameId;
    cv::Mat mRgb;
    cv::Mat mDepth;
    Eigen::Isometry3f mPose;
};
