#include "PacketHandler.hpp"
#include "PacketDefine.hpp"
#include "../Global.hpp"

PacketHandler::PacketHandler()
{
}

PacketHandler::~PacketHandler()
{
}

void PacketHandler::LoadPacketHandlerMap()
{
    m_PacketHandleMap[CMSG::C_Connexion] = &PacketHandler::HandleConnexion;
    m_PacketHandleMap[CMSG::C_Save] = &PacketHandler::HandleSave;
    m_PacketHandleMap[CMSG::C_StatAction] = &PacketHandler::HandleStatAction;
    m_PacketHandleMap[CMSG::C_UnitCreate] = &PacketHandler::HandleUnitUnknow;
    m_PacketHandleMap[CMSG::C_UnitGoDirection] = &PacketHandler::HandleGoDirection;
    m_PacketHandleMap[CMSG::C_UnitStopMovement] = &PacketHandler::HandleStopMovement;
    m_PacketHandleMap[CMSG::C_UnitTalk] = &PacketHandler::HandleTalk;
    m_PacketHandleMap[CMSG::C_UnitStartAttack] = &PacketHandler::HandleStartAttack;
    m_PacketHandleMap[CMSG::C_UnitEventAction] = &PacketHandler::HandleEventAction;
    m_PacketHandleMap[CMSG::C_UnitStopAttack] = &PacketHandler::HandleStopAttack;
    m_PacketHandleMap[CMSG::C_LoadingPong] = &PacketHandler::HandleLoadingPong;
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

    p_WorldSocket->SendUnitCreate(l_UniknowUnit->GetType(), l_UniknowUnit->GetID(), l_UniknowUnit->GetName(), l_UniknowUnit->GetLevel(), l_UniknowUnit->GetResourceNb(eResourceType::Health), l_UniknowUnit->GetResourceNb(eResourceType::Mana), l_UniknowUnit->GetResourceNb(eResourceType::Alignment),  l_UniknowUnit->GetSkinID(), l_UniknowUnit->GetSizeX(), l_UniknowUnit->GetSizeY(), l_UniknowUnit->GetSpeedUint8(), l_UniknowUnit->GetMapID(), l_UniknowUnit->GetPosition(), l_UniknowUnit->GetOrientation(), l_UniknowUnit->IsInMovement(), l_UniknowUnit->GetMovementHandler()->IsInAttack(), l_UniknowUnit->IsBlocking(), l_UniknowUnit->IsInGroupWith(l_Player));
}

void PacketHandler::HandleGoDirection(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    uint8 l_Orientation = 0;
    Position l_Pos(0, 0);

    p_Packet >> l_Orientation;
    p_Packet >> l_Pos.m_X;
    p_Packet >> l_Pos.m_Y;

    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    l_Player->GetMovementHandler()->AddMovementToStack(eActionType::Go, l_Pos, (Orientation)l_Orientation);
    p_WorldSocket->SendUnitGoDirectionToSet((uint8)TypeUnit::PLAYER, l_Player->GetID(), l_Pos, l_Orientation);
}

void PacketHandler::HandleStopMovement(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Position l_Pos(0, 0);

    p_Packet >> l_Pos.m_X;
    p_Packet >> l_Pos.m_Y;

    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    l_Player->GetMovementHandler()->AddMovementToStack(eActionType::Stop, l_Pos, (Orientation)l_Player->GetOrientation());
    l_Player->GetSession()->SendUnitStopMovement((uint8)TypeUnit::PLAYER, l_Player->GetID(), l_Pos, l_Player->GetOrientation());
}

void PacketHandler::HandleSave(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    l_Player->Save();
    PacketWarningMsg l_Packet;
    l_Packet.BuildPacket(eTypeWarningMsg::Yellow, "Sauvegarde effectué");
    l_Player->GetSession()->send(l_Packet.m_Packet);
}

void PacketHandler::HandleTalk(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    std::string l_String;
    p_Packet >> l_String;

    if (l_Player->CheckCommand(l_String))
        return;

    l_Player->GetSession()->SendUnitTalk((uint8)TypeUnit::PLAYER, l_Player->GetID(), l_String);
}

void PacketHandler::HandleLoadingPong(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    l_Player->SetInLoading(false);
}

void PacketHandler::HandleStartAttack(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Position l_Pos(0, 0);

    p_Packet >> l_Pos.m_X;
    p_Packet >> l_Pos.m_Y;

    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    l_Player->GetMovementHandler()->AddMovementToStack(eActionType::Attack, l_Pos, (Orientation)l_Player->GetOrientation());
    l_Player->GetSession()->SendUnitStartAttack((uint8)TypeUnit::PLAYER, l_Player->GetID(), l_Pos, l_Player->GetOrientation());
}

void PacketHandler::HandleStopAttack(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    l_Player->GetMovementHandler()->AddMovementToStack(eActionType::StopAttack);
    l_Player->GetSession()->SendUnitStopAttack((uint8)TypeUnit::PLAYER, l_Player->GetID());
}

void PacketHandler::HandleEventAction(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Player* l_Player = p_WorldSocket->GetPlayer();
    uint8 l_ActionID;

    p_Packet >> l_ActionID;

    if (l_Player == nullptr)
        return;

    l_Player->EventAction((eKeyBoardAction)l_ActionID);
}

void PacketHandler::HandleStatAction(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    Player* l_Player = p_WorldSocket->GetPlayer();
    uint8 l_TypeStat;
    bool l_Add;
    uint8 l_Nb;

    p_Packet >> l_TypeStat;
    p_Packet >> l_Add;
    p_Packet >> l_Nb;

    if (l_Player == nullptr)
        return;

    if (l_Add)
        l_Player->AddPointsStat((eStats)l_TypeStat, l_Nb);
    else
        l_Player->SubPointsStat((eStats)l_TypeStat, l_Nb);
}

