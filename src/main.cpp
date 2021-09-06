#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>

#include "Mapping.h"

int main(int argc, char** argv)
{
    const std::string rgbdDatasetFolder("/home/lutao/data/rgbd_dataset_freiburg1_xyz/");
    std::ifstream fin(rgbdDatasetFolder + "associate_rgb_depth_gt.txt");
    if(!fin.is_open())
    {
        std::cout << "cannot open dataset file" << std::endl;
        return -1;
    }

    std::vector<std::string> rgbFiles, depthFiles;
    std::vector<Eigen::Isometry3f> poses;

    long double ts;
    std::string rgbFile, depthFile;
    float tx, ty, tz, qx, qy, qz, qw;
    while(!fin.eof())
    {
        fin >> ts >> rgbFile >> ts >> depthFile >> ts >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        if(fin.fail()) break;

        rgbFiles.push_back(rgbFile);
        depthFiles.push_back(depthFile);

        Eigen::Quaternionf q = Eigen::Quaternionf(qw, qx, qy, qz);
        Eigen::Isometry3f pose = Eigen::Isometry3f::Identity();
        pose.linear() = q.matrix();
        pose.translation() = Eigen::Vector3f(tx, ty, tz);
        poses.push_back(pose);
    }
    
    std::shared_ptr<Camera> pCamera(new Camera(517.3, 525.0, 318.6, 255.3, 5000, 640, 480));
    std::shared_ptr<Mapping> pMapping(new Mapping(pCamera, 0.01));
    

    int num_frames = (int)rgbFiles.size();
    for(int i = 0; i < num_frames; i++)
    {
        cv::Mat rgb = cv::imread(rgbdDatasetFolder + rgbFiles[i]);
        cv::Mat depth = cv::imread(rgbdDatasetFolder + depthFiles[i], -1);

        auto t1 = std::chrono::steady_clock::now();
        pMapping->Process(rgb, depth, poses[i]);
        auto t2 = std::chrono::steady_clock::now();
        double t = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
        std::cout << "mapping frame:" << i << ", fps=" << 1000/t << std::endl;
    }

    pMapping->SaveOctoMap("../output/map.ot");

    return 0;
}