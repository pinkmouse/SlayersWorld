#include "Player.hpp"
#include "../World/WorldSocket.hpp"
#include "../System/Spell/Spell.hpp"
#include "../World/PacketDefine.hpp"
#include "../Map/Map.hpp"
#include "../Global.hpp"

Player::Player(uint32 p_AccountID, int32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_Health, uint8 p_Mana, uint8 p_Alignment, int16 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, Orientation p_Orientation, uint32 p_Xp, eAccessType p_AccessType) :
    Unit(p_ID, TypeUnit::PLAYER, eFactionType::Ally),
    m_AccessType(p_AccessType),
    m_AccountID(p_AccountID)
{
    InitializeCommands();

    m_Name = p_Name;
    m_Level = p_Level;
    m_SkinID = p_SkinID;
    SetMapID(p_MapID);
    SetPosX(p_PosX);
    SetPosY(p_PosY);
    SetOrientation(p_Orientation);
    m_Session = nullptr;
    m_InLoading = false;
    m_Initilize = false;
    SetResourceNb(eResourceType::Health, p_Health);
    SetResourceNb(eResourceType::Mana, p_Mana);
    SetResourceNb(eResourceType::Alignment, p_Alignment);
    m_Xp = p_Xp;
    m_RespawnTime = PLAYER_TIME_RESPAWN * IN_MICROSECOND;
}

WorldSocket* Player::GetSession() const
{
    return m_Session;
}

void Player::HasBeenInitialize()
{
    m_Initilize = true;
}

void Player::SetSession(WorldSocket* p_Session)
{
    m_Session = p_Session;
}

Player::~Player()
{
    printf("Erase Player %d:%s\n", m_ID, m_Name.c_str());

    for (std::map< uint16, Quest* >::iterator l_It = m_Quests.begin(); l_It != m_Quests.end(); ++l_It)
    {
        delete (*l_It).second;
    }
}

uint32 Player::GetAccountID() const
{
    return m_AccountID;
}

void Player::Update(sf::Time p_Diff)
{
    Unit::Update(p_Diff);
    UpdateQuests();
}

void Player::UpdateNewSquares(uint16 p_OldSquareID, uint16 p_NewSquareID, bool p_UpdateAll)
{
    std::vector<uint16> l_OldSquareSet;
    if (!p_UpdateAll)
        l_OldSquareSet = m_Map->GetSquareSetID(p_OldSquareID);
    std::vector<uint16> l_NewSquareSet = m_Map->GetSquareSetID(p_NewSquareID);

    std::vector<uint16> l_DiffSquareSet;
    for (uint16 l_Id : l_NewSquareSet)
    {
        std::vector<uint16>::iterator l_It = std::find(l_OldSquareSet.begin(), l_OldSquareSet.end(), l_Id);

        if (l_It == l_OldSquareSet.end())
            l_DiffSquareSet.push_back(l_Id);
    }

    for (uint16 l_Id : l_DiffSquareSet)
    {
        Square* l_Square = m_Map->GetSquare(l_Id);

        if (l_Square == nullptr)
            continue;

        std::map<TypeUnit, std::map<uint16, Unit*>>* l_SquareList = l_Square->GetList();

        for (std::pair<TypeUnit, std::map<uint16, Unit*>> l_SquareMap : *l_SquareList)
        {
            for (std::pair<uint16, Unit*> l_SquareList : l_SquareMap.second)
            {
                Unit* l_Unit = l_SquareList.second;

                if (l_Unit == nullptr || !l_Unit->IsInWorld())
                    continue;

                if (l_Unit->IsPlayer() && l_Unit->GetID() == GetID())
                    continue;

                    GetSession()->SendUnitCreate(l_Unit->GetType(), l_Unit->GetID(), l_Unit->GetName(), l_Unit->GetLevel(), l_Unit->GetResourceNb(eResourceType::Health), l_Unit->GetResourceNb(eResourceType::Mana), l_Unit->GetResourceNb(eResourceType::Alignment), l_Unit->GetSkinID(), l_Unit->GetSizeX(), l_Unit->GetSizeY(), l_Unit->GetSpeedUint8(), l_Unit->GetMapID(), l_Unit->GetPosition(), l_Unit->GetOrientation(), l_Unit->IsInMovement(), l_Unit->GetMovementHandler()->IsInAttack(), l_Unit->IsBlocking());
            }
        }
    }
}

