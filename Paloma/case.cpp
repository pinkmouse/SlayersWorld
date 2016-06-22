#include "case.h"

Case::Case(int p_ID) :
    m_ID(p_ID),
    m_Block(false)
{
    for (int i = 0; i < m_TileList.size(); ++i)
        m_TileList[i] = nullptr;
}
