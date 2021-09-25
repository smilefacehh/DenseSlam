#pragma once
#include <string>

class Config
{
public:
    static Config& Instance();

    std::string ICL_NUIM_FolderPath;
    std::string ICL_NUIM_AssociationFPath;
    std::string ICL_NUIM_GroundTruthFPath;

    std::string TUM_FolderPath;
    std::string TUM_AssociationFPath;
private:
    Config();
    void LoadConfig();
};