#include "SqlManager.hpp"
#include "../Define.hpp"
#include "../Entities/Creature.hpp"
#include "../Global.hpp"
#include <cstdlib>

SqlManager::SqlManager()
{
	mysql_init(&m_MysqlCharacters);
    mysql_init(&m_MysqlWorld);
}


SqlManager::~SqlManager()
{
    mysql_close(&m_MysqlCharacters);
    mysql_close(&m_MysqlWorld);
}

bool SqlManager::InitializeCharacters(std::string p_Host, std::string p_User, std::string p_PassWord, std::string p_DB, std::string p_Port)
{
	if (!mysql_real_connect(&m_MysqlCharacters, p_Host.c_str(), p_User.c_str(), p_PassWord.c_str(), p_DB.c_str(), std::stoi(p_Port), NULL, 0))
		return false;
    my_bool l_AutoRecoonect = true; ///< To auto reconnect if it get disconnected
    mysql_options(&m_MysqlCharacters, MYSQL_OPT_RECONNECT, &l_AutoRecoonect);
	return true;
}

bool SqlManager::InitializeWorld(std::string p_Host, std::string p_User, std::string p_PassWord, std::string p_DB, std::string p_Port)
{
    if (!mysql_real_connect(&m_MysqlWorld, p_Host.c_str(), p_User.c_str(), p_PassWord.c_str(), p_DB.c_str(), std::stoi(p_Port), NULL, 0))
        return false;
    my_bool l_AutoRecoonect = true; ///< To auto reconnect if it get disconnected
    mysql_options(&m_MysqlWorld, MYSQL_OPT_RECONNECT, &l_AutoRecoonect);
    return true;
}

