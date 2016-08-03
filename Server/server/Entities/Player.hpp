#pragma once
#include "Unit.hpp"

class WorldSocket;

class Player : public Unit
{
public:
    Player(int32, std::string, uint8, uint8, uint16, uint32, uint32, Orientation);
    ~Player();
    WorldSocket* GetSession() const;
    void SetSession(WorldSocket*);

    void Update(sf::Time);
    void UpdateNewSquares(uint16, uint16, bool p_UpdateAll = false);

private:
    WorldSocket* m_Session;
};

