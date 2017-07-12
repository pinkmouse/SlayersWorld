#pragma once
#include "../../../Map/Map.hpp"
#define NB_PLAYERS_CAPTURE_POINT 20

class BGCapturePoint : public Map
{
public:
    BGCapturePoint(uint16, uint16);
    ~BGCapturePoint();
    void Update(sf::Time);
    void AddUnit(Unit*);
    void RemoveUnit(Unit*);
    void StartBG();
    void EndBG();
    bool IsFinish();

private:
    bool    m_Finished;
    uint64 m_GlobalTimes;
    eBGState m_Step;
};