#pragma once

#include "World/SqlManager.hpp"
#include "Entities/LevelManager.hpp"
#include "World/ConfigHandler.hpp"
#include "Map\MapManager.hpp"

extern ConfigHandler    *g_Config;
extern SqlManager       *g_SqlManager;
extern LevelManager     *g_LevelManager;
extern MapManager       *g_MapManager;