#include "BGManager.hpp"
#include "BGCapturePoint.hpp"

BGManager::BGManager()
{
}

BGManager::~BGManager()
{
}

void BGManager::StartBG(TypeBG p_TypeBG, std::vector<Player*> p_ListPlayers)
{
    if (p_TypeBG == TypeBG::PointCapture)
        m_BGInProgress[p_TypeBG].push_back(new BGCapturePoint(p_ListPlayers));
}

std::vector<Player*> BGManager::GetPlayersToStartBG(TypeBG p_TypeBG, uint8 p_NbPlayers)
{
    std::vector<Player*> l_ListPlayers;
    std::vector<Player*>::iterator l_It;

    for (l_It = m_Queue[p_TypeBG].begin(); l_It != m_Queue[p_TypeBG].end();)
    {
        l_ListPlayers.push_back((*l_It));
        if (l_ListPlayers.size() >= p_NbPlayers)
            break;

        l_It = m_Queue[p_TypeBG].erase(l_It);
    }

    return l_ListPlayers;
}

void BGManager::CheckQueues()
{
    for (auto m_SubQueue : m_Queue)
    {
        if (m_SubQueue.first == TypeBG::PointCapture && m_SubQueue.second.size() >= NB_PLAYERS_CAPTURE_POINT)
            StartBG(m_SubQueue.first, GetPlayersToStartBG(m_SubQueue.first, NB_PLAYERS_CAPTURE_POINT));
    }
}


void BGManager::Update(sf::Time p_Time)
{
    CheckQueues();
}

void BGManager::RemovePlayerFromQueue(TypeBG p_TypeBG, Player* p_Player)
{
    std::vector<Player*>::iterator l_It = std::find(m_Queue[p_TypeBG].begin(), m_Queue[p_TypeBG].end(), p_Player);

    if (l_It == m_Queue[p_TypeBG].end())
        return;

    m_Queue[p_TypeBG].erase(l_It);
}

void BGManager::RemovePlayerFromQueue(Player* p_Player)
{
    std::vector<Player*>::iterator l_It;
    for (auto m_SubQueue : m_Queue)
    {
        l_It = std::find(m_SubQueue.second.begin(), m_SubQueue.second.end(), p_Player);
        if (l_It != m_SubQueue.second.end())
            m_SubQueue.second.erase(l_It);
    }
}

void BGManager::AddPlayerToQueue(TypeBG p_TypeBG, Player* p_Player)
{
    if (std::find(m_Queue[p_TypeBG].begin(), m_Queue[p_TypeBG].end(), p_Player) != m_Queue[p_TypeBG].end())
        return;

    m_Queue[p_TypeBG].push_back(p_Player);
}

std::vector<TypeBG> BGManager::PlayerIsInQueue(Player* p_Player)
{
    std::vector<TypeBG> l_ListBG;
    for (auto m_SubQueue : m_Queue)
    {
        if (std::find(m_SubQueue.second.begin(), m_SubQueue.second.end(), p_Player) != m_SubQueue.second.end())
            l_ListBG.push_back(m_SubQueue.first);
    }
    return l_ListBG;
}