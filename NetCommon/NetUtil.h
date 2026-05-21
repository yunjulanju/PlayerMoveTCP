#pragma once

#include "pch.h"
#include <string>
#include <map>

struct ClientData {
	std::string ID;
	int Num;
	int X;
	int Y;
};

extern int SendAll(SOCKET ReceiverSocket, const char* Data, int Size);

extern void DisconnectSocket(SOCKET DisconnectedSocket, fd_set* Sockets);

extern void PrintCanvas(std::map<SOCKET, ClientData>& Clients);