uint32 Player::GetXp() const
{
    return m_Xp;
}

void Player::SetResourceNb(eResourceType p_Resource, uint8 p_Nb)
{
    Unit::SetResourceNb(p_Resource, p_Nb);
    if (!m_Initilize)
        return;
    m_Session->SendUpdateUnitResource(GetType(), GetID(), p_Resource, p_Nb);
}

void Player::AddResourceNb(eResourceType p_Resource, uint8 p_Nb)
{
    Unit::AddResourceNb(p_Resource, p_Nb);
    if (!m_Initilize)
        return;
    m_Session->SendUpdateUnitResource(GetType(), GetID(), p_Resource, GetResourceNb(p_Resource));
}

bool Player::CheckCommand(const std::string & p_String)
{
    if (p_String.empty())
        return false;

    if (p_String[0] == '/')
    {
        std::string l_Cmd = p_String;
        std::vector<std::string> l_CmdList;

        l_Cmd.erase(0, (size_t)1);

        size_t l_Pos = 0;
        std::string l_Token;
        while ((l_Pos = l_Cmd.find(' ')) != std::string::npos) 
        {
            l_Token = l_Cmd.substr(0, l_Pos);
            l_CmdList.push_back(l_Token);
            l_Cmd.erase(0, l_Pos + 1);
        }
        l_CmdList.push_back(l_Cmd.c_str());

        if (GetAccessType() < m_CmdHandleMap[l_CmdList[0]].first)
            return false;

        m_Func l_Fun = m_CmdHandleMap[l_CmdList[0]].second;
        if (l_Fun != nullptr)
        {
            l_CmdList.erase(l_CmdList.begin());
            return (this->*(l_Fun))(l_CmdList);
        }

        return false;
    }
    return false;
}


void Player::SetXp(uint32 p_Xp)
{
    if (g_LevelManager->IsMaxLevel(GetLevel()))
        return;

    if (p_Xp > g_LevelManager->GetXpForLevel(GetLevel()))
    {
        /// Earn new level
        p_Xp -= g_LevelManager->GetXpForLevel(GetLevel());
        SetLevel(GetLevel() + 1);

		/// Earn new point
        AddPointsStat(eStats::Free, 1);

        PacketUnitPlayVisual l_Packet;
        l_Packet.BuildPacket(GetType(), GetID(), 4);
        GetMap()->SendToSet(l_Packet.m_Packet, this);
		SendMsg("Level UP : " + std::to_string(GetLevel()));
    }

    float l_Pct = g_LevelManager->XpPct(GetLevel(), p_Xp);
    if (m_Initilize)
        m_Session->SendUpdateXpPct(l_Pct);
    m_Xp = p_Xp;
}

void Player::SetLevel(const uint8 & p_Level)
{
    Unit::SetLevel(p_Level);

    PacketUnitUpdateStat l_Packet2;
    l_Packet2.BuildPacket(GetType(), GetID(), eStats::Level, p_Level);
    GetSession()->send(l_Packet2.m_Packet);
}


void Player::SendMsg(const std::string & p_Msg)
{
    PacketSrvPlayerMsg l_Packet;
    l_Packet.BuildPacket(p_Msg);
    WorldSocket* l_Session = GetSession();
    l_Session->SendPacket(l_Packet.m_Packet);
}

void Player::Respawn()
{
    Unit::Respawn();

    SetResourceNb(eResourceType::Health, MAX_HEALTH);
    m_ResTimer = 0;
}

eAccessType Player::GetAccessType() const
{
    return m_AccessType;
}

void Player::ActionFrom(Player* p_Player)
{
    ;
}

void Player::EventAction(eKeyBoardAction p_PlayerAction)
{
    if (m_MovementHandler == nullptr)
        return;

    switch (p_PlayerAction)
    {
        case eKeyBoardAction::KeyBoardAction: /// GOSSIP
        {
            Unit* l_Unit = m_Map->GetCloserUnit(this, MELEE_RANGE, true);
            if (l_Unit == nullptr)
                return;

            l_Unit->ActionFrom(this);
            break;
        }
        case eKeyBoardAction::KeyBoardSpell1:
        case eKeyBoardAction::KeyBoardSpell0:
            if (m_MovementHandler->IsInAttack())
                return;
            if (GetSpellOnBind(p_PlayerAction) > 0)
                CastSpell(GetSpellOnBind(p_PlayerAction));
            break;
        default:
            break;
    }
}

