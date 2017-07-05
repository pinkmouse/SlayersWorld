#pragma once
#include "../../Define.hpp"
#include "BG.hpp"

class BGManager
{
public:
    BGManager();
    ~BGManager();
    void Update(sf::Time);
    void AddPlayerToQueue(TypeBG, Player*);
    std::vector<TypeBG> PlayerIsInQueue(Player*);
    void CheckQueues();
    void RemovePlayerFromQueue(TypeBG, Player*);
    void RemovePlayerFromQueue(Player*);
    std::vector<Player*> GetPlayersToStartBG(TypeBG, uint8);
    void StartBG(TypeBG, std::vector<Player*>);

private:
    std::map< TypeBG, std::vector<BG*> > m_BGInProgress;
    std::map< TypeBG, std::vector<Player*> > m_Queue;
};