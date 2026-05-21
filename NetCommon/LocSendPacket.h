#pragma once
#include "Packet.h"
class LocSendPacket : public IPacket
{
public:
	std::string UserID;
	int Num;
	int X;
	int Y;

	// Inherited via IPacket
	void Parse(std::string InString) override;
	std::string ToString() override;
	int Length() override;
};

