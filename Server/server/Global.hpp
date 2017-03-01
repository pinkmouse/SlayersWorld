#pragma once

#include "World/SqlManager.hpp"
#include "Entities/LevelManager.hpp"
#include "Entities/GroupManager.hpp"
#include "World/ConfigHandler.hpp"
#include "Map/MapManager.hpp"
#include "System/Spell/SpellManager.hpp"
#include "System/Quest/QuestManager.hpp"

extern ConfigHandler    *g_Config;
extern SqlManager       *g_SqlManager;
extern LevelManager     *g_LevelManager;
extern MapManager       *g_MapManager;
extern SpellManager     *g_SpellManager;
extern QuestManager     *g_QuestManager;
extern GroupManager     *g_GroupManager;
