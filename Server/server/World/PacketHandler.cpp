#include "PacketHandler.hpp"
#include "PacketDefine.hpp"

PacketHandler::PacketHandler(SqlManager* p_SqlManager, MapManager* p_MapManager) :
    m_SqlManager(p_SqlManager),
    m_MapManager(p_MapManager)
{
}

PacketHandler::~PacketHandler()
{
}

void PacketHandler::LoadPacketHandlerMap()
{
    m_PacketHandleMap[CMSG::C_Connexion] = &PacketHandler::HandleConnexion;
    m_PacketHandleMap[CMSG::C_UnitCreate] = &PacketHandler::HandleUnitUnknow;
    m_PacketHandleMap[CMSG::C_UnitGoDirection] = &PacketHandler::HandleGoDirection;
    m_PacketHandleMap[CMSG::C_UnitStopMovement] = &PacketHandler::HandleStopMovement;
    m_PacketHandleMap[CMSG::C_UnitTalk] = &PacketHandler::HandleTalk;
    m_PacketHandleMap[CMSG::C_UnitStartAttack] = &PacketHandler::HandleStartAttack;
    m_PacketHandleMap[CMSG::C_UnitStopAttack] = &PacketHandler::HandleStopAttack;
}

void PacketHandler::HandleUnitUnknow(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    uint8 l_Type = 0;
    uint16 l_ID = 0;

    p_Packet >> l_Type;
    p_Packet >> l_ID;


    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    Unit* l_UniknowUnit = l_Player->GetMap()->GetUnit((TypeUnit)l_Type, l_ID);
    if (l_UniknowUnit == nullptr)
        return;

    p_WorldSocket->SendUnitCreate(l_UniknowUnit->GetType(), l_UniknowUnit->GetID(), l_UniknowUnit->GetName(), l_UniknowUnit->GetLevel(), l_UniknowUnit->GetSkinID(), l_UniknowUnit->GetMapID(), l_UniknowUnit->GetPosX(), l_UniknowUnit->GetPosY(), l_UniknowUnit->GetOrientation(), l_UniknowUnit->IsInMovement());
}

void PacketHandler::HandleGoDirection(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    uint8 l_Orientation = 0;
    Position l_Pos;

    l_Pos.x = 0;
    l_Pos.y = 0;

    p_Packet >> l_Orientation;
    p_Packet >> l_Pos.x;
    p_Packet >> l_Pos.y;

    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    l_Player->GetMovementHandler()->AddMovementToStack(eActionType::Go, l_Pos, (Orientation)l_Orientation);
    p_WorldSocket->SendUnitGoDirectionToSet((uint8)TypeUnit::PLAYER, l_Player->GetID(), l_Pos.x, l_Pos.y, l_Orientation);
}

void PacketHandler::HandleStopMovement(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Position l_Pos;

    l_Pos.x = 0;
    l_Pos.y = 0;


    p_Packet >> l_Pos.x;
    p_Packet >> l_Pos.y;

    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    l_Player->GetMovementHandler()->AddMovementToStack(eActionType::Stop, l_Pos, (Orientation)l_Player->GetOrientation());
    l_Player->GetSession()->SendUnitStopMovement((uint8)TypeUnit::PLAYER, l_Player->GetID(), l_Pos.x, l_Pos.y, l_Player->GetOrientation());
}

void PacketHandler::HandleTalk(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    std::string l_String;
    p_Packet >> l_String;

    l_Player->GetSession()->SendUnitTalk((uint8)TypeUnit::PLAYER, l_Player->GetID(), l_String);
}

void PacketHandler::HandleStartAttack(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Position l_Pos;

    l_Pos.x = 0;
    l_Pos.y = 0;


    p_Packet >> l_Pos.x;
    p_Packet >> l_Pos.y;

    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    l_Player->GetMovementHandler()->AddMovementToStack(eActionType::Attack, l_Pos, (Orientation)l_Player->GetOrientation());
    l_Player->GetSession()->SendUnitStartAttack((uint8)TypeUnit::PLAYER, l_Player->GetID(), l_Pos.x, l_Pos.y, l_Player->GetOrientation());
}

void PacketHandler::HandleStopAttack(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Player* l_Player = p_WorldSocket->GetPlayer();
    Position l_Pos;
    l_Pos.x = 0;
    l_Pos.y = 0;

    if (l_Player == nullptr)
        return;

    l_Player->GetMovementHandler()->AddMovementToStack(eActionType::StopAttack);
    l_Player->GetSession()->SendUnitStopAttack((uint8)TypeUnit::PLAYER, l_Player->GetID());
}

void PacketHandler::HandleDisconnected(WorldSocket* p_WorldSocket)
{
    if (m_SqlManager == nullptr)
        return;

    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    m_SqlManager->SavePlayer(l_Player);
}

void PacketHandler::HandleConnexion(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    if (m_SqlManager == nullptr)
        return;

    std::string l_Login;
    std::string l_Password;
    p_Packet >> l_Login;
    p_Packet >> l_Password;

    if (g_Config->IsPositiveValue("LoginDebug"))
    {
        p_WorldSocket->SendAuthResponse(1);
        return;
    }

    uint32 l_Id = m_SqlManager->GetIDLogin(l_Login, l_Password);
    if (!l_Id)
    {
        p_WorldSocket->SendAuthResponse(0); ///< Auth Failed
        return;
    }
    if (m_MapManager->IsOnline(TypeUnit::PLAYER, l_Id))
    {
        p_WorldSocket->SendAuthResponse(2); ///< Already connected
        return;
    }
    /// Auth Success
    p_WorldSocket->SendAuthResponse(1); ///< Auth Success

    /// Creation Player
    Player* l_Player = m_SqlManager->GetNewPlayer(l_Id);
    Map* l_Map = m_MapManager->GetMap(l_Player->GetMapID());

    if (l_Player == nullptr || l_Map == nullptr)
    {
        printf("Failed load player");
        if (l_Player != nullptr)
            printf(": Map = %d", l_Player->GetMapID());
        printf("\n");
        return;
    }
    l_Player->SetSession(p_WorldSocket);
    l_Player->SetMap(l_Map);
    printf("Load Player success\n");

    /// Send to Player
    p_WorldSocket->SendPlayerCreate(l_Player->GetID(), l_Player->GetName(), l_Player->GetLevel(), l_Player->GetHealth(), l_Player->GetAlignment(), l_Player->GetSkinID(), l_Player->GetMapID(), l_Player->GetPosX(), l_Player->GetPosY(), l_Player->GetOrientation());
    p_WorldSocket->SetPlayer(l_Player);
    l_Player->HasBeenInitialize();
    l_Player->GetMap()->AddUnit(l_Player);

    l_Player->UpdateNewSquares(0, l_Player->GetSquareID(), true);
}

void PacketHandler::OperatePacket(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    uint8 l_PacketID;
    p_Packet >> l_PacketID;
    printf("Receive Packet %d\n", l_PacketID);
    m_Func l_Fun = m_PacketHandleMap[l_PacketID];
    if (l_Fun != nullptr)
        (this->*(l_Fun))(p_Packet, p_WorldSocket);
    else
        printf("Packet %d Unknow\n", l_PacketID);

}
