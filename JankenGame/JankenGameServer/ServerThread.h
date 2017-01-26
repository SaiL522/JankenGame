/**
@file ServerThread.h
@brief �T�[�o�[�X���b�h
@author Yuuho Aritomi
@date 2017/01/23
*/
#pragma once
#include <Thread.h>
#include <CriticalSection.h>
#include <list>
#include <memory>

//�N���C�A���g�X���b�h
class ClientThread;

/**@brief �T�[�o�[�X���b�h*/
class ServerThread :
	public Thread
{
public:
	/**@brief �R���X�g���N�^*/
	ServerThread(int port);
	/**@brief �f�X�g���N�^*/
	~ServerThread();
	/**@brief �T�[�o�[�X���b�h�̋N��*/
	void run();
	/**@brief �N���C�A���g�̒ǉ�*/
	void addClient(ClientThread* client);
	/**@brief �N���C�A���g�̍폜*/
	void removeClient();
	/**@brief �N���C�A���g�̑S�č폜*/
	void clearClient();
	/**@brief �N���C�A���g�Ƀ��b�Z�[�W�𑗐M*/
	void sendClient(const std::string& message);
	/**@brief �T�[�o�[�X���b�h�̏I��*/
	void end();
	/**@return �|�[�g�ԍ�*/
	int getPort() const { return mPort; }
	/**@return �I�����Ă��邩���ׂ�*/
	bool isEnd() { return mIsEnd; }
	/**@return */
	int getConnectNum() { return mConnectNum; }
private:
	//�N���C�A���g�X���b�h�̃|�C���^�^
	using ClientThreadPtr = std::shared_ptr<ClientThread>;
	//�N���C�A���g�X���b�h�̃R���e�i�^
	using ClientContainer = std::list<ClientThreadPtr>;
	//�|�[�g�ԍ�
	int mPort;
	//�I���t���O
	bool mIsEnd;
	//�ڑ��l��
	ClientContainer::size_type mConnectNum;
	//�N���C�A���g�X���b�h�R���e�i
	ClientContainer mClientContainer;
	//�N���C�A���g�X���b�h�R���e�i���b�N
	CriticalSection mClientContainerLock;
};

