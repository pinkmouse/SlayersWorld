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

    void SendPlayerCreate(uint32, std::string, uint8, uint8, uint8, uint8, uint8, uint16, uint32, uint32, uint8);
    void SendUnitCreateToSet(uint8, uint32, std::string, uint8, uint8, uint8, uint8, uint8, uint16, uint32, uint32, uint8, bool, bool);
    void SendUnitCreate(uint8, uint32, std::string, uint8, uint8, uint8, uint8, uint8, uint16, uint32, uint32, uint8, bool, bool);
    void SendUnitGoDirectionToSet(uint8, uint16, const Position &, uint8);
    void SendUnitStopMovement(uint8, uint16, const Position &, uint8);
    void SendUnitStartAttack(uint8, uint16, const Position &, uint8);
    void SendUnitStopAttack(uint8, uint16);
    void SendUpdatePosition(uint8, uint16, uint32, uint32, uint8);
    void SendUpdatePositionToSet(uint8, uint16, uint32, uint32, uint8);
    void SendUpdateXpPct(float);
    void SendUpdateUnitResource(uint8, uint16, uint8, uint8);
    void SendUnitTalk(uint8, uint16, const std::string &);
	void SendLogDamage(uint8, uint16, uint8, bool);
	void SendAuthResponse(uint8);
    void SendMsg(WorldPacket);
    void SendToSet(WorldPacket, bool p_ExcludePlayer = false);

    Player* GetPlayer();
    void SetPlayer(Player*);

private:
    Player* m_Player;
};

