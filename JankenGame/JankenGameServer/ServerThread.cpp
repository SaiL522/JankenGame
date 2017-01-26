#include "ServerThread.h"
#include "ClientThread.h"
#include "AcceptThread.h"
#include <Socket.h>
#include <functional>
#include <algorithm>

//�R���X�g���N�^
ServerThread::ServerThread(int port) :
	mPort(port),
	mIsEnd(false),
	mConnectNum(0)
{
}

//�f�X�g���N�^
ServerThread::~ServerThread()
{
	//�����I��
	end();
}

void ServerThread::run()
{
	//�X���b�h���J�n
	mIsEnd = false;
	//�A�N�Z�v�g�X���b�h���쐬����
	std::unique_ptr<AcceptThread> acceptor(new AcceptThread(*this));
	//��M�X���b�h���N��
	acceptor->start();
	//�I������܂ŌJ��Ԃ�
	while (isEnd() == false)
	{
		//�ڑ����𒲂ׂ�
		mConnectNum = mClientContainer.size();
		//�I�������N���C�A���g���폜
		removeClient();
		//���̃X���b�h�ɐ����n��
		Thread::sleep(0);
	}
	//�A�N�Z�v�g�X���b�h��S�č폜
	clearClient();
	//�I���t���O
	mIsEnd = true;
}

//�N���C�A���g�̒ǉ�
void ServerThread::addClient(ClientThread * client)
{
	CriticalSection::Lock guard(mClientContainerLock);
	mClientContainer.push_back(ClientThreadPtr(client));

	client->start();
}

//�N���C�A���g�̍폜
void ServerThread::removeClient()
{
	CriticalSection::Lock guard(mClientContainerLock);
	mClientContainer.remove_if(std::mem_fn(&ClientThread::isEnd));
	//mConnectNum--;
}

//�N���C�A���g�����ׂč폜
void ServerThread::clearClient()
{
	CriticalSection::Lock guard(mClientContainerLock);
	mClientContainer.clear();
	mConnectNum = 0;
}

void ServerThread::sendClient(const std::string & message)
{
	CriticalSection::Lock guard(mClientContainerLock);

	//�S�N���C�A���g�Ƀ��b�Z�[�W�𑗂�
	std::for_each(
		mClientContainer.begin(),
		mClientContainer.end(),
		std::bind(std::mem_fn(&ClientThread::send), std::placeholders::_1, message)
	);
}

//�����I��
void ServerThread::end()
{
	mIsEnd = true;
	join();
}
