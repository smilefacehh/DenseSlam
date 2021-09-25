#pragma once
#include <string>
#include <vector>
#include <memory>
// eigen
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
// opencv
#include <opencv2/opencv.hpp>
// octomap
#include <octomap/Pointcloud.h>
#include <octomap/ColorOcTree.h>

#include "Camera.h"
#include "Frame.h"

class Mapping
{
public:
    /**
     * 构造函数
     * @param camera 相机
     * @param resolution 建图精度
    */
    Mapping(const std::shared_ptr<Camera>& camera, double resolution);
    ~Mapping();

    /**
     * main procedure
     * @param frame a frame
     */
    void Process(Frame& frame);

    /**
     * @brief 主流程入口
     * @param frameId 帧序号
     * @param rgb 彩色图像
     * @param depth 深度图像
     * @param pose 位姿
    */
    void Process(int frameId, const cv::Mat& rgb, const cv::Mat& depth, const Eigen::Isometry3f& pose);

    /**
     * @brief 地图校正
     * @param gtPoses groundTruth位姿
    */
    void MapAdjust(std::vector<Eigen::Isometry3f>& gtPoses);

    /**
     * @brief 保存八叉树地图
     * @param octoFile 文件路径
    */
    void SaveOctoMap(const std::string& octoFile);

private:
    std::shared_ptr<Camera> mpCamera;
    std::shared_ptr<octomap::ColorOcTree> mpColorOcTree;
};