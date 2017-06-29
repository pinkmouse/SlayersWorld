#include "PacketHandler.hpp"
#include "../Entities/Player.hpp"
#include "../Entities/Creature.hpp"
#include "../Entities/DynamicObject.hpp"
#include "../Global.hpp"
#include "PacketDefine.hpp"

PacketHandler::PacketHandler(MapManager* p_MapManager, InterfaceManager* p_InterfaceManager) :
	m_MapManager(p_MapManager),
    m_InterfaceManager(p_InterfaceManager)
{
    p_HasMinimalRequiered = false;
}

PacketHandler::~PacketHandler()
{
}

void PacketHandler::LoadPacketHandlerMap()
{
	m_PacketHandleMap[SMSG::S_Connexion] = &PacketHandler::HandleConnexion;
    m_PacketHandleMap[SMSG::S_PlayerCreate] = &PacketHandler::HandleCreateMainPlayer;
    m_PacketHandleMap[SMSG::S_UnitCreate] = &PacketHandler::HandleCreateUnit;
    m_PacketHandleMap[SMSG::S_UnitRemove] = &PacketHandler::HandleRemoveUnit;
    m_PacketHandleMap[SMSG::S_SwitchMap] = &PacketHandler::HandleSwitchMap;
    m_PacketHandleMap[SMSG::S_UnitUpdateResource] = &PacketHandler::HandleUpdateResource;
    m_PacketHandleMap[SMSG::S_PlayerUpdateXp] = &PacketHandler::HandleUpdateXpPct;
    m_PacketHandleMap[SMSG::S_UnitGoDirection] = &PacketHandler::HandleUnitGoDirection;
    m_PacketHandleMap[SMSG::S_UnitStopMovement] = &PacketHandler::HandleStopMovement;
    m_PacketHandleMap[SMSG::S_UnitUpdatePosition] = &PacketHandler::HandleUpdatePosition;
    m_PacketHandleMap[SMSG::S_UnitUpdateOrientation] = &PacketHandler::HandleUpdateOrientation;
    m_PacketHandleMap[SMSG::S_UnitTalk] = &PacketHandler::HandleTalk;
    m_PacketHandleMap[SMSG::S_SrvPlayerMsg] = &PacketHandler::HandleSrvPlayerMsg;
    m_PacketHandleMap[SMSG::S_UnitStartAttack] = &PacketHandler::HandleUnitStartAttack;
    m_PacketHandleMap[SMSG::S_UnitStopAttack] = &PacketHandler::HandleUnitStopAttack;
    m_PacketHandleMap[SMSG::S_UnitUpdateSkin] = &PacketHandler::HandleUpdateSkin;
	m_PacketHandleMap[SMSG::S_LogDamage] = &PacketHandler::HandleLogDamage;
    m_PacketHandleMap[SMSG::S_WarningMsg] = &PacketHandler::HandleWarningMsg;
    m_PacketHandleMap[SMSG::S_UnitPlayVisual] = &PacketHandler::HandleUnitPlayVisual;
    m_PacketHandleMap[SMSG::S_UnitUpdateStat] = &PacketHandler::HandleUpdateStat;
    m_PacketHandleMap[SMSG::S_KeyBoardBind] = &PacketHandler::HandleKeyBoardBind;
    m_PacketHandleMap[SMSG::S_BlockBind] = &PacketHandler::HandleKeyBindBlock;
    m_PacketHandleMap[SMSG::S_CastBar] = &PacketHandler::HandleCastBar;
    m_PacketHandleMap[SMSG::S_LoadingPing] = &PacketHandler::HandleLoadingPing;
}

void PacketHandler::HandleRemoveUnit(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
            return;

        l_Map->RemoveUnit(l_Unit);

        delete l_Unit;
    }
}

void PacketHandler::HandleUpdateResource(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    uint8 l_Resource;
    uint8 l_NewNb;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_Resource;
    p_Packet >> l_NewNb;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = nullptr;

        l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);
        if (l_Unit == nullptr)
            return;

        l_Unit->SetResourceNb((eResourceType)l_Resource, l_NewNb);
    }
}

void PacketHandler::HandleUpdateXpPct(WorldPacket &p_Packet)
{
    float l_XpPct;

    p_Packet >> l_XpPct;
    g_Player->SetXpPct(l_XpPct);
}

void PacketHandler::HandleTalk(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    std::string l_String;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_String;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = nullptr;
       
        l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);
        if (l_Unit == nullptr)
            return;

        l_Unit->SetTalk(l_String);
        m_InterfaceManager->GetHistoryField()->AddHistoryLine(l_Unit->GetName() + ": " + l_String);
    }
}

