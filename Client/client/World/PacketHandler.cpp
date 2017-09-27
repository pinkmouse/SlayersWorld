#include "PacketHandler.hpp"
#include "../Entities/Player.hpp"
#include "../Entities/Creature.hpp"
#include "../Entities/AnimationUnit.hpp"
#include "../Entities/DynamicObject.hpp"
#include "../Graphics/Interface/MenuTitles.hpp"
#include "../Graphics/Interface/MenuWardrobe.hpp"
#include "../Graphics/Interface/MenuSpells.hpp"
#include "../Graphics/Interface/MenuBag.hpp"
#include "../Graphics/Interface/MenuEquipment.hpp"
#include "../Graphics/Interface/MenuSell.hpp"

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
    m_PacketHandleMap[SMSG::S_ExtraInterface] = &PacketHandler::HandleExtraUI;
    m_PacketHandleMap[SMSG::S_UnitIsInGroup] = &PacketHandler::HandleUnitIsInGroup;
    m_PacketHandleMap[SMSG::S_SrvPlayerQuestion] = &PacketHandler::HandleSrvPlayerQuestion;
    m_PacketHandleMap[SMSG::S_ExtraInterfaceData] = &PacketHandler::HandleExtraUIData;
    m_PacketHandleMap[SMSG::S_UnitPlayAuraVisual] = &PacketHandler::HandleUnitPlayVisualAura;
    m_PacketHandleMap[SMSG::S_UnitMount] = &PacketHandler::HandleMount;
    m_PacketHandleMap[SMSG::S_PlayerTitle] = &PacketHandler::HandlePlayerTitle;
    m_PacketHandleMap[SMSG::S_PlayerSkin] = &PacketHandler::HandlePlayerSkin;
    m_PacketHandleMap[SMSG::S_PlayerSpell] = &PacketHandler::HandlePlayerSpell;
    m_PacketHandleMap[SMSG::S_PlayerItem] = &PacketHandler::HandlePlayerItem;
    m_PacketHandleMap[SMSG::S_PlayerEquipment] = &PacketHandler::HandlePlayerEquipment;
    m_PacketHandleMap[SMSG::S_PlayerBagSize] = &PacketHandler::HandlePlayerBagSize;
    m_PacketHandleMap[SMSG::S_PlayerRemoveItem] = &PacketHandler::HandlePlayerRemoveItem;
    m_PacketHandleMap[SMSG::S_PlayerRemoveEquipment] = &PacketHandler::HandlePlayerRemoveEquipment;
    m_PacketHandleMap[SMSG::S_PlayerStackItem] = &PacketHandler::HandlePlayerStackItem;
    m_PacketHandleMap[SMSG::S_PlayerUpdateCurrency] = &PacketHandler::HandlePlayerUpdateCurrency;
    m_PacketHandleMap[SMSG::S_UnitUpdateName] = &PacketHandler::HanleUnitUpdateName;
    m_PacketHandleMap[SMSG::S_SellItemInterface] = &PacketHandler::HandleSellItemInterface;
    m_PacketHandleMap[SMSG::S_BindingSpell] = &PacketHandler::HandleBindingSpell;
}

void PacketHandler::HandleExtraUI(WorldPacket &p_Packet)
{
    uint8 l_ExtraUI;
    bool l_Enable;

    p_Packet >> l_ExtraUI;
    p_Packet >> l_Enable;

    if (l_Enable)
        m_InterfaceManager->AddExtraInterface((eExtraInterface)l_ExtraUI);
    else
        m_InterfaceManager->RemoveExtraInterface((eExtraInterface)l_ExtraUI);
}

void PacketHandler::HandleExtraUIData(WorldPacket &p_Packet)
{
    uint8 l_ExtraUI;
    uint8 l_Index;
    uint8 l_Type;
    int16 l_Data;

    p_Packet >> l_ExtraUI;
    p_Packet >> l_Index;
    p_Packet >> l_Type;
    p_Packet >> l_Data;

    m_InterfaceManager->AddExtraUiData((eExtraInterface)l_ExtraUI, l_Index, l_Type, l_Data);
    printf("Data = %d\n", l_Data);
}

