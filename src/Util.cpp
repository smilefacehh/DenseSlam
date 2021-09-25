#include "Util.h"

void Util::SaveTrajectory(std::vector<Eigen::Isometry3f>& poses, const std::string& savedPath)
{
    std::ofstream fo(savedPath);
    for(int i = 0; i < poses.size(); ++i)
    {
        double x = poses[i].translation()(0);
        double y = poses[i].translation()(1);
        double z = poses[i].translation()(2);
        Eigen::Quaternionf q(poses[i].rotation());
        double qx = q.x();
        double qy = q.y();
        double qz = q.z();
        double qw = q.w();
        fo << i + 1 << " " << x << " "  << y << " "  << z << " "  << qx << " "  << qy << " "  << qz << " "  << qw << std::endl;
    }
    fo.close();
}

void Util::ImShow(const cv::Mat& img)
{
    cv::imshow("img", img);
    cv::waitKey(-1);
    cv::destroyWindow("img");
}