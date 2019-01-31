#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>

void main() {
	bool loop = true;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //START WINDOWS SOCKET
		return;

	SOCKET servSock;
	servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (servSock== INVALID_SOCKET) {
		WSACleanup();
		std::cout << "Bad Socket connect" << std::endl;
		return;
	}

	//binds socket to address
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY; //RECIEVES ANY INCOMING ADDRESS - THIS DIFFERS FROM THE CLIENT THAT CONNECTS TO A SPECIFIC IP - IN THIS CASE THE LOCAL HOST
	SvrAddr.sin_port = htons(9000);
	if (bind(servSock, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
	{
		closesocket(servSock);
		WSACleanup();
		return;
	}

	// THIS SHOULD BE SUFFICIENT TO START LISTENING
	SOCKET clientSock = SOCKET_ERROR;

	do {
		if (listen(servSock, 1) == SOCKET_ERROR) {
			closesocket(servSock);
			WSACleanup();
			return;
		}
		std::cout << "Waiting for client connection" << std::endl;
		int addsize = sizeof(SvrAddr);
	//		if ((clientSock = accept(servSock, NULL, NULL)) == SOCKET_ERROR) {
			if ((clientSock = accept(servSock, (SOCKADDR*)&SvrAddr, &addsize)) == SOCKET_ERROR) {

			closesocket(servSock);
			WSACleanup();
			return;
	}
		std::cout << "Conection established" << std::endl;
		while (1) { //loops until closed
			char recBuffer[128] = {};

			int n = recv(clientSock, recBuffer, sizeof(recBuffer), 0); // Message is recieved here, int n is the size of the recieved bytes of the packet
			std::cout << "Message recieved: " << recBuffer << std::endl;
			send(clientSock, "Message Recieved", 17, 0); // 17 is the size of the ACK message
			//Check for quit message
			if (recBuffer[0] == 'q' && recBuffer[1] == 'u' && recBuffer[2] == 'i' && recBuffer[3] == 't') {
				std::cout << "Connection Terminated" << std::endl;
				break;
			}
			std::cout <<"Message: " << recBuffer << std::endl;
		}
	} while (loop == true);
	closesocket(clientSock);
	WSACleanup();
	std::cout << "Closing" << std::endl;	
	
	

}