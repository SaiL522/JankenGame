/**
@file ClientThread.h
@brief �N���C�A���g�X���b�h�N���X
@author Yuuho Aritomi
@date 2017/01/23
*/
#pragma once
#include <Thread.h>
#include <string>
#include <memory>

//�T�[�o�[�X���b�h�N���X
class ServerThread;

//�\�P�b�g�N���X
class Socket;

/**@brief �N���C�A���g�X���b�h�N���X*/
class ClientThread :
	public Thread
{
public:
	/**@brief �R���X�g���N�^*/
	ClientThread(ServerThread& server, Socket* socket, const std::string& name);
	/**@brief �f�X�g���N�^*/
	~ClientThread();
	/**@brief �X���b�h�̋N��*/
	void run();
	/**@brief ���b�Z�[�W�𑗐M*/
	void send(const std::string& message);
	/**@brief �����I��*/
	void end();
	/**@brief �X���b�h���I�����Ă��邩�H*/
	bool isEnd() { return mIsEnd; }
private:
	//�T�[�o�[�X���b�h
	ServerThread& mServer;
	//�N���C�A���g�p�̑���M�\�P�b�g
	std::unique_ptr<Socket> mSocket;
	//���[�U�[��
	std::string mName;
	//�I���t���O
	bool mIsEnd;
};

