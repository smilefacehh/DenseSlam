#include "Mapping.h"


Mapping::Mapping(const std::shared_ptr<Camera>& camera, double resolution)
    : mpCamera(camera)
{
    mpColorOcTree = std::shared_ptr<octomap::ColorOcTree>(new octomap::ColorOcTree(resolution));
}

Mapping::~Mapping() {}

void Mapping::Process(Frame &frame)
{
    Process(frame.FrameId(), frame.RgbImage(), frame.DepthImage(), frame.Pose());
}

void Mapping::Process(int frameId, const cv::Mat& rgb, const cv::Mat& depth, const Eigen::Isometry3f& pose)
{
    assert(rgb.channels() == 3);

    // 相机内参
    const float& fx = mpCamera->Fx();
    const float& fxInv = 1 / fx;
    const float& fy = mpCamera->Fy();
    const float& fyInv = 1 / fy;
    const float& cx = mpCamera->Cx();
    const float& cy = mpCamera->Cy();
    const float& depthScale = mpCamera->DepthScale();
    const float& depthScaleInv = 1 / depthScale;

    float X, Y, Z;
    uchar B, G, R;
    const ushort* depthRow;
    const uchar* rgbRow;

    // 遍历像素点 
    for(int r = 0; r < depth.rows; r++)
    {
        depthRow = depth.ptr<ushort>(r);
        rgbRow = rgb.ptr<uchar>(r);
        for(int c = 0; c < depth.cols; c++)
        {
            if(depthRow[c] > 0)
            {
                // TUM的深度计算方式
                Z = depthRow[c] * depthScaleInv;
                X = (c - cx) * Z * fxInv;
                Y = (r - cy) * Z * fyInv;

                B = rgbRow[3 * c];
                G = rgbRow[3 * c + 1];
                R = rgbRow[3 * c + 2];


                Eigen::Vector4f worldPt = pose.matrix() * Eigen::Vector4f(X, Y, Z, 1);
                octomap::ColorOcTreeNode* node = mpColorOcTree->updateNode(octomap::point3d(worldPt(0), worldPt(1), worldPt(2)), true);
                if(node)
                {
                    node->setColor(R, G, B);
                    node->setValue(frameId);
                }
            }
        }
    }
}

void Mapping::MapAdjust(std::vector<Eigen::Isometry3f>& gtPoses)
{
    
    for(auto iter = mpColorOcTree->begin(); iter != mpColorOcTree->end(); iter++)
    {
        if(mpColorOcTree->isNodeOccupied(*iter))
        {
            octomap::point3d pt = iter.getCoordinate();
            // iter->setColor(octomap::ColorOcTreeNode::Color(0,0,0));

            Eigen::Vector4f newPt = gtPoses[int(iter->getValue())].matrix() * Eigen::Vector4f(pt.x(), pt.y(), pt.z(), 1);
            octomap::ColorOcTreeNode* newNode = mpColorOcTree->updateNode(octomap::point3d(newPt(0), newPt(1), newPt(2)), true);
            if(newNode)
            {
                octomap::ColorOcTreeNode::Color color = iter->getColor();
                newNode->setColor(color);
            }

            octomap::ColorOcTreeNode* node = mpColorOcTree->updateNode(pt, false);
            if(node)
            {
                octomap::ColorOcTreeNode::Color color;
                node->setColor(color);
            }
        }
    }
}

void Mapping::SaveOctoMap(const std::string& octoFile)
{
    // mpColorOcTree->updateInnerOccupancy();
    mpColorOcTree->prune();
    mpColorOcTree->write(octoFile);
}