/**
@file AcceptThread.h
@brief �A�N�Z�v�g�X���b�h�N���X
@author Yuuho Aritomi
@date 2017/01/23
*/
#pragma once
#include <ServerSocket.h>
#include <Thread.h>

//�T�[�o�[�X���b�h�N���X
class ServerThread;

/**@brief �A�N�Z�v�g�X���b�h�N���X*/
class AcceptThread :
	public Thread
{
public:
	/**@brief �R���X�g���N�^*/
	AcceptThread(ServerThread& server);
	/**@brief �f�X�g���N�^*/
	~AcceptThread();
	/**@brief �X���b�h�̎��s*/
	void run();
	/**@brief �����I��*/
	void end();
	/**@brief �I���t���O*/
	bool isEnd() { return mIsEnd; }
private:
	//�T�[�o�[�X���b�h
	ServerThread& mServer;
	//�T�[�o�[�\�P�b�g
	ServerSocket mServerSocket;
	//�I���t���O
	bool mIsEnd;
};

