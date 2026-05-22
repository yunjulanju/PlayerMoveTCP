#include "pch.h"
#include "S2C_Spawn.h"

void S2C_Spawn::Parse(std::string InString)
{
    JSONDocument.Parse(InString.c_str());

    ClientSocket = JSONDocument["ClientSocket"].GetInt();
    X = JSONDocument["X"].GetInt();
    Y = JSONDocument["Y"].GetInt();
    Shape = JSONDocument["Shape"].GetInt();
    r = JSONDocument["r"].GetInt();
    g = JSONDocument["g"].GetInt();
    b = JSONDocument["b"].GetInt();
    a = JSONDocument["a"].GetInt();

}

std::string S2C_Spawn::ToString()
{
    JSONDocument.SetObject();
    JSONDocument.AddMember("ClientSocket", ClientSocket, JSONDocument.GetAllocator());
    JSONDocument.AddMember("X", X, JSONDocument.GetAllocator());
    JSONDocument.AddMember("Y", Y, JSONDocument.GetAllocator());
    JSONDocument.AddMember("Shape", Shape, JSONDocument.GetAllocator());

    JSONDocument.AddMember("r", r, JSONDocument.GetAllocator());
    JSONDocument.AddMember("g", g, JSONDocument.GetAllocator());
    JSONDocument.AddMember("b", b, JSONDocument.GetAllocator());
    JSONDocument.AddMember("a", a, JSONDocument.GetAllocator());

    rapidjson::StringBuffer Buffer;
    rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
    JSONDocument.Accept(Writer);

    return Buffer.GetString();
}