void Player::Save()
{
    g_SqlManager->SavePlayer(this);
}

void Player::LearnSpell(uint16 p_SpellID)
{
    ;
}


void Player::UpdateQuests()
{
    /*for (std::map< uint16, Quest* >::iterator l_It = m_Quests.begin(); l_It != m_Quests.end(); ++l_It)
    {
        if ((*l_It).second->IsDone())
            printf("-----> Quest %d is DONE\n", (*l_It).first);
    }*/
}

void Player::ValidateQuest(Quest* p_Quest)
{
    if (m_Quests.find(p_Quest->GetID()) == m_Quests.end())
        return;

    PacketWarningMsg l_Packet;
    l_Packet.BuildPacket(eTypeWarningMsg::Yellow, "Quete : " + p_Quest->GetName() + " terminé");
    GetSession()->send(l_Packet.m_Packet);

    g_SqlManager->SaveQuestForPlayer(this, p_Quest);
    RemoveQuest(p_Quest->GetID());
}

void Player::RemoveQuest(uint16 p_QuestID)
{
    auto l_It = m_Quests.find(p_QuestID);
    if (l_It == m_Quests.end())
        return;
    
    delete (*l_It).second;
    m_Quests.erase(l_It);
}

void Player::AddKeyBoardBind(eKeyBoardAction p_Action, uint8 p_ID)
{
    if (p_ID >= sf::Keyboard::KeyCount)
        return;
    m_KeyBoardBinds[p_Action] = p_ID;
    g_SqlManager->ReplaceKeyBindsForAccount(GetAccountID(), p_Action, p_ID);
}

int16 Player::GetKeyBoardBind(eKeyBoardAction p_Action)
{
    if (m_KeyBoardBinds.find(p_Action) == m_KeyBoardBinds.end())
        return -1;
    return m_KeyBoardBinds[p_Action];
}

std::map< uint16, Quest* >* Player::GetQuestList()
{
    return &m_Quests;
}

Quest* Player::GetQuest(uint16 p_QuestID) const
{
    if (m_Quests.find(p_QuestID) != m_Quests.end())
        return m_Quests.at(p_QuestID);
    return nullptr;
}

void Player::AddQuest(Quest* p_Quest, bool p_New /*= true*/) /* False when load at conection*/
{
    if (m_Quests.find(p_Quest->GetID()) != m_Quests.end())
    {
        delete p_Quest;
        return;
    }
    m_Quests[p_Quest->GetID()] = p_Quest;

    if (!p_New)
        return;

    PacketWarningMsg l_Packet;
    l_Packet.BuildPacket(eTypeWarningMsg::Yellow, "Nouvelle Quete : " + p_Quest->GetName());
    GetSession()->send(l_Packet.m_Packet);
}

bool Player::HasQuestInProgress(uint16 p_QuestID)
{
    if (m_Quests.find(p_QuestID) != m_Quests.end())
        return true;
    return false;
}

bool Player::CheckQuestObjective(eObjectifType p_EventType, int32 p_Data0)
{
    bool l_HasMadeObjectif = false;
    for (std::map< uint16, Quest* >::iterator l_It = m_Quests.begin(); l_It != m_Quests.end(); ++l_It)
    {
        std::vector<std::string> l_ListMsg = (*l_It).second->CheckAtEvent(p_EventType, p_Data0);
        for (std::vector<std::string>::iterator l_It = l_ListMsg.begin(); l_It != l_ListMsg.end(); ++l_It)
        {
            PacketWarningMsg l_Packet;
            l_Packet.BuildPacket(eTypeWarningMsg::Yellow, (*l_It));
            GetSession()->send(l_Packet.m_Packet);
            l_HasMadeObjectif = true;
        }
    }
    return l_HasMadeObjectif;
}

void Player::EnterInZone(Zone* p_Zone)
{
    if (p_Zone == nullptr)
        return;

    printf("---> ENTER IN ZONE\n");
    if (p_Zone->m_Name != "")
    {
        PacketWarningMsg l_Packet;
        l_Packet.BuildPacket(eTypeWarningMsg::Yellow, p_Zone->m_Name);
        GetSession()->send(l_Packet.m_Packet);
    }
}

