#include "Camera.h"


Camera::Camera() {}
Camera::~Camera() {}

Camera::Camera(float fx, float fy, float cx, float cy, float depthScale, float width, float height)
    : mFx(fx), mFy(fy), mCx(cx), mCy(cy), mDepthScale(depthScale), mWidth(width), mHeight(height)
{}

Camera::Camera(const Camera& rhs)
    : mFx(rhs.mFx), mFy(rhs.mFy), mCx(rhs.mCx), mCy(rhs.mCy), mDepthScale(rhs.mDepthScale), mWidth(rhs.mWidth), mHeight(rhs.mHeight)
{}

Camera& Camera::operator=(const Camera& rhs)
{
    mFx = rhs.mFx;
    mFy = rhs.mFy;
    mCx = rhs.mCx;
    mCy = rhs.mCy;
    mDepthScale = rhs.mDepthScale;
    mWidth = rhs.mWidth;
    mHeight = rhs.mHeight;

    return *this;
}

