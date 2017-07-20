#include "SqlManager.hpp"
#include "../Define.hpp"
#include "../Entities/Creature.hpp"
#include "../Entities/Areatrigger.hpp"
#include "../Entities/GameObject.hpp"
#include "../Global.hpp"
#include <cstdlib>
#include <ctime>

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
    std::string l_Query = "SELECT `characterName` FROM `login` WHERE `id` = '" + std::to_string(p_AccountID) + "';";
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
    std::string l_Query = "insert into `characters` (`accountID`, `name`, `skinID`, `level`, `health`, `alignment`, `mapID`, `posX`, `posY`, `orientation`, `xp`) values('" + std::to_string(p_AccountID) + "', '" + GetLoginName(p_AccountID) + "','0','1','100','0','0','2069','211','0','0');";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

void SqlManager::AddKeyDefaultBindsForAccount(uint32 p_AccountID)
{
    std::string l_Query = "INSERT INTO `account_key_binds` VALUES (" + std::to_string(p_AccountID) + ", 1, 74), (" + std::to_string(p_AccountID) + ", 2, 73), (" + std::to_string(p_AccountID) + ", 3, 71), (" + std::to_string(p_AccountID) + ", 4, 72), (" + std::to_string(p_AccountID) + ", 5, 57), (" + std::to_string(p_AccountID) + ", 6, 58), (" + std::to_string(p_AccountID) + ", 7, 85), (" + std::to_string(p_AccountID) + ", 8, 18), (" + std::to_string(p_AccountID) + ", 9, 25), (" + std::to_string(p_AccountID) + ", 10, 4);";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

void SqlManager::ReplaceKeyBindsForAccount(uint32 p_AccountID, eKeyBoardAction p_Bind, uint8 p_Key)
{
    std::string l_Query = "REPLACE `account_key_binds` SET `key` = " + std::to_string(p_Key) + " WHERE `typeID` = " + std::to_string(p_Bind) + " AND `accountID` = " + std::to_string(p_AccountID) + ";";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

Player* SqlManager::GetNewPlayer(uint32 p_AccountID)
{
    std::string l_Query = "SELECT characterID, name, level, class, health, mana, alignment, skinID, mapID, posX, posY, orientation, xp FROM characters WHERE accountID = '" + std::to_string(p_AccountID) + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    uint32 l_ID = 0;
    std::string l_Name = "";
    uint8 l_Lvl = 0;
    uint8 l_Class = 0;
    uint8 l_Health = 0;
    uint8 l_Mana = 0;
    uint8 l_Alignment = 0;
    int16 l_SkinID = 0;
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
        l_Class = atoi(l_Row[3]);
        l_Health = atoi(l_Row[4]);
        l_Mana = atoi(l_Row[5]);
        l_Alignment = atoi(l_Row[6]);
        l_SkinID = atoi(l_Row[7]);
        l_MapID = atoi(l_Row[8]);
        l_PosX = atoi(l_Row[9]);
        l_PosY = atoi(l_Row[10]);
        l_Orientation = atoi(l_Row[11]);
        l_Xp = atoi(l_Row[12]);
    }
    mysql_free_result(l_Result);

    eAccessType l_PlayerAccessType = GetAccessType(p_AccountID);
    eAccessType l_AccessRequired = (eAccessType)atoi(g_Config->GetValue("AccessLevel").c_str());

    if (l_AccessRequired > l_PlayerAccessType)
        return nullptr;

    if (!l_Exist)
    {
        printf("Create new Player %d", p_AccountID);
        AddNewPlayer(p_AccountID);
        AddKeyDefaultBindsForAccount(p_AccountID);
        Player* l_Player = GetNewPlayer(p_AccountID);
        /* FOR TEST*/
        AddSpellForPlayer(l_Player, 1);
        AddSpellForPlayer(l_Player, 2);
        AddSpellBind(l_Player, 1, 9);
        AddSpellBind(l_Player, 2, 10);
        return l_Player;
    }

    l_Player = new Player(p_AccountID, l_ID, l_Name, l_Lvl, (eClass)l_Class, l_Health, l_Mana, l_Alignment, l_SkinID, l_MapID, l_PosX, l_PosY, (Orientation)l_Orientation, l_Xp, l_PlayerAccessType);
    l_Player->SetRespawnPosition(GetRespawnPositionForPlayer(l_ID));
    InitializeSpellsForPlayer(l_Player);
    InitializeKeyBindsForAccount(p_AccountID, l_Player);
    InitializeSpellsBinds(l_Player);
    InitializeQuestsProgessForPlayer(l_Player);

    return l_Player;
}

void SqlManager::AddNewRespawnPositionForPlayer(uint32 p_PlayerID)
{
    std::string l_Query = "insert into `characters_respawn` (`characterID`, `posX`, `posY`, `mapID`, `orientation`) values('" + std::to_string(p_PlayerID) + "', '2069', '211', '0', '0');";
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

    WorldPosition l_Position(l_PosX, l_PosY, l_MapID, 0, (Orientation)l_Orientation);
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

	PointsSet l_PointsSet;
    l_PointsSet.SetStat(eStats::Free, l_FreePoints);
    l_PointsSet.SetStat(eStats::Force, l_Force);
    l_PointsSet.SetStat(eStats::Stamina, l_Stamina);
    l_PointsSet.SetStat(eStats::Dexterity, l_Dexterity);

	return l_PointsSet;
}

void SqlManager::SavePlayer(Player* p_Player)
{
    /// Save cooldowns
    std::map< uint16, uint64 >* l_SpellList = p_Player->GetSpellList();
    for (std::map< uint16, uint64 >::iterator l_It = l_SpellList->begin(); l_It != l_SpellList->end(); ++l_It)
    {
        /// CHECK 
        std::string l_Query = "UPDATE characters_spells SET `cooldown` = '" + std::to_string((*l_It).second) + "' WHERE spellID = '" + std::to_string((*l_It).first) + "' AND characterID = '" + std::to_string(p_Player->GetID()) + "';";
        mysql_query(&m_MysqlCharacters, l_Query.c_str());
    }

    /// Save QuestsProgess
    std::map< uint16, Quest* >* l_QuestList = p_Player->GetQuestList();
    for (std::map< uint16, Quest* >::iterator l_It = l_QuestList->begin(); l_It != l_QuestList->end(); ++l_It)
    {
        /// Save Objectf Progress
        std::map< uint8, ObjectifProgess* >* l_ObjectProgressList = (*l_It).second->GetObjectifsProgress();
        for (std::map< uint8, ObjectifProgess* >::iterator l_Itr = l_ObjectProgressList->begin(); l_Itr != l_ObjectProgressList->end(); ++l_Itr)
        {
            /// Remove it from quest in Progress
            std::string l_Query = "DELETE FROM quest_objectif_progress WHERE characterID = '" + std::to_string(p_Player->GetID()) + "' AND questID = '" + std::to_string((*l_It).first) + "';";
            mysql_query(&m_MysqlCharacters, l_Query.c_str());
        /// CHECK 
            l_Query = "REPLACE INTO quest_objectif_progress (characterID, questID, objectifID, data0) VALUES ('" + std::to_string(p_Player->GetID()) + "', '" + std::to_string((*l_It).first) + "', '" + std::to_string((*l_Itr).first) + "', '" + std::to_string((*l_Itr).second->m_Data0) + "');";
            mysql_query(&m_MysqlCharacters, l_Query.c_str());
        }
    }

	std::string l_Query = "UPDATE characters SET `posX` = '" + std::to_string(p_Player->GetPosX()) + "', `posY` = '" + std::to_string(p_Player->GetPosY()) + "', `mapID` = '" + std::to_string(p_Player->GetMapID()) + "', `orientation` = '" + std::to_string(p_Player->GetOrientation()) + "', `health` = '" + std::to_string(p_Player->GetResourceNb(eResourceType::Health)) + "', `mana` = '" + std::to_string(p_Player->GetResourceNb(eResourceType::Mana)) + "', `alignment` = '" + std::to_string(p_Player->GetResourceNb(eResourceType::Alignment)) + "', `xp` = '" + std::to_string(p_Player->GetXp()) + "', `level` = '" + std::to_string(p_Player->GetLevel()) + "', skinID = '" + std::to_string(p_Player->GetSkinID()) + "' WHERE characterID = '" + std::to_string(p_Player->GetID()) + "';";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
	UpdatePointsSet(p_Player);
}

void SqlManager::SaveQuestForPlayer(Player const* p_Player,  Quest const* p_Quest)
{
    /// Save Quest
    std::string l_Query = "INSERT INTO quests_done (characterID, questID) VALUES ('" + std::to_string(p_Player->GetID()) + "', '" + std::to_string(p_Quest->GetID()) + "');";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    /// Remove it from quest in Progress
    l_Query = "DELETE FROM quest_objectif_progress WHERE characterID = '" + std::to_string(p_Player->GetID()) + "' AND questID = '" + std::to_string(p_Quest->GetID()) + "';";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

int32 SqlManager::GetDaysSinceLastQuestDone(Player const* p_Player, uint16 p_QuestID)
{
    std::string l_Query = "SELECT DATEDIFF(NOW(), MAX(`dateValidate`)) FROM quests_done WHERE characterID = '" + std::to_string(p_Player->GetID()) + "' AND questID = '" + std::to_string(p_QuestID)  + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    int16 l_Days = -1;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    bool l_Exist = false;
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        if (l_Row == NULL)
            break;
        if (l_Row[0])
            l_Days = atoi(l_Row[0]);
    }
    mysql_free_result(l_Result);

    return l_Days;
}

void SqlManager::UpdatePointsSet(Player const* p_Player)
{
	std::string l_Query = "UPDATE `characters_point` SET `free_point` = '" + std::to_string(p_Player->GetPointsSet().GetStat(eStats::Free)) + "', `force` = '" + std::to_string(p_Player->GetPointsSet().GetStat(eStats::Force)) + "', `stamina` = '" + std::to_string(p_Player->GetPointsSet().GetStat(eStats::Stamina)) + "', `dexterity` = '" + std::to_string(p_Player->GetPointsSet().GetStat(eStats::Dexterity)) + "' WHERE characterID = '" + std::to_string(p_Player->GetID()) + "';";
	mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

CreatureTemplate SqlManager::GetCreatureTemplate(uint16 p_Entry)
{
    std::string l_Query = "SELECT `skinID`, `name`, `level`, `force`, `stamina`, `dexterity`, `xp`, `state`, `maxRay`, `maxVision`,`movingTimeMin`, `movingTimeMax`, `stopTimeMin`, `stopTimeMax`, `respawnTime`, `rank`, `aiType`, `faction` FROM creature_template WHERE `entry` = '" + std::to_string(p_Entry) + "'";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    int16 l_SkinID = 0;
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
    int32 l_RespawnTime = 0;
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
    if (l_Result == NULL)
    {
        printf("FAILD SQL ADD CREATURE\n");
        return 0;
    }
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
    int32 l_RespawnTime = 0;
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

bool SqlManager::InitializeSpellsForPlayer(Player* p_Player)
{
    std::string l_Query = "SELECT `spellID`, `cooldown` FROM characters_spells WHERE `characterID` = '" + std::to_string(p_Player->GetID()) + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    uint16 l_SpellID = 0;
    uint64 l_Cooldown = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_SpellID = atoi(l_Row[0]);
        l_Cooldown = atoi(l_Row[1]);
        p_Player->AddSpellID(l_SpellID, l_Cooldown);
    }
    mysql_free_result(l_Result);

    return true;
}

bool SqlManager::InitializeQuestsProgessForPlayer(Player* p_Player)
{
    std::string l_Query = "SELECT `questID`, `objectifID`, `data0`, `data1`, `data2`, `data3` FROM quest_objectif_progress WHERE `characterID` = '" + std::to_string(p_Player->GetID()) + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    uint16 l_QuestID = 0;
    uint8 l_ObjectifID = 0;
    int16 l_Data0 = -1;
    int16 l_Data1 = -1;
    int16 l_Data2 = -1;
    int16 l_Data3 = -1;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_QuestID = atoi(l_Row[0]);
        l_ObjectifID = atoi(l_Row[1]);
        l_Data0 = atoi(l_Row[2]);
        l_Data1 = atoi(l_Row[3]);
        l_Data2 = atoi(l_Row[4]);
        l_Data3 = atoi(l_Row[5]);

        Quest* l_Quest = p_Player->GetQuest(l_QuestID);
        if (l_Quest == nullptr)
        {
            l_Quest = new Quest(g_QuestManager->GetQuestTemplate(l_QuestID));
            p_Player->AddQuest(l_Quest, false);
        }
        l_Quest->SetDataOfObjectif(l_ObjectifID, 0, l_Data0); /* Actually we only use data0 */
    }
    mysql_free_result(l_Result);

    return true;
}


bool SqlManager::InitializeSpellsBinds(Player* p_Player)
{
    std::string l_Query = "SELECT `spellID`, `bindID` FROM characters_spell_binds WHERE `characterID` = '" + std::to_string(p_Player->GetID()) + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    uint16 l_SpellID = 0;
    uint8 l_BindID = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_SpellID = atoi(l_Row[0]);
        l_BindID = atoi(l_Row[1]);
        p_Player->AddSpellBindToKey(l_SpellID, l_BindID);
    }
    mysql_free_result(l_Result);

    return true;
}

