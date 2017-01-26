#include "ClientThread.h"
#include "ServerThread.h"
#include <Socket.h>

//�R���X�g���N�^
ClientThread::ClientThread(ServerThread & server, Socket * socket, const std::string & name) :
	mServer(server),
	mSocket(socket),
	mName(name),
	mIsEnd(false)
{
}

//�f�X�g���N�^
ClientThread::~ClientThread()
{
	end();
}

//�X���b�h�̋N��
void ClientThread::run()
{
	int connectNum;
	mIsEnd = false;
	try {
		//�N���C�A���g����̃��b�Z�[�W����������
		while (mIsEnd == false) {
			//�N���C�A���g����̃f�[�^����M����
			std::string message = mSocket->receive();
			//�N���C�A���g�̐ڑ��v�����`�F�b�N
			if (message == "-2") {
				break;
			}
			//���[�U�[����t�����āA�N���C�A���g�Ƀ��b�Z�[�W�𑗐M
			mServer.sendClient(
				std::to_string(mServer.getConnectNum()) + " " + mName  + " " + message);
			//���̃X���b�h�ɐ����n��
			Thread::sleep(0);
		}
	} 
	catch (SocketException&) {

	}
	mIsEnd = true;
}

void ClientThread::send(const std::string & message)
{
	mSocket->send(message);
}

//�����I��
void ClientThread::end()
{
	mIsEnd = true;
	mSocket->close();
	join();
}