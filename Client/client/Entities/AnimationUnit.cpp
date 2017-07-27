#include "AnimationUnit.hpp"

AnimationUnit::AnimationUnit(int32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_Health, int16 p_SkinID, uint8 p_SizeX, uint8 p_SizeY, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, Orientation p_Orientation) :
    Unit(p_ID, TypeUnit::ANIMATIONUNIT, p_SizeX, p_SizeY)
{
    m_Name = p_Name;
    m_Level = p_Level;
    m_SkinID = p_SkinID;
    SetPosX(p_PosX);
    SetPosY(p_PosY);
    m_MovementHandler->SetPosX(p_PosX);
    m_MovementHandler->SetPosY(p_PosY);
    m_MovementHandler->SetOrientation(p_Orientation);
    SetResourceNb(eResourceType::Health, p_Health);
    SetMapID(p_MapID);
    SetSpeed(0.5f);
    m_FrameAnim = 0;
    m_DiffAnim = 0;
    m_IsInAnimation = false;
}


AnimationUnit::~AnimationUnit()
{
}

void AnimationUnit::LaunchAnim()
{
    m_IsInAnimation = true;
    m_FrameAnim = 0;
    m_DiffAnim = 0;
    SetOrientation(Orientation::Down);
}

bool AnimationUnit::IsInAnimation() const
{
    return m_IsInAnimation;
}

uint8 AnimationUnit::GetFrame() const
{
    return m_FrameAnim;
}


void AnimationUnit::Update(sf::Time p_Diff)
{
    if (!m_IsInAnimation)
        return;

    m_DiffAnim += p_Diff.asMicroseconds();

    while (m_DiffAnim > ((UPDATE_TIME_MOVEMENT / GetSpeed()) * 1000)) ///< 1000 because microsecond
    {
        m_FrameAnim++;
        if (m_FrameAnim >= 12)
        {
            m_IsInAnimation = false;
            m_DiffAnim = 0;
            m_FrameAnim = 0;
            break;
        }
        m_DiffAnim -= (UPDATE_TIME_MOVEMENT / GetSpeed()) * 1000;
    }
}
