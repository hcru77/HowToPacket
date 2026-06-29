#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>


#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

int main(int argc, char* argv[]) {
	WSADATA wsaData;

	struct addrinfo* result = NULL, * ptr = NULL, hints = {};
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult;

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed with code: " << iResult << std::endl;
		return 1;
	}
	std::cout << "Winsock initialized successfully . " << std::endl;


	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		std::cerr << "getaddrinfo failed with code: " << iResult << std::endl;
		WSACleanup();
		return 1;
	}

	SOCKET ConnectSocket;
	ConnectSocket = INVALID_SOCKET;
	ptr = result;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		std::cerr << "error at socket: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = connect(ConnectSocket, ptr->ai_addr, ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	// Normally should try the next address from getaddrinfo

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		std::cerr << "connection to server failed!";
		WSACleanup();
		return 1;
	}

	WSACleanup();
	std::cout << "winsock cleaned" << std::endl;
	return 0;
}