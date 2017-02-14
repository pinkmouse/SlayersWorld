#pragma once
#include  <SFML/Window/Keyboard.hpp>
#include "../System/Quest/Quest.hpp"
#include "Unit.hpp"
#include <map>

class WorldSocket;

class Player : public Unit
{
public:
    Player(uint32, int32, std::string, uint8, uint8, uint8, uint8, uint8, uint16, uint32, uint32, Orientation, uint32, eAccessType);
    ~Player();

    /* BASIC */
    WorldSocket* GetSession() const;
    void SetSession(WorldSocket*);
    uint32 GetAccountID() const;
    eAccessType GetAccessType() const;
    void HasBeenInitialize();
    void Respawn();
    void SendMsg(const std::string &);
    void Save();

    /* RESOURCES */
    uint8 GetAlignment() const;
    void SetAlignment(const uint8 &);
    uint32 GetXp() const;
    void SetXp(uint32);
    void SetResourceNb(eResourceType, uint8);
    void AddResourceNb(eResourceType, uint8);

    /* UPDATE */
    void Update(sf::Time);
    void UpdateNewSquares(uint16, uint16, bool p_UpdateAll = false);

    /* QUEST */
    void UpdateQuests();
    void AddQuest(Quest*);
    void CheckQuestObjective(eObjectifType, int32);

    /* KEYBOARD */
    void AddKeyBoardBind(eKeyBoardAction, uint8);
    int16 GetKeyBoardBind(eKeyBoardAction);
    void AddSpellBindToKey(uint16, uint8);
    int32 GetBindSpell(uint16);
    std::map< eKeyBoardAction, uint8 >* GetKeyBoardBinds();

    /* SPELL */
    void AddSpellCooldown(uint16, uint64);

    /* COMMANDS */
    bool CheckCommand(const std::string &);
    void EventAction(eKeyBoardAction);
    void InitializeCommands();
    bool HandleCommandSkin(std::vector<std::string>);
    bool HandleCommandWhere(std::vector<std::string>);
    bool HandleCommandLevel(std::vector<std::string>);
    bool HandleCommandWisp(std::vector<std::string>);
	bool HandleCommandAddPoint(std::vector<std::string>);
    bool HandleSave(std::vector<std::string>);
    bool HandleCommandCreature(std::vector<std::string>);
    bool HandleCommandWho(std::vector<std::string>);
    bool HandleCommandTeleport(std::vector<std::string>);
    bool HandleCommandSpeed(std::vector<std::string>);
    bool HandleRegen(std::vector<std::string>);
    bool HandleBind(std::vector<std::string>);
    bool HandleTest(std::vector<std::string>);

private:
    uint16 m_AccountID;
    WorldSocket* m_Session;

    uint8 m_Alignment;
    uint32 m_Xp;
    bool m_Initilize;
    eAccessType m_AccessType;

    std::map< uint16, Quest* > m_Quests;

    typedef bool(Player::*m_Func)(std::vector<std::string>);
    std::map < std::string, std::pair < eAccessType,  m_Func > > m_CmdHandleMap;
    std::map < eKeyBoardAction, uint8 > m_KeyBoardBinds;
    std::map < uint16, uint8 > m_SpellsBindToKey;
};

