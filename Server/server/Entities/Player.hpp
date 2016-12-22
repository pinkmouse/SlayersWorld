#pragma once
#include "Unit.hpp"
#include <map>

class WorldSocket;

class Player : public Unit
{
public:
    Player(int32, std::string, uint8, uint8, uint8, uint16, uint32, uint32, Orientation, uint32);
    ~Player();
    WorldSocket* GetSession() const;
    void SetSession(WorldSocket*);

    void SetAlignment(const uint8 &);
    uint8 GetAlignment() const;
    uint32 GetXp() const;

    void Update(sf::Time);
    void UpdateNewSquares(uint16, uint16, bool p_UpdateAll = false);
    void SetHealth(const uint8 &);
    void SetXp(uint32);
    void HasBeenInitialize();
    bool CheckCommand(const std::string &);

    void Respawn();
    void SendMsg(const std::string &);

    /// Commands
    void InitializeCommands();
    bool HandleCommandSkin(std::vector<std::string>);
    bool HandleCommandWhere(std::vector<std::string>);
    bool HandleCommandLevel(std::vector<std::string>);
	bool HandleCommandAddPoint(std::vector<std::string>);
    bool HandleCommandCreature(std::vector<std::string>);

private:
    WorldSocket* m_Session;

    uint8 m_Alignment;
    uint32 m_Xp;
    bool m_Initilize;

    typedef bool(Player::*m_Func)(std::vector<std::string>);
    std::map<std::string, m_Func> m_CmdHandleMap;
};