void SqlManager::AddSpellForPlayer(Player* p_Player, uint16 l_SpellID)
{
    std::string l_Query = "REPLACE INTO `characters_spells` (characterID, spellID) VALUES ('" + std::to_string(p_Player->GetID()) + "', '" + std::to_string(l_SpellID) + "');";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

void SqlManager::AddSpellBind(Player* p_Player, uint16 l_SpellID, uint8 l_BindID)
{
    std::string l_Query = "DELETE FROM `characters_spell_binds` WHERE bindID = '" + std::to_string(l_BindID) + "' AND characterID = '" + std::to_string(p_Player->GetID()) + "';";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
    l_Query = "INSERT INTO `characters_spell_binds` (characterID, spellID, bindID) VALUES ('" + std::to_string(p_Player->GetID()) + "', '" + std::to_string(l_SpellID) + "', '" + std::to_string(l_BindID) + "');";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
    p_Player->AddSpellBindToKey(l_SpellID, l_BindID);
}

bool SqlManager::InitializeKeyBindsForAccount(uint32 p_Account, Player* p_Player)
{
    std::string l_Query = "SELECT `typeID`, `key` FROM account_key_binds WHERE `accountID` = '" + std::to_string(p_Account) + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    uint8 l_TypeID = 0;
    uint8 l_Key = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_TypeID = atoi(l_Row[0]);
        l_Key = atoi(l_Row[1]);
        p_Player->AddKeyBoardBind((eKeyBoardAction)l_TypeID, l_Key);
    }
    mysql_free_result(l_Result);

    return true;
}