void SqlManager::AddNewAccount(std::string p_Login, std::string p_Password)
{
    std::string l_Query = "INSERT INTO `login` (login, password) VALUES ('" + p_Login + "', MD5('" + p_Password + "'));";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

bool SqlManager::IsExistingAccound(std::string p_Login)
{
    std::string l_Query = "SELECT `id` FROM `login` WHERE `login` = '" + p_Login + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
    bool l_Existing = false;
    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;

    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
        l_Existing = true;

    mysql_free_result(l_Result);
    return l_Existing;
}

int32 SqlManager::GetIDLogin(std::string p_Login, std::string p_Password)
{
    std::string l_Query = "SELECT `id` FROM `login` WHERE `login` = '" + p_Login + "' AND `password` = MD5('" + p_Password + "')";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
    int32 l_ID = -1;
    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;

    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
        l_ID = atoi(l_Row[0]);

    mysql_free_result(l_Result);
    return l_ID;
}

int32 SqlManager::GetIDCharacter(uint32 p_AccountID)
{
    std::string l_Query = "SELECT `characterID` FROM `characters` WHERE `accountID` = '" + std::to_string(p_AccountID) + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    int32 l_ID = -1;
    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;

    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
        l_ID = atoi(l_Row[0]);

    mysql_free_result(l_Result);
    return l_ID;
}

void SqlManager::AddConnectionLogin(uint32 p_AccountID)
{
    std::string l_Query = "insert into `connection_logs` (`accountID`) values('" + std::to_string(p_AccountID) + "');";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
    l_Query = "UPDATE `characters` SET `lastConnection` = NOW() WHERE accountID = " + std::to_string(p_AccountID) + ";";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

std::string SqlManager::GetLoginName(uint32 p_AccountID)
{
    std::string l_Query = "SELECT `login` FROM `login` WHERE `id` = '" + std::to_string(p_AccountID) + "';";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    std::string l_Name = "";
    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;

    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
        l_Name = std::string(l_Row[0]);

    mysql_free_result(l_Result);
    return l_Name;
}

void SqlManager::AddNewPlayer(uint32 p_AccountID)
{
    std::string l_Query = "insert into `characters` (`accountID`, `name`, `skinID`, `level`, `health`, `alignment`, `mapID`, `posX`, `posY`, `orientation`, `xp`) values('" + std::to_string(p_AccountID) + "', '" + GetLoginName(p_AccountID) + "','0','1','100','0','0','256','296','0','0');";

    mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

Player* SqlManager::GetNewPlayer(uint32 p_AccountID)
{
    std::string l_Query = "SELECT characterID, name, level, health, alignment, skinID, mapID, posX, posY, orientation, xp FROM characters WHERE accountID = '" + std::to_string(p_AccountID) + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    uint32 l_ID = 0;
    std::string l_Name = "";
    uint8 l_Lvl = 0;
    uint8 l_Health = 0;
    uint8 l_Alignment = 0;
    uint8 l_SkinID = 0;
    uint16 l_MapID = 0;
    uint32 l_PosX = 0;
    uint32 l_PosY = 0;
    uint8 l_Orientation = 0;
    uint32 l_Xp = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    Player* l_Player = nullptr;

    bool l_Exist = false;
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Exist = true;
        l_ID = atoi(l_Row[0]);
        l_Name = std::string(l_Row[1]);
        l_Lvl = atoi(l_Row[2]);
        l_Health = atoi(l_Row[3]);
        l_Alignment = atoi(l_Row[4]);
        l_SkinID = atoi(l_Row[5]);
        l_MapID = atoi(l_Row[6]);
        l_PosX = atoi(l_Row[7]);
        l_PosY = atoi(l_Row[8]);
        l_Orientation = atoi(l_Row[9]);
        l_Xp = atoi(l_Row[10]);
    }
    mysql_free_result(l_Result);

    if (!l_Exist)
    {
        printf("Add New Player %d", p_AccountID);
        AddNewPlayer(p_AccountID);
        return GetNewPlayer(p_AccountID);
    }

    eAccessType l_PlayerAccessType = GetAccessType(p_AccountID);
    l_Player = new Player(l_ID, l_Name, l_Lvl, l_Health, l_SkinID, l_MapID, l_PosX, l_PosY, (Orientation)l_Orientation, l_Xp, l_PlayerAccessType);
    l_Player->SetAlignment(l_Alignment);
	l_Player->SetPointsSet(GetPointsSetForPlayer(l_ID));
    l_Player->SetRespawnPosition(GetRespawnPositionForPlayer(l_ID));

    return l_Player;
}

void SqlManager::AddNewRespawnPositionForPlayer(uint32 p_PlayerID)
{
    std::string l_Query = "insert into `characters_respawn` (`characterID`, `posX`, `posY`, `mapID`, `orientation`) values('" + std::to_string(p_PlayerID) + "', '256', '296', '0', '0');";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

WorldPosition SqlManager::GetRespawnPositionForPlayer(uint32 p_PlayerID)
{
    std::string l_Query = "SELECT posX, posY, mapID, orientation FROM characters_respawn WHERE characterID = '" + std::to_string(p_PlayerID) + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
 
    uint32 l_PosX = 0;
    uint32 l_PosY = 0;
    uint16 l_MapID = 0;
    uint8 l_Orientation = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    bool l_Exist = false;
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Exist = true;
        l_PosX = atoi(l_Row[0]);
        l_PosY = atoi(l_Row[1]);
        l_MapID = atoi(l_Row[2]);
        l_Orientation = atoi(l_Row[3]);
    }
    mysql_free_result(l_Result);

    if (!l_Exist)
    {
        AddNewRespawnPositionForPlayer(p_PlayerID);
        return GetRespawnPositionForPlayer(p_PlayerID);
    }

    WorldPosition l_Position(l_PosX, l_PosY, l_MapID, (Orientation)l_Orientation);
    return l_Position;
}

void SqlManager::AddNewPointsSetForPlayer(uint32 p_PlayerID)
{
	std::string l_Query = "INSERT INTO `characters_point` (`characterID`, `free_point`, `force`, `stamina`, `dexterity`) values('" + std::to_string(p_PlayerID) + "', '0', '0', '0', '0');";
	mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

PointsSet SqlManager::GetPointsSetForPlayer(uint32 p_PlayerID)
{
	std::string l_Query = "SELECT `free_point`, `force`, `stamina`, `dexterity` FROM characters_point WHERE characterID = '" + std::to_string(p_PlayerID) + "'";
	mysql_query(&m_MysqlCharacters, l_Query.c_str());

	uint16 l_FreePoints = 0;
	uint16 l_Force = 0;
	uint16 l_Stamina = 0;
	uint16 l_Dexterity = 0;

	MYSQL_RES *l_Result = NULL;
	MYSQL_ROW l_Row;
	bool l_Exist = false;
	l_Result = mysql_use_result(&m_MysqlCharacters);
	while ((l_Row = mysql_fetch_row(l_Result)))
	{
		l_Exist = true;
		l_FreePoints = atoi(l_Row[0]);
		l_Force = atoi(l_Row[1]);
		l_Stamina = atoi(l_Row[2]);
		l_Dexterity = atoi(l_Row[3]);
	}
	mysql_free_result(l_Result);

	if (!l_Exist)
	{
		AddNewPointsSetForPlayer(p_PlayerID);
		return GetPointsSetForPlayer(p_PlayerID);
	}

	PointsSet l_PointsSet(l_FreePoints, l_Force, l_Stamina, l_Dexterity);
	return l_PointsSet;
}

void SqlManager::SavePlayer(Player const* p_Player)
{
	std::string l_Query = "UPDATE characters SET `posX` = '" + std::to_string(p_Player->GetPosX()) + "', `posY` = '" + std::to_string(p_Player->GetPosY()) + "', `mapID` = '" + std::to_string(p_Player->GetMapID()) + "', `orientation` = '" + std::to_string(p_Player->GetOrientation()) + "', `health` = '" + std::to_string(p_Player->GetHealth()) + "', `alignment` = '" + std::to_string(p_Player->GetAlignment()) + "', `xp` = '" + std::to_string(p_Player->GetXp()) + "', `level` = '" + std::to_string(p_Player->GetLevel()) + "', skinID = '" + std::to_string(p_Player->GetSkinID()) + "' WHERE characterID = '" + std::to_string(p_Player->GetID()) + "';";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
	UpdatePointsSet(p_Player);
}

void SqlManager::UpdatePointsSet(Player const* p_Player)
{
	std::string l_Query = "UPDATE `characters_point` SET `free_point` = '" + std::to_string(p_Player->GetPointsSet().m_FreePoints) + "', `force` = '" + std::to_string(p_Player->GetPointsSet().m_Force) + "', `stamina` = '" + std::to_string(p_Player->GetPointsSet().m_Stamina) + "', `dexterity` = '" + std::to_string(p_Player->GetPointsSet().m_Dexterity) + "' WHERE characterID = '" + std::to_string(p_Player->GetID()) + "';";
	mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

CreatureTemplate SqlManager::GetCreatureTemplate(uint16 p_Entry)
{
    std::string l_Query = "SELECT `skinID`, `name`, `level`, `force`, `stamina`, `dexterity`, `xp`, `state`, `maxRay`, `maxVision`,`movingTimeMin`, `movingTimeMax`, `stopTimeMin`, `stopTimeMax`, `respawnTime`, `rank`, `aiType`, `faction` FROM creature_template WHERE `entry` = '" + std::to_string(p_Entry) + "'";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint8 l_SkinID = 0;
    std::string l_Name = "";
    uint8 l_Lvl = 0;
    uint8 l_Force = 0;
    uint8 l_Stamina = 0;
    uint8 l_Dexterity = 0;
    uint8 l_Xp = 0;
    uint8 l_State = 0;
    uint16 l_MaxRay = 0;
    uint16 l_MaxVision = 0;
    float l_MovingTimeMin = 0.0f;
    float l_MovingTimeMax = 0.0f;
    float l_StopTimeMin = 0.0f;
    float l_StopTimeMax = 0.0f;
    uint16 l_RespawnTime = 0;
    uint8 l_Rank = 0;
    uint8 l_AiType = 0;
    uint8 l_Faction = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_SkinID = atoi(l_Row[0]);
        l_Name = std::string(l_Row[1]);
        l_Lvl = atoi(l_Row[2]);
        l_Force = atoi(l_Row[3]);
        l_Stamina = atoi(l_Row[4]);
        l_Dexterity = atoi(l_Row[5]);
        l_Xp = atoi(l_Row[6]);
        l_State = atoi(l_Row[7]);
        l_MaxRay = atoi(l_Row[8]);
        l_MaxVision = atoi(l_Row[9]);
        l_RespawnTime = atoi(l_Row[10]);
        l_MovingTimeMin = (float)atof(l_Row[11]);
        l_MovingTimeMax = (float)atof(l_Row[12]);
        l_StopTimeMin = (float)atof(l_Row[13]);
        l_StopTimeMax = (float)atof(l_Row[14]);
        l_RespawnTime = atoi(l_Row[15]);
        l_Rank = atoi(l_Row[16]);
        l_AiType = atoi(l_Row[17]);
        l_Faction = atoi(l_Row[18]);
        return CreatureTemplate(p_Entry, l_SkinID, l_Name, l_Lvl, l_Force, l_Stamina, l_Dexterity, l_Xp, l_State, l_MaxRay, l_MaxVision, l_MovingTimeMin, l_MovingTimeMax, l_StopTimeMin, l_StopTimeMax, l_RespawnTime, l_Rank, l_AiType, (eFactionType)l_Faction);
    }
    mysql_free_result(l_Result);

    return CreatureTemplate();
}

uint16 SqlManager::AddNewCreature(uint16 p_Map, uint16 p_Entry, uint32 p_PosX, uint32 p_PosY)
{
    std::string l_Query = "insert  into `creature`(`mapID`,`entry`,`posX`,`posY`) values('" + std::to_string(p_Map) + "', '" + std::to_string(p_Entry) + "', '" + std::to_string(p_PosX) + "', '" + std::to_string(p_PosY) + "')";
    mysql_query(&m_MysqlWorld, l_Query.c_str());
    l_Query = "SELECT MAX(`id`) FROM `creature`";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_Id = 0;
    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
    }
    mysql_free_result(l_Result);
    return l_Id;
}

bool SqlManager::InitializeCreatureTemplate(UnitManager* p_CreatureManager)
{
    std::string l_Query = "SELECT `entry`, `skinID`, `name`, `level`, `force`, `stamina`, `dexterity`, `xp`, `state`, `maxRay`, `maxVision`, `movingTimeMin`, `movingTimeMax`, `stopTimeMin`, `stopTimeMax`, `respawnTime`, `rank`, `aiType`, `faction` FROM creature_template";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint32 l_Entry = 0;
    uint8 l_SkinID = 0;
    std::string l_Name = "";
    uint8 l_Lvl = 0;
    uint8 l_Force = 0;
    uint8 l_Stamina = 0;
    uint8 l_Dexterity = 0;
    uint8 l_Xp = 0;
    uint8 l_State = 0;
    uint16 l_MaxRay = 0;
    uint16 l_MaxVision = 0;
    float l_MovingTimeMin = 0.0f;
    float l_MovingTimeMax = 0.0f;
    float l_StopTimeMin = 0.0f;
    float l_StopTimeMax = 0.0f;
    uint16 l_RespawnTime = 0;
    uint8 l_Rank = 0;
    uint8 l_AiType = 0;
    uint8 l_Faction = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Entry = atoi(l_Row[0]);
        l_SkinID = atoi(l_Row[1]);
        l_Name = std::string(l_Row[2]);
        l_Lvl = atoi(l_Row[3]);
        l_Force = atoi(l_Row[4]);
        l_Stamina = atoi(l_Row[5]);
        l_Dexterity = atoi(l_Row[6]);
        l_Xp = atoi(l_Row[7]);
        l_State = atoi(l_Row[8]);
        l_MaxRay = atoi(l_Row[9]);
        l_MaxVision = atoi(l_Row[10]);
        l_MovingTimeMin = (float)atof(l_Row[11]);
        l_MovingTimeMax = (float)atof(l_Row[12]);
        l_StopTimeMin = (float)atof(l_Row[13]);
        l_StopTimeMax = (float)atof(l_Row[14]);
        l_RespawnTime = atoi(l_Row[15]);
        l_Rank = atoi(l_Row[16]);
        l_AiType = atoi(l_Row[17]);
        l_Faction = atoi(l_Row[18]);
        p_CreatureManager->AddCreatureTemplate(CreatureTemplate(l_Entry, l_SkinID, l_Name, l_Lvl, l_Force, l_Stamina, l_Dexterity, l_Xp, l_State, l_MaxRay, l_MaxVision, l_MovingTimeMin, l_MovingTimeMax, l_StopTimeMin, l_StopTimeMax, l_RespawnTime, l_Rank, l_AiType, (eFactionType)l_Faction));
    }
    mysql_free_result(l_Result);

    return true;
}

bool SqlManager::InitializeGossip(UnitManager* p_CreatureManager)
{
    std::string l_Query = "SELECT `id`, `typeUnit`, `unitEntry`, `type`, `data1`, `msg` FROM gossip";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_ID;
    uint8 l_TypeUnit;
    uint16 l_UnitEntry;
    uint8 l_GossipType;
    uint32 l_Data1;
    std::string l_Msg;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_ID = atoi(l_Row[0]);
        l_TypeUnit = atoi(l_Row[1]);
        l_UnitEntry = atoi(l_Row[2]);
        l_GossipType = atoi(l_Row[3]);
        l_Data1 = atoi(l_Row[4]);
        l_Msg = std::string(l_Row[5]);
        p_CreatureManager->AddGossip(Gossip(l_ID, (TypeUnit)l_TypeUnit, l_UnitEntry, (eGossipType)l_GossipType, l_Data1, l_Msg));
    }
    mysql_free_result(l_Result);

    return true;
}

