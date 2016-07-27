#include "../stdafx.h"
#include "WorldSocket.hpp"
#include "../Map/Map.hpp"

WorldSocket::WorldSocket()
{
    m_Player = nullptr;
}


WorldSocket::~WorldSocket()
{
    delete m_Player;
}

void WorldSocket::SendAuthResponse(uint8 p_Status)
{
	WorldPacket l_Packet;
	uint8 l_ID = 1;

	l_Packet << l_ID << p_Status;
	send(l_Packet);
	printf("Send Status Auth %d\n", p_Status);
}

void WorldSocket::SendPlayerCreate(uint32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, uint8 p_Orientation)
{
    WorldPacket l_Packet;
    uint8 l_ID = 10;

    l_Packet << l_ID << p_ID << p_Name << p_Level << p_SkinID << p_MapID << p_PosX << p_PosY << p_Orientation;
    send(l_Packet);
    printf("Send create\n");
}

void WorldSocket::SendPlayerCreateToSet(uint32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, uint8 p_Orientation)
{
    WorldPacket l_Packet;
    uint8 l_ID = 11;

    l_Packet << l_ID << p_ID << p_Name << p_Level << p_SkinID << p_MapID << p_PosX << p_PosY << p_Orientation;
    SendToSet(l_Packet, true);
    printf("Send create to square\n");
}

void WorldSocket::SendPlayerStopMovement(uint32 p_ID, uint32 p_PosX, uint32 p_PosY, uint8 p_Orientation)
{
    WorldPacket l_Packet;
    uint8 l_ID = 21;

    l_Packet << l_ID << p_ID << p_PosX << p_PosY << p_Orientation;
    send(l_Packet);
    printf("Send Stop Movement\n");
}

Player* WorldSocket::GetPlayer()
{
    return m_Player;
}

void WorldSocket::SetPlayer(Player* p_Player)
{
    m_Player = p_Player;
}

void WorldSocket::SendMsg(WorldPacket p_Packet)
{
    send(p_Packet);
}

void WorldSocket::SendToSet(WorldPacket l_Packet, bool p_ExcludePlayer /*= false*/)
{
    Map* l_Map = GetPlayer()->GetMap();

    if (l_Map == nullptr)
        return;

    std::vector<std::vector<Unit*>*> l_SquareSet = l_Map->GetSquareSet(GetPlayer()->GetSquareID());
    for (std::vector<std::vector<Unit*>*>::iterator l_It = l_SquareSet.begin(); l_It != l_SquareSet.end(); ++l_It)
    {
        for (std::vector<Unit*>::iterator l_It2 = (*l_It)->begin(); l_It2 != (*l_It)->end(); ++l_It2)
        {
            Unit* l_Unit = (*l_It2);

            if (l_Unit == nullptr)
                continue;

            if (l_Unit->GetType() != TypeUnit::PLAYER)
                continue;

            if (Player* l_Player = l_Unit->ToPlayer())
            {
                if (p_ExcludePlayer && l_Player == GetPlayer())
                    continue;

                l_Player->GetSession()->send(l_Packet);
            }
        }
    }
}