void PacketHandler::HandleMount(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    int16 l_MountSkin;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_MountSkin;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
            return;

        l_Unit->SetMount(l_MountSkin);
    }
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
        if ((TypeUnit)l_TypeID == TypeUnit::PLAYER)
            m_InterfaceManager->GetHistoryField()->AddHistoryLine(l_Unit->GetName() + ": " + l_String);
    }
}

void PacketHandler::HandleStopMovement(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    Position l_Pos;
    uint16 l_PosX;
    uint16 l_PosY;
    uint8 l_Orientation;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_PosX;
    p_Packet >> l_PosY;
    p_Packet >> l_Orientation;

    l_Pos.x = l_PosX;
    l_Pos.y = l_PosY;

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


    p_Packet >> l_TypeID;
    p_Packet >> l_ID;

    //printf("Attack for Type :%d, ID:%d, posX:%d, posY:%d, orientation:%d\n", l_TypeID, l_ID, l_Pos.x, l_Pos.y, l_Orientation);
    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }

        if ((TypeUnit)l_TypeID == TypeUnit::ANIMATIONUNIT)
            l_Unit->LaunchAnim();
        else
            l_Unit->GetMovementHandler()->AddMovementToStack(eActionType::Attack);
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
            printf("Unit Unknow %d\n", l_ID);
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
    uint16 l_PosX;
    uint16 l_PosY;
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
        WorldPosition l_WorldPosition((uint32)l_PosX, (uint32)l_PosY, l_MapId, (Orientation)l_Orientation);
        l_Unit->TeleportTo(l_WorldPosition);
    }
}

void PacketHandler::HandleUnitGoDirection(WorldPacket &p_Packet)
{
    uint8 l_Type;
    uint16 l_UnitID;
    uint8 l_Direction;
    Position l_Pos;
    uint16 l_PosX;
    uint16 l_PosY;

    p_Packet >> l_Type;
    p_Packet >> l_UnitID;
    p_Packet >> l_PosX;
    p_Packet >> l_PosY;
    p_Packet >> l_Direction;

    l_Pos.x = (uint32)l_PosX;
    l_Pos.y = (uint32)l_PosY;

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
    std::string l_MapFileName;
    std::string l_MapChipsetName;
    std::string l_MapName;
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
    p_Packet >> l_MapFileName;
    p_Packet >> l_MapChipsetName;
    p_Packet >> l_MapName;

    p_Packet >> l_PosX;
    p_Packet >> l_PosY;
    p_Packet >> l_Orientation;

    if (!m_MapManager->LoadMap(l_MapID, l_MapFileName, l_MapChipsetName, l_MapName))
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
    uint16 l_ID;
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
    bool l_IsInGroup = false;
    uint16 l_PosX;
    uint16 l_PosY;

    /* PLAYER/CREATURE : Orientation / IsMovement / IsAttack / IsGroup */
    /* AREATRIGGER/GOB : Orientation / IsMovement / IsBlocking */
    CharOn41111 l_StructData;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_Name;
    if (l_TypeID < TypeUnit::AREATRIGGER) ///< Only Player and Creature
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
    p_Packet >> l_PosX;
    p_Packet >> l_PosY;
    p_Packet >> l_StructData.m_Byte_value;

    l_Pos.x = (uint32)l_PosX;
    l_Pos.y = (uint32)l_PosY;

    l_Orientation = l_StructData.charOn41111.first;
    l_IsInMovement = (bool)l_StructData.charOn41111.second;

    if (l_TypeID < 2) ///< Only Player and Creature
    {
        l_IsInAttack = (bool)l_StructData.charOn41111.third;
        l_IsInGroup = (bool)l_StructData.charOn41111.fourth;
    }
    else
    {
        l_IsBlocking = (bool)l_StructData.charOn41111.third;
    }
   /* if (l_TypeID == (uint8)TypeUnit::PLAYER)
        printf("Create new Player: %d %s %d %d %d %d\n", l_ID, l_Name.c_str(), l_SkinID, l_MapID, l_Pos.x, l_Pos.y);
    else
        printf("Create new Unit: %d %s %d %d %d %d\n", l_ID, l_Name.c_str(), l_SkinID, l_MapID, l_Pos.x, l_Pos.y);*/

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
        else if (l_TypeID == (uint8)TypeUnit::ANIMATIONUNIT)
            l_NewUnit = new AnimationUnit(l_ID, l_Name, l_Level, l_Health, l_SkinID, l_SizeX, l_SizeY, l_MapID, l_Pos.x, l_Pos.y, (Orientation)l_Orientation);
        else if (l_TypeID == (uint8)TypeUnit::AREATRIGGER || l_TypeID == (uint8)TypeUnit::GAMEOBJECT)
        {
           /* DynamicObject* l_DynIbj*/l_NewUnit = new DynamicObject(l_ID, (TypeUnit)l_TypeID, l_Name, l_Level, l_Health, l_SkinID, l_SizeX, l_SizeY, l_MapID, l_Pos.x, l_Pos.y, (Orientation)l_Orientation, l_IsBlocking);
            VisualEffect l_VisualEffect(eVisualType::VisualGob, false, l_SkinID, 3);
            l_VisualEffect.StartAnim();
            l_NewUnit->AddVisualEffect(l_NewUnit->GetType(), l_NewUnit->GetID(), l_VisualEffect);

            l_ActualMap->GetCase(l_Pos.x, l_Pos.y)->AddDynamicOject(l_NewUnit->ToDynamicObject());
        }

        float l_SpeedFloat = (float)l_Speed / 10.0f;
        l_NewUnit->SetSpeed(l_SpeedFloat);
        l_NewUnit->SetMap(l_ActualMap);
        l_ActualMap->AddUnit(l_NewUnit);
        printf("Create new Unit DONE: %d %s %d %d %d %d\n", l_ID, l_Name.c_str(), l_SkinID, l_MapID, l_Pos.x, l_Pos.y);
        if (l_IsInMovement)
            l_NewUnit->GetMovementHandler()->AddMovementToStack(eActionType::Go, l_Pos, (Orientation)l_NewUnit->GetOrientation());
        if (l_IsInAttack)
            l_NewUnit->GetMovementHandler()->AddMovementToStack(eActionType::Attack, l_Pos, (Orientation)l_NewUnit->GetOrientation());
        l_NewUnit->SetIsInGroup(l_IsInGroup);
    }
}