bool SqlManager::InitializeGossip(UnitManager* p_CreatureManager, RequiredManager* p_RequiredManager)
{
    std::string l_Query = "SELECT `id`, `requiredID`, `typeUnit`, `unitEntry`, `type`, `data0`, `data1`, `msg` FROM gossip";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_ID = 0;
    int32 l_RequiredID = -1;
    uint8 l_TypeUnit = 0;
    uint16 l_UnitEntry = 0;
    uint8 l_GossipType = 0;
    uint32 l_Data0 = 0;
    uint32 l_Data1 = 0;
    std::string l_Msg = "";

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_ID = atoi(l_Row[0]);
        l_RequiredID = atoi(l_Row[1]);
        l_TypeUnit = atoi(l_Row[2]);
        l_UnitEntry = atoi(l_Row[3]);
        l_GossipType = atoi(l_Row[4]);
        l_Data0 = atoi(l_Row[5]);
        if (l_Row[6])
            l_Data1 = atoi(l_Row[6]);
        l_Msg = std::string(l_Row[7]);
        Required* l_Required = nullptr;
        if (l_RequiredID >= 0) /// -1 if no required
            l_Required = p_RequiredManager->GetRequiered(l_RequiredID);
        p_CreatureManager->AddGossip(Gossip(l_ID, l_Required, (TypeUnit)l_TypeUnit, l_UnitEntry, (eGossipType)l_GossipType, l_Data0, l_Data1, l_Msg));
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

        /*Creature* l_Creature = new Creature(l_Id, l_Entry, p_CreatureManager->GetCreatureTemplate(l_Entry), l_MapID, l_PosX, l_PosY);
        if (p_CreatureManager->GetGossipListFor(TypeUnit::CREATURE, l_Entry) != nullptr)
            l_Creature->SetGossipList(p_CreatureManager->GetGossipListFor(TypeUnit::CREATURE, l_Entry));*/
        MapTemplate* l_MapTemplate = g_MapManager->GetMapTemplate(l_MapID);
        // CreatureMapTemplate(uint16 p_ID, uint16 p_MapID, CreatureTemplate* p_Template, uint32 p_PosX, uint32 p_PosY, std::vector<Gossip>* p_GossipList) :

        if (l_MapTemplate == nullptr)
        {
            printf("Erreur map %d don't exist\n", l_MapID);
            continue;
        }
        
        l_MapTemplate->AddCreatureMapTemplate(CreatureMapTemplate(l_Id, l_MapID, p_CreatureManager->GetCreatureTemplate(l_Entry), l_PosX, l_PosY, p_CreatureManager->GetGossipListFor(TypeUnit::CREATURE, l_Entry)));
        //l_MapTemplate->AddUnit(l_Creature);
    }
    mysql_free_result(l_Result);

    return true;
}

