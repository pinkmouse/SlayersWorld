#pragma once
#include "../../../Map/Map.hpp"
#define NB_PLAYERS_CAPTURE_POINT 20
#define TIME_START_BG 30
#define BG_MAX_TIME (120 + TIME_START_BG)

class BGCapturePoint : public Map
{
public:
    BGCapturePoint(uint16, uint16);
    ~BGCapturePoint();
    void Update(sf::Time);
    void UpdateBGStep1(sf::Time);
    void AddUnit(Unit*);
    void RemoveUnit(Unit*);
    void OpenGraveyard();
    void StartBG();
    void Chrono();
    void ChronoPeriodic();
    void EndBG();
    bool IsFinish();

private:
    bool    m_Finished;
    uint64 m_GlobalTimes;
    eBGState m_Step;
    uint8 m_Chrono;
    std::map<uint16, WorldPosition> m_SaveRespawnPoint;
};