void PacketHandler::OperatePacket(WorldPacket &p_Packet)
{
	uint8 l_PacketID;
	p_Packet >> l_PacketID;
	printf("Receive Packet %d\n", l_PacketID);
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
    std::string l_MapFileName;
    std::string l_MapChipsetName;
    std::string l_MapName;

    p_Packet >> l_MapID;
    p_Packet >> l_MapFileName;
    p_Packet >> l_MapChipsetName;
    p_Packet >> l_MapName;

    delete  m_MapManager->GetActualMap();
    m_InterfaceManager->SetIsLoading(true);
    if (!m_MapManager->LoadMap(l_MapID, l_MapFileName, l_MapChipsetName, l_MapName))
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
    CharOn44 l_StructData;

    p_Packet >> l_StructData.m_Byte_value;
    p_Packet >> l_Msg;
    
    SWText l_Text(l_Msg, (eTextColor)l_StructData.charOn44.first, (eTextStyle)l_StructData.charOn44.second);

    m_InterfaceManager->GetHistoryField()->OpenTemporary(5000);
    m_InterfaceManager->GetHistoryField()->AddHistoryLine(l_Text);
}

void PacketHandler::HandleSrvPlayerQuestion(WorldPacket &p_Packet)
{
    uint16 l_QuestionID;
    std::string l_Msg;

    p_Packet >> l_QuestionID;
    p_Packet >> l_Msg;

    printf("I--> Question %d:%s\n", l_QuestionID, l_Msg.c_str());
    m_InterfaceManager->AddSimpleQuestion(l_QuestionID, l_Msg);
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

    m_InterfaceManager->AddWarningMsg((eTypeWarningMsg)l_Type, l_WarningMsg);
}

void PacketHandler::HandleUnitPlayVisual(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    bool l_Under;
    uint8 l_VisualID;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_Under;
    p_Packet >> l_VisualID;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }
        VisualEffect l_VisualEffect(eVisualType::VisualSpell, l_Under, l_VisualID, MAX_VISUAL_IMG_X);
        l_VisualEffect.StartAnimAndStop();
        l_Unit->AddVisualEffect(l_Unit->GetType(), l_Unit->GetID(), l_VisualEffect);
    }
}

