#include "AcceptThread.h"
#include "ServerThread.h"
#include "ClientThread.h"
#include <Socket.h>
#include <SocketException.h>
#include <InetAddress.h>
#include <iostream>
#include <memory>


//�R���X�g���N�^
AcceptThread::AcceptThread(ServerThread & server) :
	mServer(server),
	mIsEnd(false)
{
}

//�f�X�g���N�^
AcceptThread::~AcceptThread()
{
	end();
}

//�X���b�h�̋N��
void AcceptThread::run()
{
	mIsEnd = false;

	try {
		mServerSocket.bind(mServer.getPort());

		while (isEnd() == false) {
			std::cout << "�N���C�A���g�ڑ��҂�" << std::endl;
			//�N���C�A���g���󂯕t����
			std::unique_ptr<Socket> client(mServerSocket.accept());
			std::cout << client->getAddress().getHostAddress() << "�ڑ����܂���" << std::endl;
			std::string name = client->receive();
			//�T�[�o�[�ɃN���C�A���g��ǉ�����
			mServer.addClient(new ClientThread(mServer, client.release(), name));
			//���̃X���b�h�ɐ����n��
			Thread::sleep(0);
		}
	}
	catch (SocketException&) {

	}
	mServerSocket.close();
	mIsEnd = true;
}

//�����I��
void AcceptThread::end()
{
	mIsEnd = true;
	mServerSocket.close();
	join();
}
