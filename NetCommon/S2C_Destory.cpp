#include "pch.h"
#include "S2C_Destory.h"

void S2C_Destory::Parse(std::string InString)
{
    JSONDocument.Parse(InString.c_str());

    ClientSocket = JSONDocument["ClientSocket"].GetInt();
}

std::string S2C_Destory::ToString()
{
    return std::string();
}
