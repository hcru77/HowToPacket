#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define DEFAULT_PORT "27015"


int main() {
	WSADATA wsaData;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		std::cerr << "WSA failed with code" << iResult << std::endl;
		WSACleanup();
		return 1;
	}

	std::cout << "WSA started successfully! " << std::endl;


	// ptr is temp for iterating IP addresses
	struct addrinfo* result = NULL, * ptr = NULL, hints = {};
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

	SOCKET listenSocket = INVALID_SOCKET;
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (listenSocket == INVALID_SOCKET) {
		std::cerr << "Error at socket(): " << WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	std::cout << "press enter" << std::endl;
	std::cin.get();


	WSACleanup();
	std::cout << "Winsock cleaned successfully";
	return 0;
}