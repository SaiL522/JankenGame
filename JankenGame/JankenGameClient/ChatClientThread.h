/**
@file ChatClientThread.h
@brief �`���b�g�N���C�A���g�X���b�h�N���X
@author Yuuho Aritomi
@date 2017/01/25
*/
#pragma once
#include "MessageBuffer.h"
#include <Thread.h>
#include <CriticalSection.h>
#include <Socket.h>
#include <InetSocketAddress.h>
#include <string>

/**@brief �`���b�g�N���C�A���g�X���b�h�N���X*/
class ChatClientThread :
	public Thread
{
public:
	/**@brief �R���X�g���N�^*/
	ChatClientThread(unsigned int bufferSize);
	/**@brief �f�X�g���N�^*/
	~ChatClientThread();
	/**@brief ���[�U�[���̐ݒ�*/
	void setName(const std::string& name);
	/**@brief �T�[�o�[�\�P�b�g�A�h���X�̐ݒ�*/
	void setServerAddress(const InetSocketAddress& address);
	/**@brief �X���b�h�̎��s*/
	void run();
	/**@brief ���b�Z�[�W�𑗐M*/
	void sendMessage(const std::string& message);
	/**@brief �ڑ������H*/
	void printMessage(std::ostream& out);
	/**@brief �ڑ������H*/
	bool isConnect();
	/**@brief ���݂̐ڑ���*/
	int connectNum();
	/**@brief �����I������*/
	void end();
private:
	//�T�[�o�[�̃\�P�b�g�A�h���X
	InetSocketAddress mServerAddress;
	//���[�U�[��
	std::string mName;
	//�ڑ���
	int mConnectNum;
	//���b�Z�[�W�o�b�t�@
	MessageBuffer mMessageBuffer;
	//�ʐM�p�\�P�b�g
	Socket mSocket;
};

