#include "Map.hpp"

float Map::GetDistanceCase(const PFNodePosition & p_Pos1, const PFNodePosition & p_Pos2)
{
    return sqrt((p_Pos1.first - p_Pos2.first) * (p_Pos1.first - p_Pos2.first) + (p_Pos1.second - p_Pos2.second) * (p_Pos1.second - p_Pos2.second));
}

bool Map::AlreadyInList(const PFNodePosition & p_Pos, const ListNode & p_ListNode)
{
    if (p_ListNode.find(p_Pos) == p_ListNode.end())
        return false;
    return true;
}

void Map::AddAdjacentCases(const PFNodePosition & p_Pos)
{
    PFNode l_NodeTmp;

    for (int32 i = p_Pos.first - 1; i <= p_Pos.first + 1; i++)
    {
        if ((i < 0) || (i >= m_SizeX))
            continue;

        for (int32 j = p_Pos.second - 1; j <= p_Pos.second + 1; j++)
        {
            if ((j < 0) || (j >= m_SizeY))
                continue;

            Case* l_Case = GetCase(i * TILE_SIZE, j * TILE_SIZE);
            if (l_Case == nullptr || l_Case->IsBlocking())
                continue;

            PFNodePosition l_Pos(i, j);

            if (!AlreadyInList(l_Pos, m_CloseList))
            {
                l_NodeTmp.m_CostG = m_CloseList[p_Pos].m_CostG + l_Case->GetDistanceEuclidienne(Position(p_Pos.first, p_Pos.second));
                l_NodeTmp.m_CostH = l_Case->GetDistanceEuclidienne(Position(m_EndPosition.first, m_EndPosition.second));
                l_NodeTmp.m_CostF = l_NodeTmp.m_CostG + l_NodeTmp.m_CostF;
                l_NodeTmp.m_PosParent = p_Pos;

                if (AlreadyInList(l_Pos, m_OpenList))
                {
                    if (l_NodeTmp.m_CostF < m_OpenList[l_Pos].m_CostF)
                        m_OpenList[l_Pos] = l_NodeTmp;
                }
                else
                    m_OpenList[l_Pos] = l_NodeTmp;
            }
        }
    }
}

const PFNodePosition Map::BestNode(ListNode & p_ListNode)
{
    float l_CostF = p_ListNode.begin()->second.m_CostF;
    PFNodePosition l_Pos = p_ListNode.begin()->first;

    for (ListNode::iterator l_It = p_ListNode.begin(); l_It != p_ListNode.end(); l_It++)
    {
        if (l_It->second.m_CostF < l_CostF)
        {
            l_CostF = l_It->second.m_CostF;
            l_Pos = l_It->first;
        }
    }

    return l_Pos;
}

void Map::AddToCloseList(const PFNodePosition & p_Pos)
{
    PFNode& l_Node = m_OpenList[p_Pos];

    m_CloseList[p_Pos] = l_Node;
    m_OpenList.erase(p_Pos);
}

std::vector<Position> Map::GetPath()
{
    std::vector<Position> l_Path;

    PFNode& l_TmpNode = m_CloseList[m_EndPosition];
    PFNodePosition l_Before = l_TmpNode.m_PosParent;
    while (l_Before != m_StartPosition)
    {
        l_Path.push_back(Position(l_Before.first, l_Before.second));
        l_TmpNode = m_CloseList[l_TmpNode.m_PosParent];
        l_Before = l_TmpNode.m_PosParent;
    }
    return l_Path;
}

std::vector<Position> Map::LaunchPathFinding(const Position & p_PosStart, const Position & p_PosEnd)
{
    m_OpenList.clear();
    m_CloseList.clear();

    m_StartPosition.first = p_PosStart.m_X;
    m_StartPosition.second = p_PosStart.m_Y;
    m_EndPosition.first = p_PosEnd.m_X;
    m_EndPosition.second = p_PosEnd.m_Y;

    PFNode l_Start;
    PFNodePosition l_Current = m_StartPosition;
    l_Start.m_PosParent = l_Current;
    m_OpenList[l_Current] = l_Start;
    AddToCloseList(l_Current);
    AddAdjacentCases(l_Current);

    while ((l_Current != m_EndPosition) && (!m_OpenList.empty()))
    {
        l_Current = BestNode(m_OpenList);

        AddToCloseList(l_Current);
        AddAdjacentCases(l_Current);
    }

    return GetPath();
}
