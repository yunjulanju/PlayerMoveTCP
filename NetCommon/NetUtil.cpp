#include "pch.h"
#include <map>
#include "NetUtil.h"
#include <iostream>

using namespace std;

int SendAll(SOCKET ReceiverSocket, const char* Data, int Size)
{
	int TotalSendDataSize = 0;
	int WantSendDataSize = Size;
	int SentBytes = 0;
	int Count = 0;
	do
	{
		SentBytes = send(ReceiverSocket, Data + TotalSendDataSize, WantSendDataSize - TotalSendDataSize, 0);
		TotalSendDataSize += SentBytes;
		if (SentBytes <= 0)
		{
			return SentBytes;
		}
	} while (TotalSendDataSize < WantSendDataSize);

	return WantSendDataSize;
}

void DisconnectSocket(SOCKET DisconnectedSocket, fd_set* Sockets)
{
	SOCKADDR_IN ClosedSockAddr;
	memset(&ClosedSockAddr, 0, sizeof(ClosedSockAddr));
	int ClosedSockAddrLength = sizeof(ClosedSockAddr);

	SOCKET ClosedSocket = DisconnectedSocket;
	getpeername(ClosedSocket, (SOCKADDR*)&ClosedSockAddr, &ClosedSockAddrLength);
	FD_CLR(DisconnectedSocket, &Sockets);
	closesocket(ClosedSocket);
}

void PrintCanvas(map<SOCKET, ClientData>& Clients)
{
	const int Width = 10;
	const int Height = 10;

	int Grid[Width][Height];
	for (int y = 0; y < Height; y++)
		for (int x = 0; x < Width; x++)
			Grid[y][x] = -1;

	// 각 클라이언트 num을 위치에 저장
	for (auto& Client : Clients) {
		if (Client.second.X >= 0 && Client.second.X < Width &&
			Client.second.Y >= 0 && Client.second.Y < Height) {
			Grid[Client.second.Y][Client.second.X] = Client.second.Num;
		}
	}

	system("cls");
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			if (Grid[y][x] == -1)
				cout << ". ";
			else
				cout << Grid[y][x] << " ";
		}
		cout << endl;
	}
}