bool  SqlManager::InitializeSpells()
{
    std::string l_Query = "SELECT `id`, `level`, `visualID`, `visualIDTarget`,`castTime`, `cooldown`, `duration`, `speed`,`resourceType`, `resourceNb`, `effect1`, `effect2`, `effect3`, `effect4` FROM spell_template";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_Id = 0;
    uint8 l_Level = 0;
    int32 l_VisualID = -1;
    int32 l_VisualIDTarget = -1;
    uint16 l_CastTime = 0;
    uint32 l_Cooldown = 0;
    int32 l_Duration = 0;
    float l_Speed = 0.0f;
    int16 l_ResourceType = 0;
    int32 l_ResourceNb = 0;
    std::vector<int32> l_EffectList;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
        l_Level = atoi(l_Row[1]);
        l_VisualID = atoi(l_Row[2]);
        l_VisualIDTarget = atoi(l_Row[3]);
        l_CastTime = atoi(l_Row[4]);
        l_Cooldown = atoi(l_Row[5]);
        l_Duration = atoi(l_Row[6]);
        l_Speed = (float)atof(l_Row[7]);
        l_ResourceType = atoi(l_Row[8]);
        l_ResourceNb = atoi(l_Row[9]);
        for (uint8 i = 0; i < MAX_EFFECTS_FOR_SPELL; ++i)
            l_EffectList.push_back(atoi(l_Row[10 + i]));

        SpellTemplate* l_Spell = new SpellTemplate(l_Id);
        l_Spell->SetLevel(l_Level);
        l_Spell->SetVisualsID(l_VisualID, l_VisualIDTarget);
        l_Spell->SetCastTime(l_CastTime);
        l_Spell->SetCooldown(l_Cooldown);
        l_Spell->SetSpeed(l_Speed);
        l_Spell->SetDuration(l_Duration);
        if (l_ResourceType > 0)
        {
            l_Spell->AddResourceNeed(ResourceNeed((eResourceType)l_ResourceType, l_ResourceNb));
        }

        for (int32 l_SpellEffectID : l_EffectList)
        {
            if (g_SpellManager->GetSpellEffect(l_SpellEffectID) != nullptr)
                l_Spell->AddSpellEffect(g_SpellManager->GetSpellEffect(l_SpellEffectID));
        }

        g_SpellManager->AddSpell(l_Spell);
        l_EffectList.clear();
    }
    mysql_free_result(l_Result);
    return true;
}

