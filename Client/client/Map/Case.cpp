#include "Case.hpp"

Case::Case(uint32 p_ID, uint16 p_X, uint16 p_Y) :
    WorldObject(TypeWorldObject::CASE, TILE_SIZE, TILE_SIZE),
	m_ID(p_ID),
	m_Block(false)
{
    SetPosX(p_X * TILE_SIZE);
    SetPosY(p_Y * TILE_SIZE);
	for (uint8 i = 0; i < m_TileList.size(); ++i)
		m_TileList[i] = -1;
    m_DynamicObjectList.clear();
}

Case::~Case()
{
}

int16 Case::GetTile(uint8 p_Level) const
{
	return m_TileList[p_Level];
}

bool Case::IsBlocking() const
{
    if (m_Block)
        return true;

    for (uint8 i = 0; i < m_DynamicObjectList.size(); ++i)
    {
        if (m_DynamicObjectList[i] != nullptr && m_DynamicObjectList[i]->IsBlocking())
            return true;
    }
    return false;
}

void Case::SetTile(uint8 p_Level, int16 p_Value)
{
	m_TileList[p_Level] = p_Value;
}

void Case::SetBlock(bool p_Block)
{
	m_Block = p_Block;
}

uint8 Case::GetMaxTileLevel()
{
	return (uint8)m_TileList.size();
}

void Case::AddDynamicOject(DynamicObject* p_DynamicObject)
{
    m_DynamicObjectList.push_back(p_DynamicObject);
}

void Case::RemoveDynamicOject(DynamicObject* p_DynamicObject)
{
    if (p_DynamicObject == nullptr)
        return;

    std::vector<DynamicObject*>::iterator l_It = std::find(m_DynamicObjectList.begin(), m_DynamicObjectList.end(), p_DynamicObject);

    if (l_It != m_DynamicObjectList.end())
    {
        DynamicObject* l_DynObj = (*l_It);
        m_DynamicObjectList.erase(l_It);
    }
}

uint32 Case::GetID() const
{
    return m_ID;
}
