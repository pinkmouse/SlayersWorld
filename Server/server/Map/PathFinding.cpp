#include "Map.hpp"

bool Map::AlreadyInList(const Position & p_Pos, const ListNode & p_ListNode)
{
    if (p_ListNode.find(p_Pos) == p_ListNode.end())
        return false;
    return true;
}

void Map::AddAdjacentCases(const Position & p_Pos)
{
    PFNode l_NodeTmp;

    for (int64 i = p_Pos.m_X - 1; i <= p_Pos.m_X + 1; i++)
    {
        if ((i < 0) || (i >= m_SizeX))
            continue;

        for (int64 j = p_Pos.m_Y - 1; j <= p_Pos.m_Y + 1; j++)
        {
            /* Revome diagonal */
            if (i == p_Pos.m_X - 1 && j == p_Pos.m_Y - 1) /// Top Left
                continue;
            if (i == p_Pos.m_X + 1 && j == p_Pos.m_Y + 1) /// Bottom Right
                continue;
            if (i == p_Pos.m_X + 1 && j == p_Pos.m_Y - 1) /// Top Right
                continue;
            if (i == p_Pos.m_X - 1 && j == p_Pos.m_Y + 1) /// Bottom Left
                continue;
            if ((j < 0) || (j >= m_SizeY))
                continue;
            /******************/

            Position l_Pos((uint32)i, (uint32)j);

            Case* l_Case = GetCase(l_Pos.m_X * TILE_SIZE, l_Pos.m_Y * TILE_SIZE);

            if (l_Case == nullptr || l_Case->IsBlocking() || !l_Case->CanBeWalk())
                continue;

            if (!AlreadyInList(l_Pos, m_CloseList))
            {
                l_NodeTmp.m_CostG = m_CloseList[p_Pos].m_CostG + l_Case->GetDistanceEuclidienne(p_Pos);
                l_NodeTmp.m_CostH = l_Case->GetDistanceEuclidienne(m_EndPosition);
                l_NodeTmp.m_CostF = l_NodeTmp.m_CostG + l_NodeTmp.m_CostH;
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

const Position Map::BestNode(ListNode & p_ListNode)
{
    uint32 l_CostF = p_ListNode.begin()->second.m_CostF;
    Position l_Pos = p_ListNode.begin()->first;

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

void Map::AddToCloseList(const Position & p_Pos)
{
    PFNode& l_Node = m_OpenList[p_Pos];

    m_CloseList[p_Pos] = l_Node;
    m_OpenList.erase(p_Pos);
}

Path Map::GetPath()
{
    Path l_Path;

    PFNode& l_TmpNode = m_CloseList[m_EndPosition];
    Position l_Before = l_TmpNode.m_PosParent;

    l_Path.push_back(m_EndPosition);
    while (l_Before != m_StartPosition)
    {
        l_Path.push_back(l_Before);
        l_TmpNode = m_CloseList[l_TmpNode.m_PosParent];
        l_Before = l_TmpNode.m_PosParent;
        if (l_Path.size() > 500) /// !!!!!!! A revoir
        {
            //printf("######### [%d-%d] to [%d-%d] more than 500 ways !\n", m_StartPosition.m_X, m_StartPosition.m_Y, m_EndPosition.m_X, m_EndPosition.m_Y);
            l_Path.clear();
            return l_Path;
        }
    }
    l_Path.push_back(l_Before);
    return l_Path;
}

Path Map::LaunchPathFinding(const Position & p_PosStart, const Position & p_PosEnd)
{
    m_OpenList.clear();
    m_CloseList.clear();

    m_StartPosition = p_PosStart;
    m_EndPosition = p_PosEnd;

    PFNode l_Start;
    Position l_Current = m_StartPosition;
    l_Start.m_PosParent = l_Current;
    m_OpenList[l_Current] = l_Start;

    if (GetCase(p_PosEnd.m_X * TILE_SIZE, p_PosEnd.m_Y * TILE_SIZE)->IsBlocking())
        return Path();

    AddToCloseList(l_Current);
    AddAdjacentCases(l_Current);

    //printf("######### [%d-%d] to [%d-%d] \n", p_PosStart.m_X, p_PosStart.m_Y, p_PosEnd.m_X, p_PosEnd.m_Y);

    while ((l_Current != m_EndPosition) && (!m_OpenList.empty()))
    {
        l_Current = BestNode(m_OpenList);
        AddToCloseList(l_Current);
        AddAdjacentCases(l_Current);
    }

    return GetPath();
}