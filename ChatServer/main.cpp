#include <iostream>
#include <WinSock2.h>


int main() {
	
	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0) {
		std::cerr << "WSA failed with code" << result << std::endl;
		return 1;
	}

	std::cout << "WSA started successfully! " << std::endl;

	std::cout << "press enter" << std::endl;
	std::cin.get();


	WSACleanup();
	std::cout << "Winsock cleaned successfully";
	return 0;
}