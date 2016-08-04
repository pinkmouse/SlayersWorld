#pragma once
#include "../Define.hpp"

class Case
{
public:
	Case(uint16, uint16, uint16);
	~Case();
	void SetBlock(bool);
    bool IsBlocking() const;

private:
	bool m_Block;
	uint16 m_ID;
	uint16 m_X;
	uint16 m_Y;
};

