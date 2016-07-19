#pragma once
#include "Unit.hpp"
#include "../World/WorldSocket.hpp"

class Player : public Unit
{
public:
    Player(int32, std::string, uint8, uint8, uint16, uint32, uint32);
    ~Player();
    uint32 GetID() const;
    WorldSocket* GetSession() const;
    void SetSession(WorldSocket*);

private:
    uint32 m_ID;
    WorldSocket* m_Session;
};