void PacketHandler::HandleStopMovement(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    Position l_Pos;
    uint8 l_Orientation;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_Pos.x;
    p_Packet >> l_Pos.y;
    p_Packet >> l_Orientation;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }

        l_Unit->GetMovementHandler()->AddMovementToStack(eActionType::Stop, l_Pos, (Orientation)l_Unit->GetOrientation());
    }
}

void PacketHandler::HandleUnitStartAttack(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    Position l_Pos;
    uint8 l_Orientation;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_Pos.x;
    p_Packet >> l_Pos.y;
    p_Packet >> l_Orientation;

    printf("Attack for Type :%d, ID:%d, posX:%d, posY:%d, orientation:%d\n", l_TypeID, l_ID, l_Pos.x, l_Pos.y, l_Orientation);
    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }

        l_Unit->GetMovementHandler()->AddMovementToStack(eActionType::Attack, l_Pos, (Orientation)l_Orientation);
    }
}

void PacketHandler::HandleUnitStopAttack(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    Position l_Pos;
    l_Pos.x = 0;
    l_Pos.y = 0;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }

        l_Unit->GetMovementHandler()->AddMovementToStack(eActionType::StopAttack);
    }
}

void PacketHandler::HandleUpdateOrientation(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    uint8 l_Orientation;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_Orientation;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }

        l_Unit->SetOrientation((Orientation)l_Orientation);
    }
}

void PacketHandler::HandleUpdatePosition(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_UnitID;
    uint16 l_MapId;
    uint32 l_PosX;
    uint32 l_PosY;
    uint8 l_Orientation;

    p_Packet >> l_TypeID;
    p_Packet >> l_UnitID;
    p_Packet >> l_MapId;
    p_Packet >> l_PosX;
    p_Packet >> l_PosY;
    p_Packet >> l_Orientation;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_UnitID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_UnitID); ///< Ask for unknow unit to server
            return;
        }
        WorldPosition l_WorldPosition(l_PosX, l_PosY, l_MapId, (Orientation)l_Orientation);
        l_Unit->TeleportTo(l_WorldPosition);
    }
}

void PacketHandler::HandleUnitGoDirection(WorldPacket &p_Packet)
{
    uint8 l_Type;
    uint16 l_UnitID;
    uint8 l_Direction;
    Position l_Pos;

    p_Packet >> l_Type;
    p_Packet >> l_UnitID;
    p_Packet >> l_Pos.x;
    p_Packet >> l_Pos.y;
    p_Packet >> l_Direction;

    if (Map* l_Map = m_MapManager->GetActualMap())
        l_Map->MoveUnitToDirection((TypeUnit)l_Type, l_UnitID, l_Pos, l_Direction);
}

void PacketHandler::HandleConnexion(WorldPacket &p_Packet)
{
	uint8 l_Status;

	p_Packet >> l_Status;
	printf("Auth Status: %d\n", l_Status);

    switch (l_Status)
    {
    case 0:
        m_InterfaceManager->SetSystemMsg("Authentication failed");
        printf("Auth Failed\n");
        break;
    case 1:
        m_InterfaceManager->SetSystemMsg("Authentication success");
        printf("Auth Success\n");
        break;
    case 2:
        m_InterfaceManager->SetSystemMsg("Already connected");
        printf("Already connected\n");
        break;
    case 3:
        m_InterfaceManager->SetSystemMsg("Access denied");
        printf("Access denied\n");
        break;
    }
}

void PacketHandler::HandleCreateMainPlayer(WorldPacket &p_Packet)
{
    uint32 l_ID;
    std::string l_Name;
    uint8 l_Level;
    uint8 l_Health;
    uint8 l_Mana;
    uint8 l_Alignment;
    int16 l_SkinID;
    uint16 l_MapID;
    uint32 l_PosX;
    uint32 l_PosY;
    uint8 l_Orientation;

    p_Packet >> l_ID;
    p_Packet >> l_Name;
    p_Packet >> l_Level;
    p_Packet >> l_Health;
    p_Packet >> l_Mana;
    p_Packet >> l_Alignment;
    p_Packet >> l_SkinID;
    p_Packet >> l_MapID;
    p_Packet >> l_PosX;
    p_Packet >> l_PosY;
    p_Packet >> l_Orientation;

    if (!m_MapManager->LoadMap(l_MapID))
        return;

    g_Player = new Player(l_ID, l_Name, l_Level, l_Health, l_Mana, l_Alignment, l_SkinID, 24, 32, l_MapID, l_PosX, l_PosY, (Orientation)l_Orientation);
    m_MapManager->SetPosX(l_PosX);
    m_MapManager->SetPosY(l_PosY);

    if (Map* l_ActualMap = m_MapManager->GetActualMap())
    {
        g_Player->SetMap(l_ActualMap);
        l_ActualMap->AddUnit(g_Player);
        p_HasMinimalRequiered = true;
    }
    else
        delete g_Player;
}

