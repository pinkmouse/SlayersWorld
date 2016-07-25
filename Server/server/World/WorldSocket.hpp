#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include "WorldPacket.hpp"
#include "../Define.hpp"
#include "../Entities/Player.hpp"

class WorldSocket : public sf::TcpSocket
{
public:
	WorldSocket();
	~WorldSocket();

    void SendPlayerCreate(uint32, std::string, uint8, uint8, uint16, uint32, uint32, uint8);
    void SendPlayerStopMovement(uint32, uint32, uint32, uint8);
	void SendAuthResponse(uint8);
    void SendMsg(WorldPacket);

    Player* GetPlayer();
    void SetPlayer(Player*);

private:
    Player* m_Player;
};

