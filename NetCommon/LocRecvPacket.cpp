#include "pch.h"
#include "LocRecvPacket.h"
#include <iostream>
#include <sstream>

using namespace std;

void LocRecvPacket::Parse(std::string InString)
{
    JSONDocument.Parse(InString.c_str());
    UserID = JSONDocument["UserID"].GetString();
    Num = JSONDocument["Num"].GetInt();

    // Input 타입 확인
    if (JSONDocument["Input"].IsString()) {
        Input = JSONDocument["Input"].GetString()[0];
    }
    else if (JSONDocument["Input"].IsInt()) {
        Input = (char)JSONDocument["Input"].GetInt();  // 숫자면 char로 변환
    }
}

std::string LocRecvPacket::ToString()
{
    JSONDocument.SetObject();

    JSONDocument.AddMember("Num", Num, JSONDocument.GetAllocator());

    rapidjson::Value userID;
    userID.SetString(UserID.c_str(), UserID.length(), JSONDocument.GetAllocator());
    JSONDocument.AddMember("UserID", userID, JSONDocument.GetAllocator());

    rapidjson::Value input;
    std::string InputStr(1, Input);
    input.SetString(InputStr.c_str(), InputStr.length(), JSONDocument.GetAllocator());
    JSONDocument.AddMember("Input", input, JSONDocument.GetAllocator());

    rapidjson::StringBuffer Buffer;
    rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
    JSONDocument.Accept(Writer);

    return Buffer.GetString();
}

int LocRecvPacket::Length()
{
    return (int)ToString().length();
}
