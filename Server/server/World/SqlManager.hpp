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
#include "UnitManager.hpp"

class SqlManager
{
public:
	SqlManager();
	~SqlManager();
	bool InitializeCharacters(std::string, std::string, std::string, std::string, std::string);
    bool InitializeWorld(std::string, std::string, std::string, std::string, std::string);

    int32 GetIDLogin(std::string, std::string);
    int32 GetIDCharacter(uint32);
    void AddNewAccount(std::string, std::string);
    bool IsExistingAccound(std::string);
    Player* GetNewPlayer(uint32);
    void AddNewPlayer(uint32);
    std::string GetLoginName(uint32);
    int16 GetLevel(const std::string &);
    WorldPosition GetPosition(const std::string &);
    WorldPosition GetRespawnPositionForPlayer(uint32);
    eAccessType GetAccessType(uint32);

    void AddNewRespawnPositionForPlayer(uint32);
	PointsSet GetPointsSetForPlayer(uint32);
	void AddNewPointsSetForPlayer(uint32);
    void SavePlayer(Player*);
	void UpdatePointsSet(Player const*);
    void AddConnectionLogin(uint32);

    bool InitializeSpells();
    bool InitializeSpellEffects();
    bool InitializeCreatureTemplate(UnitManager*);
    bool InitializeGossip(UnitManager*);
    bool InitializeSpellsForPlayer(Player*);
    CreatureTemplate GetCreatureTemplate(uint16);
    uint16 AddNewCreature(uint16, uint16, uint32, uint32);
    bool InitializeCreature(UnitManager*);
    std::map<uint8, uint16> GetXpLevel();
    int32 GetPlayerID(const std::string &);

private:
	MYSQL m_MysqlCharacters;
    MYSQL m_MysqlWorld;
};

