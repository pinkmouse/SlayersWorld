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
#include "../Entities/DynamicObjectManager.hpp"
#include "../System/Required/RequiredManager.hpp"
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
    bool IsAccountBan(const uint32 &);
    bool IsIPBan(const std::string &);
    void AddNewPlayer(uint32);
    void AddKeyDefaultBindsForAccount(uint32);
    void AddSpellBind(Player*, uint16, uint8);
    void RemoveSpellBind(Player*, uint16);
    void AddSpellForPlayer(Player*, uint16);
    void RemoveSpellForPlayer(Player*, uint16);
    void ReplaceKeyBindsForAccount(uint32, eKeyBoardAction, uint8);
    std::string GetLoginName(uint32);
    int16 GetLevel(const std::string &);
    WorldPosition GetPosition(const std::string &);
    WorldPosition GetRespawnPositionForPlayer(uint32);
    eAccessType GetAccessType(uint32);

    void AddNewSkinForPlayer(const uint32 &, const uint16 &);
    void AddNewTitleForPlayer(const uint32 &, const uint16 &);
    void AddNewItemForPlayer(const uint32 &, const uint8 &, const uint16 &, const uint8 &);
    void UpdateItemStackForPlayer(const uint32 &, const uint8 &, const uint16 &, const uint8 &);
    void UpdateItemSlotForPlayer(const uint32 &, const uint8 &, const uint8 &);
    void UpdateCurrencyForPlayer(const uint32 &, const eTypeCurrency &, const uint16 &);
    void AddNewEquipmentForPlayer(const uint32 &, const eTypeEquipment &, const uint16 &);
    void RemoveEquipmentForPlayer(const uint32 &, const eTypeEquipment &);
    void RemoveItemForPlayer(const uint32 &, const uint8 &);
    void AddNewRespawnPositionForPlayer(uint32);
	PointsSet GetPointsSetForPlayer(uint32);
	void AddNewPointsSetForPlayer(uint32);
    void SavePlayer(Player*);
    void BlackListIp(const std::string &, const uint32 &, const uint32 &, const std::string &);
    void BlackListAccount(const uint32 &, const uint32 &, const uint32 &, const std::string &);
    void SaveQuestForPlayer(Player const*, Quest const*);
	void UpdatePointsSet(Player const*);
    void AddConnectionLogin(uint32);
    int32 GetDaysSinceLastQuestDone(Player const*,  uint16);

    bool InitializeSpells();
    bool InitializeSpellEffects();
    bool InitializeCreatureTemplate(UnitManager*);
    bool InitializeAnimationUnitTemplate(UnitManager*);
    bool InitializeRequired(RequiredManager*);
    bool InitializeGossip(UnitManager*, RequiredManager*);
    bool InitializeSpellsForPlayer(Player*);
    void InitializeListTitlesForPlayer(Player*);
    void InitializeListSkinsForPlayer(Player*);
    void InitializeListItemForPlayer(Player*);
    void InitializeListEquipmentsForPlayer(Player*);
    void InitializeListCurrenciesForPlayer(Player*);
    bool InitializeBattlegrounds();
    bool InitializeQuestsProgessForPlayer(Player*);
    bool InitializeKeyBindsForAccount(uint32, Player*);
    bool InitializeSpellsBinds(Player*);
    bool InitializeQuests();
    bool InitializeTitles();
    bool InitializeItems(RequiredManager*);
    bool InitializeSkins();
    bool InitializeMaps();
    bool InitializeZones();
    bool InitializeAreatrigger(DynamicObjectManager*, UnitManager*);
    bool InitializeGameObject(DynamicObjectManager*, RequiredManager*, UnitManager*);
    CreatureTemplate GetCreatureTemplate(uint16);
    uint16 AddNewCreature(uint16, uint16, uint32, uint32);
    bool InitializeCreature(UnitManager*);
    bool InitializeAnimationUnit(UnitManager*);
    std::map<uint8, uint16> GetXpLevel();
    int32 GetPlayerID(const std::string &);

private:
	MYSQL m_MysqlCharacters;
    MYSQL m_MysqlWorld;
};

