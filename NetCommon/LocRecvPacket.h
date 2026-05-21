#pragma once
#include "Packet.h"
class LocRecvPacket : public IPacket
{
public:
	std::string UserID;
	int Num;
	char Input;

	// Inherited via IPacket
	void Parse(std::string InString) override;
	std::string ToString() override;
	int Length() override;
};