void Player::OutOfZone(Zone* p_Zone)
{
    if (p_Zone == nullptr)
        return;

    printf("---> OUT OF ZONE\n");
    if (p_Zone->m_Name != "")
    {
        PacketWarningMsg l_Packet;
        l_Packet.BuildPacket(eTypeWarningMsg::Yellow, "out of " + p_Zone->m_Name);
        GetSession()->send(l_Packet.m_Packet);
    }
}

std::map< eKeyBoardAction, uint8 >* Player::GetKeyBoardBinds()
{
    return &m_KeyBoardBinds;
}

void Player::AddSpellCooldown(uint16 p_SpellID, uint64 p_Time)
{
    Unit::AddSpellCooldown(p_SpellID, p_Time);

    int32 l_BindSpell = GetBindSpell(p_SpellID);
    if (l_BindSpell < 0)
        return;

    /// Block bind of spell
    PacketKeyBoardBlock l_Packet;
    l_Packet.BuildPacket((uint8)l_BindSpell, (uint16)(p_Time / 1000));
    WorldSocket* l_Session = GetSession();
    l_Session->SendPacket(l_Packet.m_Packet);
}

void Player::AddSpellBindToKey(uint16 p_SpellID, uint8 p_Bind)
{
    m_SpellsBindToKey[p_SpellID] = p_Bind;
}

int32 Player::GetSpellOnBind(uint8 p_Bind)
{
    int32 l_Spell = -1;
    for (std::map<uint16, uint8>::iterator l_It = m_SpellsBindToKey.begin();  l_It != m_SpellsBindToKey.end(); ++l_It)
    {
        if ((*l_It).second == p_Bind)
        {
            l_Spell = (*l_It).first;
            break;
        }
    }

    return l_Spell;
}

int32 Player::GetBindSpell(uint16 p_SpellID)
{
    if (m_SpellsBindToKey.find(p_SpellID) == m_SpellsBindToKey.end())
        return -1;
    return m_SpellsBindToKey[p_SpellID];
}

void Player::SetInLoading(bool p_InLoading)
{
    m_InLoading = p_InLoading;
    if (!p_InLoading)
        return;

    PacketLoadingPing l_Packet;
    l_Packet.BuildPacket();
    WorldSocket* l_Session = GetSession();
    l_Session->SendPacket(l_Packet.m_Packet);
}

bool Player::GetInLoading() const
{
    return m_InLoading;
}

void Player::SetPointsSet(const PointsSet & p_PointSet)
{
    Unit::SetPointsSet(p_PointSet);

    for (uint8 i = 0; i < eStats::MaxStat; ++i)
    {
        if (i == eStats::Speed)
            continue;
        PacketUnitUpdateStat l_Packet;
        l_Packet.BuildPacket(GetType(), GetID(), i, GetPointsSet().GetStat((eStats)i));
        GetSession()->send(l_Packet.m_Packet);
    }
}

bool Player::AddPointsStat(eStats p_TypeStat, uint8 p_Nb)
{
    if (!Unit::AddPointsStat(p_TypeStat, p_Nb))
        return false;

    if (p_TypeStat != eStats::Free)
    {
        PacketUnitUpdateStat l_Packet;
        l_Packet.BuildPacket(GetType(), GetID(), p_TypeStat, GetPointsSet().GetStat(p_TypeStat));
        GetSession()->send(l_Packet.m_Packet);
    }
    PacketUnitUpdateStat l_Packet2;
    l_Packet2.BuildPacket(GetType(), GetID(), eStats::Free, GetPointsSet().GetStat(eStats::Free));
    GetSession()->send(l_Packet2.m_Packet);
    return true;
}

bool Player::SubPointsStat(eStats p_TypeStat, uint8 p_Nb)
{
    if (!Unit::SubPointsStat(p_TypeStat, p_Nb))
        return false;

    PacketUnitUpdateStat l_Packet;
    l_Packet.BuildPacket(GetType(), GetID(), p_TypeStat, GetPointsSet().GetStat(p_TypeStat));
    GetSession()->send(l_Packet.m_Packet);
    PacketUnitUpdateStat l_Packet2;
    l_Packet2.BuildPacket(GetType(), GetID(), eStats::Free, GetPointsSet().GetStat(eStats::Free));
    GetSession()->send(l_Packet2.m_Packet);
    return true;
}