bool  SqlManager::InitializeSpellEffects()
{
    std::string l_Query = "SELECT `id`, `effectType`, `target`, `basepoint1`, `basepoint2`, `basepoint3`, `basepoint4`, `radiusMin`, `radiusMax` FROM spell_effect";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_Id = 0;
    uint8 l_EffectType = 0;
    uint8 l_Target = 0;
    int32 l_BasePoint1 = 0;
    int32 l_BasePoint2 = 0;
    int32 l_BasePoint3 = 0;
    int32 l_BasePoint4 = 0;
    float l_RadiusMin = 0.0f;
    float l_RadiusMax = 0.0f;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
        l_EffectType = atoi(l_Row[1]);
        l_Target = atoi(l_Row[2]);
        l_BasePoint1 = atoi(l_Row[3]);
        l_BasePoint2 = atoi(l_Row[4]);
        l_BasePoint3 = atoi(l_Row[5]);
        l_BasePoint4 = atoi(l_Row[6]);
        l_RadiusMin = (float)atof(l_Row[7]);
        l_RadiusMax = (float)atof(l_Row[8]);

        SpellEffect l_SpellEffect(l_Id, (SpellEffectType)l_EffectType, (SpellTarget)l_Target, l_BasePoint1, l_BasePoint2, l_BasePoint3, l_BasePoint4, l_RadiusMin, l_RadiusMax);
        g_SpellManager->AddSpellEffect(l_SpellEffect);
    }
    mysql_free_result(l_Result);
    return true;
}

bool SqlManager::InitializeRequired(RequiredManager* p_RequiredManager)
{
    std::string l_Query = "SELECT `id`, `requiredID`, `typeID`, `data0`, `data1` FROM sub_required";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_Id = 0;
    uint16 l_RequiredID = 0;
    eRequiredType l_TypeID;
    uint32 l_Data0 = 0;
    uint32 l_Data1 = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
        l_RequiredID = atoi(l_Row[1]);
        l_TypeID = (eRequiredType)atoi(l_Row[2]);
        l_Data0 = atoi(l_Row[3]);
        l_Data1 = atoi(l_Row[4]);

        p_RequiredManager->AddSubRequiered(l_RequiredID, (eRequiredType)l_TypeID, l_Data0, l_Data1);
    }
    mysql_free_result(l_Result);
    return true;
}

bool  SqlManager::InitializeQuests()
{
    /// QUEST TEMPLATE
    std::string l_Query = "SELECT `id`, `repetitionType`, `name`  FROM quest_template";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_Id = 0;
    uint8 l_RepetitionType = 0;
    std::string l_Name = "";

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
        l_RepetitionType = atoi(l_Row[1]);
        l_Name = std::string(l_Row[2]);

        //printf("Add Quest %d %d %s\n", l_Id, l_RepetitionType, l_Name.c_str());
        QuestTemplate* l_QuestTempalte = new QuestTemplate(l_Id, (eRepetitionType)l_RepetitionType, l_Name);
        g_QuestManager->AddQuestTemplate(l_QuestTempalte);
    }
    mysql_free_result(l_Result);

    /// OBJECTIFS
    l_Query = "SELECT `questID`, `id`, `typeID`, `data0`, `data1`, `data2`, `data3`, `entitled` FROM objectif_quest_template";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint8 l_IdObjective = 0;
    uint16 l_QuestID = 0;
    uint8 l_TypeID = 0;
    int16 l_Data0 = 0;
    int16 l_Data1 = 0;
    int16 l_Data2 = 0;
    int16 l_Data3 = 0;
    std::string l_Entitled = "";

    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_QuestID = atoi(l_Row[0]);
        l_IdObjective = atoi(l_Row[1]);
        l_TypeID = atoi(l_Row[2]);
        l_Data0 = atoi(l_Row[3]);
        l_Data1 = atoi(l_Row[4]);
        l_Data2 = atoi(l_Row[5]);
        l_Data3 = atoi(l_Row[6]);
        l_Entitled = std::string(l_Row[7]);

        ObjectifQuestTemplate* l_ObjectifQuestTemplate = new ObjectifQuestTemplate(l_IdObjective, (eObjectifType)l_TypeID, l_Data0, l_Data1, l_Data2, l_Data3, l_Entitled);
        g_QuestManager->AddObjectifToTemplate(l_QuestID, l_ObjectifQuestTemplate);
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