bool SqlManager::InitializeCreature(UnitManager* p_CreatureManager)
{
    std::string l_Query = "SELECT `id`, `entry`, `mapID`, `posX`, `posY` FROM creature";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_Id = 0;
    uint16 l_Entry = 0;
    uint16 l_MapID = 0;
    uint32 l_PosX = 0;
    uint32 l_PosY = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
        l_Entry = atoi(l_Row[1]);
        l_MapID = atoi(l_Row[2]);
        l_PosX = atoi(l_Row[3]);
        l_PosY = atoi(l_Row[4]);

        /*if (l_Entry != 5)
            continue;*/

        Creature* l_Creature = new Creature(l_Id, l_Entry, p_CreatureManager->GetCreatureTemplate(l_Entry), l_MapID, l_PosX, l_PosY);
        if (p_CreatureManager->GetGossipListFor(TypeUnit::CREATURE, l_Entry) != nullptr)
            l_Creature->SetGossipList(p_CreatureManager->GetGossipListFor(TypeUnit::CREATURE, l_Entry));
        Map* l_Map = g_MapManager->GetMap(l_MapID);
        l_Map->AddUnit(l_Creature);
    }
    mysql_free_result(l_Result);

    return true;
}

std::map<uint8, uint16> SqlManager::GetXpLevel()
{
    std::map<uint8, uint16> l_XpLevel;

    std::string l_Query = "SELECT `level`, `xp` FROM level_xp";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint8 l_Level = 0;
    uint16 l_Xp = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Level = atoi(l_Row[0]);
        l_Xp = atoi(l_Row[1]);

        l_XpLevel[l_Level] = l_Xp;
    }
    mysql_free_result(l_Result);

    return l_XpLevel;
}

