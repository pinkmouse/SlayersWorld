#include "Player.hpp"
#include "../World/WorldSocket.hpp"
#include "../World/PacketDefine.hpp"
#include "../Map/Map.hpp"
#include "../Global.hpp"

Player::Player(int32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_Health, uint8 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, Orientation p_Orientation, uint32 p_Xp) :
    Unit(p_ID, TypeUnit::PLAYER)
{
    InitializeCommands();
    m_Name = p_Name;
    m_Level = p_Level;
    m_SkinID = p_SkinID;
    m_MapID = p_MapID;
    SetPosX(p_PosX);
    SetPosY(p_PosY);
    SetOrientation(p_Orientation);
    m_Session = nullptr;
    m_Alignment = 0;
    m_Initilize = false;
    m_Health = p_Health;
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
}

void Player::Update(sf::Time p_Diff)
{
    Unit::Update(p_Diff);
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

                GetSession()->SendUnitCreate(l_Unit->GetType(), l_Unit->GetID(), l_Unit->GetName(), l_Unit->GetLevel(), l_Unit->GetHealth(), l_Unit->GetSkinID(), l_Unit->GetMapID(), l_Unit->GetPosX(), l_Unit->GetPosY(), l_Unit->GetOrientation(), l_Unit->IsInMovement());
            }
        }
    }
}

void Player::SetAlignment(const uint8 & p_Alignment)
{
    m_Alignment = p_Alignment;
}

uint8 Player::GetAlignment() const
{
    return m_Alignment;
}

uint32 Player::GetXp() const
{
    return m_Xp;
}

void Player::SetHealth(const uint8 & p_Health)
{
    Unit::SetHealth(p_Health);
    if (m_Initilize)
        m_Session->SendUpdateUnitHealth(GetType(), GetID(), p_Health);
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

        m_Func l_Fun = m_CmdHandleMap[l_CmdList[0]];
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
    }

    float l_Pct = g_LevelManager->XpPct(GetLevel(), p_Xp);
    if (m_Initilize)
        m_Session->SendUpdateXpPct(l_Pct);
    m_Xp = p_Xp;
}

void Player::SendMsg(const std::string & p_Msg)
{
    PacketSrvPlayerMsg l_Packet;
    l_Packet.BuildPacket(p_Msg);
    WorldSocket* l_Session = GetSession();
    l_Session->SendMsg(l_Packet.m_Packet);
}

void Player::Respawn()
{
    Unit::Respawn();
    GetSession()->SendUpdatePositionToSet(GetType(), GetID(), GetPosX(), GetPosY(), GetOrientation());

    SetHealth(MAX_HEALTH);
    m_ResTimer = 0;
}