bool SqlManager::InitializeGameObject(DynamicObjectManager* p_DynamicObjectManager, RequiredManager* p_RequiredManager, UnitManager* p_UnitManager)
{
    std::string l_Query = "SELECT `id`, `typeID`, `requiredID`, `blocking`, `skinID`, `duration`, `respawnTime`, `data0`, `data1`, `data2`, `data3` FROM gob_template";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_Id = 0;
    uint16 l_TypeID = 0;
    int32 l_RequiredID = -1;
    int16 l_SkinID = 0;
    bool l_Blocking = false;
    int32 l_Duration = 0;
    int32 l_RespawnTime = 0;
    uint32 l_Data0 = 0;
    uint32 l_Data1 = 0;
    uint32 l_Data2 = 0;
    uint32 l_Data3 = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
        l_TypeID = atoi(l_Row[1]);
        l_RequiredID = atoi(l_Row[2]);
        l_Blocking = (bool)atoi(l_Row[3]);
        l_SkinID = atoi(l_Row[4]);
        l_Duration = atoi(l_Row[5]);
        l_RespawnTime = atoi(l_Row[6]);
        l_Data0 = atoi(l_Row[7]);
        l_Data1 = atoi(l_Row[8]);
        l_Data2 = atoi(l_Row[9]);
        l_Data3 = atoi(l_Row[10]);

        Required* l_Required = nullptr;
        if (l_RequiredID >= 0) /// -1 if no required
            l_Required = p_RequiredManager->GetRequiered(l_RequiredID);
        GameObjectTemplate l_GameObjectTemplate(l_Id, l_Duration, l_RespawnTime, (eGameObjectTemplate)l_TypeID, l_SkinID, l_Blocking, l_Required);
        l_GameObjectTemplate.SetData(0, l_Data0);
        l_GameObjectTemplate.SetData(1, l_Data1);
        l_GameObjectTemplate.SetData(2, l_Data2);
        l_GameObjectTemplate.SetData(3, l_Data3);
        p_DynamicObjectManager->AddGameObjectTemplate(l_GameObjectTemplate);
    }
    mysql_free_result(l_Result);
    

    l_Query = "SELECT `id`, `gobID`, `mapID`, `caseNb` FROM gob";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    l_Id = 0;
    uint16 l_GobID = 0;
    uint16 l_MapID = 0;
    uint32 l_CaseNb = 0;

    uint32 l_PosX = 0;
    uint32 l_PosY = 0;

    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
        l_GobID = atoi(l_Row[1]);
        l_MapID = atoi(l_Row[2]);
        l_CaseNb = atoi(l_Row[3]);

        //Map* l_Map = g_MapManager->GetMap(l_MapID);
        MapTemplate* l_MapTemplate = g_MapManager->GetMapTemplate(l_MapID);

        if (l_MapTemplate == nullptr)
        {
            printf("Erreur map %d don't exist\n", l_MapID);
            continue;
        }

        l_PosX = (l_CaseNb % l_MapTemplate->GetSizeX() * TILE_SIZE) + TILE_SIZE / 2;
        l_PosY = (l_CaseNb / l_MapTemplate->GetSizeX() * TILE_SIZE) + TILE_SIZE / 2;


        GameObjectTemplate* l_GobTemplate = p_DynamicObjectManager->GetGameObjectTemplate(l_GobID);

        if (l_GobTemplate == nullptr)
            continue;

        //GameObject* l_Gob = new GameObject(l_Id, l_MapTemplate->GetID(), l_PosX, l_PosY, l_GobTemplate);
        /*if (p_UnitManager->GetGossipListFor(TypeUnit::GAMEOBJECT, l_GobID) != nullptr)
            l_Gob->SetGossipList(p_UnitManager->GetGossipListFor(TypeUnit::GAMEOBJECT, l_GobID));*/
        l_MapTemplate->AddGobMapTemplate(GobMapTemplate(l_Id, l_MapID, l_GobTemplate, l_CaseNb, l_PosX, l_PosY, p_UnitManager->GetGossipListFor(TypeUnit::GAMEOBJECT, l_GobID)));
        //l_MapTemplate->GetCase(l_CaseNb)->AddDynamicOject(l_Gob);
    }
    mysql_free_result(l_Result);

    return true;
}

