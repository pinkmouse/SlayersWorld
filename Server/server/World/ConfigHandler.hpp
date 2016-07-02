#pragma once

#include "../Define.hpp"
#include <map>
#include <vector>

class ConfigHandler
{
public:
	ConfigHandler();
	~ConfigHandler();
	void ParseLine(const std::string & p_String);
	bool Initialize();
	std::string GetValue(const std::string &);
	std::vector<std::string> GetValueList(const std::string &);

private:
	std::map < std::string, std::string > m_StockConfig;
};

