#include <iostream>
#include <WinSock2.h>


int main() {
	WSADATA wsaData;

	// Requesting socket version 2.2 from OS
	int result = WSAStartup(MAKEWORD(2,2), &wsaData);
	
	if (result != 0) {
		std::cerr << "WSAStartup failed with code: " << result << std::endl;
		return 1;
	}

	std::cout << "Winsock initialized successfully . " << std::endl;

	std::cout << "press enter " << std::endl;
	std::cin.get();


	WSACleanup();
	std::cout << "winsock cleaned" << std::endl;
	return 0;
}