#include <WinSock2.h>
#include <iostream>
#include "ServerThread.h"

#pragma comment(lib, "WSock32.lib")
#pragma comment(lib, "NetLibrary.lib")
#pragma comment(lib, "ThreadLibrary.lib")

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);

	int port;
	std::cout << "�|�[�g�ԍ������-->";
	std::cin >> port;
	ServerThread server(port);
	server.start();
	while (GetAsyncKeyState(VK_ESCAPE) == 0) {
		Thread::sleep(0);
	}
	//�T�[�o�[�X���b�h�������I��
	server.end();

	WSACleanup();
}