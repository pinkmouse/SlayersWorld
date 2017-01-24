#include "Required.hpp"

Required::Required()
{

}

Required::~Required()
{
}

void Required::AddSubRequired(SubRequired p_SubRequired)
{
    m_SubRequiredList.push_back(p_SubRequired);
}

bool Required::IsValid(const Player* p_Player) const
{
    for (SubRequired l_SubRequired : m_SubRequiredList)
    {
        if (!l_SubRequired.IsValid(p_Player))
            return false;
    }
    return true;
}