#include "Socket.hpp"

#include "../Define.hpp"
#include <SFML/Network/IpAddress.hpp>
#include "../World/WorldPacket.hpp"
#include "../World/PacketDefine.hpp"


Socket::Socket()
{
}


Socket::~Socket()
{
}

bool Socket::Connection()
{
	sf::Socket::Status l_Status = this->connect(sf::IpAddress(IP_SERVER), PORT_SERVER);

	if (l_Status == sf::Socket::Status::Done)
		return true;

	return false;
}

void Socket::SendAuth(const std::string & l_Login, const std::string & l_Password)
{
	WorldPacket packet;
	uint8 l_ID = CMSG::C_Connexion;

	packet << l_ID << l_Login << l_Password;

	send(packet);
}

void Socket::SendGoDirection(const uint8& p_Orientation, const uint32& p_PosX, const uint32& p_PosY)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitGoDirection;

    packet << l_ID << p_Orientation << p_PosX << p_PosY;

    send(packet);
}

void Socket::SendStopMovement(const uint32& p_Posx, const uint32& p_PosY)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitStopMovement;

    packet << l_ID << p_Posx << p_PosY;

    send(packet);
}

void Socket::SendStartAttack(const uint32& p_Posx, const uint32& p_PosY)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitStartAttack;

    packet << l_ID << p_Posx << p_PosY;

    send(packet);
}

void Socket::SendStopAttack()
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitStopAttack;

    packet << l_ID;

    send(packet);
}

void Socket::SendUnitUnknow(const uint8& p_TypeID, const uint16& p_ID)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitCreate;

    packet << l_ID << p_TypeID << p_ID;

    send(packet);
}

void Socket::SendTalk(const std::string & p_TalkString)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitTalk;

    packet << l_ID << p_TalkString;

    send(packet);
}