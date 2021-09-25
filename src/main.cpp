#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <random>

#include "Mapping.h"
#include "Util.h"
#include "DataLoader.h"

int main(int argc, char** argv)
{
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::normal_distribution<double> randX(0, 0.005);
    std::normal_distribution<double> randY(0, 0.005);
    std::normal_distribution<double> randZ(0, 0.005);
    std::normal_distribution<double> randRoll(0, 0.005);
    std::normal_distribution<double> randPitch(0, 0.005);
    std::normal_distribution<double> randYaw(0, 0.005);

    // std::shared_ptr<Camera> pCamera(new Camera(517.3, 525.0, 318.6, 255.3, 5000, 640, 480)); // TUM内参
    std::shared_ptr<Camera> pCamera(new Camera(481.20, 	-480, 319.5, 239.5, 5000, 640, 480)); // ICL-Nuim内参
    std::shared_ptr<Mapping> pMapping(new Mapping(pCamera, 0.05));
    DataLoader dataLoader(DatasetType::ICL_NUIM);

    int validCnt = -1;
    std::vector<Eigen::Isometry3f> gtPoses;
    std::vector<Eigen::Isometry3f> noisyPoses;
    while(dataLoader.CurrFrameId() < dataLoader.NumFrames() - 1)
    {
        Frame frame;
        InputHelper::OP op = dataLoader.GetAFrame(frame);

        if(op == InputHelper::OP::Pause || op == InputHelper::OP::StepJump)
        {
            if(op == InputHelper::OP::StepJump)
            {
                std::cout << "jump frame:" << dataLoader.CurrFrameId() << std::endl;
            }
            continue;
        }

        ++validCnt;
        gtPoses.push_back(frame.Pose());

        Eigen::Isometry3f noisyPose;

        if(validCnt == 0)
        {
            noisyPose = frame.Pose();
        }
        else
        {
            // random number
            double dx, dy, dz, roll, pitch, yaw;
            dx = randX(eng);
            dy = randY(eng);
            dz = randZ(eng);
            roll = randRoll(eng);
            pitch = randPitch(eng);
            yaw = randYaw(eng);

            // curr_noisyPose = prev_noisyPose * gt_delta * noise
            Eigen::Matrix4f deltaPoseGt = gtPoses[validCnt - 1].matrix().inverse() * gtPoses[validCnt].matrix();
            Eigen::Matrix4f deltaPoseNoisy = Eigen::Matrix4f::Zero();
            Eigen::Matrix3f noisyRotation;
            noisyRotation << cos(roll) * cos(yaw)  - cos(pitch) * sin(roll) * sin(yaw), - cos(pitch) * cos(yaw) * sin(roll) - cos(roll) * sin(yaw),   sin(roll) * sin(pitch),
                    cos(yaw)  * sin(roll) + cos(roll) * cos(pitch) * sin(yaw),   cos(roll) * cos(pitch) * cos(yaw) - sin(roll) * sin(yaw), - cos(roll) * sin(pitch),
                    sin(pitch)  * sin(yaw), cos(yaw) * sin(pitch), cos(pitch);
            Eigen::Vector3f noisyTranslation(dx, dy, dz);
            deltaPoseNoisy.block(0,0,3,3) = deltaPoseGt.block(0,0,3,3) * noisyRotation;
            deltaPoseNoisy.block(0,3,3,1) = deltaPoseGt.block(0,3,3,1) + noisyTranslation;
            deltaPoseNoisy(3,3) = 1;
            Eigen::Matrix4f poseMatrix = noisyPoses[validCnt - 1].matrix() * deltaPoseNoisy;
            noisyPose = Eigen::Isometry3f(poseMatrix);
        }
        noisyPoses.push_back(noisyPose);

//        Util::ImShow(frame.RgbImage());

        // mapping
        auto t1 = std::chrono::steady_clock::now();
        cv::Mat cropRgb(frame.RgbImage(), cv::Rect(0, 0, frame.RgbImage().cols, 20));
        cv::Mat cropDepth(frame.DepthImage(), cv::Rect(0, 0, frame.DepthImage().cols, 20));
        pMapping->Process(validCnt, cropRgb, cropDepth, noisyPose);
        auto t2 = std::chrono::steady_clock::now();
        double t = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
        std::cout << "mapping frame:" << validCnt << ", org:" << dataLoader.CurrFrameId() << ", fps=" << 1000/t << std::endl;
    }

    pMapping->SaveOctoMap("../output/map-noisy.ot");
    pMapping->MapAdjust(gtPoses);
    pMapping->SaveOctoMap("../output/map-adjust.ot");

    Util::SaveTrajectory(noisyPoses, "../output/noisyPose.txt");
    Util::Cout("done");

    return 0;
}