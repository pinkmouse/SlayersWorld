#pragma once
#include "Unit.hpp"

class WorldSocket;

class Player : public Unit
{
public:
    Player(int32, std::string, uint8, uint8, uint8, uint16, uint32, uint32, Orientation);
    ~Player();
    WorldSocket* GetSession() const;
    void SetSession(WorldSocket*);

    void SetAlignment(const uint8 &);
    uint8 GetAlignment() const;

    void Update(sf::Time);
    void UpdateNewSquares(uint16, uint16, bool p_UpdateAll = false);
    void SetHealth(const uint8 &);
    void HasBeenInitialize();

    void Respawn();
private:
    WorldSocket* m_Session;

    uint8 m_Alignment;
    bool m_Initilize;
};