void PacketHandler::HandleDisconnected(WorldSocket* p_WorldSocket)
{
    if (g_SqlManager == nullptr)
        return;

    Player* l_Player = p_WorldSocket->GetPlayer();

    if (l_Player == nullptr)
        return;

    l_Player->Save();
}

void PacketHandler::HandleConnexion(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    if (g_SqlManager == nullptr)
        return;

    std::string l_Login;
    std::string l_Password;
    p_Packet >> l_Login;
    p_Packet >> l_Password;

    int32 l_Id = g_SqlManager->GetIDLogin(l_Login, l_Password);

    if (l_Id < 0)
    {
        /*if (!g_SqlManager->IsExistingAccound(l_Login) /* && g_Config->IsPositiveValue("LoginDebug"))
        {
            g_SqlManager->AddNewAccount(l_Login, l_Password);
            WorldPacket l_Packet;
            l_Packet << l_Login;
            l_Packet << l_Password;
            HandleConnexion(l_Packet, p_WorldSocket);
        }
        else*/
            p_WorldSocket->SendAuthResponse(0); ///< Auth Failed
            printf("Auth failed\n");
        return;
    }
    int32 l_IdCharacter = g_SqlManager->GetIDCharacter(l_Id);

    if (l_IdCharacter >= 0)
    {
        if (g_MapManager->IsOnline(TypeUnit::PLAYER, l_IdCharacter))
        {
            p_WorldSocket->SendAuthResponse(2); ///< Already connected
            return;
        }
    }

    /// Keep Log of Connection
    g_SqlManager->AddConnectionLogin(l_Id);

    /// Creation Player
    Player* l_Player = g_SqlManager->GetNewPlayer(l_Id);
    if (l_Player == nullptr)
    {
        p_WorldSocket->SendAuthResponse(3); ///< Auth Success
        return;
    }

    /// Auth Success
    p_WorldSocket->SendAuthResponse(1); ///< Auth Success

    Map* l_Map = g_MapManager->GetMap(l_Player->GetMapID(), l_Player->GetInstanceID());

    if (l_Map == nullptr || g_MapManager->GetMapTemplate(l_Player->GetMapID())->IsInstance())
    {
        l_Player->SetOrientation(l_Player->GetRespawnPoint().GetOrientation());
        l_Player->SetMapID(l_Player->GetRespawnPoint().GetMapID());
        l_Player->SetPos(l_Player->GetRespawnPoint().GetPosX(), l_Player->GetRespawnPoint().GetPosY());
        l_Player->SetInstanceID(l_Player->GetRespawnPoint().GetInstanceID());
        l_Map = g_MapManager->GetMap(l_Player->GetMapID(), l_Player->GetInstanceID());
        if (l_Map == nullptr)
        {
            printf("Failed load map %d\n", l_Player->GetMapID());
            return;
        }
    }

    l_Player->SetSession(p_WorldSocket);
    printf("Load Player success\n");

    /// Send to Player
    p_WorldSocket->SendPlayerCreate(l_Player->GetID(), l_Player->GetName(), l_Player->GetLevel(), l_Player->GetResourceNb(eResourceType::Health), l_Player->GetResourceNb(eResourceType::Mana), l_Player->GetResourceNb(eResourceType::Alignment), l_Player->GetSkinID(), l_Player->GetMapID(), g_MapManager->GetMapTemplate(l_Player->GetMapID())->GetFileName(), g_MapManager->GetMapTemplate(l_Player->GetMapID())->GetFileChipset(), g_MapManager->GetMapTemplate(l_Player->GetMapID())->GetName(), l_Player->GetPosX(), l_Player->GetPosY(), l_Player->GetOrientation());
    p_WorldSocket->SendUpdateXpPct(g_LevelManager->XpPct(l_Player->GetLevel(), l_Player->GetXp()));

    /// Trick to send stats to player
    l_Player->SetPointsSet(g_SqlManager->GetPointsSetForPlayer(l_Player->GetID()));
    l_Player->SetLevel(l_Player->GetLevel());

    /// Send KeyBoard Binds
    for (std::map< eKeyBoardAction, uint8 >::iterator l_It = l_Player->GetKeyBoardBinds()->begin(); l_It != l_Player->GetKeyBoardBinds()->end(); ++l_It)
        p_WorldSocket->SendKeyBoardBind((*l_It).first, (*l_It).second);

    p_WorldSocket->SetPlayer(l_Player);

    printf("Player pos=%d-%d, %d, %d\n", l_Player->GetPosX(), l_Player->GetPosY(), l_Player->GetSizeX(), (l_Player->GetPosY() / TILE_SIZE) * (uint32)l_Player->GetSizeX()) + (l_Player->GetPosX() / TILE_SIZE);
    l_Map->AddUnit(l_Player);
    l_Player->HasBeenInitialize();

    l_Player->SendMsg("- Bienvenue sur SlayersWorld V0.1 -");
}

void PacketHandler::OperatePacket(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    uint8 l_PacketID;
    p_Packet >> l_PacketID;

    if (p_WorldSocket->GetPlayer() && p_WorldSocket->GetPlayer()->GetInLoading() && l_PacketID != CMSG::C_LoadingPong) ///< We should not conciderate packet while loading
        return;
    
    Log("Receive Packet " +  std::to_string(l_PacketID));
    m_Func l_Fun = m_PacketHandleMap[l_PacketID];
    if (l_Fun != nullptr)
        (this->*(l_Fun))(p_Packet, p_WorldSocket);
    else
        Log("Packet " + std::to_string(l_PacketID) + " unknow");

}
