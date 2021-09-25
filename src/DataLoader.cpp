#include "DataLoader.h"

DataLoader::DataLoader() : mFrameId(-1), mNumFrames(0) {}

DataLoader::DataLoader(const DatasetType& dataSetType) : mFrameId(-1), mNumFrames(0)
{
    if(dataSetType == DatasetType::TUM)
    {
        LoadDatasetTUM(Config::Instance().TUM_FolderPath, Config::Instance().TUM_AssociationFPath, mRgbFiles, mDepthFiles, mPoses);
    }
    else if(dataSetType == DatasetType::ICL_NUIM)
    {
        LoadDatasetICL_NUIM(Config::Instance().ICL_NUIM_FolderPath, Config::Instance().ICL_NUIM_AssociationFPath, Config::Instance().ICL_NUIM_GroundTruthFPath, mRgbFiles, mDepthFiles, mPoses);
    }
    else if(dataSetType == DatasetType::KITTI)
    {

    }

    mNumFrames = mPoses.size();
}

InputHelper::OP DataLoader::GetAFrame(Frame& frame)
{
    InputHelper::OP op = InputHelper::Instance().GetOp();
    switch (op)
    {
        case InputHelper::OP::Normal:
        case InputHelper::OP::StepForward:
        {
            ++mFrameId;
            if(mFrameId < mNumFrames)
            {
                cv::Mat rgb = cv::imread(mRgbFiles[mFrameId]);
                cv::Mat depth = cv::imread(mDepthFiles[mFrameId]);
                frame.Init(mFrameId, rgb, depth, mPoses[mFrameId]);
            }
            break;
        }
        case InputHelper::OP::Pause:
        {
            break;
        }
        case InputHelper::OP::StepBackward:
        {
            --mFrameId;
            if(mFrameId >= 0)
            {
                cv::Mat rgb = cv::imread(mRgbFiles[mFrameId]);
                cv::Mat depth = cv::imread(mDepthFiles[mFrameId]);
                frame.Init(mFrameId, rgb, depth, mPoses[mFrameId]);
            }
            else
            {
                mFrameId = 0;
            }
            break;
        }
        case InputHelper::OP::StepJump:
        {
            ++mFrameId;
            break;
        }
        default: break;
    }

    return op;
}

DataLoader::~DataLoader() {}

void DataLoader::LoadDatasetTUM(const std::string& tumFolderPath,
                    const std::string& tumAssociationFPath,
                    std::vector<std::string>& rgbFiles,
                    std::vector<std::string>& depthFiles,
                    std::vector<Eigen::Isometry3f>& poses)
{
    rgbFiles.clear();
    depthFiles.clear();
    poses.clear();

    std::ifstream fin(tumAssociationFPath);
    if(!fin.is_open())
    {
        std::cout << "cannot open dataset file:" << tumAssociationFPath << std::endl;
        return;
    }

    long double ts;
    std::string rgbFile, depthFile;
    float tx, ty, tz, qx, qy, qz, qw;
    while(!fin.eof())
    {
        fin >> ts >> rgbFile >> ts >> depthFile >> ts >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        if(fin.fail()) break;

        rgbFiles.push_back(tumFolderPath + rgbFile);
        depthFiles.push_back(tumFolderPath + depthFile);

        Eigen::Quaternionf q = Eigen::Quaternionf(qw, qx, qy, qz);
        Eigen::Isometry3f pose = Eigen::Isometry3f::Identity();
        pose.linear() = q.matrix();
        pose.translation() = Eigen::Vector3f(tx, ty, tz);
        poses.push_back(pose);
    }
    fin.close();
}

void DataLoader::LoadDatasetICL_NUIM(const std::string& iclNuimFolderPath,
                         const std::string& iclNuimAssociationFPath,
                         const std::string& iclNuimGroundTruthFPath,
                         std::vector<std::string>& rgbFiles,
                         std::vector<std::string>& depthFiles,
                         std::vector<Eigen::Isometry3f>& poses)
{
    rgbFiles.clear();
    depthFiles.clear();
    poses.clear();

    std::ifstream fin(iclNuimAssociationFPath);
    if(!fin.is_open())
    {
        std::cout << "cannot open dataset file:" << iclNuimAssociationFPath << std::endl;
        return;
    }

    int id;
    std::string rgbFile, depthFile;
    while(!fin.eof())
    {
        fin >> id >> depthFile >> id >> rgbFile;
        if(fin.fail()) break;

        rgbFiles.push_back(iclNuimFolderPath + rgbFile);
        depthFiles.push_back(iclNuimFolderPath + depthFile);
    }
    fin.close();

    fin.open(iclNuimGroundTruthFPath);
    if(!fin.is_open())
    {
        std::cout << "cannot open ground truth file:" << iclNuimGroundTruthFPath << std::endl;
        return;
    }
    float tx, ty, tz, qx, qy, qz, qw;
    while(!fin.eof())
    {
        fin >> id >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        if(fin.fail()) break;

        Eigen::Quaternionf q = Eigen::Quaternionf(qw, qx, qy, qz);
        Eigen::Isometry3f pose = Eigen::Isometry3f::Identity();
        pose.linear() = q.matrix();
        pose.translation() = Eigen::Vector3f(tx, ty, tz);
        poses.push_back(pose);
    }
    fin.close();
}