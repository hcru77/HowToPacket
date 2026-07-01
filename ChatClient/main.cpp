#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>


#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

int main(int argc, char* argv[]) {
	WSADATA wsaData;

	struct addrinfo* result = NULL, * ptr = NULL, hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int recvbuflen = DEFAULT_BUFLEN;
	const char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];

	int iResult;

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed with code: " << iResult << std::endl;
		return 1;
	}
	std::cout << "Winsock initialized successfully . " << std::endl;

	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <Server IP Address>" << std::endl;
		return 1;
	}
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

	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
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

	iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		std::cerr << "send failed: " << WSAGetLastError();
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "bytes sent: " << iResult << std::endl;

	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		std::cerr << "shutdown failed: " << WSAGetLastError();
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			std::cout << "bytes received: " << iResult << std::endl;
		}
		else if(iResult == 0) {
			std::cout << "connection closed";
		}
		else {
			std::cerr << "recv failed: " << WSAGetLastError();
		}

	} while (iResult > 0);

	closesocket(ConnectSocket);
	WSACleanup();
	std::cout << "winsock cleaned" << std::endl;
	return 0;
}