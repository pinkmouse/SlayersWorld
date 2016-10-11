#pragma once

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <mysql.h>
#include <string>
#include "../Define.hpp"
#include "../Entities/Player.hpp"
#include "../System/WorldPosition.hpp"
#include "../Map/MapManager.hpp"
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
    bool InitializeCreature(MapManager*, CreatureManager*);
    std::map<uint8, uint16> GetXpLevel();

private:
	MYSQL m_MysqlCharacters;
    MYSQL m_MysqlWorld;
};

