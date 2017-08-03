#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include "Visual.hpp"

enum eTypeAnim
{
    AnimLoop = 1,
    AnimAndStop = 2,
    AnimAndShow = 3
};

class VisualEffect
{
public:
    VisualEffect(eVisualType, bool, uint8, uint8);
    ~VisualEffect();
    void Update(sf::Time);
    uint8 GetFrame();
    uint8 GetMaxFrame() const;
    void StartAnim();
    void StartAnimAndStop();
    void StartAnimAndShow();
    bool HasStarted() const;
    bool HasFinishAnim() const;
    uint8 GetID() const;
    eVisualType GetType() const;
    bool IsUnder() const;

private:
    eVisualType m_Type;
    eTypeAnim m_TypeAnim;
    uint8 m_ID;

    bool m_Started;
    bool m_Show;
    bool m_IsFinish;

    uint8 m_ActualFrame;
    uint8 m_NbFrames;

    bool m_Under;
    uint32 m_TimerUpdate;
    uint64 m_DiffTimeAnim;
};

