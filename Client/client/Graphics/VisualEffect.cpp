#include "VisualEffect.hpp"

VisualEffect::VisualEffect(eVisualType p_Type, uint8 p_ID, uint8 p_NbFrames) :
    m_Type(p_Type),
    m_ID(p_ID),
    m_NbFrames(p_NbFrames)
{
    m_DiffTimeAnim = 0;
    m_ActualFrame = 0;
    m_Started = false;
    m_Show = false;
    m_IsFinish = false;

    m_TypeAnim = eTypeAnim::AnimLoop;
    switch (p_Type)
    {
    case eVisualType::VisualSpell:
        m_TimerUpdate = 250000; /// 0.25s
        break;
    default:
        m_TimerUpdate = 500000; /// 0.5s
        break;

    }
}

VisualEffect::~VisualEffect()
{
}

void VisualEffect::Update(sf::Time p_Time)
{
    if (!m_Started || m_IsFinish)
        return;

    m_DiffTimeAnim += p_Time.asMicroseconds();

    /// ANIM FINISH
    if (m_DiffTimeAnim >= m_TimerUpdate * (m_ActualFrame + 1))
    {
        m_ActualFrame += 1;

        if (m_ActualFrame >= m_NbFrames)
        {
            m_ActualFrame -= 1;
            switch (m_TypeAnim)
            {
            case eTypeAnim::AnimLoop:
                m_ActualFrame = 0;
                m_DiffTimeAnim = 0;
                break;
            default:
                m_IsFinish = true;
                break;
            }
        }
    }
}

uint8 VisualEffect::GetID() const
{
    return m_ID;
}

void VisualEffect::StartAnim()
{
    m_TypeAnim = eTypeAnim::AnimLoop;
    m_Started = true;
}

void VisualEffect::StartAnimAndStop()
{
    m_TypeAnim = eTypeAnim::AnimAndStop;
    m_Started = true;
}

void VisualEffect::StartAnimAndShow() /// TODO
{
    m_TypeAnim = eTypeAnim::AnimAndShow;
    m_Started = true;
}

uint8 VisualEffect::GetFrame()
{
    return (uint8)m_ActualFrame;
}

eVisualType VisualEffect::GetType() const
{
    return m_Type;
}

bool VisualEffect::HasStarted() const
{
    return m_Started;
}

bool VisualEffect::HasFinishAnim() const
{
    return m_IsFinish;
}