void PacketHandler::HandleUnitPlayVisualAura(WorldPacket &p_Packet)
{
    bool l_Apply = false;
    uint8 l_TypeID;
    uint16 l_ID;
    uint8 l_TypeIDFrom;
    uint16 l_IDFrom;
    bool l_Under;
    uint8 l_VisualID;
    CharOn116 l_Struct;

    //p_Packet >> l_Apply;
    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_TypeIDFrom;
    p_Packet >> l_IDFrom;
    p_Packet >> l_Struct.m_Byte_value;
    //p_Packet >> l_Under;
    //p_Packet >> l_VisualID;
 
    l_Apply = l_Struct.charOn116.first ? true : false;
    l_Under = l_Struct.charOn116.second ? true : false;
    l_VisualID = l_Struct.charOn116.third;
    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }
        if (l_Apply)
        {
            VisualEffect l_VisualEffect(eVisualType::VisualSpell, l_Under, l_VisualID, MAX_VISUAL_IMG_X);
            l_VisualEffect.StartAnim();
            l_Unit->AddVisualEffect((TypeUnit)l_TypeIDFrom, l_IDFrom, l_VisualEffect);
        }
        else
        {
            l_Unit->RemoveVisualEffect((TypeUnit)l_TypeIDFrom, l_IDFrom, l_VisualID);
        }
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
        else
        {
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
}

void PacketHandler::HandleKeyBoardBind(WorldPacket &p_Packet)
{
    uint8 l_Nb;

    p_Packet >> l_Nb;

    for (uint8 i = 0; i < l_Nb; i++)
    {
        uint8 l_TypeAction;
        uint8 l_Key;

        p_Packet >> l_TypeAction;
        p_Packet >> l_Key;
        m_InterfaceManager->AddKeyBind(l_Key, l_TypeAction);
    }
}

void PacketHandler::HandleKeyBindBlock(WorldPacket &p_Packet)
{
    uint8 l_TypeAction;
    uint16 l_Time;

    p_Packet >> l_TypeAction;
    p_Packet >> l_Time;

    m_InterfaceManager->AddBlockingBind(l_TypeAction, l_Time);
}

void PacketHandler::HandleUnitIsInGroup(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    bool l_IsInGroup;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_IsInGroup;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }
        l_Unit->SetIsInGroup(l_IsInGroup);
    }
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

void PacketHandler::HandlePlayerTitle(WorldPacket &p_Packet)
{
    uint8 l_Nb;
    MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
    MenuTitles* l_MenuTitles = reinterpret_cast<MenuTitles*>(l_MenuManager->GetMenu(eMenuType::TitlesMenu));
    if (l_MenuTitles == nullptr)
        return;

    p_Packet >> l_Nb;
    for (uint8 i = 0; i < l_Nb; i++)
    {
        uint16 l_ID;
        std::string l_Name;

        p_Packet >> l_ID;
        p_Packet >> l_Name;

        l_MenuTitles->AddTitle(l_ID, l_Name);
    }
}

void PacketHandler::HandlePlayerSkin(WorldPacket &p_Packet)
{
    uint8 l_Nb;
    MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
    MenuWardrobe* l_MenuWardRobe = reinterpret_cast<MenuWardrobe*>(l_MenuManager->GetMenu(eMenuType::WardrobeMenu));
    if (l_MenuWardRobe == nullptr)
        return;

    p_Packet >> l_Nb;
    for (uint8 i = 0; i < l_Nb; i++)
    {
        uint16 l_ID;
        std::string l_Name;

        p_Packet >> l_ID;
        p_Packet >> l_Name;

        l_MenuWardRobe->AddSkin(l_ID, l_Name);
    }
}

void PacketHandler::HandlePlayerRemoveItem(WorldPacket &p_Packet)
{
    uint8 l_SlotID;
    MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
    MenuBag* l_MenuBag = reinterpret_cast<MenuBag*>(l_MenuManager->GetMenu(eMenuType::BagMenu));
    if (l_MenuBag == nullptr)
        return;

    MenuSell* l_MenuSell = reinterpret_cast<MenuSell*>(m_InterfaceManager->GetMenuInterface(eMenuType::SellMenu));

    if (l_MenuSell == nullptr)
        return;

    p_Packet >> l_SlotID;
    l_MenuBag->RemoveItem(l_SlotID);
    l_MenuSell->RemoveItem(l_SlotID);
}