void PacketHandler::HandleCreateUnit(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint32 l_ID;
    std::string l_Name;
    uint8 l_Level = 1;
    uint8 l_Health = 100;
    uint8 l_Mana = 100;
    uint8 l_Alignment = 100;
    int16 l_SkinID;
    uint8 l_SizeX;
    uint8 l_SizeY;
    uint8 l_Speed;
    uint16 l_MapID;
    Position l_Pos;
    uint8 l_Orientation;
    bool l_IsInMovement;
    bool l_IsInAttack = false;
    bool l_IsBlocking = false;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_Name;
    if (l_TypeID < 2) ///< Only Player and Creature
    {
        p_Packet >> l_Level;
        p_Packet >> l_Health;
        p_Packet >> l_Mana;
        p_Packet >> l_Alignment;
    }
    p_Packet >> l_SkinID;
    p_Packet >> l_SizeX;
    p_Packet >> l_SizeY;
    p_Packet >> l_Speed;
    p_Packet >> l_MapID;
    p_Packet >> l_Pos.x;
    p_Packet >> l_Pos.y;
    p_Packet >> l_Orientation;
    p_Packet >> l_IsInMovement;

    if (l_TypeID < 2) ///< Only Player and Creature
        p_Packet >> l_IsInAttack;
    else
        p_Packet >> l_IsBlocking;

    if (l_TypeID == (uint8)TypeUnit::PLAYER)
        printf("Create new Player: %d %s %d %d %d %d\n", l_ID, l_Name.c_str(), l_SkinID, l_MapID, l_Pos.x, l_Pos.y);
    else
        printf("Create new Unit: %d %s %d %d %d %d\n", l_ID, l_Name.c_str(), l_SkinID, l_MapID, l_Pos.x, l_Pos.y);

    Unit* l_NewUnit = nullptr;

    Map* l_ActualMap = m_MapManager->GetActualMap();

    if (l_ActualMap == nullptr)
        return;

    if (l_ActualMap->GetID() == l_MapID && l_ActualMap->GetUnit((TypeUnit)l_TypeID, l_ID) == nullptr)
    {
        if (l_TypeID == (uint8)TypeUnit::PLAYER)
            l_NewUnit = new Player(l_ID, l_Name, l_Level, l_Health, l_Mana, l_Alignment, l_SkinID, l_SizeX, l_SizeY, l_MapID, l_Pos.x, l_Pos.y, (Orientation)l_Orientation);
        else if (l_TypeID == (uint8)TypeUnit::CREATURE)
            l_NewUnit = new Creature(l_ID, l_Name, l_Level, l_Health, l_SkinID, l_SizeX, l_SizeY, l_MapID, l_Pos.x, l_Pos.y, (Orientation)l_Orientation);
        else if (l_TypeID == (uint8)TypeUnit::AREATRIGGER || l_TypeID == (uint8)TypeUnit::GAMEOBJECT)
        {
           /* DynamicObject* l_DynIbj*/l_NewUnit = new DynamicObject(l_ID, (TypeUnit)l_TypeID, l_Name, l_Level, l_Health, l_SkinID, l_SizeX, l_SizeY, l_MapID, l_Pos.x, l_Pos.y, (Orientation)l_Orientation, l_IsBlocking);
            VisualEffect l_VisualEffect(eVisualType::VisualGob, l_SkinID, 3);
            l_VisualEffect.StartAnim();
            l_NewUnit->AddVisualEffect(l_VisualEffect);

            l_ActualMap->GetCase(l_Pos.x, l_Pos.y - TILE_SIZE)->AddDynamicOject(l_NewUnit->ToDynamicObject());
        }

        float l_SpeedFloat = (float)l_Speed / 10.0f;
        l_NewUnit->SetSpeed(l_SpeedFloat);
        l_NewUnit->SetMap(l_ActualMap);
        l_ActualMap->AddUnit(l_NewUnit);
        //printf("Create new Unit DONE: %d %s %d %d %d %d\n", l_ID, l_Name.c_str(), l_SkinID, l_MapID, l_Pos.x, l_Pos.y);
        if (l_IsInMovement)
            l_NewUnit->GetMovementHandler()->AddMovementToStack(eActionType::Go, l_Pos, (Orientation)l_NewUnit->GetOrientation());
        if (l_IsInAttack)
            l_NewUnit->GetMovementHandler()->AddMovementToStack(eActionType::Attack, l_Pos, (Orientation)l_NewUnit->GetOrientation());
    }
}

