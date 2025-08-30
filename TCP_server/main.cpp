#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h> // Windows Sockets 2 TCP/IP Extensions

#pragma comment(lib, "Ws2_32.lib")

int main() {
	// WSA it is Windows Sockets API
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0) {
		std::cout << "WSAStartup failed with error: " << wsaerr << std::endl;
		return -1;
	} else {
		std::cout << "WSAStartup succeeded" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
	}


	// Create a socket with TCP protocol (Transmission Control Protocol)
	SOCKET serverSocket;
	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // af_inet = address family internet. Members of AF_INET address family are IPv4 addresses.

	if (serverSocket == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "Socket is OK!" << std::endl;
	}


	// Bind the socket to an IP address and port number
	sockaddr_in service;
	IN_ADDR addr;
	service.sin_family = AF_INET; // sin = short int
	inet_pton(AF_INET, "192.168.0.105", &service.sin_addr);
	service.sin_port = htons(52052); // 49152–65535 - private or dynamic ports

	if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "bind() is OK!" << std::endl;
	}


	// Listen for incoming connections
	if (listen(serverSocket, 1) == SOCKET_ERROR) {
		std::cout << "listen(): Error listening on socket: " << WSAGetLastError() << std::endl;
	}
	else {
		std::cout << "listen() is OK! I'm waiting for new connections..." << std::endl;
	}



	// Accept incoming connections
	SOCKET acceptSocket;
	acceptSocket = accept(serverSocket, nullptr, nullptr);

	if (acceptSocket == INVALID_SOCKET) {
		std::cout << "accept failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}
	else {
		std::cout << "accept() is OK!" << std::endl;
	}


	// Receive data from the client
	char receiveBuffer[200];
	int rbyteCount = recv(acceptSocket, receiveBuffer, 200, 0);
	if (rbyteCount < 0) {
		std::cout << "Server recv error: " << WSAGetLastError() << std::endl;
		return 0;
	}
	else {
		std::cout << "Received data: " << receiveBuffer << std::endl;
	}

	char buffer[200];
	std::cout << "Enter the message: ";
	std::cin.getline(buffer, 200);
	int sbyteCount = send(acceptSocket, buffer, 200, 0);
	if (sbyteCount == SOCKET_ERROR) {
		std::cout << "Server send error: " << WSAGetLastError() << std::endl;
		return -1;
	}
	else {
		std::cout << "Server: Sent " << sbyteCount << " bytes" << std::endl;
	}


	return 0;
}