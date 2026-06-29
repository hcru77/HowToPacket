#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512


int main() {
	WSADATA wsaData;

	int iResult; 
	int iSendResult;

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSA failed with code" << iResult << std::endl;
		WSACleanup();
		return 1;
	}

	std::cout << "WSA started successfully! " << std::endl;


	// ptr is temp for iterating IP addresses
	struct addrinfo* result = NULL, *ptr = NULL, hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	// hints is struct to tell OS what kind of socket you want

	// Resolving the local address and port to be used by server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		std::cerr << "getaddrinfo failed with code: " << iResult << std::endl;
		WSACleanup();
		return 1;
	}

	SOCKET ListenSocket;
	ListenSocket = INVALID_SOCKET;

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		std::cerr << "Error at socket(): " << WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult != 0) {
		std::cerr << "binding failed with code: " << iResult << std::endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result); // address information is no longer needed after the bind

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "listen failed with error: " << SOCKET_ERROR << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;

	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		std::cerr << "accept failed with error: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ListenSocket);

	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			std::cout << "bytes received: " << iResult << std::endl;

			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				std::cerr << "send failed: " << WSAGetLastError() << std::endl;
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			std::cout << "bytes sent: " << iSendResult << std::endl;
		}
		else if (iResult == 0) {
			std::cout << "connection closing... " << std::endl;
		}
		else {
			std::cerr << "recv failed" << WSAGetLastError() << std::endl;
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0);

	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		std::cerr << "shutdown failed" << WSAGetLastError() << std::endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}


	//std::cout << "press enter" << std::endl;
	//std::cin.get();

	closesocket(ClientSocket);
	WSACleanup();
	std::cout << "Winsock cleaned successfully";
	return 0;
}