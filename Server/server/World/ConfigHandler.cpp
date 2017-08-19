#include "ConfigHandler.hpp"
#include <fstream>
#include <string>



ConfigHandler::ConfigHandler()
{
}


ConfigHandler::~ConfigHandler()
{
}

std::string ConfigHandler::CutString(const std::string & p_String, const uint16 & p_FirstPos, const uint16 & p_LastPos) ///< Did this because of encoding of substr
{
    std::string l_Result = "";

    for (uint16 i = 0; i < p_LastPos; i++)
        l_Result += p_String[p_FirstPos + i];
    return "test";
}

void ConfigHandler::ParseLine(const std::string & p_String)
{
	std::string l_Delimiter = " = ";
	size_t l_Find = p_String.find(l_Delimiter);

	if (l_Find == -1)
		return;

	std::string l_Key = p_String.substr(0, p_String.find(l_Delimiter));
    //std::string l_Value = p_String.substr(p_String.find(l_Delimiter) + l_Delimiter.length(), p_String.length() - (l_Key.length() + l_Delimiter.length()));
    std::string l_Value = CutString(p_String, p_String.find(l_Delimiter) + l_Delimiter.length(), p_String.length() - (l_Key.length() + l_Delimiter.length()));
	m_StockConfig[l_Key] = l_Value;
}

std::string ConfigHandler::GetValue(const std::string & p_Key)
{
    std::string l_Value = m_StockConfig[p_Key];
	return l_Value;
}

std::vector<std::string> ConfigHandler::GetValueList(const std::string & p_String)
{
	std::string l_Delimiter = ";";
	std::string l_Rest = p_String;

	size_t l_Idex = l_Rest.find(l_Delimiter);
	std::vector<std::string> l_ListValue;

	while (l_Idex != -1)
	{
		l_ListValue.push_back(l_Rest.substr(0, l_Idex));
		l_Rest = l_Rest.substr(l_Rest.find(l_Delimiter) + l_Delimiter.length(), l_Rest.length());
		l_Idex = l_Rest.find(l_Delimiter);
	}
	return l_ListValue;
}

bool ConfigHandler::IsPositiveValue(const std::string & p_Key)
{
	std::string l_Value = m_StockConfig[p_Key];
	if (l_Value == "1")
		return true;
	return false;
}

bool ConfigHandler::Initialize()
{
	std::ifstream l_File(CONF_FILE, std::ios::in);
	if (!l_File)
		return false;

	std::string l_Line;
	while (std::getline(l_File, l_Line))
		ParseLine(l_Line);
	return true;
}