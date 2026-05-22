#pragma once
#include "Packet.h"
class S2C_Spawn : public IPacket
{
public:

	SOCKET ClientSocket;
	int X;
	int Y;
	char Shape = ' ';
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	// Inherited via IPacket
	void Parse(std::string InString) override;
	std::string ToString() override;
};

