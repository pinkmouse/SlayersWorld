#include "PacketHandler.hpp"


PacketHandler::PacketHandler()
{
}


PacketHandler::~PacketHandler()
{
}

void PacketHandler::LoadPacketHandlerMap()
{
	m_PacketHandleMap[1] = &PacketHandler::HandleConnexion;
}

void PacketHandler::HandleConnexion(WorldPacket &p_Packet, Socket* p_Socket)
{
	std::string l_Login;
	std::string l_Password;
	p_Packet >> l_Login;
	p_Packet >> l_Password;
	printf("Login: %s:%s\n", l_Login.c_str(), l_Password.c_str());
}



void PacketHandler::OperatePacket(WorldPacket &p_Packet, Socket* p_Socket)
{
	uint8 l_PacketID;
	p_Packet >> l_PacketID;
	printf("Receive Packet %d\n", l_PacketID);
	m_Func l_Fun = m_PacketHandleMap[l_PacketID];
	if (l_Fun != nullptr)
		(this->*(l_Fun))(p_Packet, p_Socket);
	else
		printf("Packet %d Unknow\n", l_PacketID);

}

