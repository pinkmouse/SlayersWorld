#include "Socket.hpp"

#include "../Define.hpp"
#include <Nazara/Network/IpAddress.hpp>
#include <Nazara/Network/NetPacket.hpp>
#include "../World/PacketDefine.hpp"


Socket::Socket()
{
}


Socket::~Socket()
{
}

bool Socket::Connection()
{
	Nz::IpAddress serverIp(IP_SERVER);
	serverIp.SetPort(PORT_SERVER);

	Connect(serverIp);

	return WaitForConnected();
}

void Socket::SendAuth(const std::string & l_Login, const std::string & l_Password)
{
	Nz::NetPacket packet(CMSG::C_Connexion);

	packet <<  l_Login << l_Password;

	SendPacket(packet);
}

void Socket::SendGoDirection(const uint8& p_Orientation, const uint32& p_PosX, const uint32& p_PosY)
{
    Nz::NetPacket packet(CMSG::C_UnitGoDirection);

    packet << p_Orientation << p_PosX << p_PosY;

	SendPacket(packet);
}

void Socket::SendStopMovement(const uint32& p_Posx, const uint32& p_PosY)
{
    Nz::NetPacket packet(CMSG::C_UnitStopMovement);

    packet << p_Posx << p_PosY;

	SendPacket(packet);
}

void Socket::SendStartAttack(const uint32& p_Posx, const uint32& p_PosY)
{
    Nz::NetPacket packet(CMSG::C_UnitStartAttack);

    packet << p_Posx << p_PosY;

	SendPacket(packet);
}

void Socket::SendStopAttack()
{
    Nz::NetPacket packet(CMSG::C_UnitStopAttack);

	SendPacket(packet);
}

void Socket::SendUnitUnknow(const uint8& p_TypeID, const uint16& p_ID)
{
	Nz::NetPacket packet(CMSG::C_UnitCreate);

    packet << p_TypeID << p_ID;

	SendPacket(packet);
}

void Socket::SendTalk(const std::string & p_TalkString)
{
    Nz::NetPacket packet(CMSG::C_UnitTalk);

    packet << p_TalkString;

	SendPacket(packet);
}