#pragma once

#include <winsock.h>
#include <mysql.h>
#include <string>
#include "../Define.hpp"
#include "../Entities/Player.hpp"
#include "../System/WorldPosition.hpp"
#include "CreatureManager.hpp"

class SqlManager
{
public:
	SqlManager();
	~SqlManager();
	bool InitializeCharacters(std::string, std::string, std::string, std::string, std::string);
    bool InitializeWorld(std::string, std::string, std::string, std::string, std::string);

    int32 GetIDLogin(std::string, std::string);
    Player* GetNewPlayer(uint32);
    WorldPosition GetRespawnPositionForPlayer(uint32);
    void SavePlayer(Player const*);
    bool InitializeCreatureTemplate(CreatureManager*);

private:
	MYSQL m_MysqlCharacters;
    MYSQL m_MysqlWorld;
};

