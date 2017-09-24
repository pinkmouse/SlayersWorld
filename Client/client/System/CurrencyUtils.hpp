#pragma once

#include "../Define.hpp"

struct CurrencyBasicConvert
{
    uint16 m_Pc;
    uint16 m_Pa;
    uint16 m_Po;
};

static CurrencyBasicConvert ConvertPcToBasicCurrency(const uint16 & p_Value)
{
    CurrencyBasicConvert l_Result;
    l_Result.m_Pc = p_Value % 100;
    l_Result.m_Pa = p_Value / 100;
    l_Result.m_Po = l_Result.m_Pa / 100;
    l_Result.m_Pa = l_Result.m_Pa - (l_Result.m_Po * 100);
    return l_Result;
}

static std::string CurrencyToString(const CurrencyBasicConvert & p_Currency)
{
    std::string l_Result = "";
    if (p_Currency.m_Po > 0)
        l_Result += std::to_string(p_Currency.m_Po) + "po ";
    if (p_Currency.m_Po > 0 || p_Currency.m_Pa > 0)
        l_Result += std::to_string(p_Currency.m_Pa) + "pa ";
    if (p_Currency.m_Po > 0 || p_Currency.m_Pa > 0 || p_Currency.m_Pc > 0)
        l_Result += std::to_string(p_Currency.m_Pc) + "pc";
    return l_Result;
}