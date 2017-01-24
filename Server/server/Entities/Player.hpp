#pragma once
#include "Unit.hpp"
#include <map>

class WorldSocket;

class Player : public Unit
{
public:
    Player(int32, std::string, uint8, uint8, uint8, uint16, uint32, uint32, Orientation, uint32, eAccessType);
    ~Player();
    WorldSocket* GetSession() const;
    void SetSession(WorldSocket*);

    void SetAlignment(const uint8 &);
    uint8 GetAlignment() const;
    uint32 GetXp() const;
    eAccessType GetAccessType() const;

    void Update(sf::Time);
    void UpdateNewSquares(uint16, uint16, bool p_UpdateAll = false);
    void SetXp(uint32);
    void HasBeenInitialize();
    bool CheckCommand(const std::string &);
    void EventAction();

    void SetResourceNb(eResourceType, uint8);
    void Respawn();
    void SendMsg(const std::string &);
    void Save();

    /// Commands
    void InitializeCommands();
    bool HandleCommandSkin(std::vector<std::string>);
    bool HandleCommandWhere(std::vector<std::string>);
    bool HandleCommandLevel(std::vector<std::string>);
	bool HandleCommandAddPoint(std::vector<std::string>);
    bool HandleCommandCreature(std::vector<std::string>);
    bool HandleCommandWho(std::vector<std::string>);

private:
    WorldSocket* m_Session;

    uint8 m_Alignment;
    uint32 m_Xp;
    bool m_Initilize;
    eAccessType m_AccessType;

    typedef bool(Player::*m_Func)(std::vector<std::string>);
    std::map < std::string, std::pair < eAccessType,  m_Func > > m_CmdHandleMap;
};

