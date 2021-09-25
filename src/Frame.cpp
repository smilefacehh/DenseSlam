#include "Frame.h"

Frame::Frame()
{
    mFrameId = -1;
    mRgb = cv::Mat();
    mDepth = cv::Mat();
    mPose = Eigen::Isometry3f::Identity();
}

Frame::Frame(const int& frameId, const cv::Mat& rgb, const cv::Mat& depth, const Eigen::Isometry3f& pose)
    : mFrameId(frameId), mRgb(rgb), mDepth(depth), mPose(pose)
{}

Frame::~Frame() {}