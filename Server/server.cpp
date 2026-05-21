#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "NetUtil.h"

#include <winsock2.h>
#include <iostream>
#include <map>
#include <LocRecvPacket.h>
#include <LocSendPacket.h>

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "NetCommon")

using namespace std;

char Buffer[1024] = { 0, };

struct ClientData {
	string ID;
	int Num;
	int X;
	int Y;
};

//blocking, synchrous, multiplexing(polling)
int main()
{
	map<SOCKET, ClientData> Clients;
	int count = 0;
	cout << "server start" << endl;

	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;
	ListenSockAddr.sin_port = htons(35000);

	//already use port 이미 포트 사용중
	::bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, SOMAXCONN);

	//blocking, synchronous(TimeOut)
	TIMEVAL TimeOut;
	TimeOut.tv_sec = 0;
	TimeOut.tv_usec = 500000;

	fd_set ReadSockets;
	fd_set CopyReadSockets;

	FD_ZERO(&ReadSockets);
	FD_SET(ListenSocket, &ReadSockets);

	while (true)
	{
		CopyReadSockets = ReadSockets;

		//0.5초씩 blocking
		int ChangeCount = select(0, &CopyReadSockets, 0, 0, &TimeOut);

		if (ChangeCount <= 0)
		{
			//Server Work
			//0.5초한번 서버 작업을 하는거
			continue;
		}

		//몬가 자료 있다.
		for (int i = 0; i < (int)ReadSockets.fd_count; ++i)
		{
			if (FD_ISSET(ReadSockets.fd_array[i], &CopyReadSockets))
			{
				if (ReadSockets.fd_array[i] == ListenSocket)
				{
					//connect process
					SOCKADDR_IN ClientSockAddr;
					memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
					int ClientSockSockLength = sizeof(ClientSockAddr);

					//blocking, synchronous
					SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockSockLength);

					//예외처리 해야할 수도?
					string IP = inet_ntoa(ClientSockAddr.sin_addr);
					cout << "connect client " << IP << endl;
					//맵에 소켓의 inet_ntoa(ClientSockAddr.sin_addr)값을 ID로 넣고 값에 구조체 pos.x,y를 0,0으로 넣어 + ID까지
					ClientData Data;
					Data.X = 0;
					Data.Y = 0;
					Data.Num = count;
					count++;
					Clients[ClientSocket] = Data;

					FD_SET(ClientSocket, &ReadSockets);
				}
				else
				{
					//Data Receive

					//header
					unsigned short PacketSize = 0;
					int RecvBytes = recv(ReadSockets.fd_array[i], (char*)&PacketSize, sizeof(PacketSize), MSG_WAITALL);
					if (RecvBytes <= 0)
					{
						cout << "header recv fail " << endl;
						DisconnectSocket(ReadSockets.fd_array[i], &ReadSockets);
						continue;
					}

					PacketSize = ntohs(PacketSize);

					memset(Buffer, 0, sizeof(Buffer));
					//data JSON Buffer
					RecvBytes = recv(ReadSockets.fd_array[i], Buffer, PacketSize, MSG_WAITALL);
					if (RecvBytes <= 0)
					{
						cout << "data recv fail " << endl;
						DisconnectSocket(ReadSockets.fd_array[i], &ReadSockets);
						continue;
					}
					else
					{
						//Buffer 파싱
						LocRecvPacket Data;
						Data.Parse(Buffer);

						SOCKADDR_IN ClientSockAddr;
						memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
						int ClientSockAddrLength = sizeof(ClientSockAddr);

						getpeername(ReadSockets.fd_array[i], (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);
						//맵에 소켓의 inet_ntoa(ClientSockAddr.sin_addr)값으로 찾아서 계산하고? 저장한 다음?
						string IP = inet_ntoa(ClientSockAddr.sin_addr);
						cout << "client(" << IP<< " / " << Data.UserID << Data.Num;
						Clients[ReadSockets.fd_array[i]].ID = Data.UserID;
						for (auto& Client : Clients)
						{
							if (Client.first == ReadSockets.fd_array[i])
							{
								//Client.second.ID = Data.UserID;
								char input = Data.Input;
								switch (input) {
								case 'w': // 위
								case 'W':
									Client.second.Y -= 1;
									break;
								case 'a': // 왼쪽
								case 'A':
									Client.second.X -= 1;
									break;
								case 's': // 아래
								case 'S':
									Client.second.Y += 1;
									break;
								case 'd': // 오른쪽
								case 'D':
									Client.second.X += 1;
									break;
								}
								//Data.ID = IP;
								//Data.X = Client.second.X;
								//Data.Y = Client.second.Y;
								
							}
							else {
								// 이미 있는 클라이언트
							}
						}
						//Clients에 저장한 값을 출력
						cout << ") " << Clients[ReadSockets.fd_array[i]].X << "," << Clients[ReadSockets.fd_array[i]].Y << " Move" << endl;
						
						//모든 접속한 유저한테 전달
						for (auto& Client : Clients) {
							// 각 클라이언트 위치 JSON 만들기
							LocSendPacket SendData;
							SendData.UserID = Client.second.ID;
							SendData.X = Client.second.X;
							SendData.Y = Client.second.Y;
							SendData.Num = Client.second.Num;
							string JSONString = SendData.ToString();

							for (int j = 0; j < (int)ReadSockets.fd_count; ++j) {
								if (ReadSockets.fd_array[j] != ListenSocket) {
									PacketSize = (unsigned short)JSONString.length();
									PacketSize = htons(PacketSize);

									// header
									int SentBytes = SendAll(ReadSockets.fd_array[j], (char*)&PacketSize, 2);
									if (SentBytes <= 0) {
										cout << "header send fail." << endl;
										DisconnectSocket(ReadSockets.fd_array[j], &ReadSockets);
									}
									// Data
									SentBytes = SendAll(ReadSockets.fd_array[j], JSONString.c_str(), ntohs(PacketSize));
									if (SentBytes <= 0) {
										cout << "Data send fail." << endl;
										DisconnectSocket(ReadSockets.fd_array[j], &ReadSockets);
									}
								}
							}
						}
					}
				}
			}
		}
	}






	closesocket(ListenSocket);
	WSACleanup();

	return 0;
}