void PacketHandler::HandlePlayerUpdateCurrency(WorldPacket &p_Packet)
{
    uint8 l_Nb;
    MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
    MenuBag* l_MenuBag = reinterpret_cast<MenuBag*>(l_MenuManager->GetMenu(eMenuType::BagMenu));
    if (l_MenuBag == nullptr)
        return;

    MenuSell* l_MenuSell = reinterpret_cast<MenuSell*>(m_InterfaceManager->GetMenuInterface(eMenuType::SellMenu));

    if (l_MenuSell == nullptr)
        return;

    printf("Set Currency\n");

    p_Packet >> l_Nb;
    for (uint8 i = 0; i < l_Nb; i++)
    {
        uint8 l_Type;
        uint16 l_Value;

        p_Packet >> l_Type;
        p_Packet >> l_Value;

        l_MenuBag->SetCurrency((eTypeCurrency)l_Type, l_Value);
        l_MenuSell->SetCurrency((eTypeCurrency)l_Type, l_Value);
    }
}

void PacketHandler::HandlePlayerStackItem(WorldPacket &p_Packet)
{
    uint8 l_SlotID;
    uint8 l_Stack;
    MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
    MenuBag* l_MenuBag = reinterpret_cast<MenuBag*>(l_MenuManager->GetMenu(eMenuType::BagMenu));
    if (l_MenuBag == nullptr)
        return;

    MenuSell* l_MenuSell = reinterpret_cast<MenuSell*>(m_InterfaceManager->GetMenuInterface(eMenuType::SellMenu));

    if (l_MenuSell == nullptr)
        return;

    p_Packet >> l_SlotID;
    p_Packet >> l_Stack;
    l_MenuBag->SetStackItem(l_SlotID, l_Stack);
    l_MenuSell->SetStackItem(l_SlotID, l_Stack);
}

void PacketHandler::HandlePlayerRemoveEquipment(WorldPacket &p_Packet)
{
    uint8 l_SlotID;
    MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
    MenuEquipment* l_MenuEquipment = reinterpret_cast<MenuEquipment*>(l_MenuManager->GetMenu(eMenuType::EquipmentMenu));
    if (l_MenuEquipment == nullptr)
        return;

    p_Packet >> l_SlotID;
    l_MenuEquipment->RemoveEquipment((eTypeEquipment)l_SlotID);
}

void PacketHandler::HandlePlayerBagSize(WorldPacket &p_Packet)
{
    uint8 l_Size;
    MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
    MenuBag* l_MenuBag = reinterpret_cast<MenuBag*>(l_MenuManager->GetMenu(eMenuType::BagMenu));
    if (l_MenuBag == nullptr)
        return;

    MenuSell* l_MenuSell = reinterpret_cast<MenuSell*>(m_InterfaceManager->GetMenuInterface(eMenuType::SellMenu));

    if (l_MenuSell == nullptr)
        return;

    p_Packet >> l_Size;
    l_MenuManager->GetElement(0, 4)->SetLabel(l_MenuManager->GetElement(0, 4)->GetLabel() + " x" + std::to_string(l_Size));
    l_MenuBag->SetSize(l_Size);
    l_MenuSell->SetSize(l_Size);
}

void PacketHandler::HandleBindingSpell(WorldPacket &p_Packet)
{
    uint8 l_Nb;

    p_Packet >> l_Nb;

    uint8 l_Bind = 0;
    uint16 l_SpellID = 0;
    for (uint8 i = 0; i < l_Nb; i++)
    {
        p_Packet >> l_Bind;
        p_Packet >> l_SpellID;

        m_InterfaceManager->AddSpellBind(l_Bind, l_SpellID);
    }
}

void PacketHandler::HandleSellItemInterface(WorldPacket &p_Packet)
{
    uint8 l_Nb;
    MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
    MenuSell* l_MenuSell = reinterpret_cast<MenuSell*>(m_InterfaceManager->GetMenuInterface(eMenuType::SellMenu));
    if (l_MenuSell == nullptr)
        return;

    p_Packet >> l_Nb;

    uint8 l_SlotID = 0;
    uint16 l_Price = 0;
    for (uint8 i = 0; i < l_Nb; i++)
    {
        p_Packet >> l_SlotID;
        p_Packet >> l_Price;

        l_MenuSell->AddItemPrice(l_SlotID, l_Price);
    }
    l_MenuSell->Open();
}

