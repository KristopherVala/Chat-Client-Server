#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
using namespace std;
void main() {

	//starts Winsock DLLs
	WSADATA wsaData;
	if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		std::cout << "Winsock error" << std::endl;

		return;
	}

	//initializes socket. SOCK_STREAM: TCP
	SOCKET ClientSocket;
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		WSACleanup();
		std::cout << "Socket init. error" << std::endl;

		return;
	}

	//Connect socket to specified server
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;						//Address family type itnernet
	SvrAddr.sin_port = htons(9000);					//port (host to network conversion)
	//SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//IP address
	SvrAddr.sin_addr.s_addr = inet_addr("64.228.89.186");	//IP address
	if ((connect(ClientSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
		closesocket(ClientSocket);
		WSACleanup();
		std::cout << "Socket connect error" << std::endl;

		return;
	}
	//receives Rxbuffer

	while (1) {
		//sends Txbuffer		
		char TxBuffer[128] = {};
		cout << "Enter a String to transmit" << endl;
		cin >> TxBuffer;

		//Check for quit
		if (TxBuffer[0] == 'q' && TxBuffer[1] == 'u' && TxBuffer[2] == 'i' && TxBuffer[3] == 't') {
			send(ClientSocket, TxBuffer, sizeof(TxBuffer), 0);
			break;
		}
		send(ClientSocket, TxBuffer, sizeof(TxBuffer), 0);
		char Ack[128];
		recv(ClientSocket, Ack, sizeof(Ack), 0); //MESSAGE RECIEVED HERE
		cout << Ack << endl;

	}

	//closes connection and socket
	closesocket(ClientSocket);
	//frees Winsock DLL resources
	WSACleanup();





}