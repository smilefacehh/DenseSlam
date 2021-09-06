#pragma once

class Camera
{
public:
    Camera();
    Camera(float fx, float fy, float cx, float cy, float depthScale, float width, float height);
    Camera(const Camera& rhs);
    Camera& operator=(const Camera& rhs);
    ~Camera();

    inline float Fx() { return mFx; }
    inline float Fy() { return mFy; }
    inline float Cx() { return mCx; }
    inline float Cy() { return mCy; }
    inline float DepthScale() { return mDepthScale; }
    inline float Width() { return mWidth; }
    inline float Height() { return mHeight; }


private:
    float mFx;
    float mFy;
    float mCx;
    float mCy;
    float mDepthScale;
    float mWidth;
    float mHeight;
};