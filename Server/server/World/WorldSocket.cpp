#include "WorldSocket.hpp"
#include "../Map/Map.hpp"
#include "../System/Spell/Aura.hpp"
#include "PacketDefine.hpp"

WorldSocket::WorldSocket()
{
    m_Player = nullptr;
    m_Disconnected = false;
}

WorldSocket::~WorldSocket()
{
    if (m_Player == nullptr)
        return;

    /// Save Player
    delete m_Player;
    //m_Player->SetInWorld(false);
    m_Player = nullptr;
}

void WorldSocket::SendAuthResponse(uint8 p_Status)
{
	WorldPacket l_Packet;
	uint8 l_ID = SMSG::S_Connexion;

	l_Packet << l_ID << p_Status;
	send(l_Packet);
	printf("Send Status Auth %d\n", p_Status);
}

void WorldSocket::SendPlayerCreate(uint32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_Health, uint8 p_Mana, uint8 p_Alignment, int16 p_SkinID, uint16 p_MapID, const std::string & p_MapFileName, const std::string & p_FileChipsetName, const std::string & p_MapName, uint32 p_PosX, uint32 p_PosY, uint8 p_Orientation)
{
    WorldPacket l_Packet;
    uint8 l_ID = SMSG::S_PlayerCreate;

    l_Packet << l_ID << p_ID << p_Name << p_Level << p_Health << p_Mana << p_Alignment << p_SkinID << p_MapID << p_MapFileName << p_FileChipsetName << p_MapName << p_PosX << p_PosY << p_Orientation;
    send(l_Packet);
    printf("Send create\n");
}

void WorldSocket::SendUnitCreate(Unit* p_Unit, bool p_IsInGroup)
{
    if (p_Unit->GetType() == TypeUnit::PLAYER && p_Unit->GetID() == GetPlayer()->GetID())
        return;

    if (p_Unit->GetSkinID() < 0)
        return;

    PacketUnitCreate l_Packet;
    if (p_Unit->GetType() < TypeUnit::AREATRIGGER)
        l_Packet.BuildPacket(p_Unit->GetType(), p_Unit->GetID(), p_Unit->GetNameWithTitle() , p_Unit->GetLevel(), p_Unit->GetResourceNb(eResourceType::Health), p_Unit->GetResourceNb(eResourceType::Mana), p_Unit->GetResourceNb(eResourceType::Alignment), p_Unit->GetSkinID(),
            p_Unit->GetSizeX(), p_Unit->GetSizeY(), p_Unit->GetSpeedUint8(), p_Unit->GetMapID(), p_Unit->GetPosition(), p_Unit->GetOrientation(), p_Unit->IsInMovement(), p_Unit->GetMovementHandler()->IsInAttack(), p_IsInGroup);
    else
        l_Packet.BuildPacket(p_Unit->GetType(), p_Unit->GetID(), p_Unit->GetName(), p_Unit->GetSkinID(), p_Unit->GetSizeX(), p_Unit->GetSizeY(), p_Unit->GetSpeedUint8(), p_Unit->GetMapID(), p_Unit->GetPosition(), p_Unit->GetOrientation(), p_Unit->IsInMovement(), p_Unit->IsBlocking());

    send(l_Packet.m_Packet);


    std::vector<AuraEffect*> l_ListMount = p_Unit->GetAuraEffectType(eTypeAuraEffect::MOUNT);
    std::vector<Aura*>* l_ListAura = p_Unit->GetAuraList();
    int16 l_Mount = -1;
    if (!l_ListMount.empty())
        l_Mount =l_ListMount[0]->GetAmount();
    if (l_Mount >= 0)
        SendUnitMount(p_Unit->GetType(), p_Unit->GetID(), l_Mount);
    for (std::vector<Aura*>::iterator l_It = l_ListAura->begin(); l_It != l_ListAura->end(); l_It++)
    {
        if ((*l_It)->GetSpellTemplate()->GetVisualIDTarget() >= 0)
        {
            PacketPlayAuraVisual l_Packet;
            l_Packet.BuildPacket(true, p_Unit->GetType(), p_Unit->GetID(), (*l_It)->GetCasterType(), (*l_It)->GetCasterID(), false, (*l_It)->GetSpellTemplate()->GetVisualIDTarget());
            send(l_Packet.m_Packet);
        }
        if ((*l_It)->GetSpellTemplate()->GetVisualIDTargetUnder() >= 0)
        {
            PacketPlayAuraVisual l_Packet;
            l_Packet.BuildPacket(true, p_Unit->GetType(), p_Unit->GetID(), (*l_It)->GetCasterType(), (*l_It)->GetCasterID(), true, (*l_It)->GetSpellTemplate()->GetVisualIDTargetUnder());
            send(l_Packet.m_Packet);
        }
    }
}

void WorldSocket::SendUnitMount(const uint8 & p_Type, const uint32 & p_ID, const int16 & p_Skin)
{
    if (p_Type == TypeUnit::PLAYER && p_ID == GetPlayer()->GetID())
        return;

    PacketUnitMount l_Packet;

    l_Packet.BuildPacket(p_Type, p_ID, p_Skin);
    send(l_Packet.m_Packet);
}

void WorldSocket::SendUnitIsInGroup(uint8 p_Type, uint32 p_ID, bool p_IsInGroup)
{
    if (p_Type == TypeUnit::PLAYER && p_ID == GetPlayer()->GetID())
        return;

    PacketUnitIsInGroup l_Packet;

    l_Packet.BuildPacket(p_Type, p_ID, p_IsInGroup);
    send(l_Packet.m_Packet);
}

