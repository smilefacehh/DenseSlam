#pragma once

#include <opencv2/opencv.hpp>

#include "Frame.h"
#include "Config.h"
#include "InputHelper.h"
#include "Util.h"

enum DatasetType
{
    TUM,
    KITTI,
    ICL_NUIM
};

class DataLoader
{
public:
    DataLoader();

    /**
     * load data from file
     * @param dataSetType
     */
    DataLoader(const DatasetType& dataSetType);

    /**
     * get a frame
     */
    InputHelper::OP GetAFrame(Frame& frame);

    ~DataLoader();

    /**
     * current frame id
     */
    inline int CurrFrameId() { return mFrameId; }

    /**
     * total num of frames
     */
    inline int NumFrames() { return mNumFrames; }

private:
    /**
     * load tum frame
     * @param tumFolderPath data folder path
     * @param tumAssociationFPath association file, rgb|depth|pose
     * @param rgbFiles output rgb file list
     * @param depthFiles output depth file list
     * @param poses output pose list
     */
    void LoadDatasetTUM(const std::string& tumFolderPath,
                        const std::string& tumAssociationFPath,
                        std::vector<std::string>& rgbFiles,
                        std::vector<std::string>& depthFiles,
                        std::vector<Eigen::Isometry3f>& poses);

    /**
     * load icl-nuim frame
     * @param iclNuimFolderPath data folder path
     * @param iclNuimAssociationFPath association file, rgb|depth
     * @param iclNuimGroundTruthFPath pose file
     * @param rgbFiles output rgb file list
     * @param depthFiles output depth file list
     * @param poses output pose list
     */
    void LoadDatasetICL_NUIM(const std::string& iclNuimFolderPath,
                             const std::string& iclNuimAssociationFPath,
                             const std::string& iclNuimGroundTruthFPath,
                             std::vector<std::string>& rgbFiles,
                             std::vector<std::string>& depthFiles,
                             std::vector<Eigen::Isometry3f>& poses);

    std::vector<std::string> mRgbFiles;
    std::vector<std::string> mDepthFiles;
    std::vector<Eigen::Isometry3f> mPoses;

    int mFrameId;
    int mNumFrames;
};