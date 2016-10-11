#pragma once
#include "Unit.hpp"

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

    void Respawn();
private:
    WorldSocket* m_Session;

    uint8 m_Alignment;
    uint32 m_Xp;
    bool m_Initilize;
};