bool SqlManager::InitializeZones()
{
    std::string l_Query = "SELECT `id`, `typeID`, `name`,`mapID`, `caseBegin`, `caseEnd` FROM zone";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_ID = 0;
    uint16 l_TypeID = 0;
    uint16 l_MapID = 0;
    uint16 l_CaseNbBegin = 0;
    uint16 l_CaseNbEnd = 0;
    std::string l_Name = "";

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_ID = atoi(l_Row[0]);
        l_TypeID = atoi(l_Row[1]);
        l_Name = std::string(l_Row[2]);
        l_MapID = atoi(l_Row[3]);
        l_CaseNbBegin = atoi(l_Row[4]);
        l_CaseNbEnd = atoi(l_Row[5]);

        MapTemplate* l_MapTemplate = g_MapManager->GetMapTemplate(l_MapID);
        //Map* l_Map = g_MapManager->GetMap(l_MapID);
        if (l_MapTemplate == nullptr)
        {
            printf("Erreur map %d don't exist\n", l_MapID);
            continue;
        }
        Zone l_Zone = Zone(l_ID, (eTypeZone)l_TypeID, l_Name, l_CaseNbBegin, l_CaseNbEnd);
        l_MapTemplate->AddZone(l_Zone);
    }
    mysql_free_result(l_Result);

    return true;
}

bool SqlManager::InitializeAreatrigger(DynamicObjectManager* p_DynamicObjectManager, UnitManager* p_UnitManager)
{
    std::string l_Query = "SELECT `id`, `typeID`,`skinID`, `radius`, `data0`, `data1`, `data2`, `data3` FROM areatrigger_template";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_Id = 0;
    uint16 l_TypeID = 0;
    int16 l_SkinID = 0;
    float l_Radius = 0;
    uint32 l_Data0 = 0;
    uint32 l_Data1 = 0;
    uint32 l_Data2 = 0;
    uint32 l_Data3 = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
        l_TypeID = atoi(l_Row[1]);
        l_SkinID = atoi(l_Row[2]);
        l_Radius = (float)atof(l_Row[3]);
        l_Data0 = atoi(l_Row[4]);
        l_Data1 = atoi(l_Row[5]);
        l_Data2 = atoi(l_Row[6]);
        l_Data3 = atoi(l_Row[7]);

        AreatriggerTemplate l_AreatriggerTemplate(l_Id, l_Radius, (eAreatriggerType)l_TypeID, l_SkinID);
        l_AreatriggerTemplate.SetData(0, l_Data0);
        l_AreatriggerTemplate.SetData(1, l_Data1);
        l_AreatriggerTemplate.SetData(2, l_Data2);
        l_AreatriggerTemplate.SetData(3, l_Data3);
        p_DynamicObjectManager->AddAreatriggerTemplate(l_AreatriggerTemplate);
    }
    mysql_free_result(l_Result);


    l_Query = "SELECT `id`, `areatriggerID`, `mapID`, `caseNb` FROM areatrigger";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    l_Id = 0;
    uint16 l_AreatriggerID = 0;
    uint16 l_MapID = 0;
    uint32 l_CaseNb = 0;

    uint32 l_PosX = 0;
    uint32 l_PosY = 0;

    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
        l_AreatriggerID = atoi(l_Row[1]);
        l_MapID = atoi(l_Row[2]);
        l_CaseNb = atoi(l_Row[3]);

        MapTemplate* l_MapTemplate = g_MapManager->GetMapTemplate(l_MapID);
        if (l_MapTemplate == nullptr)
        {
            printf("Erreur map %d don't exist\n", l_MapID);
            continue;
        }
        l_PosX = (l_CaseNb % l_MapTemplate->GetSizeX() * TILE_SIZE) + TILE_SIZE / 2;
        l_PosY = (l_CaseNb / l_MapTemplate->GetSizeX() * TILE_SIZE) + TILE_SIZE;


        AreatriggerTemplate* l_AreatriggerTemplate = p_DynamicObjectManager->GetAreatriggerTemplate(l_AreatriggerID);

        if (l_AreatriggerTemplate == nullptr)
            continue;
        //Areatrigger* l_Areatrigger = new Areatrigger(l_Id, l_MapTemplate->GetID(), l_PosX, l_PosY, l_AreatriggerTemplate);
        l_MapTemplate->AddAreaTriggerMapTemplate(AreaTriggerMapTemplate(l_Id, l_MapID, l_AreatriggerTemplate, l_CaseNb, l_PosX, l_PosY, p_UnitManager->GetGossipListFor(TypeUnit::AREATRIGGER, l_AreatriggerID)));
        /*l_MapTemplate->AddUnit(l_Areatrigger);
        l_MapTemplate->GetCase(l_CaseNb)->AddDynamicOject(l_Areatrigger);*/
    }
    mysql_free_result(l_Result);

    return true;
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

    WorldPosition l_Position(l_PosX, l_PosY, l_MapID, 0, Orientation::Up);
    return l_Position;
}