void PacketHandler::OperatePacket(WorldPacket &p_Packet)
{
	uint8 l_PacketID;
	p_Packet >> l_PacketID;
	//printf("Receive Packet %d\n", l_PacketID);
	m_Func l_Fun = m_PacketHandleMap[l_PacketID];
	if (l_Fun != nullptr)
		(this->*(l_Fun))(p_Packet);
	else
		printf("Packet %d Unknow\n", l_PacketID);
}

bool PacketHandler::HasMinimalRequiered() const
{
    return p_HasMinimalRequiered;
}

void PacketHandler::HandleUpdateSkin(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    int16 l_Skin;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_Skin;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }

        l_Unit->SetSkinID(l_Skin);
    }
}

void PacketHandler::HandleSwitchMap(WorldPacket &p_Packet)
{
    uint16 l_MapID;

    p_Packet >> l_MapID;

    delete  m_MapManager->GetActualMap();
    if (!m_MapManager->LoadMap(l_MapID))
        return;

    if (Map* l_ActualMap = m_MapManager->GetActualMap())
    {
        g_Player->GetMovementHandler()->StopMovement();
        g_Player->GetMovementHandler()->StopAttack();
        g_Player->SetMap(l_ActualMap);
        l_ActualMap->AddUnit(g_Player);
    }
}

void PacketHandler::HandleSrvPlayerMsg(WorldPacket &p_Packet)
{
    std::string l_Msg;

    p_Packet >> l_Msg;
    
    m_InterfaceManager->GetHistoryField()->OpenTemporary(5000);
    m_InterfaceManager->GetHistoryField()->AddHistoryLine(l_Msg);
}

void PacketHandler::HandleLogDamage(WorldPacket &p_Packet)
{
	uint8 l_TypeID;
	uint16 l_ID;
	sf::Int8 l_Damage;
    uint8 l_DamageResult;

	p_Packet >> l_TypeID;
	p_Packet >> l_ID;
	p_Packet >> l_Damage;
	p_Packet >> l_DamageResult;

	if (Map* l_Map = m_MapManager->GetActualMap())
	{
		Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

		if (l_Unit == nullptr)
		{
			g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
			return;
		}
		l_Unit->AddDamageLog(DamageInfo(l_Damage, (DamageResult)l_DamageResult));
	}
}

void PacketHandler::HandleWarningMsg(WorldPacket &p_Packet)
{
    uint8 l_Type;
    std::string l_WarningMsg;

    p_Packet >> l_Type;
    p_Packet >> l_WarningMsg;

    if (l_Type == eTypeWarningMsg::Top)
        m_InterfaceManager->AddTopMsg(l_WarningMsg);
    else
        m_InterfaceManager->AddWarningMsg((eTypeWarningMsg)l_Type, l_WarningMsg);
}

void PacketHandler::HandleUnitPlayVisual(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    uint8 l_VisualID;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_VisualID;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }
        VisualEffect l_VisualEffect(eVisualType::VisualSpell, l_VisualID, 3);
        l_VisualEffect.StartAnimAndStop();
        l_Unit->AddVisualEffect(l_VisualEffect);
    }
}

void PacketHandler::HandleUpdateStat(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    uint8 l_TypeStat;
    uint16 l_StatNb;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_TypeStat;
    p_Packet >> l_StatNb;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }
        if (l_TypeStat == eStats::Speed)
        {
            float l_SpeedFloat = (float)l_StatNb / 10.0f;
            l_Unit->SetSpeed(l_SpeedFloat);
        }

        MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
        if (l_MenuManager == nullptr)
            return;
        if (l_TypeStat == eStats::Level)
        {
            l_MenuManager->AddElementToMenu(eMenuType::StatsMenu, 1, 1, std::to_string(l_StatNb));
            return;
        }
        l_MenuManager->AddElementToMenu(eMenuType::StatsMenu, 1, l_TypeStat + 2, std::to_string(l_StatNb));
    }
}

void PacketHandler::HandleKeyBoardBind(WorldPacket &p_Packet)
{
    uint8 l_TypeAction;
    uint8 l_Key;

    p_Packet >> l_TypeAction;
    p_Packet >> l_Key;

    m_InterfaceManager->AddKeyBind(l_Key, l_TypeAction);
}

void PacketHandler::HandleKeyBindBlock(WorldPacket &p_Packet)
{
    uint8 l_TypeAction;
    uint16 l_Time;

    p_Packet >> l_TypeAction;
    p_Packet >> l_Time;

    m_InterfaceManager->AddBlockingBind(l_TypeAction, l_Time);
}

void PacketHandler::HandleCastBar(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    uint8 l_Time;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_Time;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }
        l_Unit->LaunchCastBar((uint16)l_Time * 100);
    }
}

void PacketHandler::HandleLoadingPing(WorldPacket &p_Packet)
{
    g_Socket->SendLoadingPong();
}
