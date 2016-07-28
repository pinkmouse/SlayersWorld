#include "SqlManager.hpp"
#include "../Define.hpp"
#include <cstdlib>

SqlManager::SqlManager()
{
	mysql_init(&m_Mysql);
}


SqlManager::~SqlManager()
{
    mysql_close(&m_Mysql);
}

bool SqlManager::Initialize(std::string p_Host, std::string p_User, std::string p_PassWord, std::string p_DB, std::string p_Port)
{
	if (!mysql_real_connect(&m_Mysql, p_Host.c_str(), p_User.c_str(), p_PassWord.c_str(), p_DB.c_str(), std::stoi(p_Port), NULL, 0))
		return false;
	return true;
}

int32 SqlManager::GetIDLogin(std::string p_Login, std::string p_Password)
{
    std::string l_Query = "SELECT id FROM login WHERE login = '" + p_Login + "' AND password = MD5('" + p_Password + "')";
    mysql_query(&m_Mysql, l_Query.c_str());

    int32 l_ID = 0;
    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;

    l_Result = mysql_use_result(&m_Mysql);
    while ((l_Row = mysql_fetch_row(l_Result)))
        l_ID = atoi(l_Row[0]);

    mysql_free_result(l_Result);
    return l_ID;
}

Player* SqlManager::GetNewPlayer(uint32 p_AccountID)
{
    std::string l_Query = "SELECT characterID, name, level, skinID, mapID, posX, posY, orientation FROM characters WHERE accountID = '" + std::to_string(p_AccountID) + "'";
    mysql_query(&m_Mysql, l_Query.c_str());

    uint32 l_ID = 0;
    std::string l_Name = "";
    uint8 l_Lvl = 0;
    uint8 l_SkinID = 0;
    uint16 l_MapID = 0;
    uint32 l_PosX = 0;
    uint32 l_PosY = 0;
    uint8 l_Orientation = 0;

    MYSQL_RES *l_Result = NULL;
    MYSQL_ROW l_Row;

    l_Result = mysql_use_result(&m_Mysql);
    while ((l_Row = mysql_fetch_row(l_Result)))
    {
        l_ID = atoi(l_Row[0]);
        l_Name = std::string(l_Row[1]);
        l_Lvl = atoi(l_Row[2]);
        l_SkinID = atoi(l_Row[3]);
        l_MapID = atoi(l_Row[4]);
        l_PosX = atoi(l_Row[5]);
        l_PosY = atoi(l_Row[6]);
        l_Orientation = atoi(l_Row[7]);
    }
    mysql_free_result(l_Result);

    Player* l_Player = new Player(l_ID, l_Name, l_Lvl, l_SkinID, l_MapID, l_PosX, l_PosY, (Orientation)l_Orientation);
    return l_Player;
}