bool SqlManager::InitializeMaps()
{
    std::string l_Query = "SELECT `id`, `typeID`,`name`, `fileName`, `fileChipsets`, `maxPlayers`, `instance` FROM maps";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_Id = 0;
    uint16 l_TypeID = 0;
    uint16 l_MaxPlayers = 0;
    uint16 l_Instance = 0;
    std::string l_FileName = "";
    std::string l_FileChipsets = "";
    std::string l_Name = "";

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
        l_TypeID = atoi(l_Row[1]);
        l_Name = std::string(l_Row[2]);
        l_FileName = std::string(l_Row[3]);
        l_FileChipsets = std::string(l_Row[4]);
        l_MaxPlayers = atoi(l_Row[5]);
        l_Instance = atoi(l_Row[6]);

        MapTemplate* l_MapTemplate = new MapTemplate(l_Id, (eTypeMap)l_TypeID, l_MaxPlayers, l_Name, l_FileName, l_FileChipsets, (bool)l_Instance);
        g_MapManager->AddMapTemplate(l_MapTemplate);
    }
    mysql_free_result(l_Result);
    return true;
}

bool SqlManager::InitializeBattlegrounds()
{
    std::string l_Query = "SELECT `id`, `mapID`,`minPlayers`, `maxPlayers`, `timeMax` FROM battleground";
    mysql_query(&m_MysqlWorld, l_Query.c_str());

    uint16 l_Id = 0;
    uint16 l_MapID = 0;
    uint16 l_MinPlayers = 0;
    uint16 l_MaxPlayers = 0;
    uint16 l_TimeMax = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;
    l_Result = mysql_use_result(&m_MysqlWorld);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Id = atoi(l_Row[0]);
        l_MapID = atoi(l_Row[1]);
        l_MinPlayers = atoi(l_Row[2]);
        l_MaxPlayers = atoi(l_Row[3]);
        l_TimeMax = atoi(l_Row[4]);

        g_MapManager->AddBGTemplate(new BGTemplate(l_Id, l_MapID, l_MinPlayers, l_MaxPlayers, l_TimeMax));
    }
    mysql_free_result(l_Result);
    return true;
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

void SqlManager::BlackListIp(const std::string & p_AdressIP, const uint32 & p_AccountAdm, const uint32 & p_TotalHours, const std::string & p_Comment)
{
    std::string l_Query = "INSERT INTO `black_list` (ip, accountAdmin, totalHours, comment) VALUES ('" + p_AdressIP + "', '" + std::to_string(p_AccountAdm) + "', '" + std::to_string(p_TotalHours) + "', '" + p_Comment + "');";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

void SqlManager::BlackListAccount(const uint32 & p_AccountID, const uint32 & p_AccountAdm, const uint32 & p_TotalHours, const std::string & p_Comment)
{
    std::string l_Query = "INSERT INTO `black_list` (accountID, accountAdmin, totalHours, comment) VALUES ('" + std::to_string(p_AccountID) + "', '" + std::to_string(p_AccountAdm) + "', '" + std::to_string(p_TotalHours) + "', '" + p_Comment + "');";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());
}

bool SqlManager::IsAccountBan(const uint32 & p_AccountID)
{
    std::string l_Query = "SELECT Unix_Timestamp(`date`), `totalHours` FROM `black_list` WHERE `accountID` = '" + std::to_string(p_AccountID) + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    bool m_IsBan = false;
    std::time_t l_Now;
    std::time_t l_Time;
    uint32 l_Hours;
    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;

    time(&l_Now);
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Time = (std::time_t)atoll(l_Row[0]);
        l_Hours = atoi(l_Row[1]);
        struct tm now_tm = *localtime(&l_Time);
        struct tm then_tm = now_tm;

        then_tm.tm_hour += l_Hours;
        const double diff = std::difftime(l_Now, mktime(&then_tm));
        if (diff <= 0)
        {
            m_IsBan = true;
            break;
        }
    }

    mysql_free_result(l_Result);
    return m_IsBan;
}

bool SqlManager::IsIPBan(const std::string & p_IP)
{
    std::string l_Query = "SELECT Unix_Timestamp(`date`), `totalHours` FROM `black_list` WHERE `ip` = '" + p_IP + "'";
    mysql_query(&m_MysqlCharacters, l_Query.c_str());

    bool m_IsBan = false;
    std::time_t l_Now;
    std::time_t l_Time;
    uint32 l_Hours;
    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;

    time(&l_Now);
    l_Result = mysql_use_result(&m_MysqlCharacters);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_Time = (std::time_t)atoll(l_Row[0]);
        l_Hours = atoi(l_Row[1]);
        struct tm now_tm = *localtime(&l_Time);
        struct tm then_tm = now_tm;

        then_tm.tm_hour += l_Hours;
        const double diff = std::difftime(l_Now, mktime(&then_tm));
        if (diff <= 0)
        {
            m_IsBan = true;
            break;
        }
    }

    mysql_free_result(l_Result);
    return m_IsBan;
}