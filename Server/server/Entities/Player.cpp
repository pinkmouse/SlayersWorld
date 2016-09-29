#include "Player.hpp"
#include "../World/WorldSocket.hpp"
#include "../Map/Map.hpp"

Player::Player(int32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_Health, uint8 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, Orientation p_Orientation) :
    Unit(p_ID, TypeUnit::PLAYER)
{
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
    printf("Erase Player %d:%s", m_ID, m_Name.c_str());
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
                    return;

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

void Player::SetHealth(const uint8 & p_Health)
{
    Unit::SetHealth(p_Health);
    if (m_Initilize)
        m_Session->SendUpdateUnitHealth(GetType(), GetID(), p_Health);
}

void Player::Respawn()
{
    Unit::Respawn();
    GetSession()->SendUpdatePositionToSet(GetType(), GetID(), GetPosX(), GetPosY(), GetOrientation());

    SetHealth(MAX_HEALTH);
    m_ResTimer = 0;
}