void PacketHandler::HandlePlayerItem(WorldPacket &p_Packet)
{
    uint8 l_Nb;
    MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
    MenuBag* l_MenuBag = reinterpret_cast<MenuBag*>(l_MenuManager->GetMenu(eMenuType::BagMenu));

    if (l_MenuBag == nullptr)
        return;

    MenuSell* l_MenuSell = reinterpret_cast<MenuSell*>(m_InterfaceManager->GetMenuInterface(eMenuType::SellMenu));

    if (l_MenuSell == nullptr)
        return;

    p_Packet >> l_Nb;

    uint8 l_SlotID;
    std::string l_Name;
    uint8 l_StackNb;
    uint8 l_TypeID;
    uint8 l_SubType;
    uint8 l_Level;
    uint8 l_RareLevel;
    int32 l_Data;

    for (uint8 i = 0; i < l_Nb; i++)
    {
        p_Packet >> l_SlotID;
        p_Packet >> l_Name;
        p_Packet >> l_StackNb;
        p_Packet >> l_TypeID;
        p_Packet >> l_SubType;
        p_Packet >> l_Level;
        p_Packet >> l_RareLevel;

        Item l_Item((eItemType)l_TypeID, l_SubType, l_Name, (eItemRareLevel)l_RareLevel, l_Level, l_StackNb);
        for (uint8 j = 0; j < 4; j++)
        {
            p_Packet >> l_Data;
            l_Item.AddData(l_Data);
        }

        l_MenuBag->AddItem(l_SlotID, l_Item);
        l_MenuSell->AddItem(l_SlotID, l_Item);
        printf("Get Item on slot %d : %s\n", l_SlotID, l_Name.c_str());
    }
}

void PacketHandler::HandlePlayerEquipment(WorldPacket &p_Packet)
{
    uint8 l_Nb;
    MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
    MenuEquipment* l_MenuEquipment = reinterpret_cast<MenuEquipment*>(l_MenuManager->GetMenu(eMenuType::EquipmentMenu));
    if (l_MenuEquipment == nullptr)
        return;

    p_Packet >> l_Nb;

    uint8 l_TypeID;
    std::string l_Name;
    uint8 l_Level;
    uint8 l_RareLevel;
    int32 l_Data;

    for (uint8 i = 0; i < l_Nb; i++)
    {
        p_Packet >> l_TypeID;
        p_Packet >> l_Name;
        p_Packet >> l_Level;
        p_Packet >> l_RareLevel;

        Item l_Item(eItemType::ITEM_EQUIPMENT, l_TypeID, l_Name, (eItemRareLevel)l_RareLevel, l_Level, 1);
        for (uint8 j = 0; j < 4; j++)
        {
            p_Packet >> l_Data;
            l_Item.AddData(l_Data);
        }
        l_MenuEquipment->AddEquipment((eTypeEquipment)l_TypeID, l_Item);
        printf("Get Equipement on placement %d : %s\n", l_TypeID, l_Name.c_str());
    }
}

void PacketHandler::HandlePlayerSpell(WorldPacket &p_Packet)
{
    uint8 l_Nb;
    MenuManager* l_MenuManager = m_InterfaceManager->GetMenuManager();
    MenuSpells* l_MenuSpells = reinterpret_cast<MenuSpells*>(l_MenuManager->GetMenu(eMenuType::SpellsMenu));
    if (l_MenuSpells == nullptr)
        return;

    p_Packet >> l_Nb;
    for (uint8 i = 0; i < l_Nb; i++)
    {
        uint16 l_ID;
        std::string l_Name;

        p_Packet >> l_ID;
        p_Packet >> l_Name;

        l_MenuSpells->AddSpell(l_ID, l_Name);
    }
}

void PacketHandler::HanleUnitUpdateName(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    std::string l_Name;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_Name;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }
        l_Unit->SetName(l_Name);
    }
}

void PacketHandler::HandleLoadingPing(WorldPacket &p_Packet)
{
    g_Socket->SendLoadingPong();
}
