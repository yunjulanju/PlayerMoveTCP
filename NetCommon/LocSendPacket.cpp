#include "pch.h"
#include "LocSendPacket.h"
#include <sstream>

void LocSendPacket::Parse(std::string InString)
{
    JSONDocument.Parse(InString.c_str());

    UserID = JSONDocument["UserID"].GetString();
    Num = JSONDocument["Num"].GetInt();
    X = JSONDocument["X"].GetInt();
    Y = JSONDocument["Y"].GetInt();
}

std::string LocSendPacket::ToString()
{
    //JSONDocument를 문자열 변환 요청
    JSONDocument.SetObject();
    JSONDocument.AddMember("UserID", UserID, JSONDocument.GetAllocator());
    JSONDocument.AddMember("Num", Num, JSONDocument.GetAllocator());
    JSONDocument.AddMember("X", X, JSONDocument.GetAllocator());
    JSONDocument.AddMember("Y", Y, JSONDocument.GetAllocator());

    rapidjson::StringBuffer Buffer;
    rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
    JSONDocument.Accept(Writer);

    return Buffer.GetString();
}

int LocSendPacket::Length()
{
    return (int)ToString().length();
}
