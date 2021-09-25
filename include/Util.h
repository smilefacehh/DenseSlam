#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <eigen3/Eigen/Geometry>

#include <opencv2/opencv.hpp>


class Util
{
public:
    /**
     * save trajectory
     * @param poses estimate pose
     * @param savedPath save path
     */
    static void SaveTrajectory(std::vector<Eigen::Isometry3f>& poses, const std::string& savedPath);

    /**
     * debug show image
     * @param img img
     */
    static void ImShow(const cv::Mat& img);

    /**
     * debug cout
     * @tparam T any data type
     * @param tip str
     * @param content content to cout
     */
    template<class T>
    static void Cout(const std::string& tip, T content)
    {
        std::cout << tip << ":" << content << std::endl;
    }

    static inline void Cout(const std::string& c) { std::cout << c << std::endl; }
    static inline void Cout(const int& c) { std::cout << c << std::endl; }
    static inline void Cout(const float& c) { std::cout << c << std::endl; }
    static inline void Cout(const double& c) { std::cout << c << std::endl; }
};