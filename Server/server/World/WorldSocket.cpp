#include "../stdafx.h"
#include "WorldSocket.hpp"
#include "../Map/Map.hpp"

WorldSocket::WorldSocket()
{
    m_Player = nullptr;
}


WorldSocket::~WorldSocket()
{
    if (m_Player == nullptr)
        return;

    WorldPacket l_Packet;
    uint8 l_ID = 12;
    uint8 l_Type = (uint8)TypeUnit::PLAYER;

    l_Packet << l_ID << l_Type << m_Player->GetID();
    SendToSet(l_Packet, true);
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

void WorldSocket::SendUpdatePosition(uint8 p_Type, uint16 p_ID, uint32 p_PosX, uint32 p_PosY)
{
    WorldPacket l_Packet;
    uint8 l_ID = 22;

    l_Packet << l_ID << p_Type << p_ID << p_PosX << p_PosY;
    send(l_Packet);
}

void WorldSocket::SendPlayerCreate(uint32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, uint8 p_Orientation)
{
    WorldPacket l_Packet;
    uint8 l_ID = 10;

    l_Packet << l_ID << p_ID << p_Name << p_Level << p_SkinID << p_MapID << p_PosX << p_PosY << p_Orientation;
    send(l_Packet);
    printf("Send create\n");
}

void WorldSocket::SendUnitCreateToSet(uint8 p_Type, uint32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, uint8 p_Orientation)
{
    WorldPacket l_Packet;
    uint8 l_ID = 11;

    l_Packet << l_ID << p_Type << p_ID << p_Name << p_Level << p_SkinID << p_MapID << p_PosX << p_PosY << p_Orientation;
    SendToSet(l_Packet, true);
    printf("Send create to square\n");
}

void WorldSocket::SendUnitCreate(uint8 p_Type, uint32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, uint8 p_Orientation)
{
    WorldPacket l_Packet;
    uint8 l_ID = 11;

    if (p_Type == TypeUnit::PLAYER && p_ID == GetPlayer()->GetID())
        return;

    l_Packet << l_ID << p_Type << p_ID << p_Name << p_Level << p_SkinID << p_MapID << p_PosX << p_PosY << p_Orientation;
    send(l_Packet);
    printf("Send create to unit\n");
}

void WorldSocket::SendUnitGoDirationToSet(uint8 p_Type, uint16 p_UnitID, uint8 p_Direction)
{
    WorldPacket l_Packet;
    uint8 l_ID = 20;

    l_Packet << l_ID << p_Type << p_UnitID << p_Direction;
    SendToSet(l_Packet, true);
}

void WorldSocket::SendUnitStopMovement(uint8 p_TypeID, uint16 p_ID, uint32 p_PosX, uint32 p_PosY, uint8 p_Orientation)
{
    WorldPacket l_Packet;
    uint8 l_ID = 21;

    l_Packet << l_ID << p_TypeID << p_ID << p_PosX << p_PosY << p_Orientation;
    SendToSet(l_Packet, true);
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

void WorldSocket::SendToSet(WorldPacket p_Packet, bool p_ExcludePlayer /*= false*/)
{
    Map* l_Map = GetPlayer()->GetMap();

    if (l_Map == nullptr)
        return;

    std::vector<Square*> l_SquareSet = l_Map->GetSquareSet(GetPlayer()->GetSquareID());
    for (std::vector<Square*>::iterator l_It = l_SquareSet.begin(); l_It != l_SquareSet.end(); ++l_It)
    {
        std::vector<Unit*>* l_Square = (*l_It)->GetList();
        if (l_Square == nullptr)
            continue;

        for (std::vector<Unit*>::iterator l_It2 = l_Square->begin(); l_It2 != l_Square->end(); ++l_It2)
        {
            Unit* l_Unit = (*l_It2);

            if (l_Unit == nullptr)
                continue;

            if (l_Unit->GetType() != TypeUnit::PLAYER)
                continue;

            if (Player* l_Player = l_Unit->ToPlayer())
            {
                if (p_ExcludePlayer && l_Player->GetID() == GetPlayer()->GetID())
                    continue;

                printf("Send to %s\n", l_Player->GetName().c_str());
                l_Player->GetSession()->send(p_Packet);
            }
        }
    }
}
