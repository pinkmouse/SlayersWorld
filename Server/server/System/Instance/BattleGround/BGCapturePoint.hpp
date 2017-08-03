#pragma once
#include "../../../Map/Map.hpp"
#define NB_PLAYERS_CAPTURE_POINT 20
#define TIME_START_BG 45
#define BG_MAX_TIME (600 + TIME_START_BG)

#define MAX_FOINT 3

#define TEAM_RIGHT_NAME "Corsaire"
#define TEAM_LEFT_NAME "Pirate"

#define TEAM_RIGHTID 0
#define TEAM_LEFTID 1

#define BASE_FLAG_RIGHT 1572
#define AURA_FLAG_RIGHT 9
#define GOB_FLAG_RIGHT 15

#define BASE_FLAG_LEFT 1529
#define AURA_FLAG_LEFT 10
#define GOB_FLAG_LEFT 16


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
    void UnitUnaura(Unit*, const uint16 &);
    void UnitAddaura(Unit*, const uint16 &, Aura*);
    void PopFlagAtPoint(const uint32 &, GameObject*);
    bool LauchTrapHandle(GameObject*, Unit*);
    bool IsBattleground() { return true; };
    void TeamMakePoint(const uint8 &);

private:
    bool    m_Finished;
    uint64 m_GlobalTimes;
    eBGState m_Step;
    uint8 m_Chrono;
    std::map<uint16, WorldPosition> m_SaveRespawnPoint;
    GameObject* m_FlagRight;
    GameObject* m_FlagLeft;
    uint8 m_PointsRight;
    uint8 m_PointsLeft;
};