int16 SqlManager::GetLevel(const std::string & p_PlayerName)
{
    std::string l_Query = "SELECT `level` FROM characters WHERE `name` = '" + p_PlayerName + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    int16 l_Level = -1;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Level = atoi(l_Row[0]);
    }
    mysql_free_result(l_Result);

    return l_Level;
}

WorldPosition SqlManager::GetPosition(const std::string & p_PlayerName)
{
    std::string l_Query = "SELECT `mapID`, `posX`, `posY` FROM characters WHERE `name` = '" + p_PlayerName + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    uint16 l_MapID = 0;
    uint32 l_PosX = 0;
    uint32 l_PosY = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_MapID = atoi(l_Row[0]);
        l_PosX = atoi(l_Row[1]);
        l_PosY = atoi(l_Row[2]);
    }
    mysql_free_result(l_Result);

    WorldPosition l_Position(l_PosX, l_PosY, l_MapID, Orientation::Up);
    return l_Position;
}


int32 SqlManager::GetPlayerID(const std::string & p_PlayerName)
{
    std::string l_Query = "SELECT `characterID` FROM characters WHERE `name` = '" + p_PlayerName + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    int32 l_PlayerID = -1;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_PlayerID = atoi(l_Row[0]);
    }
    mysql_free_result(l_Result);

    return l_PlayerID;
}

eAccessType SqlManager::GetAccessType(uint32 p_AccountID)
{
    std::string l_Query = "SELECT `accessType` FROM `login_access` WHERE `accountID` = '" + std::to_string(p_AccountID) + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    int8 l_ID = 0;
    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;

    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
        l_ID = atoi(l_Row[0]);

    mysql_free_result(l_Result);
    return (eAccessType)l_ID;
}