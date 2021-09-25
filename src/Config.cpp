#include "Config.h"

Config& Config::Instance()
{
    static Config cf;
    return cf;
}

Config::Config()
{
    LoadConfig();
}

void Config::LoadConfig()
{
    ICL_NUIM_FolderPath = "/home/lutao/narwal/dataset/ICL-Nuim/living_room_traj0_frei_png/";
    ICL_NUIM_AssociationFPath = "/home/lutao/narwal/dataset/ICL-Nuim/living_room_traj0_frei_png/associations.txt";
    ICL_NUIM_GroundTruthFPath = "/home/lutao/narwal/dataset/ICL-Nuim/living_room_traj0_frei_png/livingRoom0.gt.freiburg";

    TUM_FolderPath = "";
    TUM_AssociationFPath = "";
}