void WorldSocket::SendUnitGoDirectionToSet(uint8 p_Type, uint16 p_UnitID, const Position & p_Pos, uint8 p_Direction)
{
    WorldPacket l_Packet;
    uint8 l_ID = SMSG::S_UnitGoDirection;

    l_Packet << l_ID << p_Type << p_UnitID << (uint16)p_Pos.m_X << (uint16)p_Pos.m_Y << p_Direction;
    SendToSet(l_Packet, true);
}

void WorldSocket::SendUnitTalk(uint8 p_Type, uint16 p_UnitID, const std::string & p_TalkString)
{
    WorldPacket l_Packet;
    uint8 l_ID = SMSG::S_UnitTalk;

    l_Packet << l_ID << p_Type << p_UnitID << p_TalkString;
    SendToSet(l_Packet);
}

void WorldSocket::SendUnitStopMovement(uint8 p_TypeID, uint16 p_ID, const Position & p_Pos, uint8 p_Orientation)
{
    WorldPacket l_Packet;
    uint8 l_ID = SMSG::S_UnitStopMovement;

    l_Packet << l_ID << p_TypeID << p_ID << (uint16)p_Pos.m_X << (uint16)p_Pos.m_Y << p_Orientation;
    SendToSet(l_Packet, true);
}

void WorldSocket::SendUnitStartAttack(uint8 p_TypeID, uint16 p_ID)
{
    WorldPacket l_Packet;
    uint8 l_ID = SMSG::S_UnitStartAttack;

    l_Packet << l_ID << p_TypeID << p_ID ;
    SendToSet(l_Packet, true);
}

void WorldSocket::SendUnitStopAttack(uint8 p_TypeID, uint16 p_ID)
{
    WorldPacket l_Packet;
    uint8 l_ID = SMSG::S_UnitStopAttack;

    l_Packet << l_ID << p_TypeID << p_ID;
    SendToSet(l_Packet, true);
}

void WorldSocket::SendLogDamage(uint8 p_TypeID, uint16 p_ID, DamageInfo p_DamageInfo)
{
	PacketLogDamage l_Packet;
	l_Packet.BuildPacket(p_TypeID, p_ID, p_DamageInfo.m_Damage, p_DamageInfo.m_Result);
	SendPacket(l_Packet.m_Packet);
}

void WorldSocket::SendSwitchMap(uint16 p_MapID, const std::string & p_MapFileName, const std::string & p_ChipsetFile, const std::string & p_MapName)
{
    PacketSwitchMap l_Packet;
    l_Packet.BuildPacket(p_MapID, p_MapFileName, p_ChipsetFile, p_MapName);
    send(l_Packet.m_Packet);
}

void WorldSocket::SendKeyBoardBind(eKeyBoardAction p_Type, uint8 p_Key)
{
    PacketKeyBoardBind l_Packet;
    l_Packet.BuildPacket(p_Type, p_Key);
    send(l_Packet.m_Packet);
}

void WorldSocket::SendKeyBoardBind(std::map< eKeyBoardAction, uint8 >* p_ListKey)
{
    PacketKeyBoardBind l_Packet;
    l_Packet.BuildPacket(p_ListKey);
    send(l_Packet.m_Packet);
}

void WorldSocket::SendTitles(std::map<uint16, Title*>* p_Titles)
{
    if (p_Titles == nullptr)
        return;

    PacketPlayerTitle l_Packet;

    l_Packet.BuildPacket(p_Titles);
    SendPacket(l_Packet.m_Packet);
}

void WorldSocket::SendUpdateUnitResource(uint8 p_TypeID, uint16 p_ID, uint8 p_Resource, uint8 p_NewHealth)
{
    WorldPacket l_Packet;
    uint8 l_ID = SMSG::S_UnitUpdateResource;

    l_Packet << l_ID << p_TypeID << p_ID << p_Resource << p_NewHealth;
    SendToSet(l_Packet);
}

void WorldSocket::SendUpdateXpPct(float p_NewPct)
{
    WorldPacket l_Packet;
    uint8 l_ID = SMSG::S_PlayerUpdateXp;

    l_Packet << l_ID << p_NewPct;
    send(l_Packet);
}

Player* WorldSocket::GetPlayer()
{
    return m_Player;
}

void WorldSocket::SetPlayer(Player* p_Player)
{
    m_Player = p_Player;
}

void WorldSocket::SendPacket(WorldPacket p_Packet)
{
    send(p_Packet);
}

void WorldSocket::SendToSet(WorldPacket p_Packet, bool p_ExcludePlayer /*= false*/, bool p_OnlyGroup /* = false*/)
{
    Map* l_Map = GetPlayer()->GetMap();

    if (l_Map == nullptr)
        return;

    std::vector<Square*> l_SquareSet = l_Map->GetSquareSet(GetPlayer()->GetSquareID());
    for (Square* l_Square : l_SquareSet)
    {
        std::map<uint16, Unit*>* l_SquareList = l_Square->GetList(TypeUnit::PLAYER);
        if (l_SquareList == nullptr)
            continue;

        for (std::pair<uint16, Unit*> l_UnitPair : *l_SquareList)
        {
            Unit* l_Unit = l_UnitPair.second;

            if (l_Unit == nullptr)
                continue;

            if (Player* l_Player = l_Unit->ToPlayer())
            {
                if (p_ExcludePlayer && l_Player->GetID() == GetPlayer()->GetID())
                    continue;

                ///printf("Send to %s\n", l_Player->GetName().c_str());
                l_Player->GetSession()->send(p_Packet);
            }
        }
    }
}

bool WorldSocket::IsDisonnected() const
{
    return m_Disconnected;
}

void WorldSocket::Kick()
{
    m_Disconnected = true;
    disconnect();
}